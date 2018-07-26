#ifndef PASTEL_INTEGRATE_VVERLET_PREDICT_PARTICLES_HPP
# define PASTEL_INTEGRATE_VVERLET_PREDICT_PARTICLES_HPP

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
    namespace vverlet
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
      } // namespace predict_particles_detail


      // v_i(t+dt/2) = v_i(t) + [f_i(t)/m_i] (dt/2)
      // x_i(t+dt) = x_i(t) + v_i(t+dt/2) dt
      template <typename System, typename Time>
      inline void predict_particles(System& system, Time time_step)
      {
        auto const half_time_step = time_step / Time{2};
        ::pastel::system::for_each_container(
          system,
          ::pastel::integrate::vverlet::predict_particles_detail::update_velocities<Time>{half_time_step});
        // update_local_angular_velocities, modify_global_angular_velocities

        ::pastel::system::for_each_container(
          system,
          ::pastel::integrate::vverlet::predict_particles_detail::update_positions<Time>{time_step});
        // update_orientations
      }
    } // namespace vverlet
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_VVERLET_PREDICT_PARTICLES_HPP

