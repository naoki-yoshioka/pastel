#ifndef PASTEL_CONTAINER_META_REVERSE_ITERATOR_OF_HPP
# define PASTEL_CONTAINER_META_REVERSE_ITERATOR_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct reverse_iterator_of
      { using type = typename Particles::reverse_iterator; };

      template <typename Particles>
      struct reverse_iterator_of<Particles const>
      { using type = typename Particles::const_reverse_iterator; };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_REVERSE_ITERATOR_OF_HPP

