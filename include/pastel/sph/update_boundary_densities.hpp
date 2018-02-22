#ifndef PASTEL_SPH_UPDATE_BOUNDARY_DENSITIES_HPP
# define PASTEL_SPH_UPDATE_BOUNDARY_DENSITIES_HPP

# include <cmath>

# include <pastel/geometry/squared_norm.hpp>
# include <pastel/particle/tags.hpp>
# include <pastel/particle/meta/is_sph_particle.hpp>
# include <pastel/particle/meta/has_divergence.hpp>
# include <pastel/container/get.hpp>
# include <pastel/container/meta/scalar_of.hpp>
# include <pastel/container/meta/has_mass.hpp>
# include <pastel/neighbor/num_keys.hpp>
# include <pastel/neighbor/num_partners.hpp>
# include <pastel/neighbor/partner_begin.hpp>
# include <pastel/neighbor/partner_end.hpp>
# include <pastel/neighbor/meta/interaction_pair_of.hpp>
# include <pastel/neighbor/meta/is_partner_data_accessible.hpp>
# include <pastel/force/squared_cutoff_length.hpp>
# include <pastel/system/particles.hpp>
# include <pastel/system/boundary_particles.hpp>
# include <pastel/utility/pair.hpp>


namespace pastel
{
  namespace sph
  {
    namespace update_boundary_densities_detail
    {
      template <bool is_sph_particles_without_divergences, bool is_partner_data_accessible, bool has_partner_mass>
      struct update_boundary_densities;

      template <>
      struct update_boundary_densities<true, true, true>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const& force, NeighborList const& neighbor_list, KeyParticles& key_particles, PartnerParticles& partner_particles)
        {
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            using scalar_type
              = typename ::pastel::container::meta::scalar_of<KeyParticles>::type;
            auto key_density = scalar_type{};

            auto const num_partners = ::pastel::neighbor::num_partners(neighbor_list, key);
            auto const partner_data = neighbor_list.partner_data(key);
            for (auto partner_index = static_cast<decltype(num_partners)>(0); partner_index < num_partners; ++partner_index)
            {
              auto const partner = partner_data[partner_index];

              auto const squared_distance
                = ::pastel::geometry::squared_norm(
                    key_position - ::pastel::container::get< ::pastel::particle::tags::position >(partner_particles, partner));
              if (squared_distance > ::pastel::force::squared_cutoff_length(force))
                continue;

              using std::sqrt;
              auto const weight = force.kernel()(sqrt(squared_distance));
              key_density
                += weight * ::pastel::container::get< ::pastel::particle::tags::mass >(partner_particles, partner);
              // partner_particles must be immobile because of boundary_particles
            }
            ::pastel::container::get< ::pastel::particle::tags::density >(key_particles, key) += key_density;
          }
        }
      }; // struct update_boundary_densities<true, true, true>

      template <>
      struct update_boundary_densities<true, true, false>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const& force, NeighborList const& neighbor_list, KeyParticles& key_particles, PartnerParticles& partner_particles)
        {
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            using scalar_type
              = typename ::pastel::container::meta::scalar_of<KeyParticles>::type;
            auto key_density = scalar_type{};

            auto const num_partners = ::pastel::neighbor::num_partners(neighbor_list, key);
            auto const partner_data = neighbor_list.partner_data(key);
            for (auto partner_index = static_cast<decltype(num_partners)>(0); partner_index < num_partners; ++partner_index)
            {
              auto const partner = partner_data[partner_index];

              auto const squared_distance
                = ::pastel::geometry::squared_norm(
                    key_position - ::pastel::container::get< ::pastel::particle::tags::position >(partner_particles, partner));
              if (squared_distance > ::pastel::force::squared_cutoff_length(force))
                continue;

              using std::sqrt;
              auto const weight = force.kernel()(sqrt(squared_distance));
              key_density += weight;
              // partner_particles must be immobile because of boundary_particles
            }
            ::pastel::container::get< ::pastel::particle::tags::density >(key_particles, key) += key_density;
          }
        }
      }; // struct update_boundary_densities<true, true, false>


      template <>
      struct update_boundary_densities<true, false, true>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const& force, NeighborList const& neighbor_list, KeyParticles& key_particles, PartnerParticles& partner_particles)
        {
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            using scalar_type
              = typename ::pastel::container::meta::scalar_of<KeyParticles>::type;
            auto key_density = scalar_type{};

            auto const partner_end = ::pastel::neighbor::partner_end(neighbor_list, key);
            for (auto partner_iter = ::pastel::neighbor::partner_begin(neighbor_list, key);
                 partner_iter != partner_end; ++partner_iter)
            {
              auto const partner = *partner_iter;

              auto const squared_distance
                = ::pastel::geometry::squared_norm(
                    key_position - ::pastel::container::get< ::pastel::particle::tags::position >(partner_particles, partner));
              if (squared_distance > ::pastel::force::squared_cutoff_length(force))
                continue;

              using std::sqrt;
              auto const weight = force.kernel()(sqrt(squared_distance));
              key_density
                += weight * ::pastel::container::get< ::pastel::particle::tags::mass >(partner_particles, partner);
              // partner_particles must be immobile because of boundary_particles
            }
            ::pastel::container::get< ::pastel::particle::tags::density >(key_particles, key) += key_density;
          }
        }
      }; // struct update_boundary_densities<true, false, true>

      template <>
      struct update_boundary_densities<true, false, false>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const& force, NeighborList const& neighbor_list, KeyParticles& key_particles, PartnerParticles& partner_particles)
        {
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            using scalar_type
              = typename ::pastel::container::meta::scalar_of<KeyParticles>::type;
            auto key_density = scalar_type{};

            auto const partner_end = ::pastel::neighbor::partner_end(neighbor_list, key);
            for (auto partner_iter = ::pastel::neighbor::partner_begin(neighbor_list, key);
                 partner_iter != partner_end; ++partner_iter)
            {
              auto const partner = *partner_iter;

              auto const squared_distance
                = ::pastel::geometry::squared_norm(
                    key_position - ::pastel::container::get< ::pastel::particle::tags::position >(partner_particles, partner));
              if (squared_distance > ::pastel::force::squared_cutoff_length(force))
                continue;

              using std::sqrt;
              auto const weight = force.kernel()(sqrt(squared_distance));
              key_density += weight;
              // partner_particles must be immobile because of boundary_particles
            }
            ::pastel::container::get< ::pastel::particle::tags::density >(key_particles, key) += key_density;
          }
        }
      }; // struct update_boundary_densities<true, false, false>


      template <>
      struct update_boundary_densities<false, true, true>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const&, NeighborList const&, KeyParticles const&, PartnerParticles const&)
        { }
      }; // struct update_boundary_densities<false, true, true>

      template <>
      struct update_boundary_densities<false, true, false>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const&, NeighborList const&, KeyParticles const&, PartnerParticles const&)
        { }
      }; // struct update_boundary_densities<false, true, false>

      template <>
      struct update_boundary_densities<false, false, true>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const&, NeighborList const&, KeyParticles const&, PartnerParticles const&)
        { }
      }; // struct update_boundary_densities<false, false, true>

      template <>
      struct update_boundary_densities<false, false, false>
      {
        template <typename Force, typename NeighborList, typename KeyParticles, typename PartnerParticles>
        static void call(Force const&, NeighborList const&, KeyParticles const&, PartnerParticles const&)
        { }
      }; // struct update_boundary_densities<false, false, false>
    } // namespace update_boundary_densities_detail


    template<typename Force, typename NeighborList, typename System>
    inline void update_boundary_densities(Force const& force, NeighborList const& neighbor_list, System& system)
    {
      using interaction_pair_type = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
      using key_particles_type = typename ::pastel::system::meta::particles_of<interaction_pair_type::first, System>::type;
      using partner_particles_type = typename ::pastel::system::meta::particles_of<interaction_pair_type::second, System>::type;
      using key_particle_type = typename ::pastel::container::meta::value_of<key_particles_type>::type;
      using partner_particle_type = typename ::pastel::container::meta::value_of<partner_particles_type>::type;
      static constexpr bool is_sph_particles_without_divergences
        = (::pastel::particle::meta::is_sph_particle<key_particle_type>::value
           && !::pastel::particle::meta::has_divergence<key_particle_type>::value)
          && (::pastel::particle::meta::is_sph_particle<partner_particle_type>::value
              && !::pastel::particle::meta::has_divergence<partner_particle_type>::value);

      static constexpr bool is_partner_data_accessible
        = ::pastel::neighbor::meta::is_partner_data_accessible<NeighborList>::value;
      static constexpr bool has_partner_mass = ::pastel::container::meta::has_mass<partner_particles_type>::value;
      ::pastel::sph::update_boundary_densities_detail::update_boundary_densities<is_sph_particles_without_divergences, is_partner_data_accessible, has_partner_mass>::call(
        force, neighbor_list,
        ::pastel::system::particles<interaction_pair_type::first>(system),
        ::pastel::system::boundary_particles<interaction_pair_type::second>(system));
    }
  } // namespace sph
} // namespace pastel


#endif // PASTEL_SPH_UPDATE_BOUNDARY_DENSITIES_HPP

