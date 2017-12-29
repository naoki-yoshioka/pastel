#ifndef PASTEL_GEOMETRY_DOT_PRODUCT_HPP
# define PASTEL_GEOMETRY_DOT_PRODUCT_HPP

# include <cstddef>
# include <cmath>

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

    template <std::size_t dimension_>
    inline float dot_product(
      ::pastel::geometry::vector<dimension_, float> const& lhs,
      ::pastel::geometry::vector<dimension_, float> const& rhs)
    {
      auto result = float{};
      auto const lhs_data = lhs.data();
      auto const rhs_data = rhs.data();
      for (auto index = std::size_t{0}; index < dimension_; ++index)
        result = std::fma(lhs_data[index], rhs_data[index], result);
      return result;
    }

    template <std::size_t dimension_>
    inline double dot_product(
      ::pastel::geometry::vector<dimension_, double> const& lhs,
      ::pastel::geometry::vector<dimension_, double> const& rhs)
    {
      auto result = double{};
      auto const lhs_data = lhs.data();
      auto const rhs_data = rhs.data();
      for (auto index = std::size_t{0}; index < dimension_; ++index)
        result = std::fma(lhs_data[index], rhs_data[index], result);
      return result;
    }

    template <std::size_t dimension_>
    inline long double dot_product(
      ::pastel::geometry::vector<dimension_, long double> const& lhs,
      ::pastel::geometry::vector<dimension_, long double> const& rhs)
    {
      using value_type = long double;
      auto result = value_type{};
      auto const lhs_data = lhs.data();
      auto const rhs_data = rhs.data();
      for (auto index = std::size_t{0}; index < dimension_; ++index)
        result = std::fma(lhs_data[index], rhs_data[index], result);
      return result;
    }

    inline constexpr float dot_product(float lhs, float rhs) { return lhs * rhs; }
    inline constexpr double dot_product(double lhs, double rhs) { return lhs * rhs; }
    inline constexpr long double dot_product(long double lhs, long double rhs) { return lhs * rhs; }
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_DOT_PRODUCT_HPP

