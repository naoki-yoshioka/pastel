#ifndef PASTEL_FORCE_UPDATE_FORCES_HPP
# define PASTEL_FORCE_UPDATE_FORCES_HPP

# include <pastel/force/tags.hpp>
# include <pastel/force/meta/required_arguments_category_of.hpp>
# include <pastel/particle/tags.hpp>
# include <pastel/container/get.hpp>
# include <pastel/container/increase_force.hpp>
# include <pastel/container/decrease_force.hpp>
# include <pastel/container/meta/vector_of.hpp>
# include <pastel/container/meta/scalar_of.hpp>
# include <pastel/container/meta/has_mass.hpp>
# include <pastel/neighbor/num_keys.hpp>
# include <pastel/neighbor/num_partners.hpp>
# include <pastel/neighbor/partner_begin.hpp>
# include <pastel/neighbor/partner_end.hpp>
# include <pastel/neighbor/meta/interaction_pair_of.hpp>
# include <pastel/neighbor/meta/is_partner_data_accessible.hpp>
# include <pastel/system/particles.hpp>
# include <pastel/system/meta/particles_of.hpp>
# include <pastel/utility/pair.hpp>


namespace pastel
{
  namespace force
  {
    namespace update_forces_detail
    {
      template <bool has_key_mass, bool has_partner_mass>
      struct update_newtonian_sph_forces_1;

      template <>
      struct update_newtonian_sph_forces_1<true, true>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(
          Force const& force, NeighborList const& neighbor_list,
          KeyParticles& key_partices, PartnerParticles& partner_particles)
        {
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_velocity
              = ::pastel::container::get< ::pastel::particle::tags::velocity >(key_particles, key);
            auto const& key_mass
              = ::pastel::container::get< ::pastel::particle::tags::mass >(key_particles, key);
            auto const& key_density
              = ::pastel::container::get< ::pastel::particle::tags::density >(key_particles, key);
            auto const& key_pressure
              = ::pastel::container::get< ::pastel::particle::tags::pressure >(key_particles, key);
            using vector_type
              = typename ::pastel::container::meta::vector_of<KeyParticles>::type;
            auto key_force = vector_type{};

            auto const num_partners = ::pastel::neighbor::num_partners(neighbor_list, key);
            auto const partner_data = neighbor_list.partner_data(key);
            for (auto partner_index = static_cast<decltype(num_partners)>(0); partner_index < num_partners; ++partner_index)
            {
              auto const partner = partner_data[partner_index];
              auto const force_increment
                = force(
                    key_position, key_velocity, key_mass, key_density, key_pressure,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::mass >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::density >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::pressure >(
                      partner_particles, partner));
              key_force += force_increment;
              ::pastel::container::decrease_force(partner_particles, partner, force_increment);
            }
            ::pastel::container::increase_force(key_particles, key, key_force);
          }
        }
      }; // struct update_newtonian_sph_forces_1<true, true>

      template <>
      struct update_newtonian_sph_forces_1<true, false>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(
          Force const& force, NeighborList const& neighbor_list,
          KeyParticles& key_partices, PartnerParticles& partner_particles)
        {
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_velocity
              = ::pastel::container::get< ::pastel::particle::tags::velocity >(key_particles, key);
            auto const& key_mass
              = ::pastel::container::get< ::pastel::particle::tags::mass >(key_particles, key);
            auto const& key_density
              = ::pastel::container::get< ::pastel::particle::tags::density >(key_particles, key);
            auto const& key_pressure
              = ::pastel::container::get< ::pastel::particle::tags::pressure >(key_particles, key);
            using vector_type
              = typename ::pastel::container::meta::vector_of<KeyParticles>::type;
            auto key_force = vector_type{};

            auto const num_partners = ::pastel::neighbor::num_partners(neighbor_list, key);
            auto const partner_data = neighbor_list.partner_data(key);
            for (auto partner_index = static_cast<decltype(num_partners)>(0); partner_index < num_partners; ++partner_index)
            {
              auto const partner = partner_data[partner_index];
              using scalar_type
                = typename ::pastel::container::meta::scalar_of<PartnerParticles>::type;
              auto const force_increment
                = force(
                    key_position, key_velocity, key_mass, key_density, key_pressure,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, partner),
                    scalar_type{1},
                    ::pastel::container::get< ::pastel::particle::tags::density >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::pressure >(
                      partner_particles, partner));
              key_force += force_increment;
              ::pastel::container::decrease_force(partner_particles, partner, force_increment);
            }
            ::pastel::container::increase_force(key_particles, key, key_force);
          }
        }
      }; // struct update_newtonian_sph_forces_1<true, false>

      template <>
      struct update_newtonian_sph_forces_1<false, true>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(
          Force const& force, NeighborList const& neighbor_list,
          KeyParticles& key_partices, PartnerParticles& partner_particles)
        {
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_velocity
              = ::pastel::container::get< ::pastel::particle::tags::velocity >(key_particles, key);
            auto const& key_density
              = ::pastel::container::get< ::pastel::particle::tags::density >(key_particles, key);
            auto const& key_pressure
              = ::pastel::container::get< ::pastel::particle::tags::pressure >(key_particles, key);
            using vector_type
              = typename ::pastel::container::meta::vector_of<KeyParticles>::type;
            auto key_force = vector_type{};

            auto const num_partners = ::pastel::neighbor::num_partners(neighbor_list, key);
            auto const partner_data = neighbor_list.partner_data(key);
            for (auto partner_index = static_cast<decltype(num_partners)>(0); partner_index < num_partners; ++partner_index)
            {
              auto const partner = partner_data[partner_index];
              using scalar_type
                = typename ::pastel::container::meta::scalar_of<PartnerParticles>::type;
              auto const force_increment
                = force(
                    key_position, key_velocity, scalar_type{1}, key_density, key_pressure,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::mass >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::density >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::pressure >(
                      partner_particles, partner));
              key_force += force_increment;
              ::pastel::container::decrease_force(partner_particles, partner, force_increment);
            }
            ::pastel::container::increase_force(key_particles, key, key_force);
          }
        }
      }; // struct update_newtonian_sph_forces_1<false, true>

      template <>
      struct update_newtonian_sph_forces_1<false, false>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(
          Force const& force, NeighborList const& neighbor_list,
          KeyParticles& key_partices, PartnerParticles& partner_particles)
        {
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_velocity
              = ::pastel::container::get< ::pastel::particle::tags::velocity >(key_particles, key);
            auto const& key_density
              = ::pastel::container::get< ::pastel::particle::tags::density >(key_particles, key);
            auto const& key_pressure
              = ::pastel::container::get< ::pastel::particle::tags::pressure >(key_particles, key);
            using vector_type
              = typename ::pastel::container::meta::vector_of<KeyParticles>::type;
            auto key_force = vector_type{};

            auto const num_partners = ::pastel::neighbor::num_partners(neighbor_list, key);
            auto const partner_data = neighbor_list.partner_data(key);
            for (auto partner_index = static_cast<decltype(num_partners)>(0); partner_index < num_partners; ++partner_index)
            {
              auto const partner = partner_data[partner_index];
              using scalar_type
                = typename ::pastel::container::meta::scalar_of<PartnerParticles>::type;
              auto const force_increment
                = force(
                    key_position, key_velocity, scalar_type{1}, key_density, key_pressure,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, partner),
                    scalar_type{1},
                    ::pastel::container::get< ::pastel::particle::tags::density >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::pressure >(
                      partner_particles, partner));
              key_force += force_increment;
              ::pastel::container::decrease_force(partner_particles, partner, force_increment);
            }
            ::pastel::container::increase_force(key_particles, key, key_force);
          }
        }
      }; // struct update_newtonian_sph_forces_1<false, false>


      template <bool has_key_mass, bool has_partner_mass>
      struct update_newtonian_sph_forces_2;

      template <>
      struct update_newtonian_sph_forces_2<true, true>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(
          Force const& force, NeighborList const& neighbor_list,
          KeyParticles& key_partices, PartnerParticles& partner_particles)
        {
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_velocity
              = ::pastel::container::get< ::pastel::particle::tags::velocity >(key_particles, key);
            auto const& key_mass
              = ::pastel::container::get< ::pastel::particle::tags::mass >(key_particles, key);
            auto const& key_density
              = ::pastel::container::get< ::pastel::particle::tags::density >(key_particles, key);
            auto const& key_pressure
              = ::pastel::container::get< ::pastel::particle::tags::pressure >(key_particles, key);
            using vector_type
              = typename ::pastel::system::meta::vector_of<interaction_pair_type::first, System>::type;
            auto key_force = vector_type{};

            auto const partner_end = ::pastel::neighbor::partner_end(neighbor_list, key);
            for (auto partner_iter = ::pastel::neighbor::partner_begin(neighbor_list, key);
                 partner_iter != partner_end; ++partner_iter)
            {
              auto const force_increment
                = force(
                    key_position, key_velocity, key_mass, key_density, key_pressure,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::mass >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::density >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::pressure >(
                      partner_particles, *partner_iter));
              key_force += force_increment;
              ::pastel::container::decrease_force(partner_particles, *partner_iter, force_increment);
            }
            ::pastel::container::increase_force(key_particles, key, key_force);
          }
        }
      }; // struct update_newtonian_sph_forces_2<true, true>

      template <>
      struct update_newtonian_sph_forces_2<true, false>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(
          Force const& force, NeighborList const& neighbor_list,
          KeyParticles& key_partices, PartnerParticles& partner_particles)
        {
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_velocity
              = ::pastel::container::get< ::pastel::particle::tags::velocity >(key_particles, key);
            auto const& key_mass
              = ::pastel::container::get< ::pastel::particle::tags::mass >(key_particles, key);
            auto const& key_density
              = ::pastel::container::get< ::pastel::particle::tags::density >(key_particles, key);
            auto const& key_pressure
              = ::pastel::container::get< ::pastel::particle::tags::pressure >(key_particles, key);
            using vector_type
              = typename ::pastel::system::meta::vector_of<interaction_pair_type::first, System>::type;
            auto key_force = vector_type{};

            auto const partner_end = ::pastel::neighbor::partner_end(neighbor_list, key);
            for (auto partner_iter = ::pastel::neighbor::partner_begin(neighbor_list, key);
                 partner_iter != partner_end; ++partner_iter)
            {
              using scalar_type
                = typename ::pastel::container::meta::scalar_of<PartnerParticles>::type;
              auto const force_increment
                = force(
                    key_position, key_velocity, key_mass, key_density, key_pressure,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, *partner_iter),
                    scalar_type{1},
                    ::pastel::container::get< ::pastel::particle::tags::density >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::pressure >(
                      partner_particles, *partner_iter));
              key_force += force_increment;
              ::pastel::container::decrease_force(partner_particles, *partner_iter, force_increment);
            }
            ::pastel::container::increase_force(key_particles, key, key_force);
          }
        }
      }; // struct update_newtonian_sph_forces_2<true, false>

      template <>
      struct update_newtonian_sph_forces_2<false, true>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(
          Force const& force, NeighborList const& neighbor_list,
          KeyParticles& key_partices, PartnerParticles& partner_particles)
        {
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_velocity
              = ::pastel::container::get< ::pastel::particle::tags::velocity >(key_particles, key);
            auto const& key_density
              = ::pastel::container::get< ::pastel::particle::tags::density >(key_particles, key);
            auto const& key_pressure
              = ::pastel::container::get< ::pastel::particle::tags::pressure >(key_particles, key);
            using vector_type
              = typename ::pastel::system::meta::vector_of<interaction_pair_type::first, System>::type;
            auto key_force = vector_type{};

            auto const partner_end = ::pastel::neighbor::partner_end(neighbor_list, key);
            for (auto partner_iter = ::pastel::neighbor::partner_begin(neighbor_list, key);
                 partner_iter != partner_end; ++partner_iter)
            {
              using scalar_type
                = typename ::pastel::container::meta::scalar_of<PartnerParticles>::type;
              auto const force_increment
                = force(
                    key_position, key_velocity, scalar_type{1}, key_density, key_pressure,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::mass >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::density >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::pressure >(
                      partner_particles, *partner_iter));
              key_force += force_increment;
              ::pastel::container::decrease_force(partner_particles, *partner_iter, force_increment);
            }
            ::pastel::container::increase_force(key_particles, key, key_force);
          }
        }
      }; // struct update_newtonian_sph_forces_2<false, true>

      template <>
      struct update_newtonian_sph_forces_2<false, false>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(
          Force const& force, NeighborList const& neighbor_list,
          KeyParticles& key_partices, PartnerParticles& partner_particles)
        {
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_velocity
              = ::pastel::container::get< ::pastel::particle::tags::velocity >(key_particles, key);
            auto const& key_density
              = ::pastel::container::get< ::pastel::particle::tags::density >(key_particles, key);
            auto const& key_pressure
              = ::pastel::container::get< ::pastel::particle::tags::pressure >(key_particles, key);
            using vector_type
              = typename ::pastel::system::meta::vector_of<interaction_pair_type::first, System>::type;
            auto key_force = vector_type{};

            auto const partner_end = ::pastel::neighbor::partner_end(neighbor_list, key);
            for (auto partner_iter = ::pastel::neighbor::partner_begin(neighbor_list, key);
                 partner_iter != partner_end; ++partner_iter)
            {
              using scalar_type
                = typename ::pastel::container::meta::scalar_of<PartnerParticles>::type;
              auto const force_increment
                = force(
                    key_position, key_velocity, scalar_type{1}, key_density, key_pressure,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, *partner_iter),
                    scalar_type{1},
                    ::pastel::container::get< ::pastel::particle::tags::density >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::pressure >(
                      partner_particles, *partner_iter));
              key_force += force_increment;
              ::pastel::container::decrease_force(partner_particles, *partner_iter, force_increment);
            }
            ::pastel::container::increase_force(key_particles, key, key_force);
          }
        }
      }; // struct update_newtonian_sph_forces_2<false, false>


      template <bool is_partner_data_accessible>
      struct update_forces;

      template <>
      struct update_forces<true>
      {
        template <typename Force, typename NeighborList, typename System>
        static void call(
          Force const& force, NeighborList const& neighbor_list, System& system,
          ::pastel::force::tags::requires_position)
        {
          using interaction_pair_type
            = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
          auto& key_particles
            = ::pastel::system::particles<interaction_pair_type::first>(system);
          auto& partner_particles
            = ::pastel::system::particles<interaction_pair_type::second>(system);

          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            using vector_type
              = typename ::pastel::system::meta::vector_of<interaction_pair_type::first, System>::type;
            auto key_force = vector_type{};

            auto const num_partners = ::pastel::neighbor::num_partners(neighbor_list, key);
            auto const partner_data = neighbor_list.partner_data(key);
            for (auto partner_index = static_cast<decltype(num_partners)>(0); partner_index < num_partners; ++partner_index)
            {
              auto const partner = partner_data[partner_index];
              auto const force_increment
                = force(
                    key_position,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, partner));
              key_force += force_increment;
              ::pastel::container::decrease_force(partner_particles, partner, force_increment);
            }
            ::pastel::container::increase_force(key_particles, key, key_force);
          }
        }

        template <typename Force, typename NeighborList, typename System>
        static void call(
          Force const& force, NeighborList const& neighbor_list, System& system,
          ::pastel::force::tags::requires_position_velocity)
        {
          using interaction_pair_type
            = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
          auto& key_particles
            = ::pastel::system::particles<interaction_pair_type::first>(system);
          auto& partner_particles
            = ::pastel::system::particles<interaction_pair_type::second>(system);

          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_velocity
              = ::pastel::container::get< ::pastel::particle::tags::velocity >(key_particles, key);
            using vector_type
              = typename ::pastel::system::meta::vector_of<interaction_pair_type::first, System>::type;
            auto key_force = vector_type{};

            auto const num_partners = ::pastel::neighbor::num_partners(neighbor_list, key);
            auto const partner_data = neighbor_list.partner_data(key);
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
              ::pastel::container::decrease_force(partner_particles, partner, force_increment);
            }
            ::pastel::container::increase_force(key_particles, key, key_force);
          }
        }

        template <typename Force, typename NeighborList, typename System>
        static void call(
          Force const& force, NeighborList const& neighbor_list, System& system,
          ::pastel::force::tags::newtonian_sph_requirement)
        {
          using interaction_pair_type
            = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
          using key_particles
            = typename ::pastel::system::meta::particles_of<interaction_pair_type::first, System>::type;
          static constexpr bool has_key_mass = ::pastel::container::meta::has_mass<key_particles>::value;
          using partner_particles
            = typename ::pastel::system::meta::particles_of<interaction_pair_type::second, System>::type;
          static constexpr bool has_partner_mass = ::pastel::container::meta::has_mass<partner_particles>::value;

          auto& key_particles
            = ::pastel::system::particles<interaction_pair_type::first>(system);
          auto& partner_particles
            = ::pastel::system::particles<interaction_pair_type::second>(system);

          ::pastel::force::update_forces_detail::update_newtonian_sph_forces_1<has_key_mass, has_partner_mass>::call(
            force, neighbor_list, key_particles, partner_particles);
        }
      }; // struct update_forces<true>


      template <>
      struct update_forces<false>
      {
        template <typename Force, typename NeighborList, typename System>
        static void call(
          Force const& force, NeighborList const& neighbor_list, System& system,
          ::pastel::force::tags::requires_position)
        {
          using interaction_pair_type
            = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
          auto& key_particles
            = ::pastel::system::particles<interaction_pair_type::first>(system);
          auto& partner_particles
            = ::pastel::system::particles<interaction_pair_type::second>(system);

          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            using vector_type
              = typename ::pastel::system::meta::vector_of<interaction_pair_type::first, System>::type;
            auto key_force = vector_type{};

            auto const partner_end = ::pastel::neighbor::partner_end(neighbor_list, key);
            for (auto partner_iter = ::pastel::neighbor::partner_begin(neighbor_list, key);
                 partner_iter != partner_end; ++partner_iter)
            {
              auto const force_increment
                = force(
                    key_position,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, *partner_iter));
              key_force += force_increment;
              ::pastel::container::decrease_force(partner_particles, *partner_iter, force_increment);
            }
            ::pastel::container::increase_force(key_particles, key, key_force);
          }
        }

        template <typename Force, typename NeighborList, typename System>
        static void call(
          Force const& force, NeighborList const& neighbor_list, System& system,
          ::pastel::force::tags::requires_position_velocity)
        {
          using interaction_pair_type
            = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
          auto& key_particles
            = ::pastel::system::particles<interaction_pair_type::first>(system);
          auto& partner_particles
            = ::pastel::system::particles<interaction_pair_type::second>(system);

          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_velocity
              = ::pastel::container::get< ::pastel::particle::tags::velocity >(key_particles, key);
            using vector_type
              = typename ::pastel::system::meta::vector_of<interaction_pair_type::first, System>::type;
            auto key_force = vector_type{};

            auto const partner_end = ::pastel::neighbor::partner_end(neighbor_list, key);
            for (auto partner_iter = ::pastel::neighbor::partner_begin(neighbor_list, key);
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
              ::pastel::container::decrease_force(partner_particles, *partner_iter, force_increment);
            }
            ::pastel::container::increase_force(key_particles, key, key_force);
          }
        }

        template <typename Force, typename NeighborList, typename System>
        static void call(
          Force const& force, NeighborList const& neighbor_list, System& system,
          ::pastel::force::tags::newtonian_sph_requirement)
        {
          using interaction_pair_type
            = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
          using key_particles
            = typename ::pastel::system::meta::particles_of<interaction_pair_type::first, System>::type;
          static constexpr bool has_key_mass = ::pastel::container::meta::has_mass<key_particles>::value;
          using partner_particles
            = typename ::pastel::system::meta::particles_of<interaction_pair_type::second, System>::type;
          static constexpr bool has_partner_mass = ::pastel::container::meta::has_mass<partner_particles>::value;

          auto& key_particles
            = ::pastel::system::particles<interaction_pair_type::first>(system);
          auto& partner_particles
            = ::pastel::system::particles<interaction_pair_type::second>(system);

          ::pastel::force::update_forces_detail::update_newtonian_sph_forces_2<has_key_mass, has_partner_mass>::call(
            force, neighbor_list, key_particles, partner_particles);
        }
      }; // struct update_forces<false>
    } // namespace update_forces_detail


    template<typename Force, typename NeighborList, typename System>
    inline void update_forces(
      Force const& force, NeighborList const& neighbor_list, System& system)
    {
      static constexpr bool is_partner_data_accessible
        = ::pastel::neighbor::meta::is_partner_data_accessible<NeighborList>::value;
      using required_arguments_category
        = typename ::pastel::force::meta::required_arguments_category_of<Force>::type;
      ::pastel::force::update_forces_detail::update_forces<is_partner_data_accessible>::call(
        force, neighbor_list, system, required_arguments_category{});
    }
  } // namespace force
} // namespace pastel


#endif // PASTEL_FORCE_UPDATE_FORCES_HPP

