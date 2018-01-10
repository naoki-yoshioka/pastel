#ifndef PASTEL_FORCE_UPDATE_BOUNDARY_FORCES_HPP
# define PASTEL_FORCE_UPDATE_BOUNDARY_FORCES_HPP

# include <pastel/force/tags.hpp>
# include <pastel/force/meta/required_arguments_category_of.hpp>
# include <pastel/particle/tags.hpp>
# include <pastel/container/get.hpp>
# include <pastel/container/increase_force.hpp>
# include <pastel/container/decrease_force.hpp>
# include <pastel/neighbor/num_keys.hpp>
# include <pastel/neighbor/num_partners.hpp>
# include <pastel/neighbor/partner_begin.hpp>
# include <pastel/neighbor/partner_end.hpp>
# include <pastel/neighbor/meta/interaction_pair_of.hpp>
# include <pastel/neighbor/meta/is_partner_data_accessible.hpp>
# include <pastel/system/particles.hpp>
# include <pastel/system/boundary_particles.hpp>
# include <pastel/system/meta/vector_of.hpp>
# include <pastel/utility/pair.hpp>


namespace pastel
{
  namespace force
  {
    namespace update_boundary_forces_detail
    {
      template <bool is_partner_data_accessible>
      struct update_boundary_forces;

      template <>
      struct update_boundary_forces<true>
      {
        template <typename Force, typename BoundaryNeighborList, typename System>
        static void call(
          Force const& force, BoundaryNeighborList const& boundary_neighbor_list, System& system,
          ::pastel::force::tags::requires_position)
        {
          using interaction_pair_type
            = typename ::pastel::neighbor::meta::interaction_pair_of<BoundaryNeighborList>::type;
          auto& key_particles
            = ::pastel::system::particles<interaction_pair_type::first>(system);
          auto& partner_particles
            = ::pastel::system::boundary_particles<interaction_pair_type::second>(system);

          auto const num_keys = ::pastel::neighbor::num_keys(boundary_neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            using vector_type
              = typename ::pastel::system::meta::vector_of<interaction_pair_type::first, System>::type;
            auto key_force = vector_type{};

            auto const num_partners = ::pastel::neighbor::num_partners(boundary_neighbor_list, key);
            auto const partner_data = boundary_neighbor_list.partner_data(key);
            for (auto partner_index = static_cast<decltype(num_partners)>(0); partner_index < num_partners; ++partner_index)
            {
              auto const partner = partner_data[partner_index];
              auto const force_increment
                = force(
                    key_position,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, partner));
              key_force += force_increment;
              // partner_particles must be immobile because of boundary_particles
            }
            ::pastel::container::increase_force(key_particles, key, key_force);
          }
        }

        template <typename Force, typename BoundaryNeighborList, typename System>
        static void call(
          Force const& force, BoundaryNeighborList const& boundary_neighbor_list, System& system,
          ::pastel::force::tags::requires_position_velocity)
        {
          using interaction_pair_type
            = typename ::pastel::neighbor::meta::interaction_pair_of<BoundaryNeighborList>::type;
          auto& key_particles
            = ::pastel::system::particles<interaction_pair_type::first>(system);
          auto& partner_particles
            = ::pastel::system::boundary_particles<interaction_pair_type::second>(system);

          auto const num_keys = ::pastel::neighbor::num_keys(boundary_neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_velocity
              = ::pastel::container::get< ::pastel::particle::tags::velocity >(key_particles, key);
            using vector_type
              = typename ::pastel::system::meta::vector_of<interaction_pair_type::first, System>::type;
            auto key_force = vector_type{};

            auto const num_partners = ::pastel::neighbor::num_partners(boundary_neighbor_list, key);
            auto const partner_data = boundary_neighbor_list.partner_data(key);
            for (auto partner_index = static_cast<decltype(num_partners)>(0); partner_index < num_partners; ++partner_index)
            {
              auto const partner = partner_data[partner_index];
              auto const force_increment
                = force(
                    key_position, key_velocity,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, partner));
              key_force += force_increment;
              // partner_particles must be immobile because of boundary_particles
            }
            ::pastel::container::increase_force(key_particles, key, key_force);
          }
        }
      }; // struct update_boundary_forces<true>


      template <>
      struct update_boundary_forces<false>
      {
        template <typename Force, typename BoundaryNeighborList, typename System>
        static void call(
          Force const& force, BoundaryNeighborList const& boundary_neighbor_list, System& system,
          ::pastel::force::tags::requires_position)
        {
          using interaction_pair_type
            = typename ::pastel::neighbor::meta::interaction_pair_of<BoundaryNeighborList>::type;
          auto& key_particles
            = ::pastel::system::particles<interaction_pair_type::first>(system);
          auto& partner_particles
            = ::pastel::system::boundary_particles<interaction_pair_type::second>(system);

          auto const num_keys = ::pastel::neighbor::num_keys(boundary_neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            using vector_type
              = typename ::pastel::system::meta::vector_of<interaction_pair_type::first, System>::type;
            auto key_force = vector_type{};

            auto const partner_end = ::pastel::neighbor::partner_end(boundary_neighbor_list, key);
            for (auto partner_iter = ::pastel::neighbor::partner_begin(boundary_neighbor_list, key);
                 partner_iter != partner_end; ++partner_iter)
            {
              auto const force_increment
                = force(
                    key_position,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, *partner_iter));
              key_force += force_increment;
              // partner_particles must be immobile because of boundary_particles
            }
            ::pastel::container::increase_force(key_particles, key, key_force);
          }
        }

        template <typename Force, typename BoundaryNeighborList, typename System>
        static void call(
          Force const& force, BoundaryNeighborList const& boundary_neighbor_list, System& system,
          ::pastel::force::tags::requires_position_velocity)
        {
          using interaction_pair_type
            = typename ::pastel::neighbor::meta::interaction_pair_of<BoundaryNeighborList>::type;
          auto& key_particles
            = ::pastel::system::particles<interaction_pair_type::first>(system);
          auto& partner_particles
            = ::pastel::system::boundary_particles<interaction_pair_type::second>(system);

          auto const num_keys = ::pastel::neighbor::num_keys(boundary_neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_velocity
              = ::pastel::container::get< ::pastel::particle::tags::velocity >(key_particles, key);
            using vector_type
              = typename ::pastel::system::meta::vector_of<interaction_pair_type::first, System>::type;
            auto key_force = vector_type{};

            auto const partner_end = ::pastel::neighbor::partner_end(boundary_neighbor_list, key);
            for (auto partner_iter = ::pastel::neighbor::partner_begin(boundary_neighbor_list, key);
                 partner_iter != partner_end; ++partner_iter)
            {
              auto const force_increment
                = force(
                    key_position, key_velocity,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, *partner_iter));
              key_force += force_increment;
              // partner_particles must be immobile because of boundary_particles
            }
            ::pastel::container::increase_force(key_particles, key, key_force);
          }
        }
      }; // struct update_boundary_forces<false>
    } // namespace update_boundary_forces_detail


    template<typename Force, typename BoundaryNeighborList, typename System>
    inline void update_boundary_forces(
      Force const& force, BoundaryNeighborList const& boundary_neighbor_list, System& system)
    {
      static constexpr bool is_partner_data_accessible
        = ::pastel::neighbor::meta::is_partner_data_accessible<BoundaryNeighborList>::value;
      using required_arguments_category
        = typename ::pastel::force::meta::required_arguments_category_of<Force>::type;
      ::pastel::force::update_boundary_forces_detail::update_boundary_forces<is_partner_data_accessible>::call(
        force, boundary_neighbor_list, system, required_arguments_category{});
    }
  } // namespace force
} // namespace pastel


#endif // PASTEL_FORCE_UPDATE_BOUNDARY_FORCES_HPP

