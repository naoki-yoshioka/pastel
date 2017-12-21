#ifndef PASTEL_CONTAINER_META_ORIENTATION_OF_HPP
# define PASTEL_CONTAINER_META_ORIENTATION_OF_HPP


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct orientation_of
      { using type = typename Particles::orientation_type; };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_ORIENTATION_OF_HPP

