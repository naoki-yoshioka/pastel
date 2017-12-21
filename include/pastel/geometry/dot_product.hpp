#ifndef PASTEL_GEOMETRY_DOT_PRODUCT_HPP
# define PASTEL_GEOMETRY_DOT_PRODUCT_HPP

# include <cstddef>

# include <pastel/geometry/vector.hpp>


namespace pastel
{
  namespace geometry
  {
    template <std::size_t dimension_, typename Value>
    inline Value dot_product(
      ::pastel::geometry::vector<dimension_, Value> const& lhs,
      ::pastel::geometry::vector<dimension_, Value> const& rhs)
    {
      auto result = Value{};
      auto const lhs_data = lhs.data();
      auto const rhs_data = rhs.data();
      for (auto index = std::size_t{0}; index < dimension_; ++index)
        result += lhs_data[index] * rhs_data[index];
      return result;
    }

    inline constexpr float dot_product(float lhs, float rhs) { return lhs * rhs; }
    inline constexpr double dot_product(double lhs, double rhs) { return lhs * rhs; }
    inline constexpr long double dot_product(long double lhs, long double rhs) { return lhs * rhs; }
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_DOT_PRODUCT_HPP

