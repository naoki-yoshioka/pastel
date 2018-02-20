#ifndef PASTEL_PARTICLE_META_IS_SPH_PARTICLE_HPP
# define PASTEL_PARTICLE_META_IS_SPH_PARTICLE_HPP

# include <type_traits>


namespace pastel
{
  namespace particle
  {
    namespace meta
    {
      template <typename Particle>
      struct is_sph_particle
        : std::integral_constant<bool, Particle::is_sph_particle>
      { };
    } // namespace meta
  } // namespace particle
} // namespace pastel


#endif // PASTEL_PARTICLE_META_IS_SPH_PARTICLE_HPP

