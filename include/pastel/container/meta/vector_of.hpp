#ifndef PASTEL_CONTAINER_META_VECTOR_OF_HPP
# define PASTEL_CONTAINER_META_VECTOR_OF_HPP


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct vector_of
      { using type = typename Particles::vector_type; };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_VECTOR_OF_HPP

