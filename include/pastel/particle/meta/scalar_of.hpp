#ifndef PASTEL_PARTICLE_META_SCALAR_OF_HPP
# define PASTEL_PARTICLE_META_SCALAR_OF_HPP


namespace pastel
{
  namespace particle
  {
    namespace meta
    {
      template <typename Particle>
      struct scalar_of
      { using type = typename Particle::scalar_type; };
    } // namespace meta
  } // namespace particle
} // namespace pastel


#endif // PASTEL_PARTICLE_META_SCALAR_OF_HPP

