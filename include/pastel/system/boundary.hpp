#ifndef PASTEL_SYSTEM_BOUNDARY_HPP
# define PASTEL_SYSTEM_BOUNDARY_HPP

# include <pastel/system/meta/boundary_of.hpp>


namespace pastel
{
  namespace system
  {
    template <typename System>
    inline typename ::pastel::system::meta::boundary_of<System const>::type const& boundary(System const& system)
    { return system.boundary(); }

    template <typename System>
    inline typename ::pastel::system::meta::boundary_of<System>::type& boundary(System& system)
    { return system.boundary(); }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_BOUNDARY_HPP

