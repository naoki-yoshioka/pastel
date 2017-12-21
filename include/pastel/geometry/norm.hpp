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

    inline constexpr float norm(float value) { return value < 0.0f ? -value : value; }
    inline constexpr double norm(double value) { return value < 0.0 ? -value : value; }
    inline constexpr long double norm(long double value) { return value < 0.0l ? -value : value; }
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_NORM_HPP

