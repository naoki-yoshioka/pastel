#ifndef PASTEL_CONTAINER_ADD_PARTICLES_HPP
# define PASTEL_CONTAINER_ADD_PARTICLES_HPP

# include <vector>

# include <pastel/container/meta/size_of.hpp>
# include <pastel/container/meta/boundary_container_of.hpp>
# include <pastel/system/index_with_origin.hpp>


namespace pastel
{
  namespace container
  {
    namespace dispatch
    {
      template <typename Particles>
      struct add_particles;
    } // namespace dispatch

    template <typename TargetParticles, typename Particles, typename Allocator, typename Size>
    inline void add_particles(
      TargetParticles& target_particles,
      Particles const& particles,
      typename ::pastel::container::meta::boundary_container_of<Particles>::type const& boundary_particles,
      std::vector< ::pastel::system::index_with_origin<typename ::pastel::container::meta::size_of<TargetParticles>::type>, Allocator > const& particle_indices_for_boundary,
      Size first, Size last)
    {
      ::pastel::container::dispatch::add_particles<Particles>::call(
        target_particles, particles, boundary_particles,
        particle_indices_for_boundary, first, last);
    }
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_ADD_PARTICLES_HPP

