#ifndef PASTEL_INTEGRATE_DETAIL_UPDATE_HPP
# define PASTEL_INTEGRATE_DETAIL_UPDATE_HPP

# include <cstddef>

# include <pastel/system/for_each_neighbor_list.hpp>
# include <pastel/neighbor/update_if_needed.hpp>


namespace pastel
{
  namespace integrate
  {
    namespace detail
    {
      namespace update_detail
      {
        template <typename Time>
        struct update_neighbor_list
        {
         private:
          Time time_step_;

         public:
          update_neighbor_list(Time time_step) : time_step_{time_step} { }

          template <typename NeighborList, typename System>
          void operator()(NeighborList& neighbor_list, System const& system) const
          { ::pastel::neighbor::update_if_needed(neighbor_list, system, time_step_); }
        }; // struct update_neighbor_list<Time>
      } // namespace update_detail


      template <typename System, typename Time, typename Function>
      inline void update(System& system, Time time_step, Function&& update_particles)
      {
        ::pastel::system::for_each_neighbor_list(
          system,
          ::pastel::integrate::detail::update_detail::update_neighbor_list<Time>{time_step});

        update_particles(system, time_step);

        // boundary conditions, mpi communications
      }
    } // namespace detail
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_DETAIL_UPDATE_HPP

