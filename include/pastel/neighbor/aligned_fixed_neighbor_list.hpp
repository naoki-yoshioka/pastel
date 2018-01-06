#ifndef PASTEL_NEIGHBOR_ALIGNED_FIXED_NEIGHBOR_LIST_HPP
# define PASTEL_NEIGHBOR_ALIGNED_FIXED_NEIGHBOR_LIST_HPP

# include <boost/align/aligned_allocator.hpp>

# include <pastel/neighbor/fixed_neighbor_list.hpp>


namespace pastel
{
  namespace neighbor
  {
    template <typename Force, typename InteractionPair = ::pastel::utility::size_pair<0u, 0u>>
    using aligned_fixed_neighbor_list
      = ::pastel::neighbor::fixed_neighbor_list<
          Force, InteractionPair, boost::alignment::aligned_allocator<std::size_t>>;
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_ALIGNED_FIXED_NEIGHBOR_LIST_HPP

