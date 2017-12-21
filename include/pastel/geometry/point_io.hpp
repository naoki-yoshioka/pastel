#ifndef PASTEL_GEOMETRY_POINT_IO_HPP
# define PASTEL_GEOMETRY_POINT_IO_HPP

# include <cstddef>
# include <ostream>

# include <pastel/geometry/point.hpp>


namespace pastel
{
  namespace geometry
  {
    template <typename Character, typename CharacterTraits, std::size_t dimension_, typename Value>
    std::basic_ostream<Character, CharacterTraits>& operator<<(
      std::basic_ostream<Character, CharacterTraits>& output_stream,
      ::pastel::geometry::point<dimension_, Value> const& point)
    {
      auto const point_data = point.data();
      output_stream << '(' << point_data[0u];
      for (auto index = std::size_t{1}; index < dimension_; ++index)
        output_stream << ',' << point_data[index];
      return output_stream << ')';
    }
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_POINT_IO_HPP

