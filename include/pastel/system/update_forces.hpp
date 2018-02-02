#ifndef PASTEL_SYSTEM_UPDATE_FORCES_HPP
# define PASTEL_SYSTEM_UPDATE_FORCES_HPP

# include <pastel/system/for_each_container.hpp>
# include <pastel/system/for_each_neighbor_list.hpp>
# include <pastel/system/for_each_boundary_neighbor_list.hpp>
# include <pastel/force/update_forces.hpp>
# include <pastel/force/update_boundary_forces.hpp>
# include <pastel/container/clear_forces.hpp>
# include <pastel/container/apply_external_forces.hpp>
# include <pastel/neighbor/meta/required_arguments_category_of.hpp>


namespace pastel
{
  namespace system
  {
    namespace update_forces_detail
    {
      struct clear_forces
      {
        template <typename Particles, typename ExternalForce>
        void operator()(Particles& particles, ExternalForce&&) const
        { ::pastel::container::clear_forces(particles); }
      }; // struct clear_forces


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
            typename ::pastel::neighbor::meta::required_arguments_category_of<BoundaryNeighborList>::type{});
        }

       private:
        template <typename BoundaryNeighborList, typename System>
        void call(BoundaryNeighborList const& boundary_neighbor_list, System& system, ::pastel::force::tags::requires_position) const
        { ::pastel::force::update_boundary_forces(boundary_neighbor_list.force(), boundary_neighbor_list, system); }

        template <typename BoundaryNeighborList, typename System>
        void call(BoundaryNeighborList const& boundary_neighbor_list, System& system, ::pastel::force::tags::requires_position_orientation) const
        { ::pastel::force::update_boundary_forces(boundary_neighbor_list.force(), boundary_neighbor_list, system); }
      }; // struct update_boundary_force


      struct apply_external_forces
      {
        template <typename Particles, typename ExternalForce>
        void operator()(Particles& particles, ExternalForce&& external_force) const
        { ::pastel::container::apply_external_forces(particles, std::forward<ExternalForce>(external_force)); }
      }; // struct apply_external_forces
    } // namespace update_forces_detail

    template <typename System>
    inline void update_forces(System& system)
    {
      ::pastel::system::for_each_container(
        system,
        ::pastel::system::update_forces_detail::clear_forces{});
      ::pastel::system::for_each_neighbor_list(
        system,
        ::pastel::system::update_forces_detail::update_forces{});
      ::pastel::system::for_each_boundary_neighbor_list(
        system,
        ::pastel::system::update_forces_detail::update_boundary_forces{});
      ::pastel::system::for_each_container(
        system,
        ::pastel::system::update_forces_detail::apply_external_forces{});
    }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_UPDATE_FORCES_HPP

