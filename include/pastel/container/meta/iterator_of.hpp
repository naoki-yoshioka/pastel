#ifndef PASTEL_CONTAINER_META_ITERATOR_OF_HPP
# define PASTEL_CONTAINER_META_ITERATOR_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct iterator_of
      { using type = typename Particles::iterator; };

      template <typename Particles>
      struct iterator_of<Particles const>
      { using type = typename Particles::const_iterator; };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_ITERATOR_OF_HPP

