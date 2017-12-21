#ifndef PASTEL_NEIGHBOR_UPDATE_FORCES_HPP
# define PASTEL_NEIGHBOR_UPDATE_FORCES_HPP

# include <pastel/force/update_forces.hpp>


namespace pastel
{
  namespace neighbor
  {
    template <typename NeighborList, typename System>
    inline void update_forces(NeighborList const& neighbor_list, System& system)
    { ::pastel::force::update_forces(neighbor_list.force(), neighbor_list, system); }
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_UPDATE_FORCES_HPP

