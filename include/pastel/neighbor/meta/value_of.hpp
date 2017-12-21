#ifndef PASTEL_NEIGHBOR_META_VALUE_OF_HPP
# define PASTEL_NEIGHBOR_META_VALUE_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace neighbor
  {
    namespace meta
    {
      template <typename NeighborList>
      struct value_of
      { using type = typename NeighborList::value_type; };
    } // namespace meta
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_META_VALUE_OF_HPP

