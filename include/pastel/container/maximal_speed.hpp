#ifndef PASTEL_CONTAINER_MAXIMAL_SPEED_HPP
# define PASTEL_CONTAINER_MAXIMAL_SPEED_HPP

# include <cmath>
# include <algorithm>
# include <numeric>
# include <iterator>
# include <utility>

# include <pastel/geometry/squared_norm.hpp>
# include <pastel/particle/tags.hpp>
# include <pastel/container/get.hpp>
# include <pastel/container/num_particles.hpp>
# include <pastel/container/meta/scalar_of.hpp>
# include <pastel/container/meta/vector_of.hpp>
# include <pastel/container/meta/is_data_accessible.hpp>


namespace pastel
{
  namespace container
  {
    namespace maximal_speed_detail
    {
      template <bool is_data_accessible>
      struct maximal_speed
      {
        template <typename Particles>
        static typename ::pastel::container::meta::scalar_of<Particles const>::type call(Particles const& particles)
        {
          auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
          auto const num_particles = ::pastel::container::num_particles(particles);

          using scalar_type = typename ::pastel::container::meta::scalar_of<Particles const>::type;
          using vector_type = typename ::pastel::container::meta::vector_of<Particles const>::type;
          using std::sqrt;
          return sqrt(
            std::accumulate(
              velocities_data, velocities_data+num_particles, scalar_type{0},
              [](scalar_type const last_maximal_squared_speed, vector_type const& velocity)
              { return std::max(last_maximal_squared_speed, ::pastel::geometry::squared_norm(velocity)); }));
        }

        template <typename Particles, typename Indices>
        static typename ::pastel::container::meta::scalar_of<Particles const>::type call(Particles const& particles, Indices const& indices)
        {
          auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();

          using scalar_type = typename ::pastel::container::meta::scalar_of<Particles const>::type;
          using std::sqrt;
          return sqrt(
            std::accumulate(
              std::begin(indices), std::end(indices), scalar_type{0},
              [velocities_data](scalar_type const last_maximal_squared_speed, decltype(*std::begin(indices)) const index)
              { return std::max(last_maximal_squared_speed, ::pastel::geometry::squared_norm(velocities_data[index])); }));
        }
      }; // struct maximal_speed<is_data_accessible>

      template <>
      struct maximal_speed<false>
      {
        template <typename Particles>
        static typename ::pastel::container::meta::scalar_of<Particles const>::type call(Particles const& particles)
        {
          auto const num_particles = ::pastel::container::num_particles(particles);

          using scalar_type = typename ::pastel::container::meta::scalar_of<Particles const>::type;
          auto last_maximal_squared_speed = scalar_type{0};
          for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
          {
            auto const squared_speed = ::pastel::geometry::squared_norm(::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index));
            last_maximal_squared_speed = std::max(squared_speed, last_maximal_squared_speed);
          }

          using std::sqrt;
          return sqrt(last_maximal_squared_speed);
        }

        template <typename Particles, typename Indices>
        static typename ::pastel::container::meta::scalar_of<Particles const>::type call(Particles const& particles, Indices const& indices)
        {
          using scalar_type = typename ::pastel::container::meta::scalar_of<Particles const>::type;
          using std::sqrt;
          return sqrt(
            std::accumulate(
              std::begin(indices), std::end(indices), scalar_type{0},
              [&particles](scalar_type const last_maximal_squared_speed, decltype(*std::begin(indices)) const index)
              {
                return std::max(
                  last_maximal_squared_speed,
                  ::pastel::geometry::squared_norm(::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index)));
              }));
        }
      }; // struct maximal_speed<false>
    } // namespace maximal_speed_detail

    template <typename Particles>
    inline typename ::pastel::container::meta::scalar_of<Particles const>::type maximal_speed(Particles const& particles)
    {
      static constexpr bool is_data_accessible
        = ::pastel::container::meta::is_data_accessible<Particles const>::value;
      using maximal_speed_func
        = ::pastel::container::maximal_speed_detail::maximal_speed<is_data_accessible>;
      return maximal_speed_func::call(particles);
    }

    template <typename Particles, typename Indices>
    inline typename ::pastel::container::meta::scalar_of<Particles const>::type maximal_speed(Particles const& particles, Indices const& indices)
    {
      static constexpr bool is_data_accessible
        = ::pastel::container::meta::is_data_accessible<Particles const>::value;
      using maximal_speed_func
        = ::pastel::container::maximal_speed_detail::maximal_speed<is_data_accessible>;
      return maximal_speed_func::call(particles, indices);
    }
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_MAXIMAL_SPEED_HPP

