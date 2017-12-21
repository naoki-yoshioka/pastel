#ifndef PASTEL_CONTAINER_META_DIMENSION_OF_HPP
# define PASTEL_CONTAINER_META_DIMENSION_OF_HPP

# include <cstddef>
# include <type_traits>


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct dimension_of
        : std::integral_constant<std::size_t, Particles::dimension>
      { };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_DIMENSION_OF_HPP

