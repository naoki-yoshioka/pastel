#ifndef PASTEL_SYSTEM_META_PARTICLE_INDICES_FOR_BOUNDARY_OF_HPP
# define PASTEL_SYSTEM_META_PARTICLE_INDICES_FOR_BOUNDARY_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace system
  {
    namespace meta
    {
      template <std::size_t index, typename System>
      struct particle_indices_for_boundary_of
      { using type = typename System::template particle_indices_for_boundary_type<index>; };

      template <std::size_t index, typename System>
      struct particle_indices_for_boundary_of<index, System const>
      { using type = typename System::template particle_indices_for_boundary_type<index> const; };
    } // namespace meta
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_META_PARTICLE_INDICES_FOR_BOUNDARY_OF_HPP

