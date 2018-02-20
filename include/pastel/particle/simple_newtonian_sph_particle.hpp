#ifndef PASTEL_PARTICLE_SIMPLE_NEWTONIAN_SPH_PARTICLE_HPP
# define PASTEL_PARTICLE_SIMPLE_NEWTONIAN_SPH_PARTICLE_HPP

# include <cstddef>
# include <utility>

# include <pastel/particle/tags.hpp>
# include <pastel/particle/get.hpp>
# include <pastel/particle/detail/lexicographical_compare.hpp>
# include <pastel/geometry/point.hpp>
# include <pastel/geometry/vector.hpp>
# include <pastel/utility/is_nothrow_swappable.hpp>


namespace pastel
{
  namespace particle
  {
    template <
      std::size_t dimension_, typename Value = double,
      typename Point = ::pastel::geometry::point<dimension_, Value>,
      typename Vector = ::pastel::geometry::vector<dimension_, Value>>
    class simple_newtonian_sph_particle;

    namespace simple_newtonian_sph_particle_detail
    {
      template <typename Tag, std::size_t dimension_, typename Value, typename Point, typename Vector>
      struct get;

      template <std::size_t dimension_, typename Value, typename Point, typename Vector>
      struct get< ::pastel::particle::tags::position, dimension_, Value, Point, Vector>
      {
        using value_type = Point;
        using reference = value_type&;
        using const_reference = value_type const&;
        using particle_type = ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector>;

        static reference call(particle_type& particle) noexcept { return particle.position_; }
        static const_reference call(particle_type const& particle) noexcept { return particle.position_; }
      }; // struct get< ::pastel::particle::tags::position, dimension_, Value, Point, Vector>

      template <std::size_t dimension_, typename Value, typename Point, typename Vector>
      struct get< ::pastel::particle::tags::velocity, dimension_, Value, Point, Vector>
      {
        using value_type = Vector;
        using reference = value_type&;
        using const_reference = value_type const&;
        using particle_type = ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector>;

        static reference call(particle_type& particle) noexcept { return particle.velocity_; }
        static const_reference call(particle_type const& particle) noexcept { return particle.velocity_; }
      }; // struct get< ::pastel::particle::tags::velocity, dimension_, Value, Point, Vector>

      template <std::size_t dimension_, typename Value, typename Point, typename Vector>
      struct get< ::pastel::particle::tags::force, dimension_, Value, Point, Vector>
      {
        using value_type = Vector;
        using reference = value_type&;
        using const_reference = value_type const&;
        using particle_type = ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector>;

        static reference call(particle_type& particle) noexcept { return particle.force_; }
        static const_reference call(particle_type const& particle) noexcept { return particle.force_; }
      }; // struct get< ::pastel::particle::tags::force, dimension_, Value, Point, Vector>

      template <std::size_t dimension_, typename Value, typename Point, typename Vector>
      struct get< ::pastel::particle::tags::density, dimension_, Value, Point, Vector>
      {
        using value_type = Value;
        using reference = value_type&;
        using const_reference = value_type const&;
        using particle_type = ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector>;

        static reference call(particle_type& particle) noexcept { return particle.density_; }
        static const_reference call(particle_type const& particle) noexcept { return particle.density_; }
      }; // struct get< ::pastel::particle::tags::density, dimension_, Value, Point, Vector>

      template <std::size_t dimension_, typename Value, typename Point, typename Vector>
      struct get< ::pastel::particle::tags::sph_divergence, dimension_, Value, Point, Vector>
      {
        using value_type = Value;
        using reference = value_type&;
        using const_reference = value_type const&;
        using particle_type = ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector>;

        static reference call(particle_type& particle) noexcept { return particle.sph_divergence_; }
        static const_reference call(particle_type const& particle) noexcept { return particle.sph_divergence_; }
      }; // struct get< ::pastel::particle::tags::sph_divergence, dimension_, Value, Point, Vector>

      template <std::size_t dimension_, typename Value, typename Point, typename Vector>
      struct get< ::pastel::particle::tags::pressure, dimension_, Value, Point, Vector>
      {
        using value_type = Value;
        using reference = value_type&;
        using const_reference = value_type const&;
        using particle_type = ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector>;

        static reference call(particle_type& particle) noexcept { return particle.pressure_; }
        static const_reference call(particle_type const& particle) noexcept { return particle.pressure_; }
      }; // struct get< ::pastel::particle::tags::pressure, dimension_, Value, Point, Vector>
    } // namespace simple_newtonian_sph_particle_detail


    template <std::size_t dimension_, typename Value, typename Point, typename Vector>
    class simple_newtonian_sph_particle final
    {
     public:
      static constexpr std::size_t dimension = dimension_;
      static constexpr bool has_mass = false;
      static constexpr bool is_sph_particle = true;
      using point_type = Point;
      using vector_type = Vector;
      using scalar_type = Value;
      using orientation_type = void;

     private:
      point_type position_;
      vector_type velocity_;
      vector_type force_;
      scalar_type density_;
      scalar_type sph_divergence_;
      scalar_type pressure_;

     public:
      constexpr simple_newtonian_sph_particle() noexcept = default;
      ~simple_newtonian_sph_particle() noexcept = default;
      simple_newtonian_sph_particle(simple_newtonian_sph_particle const&) = default;
      simple_newtonian_sph_particle(simple_newtonian_sph_particle&&) noexcept = default;
      simple_newtonian_sph_particle& operator=(simple_newtonian_sph_particle const&) & = default;
      simple_newtonian_sph_particle& operator=(simple_newtonian_sph_particle&&) & noexcept = default;

      constexpr simple_newtonian_sph_particle(
        point_type const& position, vector_type const& velocity, vector_type const& force,
        scalar_type const& density, scalar_type const& sph_divergence, scalar_type const& pressure)
        : position_{position},
          velocity_{velocity},
          force_{force},
          density_{density},
          sph_divergence_{sph_divergence},
          pressure_{pressure}
      { }

      constexpr simple_newtonian_sph_particle(
        point_type&& position, vector_type&& velocity, vector_type&& force,
        scalar_type&& density, scalar_type&& sph_divergence, scalar_type&& pressure)
        : position_{std::move(position)},
          velocity_{std::move(velocity)},
          force_{std::move(force)},
          density_{std::move(density)},
          sph_divergence_{std::move(sph_divergence)},
          pressure_{std::move(pressure)}
      { }


      void swap(simple_newtonian_sph_particle& other)
        noexcept(
          ::pastel::utility::is_nothrow_swappable<point_type>::value
          && ::pastel::utility::is_nothrow_swappable<vector_type>::value)
      {
        using std::swap;
        swap(position_, other.position_);
        swap(velocity_, other.velocity_);
        swap(force_, other.force_);
        swap(density_, other.density_);
        swap(sph_divergence_, other.sph_divergence_);
        swap(pressure_, other.pressure_);
      }


     private:
      template <typename Tag, int, typename, typename, typename>
      friend class ::pastel::particle::simple_newtonian_sph_particle_detail::get;

      template <typename Tag>
      using simple_newtonian_sph_particle_get = ::pastel::particle::simple_newtonian_sph_particle_detail::get<Tag, dimension, Value, Point, Vector>;

     public:
      template <typename Tag>
      constexpr typename simple_newtonian_sph_particle_get<Tag>::reference get() & noexcept
      { return simple_newtonian_sph_particle_get<Tag>::call(*this); }

      template <typename Tag>
      constexpr typename simple_newtonian_sph_particle_get<Tag>::const_reference get() const& noexcept
      { return simple_newtonian_sph_particle_get<Tag>::call(*this); }

      template <typename Tag>
      constexpr typename simple_newtonian_sph_particle_get<Tag>::value_type get() && noexcept
      { return std::move(simple_newtonian_sph_particle_get<Tag>::call(*this)); }
    }; // class simple_newtonian_sph_particle<dimension_, Value, Point, Vector>

    template <std::size_t dimension_, typename Value, typename Point, typename Vector>
    inline bool operator==(
      ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector> const& lhs,
      ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector> const& rhs)
    {
      return
        lhs.template get< ::pastel::particle::tags::position >() == rhs.template get< ::pastel::particle::tags::position >()
        && lhs.template get< ::pastel::particle::tags::velocity >() == rhs.template get< ::pastel::particle::tags::velocity >()
        && lhs.template get< ::pastel::particle::tags::force >() == rhs.template get< ::pastel::particle::tags::force >()
        && lhs.template get< ::pastel::particle::tags::density >() == rhs.template get< ::pastel::particle::tags::density >()
        && lhs.template get< ::pastel::particle::tags::sph_divergence >() == rhs.template get< ::pastel::particle::tags::sph_divergence >()
        && lhs.template get< ::pastel::particle::tags::pressure >() == rhs.template get< ::pastel::particle::tags::pressure >();
    }

    template <std::size_t dimension_, typename Value, typename Point, typename Vector>
    inline bool operator!=(
      ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector> const& lhs,
      ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector> const& rhs)
    { return !(lhs == rhs); }

    template <std::size_t dimension_, typename Value, typename Point, typename Vector>
    inline bool operator<(
      ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector> const& lhs,
      ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector> const& rhs)
    {
      using lexicographical_compare
        = ::pastel::particle::detail::lexicographical_compare<
            ::pastel::particle::tags::position,
            ::pastel::particle::tags::velocity,
            ::pastel::particle::tags::force,
            ::pastel::particle::tags::density,
            ::pastel::particle::tags::sph_divergence,
            ::pastel::particle::tags::pressure>;
      return lexicographical_compare::call(lhs, rhs);
    }

    template <std::size_t dimension_, typename Value, typename Point, typename Vector>
    inline bool operator>(
      ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector> const& lhs,
      ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector> const& rhs)
    { return rhs < lhs; }

    template <std::size_t dimension_, typename Value, typename Point, typename Vector>
    inline bool operator<=(
      ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector> const& lhs,
      ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector> const& rhs)
    { return !(lhs > rhs); }

    template <std::size_t dimension_, typename Value, typename Point, typename Vector>
    inline bool operator>=(
      ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector> const& lhs,
      ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector> const& rhs)
    { return !(lhs < rhs); }

    template <std::size_t dimension_, typename Value, typename Point, typename Vector>
    inline void swap(
      ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector>& lhs,
      ::pastel::particle::simple_newtonian_sph_particle<dimension_, Value, Point, Vector>& rhs)
      noexcept(noexcept(lhs.swap(rhs)))
    { lhs.swap(rhs); }
  } // namespace particle
} // namespace pastel


#endif // PASTEL_PARTICLE_SIMPLE_NEWTONIAN_SPH_PARTICLE_HPP

