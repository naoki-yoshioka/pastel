#ifndef PASTEL_CONTAINER_CLEAR_SPH_DIVERGENCES_HPP
# define PASTEL_CONTAINER_CLEAR_SPH_DIVERGENCES_HPP

# include <pastel/container/get.hpp>
# include <pastel/container/num_particles.hpp>
# include <pastel/container/meta/value_of.hpp>
# include <pastel/container/meta/scalar_of.hpp>
# include <pastel/particle/tags.hpp>
# include <pastel/particle/meta/has_divergence.hpp>


namespace pastel
{
  namespace container
  {
    namespace clear_sph_divergences
    {
      template <bool has_divergence>
      struct clear;

      template <>
      struct clear<true>
      {
        template <typename Particles>
        static void call(Particles& particles)
        {
          auto const num_particles = ::pastel::container::num_particles(particles);
          using scalar_type = typename ::pastel::container::meta::scalar_of<Particles>::type;
          for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            ::pastel::container::get< ::pastel::particle::tags::sph_divergence >(particles, index) = scalar_type{};
        }
      }; // struct clear<true>

      template <>
      struct clear<false>
      {
        template <typename Particles>
        static void call(Particles const&) { }
      }; // struct clear<false>
    } // namespace clear_sph_divergences

    template <typename Particles>
    inline void clear_sph_divergences(Particles& particles)
    {
      static constexpr bool has_divergence
        = ::pastel::particle::meta::has_divergence<typename ::pastel::container::meta::value_of<Particles>::type>::value;
      ::pastel::container::clear_sph_divergences_detail::clear<has_divergence>::call(particles);
    }
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_CLEAR_SPH_DIVERGENCES_HPP

