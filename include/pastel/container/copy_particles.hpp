#ifndef PASTEL_CONTAINER_COPY_PARTICLES_HPP
# define PASTEL_CONTAINER_COPY_PARTICLES_HPP


namespace pastel
{
  namespace container
  {
    namespace dispatch
    {
      template <typename Particles>
      struct copy_particles;
    } // namespace dispatch

    template <typename TargetParticles, typename Particles, typename Allocator, typename Size>
    inline void copy_particles(
      TargetParticles& target_particles,
      Partcles const& particles,
      typename ::pastel::container::meta::boundary_container_of<Particles>::type const& boundary_particles,
      std::vector< ::pastel::system::index_with_origin<typename ::pastel::container::meta::size_of<TargetParticles>::type>, Allocator > const& particle_indices_for_boundary,
      Size first, Size last)
    {
      ::pastel::container::dispatch::copy_particles<Particles>::call(
        target_particles, particles, boundary_particles,
        particle_indices_for_boundary, first, last);
    }
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_COPY_PARTICLES_HPP

