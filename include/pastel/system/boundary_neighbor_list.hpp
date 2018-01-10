#ifndef PASTEL_SYSTEM_BOUNDARY_NEIGHBOR_LIST_HPP
# define PASTEL_SYSTEM_BOUNDARY_NEIGHBOR_LIST_HPP

# include <cstddef>
# include <utility>

# include <pastel/system/meta/boundary_neighbor_list_of.hpp>


namespace pastel
{
  namespace system
  {
    namespace dispatch
    {
      template <std::size_t index, typename System>
      struct call_boundary_neighbor_list
      {
        static typename ::pastel::system::meta::boundary_neighbor_list_of<index, System const>::type const& call(System const& system)
        { return system.template boundary_neighbor_list<index>(); }

        static typename ::pastel::system::meta::boundary_neighbor_list_of<index, System>::type& call(System& system)
        { return system.template boundary_neighbor_list<index>(); }

        template <typename BoundaryNeighborList>
        static void call(System& system, BoundaryNeighborList&& new_boundary_neighbor_list)
        { system.template boundary_neighbor_list<index>(std::forward<BoundaryNeighborList>(new_boundary_neighbor_list)); }
      }; // struct call_boundary_neighbor_list<index, System>
    } // namespace dispatch

    template <std::size_t index, typename System>
    inline typename ::pastel::system::meta::boundary_neighbor_list_of<index, System const>::type const& boundary_neighbor_list(System const& system)
    { return ::pastel::system::dispatch::call_boundary_neighbor_list<index, System>::call(system); }

    template <std::size_t index, typename System>
    inline typename ::pastel::system::meta::boundary_neighbor_list_of<index, System>::type& boundary_neighbor_list(System& system)
    { return ::pastel::system::dispatch::call_boundary_neighbor_list<index, System>::call(system); }

    template <std::size_t index, typename System, typename BoundaryNeighborList>
    inline void boundary_neighbor_list(System& system, BoundaryNeighborList&& new_boundary_neighbor_list)
    { ::pastel::system::dispatch::call_boundary_neighbor_list<index, System>::call(system, std::forward<BoundaryNeighborList>(new_boundary_neighbor_list)); }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_BOUNDARY_NEIGHBOR_LIST_HPP

