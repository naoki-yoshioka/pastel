#ifndef PASTEL_GEOMETRY_VECTOR_IO_HPP
# define PASTEL_GEOMETRY_VECTOR_IO_HPP

# include <cstddef>
# include <ostream>

# include <pastel/geometry/vector.hpp>


namespace pastel
{
  namespace geometry
  {
    template <typename Character, typename CharacterTraits, std::size_t dimension_, typename Value>
    std::basic_ostream<Character, CharacterTraits>& operator<<(
      std::basic_ostream<Character, CharacterTraits>& output_stream,
      ::pastel::geometry::vector<dimension_, Value> const& vector)
    {
      auto const vector_data = vector.data();
      output_stream << '[' << vector_data[0];
      for (auto index = std::size_t{1}; index < dimension_; ++index)
        output_stream << ',' << vector_data[index];
      return output_stream << ']';
    }
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_VECTOR_IO_HPP

