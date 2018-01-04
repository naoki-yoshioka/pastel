#ifndef PASTEL_PARTICLE_POINT_PARTICLE_HPP
# define PASTEL_PARTICLE_POINT_PARTICLE_HPP

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
    class point_particle;

    namespace point_particle_detail
    {
      template <typename Tag, std::size_t dimension_, typename Value, typename Point, typename Vector>
      struct get;

      template <std::size_t dimension_, typename Value, typename Point, typename Vector>
      struct get< ::pastel::particle::tags::position, dimension_, Value, Point, Vector>
      {
        using value_type = Point;
        using reference = value_type&;
        using const_reference = value_type const&;
        using particle_type = ::pastel::particle::point_particle<dimension_, Value, Point, Vector>;

        static reference call(particle_type& particle) noexcept { return particle.position_; }
        static const_reference call(particle_type const& particle) noexcept { return particle.position_; }
      }; // struct get< ::pastel::particle::tags::position, dimension_, Value, Point, Vector>

      template <std::size_t dimension_, typename Value, typename Point, typename Vector>
      struct get< ::pastel::particle::tags::velocity, dimension_, Value, Point, Vector>
      {
        using value_type = Vector;
        using reference = value_type&;
        using const_reference = value_type const&;
        using particle_type = ::pastel::particle::point_particle<dimension_, Value, Point, Vector>;

        static reference call(particle_type& particle) noexcept { return particle.velocity_; }
        static const_reference call(particle_type const& particle) noexcept { return particle.velocity_; }
      }; // struct get< ::pastel::particle::tags::velocity, dimension_, Value, Point, Vector>

      template <std::size_t dimension_, typename Value, typename Point, typename Vector>
      struct get< ::pastel::particle::tags::force, dimension_, Value, Point, Vector>
      {
        using value_type = Vector;
        using reference = value_type&;
        using const_reference = value_type const&;
        using particle_type = ::pastel::particle::point_particle<dimension_, Value, Point, Vector>;

        static reference call(particle_type& particle) noexcept { return particle.force_; }
        static const_reference call(particle_type const& particle) noexcept { return particle.force_; }
      }; // struct get< ::pastel::particle::tags::force, dimension_, Value, Point, Vector>

      template <std::size_t dimension_, typename Value, typename Point, typename Vector>
      struct get< ::pastel::particle::tags::mass, dimension_, Value, Point, Vector>
      {
        using value_type = Value;
        using reference = value_type&;
        using const_reference = value_type const&;
        using particle_type = ::pastel::particle::point_particle<dimension_, Value, Point, Vector>;

        static reference call(particle_type& particle) noexcept { return particle.mass_; }
        static const_reference call(particle_type const& particle) noexcept { return particle.mass_; }
      }; // struct get< ::pastel::particle::tags::mass, dimension_, Value, Point, Vector>
    } // namespace point_particle_detail


    template <std::size_t dimension_, typename Value, typename Point, typename Vector>
    class point_particle final
    {
     public:
      static constexpr std::size_t dimension = dimension_;
      static constexpr bool has_mass = true
      using point_type = Point;
      using vector_type = Vector;
      using scalar_type = Value;
      using orientation_type = void;

     private:
      point_type position_;
      vector_type velocity_;
      vector_type force_;
      scalar_type mass_;

     public:
      constexpr point_particle() noexcept = default;
      ~point_particle() noexcept = default;
      point_particle(point_particle const&) = default;
      point_particle(point_particle&&) noexcept = default;
      point_particle& operator=(point_particle const&) & = default;
      point_particle& operator=(point_particle&&) & noexcept = default;

      constexpr point_particle(
        point_type const& position, vector_type const& velocity, vector_type const& force,
        scalar_type const& mass)
        : position_{position},
          velocity_{velocity},
          force_{force},
          mass_{mass}
      { }

      constexpr point_particle(
        point_type&& position, vector_type&& velocity, vector_type&& force,
        scalar_type&& mass)
        : position_{std::move(position)},
          velocity_{std::move(velocity)},
          force_{std::move(force)},
          mass_{std::move(mass)}
      { }


      void swap(point_particle& other)
        noexcept(
          ::pastel::utility::is_nothrow_swappable<point_type>::value
          && ::pastel::utility::is_nothrow_swappable<vector_type>::value
          && ::pastel::utility::is_nothrow_swappable<scalar_type>::value)
      {
        using std::swap;
        swap(position_, other.position_);
        swap(velocity_, other.velocity_);
        swap(force_, other.force_);
        swap(mass_, other.mass_);
      }


     private:
      template <typename Tag, int, typename, typename, typename>
      friend class ::pastel::particle::point_particle_detail::get;

      template <typename Tag>
      using point_particle_get = ::pastel::particle::point_particle_detail::get<Tag, dimension, Value, Point, Vector>;

     public:
      template <typename Tag>
      constexpr typename point_particle_get<Tag>::reference get() & noexcept
      { return point_particle_get<Tag>::call(*this); }

      template <typename Tag>
      constexpr typename point_particle_get<Tag>::const_reference get() const& noexcept
      { return point_particle_get<Tag>::call(*this); }

      template <typename Tag>
      constexpr typename point_particle_get<Tag>::value_type get() && noexcept
      { return std::move(point_particle_get<Tag>::call(*this)); }
    }; // class point_particle<dimension_, Value, Point, Vector>

    template <std::size_t dimension_, typename Value, typename Point, typename Vector>
    inline bool operator==(
      ::pastel::particle::point_particle<dimension_, Value, Point, Vector> const& lhs,
      ::pastel::particle::point_particle<dimension_, Value, Point, Vector> const& rhs)
    {
      return
        lhs.template get< ::pastel::particle::tags::position >() == rhs.template get< ::pastel::particle::tags::position >()
        && lhs.template get< ::pastel::particle::tags::velocity >() == rhs.template get< ::pastel::particle::tags::velocity >()
        && lhs.template get< ::pastel::particle::tags::force >() == rhs.template get< ::pastel::particle::tags::force >();
    }

    template <std::size_t dimension_, typename Value, typename Point, typename Vector>
    inline bool operator!=(
      ::pastel::particle::point_particle<dimension_, Value, Point, Vector> const& lhs,
      ::pastel::particle::point_particle<dimension_, Value, Point, Vector> const& rhs)
    { return !(lhs == rhs); }

    template <std::size_t dimension_, typename Value, typename Point, typename Vector>
    inline bool operator<(
      ::pastel::particle::point_particle<dimension_, Value, Point, Vector> const& lhs,
      ::pastel::particle::point_particle<dimension_, Value, Point, Vector> const& rhs)
    {
      using lexicographical_compare
        = ::pastel::particle::detail::lexicographical_compare<
            ::pastel::particle::tags::position,
            ::pastel::particle::tags::velocity,
            ::pastel::particle::tags::force>;
      return lexicographical_compare::call(lhs, rhs);
    }

    template <std::size_t dimension_, typename Value, typename Point, typename Vector>
    inline bool operator>(
      ::pastel::particle::point_particle<dimension_, Value, Point, Vector> const& lhs,
      ::pastel::particle::point_particle<dimension_, Value, Point, Vector> const& rhs)
    { return rhs < lhs; }

    template <std::size_t dimension_, typename Value, typename Point, typename Vector>
    inline bool operator<=(
      ::pastel::particle::point_particle<dimension_, Value, Point, Vector> const& lhs,
      ::pastel::particle::point_particle<dimension_, Value, Point, Vector> const& rhs)
    { return !(lhs > rhs); }

    template <std::size_t dimension_, typename Value, typename Point, typename Vector>
    inline bool operator>=(
      ::pastel::particle::point_particle<dimension_, Value, Point, Vector> const& lhs,
      ::pastel::particle::point_particle<dimension_, Value, Point, Vector> const& rhs)
    { return !(lhs < rhs); }

    template <std::size_t dimension_, typename Value, typename Point, typename Vector>
    inline void swap(
      ::pastel::particle::point_particle<dimension_, Value, Point, Vector>& lhs,
      ::pastel::particle::point_particle<dimension_, Value, Point, Vector>& rhs)
      noexcept(noexcept(lhs.swap(rhs)))
    { lhs.swap(rhs); }
  } // namespace particle
} // namespace pastel


#endif // PASTEL_PARTICLE_POINT_PARTICLE_HPP

