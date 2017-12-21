#ifndef PASTEL_CONTAINER_META_SIZE_OF_HPP
# define PASTEL_CONTAINER_META_SIZE_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct size_of
      { using type = typename Particles::size_type; };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_SIZE_OF_HPP

