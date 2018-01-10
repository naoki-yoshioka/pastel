#ifndef PASTEL_SYSTEM_META_BOUNDARY_PARTICLES_OF_HPP
# define PASTEL_SYSTEM_META_BOUNDARY_PARTICLES_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace system
  {
    namespace meta
    {
      template <std::size_t index, typename System>
      struct boundary_particles_of
      { using type = typename System::template boundary_particles_type<index>; };

      template <std::size_t index, typename System>
      struct boundary_particles_of<index, System const>
      { using type = typename System::template boundary_particles_type<index> const; };
    } // namespace meta
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_META_BOUNDARY_PARTICLES_OF_HPP

