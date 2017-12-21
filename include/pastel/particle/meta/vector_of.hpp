#ifndef PASTEL_PARTICLE_META_VECTOR_OF_HPP
# define PASTEL_PARTICLE_META_VECTOR_OF_HPP


namespace pastel
{
  namespace particle
  {
    namespace meta
    {
      template <typename Particle>
      struct vector_of
      { using type = typename Particle::vector_type; };
    } // namespace meta
  } // namespace particle
} // namespace pastel


#endif // PASTEL_PARTICLE_META_VECTOR_OF_HPP

