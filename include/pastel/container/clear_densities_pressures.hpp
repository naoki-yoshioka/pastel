#ifndef PASTEL_CONTAINER_CLEAR_DENSITIES_PRESSURES_HPP
# define PASTEL_CONTAINER_CLEAR_DENSITIES_PRESSURES_HPP

# include <pastel/container/get.hpp>
# include <pastel/container/num_particles.hpp>
# include <pastel/container/meta/value_of.hpp>
# include <pastel/container/meta/scalar_of.hpp>
# include <pastel/particle/tags.hpp>
# include <pastel/particle/meta/is_sph_particle.hpp>
# include <pastel/particle/meta/has_divergence.hpp>


namespace pastel
{
  namespace container
  {
    namespace clear_densities_pressures
    {
      template <bool is_sph_particle, bool has_divergence>
      struct clear;

      template <>
      struct clear<true, false>
      {
        template <typename Particles>
        static void call(Particles& particles)
        {
          auto const num_particles = ::pastel::container::num_particles(particles);
          using scalar_type = typename ::pastel::container::meta::scalar_of<Particles>::type;
          for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
          {
            ::pastel::container::get< ::pastel::particle::tags::density >(particles, index) = scalar_type{};
            ::pastel::container::get< ::pastel::particle::tags::pressure >(particles, index) = scalar_type{};
          }
        }
      }; // struct clear<true, false>

      template <>
      struct clear<true, true>
      {
        template <typename Particles>
        static void call(Particles const&) { }
      }; // struct clear<true, true>

      template <>
      struct clear<false, true>
      {
        template <typename Particles>
        static void call(Particles const&) { }
      }; // struct clear<false, true>

      template <>
      struct clear<false, false>
      {
        template <typename Particles>
        static void call(Particles const&) { }
      }; // struct clear<false, false>
    } // namespace clear_densities_pressures

    template <typename Particles>
    inline void clear_densities_pressures(Particles& particles)
    {
      static constexpr bool is_sph_particle
        = ::pastel::particle::meta::is_sph_particle<typename ::pastel::container::meta::value_of<Particles>::type>::value;
      static constexpr bool has_divergence
        = ::pastel::particle::meta::has_divergence<typename ::pastel::container::meta::value_of<Particles>::type>::value;
      ::pastel::container::clear_densities_pressures_detail::clear<is_sph_particle, has_divergence>::call(particles);
    }
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_CLEAR_DENSITIES_PRESSURES_HPP

