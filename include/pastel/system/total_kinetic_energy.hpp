#ifndef PASTEL_SYSTEM_TOTAL_KINETIC_ENERGY_HPP
# define PASTEL_SYSTEM_TOTAL_KINETIC_ENERGY_HPP

# include <cstddef>

# include <pastel/system/particles.hpp>
# include <pastel/system/meta/scalar_of.hpp>
# include <pastel/container/total_kinetic_energy.hpp>


namespace pastel
{
  namespace system
  {
    template <std::size_t index, typename System>
    inline typename ::pastel::system::meta::scalar_of<index, System const>::type
    total_kinetic_energy(System const& system)
    { return ::pastel::container::total_kinetic_energy(::pastel::system::particles<index>(system)); }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_TOTAL_KINETIC_ENERGY_HPP

