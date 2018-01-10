#ifndef PASTEL_SYSTEM_UPDATE_FORCES_HPP
# define PASTEL_SYSTEM_UPDATE_FORCES_HPP

# include <pastel/system/for_each_neighbor_list.hpp>
# include <pastel/system/for_each_boundary_neighbor_list.hpp>
# include <pastel/force/update_forces.hpp>
# include <pastel/force/update_boundary_forces.hpp>
# include <pastel/neighbor/meta/required_arguments_category_of.hpp>


namespace pastel
{
  namespace system
  {
    namespace update_forces_detail
    {
      struct update_forces
      {
        template <typename NeighborList, typename System>
        void operator()(NeighborList const& neighbor_list, System& system) const
        {
          this->call(
            neighbor_list, system,
            typename ::pastel::neighbor::meta::required_arguments_category_of<NeighborList>::type{});
        }

       private:
        template <typename NeighborList, typename System>
        void call(NeighborList const& neighbor_list, System& system, ::pastel::force::tags::requires_position) const
        { ::pastel::force::update_forces(neighbor_list.force(), neighbor_list, system); }

        template <typename NeighborList, typename System>
        void call(NeighborList const& neighbor_list, System& system, ::pastel::force::tags::requires_position_orientation) const
        { ::pastel::force::update_forces(neighbor_list.force(), neighbor_list, system); }
      }; // struct update_force

      struct update_boundary_forces
      {
        template <typename BoundaryNeighborList, typename System>
        void operator()(BoundaryNeighborList const& boundary_neighbor_list, System& system) const
        {
          this->call(
            boundary_neighbor_list, system,
            typename ::pastel::neighbor::meta::required_arguments_category_of<NeighborList>::type{});
        }

       private:
        template <typename BoundaryNeighborList, typename System>
        void call(BoundaryNeighborList const& boundary_neighbor_list, System& system, ::pastel::force::tags::requires_position) const
        { ::pastel::force::update_boundary_forces(boundary_neighbor_list.force(), boundary_neighbor_list, system); }

        template <typename BoundaryNeighborList, typename System>
        void call(BoundaryNeighborList const& boundary_neighbor_list, System& system, ::pastel::force::tags::requires_position_orientation) const
        { ::pastel::force::update_boundary_forces(boundary_neighbor_list.force(), boundary_neighbor_list, system); }
      }; // struct update_force
    } // namespace update_forces_detail

    template <typename System>
    inline void update_forces(System& system)
    {
      ::pastel::system::for_each_neighbor_list(
        system,
        ::pastel::system::update_forces_detail::update_forces());
      ::pastel::system::for_each_boundary_neighbor_list(
        system,
        ::pastel::system::update_forces_detail::update_boundary_forces());
    }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_UPDATE_FORCES_HPP

