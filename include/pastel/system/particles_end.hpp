#ifndef PASTEL_SYSTEM_PARTICLES_END_HPP
# define PASTEL_SYSTEM_PARTICLES_END_HPP

# include <cstddef>
# include <iterator>

# include <pastel/system/particles.hpp>
# include <pastel/system/meta/particles_of.hpp>
# include <pastel/container/meta/iterator_of.hpp>


namespace pastel
{
  namespace system
  {
    template <std::size_t index, typename System>
    inline typename ::pastel::container::meta::iterator_of<
      typename ::pastel::system::meta::particles_of<index, System>::type>::type
    particles_end(System& system)
    { return std::end(::pastel::system::particles<index>(system)); }

    template <std::size_t index, typename System>
    inline typename ::pastel::container::meta::iterator_of<
      typename ::pastel::system::meta::particles_of<index, System const>::type>::type
    particles_end(System const& system)
    { return std::end(::pastel::system::particles<index>(system)); }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_PARTICLES_END_HPP

