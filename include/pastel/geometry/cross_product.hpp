#ifndef PASTEL_GEOMETRY_CROSS_PRODUCT_HPP
# define PASTEL_GEOMETRY_CROSS_PRODUCT_HPP

# include <pastel/geometry/vector.hpp>


namespace pastel
{
  namespace geometry
  {
    template <typename Value>
    inline Value cross_product(
      ::pastel::geometry::vector<2u, Value> const& lhs,
      ::pastel::geometry::vector<2u, Value> const& rhs)
    {
      auto const lhs_data = lhs.data();
      auto const rhs_data = rhs.data();
      return lhs_data[0u] * rhs_data[1u] - lhs_data[1u] * rhs_data[0u];
    }

    template <typename Value>
    inline ::pastel::geometry::vector<3u, Value> cross_product(
      ::pastel::geometry::vector<3u, Value> const& lhs,
      ::pastel::geometry::vector<3u, Value> const& rhs)
    {
      auto const lhs_data = lhs.data();
      auto const rhs_data = rhs.data();
      return {
        lhs_data[1u] * rhs_data[2u] - lhs_data[2u] * rhs_data[1u],
        lhs_data[2u] * rhs_data[0u] - lhs_data[0u] * rhs_data[2u],
        lhs_data[0u] * rhs_data[1u] - lhs_data[1u] * rhs_data[0u]};
    }

    template <typename Value>
    inline constexpr Value cross_product(
      ::pastel::geometry::vector<1u, Value> const& lhs,
      ::pastel::geometry::vector<1u, Value> const& rhs)
    { return Value{0}; }

    inline constexpr float cross_product(float lhs, float rhs) { return 0.0f; }
    inline constexpr double cross_product(double lhs, double rhs) { return 0.0; }
    inline constexpr long double cross_product(long double lhs, long double rhs) { return 0.0l; }
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_CROSS_PRODUCT_HPP

