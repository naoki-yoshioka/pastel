#ifndef PASTEL_NEIGHBOR_UPDATED_IF_NEEDED_HPP
# define PASTEL_NEIGHBOR_UPDATED_IF_NEEDED_HPP


namespace pastel
{
  namespace neighbor
  {
    namespace dispatch
    {
      template <typename NeighborList, typename System, typename Time>
      struct update_if_needed
      {
        static void call(NeighborList& neighbor_list, System const& system, Time time_step)
        { neighbor_list.update_if_needed(system, time_step); }
      }; // struct update_if_needed<NeighborList, System, Time>
    } // namespace dispatch

    template <typename NeighborList, typename System, typename Time>
    inline void update_if_needed(NeighborList& neighbor_list, System const& system, Time time_step)
    { ::pastel::neighbor::dispatch::update_if_needed<NeighborList, System, Time>::call(neighbor_list, system, time_step); }
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_UPDATED_IF_NEEDED_HPP

