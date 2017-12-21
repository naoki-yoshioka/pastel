#ifndef PASTEL_SYSTEM_NEIGHBOR_LIST_HPP
# define PASTEL_SYSTEM_NEIGHBOR_LIST_HPP

# include <cstddef>
# include <utility>

# include <pastel/system/meta/neighbor_list_of.hpp>


namespace pastel
{
  namespace system
  {
    namespace dispatch
    {
      template <std::size_t index, typename System>
      struct call_neighbor_list
      {
        static typename ::pastel::system::meta::neighbor_list_of<index, System const>::type const& call(System const& system)
        { return system.template neighbor_list<index>(); }

        static typename ::pastel::system::meta::neighbor_list_of<index, System>::type& call(System& system)
        { return system.template neighbor_list<index>(); }

        template <typename NeighborList>
        static void call(System& system, NeighborList&& new_neighbor_list)
        { system.template neighbor_list<index>(std::forward<NeighborList>(new_neighbor_list)); }
      }; // struct call_neighbor_list<index, System>
    } // namespace dispatch

    template <std::size_t index, typename System>
    inline typename ::pastel::system::meta::neighbor_list_of<index, System const>::type const& neighbor_list(System const& system)
    { return ::pastel::system::dispatch::call_neighbor_list<index, System>::call(system); }

    template <std::size_t index, typename System>
    inline typename ::pastel::system::meta::neighbor_list_of<index, System>::type& neighbor_list(System& system)
    { return ::pastel::system::dispatch::call_neighbor_list<index, System>::call(system); }

    template <std::size_t index, typename System, typename NeighborList>
    inline void neighbor_list(System& system, NeighborList&& new_neighbor_list)
    { ::pastel::system::dispatch::call_neighbor_list<index, System>::call(system, std::forward<NeighborList>(new_neighbor_list)); }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_NEIGHBOR_LIST_HPP

