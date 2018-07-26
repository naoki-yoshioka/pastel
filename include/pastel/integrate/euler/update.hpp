#ifndef PASTEL_INTEGRATE_EULER_UPDATE_HPP
# define PASTEL_INTEGRATE_EULER_UPDATE_HPP

# include <cstddef>

# include <pastel/integrate/euler/predict_particles.hpp>
# include <pastel/integrate/detail/update.hpp>


namespace pastel
{
  namespace integrate
  {
    namespace euler
    {
      template <typename System, typename Time>
      inline void update(System& system, Time time_step)
      {
        ::pastel::integrate::detail::update(
          system, time_step,
          [](System& system, Time time_step)
          { ::pastel::integrate::euler::predict_particles(system, time_step); },
          [](System&, Time) { });
      }
    } // namespace euler
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_EULER_UPDATE_HPP

