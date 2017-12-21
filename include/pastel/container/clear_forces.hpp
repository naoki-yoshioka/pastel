#ifndef PASTEL_CONTAINER_CLEAR_FORCES_HPP
# define PASTEL_CONTAINER_CLEAR_FORCES_HPP

# include <pastel/container/get.hpp>
# include <pastel/container/num_particles.hpp>
# include <pastel/container/meta/vector_of.hpp>
# include <pastel/particle/tags.hpp>


namespace pastel
{
  namespace container
  {
    template <typename Particles>
    inline void clear_forces(Particles& particles)
    {
      auto const num_particles = ::pastel::container::num_particles(particles);
      using vector_type = typename ::pastel::container::meta::vector_of<Particles>::type;
      for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
        ::pastel::container::get< ::pastel::particle::tags::force >(particles, index) = vector_type{};
    }
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_CLEAR_FORCES_HPP

