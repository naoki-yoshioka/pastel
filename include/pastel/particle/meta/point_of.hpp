#ifndef PASTEL_PARTICLE_META_POINTER_OF_HPP
# define PASTEL_PARTICLE_META_POINTER_OF_HPP


namespace pastel
{
  namespace particle
  {
    namespace meta
    {
      template <typename Particle>
      struct point_of
      { using type = typename Particle::point_type; };
    } // namespace meta
  } // namespace particle
} // namespace pastel


#endif // PASTEL_PARTICLE_META_POINTER_OF_HPP

