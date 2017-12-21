#ifndef PASTEL_INTEGRATE_EULER_UPDATE_PARTICLES_HPP
# define PASTEL_INTEGRATE_EULER_UPDATE_PARTICLES_HPP

# include <cstddef>
# include <tuple>

# include <pastel/integrate/detail/update_positions.hpp>
# include <pastel/integrate/detail/update_velocities.hpp>
//# include <pastel/integrate/detail/update_orientations_order1.hpp>
//# include <pastel/integrate/detail/update_local_angular_velocities_order1.hpp>
# include <pastel/system/for_each.hpp>
# include <pastel/system/for_each_neighbor_list.hpp>
# include <pastel/container/clear_forces.hpp>
# include <pastel/container/apply_external_forces.hpp>
//# include <pastel/container/modify_global_angular_velocities.hpp>
//# include <pastel/container/modify_local_torques.hpp>
# include <pastel/force/update_forces.hpp>


namespace pastel
{
  namespace integrate
  {
    namespace euler
    {
      namespace update_particles_detail
      {
        template <typename Time>
        struct update_positions
        {
         private:
          Time time_step_;

         public:
          update_positions(Time time_step) : time_step_{time_step} { }

          template <typename Particles, typename ExternalForce>
          void operator()(Particles& particles, ExternalForce&&) const
          { ::pastel::integrate::detail::update_positions<2u, std::tuple<>>(particles, time_step_); }
        }; // struct update_positions<Time>


        template <typename Time>
        struct update_velocities
        {
         private:
          Time time_step_;

         public:
          update_velocities(Time time_step) : time_step_{time_step} { }

          template <typename Particles, typename ExternalForce>
          void operator()(Particles& particles, ExternalForce&&) const
          { ::pastel::integrate::detail::update_velocities<1u, std::tuple<>>(particles, time_step_); }
        }; // struct update_velocities<Time>


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
          { ::pastel::force::update_forces(neighbor_list.force(), neighbor_list, system); }
        }; // struct update_force


        struct apply_external_forces
        {
          template <typename Particles, typename ExternalForce>
          void operator()(Particles& particles, ExternalForce&& external_force) const
          { ::pastel::container::apply_external_forces(particles, std::forward<ExternalForce>(external_force)); }
        };
      } // namespace update_particles_detail


      // x_i(t+dt) = x_i(t) + v_i(t) dt + [f_i(t)/m_i] dt^2 / 2
      // v_i(t+dt) = v_i(t) + [f_i(t)/m_i] dt
      // f_i(t+dt) = \sum_j f(x_i(t+dt), v_i(t+dt), ..., x_j(t+dt), v_j(t+dt), ...)
      template <typename System, typename Time>
      inline void update_particles(System& system, Time time_step)
      {
        ::pastel::system::for_each(
          system,
          ::pastel::integrate::euler::update_particles_detail::update_positions<Time>{time_step});
        ::pastel::system::for_each(
          system,
          ::pastel::integrate::euler::update_particles_detail::update_velocities<Time>{time_step});
        // update_orientations, update_local_angular_velocities, modify_global_angular_velocities
        ::pastel::system::for_each(
          system,
          ::pastel::integrate::euler::update_particles_detail::clear_forces());
        ::pastel::system::for_each_neighbor_list(
          system,
          ::pastel::integrate::euler::update_particles_detail::update_forces());
        ::pastel::system::for_each(
          system,
          ::pastel::integrate::euler::update_particles_detail::apply_external_forces());
        // modify_local_torques
      }
    } // namespace euler
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_EULER_UPDATE_PARTICLES_HPP

