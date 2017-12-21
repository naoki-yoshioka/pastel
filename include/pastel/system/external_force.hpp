#ifndef PASTEL_SYSTEM_EXTERNAL_FORCE_HPP
# define PASTEL_SYSTEM_EXTERNAL_FORCE_HPP

# include <cstddef>
# include <utility>

# include <pastel/system/meta/external_force_of.hpp>


namespace pastel
{
  namespace system
  {
    namespace dispatch
    {
      template <std::size_t index, typename System>
      struct call_external_force
      {
        static typename ::pastel::system::meta::external_force_of<index, System const>::type const& call(System const& system)
        { return system.template external_force<index>(); }

        static typename ::pastel::system::meta::external_force_of<index, System>::type& call(System& system)
        { return system.template external_force<index>(); }

        template <typename ExternalForce>
        static void call(System& system, ExternalForce&& new_external_force)
        { system.template external_force<index>(std::forward<ExternalForce>(new_external_force)); }
      }; // struct call_external_force<index, System>
    } // namespace dispatch

    template <std::size_t index, typename System>
    inline typename ::pastel::system::meta::external_force_of<index, System const>::type const& external_force(System const& system)
    { return ::pastel::system::dispatch::call_external_force<index, System>::call(system); }

    template <std::size_t index, typename System>
    inline typename ::pastel::system::meta::external_force_of<index, System>::type& external_force(System& system)
    { return ::pastel::system::dispatch::call_external_force<index, System>::call(system); }

    template <std::size_t index, typename System, typename ExternalForce>
    inline void external_force(System& system, ExternalForce&& new_external_force)
    { ::pastel::system::dispatch::call_external_force<index, System>::call(system, std::forward<ExternalForce>(new_external_force)); }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_EXTERNAL_FORCE_HPP

