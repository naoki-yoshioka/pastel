#ifndef PASTEL_EXTFORCE_META_IS_SPH_EXTERNAL_FORCE_HPP
# define PASTEL_EXTFORCE_META_IS_SPH_EXTERNAL_FORCE_HPP

# include <type_traits>


namespace pastel
{
  namespace extforce
  {
    namespace meta
    {
      template <typename ExternalForce>
      struct is_sph_external_force
        : std::integral_constant<bool, ExternalForce::is_sph_external_force>
      { };
    } // namespace meta
  } // namespace extforce
} // namespace pastel


#endif // PASTEL_EXTFORCE_META_IS_SPH_EXTERNAL_FORCE_HPP

