#ifndef PASTEL_NEIGHBOR_PARTNER_END_HPP
# define PASTEL_NEIGHBOR_PARTNER_END_HPP

# include <pastel/neighbor/meta/size_of.hpp>


namespace pastel
{
  namespace neighbor
  {
    template <typename NeighborList>
    inline typename NeighborList::partner_const_iterator
    partner_end(
      NeighborList const& neighbor_list,
      typename ::pastel::neighbor::meta::size_of<NeighborList const>::type key)
    { return neighbor_list.partner_end(key); }
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_PARTNER_END_HPP

