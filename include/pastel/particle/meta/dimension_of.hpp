#ifndef PASTEL_PARTICLE_META_DIMENSION_OF_HPP
# define PASTEL_PARTICLE_META_DIMENSION_OF_HPP

# include <cstddef>
# include <type_traits>


namespace pastel
{
  namespace particle
  {
    namespace meta
    {
      template <typename Particle>
      struct dimension_of
        : std::integral_constant<std::size_t, Particle::dimension>
      { };
    } // namespace meta
  } // namespace particle
} // namespace pastel


#endif // PASTEL_PARTICLE_META_DIMENSION_OF_HPP

