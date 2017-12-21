#ifndef PASTEL_CONTAINER_META_SCALAR_OF_HPP
# define PASTEL_CONTAINER_META_SCALAR_OF_HPP


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct scalar_of
      { using type = typename Particles::scalar_type; };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_SCALAR_OF_HPP

