#ifndef PASTEL_SYSTEM_META_BOUNDARY_NEIGHBOR_LIST_TUPLE_SIZE_OF_HPP
# define PASTEL_SYSTEM_META_BOUNDARY_NEIGHBOR_LIST_TUPLE_SIZE_OF_HPP

# include <cstddef>
# include <type_traits>


namespace pastel
{
  namespace system
  {
    namespace meta
    {
      template <typename System>
      struct boundary_neighbor_list_tuple_size_of
        : std::integral_constant<std::size_t, System::boundary_neighbor_list_tuple_size>
      { };
    } // namespace meta
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_META_BOUNDARY_NEIGHBOR_LIST_TUPLE_SIZE_OF_HPP

