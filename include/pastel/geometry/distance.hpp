#ifndef PASTEL_GEOMETRY_DISTANCE_HPP
# define PASTEL_GEOMETRY_DISTANCE_HPP

# include <cstddef>
# include <cmath>

# include <pastel/geometry/point.hpp>
# include <pastel/geometry/squared_distance.hpp>


namespace pastel
{
  namespace geometry
  {
    template <std::size_t dimension_, typename Value>
    inline Value distance(
      ::pastel::geometry::point<dimension_, Value> const& lhs,
      ::pastel::geometry::point<dimension_, Value> const& rhs)
    { using std::sqrt; return sqrt(::pastel::geometry::squared_distance(lhs, rhs)); }
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_DISTANCE_HPP

