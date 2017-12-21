#ifndef PASTEL_NEIGHBOR_FORCE_HPP
# define PASTEL_NEIGHBOR_FORCE_HPP

# include <utility>

# include <pastel/neighbor/meta/force_of.hpp>


namespace pastel
{
  namespace neighbor
  {
    template <typename NeighborList>
    inline typename ::pastel::neighbor::meta::force_of<NeighborList const>::type const&
    force(NeighborList const& neighbor_list)
    { return neighbor_list.force(); }

    template <typename NeighborList>
    inline typename ::pastel::neighbor::meta::force_of<NeighborList>::type&
    force(NeighborList& neighbor_list)
    { return neighbor_list.force(); }

    template <typename NeighborList, typename Force>
    inline void force(NeighborList& neighbor_list, Force&& new_force)
    { neighbor_list.force(std::forward<Force>(new_force)); }
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_FORCE_HPP

