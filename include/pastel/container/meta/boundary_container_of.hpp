#ifndef PASTEL_CONTAINER_META_BOUNDARY_CONTAINER_OF_HPP
# define PASTEL_CONTAINER_META_BOUNDARY_CONTAINER_OF_HPP


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct boundary_container_of
      { using type = typename Particles::boundary_container_type; };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_BOUNDARY_CONTAINER_OF_HPP

