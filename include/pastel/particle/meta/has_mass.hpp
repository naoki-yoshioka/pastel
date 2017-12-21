#ifndef PASTEL_PARTICLE_META_HAS_MASS_HPP
# define PASTEL_PARTICLE_META_HAS_MASS_HPP

# include <type_traits>


namespace pastel
{
  namespace particle
  {
    namespace meta
    {
      template <typename Particle>
      struct has_mass
        : std::integral_constant<bool, Particle::has_mass>
      { };
    } // namespace meta
  } // namespace particle
} // namespace pastel


#endif // PASTEL_PARTICLE_META_HAS_MASS_HPP

