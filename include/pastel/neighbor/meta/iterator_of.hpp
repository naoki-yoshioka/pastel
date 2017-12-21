#ifndef PASTEL_NEIGHBOR_META_ITERATOR_OF_HPP
# define PASTEL_NEIGHBOR_META_ITERATOR_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace neighbor
  {
    namespace meta
    {
      template <typename NeighborList>
      struct iterator_of
      { using type = typename NeighborList::iterator; };

      template <typename NeighborList>
      struct iterator_of<NeighborList const>
      { using type = typename NeighborList::const_iterator; };
    } // namespace meta
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_META_ITERATOR_OF_HPP

