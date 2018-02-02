#ifndef PASTEL_NEIGHBOR_TOTAL_BOUNDARY_POTENTIAL_ENERGY_HPP
# define PASTEL_NEIGHBOR_TOTAL_BOUNDARY_POTENTIAL_ENERGY_HPP

# include <pastel/neighbor/meta/force_of.hpp>
# include <pastel/force/total_boundary_potential_energy.hpp>
# include <pastel/force/meta/value_of.hpp>


namespace pastel
{
  namespace neighbor
  {
    template <typename NeighborList, typename System>
    inline typename ::pastel::force::meta::value_of<typename ::pastel::neighbor::meta::force_of<NeighborList const>::type>::type
    total_boundary_potential_energy(NeighborList const& neighbor_list, System const& system)
    { return ::pastel::force::total_boundary_potential_energy(neighbor_list.force(), neighbor_list, system); }
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_TOTAL_BOUNDARY_POTENTIAL_ENERGY_HPP

