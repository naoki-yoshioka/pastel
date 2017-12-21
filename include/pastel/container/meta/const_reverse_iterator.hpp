#ifndef PASTEL_CONTAINER_META_CONST_REVERSE_ITERATOR_OF_HPP
# define PASTEL_CONTAINER_META_CONST_REVERSE_ITERATOR_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct const_reverse_iterator_of
      { using type = typename Particles::const_reverse_iterator; };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_CONST_REVERSE_ITERATOR_OF_HPP

