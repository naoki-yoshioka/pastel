#ifndef PASTEL_NEIGHBOR_NUM_PARTNERS_HPP
# define PASTEL_NEIGHBOR_NUM_PARTNERS_HPP

# include <pastel/neighbor/meta/size_of.hpp>


namespace pastel
{
  namespace neighbor
  {
    template <typename NeighborList>
    inline typename ::pastel::neighbor::meta::size_of<NeighborList const>::type num_partners(
      NeighborList const& neighbor_list,
      typename ::pastel::neighbor::meta::size_of<NeighborList>::type key)
    { return neighbor_list.num_partners(key); }
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_NUM_PARTNERS_HPP

