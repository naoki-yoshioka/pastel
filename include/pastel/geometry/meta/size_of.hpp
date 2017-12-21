#ifndef PASTEL_GEOMETRY_META_SIZE_OF_HPP
# define PASTEL_GEOMETRY_META_SIZE_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace geometry
  {
    namespace meta
    {
      template <typename Quantity>
      struct size_of
      { using type = typename Quantity::size_type; };

      template <>
      struct size_of<float>
      { using type = std::size_t; };

      template <>
      struct size_of<float const>
      { using type = std::size_t; };

      template <>
      struct size_of<double>
      { using type = std::size_t; };

      template <>
      struct size_of<double const>
      { using type = std::size_t; };

      template <>
      struct size_of<long double>
      { using type = std::size_t; };

      template <>
      struct size_of<long double const>
      { using type = std::size_t; };
    } // namespace meta
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_META_SIZE_OF_HPP

