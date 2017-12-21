#ifndef PASTEL_CONTAINER_APPLY_EXTERNAL_FORCES_HPP
# define PASTEL_CONTAINER_APPLY_EXTERNAL_FORCES_HPP

# include <pastel/container/increase_force.hpp>
# include <pastel/container/num_particles.hpp>
# include <pastel/extforce/no_force.hpp>


namespace pastel
{
  namespace container
  {
    template <typename Particles, typename ExternalForce>
    inline void apply_external_forces(Particles& particles, ExternalForce&& external_force)
    {
      auto const num_particles = ::pastel::container::num_particles(particles);
      for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
        ::pastel::container::increase_force(particles, index, external_force(particles, index));
    }

    template <typename Particles>
    inline void apply_external_forces(Particles const&, ::pastel::extforce::no_force&)
    { }

    template <typename Particles>
    inline void apply_external_forces(Particles const&, ::pastel::extforce::no_force&&)
    { }
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_APPLY_EXTERNAL_FORCES_HPP

