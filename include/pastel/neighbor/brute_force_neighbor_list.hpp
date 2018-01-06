#ifndef PASTEL_NEIGHBOR_BRUTE_FORCE_NEIGHBOR_LIST_HPP
# define PASTEL_NEIGHBOR_BRUTE_FORCE_NEIGHBOR_LIST_HPP

# include <pastel/neighbor/neighbor_list.hpp>
# include <pastel/neighbor/brute_force_updater.hpp>


namespace pastel
{
  namespace neighbor
  {
    template <
      typename Force,
      typename InteractionPair = ::pastel::utility::size_pair<0u, 0u>,
      typename IndexAllocator = std::allocator<std::size_t>>
    using brute_force_neighbor_list
      = ::pastel::neighbor::neighbor_list<
          Force, InteractionPair,
          ::pastel::neighbor::brute_force_updater<typename ::pastel::force::meta::value_of<Force>::type>,
          IndexAllocator>;
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_BRUTE_FORCE_NEIGHBOR_LIST_HPP

