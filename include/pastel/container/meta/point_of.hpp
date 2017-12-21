#ifndef PASTEL_CONTAINER_META_POINT_OF_HPP
# define PASTEL_CONTAINER_META_POINT_OF_HPP


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct point_of
      { using type = typename Particles::point_type; };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_POINT_OF_HPP

