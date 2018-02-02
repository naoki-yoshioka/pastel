#ifndef PASTEL_SYSTEM_META_BOUNDARY_PARTICLE_INDICES_OF_HPP
# define PASTEL_SYSTEM_META_BOUNDARY_PARTICLE_INDICES_OF_HPP


namespace pastel
{
  namespace system
  {
    namespace meta
    {
      template <typename System>
      struct boundary_particle_indices_of
      { using type = typename System::boundary_particle_indices_type; };
    } // namessace meta
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_META_BOUNDARY_PARTICLE_INDICES_OF_HPP

