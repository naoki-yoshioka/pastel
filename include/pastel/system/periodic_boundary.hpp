#ifndef PASTEL_SYSTEM_PERIODIC_BOUNDARY_HPP
# define PASTEL_SYSTEM_PERIODIC_BOUNDARY_HPP

# include <cassert>
# include <cstddef>
# include <type_traits>

# include <pastel/system/particles.hpp>
# include <pastel/system/boundary_particles.hpp>
# include <pastel/system/particle_indices_for_boundary.hpp>
# include <pastel/system/boundary_particle_indices.hpp>
# include <pastel/system/index_with_origin.hpp>
# include <pastel/system/meta/point_of.hpp>
# include <pastel/geometry/get.hpp>
# include <pastel/geometry/meta/value_of.hpp>
# include <pastel/particle/tags.hpp>
# include <pastel/container/copy_particles.hpp>
# include <pastel/container/add_particles.hpp>
# include <pastel/container/get.hpp>
# include <pastel/container/num_particles.hpp>
# include <pastel/container/tags.hpp>
# include <pastel/container/meta/is_data_accessible.hpp>


namespace pastel
{
  namespace system
  {
    namespace periodic_boundary_detail
    {
      template <std::size_t n, bool is_data_accessible>
      struct increase_positions_if;

      template <std::size_t n>
      struct increase_positions_if<n, true>
      {
        template <typename Particles, typename Value, typename Predicate>
        static void call(Particles& particles, Value const& length_increment, Predicate predicate)
        {
          auto const positions_data = particles.template data< ::pastel::particle::tags::position >();

          auto const num_particles = ::pastel::container::num_particles(particles);
          for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
          {
            auto& position = ::pastel::geometry::get<n>(positions_data[index]);
            position = predicate(position) ? position + length_increment : position;
          }
        }
      }; // struct increase_positions_if<n, true>

      template <std::size_t n>
      struct increase_positions_if<n, false>
      {
        template <typename Particles, typename Value, typename Predicate>
        static void call(Particles& particles, Value const& length_increment, Predicate predicate)
        {
          auto const num_particles = ::pastel::container::num_particles(particles);
          for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
          {
            auto& position = ::pastel::geometry::get<n>(::pastel::container::get< ::pastel::particle::tags::position >(particles, index));
            position = predicate(position) ? position + length_increment : position;
          }
        }
      }; // struct increase_positions_if<n, false>


      template <std::size_t n, bool is_data_accessible, bool is_boundary_data_accessible>
      struct add_indices_within;

      template <std::size_t n>
      struct add_indices_within<n, true, true>
      {
        template <typename ParticleIndicesForBoundary, typename Particles, typename BoundaryParticles, typename Value>
        static void call(
          ParticleIndicesForBoundary& particle_indices_for_boundary,
          Particles const& particles, BoundaryParticles const& boundary_particles,
          Value const& lower_bound, Value const& upper_bound)
        {
          auto const positions_data = particles.template data< ::pastel::particle::tags::position >();

          auto const num_particles = ::pastel::container::num_particles(particles);
          for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
          {
            auto const& position = ::pastel::geometry::get<n>(positions_data[index]);
            if (position >= lower_bound && position < upper_bound)
              particle_indices_for_boundary.emplace_back(index, ::pastel::system::origin::particles);
          }

          auto const boundary_positions_data = boundary_particles.template data< ::pastel::particle::tags::position >();

          auto const num_boundary_particles = ::pastel::container::num_particles(boundary_particles);
          for (auto index = static_cast<decltype(num_boundary_particles)>(0); index < num_boundary_particles; ++index)
          {
            auto const& position = ::pastel::geometry::get<n>(boundary_positions_data[index]);
            if (position >= lower_bound && position < upper_bound)
              particle_indices_for_boundary.emplace_back(index, ::pastel::system::origin::boundary_particles);
          }
        }
      }; // struct add_indices_within<n, true, true>

      template <std::size_t n>
      struct add_indices_within<n, false, true>
      {
        template <typename ParticleIndicesForBoundary, typename Particles, typename BoundaryParticles, typename Value>
        static void call(
          ParticleIndicesForBoundary& particle_indices_for_boundary,
          Particles const& particles, BoundaryParticles const& boundary_particles,
          Value const& lower_bound, Value const& upper_bound)
        {
          auto const num_particles = ::pastel::container::num_particles(particles);
          for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
          {
            auto const& position
              = ::pastel::geometry::get<n>(::pastel::container::get< ::pastel::particle::tags::position >(particles, index));
            if (position >= lower_bound && position < upper_bound)
              particle_indices_for_boundary.emplace_back(index, ::pastel::system::origin::particles);
          }

          auto const boundary_positions_data = boundary_particles.template data< ::pastel::particle::tags::position >();

          auto const num_boundary_particles = ::pastel::container::num_particles(boundary_particles);
          for (auto index = static_cast<decltype(num_boundary_particles)>(0); index < num_boundary_particles; ++index)
          {
            auto const& position = ::pastel::geometry::get<n>(boundary_positions_data[index]);
            if (position >= lower_bound && position < upper_bound)
              particle_indices_for_boundary.emplace_back(index, ::pastel::system::origin::boundary_particles);
          }
        }
      }; // struct add_indices_within<n, false, true>

      template <std::size_t n>
      struct add_indices_within<n, true, false>
      {
        template <typename ParticleIndicesForBoundary, typename Particles, typename BoundaryParticles, typename Value>
        static void call(
          ParticleIndicesForBoundary& particle_indices_for_boundary,
          Particles const& particles, BoundaryParticles const& boundary_particles,
          Value const& lower_bound, Value const& upper_bound)
        {
          auto const positions_data = particles.template data< ::pastel::particle::tags::position >(particles);

          auto const num_particles = ::pastel::container::num_particles(particles);
          for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
          {
            auto const& position = ::pastel::geometry::get<n>(positions_data[index]);
            if (position >= lower_bound && position < upper_bound)
              particle_indices_for_boundary.emplace_back(index, ::pastel::system::origin::particles);
          }

          auto const num_boundary_particles = ::pastel::container::num_particles(boundary_particles);
          for (auto index = static_cast<decltype(num_boundary_particles)>(0); index < num_boundary_particles; ++index)
          {
            auto const& position
              = ::pastel::geometry::get<n>(::pastel::container::get< ::pastel::particle::tags::position >(boundary_particles, index));
            if (position >= lower_bound && position < upper_bound)
              particle_indices_for_boundary.emplace_back(index, ::pastel::system::origin::boundary_particles);
          }
        }
      }; // struct add_indices_within<n, true, false>

      template <std::size_t n>
      struct add_indices_within<n, false, false>
      {
        template <typename ParticleIndicesForBoundary, typename Particles, typename BoundaryParticles, typename Value>
        static void call(
          ParticleIndicesForBoundary& particle_indices_for_boundary,
          Particles const& particles, BoundaryParticles const& boundary_particles,
          Value const& lower_bound, Value const& upper_bound)
        {
          auto const num_particles = ::pastel::container::num_particles(particles);
          for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
          {
            auto const& position
              = ::pastel::geometry::get<n>(::pastel::container::get< ::pastel::particle::tags::position >(particles, index));
            if (position >= lower_bound && position < upper_bound)
              particle_indices_for_boundary.emplace_back(index, ::pastel::system::origin::particles);
          }

          auto const num_boundary_particles = ::pastel::container::num_particles(boundary_particles);
          for (auto index = static_cast<decltype(num_boundary_particles)>(0); index < num_boundary_particles; ++index)
          {
            auto const& position
              = ::pastel::geometry::get<n>(::pastel::container::get< ::pastel::particle::tags::position >(boundary_particles, index));
            if (position >= lower_bound && position < upper_bound)
              particle_indices_for_boundary.emplace_back(index, ::pastel::system::origin::boundary_particles);
          }
        }
      }; // struct add_indices_within<n, false, false>


      template <std::size_t n, bool is_boundary_data_accessible>
      struct apply_periodic_boundary;

      template <std::size_t n>
      struct apply_periodic_boundary<n, true>
      {
        template <typename BoundaryParticles, typename Size, typename Value>
        static void call(
          BoundaryParticles& boundary_particles, Size first, Size last,
          Value const& length_increment)
        {
          auto const boundary_positions_data = boundary_particles.template data< ::pastel::particle::tags::position >();

          for (Size index = first; index < last; ++index)
            ::pastel::geometry::get<n>(boundary_positions_data[index]) += length_increment;
        }
      }; // struct apply_periodic_boundary<n, true>

      template <std::size_t n>
      struct apply_periodic_boundary<n, false>
      {
        template <typename BoundaryParticles, typename Size, typename Value>
        static void call(
          BoundaryParticles& boundary_particles, Size first, Size last,
          Value const& length_increment)
        {
          for (Size index = first; index < last; ++index)
            ::pastel::geometry::get<n>(::pastel::container::get< ::pastel::particle::tags::position >(boundary_particles, index))
              += length_increment;
        }
      }; // struct apply_periodic_boundary<n, false>
    } // namespace periodic_boundary_detail


    // lower_bound_ ... lower_bound_+boundary_width ... upper_bound_-boundary_width ... upper_bound_
    //               ^                                                               ^
    //       lower_near_boundary                                             upper_near_boundary
    template <typename Value = double>
    class periodic_boundary
    {
      // lower_bound_ <= x < upper_bound_
      Value lower_bound_;
      Value upper_bound_;
      Value system_length_;

      // lower_bound_ <= lower_near_boundary < lower_bound_ + boundary_width
      // upper_bound_ - boundary_width <= upper_near_boundary < upper_bound_
      Value boundary_width_;

      Value upper_bound_of_lower_near_boundary_;
      Value lower_bound_of_upper_near_boundary_;

     public:
      using value_type = Value;

      periodic_boundary()
        : periodic_boundary{Value{-2}, Value{2}, Value{1}}
      { }

      periodic_boundary(Value const& lower_bound, Value const& upper_bound, Value const& boundary_width)
        : lower_bound_{lower_bound},
          upper_bound_{upper_bound},
          system_length_{upper_bound - lower_bound},
          boundary_width_{boundary_width},
          upper_bound_of_lower_near_boundary_{lower_bound + boundary_width},
          lower_bound_of_upper_near_boundary_{upper_bound - boundary_width}
      { assert(upper_bound_ > lower_bound_ && boundary_width_ > Value{0} && lower_bound_of_upper_near_boundary_ > upper_bound_of_lower_near_boundary_); }


      template <std::size_t dimension, std::size_t index, typename System>
      void modify_outside_particles(System& system) const
      {
        auto& particles = ::pastel::system::particles<index>(system);
        static constexpr bool is_data_accessible
          = ::pastel::container::meta::is_data_accessible<typename std::remove_reference<decltype(particles)>::type>::value;

        using increase_positions_if_func
          = ::pastel::system::periodic_boundary_detail::increase_positions_if<dimension, is_data_accessible>;
        using value_type = typename ::pastel::geometry::meta::value_of<typename ::pastel::system::meta::point_of<index, System>::type>::type;
        increase_positions_if_func::call(particles, system_length_, [this](value_type const& position) { return position < this->lower_bound_; });
        increase_positions_if_func::call(particles, -system_length_, [this](value_type const& position) { return position >= this->upper_bound_; });
      }

      template <std::size_t dimension, std::size_t index, typename System>
      void initialize_boundary_particles(System& system) const
      {
        auto const& particles = ::pastel::system::particles<index>(system);
        auto& boundary_particles = ::pastel::system::boundary_particles<index>(system);
        auto& particle_indices_for_boundary = ::pastel::system::particle_indices_for_boundary<index>(system);
        particle_indices_for_boundary.clear();
        static constexpr bool is_data_accessible
          = ::pastel::container::meta::is_data_accessible<typename std::remove_reference<decltype(particles)>::type>::value;
        static constexpr bool is_boundary_data_accessible
          = ::pastel::container::meta::is_data_accessible<typename std::remove_reference<decltype(boundary_particles)>::type>::value;

        // lower_near_boundary => upper_boundary
        auto const upper_boundary_particle_first = particle_indices_for_boundary.size();
        using add_indices_within_func
          = ::pastel::system::periodic_boundary_detail::add_indices_within<dimension, is_data_accessible, is_boundary_data_accessible>;
        add_indices_within_func::call(
          particle_indices_for_boundary, particles, boundary_particles,
          lower_bound_, upper_bound_of_lower_near_boundary_);
        auto const upper_boundary_particle_last = particle_indices_for_boundary.size();
        ::pastel::container::add_particles(
          boundary_particles,
          particles, boundary_particles, particle_indices_for_boundary,
          upper_boundary_particle_first, upper_boundary_particle_last);
        using apply_periodic_boundary_func
          = ::pastel::system::periodic_boundary_detail::apply_periodic_boundary<dimension, is_boundary_data_accessible>;
        apply_periodic_boundary_func::call(
          boundary_particles,
          upper_boundary_particle_first, upper_boundary_particle_last,
          system_length_);

        // upper_near_boundary => lower_boundary
        add_indices_within_func::call(
          particle_indices_for_boundary, particles, boundary_particles,
          lower_bound_of_upper_near_boundary_, upper_bound_);
        auto const lower_boundary_particle_last = particle_indices_for_boundary.size();
        ::pastel::container::add_particles(
          boundary_particles,
          particles, boundary_particles, particle_indices_for_boundary,
          upper_boundary_particle_last, lower_boundary_particle_last);
        apply_periodic_boundary_func::call(
          boundary_particles,
          upper_boundary_particle_last, lower_boundary_particle_last,
          -system_length_);

        ::pastel::system::boundary_particle_indices<index, dimension>(system)
          = {upper_boundary_particle_first, upper_boundary_particle_last, lower_boundary_particle_last};
      }


      template <std::size_t dimension, std::size_t index, typename System>
      void update_boundary_particles(System& system) const
      {
        auto const& particles = ::pastel::system::particles<index>(system);
        auto& boundary_particles = ::pastel::system::boundary_particles<index>(system);
        auto const& particle_indices_for_boundary = ::pastel::system::particle_indices_for_boundary<index>(system);
        static constexpr bool is_boundary_data_accessible
          = ::pastel::container::meta::is_data_accessible<typename std::remove_reference<decltype(boundary_particles)>::type>::value;

        auto const& boundary_particle_indices = ::pastel::system::boundary_particle_indices<index, dimension>(system);
        auto const& upper_boundary_particle_first = std::get<0u>(boundary_particle_indices);
        auto const& upper_boundary_particle_last = std::get<1u>(boundary_particle_indices);
        auto const& lower_boundary_particle_last = std::get<2u>(boundary_particle_indices);

        // lower_near_boundary => upper_boundary
        ::pastel::container::copy_particles(
          boundary_particles,
          particles, boundary_particles, particle_indices_for_boundary,
          upper_boundary_particle_first, upper_boundary_particle_last);
        using apply_periodic_boundary_func
          = ::pastel::system::periodic_boundary_detail::apply_periodic_boundary<dimension, is_boundary_data_accessible>;
        apply_periodic_boundary_func::call(
          boundary_particles,
          upper_boundary_particle_first, upper_boundary_particle_last,
          system_length_);

        // upper_near_boundary => lower_boundary
        ::pastel::container::copy_particles(
          boundary_particles,
          particles, boundary_particles, particle_indices_for_boundary,
          upper_boundary_particle_last, lower_boundary_particle_last);
        apply_periodic_boundary_func::call(
          boundary_particles,
          upper_boundary_particle_last, lower_boundary_particle_last,
          -system_length_);
      }
    }; // class periodic_boundary<Value>
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_PERIODIC_BOUNDARY_HPP

