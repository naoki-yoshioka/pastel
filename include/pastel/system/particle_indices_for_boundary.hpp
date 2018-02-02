#ifndef PASTEL_SYSTEM_PARTICLE_INDICES_FOR_BOUNDARY_HPP
# define PASTEL_SYSTEM_PARTICLE_INDICES_FOR_BOUNDARY_HPP

# include <cstddef>

# include <pastel/system/meta/particle_indices_for_boundary_of.hpp>


namespace pastel
{
  namespace system
  {
    namespace dispatch
    {
      template <std::size_t index, typename System>
      struct call_particle_indices_for_boundary
      {
        static typename ::pastel::system::meta::particle_indices_for_boundary_of<index, System const>::type const& call(System const& system)
        { return system.template particle_indices_for_boundary<index>(); }

        static typename ::pastel::system::meta::particle_indices_for_boundary_of<index, System>::type& call(System& system)
        { return system.template particle_indices_for_boundary<index>(); }
      }; // struct call_particle_indices_for_boundary<index, System>
    } // namespace dispatch

    template <std::size_t index, typename System>
    inline typename ::pastel::system::meta::particle_indices_for_boundary_of<index, System const>::type const& particle_indices_for_boundary(System const& system)
    { return ::pastel::system::dispatch::call_particle_indices_for_boundary<index, System>::call(system); }

    template <std::size_t index, typename System>
    inline typename ::pastel::system::meta::particle_indices_for_boundary_of<index, System>::type& particle_indices_for_boundary(System& system)
    { return ::pastel::system::dispatch::call_particle_indices_for_boundary<index, System>::call(system); }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_PARTICLE_INDICES_FOR_BOUNDARY_HPP

