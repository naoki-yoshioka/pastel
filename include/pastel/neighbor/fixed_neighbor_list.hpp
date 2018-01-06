#ifndef PASTEL_NEIGHBOR_FIXED_NEIGHBOR_LIST_HPP
# define PASTEL_NEIGHBOR_FIXED_NEIGHBOR_LIST_HPP

# include <pastel/neighbor/neighbor_list.hpp>
# include <pastel/neighbor/no_updater.hpp>


namespace pastel
{
  namespace neighbor
  {
    template <
      typename Force,
      typename InteractionPair = ::pastel::utility::size_pair<0u, 0u>,
      typename IndexAllocator = std::allocator<std::size_t>>
    using fixed_neighbor_list
      = ::pastel::neighbor::neighbor_list<
          Force, InteractionPair, ::pastel::neighbor::no_updater, IndexAllocator>;
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_FIXED_NEIGHBOR_LIST_HPP

