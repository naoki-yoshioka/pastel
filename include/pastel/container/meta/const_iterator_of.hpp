#ifndef PASTEL_CONTAINER_META_CONST_ITERATOR_OF_HPP
# define PASTEL_CONTAINER_META_CONST_ITERATOR_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct const_iterator_of
      { using type = typename Particles::const_iterator; };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_CONST_ITERATOR_OF_HPP

