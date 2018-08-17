#ifndef PASTEL_NEIGHBOR_INEXCLUSIVE_CELLS_UPDATER_HPP
# define PASTEL_NEIGHBOR_INEXCLUSIVE_CELLS_UPDATER_HPP

# include <cassert>
# include <cstddef>
# include <array>
# include <vector>
# include <iterator>
# include <algorithm>
# ifndef NDEBUG
#  include <functional>
# endif
# include <memory>

# include <pastel/neighbor/force.hpp>
# include <pastel/neighbor/meta/force_of.hpp>
# include <pastel/neighbor/meta/interaction_pair_of.hpp>
# include <pastel/geometry/squared_distance.hpp>
# include <pastel/particle/tags.hpp>
# include <pastel/container/get.hpp>
# include <pastel/container/num_particles.hpp>
# include <pastel/container/maximal_speed.hpp>
# include <pastel/container/meta/is_data_accessible.hpp>
# include <pastel/system/particles.hpp>
# include <pastel/force/cutoff_length.hpp>
# include <pastel/force/meta/has_cutoff.hpp>
# include <pastel/utility/prod.hpp>


namespace pastel
{
  namespace neighbor
  {
    namespace inexclusive_cells_updater_detail
    {
      template <typename NeighborList, typename Particles, typename Value>
      inline Value update_status(NeighborList const& neighbor_list, Particles const& particles, Value time_step, Value buffer_length)
      { return buffer_length - Value{2} * time_step * ::pastel::container::maximal_speed(particles); }

      template <typename NeighborList, typename KeyParticles, typename PartnerParticles, typename Value>
      inline Value update_status(NeighborList const& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles, Value time_step, Value buffer_length)
      {
        return buffer_length - Value{2} * time_step * std::max(
          ::pastel::container::maximal_speed(key_particles),
          ::pastel::container::maximal_speed(partner_particles));
      }


      template <typename Vector, std::size_t dimension>
      inline Vector generate_cell_vector(Vector system_vector, std::array<std::size_t, dimension> const& num_cells)
      {
        static_assert(::pastel::geometry::meta::dimension_of<Vector>::value == dimension, "dimension_of<Vector> must be equal to dimension");

        for (auto dim = std::size_t{0u}; dim < dimension_; ++dim)
          system_vector[dim] /= num_cells[dim];

        return system_vector;
      }


      template <bool is_data_accessible>
      struct generate_cell_index;

      template <>
      struct generate_cell_index<true>
      {
        template <typename Particles, typename Index, typename Point, typename Vector, std::size_t dimension>
        static std::size_t call(
          Particles const& particles, Index const index,
          Point const& lower_bound, Vector const& cell_vector,
          std::array<std::size_t, dimension> const& num_cells)
        {
          static_assert(::pastel::geometry::meta::dimension_of<Vector>::value == dimension, "dimension_of<Vector> must be equal to dimension");

          auto const position_data = particles.template data< ::pastel::particle::tags::position >();
          auto const positoin_vector_from_lower_bound = position_data[index] - lower_bound;

          auto result = std::size_t{0u};
          for (auto dim = std::size_t{0u}; dim < dimension; ++dim)
          {
            auto cell_coordinate
              = static_cast<std::size_t>(std::floor(position_vector_from_lower_bound[dim] / cell_vector[dim]));
            for (auto dim_ = std::size_t{0u}; dim_ < dim; ++dim_)
              cell_coordinate *= num_cells[dim_];
            result += cell_coordinate;
          }
        }
      }; // struct generate_cell_index<true>

      template <>
      struct generate_cell_index<false>
      {
        template <typename Particles, typename Index, typename Point, typename Vector, std::size_t dimension>
        static std::size_t call(
          Particles const& particles, Index const index,
          Point const& lower_bound, Vector const& cell_vector,
          std::array<std::size_t, dimension> const& num_cells)
        {
          static_assert(::pastel::geometry::meta::dimension_of<Vector>::value == dimension, "dimension_of<Vector> must be equal to dimension");

          auto const positoin_vector_from_lower_bound
            = ::psatel::container::get< ::pastel::particle::tags::position >(particles, index) - lower_bound;

          auto result = std::size_t{0u};
          for (auto dim = std::size_t{0u}; dim < dimension; ++dim)
          {
            auto cell_coordinate
              = static_cast<std::size_t>(std::floor(position_vector_from_lower_bound[dim] / cell_vector[dim]));
            for (auto dim_ = std::size_t{0u}; dim_ < dim; ++dim_)
              cell_coordinate *= num_cells[dim_];
            result += cell_coordinate;
          }
        }
      }; // struct generate_cell_index<false>


      inline ... generate_neighbor_cell_indices()
      {
      }


      template <
        typename Index, typename IndexAllocator, typename Iterator, typename IteratorAllocator,
        typename Point, typename Vector, std::size_t dimension_, typename Particles>
      inline void update_cell_data(
        std::vector<Index, IndexAllocator>& particle_indices_in_each_cell,
        std::vector<Iterator, IteratorAllocator>& particle_indices_in_each_cell_firsts,
        std::vector<Index, IndexAllocator>& particle_index_to_cell_index,
        std::vector<Index, IndexAllocator>& num_particles_in_each_cell,
        Point const& lower_bound, Point const& upper_bound, Vector const& cell_vector,
        std::array<std::size_t, dimension_> const& num_cells,
        std::size_t const total_num_cells,
        Particles const& particles)
      {
        assert(lower_bound < upper_bound);

        auto const num_particles = ::pastel::container::num_particles(particles);
        assert(particle_indices_in_each_cell.size() == num_particles);
        assert(particle_indices_in_each_cell_firsts.size() == total_num_cells+1);
        assert(particle_index_to_cell_index.size() == num_particles);
        assert(num_particles_in_each_cell.size() == total_num_cells);
        assert(
          std::accumulate(
            std::begin(num_cells), std::end(num_cells), 1u, std::multiplies<std::size_t>{})
          == total_num_cells);

        std::fill(std::begin(num_particles_in_each_cell), std::end(num_particles_in_each_cell), Index{0});

        static constexpr bool is_data_accessible
          = ::pastel::container::meta::is_data_accessible<Particles>::value;
        for (auto index = Index{0}; index < num_particles; ++index)
        {
          auto const cell_index
            = ::pastel::neighbor::inexclusive_cells_updater_detail::generate_cell_index<is_data_accessible>::call(
                particles, index, lower_bound, cell_vector, num_cells);
          particle_index_to_cell_index[index] = cell_index;
          ++num_particles_in_each_cell[cell_index];
        }

        particle_indices_in_each_cell_firsts.front() = std::begin(particle_indices_in_each_cell);
        for (auto cell_index = Index{0}; cell_index < total_num_cells; ++cell_index)
          particle_indices_in_each_cell_firsts[cell_index+Index{1}]
            = particle_indices_in_each_cell_firsts[cell_index] + num_particles_in_each_cell[cell_index];
        assert(particle_indices_in_each_cell_firsts.back() == std::end(particle_indices_in_each_cell));

        auto particle_indices_in_each_cell_iter = particle_indices_in_each_cell_firsts;
        for (auto index = Index{0}; index < num_particles; ++index)
          *particle_indices_in_each_cell_iter[particle_index_to_cell_index[index]]++ = index;
      }

      template <
        typename Index, typename IndexAllocator, typename Iterator, typename IteratorAllocator,
        typename Point, typename Vector, std::size_t dimension_, typename Particles1, typename Particles2>
      inline void update_cell_data(
        std::vector<Index, IndexAllocator>& particle_indices_in_each_cell,
        std::vector<Iterator, IteratorAllocator>& particle_indices_in_each_cell_firsts,
        std::vector<Index, IndexAllocator>& particle_index_to_cell_index,
        std::vector<Index, IndexAllocator>& num_particles_in_each_cell,
        Point const& lower_bound, Point const& upper_bound, Vector const& cell_vector,
        std::array<std::size_t, dimension_> const& num_cells,
        std::size_t const total_num_cells,
        Particles1 const& particles1, Particles2 const& particles2)
      {
        assert(lower_bound < upper_bound);

        auto const num_particles1 = ::pastel::container::num_particles(particles1);
        auto const num_particles2 = ::pastel::container::num_particles(particles2);
        auto const total_num_particles = num_particles1 + num_particles2;
        assert(particle_indices_in_each_cell.size() == total_num_particles);
        assert(particle_indices_in_each_cell_firsts.size() == total_num_cells+1);
        assert(particle_index_to_cell_index.size() == total_num_particles);
        assert(num_particles_in_each_cell.size() == total_num_cells);
        assert(
          std::accumulate(
            std::begin(num_cells), std::end(num_cells), 1u, std::multiplies<std::size_t>{})
          == total_num_cells);

        std::fill(std::begin(num_particles_in_each_cell), std::end(num_particles_in_each_cell), Index{0});

        static constexpr bool is_data_accessible1
          = ::pastel::container::meta::is_data_accessible<Particles1>::value;
        for (auto index = Index{0}; index < num_particles1; ++index)
        {
          auto const cell_index
            = ::pastel::neighbor::inexclusive_cells_updater_detail::generate_cell_index<is_data_accessible1>::call(
                particles1, index, lower_bound, cell_vector, num_cells);
          particle_index_to_cell_index[index] = cell_index;
          ++num_particles_in_each_cell[cell_index];
        }
        static constexpr bool is_data_accessible2
          = ::pastel::container::meta::is_data_accessible<Particles2>::value;
        for (auto index = Index{num_particles1}; index < total_num_particles; ++index)
        {
          auto const cell_index
            = ::pastel::neighbor::inexclusive_cells_updater_detail::generate_cell_index<is_data_accessible2>::call(
                particles2, index-num_particles1, lower_bound, cell_vector, num_cells);
          particle_index_to_cell_index[index] = cell_index;
          ++num_particles_in_each_cell[cell_index];
        }

        particle_indices_in_each_cell_firsts.front() = std::begin(particle_indices_in_each_cell);
        for (auto cell_index = Index{0}; cell_index < total_num_cells; ++cell_index)
          particle_indices_in_each_cell_firsts[cell_index+Index{1}]
            = particle_indices_in_each_cell_firsts[cell_index] + num_particles_in_each_cell[cell_index];
        assert(particle_indices_in_each_cell_firsts.back() == std::end(particle_indices_in_each_cell));

        auto particle_indices_in_each_cell_iter = particle_indices_in_each_cell_firsts;
        for (auto index = Index{0}; index < total_num_particles; ++index)
          *particle_indices_in_each_cell_iter[particle_index_to_cell_index[index]]++ = index;
      }


      template <bool is_data_accessible>
      struct update_neighbor_list1;

      template <>
      struct update_neighbor_list1<true>
      {
        template <
          typename NeighborList, typename Particles, typename Value,
          typename Index, typename IndexAllocator, typename Iterator, typename IteratorAllocator>
        static void call(
          NeighborList& neighbor_list, Particles const& particles, Value const& squared_search_length,
          std::vector<Index, IndexAllocator>& particle_indices_in_each_cell,
          std::vector<Iterator, IteratorAllocator>& particle_indices_in_each_cell_firsts)
        {
          neighbor_list.clear();

          auto const positions_data = particles.template data< ::pastel::particle::tags::position >();

          for (auto cell_index = Index{0}; cell_index < total_num_cells; ++cell_index)
          {
            Iterator const last = particle_indices_in_each_cell_firsts[cell_index+1];
            for (Iterator iter = particle_indices_in_each_cell_firsts[cell_index]; iter != last; ++iter)
            {
              auto const key = *iter;
            }
          }

          auto const num_particles = ::pastel::container::num_particles(particles);
          using size_type = decltype(num_particles);
          for (auto key = size_type{0}; key < num_particles; ++key)
            for (auto partner = key+size_type{1}; partner < num_particles; ++partner)
              if (::pastel::geometry::squared_distance(positions_data[key], positions_data[partner]) < squared_search_length)
                neighbor_list.unsafe_emplace_back(key, partner);
        }
      }; // struct update_neighbor_list1<true>

      template <>
      struct update_neighbor_list1<false>
      {
        template <typename NeighborList, typename Particles, typename Value>
        static void call(NeighborList& neighbor_list, Particles const& particles, Value const& squared_search_length)
        {
          neighbor_list.clear();

          auto const num_particles = ::pastel::container::num_particles(particles);
          using size_type = decltype(num_particles);
          for (auto key = size_type{0}; key < num_particles; ++key)
            for (auto partner = key+size_type{1}; partner < num_particles; ++partner)
              if (::pastel::geometry::squared_distance(
                    ::pastel::container::get< ::pastel::particle::tags::position >(particles, key),
                    ::pastel::container::get< ::pastel::particle::tags::position >(particles, partner))
                  < squared_search_length)
                neighbor_list.unsafe_emplace_back(key, partner);
        }
      }; // struct update_neighbor_list1<false>


      template <bool is_key_data_accessible, bool is_partner_data_accessible>
      struct update_neighbor_list2;

      template <>
      struct update_neighbor_list2<true, true>
      {
        template <typename NeighborList, typename KeyParticles, typename PartnerParticles, typename Value>
        static void call(NeighborList& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles, Value const& squared_search_length)
        {
          neighbor_list.clear();

          auto const key_positions_data = key_particles.template data< ::pastel::particle::tags::position >();
          auto const partner_positions_data = partner_particles.template data< ::pastel::particle::tags::position >();

          auto const num_key_particles = ::pastel::container::num_particles(key_particles);
          auto const num_partner_particles = ::pastel::container::num_particles(partner_particles);
          using size_type = decltype(num_key_particles);
          for (auto key = size_type{0}; key < num_key_particles; ++key)
            for (auto partner = size_type{0}; partner < num_partner_particles; ++partner)
              if (::pastel::geometry::squared_distance(key_positions_data[key], partner_positions_data[partner]) < squared_search_length)
                neighbor_list.unsafe_emplace_back(key, partner);
        }
      }; // struct update_neighbor_list2<true, true>

      template <>
      struct update_neighbor_list2<true, false>
      {
        template <typename NeighborList, typename KeyParticles, typename PartnerParticles, typename Value>
        static void call(NeighborList& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles, Value const& squared_search_length)
        {
          neighbor_list.clear();

          auto const key_positions_data = key_particles.template data< ::pastel::particle::tags::position >();

          auto const num_key_particles = ::pastel::container::num_particles(key_particles);
          auto const num_partner_particles = ::pastel::container::num_particles(partner_particles);
          using size_type = decltype(num_key_particles);
          for (auto key = size_type{0}; key < num_key_particles; ++key)
            for (auto partner = size_type{0}; partner < num_partner_particles; ++partner)
              if (::pastel::geometry::squared_distance(
                    key_positions_data[key],
                    ::pastel::container::get< ::pastel::particle::tags::position >(partner_particles, partner))
                  < squared_search_length)
                neighbor_list.unsafe_emplace_back(key, partner);
        }
      }; // struct update_neighbor_list2<true, false>

      template <>
      struct update_neighbor_list2<false, true>
      {
        template <typename NeighborList, typename KeyParticles, typename PartnerParticles, typename Value>
        static void call(NeighborList& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles, Value const& squared_search_length)
        {
          neighbor_list.clear();

          auto const partner_positions_data = partner_particles.template data< ::pastel::particle::tags::position >();

          auto const num_key_particles = ::pastel::container::num_particles(key_particles);
          auto const num_partner_particles = ::pastel::container::num_particles(partner_particles);
          using size_type = decltype(num_key_particles);
          for (auto key = size_type{0}; key < num_key_particles; ++key)
            for (auto partner = size_type{0}; partner < num_partner_particles; ++partner)
              if (::pastel::geometry::squared_distance(
                    ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key),
                    partner_positions_data[partner])
                  < squared_search_length)
                neighbor_list.unsafe_emplace_back(key, partner);
        }
      }; // struct update_neighbor_list2<false, true>

      template <>
      struct update_neighbor_list2<false, false>
      {
        template <typename NeighborList, typename KeyParticles, typename PartnerParticles, typename Value>
        static void call(NeighborList& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles, Value const& squared_search_length)
        {
          neighbor_list.clear();

          auto const num_key_particles = ::pastel::container::num_particles(key_particles);
          auto const num_partner_particles = ::pastel::container::num_particles(partner_particles);
          using size_type = decltype(num_key_particles);
          for (auto key = size_type{0}; key < num_key_particles; ++key)
            for (auto partner = size_type{0}; partner < num_partner_particles; ++partner)
              if (::pastel::geometry::squared_distance(
                    ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key),
                    ::pastel::container::get< ::pastel::particle::tags::position >(partner_particles, partner))
                  < squared_search_length)
                neighbor_list.unsafe_emplace_back(key, partner);
        }
      }; // struct update_neighbor_list2<false, false>
    } // namespace inexclusive_cells_updater_detail


    template <typename Point>
    class inexclusive_cells_updater
    {
     public:
      using point_type = Point;
      using vector_type = decltype(std::declval<Point>{} - std::declval<Point>{});
      static constexpr std::size_t dimension = ::pastel::geometry::meta::dimension_of<point_type>::value;
      using value_type = typename ::pastel::geometry::meta::value_of<point_type>::type;

     private:
      value_type search_length_;
      value_type squared_search_length_;
      value_type buffer_length_;

      point_type lower_bound_;
      point_type upper_bound_;
      std::array<std::size_t, dimension> num_cells_;
      std::size_t total_num_cells_;
      vector_type cell_vector_;

      using indices_type = std::vector<std::size_t>;
      indices_type particle_indices_in_each_cell_; // particle_indices_in_each_cell_.size() == num_particles
      std::vector<typename indices_type::iterator> particle_indices_in_each_cell_firsts_; // particle_indices_in_each_cell_firsts_.size() == num_cells+1
      indices_type particle_index_to_cell_index_; // particle_index_to_cell_index_.size() == num_particles
      indices_type num_particles_in_each_cell_; // num_particles_in_each_cell_.size() == num_cells

     public:
      inexclusive_cells_updater()
        : search_length_{value_type{1}},
          squared_search_length_{search_length_ * search_length_},
          buffer_length_{value_type{-1}},
          lower_bound_{},
          upper_bound_{},
          num_cells_{},
          total_num_cells_{},
          cell_vector_{},
          particle_indices_in_each_cell_{},
          particle_indices_in_each_cell_firsts_(1u, particle_indices_in_each_cell_.begin()),
          particle_index_to_cell_index_{},
          num_particles_in_each_cell_{}
      { }

      template <typename Time, typename Speed, typename... NumCells>
      inexclusive_cells_updater(
        value_type search_length, value_type cutoff_length, Time time_step, Speed maximal_speed,
        point_type const& lower_bound, point_type const& upper_bound, NumCells const... num_cells)
        : search_length_{search_length},
          squared_search_length_{search_length * search_length},
          buffer_length_{search_length - cutoff_length - value_type{2} * time_step * maximal_speed},
          lower_bound_{lower_bound},
          upper_bound_{upper_bound},
          num_cells_{num_cells...},
          total_num_cells_{::pastel::utility::prod(num_cells...)},
          cell_vector_{
            ::pastel::neighbor::inexclusive_cells_updater_detail::generate_cell_vector(
                upper_bound-lower_bound, num_cells_)},
          particle_indices_in_each_cell_{},
          particle_indices_in_each_cell_firsts_(total_num_cells_+1u, particle_indices_in_each_cell_.begin()),
          particle_index_to_cell_index_{},
          num_particles_in_each_cell_{total_num_cells_, 0u}
      {
        assert(search_length > cutoff_length && cutoff_length > value_type{0});
        assert(time_step > Time{0});
        assert(maximal_speed > Speed{0});
        assert(lower_bound_ < upper_bound_);
      }


      value_type const& search_length() const { return search_length_; }
      void search_length(value_type new_search_length)
      {
        buffer_length_ += new_search_length - search_length_;
        search_length_ = new_search_length;
        squared_search_length_ = search_length_ * search_length_;
      }

      value_type const& buffer_length() const { return buffer_length_; }

      bool is_valid() const { return buffer_length_ > value_type{0}; }


      template <typename NeighborList, typename Particles, typename Time>
      void reset_status(NeighborList const& neighbor_list, Particles const& particles, Time time_step)
      {
        using interaction_pair_type
          = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
        static_assert(interaction_pair_type::first == interaction_pair_type::second, "NeighborList must be intraparticle");
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList const>::type>::value,
          "Force of NeighborList must have cutoff");

        buffer_length_
          = ::pastel::neighbor::inexclusive_cells_updater_detail::update_status(
              neighbor_list, particles, time_step,
              search_length_ - ::pastel::force::cutoff_length(::pastel::neighbor::force(neighbor_list)));
      }

      template <typename NeighborList, typename KeyParticles, typename PartnerParticles, typename Time>
      void reset_status(NeighborList const& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles, Time time_step)
      {
        //using interaction_pair_type
        //  = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
        //static_assert(interaction_pair_type::first != interaction_pair_type::second, "NeighborList must be interparticle");
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList const>::type>::value,
          "Force of NeighborList must have cutoff");

        buffer_length_
          = ::pastel::neighbor::inexclusive_cells_updater_detail::update_status(
              neighbor_list, key_particles, partner_particles, time_step,
              search_length_ - ::pastel::force::cutoff_length(::pastel::neighbor::force(neighbor_list)));
      }


      template <typename NeighborList, typename Particles, typename Time>
      void update_status(NeighborList const& neighbor_list, Particles const& particles, Time time_step)
      {
        using interaction_pair_type
          = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
        static_assert(interaction_pair_type::first == interaction_pair_type::second, "NeighborList must be intraparticle");
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList const>::type>::value,
          "Force of NeighborList must have cutoff");

        buffer_length_ = ::pastel::neighbor::inexclusive_cells_updater_detail::update_status(neighbor_list, particles, time_step, buffer_length_);
      }

      template <typename NeighborList, typename KeyParticles, typename PartnerParticles, typename Time>
      void update_status(NeighborList const& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles, Time time_step)
      {
        //using interaction_pair_type
        //  = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
        //static_assert(interaction_pair_type::first != interaction_pair_type::second, "NeighborList must be interparticle");
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList const>::type>::value,
          "Force of NeighborList must have cutoff");

        buffer_length_ = ::pastel::neighbor::inexclusive_cells_updater_detail::update_status(neighbor_list, key_particles, partner_particles, time_step, buffer_length_);
      }


      template <typename NeighborList, typename Particles, typename Time>
      void update_neighbor_list(NeighborList& neighbor_list, Particles const& particles)
      {
        using interaction_pair_type
          = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
        static_assert(interaction_pair_type::first == interaction_pair_type::second, "NeighborList must be intraparticle");
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList>::type>::value,
          "Force of NeighborList must have cutoff");

        auto const num_particles = ::pastel::container::num_particles(particles);
        particle_indices_in_each_cell_.resize(num_particles);
        particle_index_to_cell_index_.resize(num_particles);
        ::pastel::neighbor::inexclusive_cells_updater_detail::update_cell_data(
          particle_indices_in_each_cell_, particle_indices_in_each_cell_firsts_,
          particle_index_to_cell_index_, num_particles_in_each_cell_,
          lower_bound_, upper_bound_, cell_vector_, num_cells_, total_num_cells_,
          particles);

        static constexpr bool is_data_accessible
          = ::pastel::container::meta::is_data_accessible<Particles>::value;
        using update_neighbor_list_func
          = ::pastel::neighbor::inexclusive_cells_updater_detail::update_neighbor_list1<is_data_accessible>;
        update_neighbor_list_func::call(
          neighbor_list, particles, squared_search_length_,
          particles_indices_in_each_cell_, particle_indices_in_each_cell_firsts_);
      }

      template <typename NeighborList, typename KeyParticles, typename PartnerParticles, typename Time>
      void update_neighbor_list(NeighborList& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles)
      {
        //using interaction_pair_type
        //  = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
        //static_assert(interaction_pair_type::first != interaction_pair_type::second, "NeighborList must be interparticle");
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList>::type>::value,
          "Force of NeighborList must have cutoff");

        auto const num_particles
          = ::pastel::container::num_particles(key_particles) + ::pastel::container::num_particles(partner_particles);
        particle_indices_in_each_cell_.resize(num_particles);
        particle_index_to_cell_index_.resize(num_particles);
        ::pastel::neighbor::inexclusive_cells_updater_detail::update_cell_data(
          particle_indices_in_each_cell_, particle_indices_in_each_cell_firsts_,
          particle_index_to_cell_index_, num_particles_in_each_cell_,
          lower_bound_, upper_bound_, cell_vector_, num_cells_, total_num_cells_,
          particles1, particles2);

        static constexpr bool is_key_data_accessible
          = ::pastel::container::meta::is_data_accessible<KeyParticles>::value;
        static constexpr bool is_partner_data_accessible
          = ::pastel::container::meta::is_data_accessible<PartnerParticles>::value;
        using update_neighbor_list_func
          = ::pastel::neighbor::inexclusive_cells_updater_detail::update_neighbor_list2<is_key_data_accessible, is_partner_data_accessible>;
        update_neighbor_list_func::call(neighbor_list, key_particles, partner_particles, squared_search_length_);
      }
    }; // class inexclusive_cells_updater<Cell, CellAllocator>
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_INEXCLUSIVE_CELLS_UPDATER_HPP

