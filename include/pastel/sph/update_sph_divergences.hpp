// TODO this code probably has bugs CHECK IT
#ifndef PASTEL_SPH_UPDATE_SPH_DIVERGENCES_HPP
# define PASTEL_SPH_UPDATE_SPH_DIVERGENCES_HPP

# include <pastel/sph/detail/calculate_sph_divergence.hpp>
# include <pastel/particle/tags.hpp>
# include <pastel/particle/meta/has_divergence.hpp>
# include <pastel/container/get.hpp>
# include <pastel/container/increase_sph_divergence.hpp>
# include <pastel/container/decrease_sph_divergence.hpp>
# include <pastel/container/meta/scalar_of.hpp>
# include <pastel/container/meta/has_mass.hpp>
# include <pastel/neighbor/num_keys.hpp>
# include <pastel/neighbor/num_partners.hpp>
# include <pastel/neighbor/partner_begin.hpp>
# include <pastel/neighbor/partner_end.hpp>
# include <pastel/neighbor/meta/interaction_pair_of.hpp>
# include <pastel/neighbor/meta/is_partner_data_accessible.hpp>
# include <pastel/system/particles.hpp>
# include <pastel/utility/pair.hpp>


namespace pastel
{
  namespace sph
  {
    namespace update_sph_divergences_detail
    {
      template <bool has_divergences, bool is_partner_data_accessible, bool has_key_mass, bool has_partner_mass>
      struct update_sph_divergences;

      template <>
      struct update_sph_divergences<true, true, true, true>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const& force, NeighborList const& neighbor_list, KeyParticles& key_particles, PartnerParticles& partner_particles)
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
            using scalar_type
              = typename ::pastel::container::meta::scalar_of<KeyParticles>::type;
            auto key_sph_divergence = scalar_type{};

            auto const num_partners = ::pastel::neighbor::num_partners(neighbor_list, key);
            auto const partner_data = neighbor_list.partner_data(key);
            for (auto partner_index = static_cast<decltype(num_partners)>(0); partner_index < num_partners; ++partner_index)
            {
              auto const partner = partner_data[partner_index];

              auto const sph_divergence_increment
                = ::pastel::sph::detail::calculate_sph_divergence(
                    key_position, key_velocity, key_mass,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::mass >(
                      partner_particles, partner),
                    force);
              key_sph_divergence += sph_divergence_increment;
              ::pastel::container::decrease_sph_divergence(partner_particles, partner, sph_divergence_increment);
            }
            ::pastel::container::increase_sph_divergence(key_particles, key, key_sph_divergence);
          }
        }
      }; // struct update_sph_divergences<true, true, true, true>

      template <>
      struct update_sph_divergences<true, true, true, false>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const& force, NeighborList const& neighbor_list, KeyParticles& key_particles, PartnerParticles& partner_particles)
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
            using scalar_type
              = typename ::pastel::container::meta::scalar_of<KeyParticles>::type;
            auto key_sph_divergence = scalar_type{};

            auto const num_partners = ::pastel::neighbor::num_partners(neighbor_list, key);
            auto const partner_data = neighbor_list.partner_data(key);
            for (auto partner_index = static_cast<decltype(num_partners)>(0); partner_index < num_partners; ++partner_index)
            {
              auto const partner = partner_data[partner_index];

              auto const sph_divergence_increment
                = ::pastel::sph::detail::calculate_sph_divergence(
                    key_position, key_velocity, key_mass,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, partner),
                    force);
              key_sph_divergence += sph_divergence_increment;
              ::pastel::container::decrease_sph_divergence(partner_particles, partner, sph_divergence_increment);
            }
            ::pastel::container::increase_sph_divergence(key_particles, key, key_sph_divergence);
          }
        }
      }; // struct update_sph_divergences<true, true, true, false>

      template <>
      struct update_sph_divergences<true, true, false, true>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const& force, NeighborList const& neighbor_list, KeyParticles& key_particles, PartnerParticles& partner_particles)
        {
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_velocity
              = ::pastel::container::get< ::pastel::particle::tags::velocity >(key_particles, key);
            using scalar_type
              = typename ::pastel::container::meta::scalar_of<KeyParticles>::type;
            auto key_sph_divergence = scalar_type{};

            auto const num_partners = ::pastel::neighbor::num_partners(neighbor_list, key);
            auto const partner_data = neighbor_list.partner_data(key);
            for (auto partner_index = static_cast<decltype(num_partners)>(0); partner_index < num_partners; ++partner_index)
            {
              auto const partner = partner_data[partner_index];

              auto const sph_divergence_increment
                = ::pastel::sph::detail::calculate_sph_divergence(
                    key_position, key_velocity,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::mass >(
                      partner_particles, partner),
                    force);
              key_sph_divergence += sph_divergence_increment;
              ::pastel::container::decrease_sph_divergence(partner_particles, partner, sph_divergence_increment);
            }
            ::pastel::container::increase_sph_divergence(key_particles, key, key_sph_divergence);
          }
        }
      }; // struct update_sph_divergences<true, true, false, true>

      template <>
      struct update_sph_divergences<true, true, false, false>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const& force, NeighborList const& neighbor_list, KeyParticles& key_particles, PartnerParticles& partner_particles)
        {
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_velocity
              = ::pastel::container::get< ::pastel::particle::tags::velocity >(key_particles, key);
            using scalar_type
              = typename ::pastel::container::meta::scalar_of<KeyParticles>::type;
            auto key_sph_divergence = scalar_type{};

            auto const num_partners = ::pastel::neighbor::num_partners(neighbor_list, key);
            auto const partner_data = neighbor_list.partner_data(key);
            for (auto partner_index = static_cast<decltype(num_partners)>(0); partner_index < num_partners; ++partner_index)
            {
              auto const partner = partner_data[partner_index];

              auto const sph_divergence_increment
                = ::pastel::sph::detail::calculate_sph_divergence(
                    key_position, key_velocity,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, partner),
                    force);
              key_sph_divergence += sph_divergence_increment;
              ::pastel::container::decrease_sph_divergence(partner_particles, partner, sph_divergence_increment);
            }
            ::pastel::container::increase_sph_divergence(key_particles, key, key_sph_divergence);
          }
        }
      }; // struct update_sph_divergences<true, true, true, false>


      template <>
      struct update_sph_divergences<true, false, true, true>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const& force, NeighborList const& neighbor_list, KeyParticles& key_particles, PartnerParticles& partner_particles)
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
            using scalar_type
              = typename ::pastel::container::meta::scalar_of<KeyParticles>::type;
            auto key_sph_divergence = scalar_type{};

            auto const partner_end = ::pastel::neighbor::partner_end(neighbor_list, key);
            for (auto partner_iter = ::pastel::neighbor::partner_begin(neighbor_list, key);
                 partner_iter != partner_end; ++partner_iter)
            {
              auto const sph_divergence_increment
                = ::pastel::sph::detail::calculate_sph_divergence(
                    key_position, key_velocity, key_mass,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::mass >(
                      partner_particles, *partner_iter),
                    force);
              key_sph_divergence += sph_divergence_increment;
              ::pastel::container::decrease_sph_divergence(partner_particles, partner, sph_divergence_increment);
            }
            ::pastel::container::increase_sph_divergence(key_particles, key, key_sph_divergence);
          }
        }
      }; // struct update_sph_divergences<true, false, true, true>

      template <>
      struct update_sph_divergences<true, false, true, false>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const& force, NeighborList const& neighbor_list, KeyParticles& key_particles, PartnerParticles& partner_particles)
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
            using scalar_type
              = typename ::pastel::container::meta::scalar_of<KeyParticles>::type;
            auto key_sph_divergence = scalar_type{};

            auto const partner_end = ::pastel::neighbor::partner_end(neighbor_list, key);
            for (auto partner_iter = ::pastel::neighbor::partner_begin(neighbor_list, key);
                 partner_iter != partner_end; ++partner_iter)
            {
              auto const sph_divergence_increment
                = ::pastel::sph::detail::calculate_sph_divergence(
                    key_position, key_velocity, key_mass,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, *partner_iter),
                    force);
              key_sph_divergence += sph_divergence_increment;
              ::pastel::container::decrease_sph_divergence(partner_particles, partner, sph_divergence_increment);
            }
            ::pastel::container::increase_sph_divergence(key_particles, key, key_sph_divergence);
          }
        }
      }; // struct update_sph_divergences<true, false, true, false>

      template <>
      struct update_sph_divergences<true, false, false, true>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const& force, NeighborList const& neighbor_list, KeyParticles& key_particles, PartnerParticles& partner_particles)
        {
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_velocity
              = ::pastel::container::get< ::pastel::particle::tags::velocity >(key_particles, key);
            using scalar_type
              = typename ::pastel::container::meta::scalar_of<KeyParticles>::type;
            auto key_sph_divergence = scalar_type{};

            auto const partner_end = ::pastel::neighbor::partner_end(neighbor_list, key);
            for (auto partner_iter = ::pastel::neighbor::partner_begin(neighbor_list, key);
                 partner_iter != partner_end; ++partner_iter)
            {
              auto const sph_divergence_increment
                = ::pastel::sph::detail::calculate_sph_divergence(
                    key_position, key_velocity,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::mass >(
                      partner_particles, *partner_iter),
                    force);
              key_sph_divergence += sph_divergence_increment;
              ::pastel::container::decrease_sph_divergence(partner_particles, partner, sph_divergence_increment);
            }
            ::pastel::container::increase_sph_divergence(key_particles, key, key_sph_divergence);
          }
        }
      }; // struct update_sph_divergences<true, false, false, true>

      template <>
      struct update_sph_divergences<true, false, false, false>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const& force, NeighborList const& neighbor_list, KeyParticles& key_particles, PartnerParticles& partner_particles)
        {
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_velocity
              = ::pastel::container::get< ::pastel::particle::tags::velocity >(key_particles, key);
            using scalar_type
              = typename ::pastel::container::meta::scalar_of<KeyParticles>::type;
            auto key_sph_divergence = scalar_type{};

            auto const partner_end = ::pastel::neighbor::partner_end(neighbor_list, key);
            for (auto partner_iter = ::pastel::neighbor::partner_begin(neighbor_list, key);
                 partner_iter != partner_end; ++partner_iter)
            {
              auto const sph_divergence_increment
                = ::pastel::sph::detail::calculate_sph_divergence(
                    key_position, key_velocity,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::velocity >(
                      partner_particles, *partner_iter),
                    force);
              key_sph_divergence += sph_divergence_increment;
              ::pastel::container::decrease_sph_divergence(partner_particles, partner, sph_divergence_increment);
            }
            ::pastel::container::increase_sph_divergence(key_particles, key, key_sph_divergence);
          }
        }
      }; // struct update_sph_divergences<true, false, false, false>


      template <>
      struct update_sph_divergences<false, true, true, true>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const&, NeighborList const&, KeyParticles const&, PartnerParticles const&)
        { }
      }; // struct update_sph_divergences<false, true, true, true>

      template <>
      struct update_sph_divergences<false, true, true, false>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const&, NeighborList const&, KeyParticles const&, PartnerParticles const&)
        { }
      }; // struct update_sph_divergences<false, true, true, false>

      template <>
      struct update_sph_divergences<false, true, false, true>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const&, NeighborList const&, KeyParticles const&, PartnerParticles const&)
        { }
      }; // struct update_sph_divergences<false, true, false, true>

      template <>
      struct update_sph_divergences<false, true, false, false>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const&, NeighborList const&, KeyParticles const&, PartnerParticles const&)
        { }
      }; // struct update_sph_divergences<false, true, false, false>

      template <>
      struct update_sph_divergences<false, false, true, true>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const&, NeighborList const&, KeyParticles const&, PartnerParticles const&)
        { }
      }; // struct update_sph_divergences<false, false, true, true>

      template <>
      struct update_sph_divergences<false, false, true, false>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const&, NeighborList const&, KeyParticles const&, PartnerParticles const&)
        { }
      }; // struct update_sph_divergences<false, false, true, false>

      template <>
      struct update_sph_divergences<false, false, false, true>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const&, NeighborList const&, KeyParticles const&, PartnerParticles const&)
        { }
      }; // struct update_sph_divergences<false, false, false, true>

      template <>
      struct update_sph_divergences<false, false, false, false>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const&, NeighborList const&, KeyParticles const&, PartnerParticles const&)
        { }
      }; // struct update_sph_divergences<false, false, false, false>
    } // namespace update_sph_divergences_detail


    template<typename Force, typename NeighborList, typename System>
    inline void update_sph_divergences(Force const& force, NeighborList const& neighbor_list, System& system)
    {
      using interaction_pair_type = ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
      using key_particles = ::pastel::system::meta::particles_of<interaction_pair_type::first, System>::type;
      using partner_particles = ::pastel::system::meta::particles_of<interaction_pair_type::second, System>::type;
      static constexpr bool has_divergences
        = ::pastel::particle::meta::has_divergence<typename ::pastel::container::meta::value_of<key_particles>::type>::value
          && ::pastel::particle::meta::has_divergence<typename ::pastel::container::meta::value_of<partner_particles>::type>::value;

      static constexpr bool is_partner_data_accessible
        = ::pastel::neighbor::meta::is_partner_data_accessible<NeighborList>::value;
      static constexpr bool has_key_mass = ::pastel::container::meta::has_mass<key_particles>::value;
      static constexpr bool has_partner_mass = ::pastel::container::meta::has_mass<partner_particles>::value;
      ::pastel::sph::update_sph_divergences_detail::update_sph_divergences<has_divergences, is_partner_data_accessible, has_key_mass, has_partner_mass>::call(
        force, neighbor_list,
        ::pastel::system::particles<interaction_pair_type::first>(system),
        ::pastel::system::particles<interaction_pair_type::second>(system));
    }
  } // namespace sph
} // namespace pastel


#endif // PASTEL_SPH_UPDATE_SPH_DIVERGENCES_HPP

