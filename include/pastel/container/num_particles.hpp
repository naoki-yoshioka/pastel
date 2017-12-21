#ifndef PASTEL_CONTAINER_NUM_PARTICLES_HPP
# define PASTEL_CONTAINER_NUM_PARTICLES_HPP

# include <pastel/container/meta/size_of.hpp>


namespace pastel
{
  namespace container
  {
    template <typename Particles>
    inline constexpr typename ::pastel::container::meta::size_of<Particles const>::type
    num_particles(Particles const& particles)
    { return particles.size(); }
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_NUM_PARTICLES_HPP

