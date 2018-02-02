#ifndef PASTEL_CONTAINER_ALIGNED_SIMPLE_PARTICLES_HPP
# define PASTEL_CONTAINER_ALIGNED_SIMPLE_PARTICLES_HPP

# include <cstddef>

# include <boost/align/aligned_allocator.hpp>

# include <pastel/container/simple_particles.hpp>
# include <pastel/container/mobility_tags.hpp>
# include <pastel/geometry/vector.hpp>
# include <pastel/geometry/point.hpp>


namespace pastel
{
  namespace container
  {
    template <
      std::size_t dimension_,
      typename MobilityTag = ::pastel::container::mobility_tags::mobile,
      std::size_t num_integration_vectors_ = 0u,
      std::size_t num_property_vectors_ = 0u,
      std::size_t num_property_scalars_ = 0u,
      typename Value = double,
      typename Point = ::pastel::geometry::point<dimension_, Value>,
      typename Vector = ::pastel::geometry::vector<dimension_, Value>>
    using aligned_simple_particles
      = ::pastel::container::simple_particles<
          dimension_, MobilityTag,
          num_integration_vectors_, num_property_vectors_, num_property_scalars_,
          Value, Point, Vector,
          boost::alignment::aligned_allocator<Point>,
          boost::alignment::aligned_allocator<Vector>,
          boost::alignment::aligned_allocator<Value>>;
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_ALIGNED_SIMPLE_PARTICLES_HPP

