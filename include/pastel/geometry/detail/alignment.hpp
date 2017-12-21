#ifndef PASTEL_GEOMETRY_DETAIL_ALIGNMENT_HPP
# define PASTEL_GEOMETRY_DETAIL_ALIGNMENT_HPP

# include <cstddef>

# include <pastel/utility/next_power_of_two.hpp>


namespace pastel
{
  namespace geometry
  {
    namespace detail
    {
      template <std::size_t dimension_, typename Value>
      struct alignment
      {
        static constexpr std::size_t value
          = ::pastel::utility::next_power_of_two(alignof(Value))
            * ::pastel::utility::next_power_of_two(dimension_);
      };  // struct alignment<dimension_, Value>
    } // namespace detail
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_DETAIL_ALIGNMENT_HPP

