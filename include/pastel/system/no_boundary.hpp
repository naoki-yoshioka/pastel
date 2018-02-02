#ifndef PASTEL_SYSTEM_NO_BOUNDARY_HPP
# define PASTEL_SYSTEM_NO_BOUNDARY_HPP

# include <cstddef>


namespace pastel
{
  namespace system
  {
    struct no_boundary
    {
      template <std::size_t dimension, std::size_t index, typename System>
      constexpr void modify_outside_particles(System const&) const
      { }

      template <std::size_t dimension, std::size_t index, typename System>
      constexpr void initialize_boundary_particles(System const&) const
      { }

      template <std::size_t dimension, std::size_t index, typename System>
      constexpr void update_boundary_particles(System const&) const
      { }
    }; // struct no_boundary
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_NO_BOUNDARY_HPP

