#ifndef PASTEL_SYSTEM_FORCE_HPP
# define PASTEL_SYSTEM_FORCE_HPP

# include <cstddef>

# include <pastel/system/neighbor_list.hpp>
# include <pastel/system/meta/force_of.hpp>
# include <pastel/neighbor/force.hpp>


namespace pastel
{
  namespace system
  {
    template <std::size_t index, typename System>
    inline typename ::pastel::system::meta::force_of<index, System const>::type const& force(System const& system)
    { return ::pastel::neighbor::force(::pastel::system::neighbor_list<index>(system)); }

    template <std::size_t index, typename System>
    inline typename ::pastel::system::meta::force_of<index, System>::type& force(System& system)
    { return ::pastel::neighbor::force(::pastel::system::neighbor_list<index>(system)); }

    template <std::size_t index, typename System, typename Force>
    inline void force(System& system, Force&& new_force)
    { ::pastel::neighbor::force(::pastel::system::neighbor_list<index>(system), std::forward<Force>(new_force)); }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_FORCE_HPP

