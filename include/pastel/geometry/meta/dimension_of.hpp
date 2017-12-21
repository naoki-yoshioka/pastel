#ifndef PASTEL_GEOMETRY_META_DIMENSION_OF_HPP
# define PASTEL_GEOMETRY_META_DIMENSION_OF_HPP

# include <cstddef>
# include <type_traits>


namespace pastel
{
  namespace geometry
  {
    namespace meta
    {
      template <typename Quantity>
      struct dimension_of
        : std::integral_constant<std::size_t, Quantity::dimension>
      { };

      template <>
      struct dimension_of<float>
        : std::integral_constant<std::size_t, 1u>
      { };

      template <>
      struct dimension_of<float const>
        : std::integral_constant<std::size_t, 1u>
      { };

      template <>
      struct dimension_of<double>
        : std::integral_constant<std::size_t, 1u>
      { };

      template <>
      struct dimension_of<double const>
        : std::integral_constant<std::size_t, 1u>
      { };

      template <>
      struct dimension_of<long double>
        : std::integral_constant<std::size_t, 1u>
      { };

      template <>
      struct dimension_of<long double const>
        : std::integral_constant<std::size_t, 1u>
      { };
    } // namespace meta
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_META_DIMENSION_OF_HPP

