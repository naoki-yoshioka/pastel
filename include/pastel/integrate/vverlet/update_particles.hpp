#ifndef PASTEL_INTEGRATE_VVERLET_UPDATE_PARTICLES_HPP
# define PASTEL_INTEGRATE_VVERLET_UPDATE_PARTICLES_HPP

# include <cstddef>
# include <tuple>

# include <pastel/integrate/detail/update_positions.hpp>
# include <pastel/integrate/detail/update_velocities.hpp>
//# include <pastel/integrate/detail/update_orientations_order1.hpp>
//# include <pastel/integrate/detail/update_local_angular_velocities_order1.hpp>
# include <pastel/system/for_each_container.hpp>
# include <pastel/system/update_forces.hpp>
# include <pastel/container/clear_forces.hpp>
# include <pastel/container/apply_external_forces.hpp>
//# include <pastel/container/modify_global_angular_velocities.hpp>
//# include <pastel/container/modify_local_torques.hpp>


namespace pastel
{
  namespace integrate
  {
    namespace vverlet
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
          { ::pastel::integrate::detail::update_positions<1u, std::tuple<>>(particles, time_step_); }
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


        struct apply_external_forces
        {
          template <typename Particles, typename ExternalForce>
          void operator()(Particles& particles, ExternalForce&& external_force) const
          { ::pastel::container::apply_external_forces(particles, std::forward<ExternalForce>(external_force)); }
        };
      } // namespace update_particles_detail


      // v_i(t+dt/2) = v_i(t) + [f_i(t)/m_i] (dt/2)
      // x_i(t+dt) = x_i(t) + v_i(t+dt/2) dt
      // f_i(t+dt) = \sum_j f(x_i(t+dt), ..., x_j(t+dt), ...)
      // v_i(t+dt) = v_i(t+dt/2) + [f_i(t+dt)/m_i] (dt/2)
      template <typename System, typename Time>
      inline void update_particles(System& system, Time time_step)
      {
        auto const half_time_step = time_step / Time{2};
        ::pastel::system::for_each_container(
          system,
          ::pastel::integrate::vverlet::update_particles_detail::update_velocities<Time>{half_time_step});
        // update_local_angular_velocities, modify_global_angular_velocities

        ::pastel::system::for_each_container(
          system,
          ::pastel::integrate::vverlet::update_particles_detail::update_positions<Time>{time_step});
        // update_orientations
        ::pastel::system::for_each_container(
          system,
          ::pastel::integrate::vverlet::update_particles_detail::clear_forces());
        ::pastel::system::update_forces(system);
        ::pastel::system::for_each_container(
          system,
          ::pastel::integrate::vverlet::update_particles_detail::apply_external_forces());
        // modify_local_torques

        ::pastel::system::for_each_container(
          system,
          ::pastel::integrate::vverlet::update_particles_detail::update_velocities<Time>{half_time_step});
        // update_local_angular_velocities, modify_global_angular_velocities
      }
    } // namespace vverlet
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_VVERLET_UPDATE_PARTICLES_HPP

