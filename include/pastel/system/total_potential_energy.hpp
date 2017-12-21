#ifndef PASTEL_SYSTEM_TOTAL_POTENTIAL_ENERGY_HPP
# define PASTEL_SYSTEM_TOTAL_POTENTIAL_ENERGY_HPP

# include <cstddef>

# include <pastel/system/neighbor_list.hpp>
# include <pastel/system/meta/force_of.hpp>
# include <pastel/neighbor/total_potential_energy.hpp>
# include <pastel/force/meta/value_of.hpp>


namespace pastel
{
  namespace system
  {
    template <std::size_t index, typename System>
    inline typename ::pastel::force::meta::value_of<
      typename ::pastel::system::meta::force_of<index, System>::type>::type
    total_potential_energy(System const& system)
    { return ::pastel::neighbor::total_potential_energy(::pastel::system::neighbor_list<index>(system), system); }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_TOTAL_POTENTIAL_ENERGY_HPP

