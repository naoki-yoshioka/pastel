#ifndef PASTEL_NEIGHBOR_META_INTERACTION_PAIR_OF_HPP
# define PASTEL_NEIGHBOR_META_INTERACTION_PAIR_OF_HPP


namespace pastel
{
  namespace neighbor
  {
    namespace meta
    {
      template <typename NeighborList>
      struct interaction_pair_of
      { using type = typename NeighborList::interaction_pair; };
    } // namespace meta
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_META_INTERACTION_PAIR_OF_HPP

