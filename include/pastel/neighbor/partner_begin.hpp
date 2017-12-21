#ifndef PASTEL_NEIGHBOR_PARTNER_BEGIN_HPP
# define PASTEL_NEIGHBOR_PARTNER_BEGIN_HPP

# include <pastel/neighbor/meta/size_of.hpp>


namespace pastel
{
  namespace neighbor
  {
    template <typename NeighborList>
    inline typename NeighborList::partner_const_iterator
    partner_begin(
      NeighborList const& neighbor_list,
      typename ::pastel::neighbor::meta::size_of<NeighborList const>::type key)
    { return neighbor_list.partner_begin(key); }
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_PARTNER_BEGIN_HPP

