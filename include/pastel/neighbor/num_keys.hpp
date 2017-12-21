#ifndef PASTEL_NEIGHBOR_NUM_KEYS_HPP
# define PASTEL_NEIGHBOR_NUM_KEYS_HPP

# include <pastel/neighbor/meta/size_of.hpp>


namespace pastel
{
  namespace neighbor
  {
    template <typename NeighborList>
    inline typename ::pastel::neighbor::meta::size_of<NeighborList const>::type num_keys(NeighborList const& neighbor_list)
    { return neighbor_list.num_keys(); }
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_NUM_KEYS_HPP

