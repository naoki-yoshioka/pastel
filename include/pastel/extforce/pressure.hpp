#ifndef PASTEL_EXTFORCE_PRESSURE_HPP
# define PASTEL_EXTFORCE_PRESSURE_HPP

# include <pastel/extforce/meta/is_sph_external_force.hpp>


namespace pastel
{
  namespace extforce
  {
    namespace pressure_detail
    {
      template <bool is_sph_external_force>
      struct pressure;

      template <>
      struct pressure<true>
      {
        template <typename ExternalForce, typename Density>
        static auto call(ExternalForce const& external_force, Density const& density)
          -> decltype(external_force.pressure(density))
        { return external_force.pressure()(density); }
      }; // struct pressure<true>
    } // namespace pressure_detail

    template <typename ExternalForce, typename Density>
    auto pressure(ExternalForce const& external_force, Density const& density)
      -> decltype(::pastel::extforce::pressure_detail::pressure< ::pastel::extforce::meta::is_sph_external_force<ExternalForce>::value >::call(external_force, density))
    { return ::pastel::extforce::pressure_detail::pressure< ::pastel::extforce::meta::is_sph_external_force<ExternalForce>::value >::call(external_force, density); }
  } // namespace extforce
} // namespace pastel


#endif // PASTEL_EXTFORCE_PRESSURE_HPP

