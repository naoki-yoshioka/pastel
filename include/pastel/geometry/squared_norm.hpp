#ifndef PASTEL_GEOMETRY_SQUARED_NORM_HPP
# define PASTEL_GEOMETRY_SQUARED_NORM_HPP

# include <cstddef>
# include <cmath>

# include <pastel/geometry/vector.hpp>
# include <pastel/geometry/dot_product.hpp>


namespace pastel
{
  namespace geometry
  {
    template <std::size_t dimension_, typename Value>
    inline Value squared_norm(::pastel::geometry::vector<dimension_, Value> const& vector)
    { return ::pastel::geometry::dot_product(vector, vector); }

    template <std::size_t dimension_>
    inline float squared_norm(::pastel::geometry::vector<dimension_, float> const& vector)
    {
      auto result = float{};
      auto const data = vector.data();
      for (auto index = std::size_t{0}; index < dimension_; ++index)
        result = std::fma(data[index], data[index], result);
      return result;
    }

    template <std::size_t dimension_>
    inline double squared_norm(::pastel::geometry::vector<dimension_, double> const& vector)
    {
      auto result = double{};
      auto const data = vector.data();
      for (auto index = std::size_t{0}; index < dimension_; ++index)
        result = std::fma(data[index], data[index], result);
      return result;
    }

    template <std::size_t dimension_>
    inline long double squared_norm(::pastel::geometry::vector<dimension_, long double> const& vector)
    {
      auto result = long double{};
      auto const data = vector.data();
      for (auto index = std::size_t{0}; index < dimension_; ++index)
        result = std::fma(data[index], data[index], result);
      return result;
    }

    inline constexpr float squared_norm(float value) { return value * value; }
    inline constexpr double squared_norm(double value) { return value * value; }
    inline constexpr long double squared_norm(long double value) { return value * value; }
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_SQUARED_NORM_HPP

