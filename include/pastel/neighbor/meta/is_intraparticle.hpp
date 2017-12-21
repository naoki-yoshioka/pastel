#ifndef PASTEL_NEIGHBOR_META_IS_INTRAPARTICLE_HPP
# define PASTEL_NEIGHBOR_META_IS_INTRAPARTICLE_HPP

# include <type_traits>


namespace pastel
{
  namespace neighbor
  {
    namespace meta
    {
      template <typename NeighborOrNeighborList>
      struct is_intraparticle
        : std::integral_constant<bool, NeighborOrNeighborList::is_intraparticle>
      { };
    } // namespace meta
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_META_IS_INTRAPARTICLE_HPP

