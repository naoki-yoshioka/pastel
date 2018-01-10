#ifndef PASTEL_NEIGHBOR_BRUTE_FORCE_UPDATER_HPP
# define PASTEL_NEIGHBOR_BRUTE_FORCE_UPDATER_HPP

# include <cassert>
# include <cstddef>
# include <algorithm>

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


namespace pastel
{
  namespace neighbor
  {
    namespace brute_force_updater_detail
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


      template <bool is_data_accessible>
      struct update_neighbor_list1;

      template <>
      struct update_neighbor_list1<true>
      {
        template <typename NeighborList, typename Particles, typename Value>
        static void call(NeighborList& neighbor_list, Particles const& particles, Value const& squared_search_length)
        {
          neighbor_list.clear();

          auto const positions_data = particles.template data< ::pastel::particle::tags::position >();

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
    } // namespace brute_force_updater_detail


    template <typename Value>
    class brute_force_updater
    {
      Value search_length_;
      Value squared_search_length_;
      Value buffer_length_;

     public:
      brute_force_updater()
        : search_length_{Value{1}},
          squared_search_length_{search_length_ * search_length_},
          buffer_length_{Value{-1}}
      { }

      template <typename Time, typename Speed>
      brute_force_updater(Value search_length, Value cutoff_length, Time time_step, Speed maximal_speed)
        : search_length_{search_length},
          squared_search_length_{search_length * search_length},
          buffer_length_{search_length - cutoff_length - Value{2} * time_step * maximal_speed}
      { assert(search_length > cutoff_length && cutoff_length > Value{0} && time_step > Time{0} && maximal_speed > Speed{0}); }

      Value const& search_length() const { return search_length_; }
      void search_length(Value new_search_length)
      {
        buffer_length_ += new_search_length - search_length_;
        search_length_ = new_search_length;
        squared_search_length_ = search_length_ * search_length_;
      }

      Value const& buffer_length() const { return buffer_length_; }

      bool is_valid() const { return buffer_length_ > Value{0}; }
      bool is_invalid() const { return !this->is_valid(); }


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
          = ::pastel::neighbor::brute_force_updater_detail::update_status(
              neighbor_list, particles, time_step,
              search_length_ - ::pastel::force::cutoff_length(::pastel::neighbor::force(neighbor_list)));
      }

      template <typename NeighborList, typename KeyParticles, typename PartnerParticles, typename Time>
      void reset_status(NeighborList const& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles, Time time_step)
      {
        using interaction_pair_type
          = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
        static_assert(interaction_pair_type::first != interaction_pair_type::second, "NeighborList must be interparticle");
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList const>::type>::value,
          "Force of NeighborList must have cutoff");

        buffer_length_
          = ::pastel::neighbor::brute_force_updater_detail::update_status(
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

        buffer_length_ = ::pastel::neighbor::brute_force_updater_detail::update_status(neighbor_list, particles, time_step, buffer_length_);
      }

      template <typename NeighborList, typename KeyParticles, typename PartnerParticles, typename Time>
      void update_status(NeighborList const& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles, Time time_step)
      {
        using interaction_pair_type
          = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
        static_assert(interaction_pair_type::first != interaction_pair_type::second, "NeighborList must be interparticle");
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList const>::type>::value,
          "Force of NeighborList must have cutoff");

        buffer_length_ = ::pastel::neighbor::brute_force_updater_detail::update_status(neighbor_list, key_particles, partner_particles, time_step, buffer_length_);
      }


      template <typename NeighborList, typename Particles>
      void update_neighbor_list(NeighborList& neighbor_list, Particles const& particles) const
      {
        using interaction_pair_type
          = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
        static_assert(interaction_pair_type::first == interaction_pair_type::second, "NeighborList must be intraparticle");
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList>::type>::value,
          "Force of NeighborList must have cutoff");

        static constexpr bool is_data_accessible
          = ::pastel::container::meta::is_data_accessible<Particles>::value;
        using update_neighbor_list_func
          = ::pastel::neighbor::brute_force_updater_detail::update_neighbor_list1<is_data_accessible>;
        update_neighbor_list_func::call(neighbor_list, particles, squared_search_length_);
      }

      template <typename NeighborList, typename KeyParticles, typename PartnerParticles>
      void update_neighbor_list(NeighborList& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles) const
      {
        using interaction_pair_type
          = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
        static_assert(interaction_pair_type::first != interaction_pair_type::second, "NeighborList must be interparticle");
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList>::type>::value,
          "Force of NeighborList must have cutoff");

        static constexpr bool is_key_data_accessible
          = ::pastel::container::meta::is_data_accessible<KeyParticles>::value;
        static constexpr bool is_partner_data_accessible
          = ::pastel::container::meta::is_data_accessible<PartnerParticles>::value;
        using update_neighbor_list_func
          = ::pastel::neighbor::brute_force_updater_detail::update_neighbor_list2<is_key_data_accessible, is_partner_data_accessible>;
        update_neighbor_list_func::call(neighbor_list, key_particles, partner_particles, squared_search_length_);
      }
    }; // class brute_force_updater<Value>
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_BRUTE_FORCE_UPDATER_HPP

