#ifndef PASTEL_CONTAINER_TOTAL_KINETIC_ENERGY_HPP
# define PASTEL_CONTAINER_TOTAL_KINETIC_ENERGY_HPP

# include <pastel/container/num_particles.hpp>
# include <pastel/container/get.hpp>
# include <pastel/container/meta/scalar_of.hpp>
# include <pastel/container/meta/is_data_accessible.hpp>
# include <pastel/container/meta/has_mass.hpp>
# include <pastel/particle/tags.hpp>
# include <pastel/geometry/squared_norm.hpp>


namespace pastel
{
  namespace container
  {
    namespace total_kinetic_energy_detail
    {
      template <bool is_data_accessible, bool has_mass>
      struct total_kinetic_energy;

      template <>
      struct total_kinetic_energy<true, true>
      {
        template <typename Particles>
        static typename ::pastel::container::meta::scalar_of<Particles>::type call(Particles const& particles)
        {
          auto const& velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
          auto const& masses_data = particles.template data< ::pastel::particle::tags::mass >();

          using value_type = typename ::pastel::container::meta::scalar_of<Particles>::type;
          auto result = value_type{};
          auto const num_particles = ::pastel::container::num_particles(particles);
          for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            result += (masses_data[index] / value_type{2}) * ::pastel::geometry::squared_norm(velocities_data[index]);

          return result;
        }
      }; // struct total_kinetic_energy<true, true>

      template <>
      struct total_kinetic_energy<true, false>
      {
        template <typename Particles>
        static typename ::pastel::container::meta::scalar_of<Particles>::type call(Particles const& particles)
        {
          auto const& velocities_data = particles.template data< ::pastel::particle::tags::velocity >();

          using value_type = typename ::pastel::container::meta::scalar_of<Particles>::type;
          auto result = value_type{};
          auto const num_particles = ::pastel::container::num_particles(particles);
          for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            result += ::pastel::geometry::squared_norm(velocities_data[index]) / value_type{2};

          return result;
        }
      }; // struct total_kinetic_energy<true, false>

      template <>
      struct total_kinetic_energy<false, true>
      {
        template <typename Particles>
        static typename ::pastel::container::meta::scalar_of<Particles>::type call(Particles const& particles)
        {
          using value_type = typename ::pastel::container::meta::scalar_of<Particles>::type;
          auto result = value_type{};
          auto const num_particles = ::pastel::container::num_particles(particles);
          for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            result
              += (::pastel::container::get< ::pastel::particle::tags::mass >(particles, index) / value_type{2})
                 * ::pastel::geometry::squared_norm(
                     ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index));

          return result;
        }
      }; // struct total_kinetic_energy<false, true>

      template <>
      struct total_kinetic_energy<false, false>
      {
        template <typename Particles>
        static typename ::pastel::container::meta::scalar_of<Particles>::type call(Particles const& particles)
        {
          using value_type = typename ::pastel::container::meta::scalar_of<Particles>::type;
          auto result = value_type{};
          auto const num_particles = ::pastel::container::num_particles(particles);
          for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            result
              += ::pastel::geometry::squared_norm(
                   ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index))
                 / value_type{2};

          return result;
        }
      }; // struct total_kinetic_energy<false, false>
    } // namespace total_kinetic_energy_detail


    template <typename Particles>
    inline typename ::pastel::container::meta::scalar_of<Particles>::type total_kinetic_energy(Particles const& particles)
    {
      static constexpr bool is_data_accessible
        = ::pastel::container::meta::is_data_accessible<Particles>::value;
      static constexpr bool has_mass
        = ::pastel::container::meta::has_mass<Particles>::value;
      return ::pastel::container::total_kinetic_energy_detail::total_kinetic_energy<is_data_accessible, has_mass>::call(particles);
    }
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_TOTAL_KINETIC_ENERGY_HPP

