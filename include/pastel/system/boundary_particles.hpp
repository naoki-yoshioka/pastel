#ifndef PASTEL_SYSTEM_BOUNDARY_PARTICLES_HPP
# define PASTEL_SYSTEM_BOUNDARY_PARTICLES_HPP

# include <cstddef>
# include <utility>

# include <pastel/system/meta/boundary_particles_of.hpp>


namespace pastel
{
  namespace system
  {
    namespace dispatch
    {
      template <std::size_t index, typename System>
      struct call_boundary_particles
      {
        static typename ::pastel::system::meta::boundary_particles_of<index, System const>::type const& call(System const& system)
        { return system.template boundary_particles<index>(); }

        static typename ::pastel::system::meta::boundary_particles_of<index, System>::type& call(System& system)
        { return system.template boundary_particles<index>(); }

        template <typename BoundaryParticles>
        static void call(System& system, BoundaryParticles&& new_boundary_particles)
        { system.template boundary_particles<index>(std::forward<BoundaryParticles>(new_boundary_particles)); }
      }; // struct call_boundary_particles<index, System>
    } // namespace dispatch

    template <std::size_t index, typename System>
    inline typename ::pastel::system::meta::boundary_particles_of<index, System const>::type const& boundary_particles(System const& system)
    { return ::pastel::system::dispatch::call_boundary_particles<index, System>::call(system); }

    template <std::size_t index, typename System>
    inline typename ::pastel::system::meta::boundary_particles_of<index, System>::type& boundary_particles(System& system)
    { return ::pastel::system::dispatch::call_boundary_particles<index, System>::call(system); }

    template <std::size_t index, typename System, typename BoundaryParticles>
    inline void boundary_particles(System& system, BoundaryParticles&& new_boundary_particles)
    { ::pastel::system::dispatch::call_boundary_particles<index, System>::call(system, std::forward<BoundaryParticles>(new_boundary_particles)); }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_BOUNDARY_PARTICLES_HPP

