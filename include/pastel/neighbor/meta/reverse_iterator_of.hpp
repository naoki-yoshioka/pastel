#ifndef PASTEL_NEIGHBOR_META_REVERSE_ITERATOR_OF_HPP
# define PASTEL_NEIGHBOR_META_REVERSE_ITERATOR_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace neighbor
  {
    namespace meta
    {
      template <typename NeighborList>
      struct reverse_iterator_of
      { using type = typename NeighborList::reverse_iterator; };

      template <typename NeighborList>
      struct reverse_iterator_of<NeighborList const>
      { using type = typename NeighborList::const_reverse_iterator; };
    } // namespace meta
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_META_REVERSE_ITERATOR_OF_HPP

