#ifndef PASTEL_INTEGRATE_EULER_PREDICT_PARTICLES_HPP
# define PASTEL_INTEGRATE_EULER_PREDICT_PARTICLES_HPP

# include <cstddef>
# include <tuple>

# include <pastel/integrate/detail/update_positions.hpp>
# include <pastel/integrate/detail/update_velocities.hpp>
//# include <pastel/integrate/detail/update_orientations_order1.hpp>
//# include <pastel/integrate/detail/update_local_angular_velocities_order1.hpp>
# include <pastel/system/for_each_container.hpp>
//# include <pastel/container/modify_global_angular_velocities.hpp>


namespace pastel
{
  namespace integrate
  {
    namespace euler
    {
      namespace predict_particles_detail
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
      } // namespace predict_particles_detail


      // x_i(t+dt) = x_i(t) + v_i(t) dt + [f_i(t)/m_i] dt^2 / 2
      // v_i(t+dt) = v_i(t) + [f_i(t)/m_i] dt
      // f_i(t+dt) = \sum_j f(x_i(t+dt), v_i(t+dt), ..., x_j(t+dt), v_j(t+dt), ...)
      template <typename System, typename Time>
      inline void predict_particles(System& system, Time time_step)
      {
        ::pastel::system::for_each_container(
          system,
          ::pastel::integrate::euler::predict_particles_detail::update_positions<Time>{time_step});
        ::pastel::system::for_each_container(
          system,
          ::pastel::integrate::euler::predict_particles_detail::update_velocities<Time>{time_step});
        // update_orientations, update_local_angular_velocities, modify_global_angular_velocities
      }
    } // namespace euler
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_EULER_PREDICT_PARTICLES_HPP

