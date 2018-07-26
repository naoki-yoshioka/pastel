#ifndef PASTEL_INTEGRATE_VVERLET_UPDATE_HPP
# define PASTEL_INTEGRATE_VVERLET_UPDATE_HPP

# include <cstddef>

# include <pastel/integrate/vverlet/predict_particles.hpp>
# include <pastel/integrate/vverlet/correct_particles.hpp>
# include <pastel/integrate/detail/update.hpp>


namespace pastel
{
  namespace integrate
  {
    namespace vverlet
    {
      template <typename System, typename Time>
      inline void update(System& system, Time time_step)
      {
        ::pastel::integrate::detail::update(
          system, time_step,
          [](System& system, Time time_step)
          { ::pastel::integrate::vverlet::predict_particles(system, time_step); },
          [](System& system, Time time_step)
          { ::pastel::integrate::vverlet::correct_particles(system, time_step); });
      }
    } // namespace vverlet
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_VVERLET_UPDATE_HPP

