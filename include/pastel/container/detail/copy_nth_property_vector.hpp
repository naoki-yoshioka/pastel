#ifndef PASTEL_CONTAINER_DETAIL_COPY_NTH_PROPERTY_VECTOR_HPP
# define PASTEL_CONTAINER_DETAIL_COPY_NTH_PROPERTY_VECTOR_HPP

# include <cstddef>

# include <pastel/container/get.hpp>
# include <pastel/container/tags.hpp>


namespace pastel
{
  namespace container
  {
    namespace detail
    {
      template <std::size_t n>
      struct copy_nth_property_vector
      {
        template <typename Particles, typename Index, typename TargetParticles>
        static void call(Particles const& particles, Index particle_index, TargetParticles& target_particles, Index target_particle_index)
        {
          ::pastel::container::get< ::pastel::container::tags::nth_property_vector<n> >(target_particles, target_particle_index)
            = ::pastel::container::get< ::pastel::container::tags::nth_property_vector<n> >(particles, particle_index);
        }
      }; // struct copy_nth_property_vector<n>
    } // namespace detail
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_DETAIL_COPY_NTH_PROPERTY_VECTOR_HPP

