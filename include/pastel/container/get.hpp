#ifndef PASTEL_CONTAINER_GET_HPP
# define PASTEL_CONTAINER_GET_HPP

# include <cstddef>
# include <utility>


namespace pastel
{
  namespace container
  {
    template <typename Tag, typename Particles>
    inline auto get(Particles&& particles, std::size_t index) -> decltype(std::forward<Particles>(particles).template get<Tag>(index))
    { return std::forward<Particles>(particles).template get<Tag>(index); }
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_GET_HPP

