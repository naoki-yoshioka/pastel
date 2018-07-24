#ifndef PASTEL_PARTICLE_META_HAS_DIVERGENCE_HPP
# define PASTEL_PARTICLE_META_HAS_DIVERGENCE_HPP

# include <type_traits>

# include <pastel/particle/meta/is_sph_particle.hpp>


namespace pastel
{
  namespace particle
  {
    namespace meta
    {
      namespace has_divergence_detail
      {
        template <typename Particle, bool is_sph_particle>
        struct has_divergence;

        template <typename Particle>
        struct has_divergence<Particle, true>
          : std::integral_constant<bool, Particle::has_divergence>
        { };

        template <typename Particle>
        struct has_divergence<Particle, false>
          : std::false_type
        { };
      } // namespace has_divergence_detail

      template <typename Particle>
      struct has_divergence
        : ::pastel::particle::meta::has_divergence_detail::has_divergence<Particle, ::pastel::particle::meta::is_sph_particle<Particle>::value>
      { };
    } // namespace meta
  } // namespace particle
} // namespace pastel


#endif // PASTEL_PARTICLE_META_HAS_DIVERGENCE_HPP

