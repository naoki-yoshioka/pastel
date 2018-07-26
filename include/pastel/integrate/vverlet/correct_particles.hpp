#ifndef PASTEL_INTEGRATE_VVERLET_CORRECT_PARTICLES_HPP
# define PASTEL_INTEGRATE_VVERLET_CORRECT_PARTICLES_HPP

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
      namespace correct_particles_detail
      {
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
      } // namespace correct_particles_detail


      // v_i(t+dt) = v_i(t+dt/2) + [f_i(t+dt)/m_i] (dt/2)
      template <typename System, typename Time>
      inline void correct_particles(System& system, Time time_step)
      {
        auto const half_time_step = time_step / Time{2};
        ::pastel::system::for_each_container(
          system,
          ::pastel::integrate::vverlet::correct_particles_detail::update_velocities<Time>{half_time_step});
        // update_local_angular_velocities, modify_global_angular_velocities
      }
    } // namespace vverlet
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_VVERLET_CORRECT_PARTICLES_HPP

