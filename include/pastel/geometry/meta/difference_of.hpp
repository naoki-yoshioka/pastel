#ifndef PASTEL_GEOMETRY_META_DIFFERENCE_OF_HPP
# define PASTEL_GEOMETRY_META_DIFFERENCE_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace geometry
  {
    namespace meta
    {
      template <typename Quantity>
      struct difference_of
      { using type = typename Quantity::difference_type; };

      template <>
      struct difference_of<float>
      { using type = std::ptrdiff_t; };

      template <>
      struct difference_of<float const>
      { using type = std::ptrdiff_t; };

      template <>
      struct difference_of<double>
      { using type = std::ptrdiff_t; };

      template <>
      struct difference_of<double const>
      { using type = std::ptrdiff_t; };

      template <>
      struct difference_of<long double>
      { using type = std::ptrdiff_t; };

      template <>
      struct difference_of<long double const>
      { using type = std::ptrdiff_t; };
    } // namespace meta
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_META_DIFFERENCE_OF_HPP

