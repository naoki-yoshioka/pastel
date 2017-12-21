#ifndef PASTEL_GEOMETRY_SQUARED_NORM_HPP
# define PASTEL_GEOMETRY_SQUARED_NORM_HPP

# include <cstddef>

# include <pastel/geometry/vector.hpp>
# include <pastel/geometry/dot_product.hpp>


namespace pastel
{
  namespace geometry
  {
    template <std::size_t dimension_, typename Value>
    inline Value squared_norm(::pastel::geometry::vector<dimension_, Value> const& vector)
    { return ::pastel::geometry::dot_product(vector, vector); }

    inline constexpr float squared_norm(float value) { return value * value; }
    inline constexpr double squared_norm(double value) { return value * value; }
    inline constexpr long double squared_norm(long double value) { return value * value; }
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_SQUARED_NORM_HPP

