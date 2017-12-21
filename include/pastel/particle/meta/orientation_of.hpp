#ifndef PASTEL_PARTICLE_META_ORIENTATION_OF_HPP
# define PASTEL_PARTICLE_META_ORIENTATION_OF_HPP


namespace pastel
{
  namespace particle
  {
    namespace meta
    {
      template <typename Particle>
      struct orientation_of
      { using type = typename Particle::orientation_type; };
    } // namespace meta
  } // namespace particle
} // namespace pastel


#endif // PASTEL_PARTICLE_META_ORIENTATION_OF_HPP

