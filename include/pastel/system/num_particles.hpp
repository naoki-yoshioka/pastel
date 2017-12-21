#ifndef PASTEL_SYSTEM_NUM_PARTICLES_HPP
# define PASTEL_SYSTEM_NUM_PARTICLES_HPP

# include <cstddef>

# include <pastel/system/particles.hpp>
# include <pastel/system/meta/particles_of.hpp>
# include <pastel/container/num_particles.hpp>
# include <pastel/container/meta/size_of.hpp>


namespace pastel
{
  namespace system
  {
    template <std::size_t index, typename System>
    inline typename ::pastel::container::meta::size_of<
      typename ::pastel::system::meta::particles_of<index, System const>::type>::type
    num_particles(System const& system)
    { return ::pastel::container::num_particles(::pastel::system::particles(system)); }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_NUM_PARTICLES_HPP

