#ifndef PASTEL_GEOMETRY_GET_HPP
# define PASTEL_GEOMETRY_GET_HPP

# include <cstddef>
# include <utility>


namespace pastel
{
  namespace geometry
  {
    template <std::size_t index, typename T>
    inline constexpr auto get(T&& t) -> decltype(std::forward<T>(t).template get<index>())
    { return std::forward<T>(t).template get<index>(); }
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_GET_HPP

