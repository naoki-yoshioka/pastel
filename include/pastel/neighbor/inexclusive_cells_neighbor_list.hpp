#ifndef PASTEL_NEIGHBOR_INEXCLUSIVE_CELLS_NEIGHBOR_LIST_HPP
# define PASTEL_NEIGHBOR_INEXCLUSIVE_CELLS_NEIGHBOR_LIST_HPP

# include <pastel/neighbor/neighbor_list.hpp>
# include <pastel/neighbor/inexclusive_cells_updater.hpp>


namespace pastel
{
  namespace neighbor
  {
    template <
      typename Force, typename Position,
      typename InteractionPair = ::pastel::utility::size_pair<0u, 0u>,
      typename IndexAllocator = std::allocator<std::size_t>>
    using inexclusive_cells_neighbor_list
      = ::pastel::neighbor::neighbor_list<
          Force,
          ::pastel::neighbor::inexclusive_cells_updater<Position>,
          InteractionPair, IndexAllocator>;
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_BRUTE_FORCE_NEIGHBOR_LIST_HPP

