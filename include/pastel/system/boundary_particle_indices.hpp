#ifndef PASTEL_SYSTEM_BOUNDARY_PARTICLE_INDICES_HPP
# define PASTEL_SYSTEM_BOUNDARY_PARTICLE_INDICES_HPP

# include <cstddef>

# include <pastel/system/meta/boundary_particle_indices_of.hpp>


namespace pastel
{
  namespace system
  {
    namespace dispatch
    {
      template <std::size_t index, std::size_t dimension, typename System>
      struct call_boundary_particle_indices
      {
        static typename ::pastel::system::meta::boundary_particle_indices_of<System const>::type const& call(System const& system)
        { return system.template boundary_particle_indices<index, dimension>(); }

        static typename ::pastel::system::meta::boundary_particle_indices_of<System>::type& call(System& system)
        { return system.template boundary_particle_indices<index, dimension>(); }
      }; // struct call_boundary_particle_indices<index, dimension, System>
    } // namespace dispatch

    template <std::size_t index, std::size_t dimension, typename System>
    inline typename ::pastel::system::meta::boundary_particle_indices_of<System const>::type const& boundary_particle_indices(System const& system)
    { return ::pastel::system::dispatch::call_boundary_particle_indices<index, dimension, System>::call(system); }

    template <std::size_t index, std::size_t dimension, typename System>
    inline typename ::pastel::system::meta::boundary_particle_indices_of<System>::type& boundary_particle_indices(System& system)
    { return ::pastel::system::dispatch::call_boundary_particle_indices<index, dimension, System>::call(system); }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_BOUNDARY_PARTICLE_INDICES_HPP

