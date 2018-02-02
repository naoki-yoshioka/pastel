#ifndef PASTEL_NEIGHBOR_META_BOUNDARY_NEIGHBOR_LIST_OF_HPP
# define PASTEL_NEIGHBOR_META_BOUNDARY_NEIGHBOR_LIST_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace neighbor
  {
    namespace meta
    {
      template <typename NeighborList, std::size_t boundary_partner>
      struct boundary_neighbor_list_of
      { using type = typename NeighborList::template boundary_neighbor_list_type<boundary_partner>; };
    } // namespace meta
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_META_BOUNDARY_NEIGHBOR_LIST_OF_HPP

