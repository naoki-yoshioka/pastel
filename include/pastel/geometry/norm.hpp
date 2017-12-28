#ifndef PASTEL_GEOMETRY_NORM_HPP
# define PASTEL_GEOMETRY_NORM_HPP

# include <cstddef>
# include <cmath>

# include <pastel/geometry/vector.hpp>
# include <pastel/geometry/squared_norm.hpp>


namespace pastel
{
  namespace geometry
  {
    template <std::size_t dimension_, typename Value>
    inline Value norm(::pastel::geometry::vector<dimension_, Value> const& vector)
    { using std::sqrt; return sqrt(::pastel::geometry::squared_norm(vector)); }

    inline float norm(::pastel::geometry::vector<1u, float> const& vector)
    {
      auto const data = vector.data();
      return std::fabs(data[0u]);
    }

    inline double norm(::pastel::geometry::vector<1u, double> const& vector)
    {
      auto const data = vector.data();
      return std::fabs(data[0u]);
    }

    inline long double norm(::pastel::geometry::vector<1u, long double> const& vector)
    {
      auto const data = vector.data();
      return std::fabs(data[0u]);
    }

    inline float norm(::pastel::geometry::vector<2u, float> const& vector)
    {
      auto const data = vector.data();
      return std::hypot(data[0u], data[1u]);
    }

    inline double norm(::pastel::geometry::vector<2u, double> const& vector)
    {
      auto const data = vector.data();
      return std::hypot(data[0u], data[1u]);
    }

    inline long double norm(::pastel::geometry::vector<2u, long double> const& vector)
    {
      auto const data = vector.data();
      return std::hypot(data[0u], data[1u]);
    }

    inline float norm(::pastel::geometry::vector<3u, float> const& vector)
    {
      auto const data = vector.data();
      return std::hypot(data[0u], data[1u], data[2u]);
    }

    inline double norm(::pastel::geometry::vector<3u, double> const& vector)
    {
      auto const data = vector.data();
      return std::hypot(data[0u], data[1u], data[2u]);
    }

    inline long double norm(::pastel::geometry::vector<3u, long double> const& vector)
    {
      auto const data = vector.data();
      return std::hypot(data[0u], data[1u], data[2u]);
    }

    inline constexpr float norm(float value) { return std::fabs(value); }
    inline constexpr double norm(double value) { return std::fabs(value) }
    inline constexpr long double norm(long double value) { return std::fabs(value); }
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_NORM_HPP

