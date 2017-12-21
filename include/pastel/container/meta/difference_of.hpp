#ifndef PASTEL_CONTAINER_META_DIFFERENCE_OF_HPP
# define PASTEL_CONTAINER_META_DIFFERENCE_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct difference_of
      { using type = typename Particles::difference_type; };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_DIFFERENCE_OF_HPP

