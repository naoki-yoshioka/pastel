#ifndef PASTEL_NEIGHBOR_NEIGHBOR_LIST_HPP
# define PASTEL_NEIGHBOR_NEIGHBOR_LIST_HPP

# include <cassert>
# include <cstddef>
# include <vector>
# include <iterator>
# include <algorithm>
# include <functional>
# include <utility>
# include <type_traits>
# include <initializer_list>
# include <memory>

# include <boost/iterator/iterator_facade.hpp>

# include <pastel/neighbor/neighbor.hpp>
# include <pastel/neighbor/inexclusive_cells_updater.hpp>
# include <pastel/neighbor/meta/is_partner_data_accessible.hpp>
# include <pastel/force/meta/value_of.hpp>
# include <pastel/utility/pair.hpp>
# include <pastel/utility/is_pair.hpp>
# include <pastel/utility/is_input_iterator.hpp>
# include <pastel/utility/is_const_iterator.hpp>
# include <pastel/utility/is_nothrow_swappable.hpp>


namespace pastel
{
  namespace neighbor
  {
    template <
      typename Force,
      typename InteractionPair = ::pastel::utility::size_pair<0u, 0u>,
      typename Updater = ::pastel::neighbor::inexclusive_cells_updater<typename ::pastel::force::meta::value_of<Force>::type>,
      typename IndexAllocator = std::allocator<std::size_t>>
    class neighbor_list;

    namespace neighbor_list_detail
    {
      template <typename Force, typename InteractionPair, typename Updater, typename IndexAllocator>
      class const_iterator final
        : public boost::iterators::iterator_facade<
            const_iterator<Force, InteractionPair, Updater, IndexAllocator>,
            typename ::pastel::neighbor::neighbor_list<Force, InteractionPair, Updater, IndexAllocator>::value_type const,
            std::bidirectional_iterator_tag,
            typename ::pastel::neighbor::neighbor_list<Force, InteractionPair, Updater, IndexAllocator>::value_type const>
      {
       public:
        using neighbor_list_type = ::pastel::neighbor::neighbor_list<Force, InteractionPair, Updater, IndexAllocator>;
        using neighbor_type = typename neighbor_list_type::value_type;
        using index_type = typename neighbor_list_type::size_type;
        using partner_const_pointer = typename neighbor_list_type::partner_const_pointer;
       private:
        neighbor_list_type const* neighbor_list_ptr_;
        index_type key_;
        partner_const_pointer partner_ptr_;
        index_type num_keys_;

        friend class boost::iterators::iterator_core_access;

       public:
        constexpr const_iterator() noexcept
          : neighbor_list_ptr_{nullptr}, key_{}, partner_ptr_{}, num_keys_{}
        { }

        constexpr const_iterator(neighbor_list_type const& neighbor_list, index_type key, partner_const_pointer partner_ptr)
          : neighbor_list_ptr_{std::addressof(neighbor_list)}, key_{key}, partner_ptr_{partner_ptr}, num_keys_{neighbor_list.num_keys()}
        {
          while (key_ < num_keys_ && partner_ptr_ == neighbor_list_ptr_->partner_data(key_+index_type{1}))
            ++key_;
        }

        neighbor_type dereference() const { return {key_, *partner_ptr_}; }
        bool equal(const_iterator const& other) const
        {
# ifndef NDEBUG
          return
            neighbor_list_ptr_ == other.neighbor_list_ptr_
            && key_ == other.key_ && partner_ptr_ == other.partner_ptr_
            && num_keys_ == other.num_keys_;
# else
          return key_ == other.key_ && partner_ptr_ == other.partner_ptr_;
# endif
        }

        const_iterator& increment()
        {
          ++partner_ptr_;
          while (key_ < num_keys_ && partner_ptr_ == neighbor_list_ptr_->partner_data(key_+index_type{1}))
            ++key_;
          return *this;
        }
        const_iterator& decrement()
        {
          while (key_ > index_type{0} && partner_ptr_ == neighbor_list_ptr_->partner_data(key_))
            --key_;
          --partner_ptr_;
          return *this;
        }

        void swap(const_iterator& other) noexcept
        {
          using std::swap;
          swap(neighbor_list_ptr_, other.neighbor_list_ptr_);
          swap(key_, other.key_);
          swap(partner_ptr_, other.partner_ptr_);
          swap(num_keys_, other.num_keys_);
        }

        index_type const& key() const { return key_; }
        partner_const_pointer const& partner_ptr() const { return partner_ptr_; }
      }; // class const_iterator<Force, InteractionPair, Updater, IndexAllocator>

      template <typename Force, typename InteractionPair, typename Updater, typename IndexAllocator>
      inline void swap(
        ::pastel::neighbor::neighbor_list_detail::const_iterator<Force, InteractionPair, Updater, IndexAllocator>& lhs,
        ::pastel::neighbor::neighbor_list_detail::const_iterator<Force, InteractionPair, Updater, IndexAllocator>& rhs)
        noexcept
      { lhs.swap(rhs); }


      /*
      template <bool is_intraparticle_>
      struct assertion;

      template <>
      struct assertion<true>
      {
        static void call(std::size_t key, std::size_t partner)
        { assert(key < partner); }
      }; // struct assertion<true>

      template <>
      struct assertion<false>
      {
        static void call(std::size_t key, std::size_t partner) { }
      }; // struct assertion<false>
      */


      template <bool is_const_iterator, typename KeyCompare, typename KeyEqual, typename Size>
      struct assign_from_random_access_iterators
      {
        template <typename RandomAccessIterator, typename PartnerFirstIndices, typename Partners>
        static void call(RandomAccessIterator first, RandomAccessIterator last, PartnerFirstIndices& partner_first_indices, Partners& partners)
        {
          auto temp = std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type>{first, last};
          ::pastel::neighbor::neighbor_list_detail::assign_from_random_access_iterators<false, KeyCompare, KeyEqual, Size>::call(temp.data(), temp.data() + temp.size(), partner_first_indices, partners);
        }
      }; // struct assign_from_random_access_iterators<is_const_iterator, KeyCompare, KeyEqual>

      template <typename KeyCompare, typename KeyEqual, typename Size>
      struct assign_from_random_access_iterators<false, KeyCompare, KeyEqual, Size>
      {
        template <typename RandomAccessIterator, typename PartnerFirstIndices, typename Partners>
        static void call(RandomAccessIterator first, RandomAccessIterator last, PartnerFirstIndices& partner_first_indices, Partners& partners)
        {
          if (first == last)
            return;

          partner_first_indices.clear();
          partners.clear();

          std::sort(first, last, KeyCompare());
          last = std::unique(first, last, KeyEqual());

          auto const first_key = first->key();
          auto const last_key = std::prev(last)->key() + Size{1};
          auto const num_neighbors = std::distance(first, last);

          // Need a sentinel
          partner_first_indices.reserve(last_key+1);
          partners.reserve(num_neighbors);

          // If first_key > 0, partner_first_indices_[0<=key<first_key] = 0.
          partner_first_indices.assign(first_key, Size{0});
          for (auto key = first_key; key < last_key; ++key)
          {
            partner_first_indices.push_back(partners.size());
            while (first != last && first->key() == key)
            {
              partners.push_back(first->partner());
              ++first;
            }
          }
          // Push back a sentinel
          partner_first_indices.push_back(partners.size());
        }
      }; // struct assign_from_random_access_iterators<false, KeyCompare, KeyEqual>
    } // namespace neighbor_list_detail


    template <typename Force, typename InteractionPair, typename Updater, typename IndexAllocator>
    class neighbor_list final
    {
      static_assert(::pastel::utility::is_pair<InteractionPair>::value, "InteractionPair must be pastel::utility::pair<T, n, m>");

      using indices_type = std::vector<std::size_t, IndexAllocator>;

      // partner_first_indices_.size() must be greater than or equal to 1 because it has a sentinel.
      indices_type partner_first_indices_;
      indices_type partners_;

      Force force_;

      Updater updater_;

     public:
      //static constexpr bool is_intraparticle = InteractionPair::first == InteractionPair::second;
      static constexpr bool is_partner_data_accessible = true;
      using interaction_pair = InteractionPair;

      using index_allocator_type = IndexAllocator;
      using size_type = typename indices_type::size_type;
      using difference_type = typename indices_type::difference_type;
      //using key_type = ::pastel::neighbor::neighbor<is_intraparticle>;
      //using value_type = ::pastel::neighbor::neighbor<is_intraparticle>;
      using key_type = ::pastel::neighbor::neighbor;
      using value_type = ::pastel::neighbor::neighbor;
      using key_compare = std::less<key_type>;
      using value_compare = std::less<value_type>;
      using key_equal = std::equal_to<key_type>;
      using iterator = ::pastel::neighbor::neighbor_list_detail::const_iterator<Force, InteractionPair, Updater, IndexAllocator>;
      using const_iterator = iterator;
      using reverse_iterator = std::reverse_iterator<iterator>;
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;

      using partner_const_iterator = typename indices_type::const_iterator;
      using partner_const_pointer = typename indices_type::const_pointer;
      using force_type = Force;
      using updater_type = Updater;

     private:
      using interaction_pair_key_type = typename interaction_pair::type;

     public:
      template <interaction_pair_key_type boundary_partner>
      using boundary_neighbor_list_type
        = ::pastel::neighbor::neighbor_list<
            Force,
            ::pastel::utility::pair<interaction_pair_key_type, interaction_pair::first, boundary_partner>,
            Updater, IndexAllocator>;

      ~neighbor_list()
        noexcept(
          std::is_nothrow_destructible<indices_type>::value
          && std::is_nothrow_destructible<force_type>::value
          && std::is_nothrow_destructible<updater_type>::value)
        = default;
      neighbor_list(neighbor_list const&) = default;
      neighbor_list(neighbor_list&&)
        noexcept(
          std::is_nothrow_move_constructible<indices_type>::value
          && std::is_nothrow_move_constructible<force_type>::value
          && std::is_nothrow_move_constructible<updater_type>::value)
        = default;
      neighbor_list& operator=(neighbor_list const&) = default;
      neighbor_list& operator=(neighbor_list&&)
        noexcept(
          std::is_nothrow_move_assignable<indices_type>::value
          && std::is_nothrow_move_assignable<force_type>::value
          && std::is_nothrow_move_assignable<updater_type>::value)
        = default;

      neighbor_list() : neighbor_list{IndexAllocator{}} { }

      explicit neighbor_list(IndexAllocator const& allocator)
        : neighbor_list{Force{}, Updater{}, allocator}
      { }

      explicit neighbor_list(Force const& force)
        : neighbor_list{force, IndexAllocator{}}
      { }

      neighbor_list(Force const& force, IndexAllocator const& allocator)
        : neighbor_list{force, Updater{}, allocator}
      { }

      explicit neighbor_list(Updater const& updater)
        : neighbor_list{updater, IndexAllocator{}}
      { }

      neighbor_list(Updater const& updater, IndexAllocator const& allocator)
        : neighbor_list{Force{}, updater, allocator}
      { }

      explicit neighbor_list(Force&& force)
        noexcept(noexcept(neighbor_list{std::move(force), IndexAllocator{}}))
        : neighbor_list{std::move(force), IndexAllocator{}}
      { }

      neighbor_list(Force&& force, IndexAllocator const& allocator)
        noexcept(noexcept(neighbor_list{std::move(force), Updater{}, allocator}))
        : neighbor_list{std::move(force), Updater{}, allocator}
      { }

      explicit neighbor_list(Updater&& updater)
        noexcept(noexcept(neighbor_list{std::move(updater), IndexAllocator{}}))
        : neighbor_list{std::move(updater), IndexAllocator{}}
      { }

      neighbor_list(Updater&& updater, IndexAllocator const& allocator)
        noexcept(noexcept(neighbor_list{Force{}, std::move(updater), allocator}))
        : neighbor_list{Force{}, std::move(updater), allocator}
      { }

      neighbor_list(
        Force const& force, Updater const& updater,
        IndexAllocator const& allocator = IndexAllocator{})
        : partner_first_indices_(size_type{1}, size_type{0}, allocator),
          partners_{allocator},
          force_{force},
          updater_{updater}
      { }

      neighbor_list(
        Force const& force, Updater&& updater,
        IndexAllocator const& allocator = IndexAllocator{})
        noexcept(
          noexcept(indices_type{allocator})
          && std::is_nothrow_copy_constructible<force_type>::value
          && std::is_nothrow_move_constructible<updater_type>::value)
        : partner_first_indices_(size_type{1}, size_type{0}, allocator),
          partners_{allocator},
          force_{force},
          updater_{std::move(updater)}
      { }

      neighbor_list(
        Force&& force, Updater const& updater,
        IndexAllocator const& allocator = IndexAllocator{})
        noexcept(
          noexcept(indices_type{allocator})
          && std::is_nothrow_move_constructible<force_type>::value
          && std::is_nothrow_copy_constructible<updater_type>::value)
        : partner_first_indices_(size_type{1}, size_type{0}, allocator),
          partners_{allocator},
          force_{std::move(force)},
          updater_{updater}
      { }

      neighbor_list(
        Force&& force, Updater&& updater,
        IndexAllocator const& allocator = IndexAllocator{})
        noexcept(
          noexcept(indices_type{allocator})
          && std::is_nothrow_move_constructible<force_type>::value
          && std::is_nothrow_move_constructible<updater_type>::value)
        : partner_first_indices_(size_type{1}, size_type{0}, allocator),
          partners_{allocator},
          force_{std::move(force)},
          updater_{std::move(updater)}
      { }

      template <typename Iterator, typename = typename std::enable_if< ::pastel::utility::is_input_iterator<Iterator>::value >::type>
      neighbor_list(Iterator first, Iterator last, IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{first, last, Force{}, Updater{}, allocator}
      { }

      template <typename Iterator, typename = typename std::enable_if< ::pastel::utility::is_input_iterator<Iterator>::value >::type>
      neighbor_list(
        Iterator first, Iterator last, Force const& force,
        IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{first, last, force, Updater{}, allocator}
      { }

      template <typename Iterator, typename = typename std::enable_if< ::pastel::utility::is_input_iterator<Iterator>::value >::type>
      neighbor_list(
        Iterator first, Iterator last, Updater const& updater,
        IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{first, last, Force{}, updater, allocator}
      { }

      template <typename Iterator, typename = typename std::enable_if< ::pastel::utility::is_input_iterator<Iterator>::value >::type>
      neighbor_list(
        Iterator first, Iterator last, Force&& force,
        IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{first, last, std::move(force), Updater{}, allocator}
      { }

      template <typename Iterator, typename = typename std::enable_if< ::pastel::utility::is_input_iterator<Iterator>::value >::type>
      neighbor_list(
        Iterator first, Iterator last, Updater&& updater,
        IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{first, last, Force{}, std::move(updater), allocator}
      { }

      template <typename Iterator, typename = typename std::enable_if< ::pastel::utility::is_input_iterator<Iterator>::value >::type>
      neighbor_list(
        Iterator first, Iterator last,
        Force const& force, Updater const& updater, IndexAllocator const& allocator = IndexAllocator{})
        : partner_first_indices_{size_type{1}, size_type{0}, allocator},
          partners_{allocator},
          force_{force},
          updater_{updater}
      {
        static_assert(
          std::is_same<typename std::iterator_traits<Iterator>::value_type, value_type>::value,
          "Value type of Iterator must be value_type");

        assign_from_iterators(first, last, typename std::iterator_traits<Iterator>::iterator_category{});
      }

      template <typename Iterator, typename = typename std::enable_if< ::pastel::utility::is_input_iterator<Iterator>::value >::type>
      neighbor_list(
        Iterator first, Iterator last,
        Force const& force, Updater&& updater, IndexAllocator const& allocator = IndexAllocator{})
        : partner_first_indices_{size_type{1}, size_type{0}, allocator},
          partners_{allocator},
          force_{force},
          updater_{std::move(updater)}
      {
        static_assert(
          std::is_same<typename std::iterator_traits<Iterator>::value_type, value_type>::value,
          "Value type of Iterator must be value_type");

        assign_from_iterators(first, last, typename std::iterator_traits<Iterator>::iterator_category{});
      }

      template <typename Iterator, typename = typename std::enable_if< ::pastel::utility::is_input_iterator<Iterator>::value >::type>
      neighbor_list(
        Iterator first, Iterator last,
        Force&& force, Updater const& updater, IndexAllocator const& allocator = IndexAllocator{})
        : partner_first_indices_{size_type{1}, size_type{0}, allocator},
          partners_{allocator},
          force_{std::move(force)},
          updater_{updater}
      {
        static_assert(
          std::is_same<typename std::iterator_traits<Iterator>::value_type, value_type>::value,
          "Value type of Iterator must be value_type");

        assign_from_iterators(first, last, typename std::iterator_traits<Iterator>::iterator_category{});
      }

      template <typename Iterator, typename = typename std::enable_if< ::pastel::utility::is_input_iterator<Iterator>::value >::type>
      neighbor_list(
        Iterator first, Iterator last,
        Force&& force, Updater&& updater, IndexAllocator const& allocator = IndexAllocator{})
        : partner_first_indices_{size_type{1}, size_type{0}, allocator},
          partners_{allocator},
          force_{std::move(force)},
          updater_{std::move(updater)}
      {
        static_assert(
          std::is_same<typename std::iterator_traits<Iterator>::value_type, value_type>::value,
          "Value type of Iterator must be value_type");

        assign_from_iterators(first, last, typename std::iterator_traits<Iterator>::iterator_category{});
      }

      neighbor_list(neighbor_list const& other, IndexAllocator const& allocator)
        : partner_first_indices_{other.partner_first_indices_, allocator},
          partners_{other.partners_, allocator},
          force_{other.force_},
          updater_{other.updater_}
      { }

      neighbor_list(neighbor_list&& other, IndexAllocator const& allocator)
        : partner_first_indices_{std::move(other.partner_first_indices_), allocator},
          partners_{std::move(other.partners_), allocator},
          force_{std::move(other.force_)},
          updater_{std::move(other.updater_)}
      { }

      neighbor_list(std::initializer_list<value_type> initializer_list, IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{std::begin(initializer_list), std::end(initializer_list), allocator}
      { }

      neighbor_list(
        std::initializer_list<value_type> initializer_list, Force const& force,
        IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{std::begin(initializer_list), std::end(initializer_list), force, allocator}
      { }

      neighbor_list(
        std::initializer_list<value_type> initializer_list, Updater const& updater,
        IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{std::begin(initializer_list), std::end(initializer_list), updater, allocator}
      { }

      neighbor_list(
        std::initializer_list<value_type> initializer_list, Force&& force,
        IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{std::begin(initializer_list), std::end(initializer_list), std::move(force), allocator}
      { }

      neighbor_list(
        std::initializer_list<value_type> initializer_list, Updater&& updater,
        IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{std::begin(initializer_list), std::end(initializer_list), std::move(updater), allocator}
      { }

      template <typename Force_, typename Updater_>
      neighbor_list(
        std::initializer_list<value_type> initializer_list,
        Force_&& force, Updater_&& updater, IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{std::begin(initializer_list), std::end(initializer_list), std::forward<Force_>(force), std::forward<Updater_>(updater), allocator}
      { }

      neighbor_list& operator=(std::initializer_list<value_type> initializer_list)
      { assign(initializer_list); return *this; }


      template <typename Particles>
      explicit neighbor_list(Particles const& particles)
        : neighbor_list{particles, Force{}, Updater{}}
      { }

      template <typename Particles>
      neighbor_list(Particles const& particles, IndexAllocator const& allocator)
        : neighbor_list{particles, Force{}, Updater{}, allocator}
      { }

      template <typename Particles>
      neighbor_list(Particles const& particles, Force const& force, IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{particles, force, Updater{}, allocator}
      { }

      template <typename Particles>
      neighbor_list(Particles const& particles, Updater const& updater, IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{particles, Force{}, updater, allocator}
      { }

      template <typename Particles>
      neighbor_list(Particles const& particles, Force&& force, IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{particles, std::move(force), Updater{}, allocator}
      { }

      template <typename Particles>
      neighbor_list(Particles const& particles, Updater&& updater, IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{particles, Force{}, std::move(updater), allocator}
      { }

      template <typename Particles>
      neighbor_list(Particles const& particles, Force const& force, Updater const& updater, IndexAllocator const& allocator = IndexAllocator{})
        : partner_first_indices_{size_type{1}, size_type{0}, allocator},
          partners_{allocator},
          force_{force},
          updater_{updater}
      { updater_.update_neighbor_list(*this, particles); }

      template <typename Particles>
      neighbor_list(Particles const& particles, Force const& force, Updater&& updater, IndexAllocator const& allocator = IndexAllocator{})
        : partner_first_indices_{size_type{1}, size_type{0}, allocator},
          partners_{allocator},
          force_{force},
          updater_{std::move(updater)}
      { updater_.update_neighbor_list(*this, particles); }

      template <typename Particles>
      neighbor_list(Particles const& particles, Force&& force, Updater const& updater, IndexAllocator const& allocator = IndexAllocator{})
        : partner_first_indices_{size_type{1}, size_type{0}, allocator},
          partners_{allocator},
          force_{std::move(force)},
          updater_{updater}
      { updater_.update_neighbor_list(*this, particles); }

      template <typename Particles>
      neighbor_list(Particles const& particles, Force&& force, Updater&& updater, IndexAllocator const& allocator = IndexAllocator{})
        : partner_first_indices_{size_type{1}, size_type{0}, allocator},
          partners_{allocator},
          force_{std::move(force)},
          updater_{std::move(updater)}
      { updater_.update_neighbor_list(*this, particles); }


      template <
        typename KeyParticles, typename PartnerParticles,
        typename = typename std::enable_if<!::pastel::utility::is_input_iterator<KeyParticles>::value>::type>
      neighbor_list(
        KeyParticles const& key_particles, PartnerParticles const& partner_particles,
        IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{key_particles, partner_particles, Force{}, Updater{}, allocator}
      { }

      template <
        typename KeyParticles, typename PartnerParticles,
        typename = typename std::enable_if<!::pastel::utility::is_input_iterator<KeyParticles>::value>::type>
      neighbor_list(
        KeyParticles const& key_particles, PartnerParticles const& partner_particles,
        Force const& force, IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{key_particles, partner_particles, force, Updater{}, allocator}
      { }

      template <
        typename KeyParticles, typename PartnerParticles,
        typename = typename std::enable_if<!::pastel::utility::is_input_iterator<KeyParticles>::value>::type>
      neighbor_list(
        KeyParticles const& key_particles, PartnerParticles const& partner_particles,
        Updater const& updater, IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{key_particles, partner_particles, Force{}, updater, allocator}
      { }

      template <
        typename KeyParticles, typename PartnerParticles,
        typename = typename std::enable_if<!::pastel::utility::is_input_iterator<KeyParticles>::value>::type>
      neighbor_list(
        KeyParticles const& key_particles, PartnerParticles const& partner_particles,
        Force&& force, IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{key_particles, partner_particles, std::move(force), Updater{}, allocator}
      { }

      template <
        typename KeyParticles, typename PartnerParticles,
        typename = typename std::enable_if<!::pastel::utility::is_input_iterator<KeyParticles>::value>::type>
      neighbor_list(
        KeyParticles const& key_particles, PartnerParticles const& partner_particles,
        Updater&& updater, IndexAllocator const& allocator = IndexAllocator{})
        : neighbor_list{key_particles, partner_particles, Force{}, std::move(updater), allocator}
      { }

      template <
        typename KeyParticles, typename PartnerParticles,
        typename = typename std::enable_if<!::pastel::utility::is_input_iterator<KeyParticles>::value>::type>
      neighbor_list(
        KeyParticles const& key_particles, PartnerParticles const& partner_particles,
        Force const& force, Updater const& updater, IndexAllocator const& allocator = IndexAllocator{})
        : partner_first_indices_{size_type{1}, size_type{0}, allocator},
          partners_{allocator},
          force_{force},
          updater_{updater}
      { updater_.update_neighbor_list(*this, key_particles, partner_particles); }

      template <
        typename KeyParticles, typename PartnerParticles,
        typename = typename std::enable_if<!::pastel::utility::is_input_iterator<KeyParticles>::value>::type>
      neighbor_list(
        KeyParticles const& key_particles, PartnerParticles const& partner_particles,
        Force const& force, Updater&& updater, IndexAllocator const& allocator = IndexAllocator{})
        : partner_first_indices_{size_type{1}, size_type{0}, allocator},
          partners_{allocator},
          force_{force},
          updater_{std::move(updater)}
      { updater_.update_neighbor_list(*this, key_particles, partner_particles); }

      template <
        typename KeyParticles, typename PartnerParticles,
        typename = typename std::enable_if<!::pastel::utility::is_input_iterator<KeyParticles>::value>::type>
      neighbor_list(
        KeyParticles const& key_particles, PartnerParticles const& partner_particles,
        Force&& force, Updater const& updater, IndexAllocator const& allocator = IndexAllocator{})
        : partner_first_indices_{size_type{1}, size_type{0}, allocator},
          partners_{allocator},
          force_{std::move(force)},
          updater_{updater}
      { updater_.update_neighbor_list(*this, key_particles, partner_particles); }

      template <
        typename KeyParticles, typename PartnerParticles,
        typename = typename std::enable_if<!::pastel::utility::is_input_iterator<KeyParticles>::value>::type>
      neighbor_list(
        KeyParticles const& key_particles, PartnerParticles const& partner_particles,
        Force&& force, Updater&& updater, IndexAllocator const& allocator = IndexAllocator{})
        : partner_first_indices_{size_type{1}, size_type{0}, allocator},
          partners_{allocator},
          force_{std::move(force)},
          updater_{std::move(updater)}
      { updater_.update_neighbor_list(*this, key_particles, partner_particles); }


     private:
      template <typename Iterator>
      void assign_from_iterators(Iterator first, Iterator last, std::input_iterator_tag)
      {
        auto temp = std::vector<typename std::iterator_traits<Iterator>::value_type>{first, last};
        ::pastel::neighbor::neighbor_list_detail::assign_from_random_access_iterators<false, key_compare, key_equal, size_type>::call(temp.data(), temp.data() + temp.size(), partner_first_indices_, partners_);
      }

      template <typename RandomAccessIterator>
      void assign_from_iterators(
        RandomAccessIterator first, RandomAccessIterator last,
        std::random_access_iterator_tag)
      { ::pastel::neighbor::neighbor_list_detail::assign_from_random_access_iterators< ::pastel::utility::is_const_iterator<RandomAccessIterator>::value, key_compare, key_equal, size_type >::call(first, last, partner_first_indices_, partners_); }

      /*
      template <typename RandomAccessIterator, typename = typename std::enable_if< ::pastel::utility::is_const_iterator<RandomAccessIterator>::value >::type>
      void assign_from_random_access_iterators(RandomAccessIterator first, RandomAccessIterator last)
      {
        auto temp = std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type>{first, last};
        assign_from_random_access_iterators(temp.data(), temp.data() + temp.size());
      }

      template <typename RandomAccessIterator, typename = typename std::enable_if<!::pastel::utility::is_const_iterator<RandomAccessIterator>::value>::type>
      void assign_from_random_access_iterators(RandomAccessIterator first, RandomAccessIterator last)
      {
        if (first == last)
          return;

        partner_first_indices_.clear();
        partners_.clear();

        std::sort(first, last, key_compare());
        last = std::unique(first, last, key_equal());

        auto const first_key = first->key();
        auto const last_key = std::prev(last)->key() + size_type{1};
        auto const num_neighbors = std::distance(first, last);

        // Need a sentinel
        partner_first_indices_.reserve(last_key+1);
        partners_.reserve(num_neighbors);

        // If first_key > 0, partner_first_indices_[0<=key<first_key] = 0.
        partner_first_indices_.assign(first_key, size_type{0});
        for (auto key = first_key; key < last_key; ++key)
        {
          partner_first_indices_.push_back(partners_.size());
          while (first != last && first->key() == key)
          {
            partners_.push_back(first->partner());
            ++first;
          }
        }
        // Push back a sentinel
        partner_first_indices_.push_back(partners_.size());
      }
      */

     public:
      template <typename Iterator>
      void assign(Iterator first, Iterator last)
      {
        static_assert(
          std::is_same<typename std::iterator_traits<Iterator>::value_type, value_type>::value,
          "Value type of Iterator must be value_type");

        assign_from_iterators(first, last, typename std::iterator_traits<Iterator>::iterator_category());
      }

      void assign(std::initializer_list<value_type> initializer_list)
      { assign(std::begin(initializer_list), std::end(initializer_list)); }

      index_allocator_type get_index_allocator() const { return partners_.get_allocator(); }

      // Iterators
      iterator begin() noexcept { auto const first_key = size_type{0}; return {*this, first_key, this->partner_data(first_key)}; }
      const_iterator begin() const noexcept { auto const first_key = size_type{0}; return {*this, first_key, this->partner_data(first_key)}; }
      const_iterator cbegin() const noexcept { auto const first_key = size_type{0}; return {*this, first_key, this->partner_data(first_key)}; }
      iterator end() noexcept { auto const last_key = this->num_keys(); return {*this, last_key, this->partner_data(last_key)}; }
      const_iterator end() const noexcept { auto const last_key = this->num_keys(); return {*this, last_key, this->partner_data(last_key)}; }
      const_iterator cend() const noexcept { auto const last_key = this->num_keys(); return {*this, last_key, this->partner_data(last_key)}; }
      reverse_iterator rbegin() noexcept { return reverse_iterator{end()}; }
      const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator{end()}; }
      const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator{cend()}; }
      reverse_iterator rend() noexcept { return reverse_iterator{begin()}; }
      const_reverse_iterator rend() const noexcept { return const_reverse_iterator{begin()}; }
      const_reverse_iterator crend() const noexcept { return const_reverse_iterator{cbegin()}; }

      // Capacity
      bool empty() const noexcept { return partners_.empty(); }
      size_type size() const noexcept { return partners_.size(); }
      size_type max_size() const noexcept { return std::vector<value_type>{}.max_size(); }
      void reserve(size_type new_capacity)
      {
        // Need a sentinel
        partner_first_indices_.reserve(new_capacity+size_type{1});
        partners_.reserve(new_capacity);
      }
      size_type capacity() const noexcept { return partners_.capacity(); }
      void shrink_to_fit()
      {
        partner_first_indices_.shrink_to_fit();
        partners_.shrink_to_fit();
      }

      // Modifiers
      void clear() noexcept
      {
        partner_first_indices_.clear();
        partners_.clear();
        // Need a sentinel
        partner_first_indices_.reserve(size_type{1});
        partner_first_indices_.push_back(partners_.size());
      }

     private:
      iterator do_insert(partner_const_pointer partner_ptr, size_type new_key, size_type new_partner)
      {
        assert(new_key < this->num_keys());

        auto const partner_iter
          = partners_.insert(std::begin(partners_) + (partner_ptr - partners_.data()), new_partner);

        auto const partner_first_indices_size = partner_first_indices_.size();
        auto partner_first_indices_data = partner_first_indices_.data();
        for (auto key = new_key+size_type{1}; key < partner_first_indices_size; ++key)
          ++partner_first_indices_data[key];

        return {*this, new_key, partners_.data() + (partner_iter - std::begin(partners_))};
      }

      iterator do_insert_last(size_type num_keys, size_type new_key, size_type new_partner)
      {
        assert(new_key >= num_keys);

        partners_.push_back(new_partner);

        partner_first_indices_.resize(
          new_key + size_type{2}, // num_keys becomes new_key+1 after resizing, and size of partner_first_indices_ must be num_keys+1 because it has a sentinel
          partner_first_indices_.data()[num_keys]); // partner_first_indices_[num_keys] was the original sentinel
        ++partner_first_indices_.data()[new_key + size_type{1}]; // increment the present sentinel

        return {*this, new_key, partners_.data() + (partners_.size() - size_type{1})};
      }

     public:
      std::pair<iterator, bool> insert(value_type const& new_neighbor)
      { return emplace(new_neighbor.key(), new_neighbor.partner()); }

      iterator insert(const_iterator hint, value_type const& new_neighbor)
      { return emplace_hint(hint, new_neighbor.key(), new_neighbor.partner()); }

      template <typename Iterator>
      void insert(Iterator first, Iterator last)
      {
        static_assert(
          std::is_same<typename std::iterator_traits<Iterator>::value_type, value_type>::value,
          "Value type of Iterator must be value_type");

        for (; first != last; ++first)
          this->insert(this->end(), *first);
      }

      void insert(std::initializer_list<value_type> initializer_list)
      { insert(std::begin(initializer_list), std::end(initializer_list)); }

      std::pair<iterator, bool> emplace(size_type new_key, size_type new_partner)
      {
        //::pastel::neighbor::neighbor_list_detail::assertion<is_intraparticle>::call(new_key, new_partner);

        auto const num_keys = this->num_keys();
        if (new_key >= num_keys)
          return {do_insert_last(num_keys, new_key, new_partner), true};

        auto const partner_last_ptr = this->partner_data(new_key+size_type{1});
        auto const partner_ptr = std::lower_bound(this->partner_data(new_key), partner_last_ptr, new_partner);
        if (partner_ptr != partner_last_ptr && *partner_ptr == new_partner)
          return {{*this, new_key, partner_ptr}, false};

        return {do_insert(partner_ptr, new_key, new_partner), true};
      }

      iterator emplace_hint(const_iterator hint, size_type new_key, size_type new_partner)
      {
        //::pastel::neighbor::neighbor_list_detail::assertion<is_intraparticle>::call(new_key, new_partner);

        auto const hint_partner_ptr = hint.partner_ptr();

        if (hint != this->cend())
        {
          auto const hint_key = hint.key();
          auto const hint_partner = *hint_partner_ptr;
          if (new_key > hint_key || (new_key == hint_key && new_partner >= hint_partner))
            return this->emplace(new_key, new_partner).first;
        }

        if (hint != this->cbegin())
        {
          --hint;
          auto const prev_hint_key = hint.key();
          auto const prev_hint_partner = *(hint.partner_ptr());
          if (new_key < prev_hint_key || (new_key == prev_hint_key && new_partner <= prev_hint_partner))
            return this->emplace(new_key, new_partner).first;
        }

        auto const num_keys = this->num_keys();
        if (new_key >= num_keys)
          return do_insert_last(num_keys, new_key, new_partner);

        return do_insert(hint_partner_ptr, new_key, new_partner);
      }

      void unsafe_push_back(value_type const& new_neighbor)
      { unsafe_emplace_back(new_neighbor.key(), new_neighbor.partner()); }

      void unsafe_emplace_back(size_type new_key, size_type new_partner)
      {
        //::pastel::neighbor::neighbor_list_detail::assertion<is_intraparticle>::call(new_key, new_partner);

        auto const num_keys = this->num_keys();
        if (new_key >= num_keys)
          do_insert_last(num_keys, new_key, new_partner);
        else
          do_insert(this->partner_data(num_keys), new_key, new_partner);
      }

     private:
      partner_const_pointer do_erase(size_type pos_key, partner_const_pointer pos_partner_ptr)
      {
        auto const partner_iter
          = partners_.erase(std::begin(partners_) + (pos_partner_ptr - partners_.data()));

        auto const partner_first_indices_size = partner_first_indices_.size();
        auto partner_first_indices_data = partner_first_indices_.data();
        for (auto key = pos_key+size_type{1}; key < partner_first_indices_size; ++key)
          --partner_first_indices_data[key];

        // Clean tails up
        // since C++14: use std::rbegin and std::rend
        partner_first_indices_.erase(
          std::adjacent_find(
            partner_first_indices_.rbegin(), partner_first_indices_.rend(),
            std::not_equal_to<size_type>()).base(),
          partner_first_indices_.end());

        return partners_.data() + (partner_iter - std::begin(partners_));
      }

     public:
      iterator erase(const_iterator pos)
      { auto const pos_key = pos.key(); return {*this, pos_key, this->do_erase(pos_key, pos.partner_ptr())}; }

      // TODO: implement range erase
      // iterator erase(const_iterator first, const_iterator last)

      size_type erase(value_type const& neighbor)
      {
        auto const key = neighbor.key();
        auto const partner_ptr = this->find_partner_ptr(key, neighbor.partner());
        if (partner_ptr == this->partner_data(key+size_type{1}))
          return size_type{0};

        this->do_erase(key, partner_ptr);
        return size_type{1};
      }

      void swap(neighbor_list& other)
        noexcept(
          ::pastel::utility::is_nothrow_swappable<indices_type>::value
          && ::pastel::utility::is_nothrow_swappable<force_type>::value)
      {
        using std::swap;
        swap(partner_first_indices_, other.partner_first_indices_);
        swap(partners_, other.partners_);
        swap(force_, other.force_);
      }

      // Lookup
     private:
      partner_const_pointer find_partner_ptr(size_type key, size_type partner) const
      { return std::find(this->partner_data(key), this->partner_data(key+size_type{1}), partner); }

     public:
      size_type count(value_type const& neighbor) const
      {
        auto const key = neighbor.key();
        if (key >= this->num_keys())
          return 0u;

        return std::count(
          this->partner_data(key), this->partner_data(key+size_type{1}),
          neighbor.partner());
      }

      const_iterator find(value_type const& neighbor) const
      {
        auto const key = neighbor.key();
        if (key >= this->num_keys())
          return this->end();

        auto const partner_ptr = this->find_partner_ptr(key, neighbor.partner());
        if (partner_ptr == this->partner_data(key+size_type{1}))
          return this->end();
        return {*this, key, partner_ptr};
      }

      const_iterator lower_bound(value_type const& neighbor) const
      {
        auto const key = neighbor.key();
        if (key >= this->num_keys())
          return this->end();

        return {
          *this, key,
          std::lower_bound(
            this->partner_data(key), this->partner_data(key+size_type{1}),
            neighbor.partner())};
      }

      const_iterator upper_bound(value_type const& neighbor) const
      {
        auto const key = neighbor.key();
        if (key >= this->num_keys())
          return this->end();

        return {
          *this, key,
          std::upper_bound(
            this->partner_data(key), this->partner_data(key+size_type{1}),
            neighbor.partner())};
      }

      std::pair<const_iterator, const_iterator> equal_range(value_type const& neighbor) const
      {
        auto const key = neighbor.key();
        if (key >= this->num_keys())
          return std::make_pair(this->end(), this->end());

        auto const ptr_pair
          = std::equal_range(
              this->partner_data(key), this->partner_data(key+size_type{1}),
              neighbor.partner());
        return {{*this, key, ptr_pair.first}, {*this, key, ptr_pair.second}};
      }

      // Observers
      key_compare key_comp() const { return key_compare(); }
      value_compare value_comp() const { return value_compare(); }
      key_equal key_eq() const { return key_equal(); }


      bool operator==(neighbor_list const& other) const
      {
        return
          partner_first_indices_ == other.partner_first_indices_
          && partners_ == other.partners_
          && force_ == other.force_;
      }

      bool operator<(neighbor_list const& other) const
      {
        return
          partner_first_indices_ < other.partner_first_indices_
          && partners_ < other.partners_
          && force_ < other.force_;
      }


      size_type num_keys() const { return partner_first_indices_.size() - size_type{1}; }

      size_type num_partners(size_type key) const
      { return partner_first_indices_.data()[key+1] - partner_first_indices_.data()[key]; }

      partner_const_iterator partner_begin(size_type key) const
      {
        assert(key < partner_first_indices_.size());
        return std::begin(partners_) + partner_first_indices_.data()[key];
      }

      partner_const_iterator partner_end(size_type key) const
      {
        assert(key < num_keys());
        return std::begin(partners_) + partner_first_indices_.data()[key+1];
      }

      partner_const_pointer partner_data(size_type key) const
      {
        assert(key < partner_first_indices_.size());
        return partners_.data() + partner_first_indices_.data()[key];
      }

      force_type& force() { return force_; }
      force_type const& force() const { return force_; }
      template <typename Force_>
      void force(Force_&& new_force) { force_ = std::forward<Force_>(new_force); }

      updater_type& updater() { return updater_; }
      updater_type const& updater() const { return updater_; }
      template <typename Updater_>
      void updater(Updater_&& new_updater) { updater_ = std::forward<Updater_>(new_updater); }
    }; // class neighbor_list<Force, InteractionPair, Updater, IndexAllocator>

    template <typename Force, typename InteractionPair, typename Updater, typename IndexAllocator>
    inline bool operator!=(
      ::pastel::neighbor::neighbor_list<Force, InteractionPair, Updater, IndexAllocator> const& lhs,
      ::pastel::neighbor::neighbor_list<Force, InteractionPair, Updater, IndexAllocator> const& rhs)
    { return !(lhs == rhs); }

    template <typename Force, typename InteractionPair, typename Updater, typename IndexAllocator>
    inline bool operator>(
      ::pastel::neighbor::neighbor_list<Force, InteractionPair, Updater, IndexAllocator> const& lhs,
      ::pastel::neighbor::neighbor_list<Force, InteractionPair, Updater, IndexAllocator> const& rhs)
    { return rhs < lhs; }

    template <typename Force, typename InteractionPair, typename Updater, typename IndexAllocator>
    inline bool operator<=(
      ::pastel::neighbor::neighbor_list<Force, InteractionPair, Updater, IndexAllocator> const& lhs,
      ::pastel::neighbor::neighbor_list<Force, InteractionPair, Updater, IndexAllocator> const& rhs)
    { return !(lhs > rhs); }

    template <typename Force, typename InteractionPair, typename Updater, typename IndexAllocator>
    inline bool operator>=(
      ::pastel::neighbor::neighbor_list<Force, InteractionPair, Updater, IndexAllocator> const& lhs,
      ::pastel::neighbor::neighbor_list<Force, InteractionPair, Updater, IndexAllocator> const& rhs)
    { return !(lhs < rhs); }

    template <typename Force, typename InteractionPair, typename Updater, typename IndexAllocator>
    inline void swap(
      ::pastel::neighbor::neighbor_list<Force, InteractionPair, Updater, IndexAllocator>& lhs,
      ::pastel::neighbor::neighbor_list<Force, InteractionPair, Updater, IndexAllocator>& rhs)
      noexcept(noexcept(lhs.swap(rhs)))
    { lhs.swap(rhs); }
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_NEIGHBOR_LIST_HPP

