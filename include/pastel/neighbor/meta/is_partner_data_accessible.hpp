#ifndef PASTEL_NEIGHBOR_META_IS_PARTNER_DATA_ACCESSIBLE_HPP
# define PASTEL_NEIGHBOR_META_IS_PARTNER_DATA_ACCESSIBLE_HPP

# include <type_traits>


namespace pastel
{
  namespace neighbor
  {
    namespace meta
    {
      template <typename NeighborList>
      struct is_partner_data_accessible
        : std::integral_constant<bool, NeighborList::is_partner_data_accessible>
      { };
    } // namespace meta
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_META_IS_PARTNER_DATA_ACCESSIBLE_HPP

