#ifndef PASTEL_GEOMETRY_SQUARED_DISTANCE_HPP
# define PASTEL_GEOMETRY_SQUARED_DISTANCE_HPP

# include <cstddef>

# include <pastel/geometry/point.hpp>
# include <pastel/geometry/squared_norm.hpp>


namespace pastel
{
  namespace geometry
  {
    template <std::size_t dimension_, typename Value>
    inline Value squared_distance(
      ::pastel::geometry::point<dimension_, Value> const& lhs,
      ::pastel::geometry::point<dimension_, Value> const& rhs)
    { return ::pastel::geometry::squared_norm(lhs-rhs); }
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_SQUARED_DISTANCE_HPP

