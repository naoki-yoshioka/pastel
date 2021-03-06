#ifndef PASTEL_SYSTEM_META_NEIGHBOR_LIST_OF_HPP
# define PASTEL_SYSTEM_META_NEIGHBOR_LIST_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace system
  {
    namespace meta
    {
      template <std::size_t index, typename System>
      struct neighbor_list_of
      { using type = typename System::template neighbor_list_type<index>; };

      template <std::size_t index, typename System>
      struct neighbor_list_of<index, System const>
      { using type = typename System::template neighbor_list_type<index> const; };
    } // namespace meta
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_META_NEIGHBOR_LIST_OF_HPP

