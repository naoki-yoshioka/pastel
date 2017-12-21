#ifndef PASTEL_SYSTEM_PARTICLES_HPP
# define PASTEL_SYSTEM_PARTICLES_HPP

# include <cstddef>
# include <utility>

# include <pastel/system/meta/particles_of.hpp>


namespace pastel
{
  namespace system
  {
    namespace dispatch
    {
      template <std::size_t index, typename System>
      struct call_particles
      {
        static typename ::pastel::system::meta::particles_of<index, System const>::type const& call(System const& system)
        { return system.template particles<index>(); }

        static typename ::pastel::system::meta::particles_of<index, System>::type& call(System& system)
        { return system.template particles<index>(); }

        template <typename Particles>
        static void call(System& system, Particles&& new_particles)
        { system.template particles<index>(std::forward<Particles>(new_particles)); }
      }; // struct call_particles<index, System>
    } // namespace dispatch

    template <std::size_t index, typename System>
    inline typename ::pastel::system::meta::particles_of<index, System const>::type const& particles(System const& system)
    { return ::pastel::system::dispatch::call_particles<index, System>::call(system); }

    template <std::size_t index, typename System>
    inline typename ::pastel::system::meta::particles_of<index, System>::type& particles(System& system)
    { return ::pastel::system::dispatch::call_particles<index, System>::call(system); }

    template <std::size_t index, typename System, typename Particles>
    inline void particles(System& system, Particles&& new_particles)
    { ::pastel::system::dispatch::call_particles<index, System>::call(system, std::forward<Particles>(new_particles)); }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_PARTICLES_HPP

