#ifndef PASTEL_CONTAINER_NEWTONIAN_SPH_PARTICLES_HPP
# define PASTEL_CONTAINER_NEWTONIAN_SPH_PARTICLES_HPP

# include <cstddef>
# include <cassert>
# include <vector>
# include <iterator>
# include <algorithm>
# include <utility>
# include <type_traits>
# include <memory>
# include <stdexcept>
# include <initializer_list>

# include <boost/iterator/iterator_facade.hpp>

# include <pastel/container/mobility_tags.hpp>
# include <pastel/container/tags.hpp>
# include <pastel/container/get.hpp>
# include <pastel/container/num_particles.hpp>
# include <pastel/container/increase_force.hpp>
# include <pastel/container/clear_forces.hpp>
# include <pastel/container/apply_external_forces.hpp>
# include <pastel/container/add_particles.hpp>
# include <pastel/container/copy_particles.hpp>
# include <pastel/container/meta/is_data_accessible.hpp>
# include <pastel/container/meta/size_of.hpp>
# include <pastel/container/detail/copy_nth_property_vector.hpp>
# include <pastel/container/detail/copy_nth_property_scalar.hpp>
# include <pastel/particle/tags.hpp>
# include <pastel/particle/get.hpp>
# include <pastel/particle/newtonian_sph_particle.hpp>
# include <pastel/geometry/vector.hpp>
# include <pastel/geometry/point.hpp>
# include <pastel/geometry/meta/dimension_of.hpp>
# include <pastel/system/index_with_origin.hpp>
# include <pastel/utility/is_nothrow_swappable.hpp>
# include <pastel/utility/is_input_iterator.hpp>
# include <pastel/utility/for_.hpp>


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
      typename Vector = ::pastel::geometry::vector<dimension_, Value>,
      typename PointAllocator = std::allocator<Point>,
      typename VectorAllocator = std::allocator<Vector>,
      typename ScalarAllocator = std::allocator<Value>>
    class newtonian_sph_particles;

    namespace newtonian_sph_particles_detail
    {
      template <
        typename Tag, std::size_t dimension_, typename MobilityTag,
        std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
        typename Value, typename Point, typename Vector,
        typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
      struct data;

      template <
        std::size_t dimension_, typename MobilityTag,
        std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
        typename Value, typename Point, typename Vector,
        typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
      struct data<
        ::pastel::particle::tags::position, dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>
      {
        using value_type = Point;
        using pointer = value_type*;
        using const_pointer = value_type const*;
        using particles_type
          = ::pastel::container::newtonian_sph_particles<
              dimension_, MobilityTag,
              num_integration_vectors_, num_property_vectors_, num_property_scalars_,
              Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>;

        static pointer call(particles_type& particles) noexcept { return particles.positions_.data(); }
        static const_pointer call(particles_type const& particles) noexcept { return particles.positions_.data(); }
      }; // struct data< ::pastel::particle::tags::position, dimension_, MobilityTag, num_integration_vectors_, num_property_vectors_, num_property_scalars_, Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>

      template <
        std::size_t dimension_, typename MobilityTag,
        std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
        typename Value, typename Point, typename Vector,
        typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
      struct data<
        ::pastel::particle::tags::velocity, dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>
      {
        using value_type = Vector;
        using pointer = value_type*;
        using const_pointer = value_type const*;
        using particles_type
          = ::pastel::container::newtonian_sph_particles<
              dimension_, MobilityTag,
              num_integration_vectors_, num_property_vectors_, num_property_scalars_,
              Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>;

        static pointer call(particles_type& particles) noexcept { return particles.velocities_.data(); }
        static const_pointer call(particles_type const& particles) noexcept { return particles.velocities_.data(); }
      }; // struct data< ::pastel::particle::tags::velocity, dimension_, MobilityTag, num_integration_vectors_, num_property_vectors_, num_property_scalars_, Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>

      template <
        std::size_t dimension_, typename MobilityTag,
        std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
        typename Value, typename Point, typename Vector,
        typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
      struct data<
        ::pastel::particle::tags::force, dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>
      {
        using value_type = Vector;
        using pointer = value_type*;
        using const_pointer = value_type const*;
        using particles_type
          = ::pastel::container::newtonian_sph_particles<
              dimension_, MobilityTag,
              num_integration_vectors_, num_property_vectors_, num_property_scalars_,
              Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>;

        static pointer call(particles_type& particles) noexcept { return particles.forces_.data(); }
        static const_pointer call(particles_type const& particles) noexcept { return particles.forces_.data(); }
      }; // struct data< ::pastel::particle::tags::force, dimension_, MobilityTag, num_integration_vectors_, num_property_vectors_, num_property_scalars_, Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>

      template <
        std::size_t dimension_, typename MobilityTag,
        std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
        typename Value, typename Point, typename Vector,
        typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
      struct data<
        ::pastel::particle::tags::mass, dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>
      {
        using value_type = Value;
        using pointer = value_type*;
        using const_pointer = value_type const*;
        using particles_type
          = ::pastel::container::newtonian_sph_particles<
              dimension_, MobilityTag,
              num_integration_vectors_, num_property_vectors_, num_property_scalars_,
              Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>;

        static pointer call(particles_type& particles) noexcept { return particles.masses_.data(); }
        static const_pointer call(particles_type const& particles) noexcept { return particles.masses_.data(); }
      }; // struct data< ::pastel::particle::tags::mass, dimension_, MobilityTag, num_integration_vectors_, num_property_vectors_, num_property_scalars_, Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>

      template <
        std::size_t dimension_, typename MobilityTag,
        std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
        typename Value, typename Point, typename Vector,
        typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
      struct data<
        ::pastel::particle::tags::density, dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>
      {
        using value_type = Value;
        using pointer = value_type*;
        using const_pointer = value_type const*;
        using particles_type
          = ::pastel::container::newtonian_sph_particles<
              dimension_, MobilityTag,
              num_integration_vectors_, num_property_vectors_, num_property_scalars_,
              Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>;

        static pointer call(particles_type& particles) noexcept { return particles.densities_.data(); }
        static const_pointer call(particles_type const& particles) noexcept { return particles.densities_.data(); }
      }; // struct data< ::pastel::particle::tags::density, dimension_, MobilityTag, num_integration_vectors_, num_property_vectors_, num_property_scalars_, Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>

      template <
        std::size_t dimension_, typename MobilityTag,
        std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
        typename Value, typename Point, typename Vector,
        typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
      struct data<
        ::pastel::particle::tags::sph_divergence, dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>
      {
        using value_type = Value;
        using pointer = value_type*;
        using const_pointer = value_type const*;
        using particles_type
          = ::pastel::container::newtonian_sph_particles<
              dimension_, MobilityTag,
              num_integration_vectors_, num_property_vectors_, num_property_scalars_,
              Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>;

        static pointer call(particles_type& particles) noexcept { return particles.sph_divergences_.data(); }
        static const_pointer call(particles_type const& particles) noexcept { return particles.sph_divergences_.data(); }
      }; // struct data< ::pastel::particle::tags::sph_divergence, dimension_, MobilityTag, num_integration_vectors_, num_property_vectors_, num_property_scalars_, Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>

      template <
        std::size_t dimension_, typename MobilityTag,
        std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
        typename Value, typename Point, typename Vector,
        typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
      struct data<
        ::pastel::particle::tags::pressure, dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>
      {
        using value_type = Value;
        using pointer = value_type*;
        using const_pointer = value_type const*;
        using particles_type
          = ::pastel::container::newtonian_sph_particles<
              dimension_, MobilityTag,
              num_integration_vectors_, num_property_vectors_, num_property_scalars_,
              Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>;

        static pointer call(particles_type& particles) noexcept { return particles.pressures_.data(); }
        static const_pointer call(particles_type const& particles) noexcept { return particles.pressures_.data(); }
      }; // struct data< ::pastel::particle::tags::pressure, dimension_, MobilityTag, num_integration_vectors_, num_property_vectors_, num_property_scalars_, Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>

      template <
        int n, std::size_t dimension_, typename MobilityTag,
        std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
        typename Value, typename Point, typename Vector,
        typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
      struct data<
        ::pastel::container::tags::nth_integration_vector<n>, dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>
      {
        using value_type = Vector;
        using pointer = value_type*;
        using const_pointer = value_type const*;
        using particles_type
          = ::pastel::container::newtonian_sph_particles<
              dimension_, MobilityTag,
              num_integration_vectors_, num_property_vectors_, num_property_scalars_,
              Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>;

        static pointer call(particles_type& particles) noexcept { return particles.integration_vectors_[n].data(); }
        static const_pointer call(particles_type const& particles) noexcept { return particles.integration_vectors_[n].data(); }
      }; // struct data< ::pastel::container::tags::nth_property_vector<n>, dimension_, MobilityTag, num_integration_vectors_, num_property_vectors_, num_property_scalars_, Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>

      template <
        int n, std::size_t dimension_, typename MobilityTag,
        std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
        typename Value, typename Point, typename Vector,
        typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
      struct data<
        ::pastel::container::tags::nth_property_vector<n>, dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>
      {
        using value_type = Vector;
        using pointer = value_type*;
        using const_pointer = value_type const*;
        using particles_type
          = ::pastel::container::newtonian_sph_particles<
              dimension_, MobilityTag,
              num_integration_vectors_, num_property_vectors_, num_property_scalars_,
              Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>;

        static pointer call(particles_type& particles) noexcept { return particles.property_vectors_[n].data(); }
        static const_pointer call(particles_type const& particles) noexcept { return particles.property_vectors_[n].data(); }
      }; // struct data< ::pastel::container::tags::nth_property_vector<n>, dimension_, MobilityTag, num_integration_vectors_, num_property_vectors_, num_property_scalars_, Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>

      template <
        int n, std::size_t dimension_, typename MobilityTag,
        std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
        typename Value, typename Point, typename Vector,
        typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
      struct data<
        ::pastel::container::tags::nth_property_scalar<n>, dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>
      {
        using value_type = Value;
        using pointer = value_type*;
        using const_pointer = value_type const*;
        using particles_type
          = ::pastel::container::newtonian_sph_particles<
              dimension_, MobilityTag,
              num_integration_vectors_, num_property_vectors_, num_property_scalars_,
              Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>;

        static pointer call(particles_type& particles) noexcept { return particles.property_scalars_[n].data(); }
        static const_pointer call(particles_type const& particles) noexcept { return particles.property_scalars_[n].data(); }
      }; // struct data< ::pastel::container::tags::nth_property_scalar<n>, dimension_, MobilityTag, num_integration_vectors_, num_property_vectors_, num_property_scalars_, Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>


      template <typename Particles>
      class iterator_common final
        : public boost::iterators::iterator_facade<
            iterator_common<Particles>,
            typename Particles::value_type,
            std::random_access_iterator_tag,
            typename Particles::value_type,
            typename Particles::difference_type>
      {
        using particle_type = typename Particles::value_type;
        Particles* particles_ptr_;
        std::size_t index_;

        friend class boost::iterators::iterator_core_access;
        template <typename> friend class iterator_common;

       public:
        constexpr iterator_common() noexcept
          : particles_ptr_{nullptr}, index_{}
        { }

        constexpr iterator_common(Particles& particles, std::size_t index) noexcept
          : particles_ptr_{std::addressof(particles)}, index_{index}
        { }

        template <typename Particles_, typename = typename std::enable_if<std::is_convertible<Particles_*, Particles*>::value>::type>
        constexpr iterator_common(::pastel::container::newtonian_sph_particles_detail::iterator_common<Particles_> const& other) noexcept
          : particles_ptr_{other.particles_ptr_}, index_{other.index_}
        { }

        particle_type dereference() const
        {
          return {
            ::pastel::container::get< ::pastel::particle::tags::position >(*particles_ptr_, index_),
            ::pastel::container::get< ::pastel::particle::tags::velocity >(*particles_ptr_, index_),
            ::pastel::container::get< ::pastel::particle::tags::force >(*particles_ptr_, index_),
            ::pastel::container::get< ::pastel::particle::tags::mass >(*particles_ptr_, index_),
            ::pastel::container::get< ::pastel::particle::tags::density >(*particles_ptr_, index_),
            ::pastel::container::get< ::pastel::particle::tags::sph_divergence >(*particles_ptr_, index_),
            ::pastel::container::get< ::pastel::particle::tags::pressure >(*particles_ptr_, index_)};
        }

        bool equal(iterator_common const& other) const
        {
# ifndef NDEBUG
          return particles_ptr_ == other.particles_ptr_ && index_ == other.index_;
# else
          return index_ == other.index_;
# endif
        }
        template <typename Particles_>
        bool equal(::pastel::container::newtonian_sph_particles_detail::iterator_common<Particles_> const& other) const
        {
# ifndef NDEBUG
          return particles_ptr_ == other.particles_ptr_ && index_ == other.index_;
# else
          return index_ == other.index_;
# endif
        }

        iterator_common& increment() { ++index_; return *this; }
        iterator_common& decrement() { --index_; return *this; }
        iterator_common& advance(typename Particles::difference_type n) { index_ += n; return *this; }
        typename Particles::difference_type distance_to(iterator_common const& other) const { return other.index_ - index_; }

        void swap(iterator_common& other) noexcept
        {
          using std::swap;
          swap(particles_ptr_, other.particles_ptr_);
          swap(index_, other.index_);
        }

        std::size_t index() const { return index_; }
      }; // class iterator_common<Particles>

      template <typename Particles>
      inline void swap(
        ::pastel::container::newtonian_sph_particles_detail::iterator_common<Particles>& lhs,
        ::pastel::container::newtonian_sph_particles_detail::iterator_common<Particles>& rhs)
        noexcept
      { lhs.swap(rhs); }

      template<
        std::size_t dimension_, typename MobilityTag,
        std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
        typename Value, typename Point, typename Vector,
        typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
      using iterator
        = ::pastel::container::newtonian_sph_particles_detail::iterator_common<
            ::pastel::container::newtonian_sph_particles<
              dimension_, MobilityTag,
              num_integration_vectors_, num_property_vectors_, num_property_scalars_,
              Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>>;
      template<
        std::size_t dimension_, typename MobilityTag,
        std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
        typename Value, typename Point, typename Vector,
        typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
      using const_iterator
        = ::pastel::container::newtonian_sph_particles_detail::iterator_common<
            ::pastel::container::newtonian_sph_particles<
              dimension_, MobilityTag,
              num_integration_vectors_, num_property_vectors_, num_property_scalars_,
              Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator> const>;
    } // namespace newtonian_sph_particles_detail


    template <
      std::size_t dimension_, typename MobilityTag,
      std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
      typename Value, typename Point, typename Vector,
      typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
    class newtonian_sph_particles final
    {
      static_assert(
        ::pastel::geometry::meta::dimension_of<Point>::value == dimension_,
        "Dimension of Point must be equal to dimension_");
      static_assert(
        ::pastel::geometry::meta::dimension_of<Vector>::value == dimension_,
        "Dimension of Vector must be equal to dimension_");

     public:
      static constexpr std::size_t dimension = dimension_;
      using mobility_tag = MobilityTag;
      static constexpr std::size_t num_integration_vectors = num_integration_vectors_;
      static constexpr std::size_t num_property_vectors = num_property_vectors_;
      static constexpr std::size_t num_property_scalars = num_property_scalars_;
      static constexpr bool has_mass = true;
      static constexpr bool is_data_accessible = true;
      using point_type = Point;
      using vector_type = Vector;
      using scalar_type = Value;
      using orientation_type = void;
      using point_allocator_type = PointAllocator;
      using vector_allocator_type = VectorAllocator;
      using scalar_allocator_type = ScalarAllocator;
      using boundary_container_type
        = ::pastel::container::newtonian_sph_particles<
            dimension_, ::pastel::container::mobility_tags::immobile,
            0u, num_property_vectors_, num_property_scalars_,
            Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>;

     private:
      using points_type = std::vector<Point, point_allocator_type>;
      using vectors_type = std::vector<Vector, vector_allocator_type>;
      using scalars_type = std::vector<Value, scalar_allocator_type>;
      points_type positions_;
      vectors_type velocities_;
      vectors_type forces_;
      scalars_type masses_;
      scalars_type densities_;
      scalars_type sph_divergences_;
      scalars_type pressures_;

      vectors_type integration_vectors_[num_integration_vectors];
      vectors_type property_vectors_[num_property_vectors];
      scalars_type property_scalars_[num_property_scalars];

     public:
      using value_type
        = ::pastel::particle::newtonian_sph_particle<dimension_, Value, Point, Vector>;
      using size_type = typename points_type::size_type;
      using difference_type = typename points_type::difference_type;
      using iterator
        = ::pastel::container::newtonian_sph_particles_detail::iterator<
            dimension_, MobilityTag,
            num_integration_vectors_, num_property_vectors_, num_property_scalars_,
            Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>;
      using const_iterator
        = ::pastel::container::newtonian_sph_particles_detail::const_iterator<
            dimension_, MobilityTag,
            num_integration_vectors_, num_property_vectors_, num_property_scalars_,
            Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>;
      using reverse_iterator = std::reverse_iterator<iterator>;
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;

      newtonian_sph_particles()
        noexcept(
          std::is_nothrow_default_constructible<points_type>::value
          && std::is_nothrow_default_constructible<vectors_type>::value
          && std::is_nothrow_default_constructible<scalars_type>::value)
        = default;
      ~newtonian_sph_particles()
        noexcept(
          std::is_nothrow_destructible<points_type>::value
          && std::is_nothrow_destructible<vectors_type>::value
          && std::is_nothrow_destructible<scalars_type>::value)
        = default;
      newtonian_sph_particles(newtonian_sph_particles const&) = default;
      newtonian_sph_particles(newtonian_sph_particles&&)
        noexcept(
          std::is_nothrow_move_constructible<points_type>::value
          && std::is_nothrow_move_constructible<vectors_type>::value
          && std::is_nothrow_move_constructible<scalars_type>::value)
        = default;
      newtonian_sph_particles& operator=(newtonian_sph_particles const&) & = default;
      newtonian_sph_particles& operator=(newtonian_sph_particles&&) &
        noexcept(
          std::is_nothrow_move_assignable<points_type>::value
          && std::is_nothrow_move_assignable<vectors_type>::value
          && std::is_nothrow_move_assignable<scalars_type>::value)
        = default;

      newtonian_sph_particles(
        PointAllocator const& point_allocator, VectorAllocator const& vector_allocator, ScalarAllocator const& scalar_allocator)
        noexcept(noexcept(points_type{point_allocator}) && noexcept(vectors_type{vector_allocator}) && noexcept(scalars_type{scalar_allocator}))
        : positions_{point_allocator},
          velocities_{vector_allocator},
          forces_{vector_allocator},
          masses_{scalar_allocator},
          densities_{scalar_allocator},
          sph_divergences_{scalar_allocator},
          pressures_{scalar_allocator},
          integration_vectors_{},
          property_vectors_{},
          property_scalars_{}
      {
        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index] = vectors_type{vector_allocator};
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index] = vectors_type{vector_allocator};
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index] = scalars_type{scalar_allocator};
      }

      explicit newtonian_sph_particles(size_type count)
        : positions_(count),
          velocities_(count),
          forces_(count),
          masses_(count),
          densities_(count),
          sph_divergences_(count),
          pressures_(count),
          integration_vectors_{},
          property_vectors_{},
          property_scalars_{}
      {
        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index] = vectors_type(count);
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index] = vectors_type(count);
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index] = scalars_type(count);
      }

      newtonian_sph_particles(size_type count, value_type const& particle)
        : positions_(count, ::pastel::particle::get< ::pastel::particle::tags::position >(particle)),
          velocities_(count, ::pastel::particle::get< ::pastel::particle::tags::velocity >(particle)),
          forces_(count, ::pastel::particle::get< ::pastel::particle::tags::force >(particle)),
          masses_(count, ::pastel::particle::get< ::pastel::particle::tags::mass >(particle)),
          densities_(count, ::pastel::particle::get< ::pastel::particle::tags::density >(particle)),
          sph_divergences_(count, ::pastel::particle::get< ::pastel::particle::tags::sph_divergence >(particle)),
          pressures_(count, ::pastel::particle::get< ::pastel::particle::tags::pressure >(particle)),
          integration_vectors_{},
          property_vectors_{},
          property_scalars_{}
      {
        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index] = vectors_type(count);
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index] = vectors_type(count);
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index] = scalars_type(count);
      }

      /* Since C++14
      newtonian_sph_particles(
        size_type count,
        PointAllocator const& point_allocator, VectorAllocator const& vector_allocator, ScalarAllocator const& scalar_allocator)
        : positions_(count, point_allocator),
          velocities_(count, vector_allocator),
          forces_(count, vector_allocator),
          masses_(count, scalar_allocator),
          densities_(count, scalar_allocator),
          sph_divergences_(count, scalar_allocator),
          pressures_(count, scalar_allocator),
          integration_vectors_{},
          property_vectors_{},
          property_scalars_{}
      {
        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index] = vectors_type(count, vector_allocator);
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index] = vectors_type(count, vector_allocator);
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index] = scalars_type(count, scalar_allocator);
      }
      */

      newtonian_sph_particles(
        size_type count, value_type const& particle,
        PointAllocator const& point_allocator, VectorAllocator const& vector_allocator, ScalarAllocator const& scalar_allocator)
        : positions_(count, ::pastel::particle::get< ::pastel::particle::tags::position >(particle), point_allocator),
          velocities_(count, ::pastel::particle::get< ::pastel::particle::tags::velocity >(particle), vector_allocator),
          forces_(count, ::pastel::particle::get< ::pastel::particle::tags::force >(particle), vector_allocator),
          masses_(count, ::pastel::particle::get< ::pastel::particle::tags::mass >(particle), scalar_allocator),
          densities_(count, ::pastel::particle::get< ::pastel::particle::tags::density >(particle), scalar_allocator),
          sph_divergences_(count, ::pastel::particle::get< ::pastel::particle::tags::sph_divergence >(particle), scalar_allocator),
          pressures_(count, ::pastel::particle::get< ::pastel::particle::tags::pressure >(particle), scalar_allocator),
          integration_vectors_{},
          property_vectors_{},
          property_scalars_{}
      {
        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index] = vectors_type(count, vector_allocator);
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index] = vectors_type(count, vector_allocator);
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index] = scalars_type(count, scalar_allocator);
      }

      template <
        typename Iterator,
        typename = typename std::enable_if< ::pastel::utility::is_input_iterator<Iterator>::value >::type>
      newtonian_sph_particles(Iterator first, Iterator last)
        : positions_{},
          velocities_{},
          forces_{},
          masses_{},
          densities_{},
          sph_divergences_{},
          pressures_{},
          integration_vectors_{},
          property_vectors_{},
          property_scalars_{}
      { assign(first, last); }

      template <
        typename Iterator,
        typename = typename std::enable_if< ::pastel::utility::is_input_iterator<Iterator>::value >::type>
      newtonian_sph_particles(
        Iterator first, Iterator last,
        PointAllocator const& point_allocator, VectorAllocator const& vector_allocator, ScalarAllocator const& scalar_allocator)
        : positions_{point_allocator},
          velocities_{vector_allocator},
          forces_{vector_allocator},
          masses_{scalar_allocator},
          densities_{scalar_allocator},
          sph_divergences_{scalar_allocator},
          pressures_{scalar_allocator},
          integration_vectors_{},
          property_vectors_{},
          property_scalars_{}
      {
        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index] = vectors_type{vector_allocator};
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index] = vectors_type{vector_allocator};
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index] = scalars_type{scalar_allocator};
        assign(first, last);
      }

      newtonian_sph_particles(
        newtonian_sph_particles const& other,
        PointAllocator const& point_allocator, VectorAllocator const& vector_allocator, ScalarAllocator const& scalar_allocator)
        : positions_{other.positions_, point_allocator},
          velocities_{other.velocities_, vector_allocator},
          forces_{other.forces_, vector_allocator},
          masses_{other.masses_, scalar_allocator},
          densities_{other.densities_, scalar_allocator},
          sph_divergences_{other.sph_divergences_, scalar_allocator},
          pressures_{other.pressures_, scalar_allocator},
          integration_vectors_{},
          property_vectors_{},
          property_scalars_{}
      {
        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index] = vectors_type{other.integration_vectors_[index], vector_allocator};
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index] = vectors_type{other.property_vectors_[index], vector_allocator};
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index] = scalars_type{other.property_scalars_[index], scalar_allocator};
      }

      newtonian_sph_particles(
        newtonian_sph_particles&& other,
        PointAllocator const& point_allocator, VectorAllocator const& vector_allocator, ScalarAllocator const& scalar_allocator)
        : positions_{std::move(other.positions_), point_allocator},
          velocities_{std::move(other.velocities_), vector_allocator},
          forces_{std::move(other.forces_), vector_allocator},
          masses_{std::move(other.masses_), scalar_allocator},
          densities_{std::move(other.densities_), scalar_allocator},
          sph_divergences_{std::move(other.sph_divergences_), scalar_allocator},
          pressures_{std::move(other.pressures_), scalar_allocator},
          integration_vectors_{},
          property_vectors_{},
          property_scalars_{}
      {
        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index] = vectors_type{std::move(other.integration_vectors_[index]), vector_allocator};
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index] = vectors_type{std::move(other.property_vectors_[index]), vector_allocator};
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index] = scalars_type{std::move(other.property_scalars_[index]), scalar_allocator};
      }

      newtonian_sph_particles(std::initializer_list<value_type> initializer_list)
        : positions_{},
          velocities_{},
          forces_{},
          masses_{},
          densities_{},
          sph_divergences_{},
          pressures_{},
          integration_vectors_{},
          property_vectors_{},
          property_scalars_{}
      { assign(initializer_list); }

      newtonian_sph_particles(
        std::initializer_list<value_type> initializer_list,
        PointAllocator const& point_allocator, VectorAllocator const& vector_allocator, ScalarAllocator const& scalar_allocator)
        : positions_{point_allocator},
          velocities_{vector_allocator},
          forces_{vector_allocator},
          masses_{scalar_allocator},
          densities_{scalar_allocator},
          sph_divergences_{scalar_allocator},
          pressures_{scalar_allocator},
          integration_vectors_{},
          property_vectors_{},
          property_scalars_{}
      {
        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index] = vectors_type{vector_allocator};
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index] = vectors_type{vector_allocator};
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index] = scalars_type{scalar_allocator};
        assign(initializer_list);
      }

      newtonian_sph_particles& operator=(std::initializer_list<value_type> initializer_list)
      { assign(initializer_list); return *this; }


      void assign(size_type count, value_type const& particle)
      {
        positions_.assign(count, ::pastel::particle::get< ::pastel::particle::tags::position >(particle));
        velocities_.assign(count, ::pastel::particle::get< ::pastel::particle::tags::velocity >(particle));
        forces_.assign(count, ::pastel::particle::get< ::pastel::particle::tags::force >(particle));
        masses_.assign(count, ::pastel::particle::get< ::pastel::particle::tags::mass >(particle));
        densities_.assign(count, ::pastel::particle::get< ::pastel::particle::tags::density >(particle));
        sph_divergences_.assign(count, ::pastel::particle::get< ::pastel::particle::tags::sph_divergence >(particle));
        pressures_.assign(count, ::pastel::particle::get< ::pastel::particle::tags::pressure >(particle));

        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].assign(count, vector_type{});
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].assign(count, vector_type{});
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].assign(count, scalar_type{});
      }

     private:
      template <typename Iterator>
      void assign_from_iterators(Iterator first, Iterator last, std::input_iterator_tag)
      {
        positions_.clear();
        velocities_.clear();
        forces_.clear();
        masses_.clear();
        densities_.clear();
        sph_divergences_.clear();
        pressures_.clear();

        for (; first != last; ++first)
        {
          positions_.push_back(::pastel::particle::get< ::pastel::particle::tags::position >(*first));
          velocities_.push_back(::pastel::particle::get< ::pastel::particle::tags::velocity >(*first));
          forces_.push_back(::pastel::particle::get< ::pastel::particle::tags::force >(*first));
          masses_.push_back(::pastel::particle::get< ::pastel::particle::tags::mass >(*first));
          densities_.push_back(::pastel::particle::get< ::pastel::particle::tags::density >(*first));
          sph_divergences_.push_back(::pastel::particle::get< ::pastel::particle::tags::sph_divergence >(*first));
          pressures_.push_back(::pastel::particle::get< ::pastel::particle::tags::pressure >(*first));
        }

        positions_.shrink_to_fit();
        velocities_.shrink_to_fit();
        forces_.shrink_to_fit();
        masses_.shrink_to_fit();
        densities_.shrink_to_fit();
        sph_divergences_.shrink_to_fit();
        pressures_.shrink_to_fit();
      }

      template <typename Iterator>
      void assign_from_iterators(Iterator first, Iterator last, std::forward_iterator_tag)
      {
        positions_.clear();
        velocities_.clear();
        forces_.clear();
        masses_.clear();
        densities_.clear();
        sph_divergences_.clear();
        pressures_.clear();

        auto const count = std::distance(first, last);
        assert(count >= 0);
        positions_.reserve(count);
        velocities_.reserve(count);
        forces_.reserve(count);
        masses_.reserve(count);
        densities_.reserve(count);
        sph_divergences_.reserve(count);
        pressures_.reserve(count);

        for (; first != last; ++first)
        {
          positions_.push_back(::pastel::particle::get< ::pastel::particle::tags::position >(*first));
          velocities_.push_back(::pastel::particle::get< ::pastel::particle::tags::velocity >(*first));
          forces_.push_back(::pastel::particle::get< ::pastel::particle::tags::force >(*first));
          masses_.push_back(::pastel::particle::get< ::pastel::particle::tags::mass >(*first));
          densities_.push_back(::pastel::particle::get< ::pastel::particle::tags::density >(*first));
          sph_divergences_.push_back(::pastel::particle::get< ::pastel::particle::tags::sph_divergence >(*first));
          pressures_.push_back(::pastel::particle::get< ::pastel::particle::tags::pressure >(*first));
        }
      }

     public:
      template <typename Iterator, typename = typename std::enable_if< ::pastel::utility::is_input_iterator<Iterator>::value >::type>
      void assign(Iterator first, Iterator last)
      {
        assign_from_iterators(first, last, typename std::iterator_traits<Iterator>::iterator_category());

        auto const count = positions_.size();
        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].assign(count, vector_type{});
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].assign(count, vector_type{});
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].assign(count, scalar_type{});
      }

      void assign(std::initializer_list<value_type> initializer_list)
      { assign(std::begin(initializer_list), std::end(initializer_list)); }

      point_allocator_type get_point_allocator() const { return positions_.get_allocator(); }
      vector_allocator_type get_vector_allocator() const { return velocities_.get_allocator(); }
      scalar_allocator_type get_scalar_allocator() const { return masses_.get_allocator(); }


      // Element access
     private:
      template <typename Tag, std::size_t, typename, std::size_t, std::size_t, std::size_t, typename, typename, typename, typename, typename, typename>
      friend class ::pastel::container::newtonian_sph_particles_detail::data;

      template <typename Tag>
      using newtonian_sph_particles_data
        = ::pastel::container::newtonian_sph_particles_detail::data<
            Tag, dimension, MobilityTag, num_integration_vectors, num_property_vectors, num_property_scalars,
            Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>;

     public:
      template <typename Tag>
      typename newtonian_sph_particles_data<Tag>::pointer data() & noexcept
      { return newtonian_sph_particles_data<Tag>::call(*this); }

      template <typename Tag>
      typename newtonian_sph_particles_data<Tag>::const_pointer data() const& noexcept
      { return newtonian_sph_particles_data<Tag>::call(*this); }

      template <typename Tag>
      auto get(std::size_t index) & noexcept -> decltype(data<Tag>()[index])
      { assert(index < positions_.size()); return data<Tag>()[index]; }

      template <typename Tag>
      auto get(std::size_t index) const& noexcept -> decltype(data<Tag>()[index])
      { assert(index < positions_.size()); return data<Tag>()[index]; }

      template <typename Tag>
      auto at(std::size_t index) & noexcept -> decltype(data<Tag>()[index])
      {
        if (index >= positions_.size())
          throw std::out_of_range("newtonian_sph_particles::at: index >= positions_.size()");
        return data<Tag>()[index];
      }

      template <typename Tag>
      auto at(std::size_t index) const& noexcept -> decltype(data<Tag>()[index])
      {
        if (index >= positions_.size())
          throw std::out_of_range("newtonian_sph_particles::at: index >= positions_.size()");
        return data<Tag>()[index];
      }

      template <typename Tag>
      auto front() & noexcept -> decltype(data<Tag>()[0u])
      { return data<Tag>()[0u]; }

      template <typename Tag>
      auto front() const& noexcept -> decltype(data<Tag>()[0u])
      { return data<Tag>()[0u]; }

      template <typename Tag>
      auto back() & noexcept -> decltype(data<Tag>()[positions_.size()-1u])
      { return data<Tag>()[positions_.size()-1u]; }

      template <typename Tag>
      auto back() const& noexcept -> decltype(data<Tag>()[positions_.size()-1u])
      { return data<Tag>()[positions_.size()-1u]; }

      // Iterators
      iterator begin() { return {*this, 0u}; }
      const_iterator begin() const { return {*this, 0u}; }
      const_iterator cbegin() const { return {*this, 0u}; }
      iterator end() { return {*this, positions_.size()}; }
      const_iterator end() const { return {*this, positions_.size()}; }
      const_iterator cend() const { return {*this, positions_.size()}; }
      reverse_iterator rbegin() { return reverse_iterator{end()}; }
      const_reverse_iterator rbegin() const { return const_reverse_iterator{end()}; }
      const_reverse_iterator crbegin() const { return const_reverse_iterator{cend()}; }
      reverse_iterator rend() { return reverse_iterator{begin()}; }
      const_reverse_iterator rend() const { return const_reverse_iterator{begin()}; }
      const_reverse_iterator crend() const { return const_reverse_iterator{cbegin()}; }

      // Capacity
      bool empty() const noexcept { return positions_.empty(); }
      size_type size() const noexcept { return positions_.size(); }
      size_type max_size() const noexcept
      {
        struct dummy_type
        {
          value_type particle_;
          vector_type integration_vector[num_integration_vectors];
          vector_type property_vector[num_property_vectors];
          scalar_type property_scalar[num_property_scalars]; 
        };
        return std::vector<dummy_type>{}.max_size();
      }
      void reserve(size_type new_capacity)
      {
        positions_.reserve(new_capacity);
        velocities_.reserve(new_capacity);
        forces_.reserve(new_capacity);
        masses_.reserve(new_capacity);
        densities_.reserve(new_capacity);
        sph_divergences_.reserve(new_capacity);
        pressures_.reserve(new_capacity);

        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].reserve(new_capacity);
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].reserve(new_capacity);
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].reserve(new_capacity);
      }
      size_type capacity() const noexcept { return positions_.capacity(); }
      void shrink_to_fit()
      {
        positions_.shrink_to_fit();
        velocities_.shrink_to_fit();
        forces_.shrink_to_fit();
        masses_.shrink_to_fit();
        densities_.shrink_to_fit();
        sph_divergences_.shrink_to_fit();
        pressures_.shrink_to_fit();

        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].shrink_to_fit();
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].shrink_to_fit();
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].shrink_to_fit();
      }

      // Modifiers
      void clear() noexcept
      {
        positions_.clear();
        velocities_.clear();
        forces_.clear();
        masses_.clear();
        densities_.clear();
        sph_divergences_.clear();
        pressures_.clear();

        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].clear();
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].clear();
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].clear();
      }

      iterator insert(const_iterator pos, value_type const& particle)
      {
        auto const pos_index = pos.index();
        auto const new_position_iter
          = positions_.insert(std::begin(positions_) + pos_index, ::pastel::particle::get< ::pastel::particle::tags::position >(particle));
        velocities_.insert(std::begin(velocities_) + pos_index, ::pastel::particle::get< ::pastel::particle::tags::velocity >(particle));
        forces_.insert(std::begin(forces_) + pos_index, ::pastel::particle::get< ::pastel::particle::tags::force >(particle));
        masses_.insert(std::begin(masses_) + pos_index, ::pastel::particle::get< ::pastel::particle::tags::mass >(particle));
        densities_.insert(std::begin(densities_) + pos_index, ::pastel::particle::get< ::pastel::particle::tags::density >(particle));
        sph_divergences_.insert(std::begin(sph_divergences_) + pos_index, ::pastel::particle::get< ::pastel::particle::tags::sph_divergence >(particle));
        pressures_.insert(std::begin(pressures_) + pos_index, ::pastel::particle::get< ::pastel::particle::tags::pressure >(particle));

        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].emplace(std::begin(integration_vectors_[index]) + pos_index);
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].emplace(std::begin(property_vectors_[index]) + pos_index);
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].emplace(std::begin(property_scalars_[index]) + pos_index);

        return {*this, static_cast<size_type>(new_position_iter - std::begin(positions_))};
      }

      iterator insert(const_iterator pos, value_type&& particle)
      {
        auto const pos_index = pos.index();
        auto const new_position_iter
          = positions_.insert(std::begin(positions_) + pos_index, std::move(::pastel::particle::get< ::pastel::particle::tags::position >(particle)));
        velocities_.insert(std::begin(velocities_) + pos_index, std::move(::pastel::particle::get< ::pastel::particle::tags::velocity >(particle)));
        forces_.insert(std::begin(forces_) + pos_index, std::move(::pastel::particle::get< ::pastel::particle::tags::force >(particle)));
        masses_.insert(std::begin(masses_) + pos_index, std::move(::pastel::particle::get< ::pastel::particle::tags::mass >(particle)));
        densities_.insert(std::begin(densities_) + pos_index, std::move(::pastel::particle::get< ::pastel::particle::tags::density >(particle)));
        sph_divergences_.insert(std::begin(sph_divergences_) + pos_index, std::move(::pastel::particle::get< ::pastel::particle::tags::sph_divergence >(particle)));
        pressures_.insert(std::begin(pressures_) + pos_index, std::move(::pastel::particle::get< ::pastel::particle::tags::pressure >(particle)));

        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].emplace(std::begin(integration_vectors_[index]) + pos_index);
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].emplace(std::begin(property_vectors_[index]) + pos_index);
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].emplace(std::begin(property_scalars_[index]) + pos_index);

        return {*this, static_cast<size_type>(new_position_iter - std::begin(positions_))};
      }

      iterator insert(const_iterator pos, size_type count, value_type const& particle)
      {
        auto const pos_index = pos.index();
        auto const new_position_iter
          = positions_.insert(std::begin(positions_) + pos_index, count, ::pastel::particle::get< ::pastel::particle::tags::position >(particle));
        velocities_.insert(std::begin(velocities_) + pos_index, count, ::pastel::particle::get< ::pastel::particle::tags::velocity >(particle));
        forces_.insert(std::begin(forces_) + pos_index, count, ::pastel::particle::get< ::pastel::particle::tags::force >(particle));
        masses_.insert(std::begin(masses_) + pos_index, count, ::pastel::particle::get< ::pastel::particle::tags::mass >(particle));
        densities_.insert(std::begin(densities_) + pos_index, count, ::pastel::particle::get< ::pastel::particle::tags::density >(particle));
        sph_divergences_.insert(std::begin(sph_divergences_) + pos_index, count, ::pastel::particle::get< ::pastel::particle::tags::sph_divergence >(particle));
        pressures_.insert(std::begin(pressures_) + pos_index, count, ::pastel::particle::get< ::pastel::particle::tags::pressure >(particle));

        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].insert(std::begin(integration_vectors_[index]) + pos_index, count, vector_type{});
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].insert(std::begin(property_vectors_[index]) + pos_index, count, vector_type{});
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].insert(std::begin(property_scalars_[index]) + pos_index, count, scalar_type{});

        return {*this, static_cast<size_type>(new_position_iter - std::begin(positions_))};
      }

      template <typename Iterator, typename = typename std::enable_if< ::pastel::utility::is_input_iterator<Iterator>::value >::type>
      iterator insert(const_iterator pos, Iterator first, Iterator last)
      {
        auto const pos_index = pos.index();
        auto position_iter = std::begin(positions_) + pos_index;
        auto velocity_iter = std::begin(velocities_) + pos_index;
        auto force_iter = std::begin(forces_) + pos_index;
        auto mass_iter = std::begin(masses_) + pos_index;
        auto density_iter = std::begin(densities_) + pos_index;
        auto sph_divergence_iter = std::begin(sph_divergences_) + pos_index;
        auto pressure_iter = std::begin(pressures_) + pos_index;
        for (; first != last; ++first)
        {
          position_iter = ++positions_.insert(position_iter, ::pastel::particle::get< ::pastel::particle::tags::position >(*first));
          velocity_iter = ++velocities_.insert(velocity_iter, ::pastel::particle::get< ::pastel::particle::tags::velocity >(*first));
          force_iter = ++forces_.insert(force_iter, ::pastel::particle::get< ::pastel::particle::tags::force >(*first));
          mass_iter = ++masses_.insert(mass_iter, ::pastel::particle::get< ::pastel::particle::tags::mass >(*first));
          density_iter = ++densities_.insert(density_iter, ::pastel::particle::get< ::pastel::particle::tags::density >(*first));
          sph_divergence_iter = ++sph_divergences_.insert(sph_divergence_iter, ::pastel::particle::get< ::pastel::particle::tags::sph_divergence >(*first));
          pressure_iter = ++pressures_.insert(pressure_iter, ::pastel::particle::get< ::pastel::particle::tags::pressure >(*first));
        }

        auto const new_pos_index = static_cast<size_type>(position_iter - std::begin(positions_));
        auto const count = new_pos_index - pos_index;
        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].insert(std::begin(integration_vectors_[index]) + pos_index, count, vector_type{});
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].insert(std::begin(property_vectors_[index]) + pos_index, count, vector_type{});
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].insert(std::begin(property_scalars_[index]) + pos_index, count, scalar_type{});

        return {*this, new_pos_index};
      }

      iterator insert(const_iterator pos, std::initializer_list<value_type> initializer_list)
      { return insert(pos, std::begin(initializer_list), std::end(initializer_list)); }

      template <
        typename Position, typename Velocity, typename Force, typename Mass,
        typename Density, typename DensityForce, typename Pressure>
      iterator emplace(
        const_iterator pos, Position&& position, Velocity&& velocity, Force&& force, Mass&& mass,
        Density&& density, DensityForce&& sph_divergence, Pressure&& pressure)
      {
        auto const pos_index = pos.index();
        auto const new_position_iter
          = positions_.insert(std::begin(positions_) + pos_index, std::forward<Position>(position));
        velocities_.insert(std::begin(velocities_) + pos_index, std::forward<Velocity>(velocity));
        forces_.insert(std::begin(forces_) + pos_index, std::forward<Force>(force));
        masses_.insert(std::begin(masses_) + pos_index, std::forward<Mass>(mass));
        densities_.insert(std::begin(densities_) + pos_index, std::forward<Density>(density));
        sph_divergences_.insert(std::begin(sph_divergences_) + pos_index, std::forward<DensityForce>(sph_divergence));
        pressures_.insert(std::begin(pressures_) + pos_index, std::forward<Pressure>(pressure));

        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].emplace(std::begin(integration_vectors_[index]) + pos_index);
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].emplace(std::begin(property_vectors_[index]) + pos_index);
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].emplace(std::begin(property_scalars_[index]) + pos_index);

        return {*this, static_cast<size_type>(new_position_iter - std::begin(positions_))};
      }

      iterator erase(const_iterator pos)
      {
        auto const pos_index = pos.index();
        auto const new_position_iter
          = positions_.erase(std::begin(positions_) + pos_index);
        velocities_.erase(std::begin(velocities_) + pos_index);
        forces_.erase(std::begin(forces_) + pos_index);
        masses_.erase(std::begin(masses_) + pos_index);
        densities_.erase(std::begin(densities_) + pos_index);
        sph_divergences_.erase(std::begin(sph_divergences_) + pos_index);
        pressures_.erase(std::begin(pressures_) + pos_index);

        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].erase(std::begin(integration_vectors_[index]) + pos_index);
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].erase(std::begin(property_vectors_[index]) + pos_index);
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].erase(std::begin(property_scalars_[index]) + pos_index);

        return {*this, static_cast<size_type>(new_position_iter - std::begin(positions_))};
      }

      iterator erase(const_iterator first, const_iterator last)
      {
        auto const first_index = first.index();
        auto const last_index = last.index();
        auto const new_position_iter
          = positions_.erase(std::begin(positions_) + first_index, std::begin(positions_) + last_index);
        velocities_.erase(std::begin(velocities_) + first_index, std::begin(velocities_) + last_index);
        forces_.erase(std::begin(forces_) + first_index, std::begin(forces_) + last_index);
        masses_.erase(std::begin(masses_) + first_index, std::begin(masses_) + last_index);
        densities_.erase(std::begin(densities_) + first_index, std::begin(densities_) + last_index);
        sph_divergences_.erase(std::begin(sph_divergences_) + first_index, std::begin(sph_divergences_) + last_index);
        pressures_.erase(std::begin(pressures_) + first_index, std::begin(pressures_) + last_index);

        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].erase(
            std::begin(integration_vectors_[index]) + first_index,
            std::begin(integration_vectors_[index]) + last_index);
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].erase(
            std::begin(property_vectors_[index]) + first_index,
            std::begin(property_vectors_[index]) + last_index);
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].erase(
            std::begin(property_scalars_[index]) + first_index,
            std::begin(property_vectors_[index]) + last_index);

        return {*this, static_cast<size_type>(new_position_iter - std::begin(positions_))};
      }

      void push_back(value_type const& particle)
      {
        positions_.push_back(::pastel::particle::get< ::pastel::particle::tags::position >(particle));
        velocities_.push_back(::pastel::particle::get< ::pastel::particle::tags::velocity >(particle));
        forces_.push_back(::pastel::particle::get< ::pastel::particle::tags::force >(particle));
        masses_.push_back(::pastel::particle::get< ::pastel::particle::tags::mass >(particle));
        densities_.push_back(::pastel::particle::get< ::pastel::particle::tags::density >(particle));
        sph_divergences_.push_back(::pastel::particle::get< ::pastel::particle::tags::sph_divergence >(particle));
        pressures_.push_back(::pastel::particle::get< ::pastel::particle::tags::pressure >(particle));

        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].emplace_back();
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].emplace_back();
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].emplace_back();
      }

      void push_back(value_type&& particle)
      {
        positions_.push_back(std::move(::pastel::particle::get< ::pastel::particle::tags::position >(particle)));
        velocities_.push_back(std::move(::pastel::particle::get< ::pastel::particle::tags::velocity >(particle)));
        forces_.push_back(std::move(::pastel::particle::get< ::pastel::particle::tags::force >(particle)));
        masses_.push_back(std::move(::pastel::particle::get< ::pastel::particle::tags::mass >(particle)));
        densities_.push_back(std::move(::pastel::particle::get< ::pastel::particle::tags::density >(particle)));
        sph_divergences_.push_back(std::move(::pastel::particle::get< ::pastel::particle::tags::sph_divergence >(particle)));
        pressures_.push_back(std::move(::pastel::particle::get< ::pastel::particle::tags::pressure >(particle)));

        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].emplace_back();
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].emplace_back();
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].emplace_back();
      }

      template <
        typename Position, typename Velocity, typename Force, typename Mass,
        typename Density, typename DensityForce, typename Pressure>
      void emplace_back(
        Position&& position, Velocity&& velocity, Force&& force, Mass&& mass,
        Density&& density, DensityForce&& sph_divergence, Pressure&& pressure)
      {
        positions_.push_back(std::forward<Position>(position));
        velocities_.push_back(std::forward<Velocity>(velocity));
        forces_.push_back(std::forward<Force>(force));
        masses_.push_back(std::forward<Mass>(mass));
        densities_.push_back(std::forward<Density>(density));
        sph_divergences_.push_back(std::forward<DensityForce>(sph_divergence));
        pressures_.push_back(std::forward<Pressure>(pressure));

        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].emplace_back();
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].emplace_back();
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].emplace_back();
      }

      void pop_back()
      {
        positions_.pop_back();
        velocities_.pop_back();
        forces_.pop_back();
        masses_.pop_back();
        densities_.pop_back();
        sph_divergences_.pop_back();
        pressures_.pop_back();

        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].pop_back();
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].pop_back();
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].pop_back();
      }

      void resize(size_type count)
      {
        positions_.resize(count);
        velocities_.resize(count);
        forces_.resize(count);
        masses_.resize(count);
        densities_.resize(count);
        sph_divergences_.resize(count);
        pressures_.resize(count);

        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].resize(count);
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].resize(count);
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].resize(count);
      }

      void resize(size_type count, value_type const& particle)
      {
        positions_.resize(count, ::pastel::particle::get< ::pastel::particle::tags::position >(particle));
        velocities_.resize(count, ::pastel::particle::get< ::pastel::particle::tags::velocity >(particle));
        forces_.resize(count, ::pastel::particle::get< ::pastel::particle::tags::force >(particle));
        masses_.resize(count, ::pastel::particle::get< ::pastel::particle::tags::mass >(particle));
        densities_.resize(count, ::pastel::particle::get< ::pastel::particle::tags::density >(particle));
        sph_divergences_.resize(count, ::pastel::particle::get< ::pastel::particle::tags::sph_divergence >(particle));
        pressures_.resize(count, ::pastel::particle::get< ::pastel::particle::tags::pressure >(particle));

        for (auto index = std::size_t{0}; index < num_integration_vectors; ++index)
          integration_vectors_[index].resize(count);
        for (auto index = std::size_t{0}; index < num_property_vectors; ++index)
          property_vectors_[index].resize(count);
        for (auto index = std::size_t{0}; index < num_property_scalars; ++index)
          property_scalars_[index].resize(count);
      }

      void swap(newtonian_sph_particles& other)
        noexcept(
          ::pastel::utility::is_nothrow_swappable<points_type>::value
          && ::pastel::utility::is_nothrow_swappable<vectors_type>::value
          && ::pastel::utility::is_nothrow_swappable<scalars_type>::value)
      {
        using std::swap;
        swap(positions_, other.positions_);
        swap(velocities_, other.velocities_);
        swap(forces_, other.forces_);
        swap(masses_, other.masses_);
        swap(densities_, other.densities_);
        swap(sph_divergences_, other.sph_divergences_);
        swap(pressures_, other.pressures_);

        swap(integration_vectors_, other.integration_vectors_);
        swap(property_vectors_, other.property_vectors_);
        swap(property_scalars_, other.property_scalars_);
      }


      bool operator==(newtonian_sph_particles const& other) const
      {
        return
          positions_ == other.positions_
          && velocities_ == other.velocities_
          && forces_ == other.forces_
          && masses_ == other.masses_
          && densities_ == other.densities_
          && sph_divergences_ == other.sph_divergences_
          && pressures_ == other.pressures_
          && std::equal(integration_vectors_, integration_vectors_ + num_integration_vectors, other.integration_vectors_)
          && std::equal(property_vectors_, property_vectors_ + num_property_vectors, other.property_vectors_)
          && std::equal(property_scalars_, property_scalars_ + num_property_scalars, other.property_scalars_);
      }

      bool operator<(newtonian_sph_particles const& other) const
      {
        return
          positions_ < other.positions_
          && velocities_ < other.velocities_
          && forces_ < other.forces_
          && masses_ < other.masses_
          && densities_ < other.densities_
          && sph_divergences_ < other.sph_divergences_
          && pressures_ < other.pressures_
          && std::lexicographical_compare(
               integration_vectors_, integration_vectors_ + num_integration_vectors,
               other.integration_vectors_, other.integration_vectors_ + num_integration_vectors)
          && std::lexicographical_compare(
               property_vectors_, property_vectors_ + num_property_vectors,
               other.property_vectors_, other.property_vectors_ + num_property_vectors)
          && std::lexicographical_compare(
               property_scalars_, property_scalars_ + num_property_scalars,
               other.property_scalars_, other.property_scalars_ + num_property_scalars);
      }
    }; // class newtonian_sph_particles<dimension_, MobilityTag, num_integration_vectors_, num_property_vectors_, num_property_scalars_, Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>

    template <
      std::size_t dimension_, typename MobilityTag,
      std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
      typename Value, typename Point, typename Vector,
      typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
    inline bool operator!=(
      ::pastel::container::newtonian_sph_particles<
        dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator> const& lhs,
      ::pastel::container::newtonian_sph_particles<
        dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator> const& rhs)
    { return !(lhs == rhs); }

    template <
      std::size_t dimension_, typename MobilityTag,
      std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
      typename Value, typename Point, typename Vector,
      typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
    inline bool operator>(
      ::pastel::container::newtonian_sph_particles<
        dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator> const& lhs,
      ::pastel::container::newtonian_sph_particles<
        dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator> const& rhs)
    { return rhs < lhs; }

    template <
      std::size_t dimension_, typename MobilityTag,
      std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
      typename Value, typename Point, typename Vector,
      typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
    inline bool operator<=(
      ::pastel::container::newtonian_sph_particles<
        dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator> const& lhs,
      ::pastel::container::newtonian_sph_particles<
        dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator> const& rhs)
    { return !(lhs > rhs); }

    template <
      std::size_t dimension_, typename MobilityTag,
      std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
      typename Value, typename Point, typename Vector,
      typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
    inline bool operator>=(
      ::pastel::container::newtonian_sph_particles<
        dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator> const& lhs,
      ::pastel::container::newtonian_sph_particles<
        dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator> const& rhs)
    { return !(lhs < rhs); }

    template <
      std::size_t dimension_, typename MobilityTag,
      std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
      typename Value, typename Point, typename Vector,
      typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
    inline void swap(
      ::pastel::container::newtonian_sph_particles<
        dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>& lhs,
      ::pastel::container::newtonian_sph_particles<
        dimension_, MobilityTag,
        num_integration_vectors_, num_property_vectors_, num_property_scalars_,
        Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>& rhs)
      noexcept(noexcept(lhs.swap(rhs)))
    { lhs.swap(rhs); }


    namespace dispatch
    {
      template <
        std::size_t dimension_, typename MobilityTag,
        std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
        typename Value, typename Point, typename Vector,
        typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
      struct add_particles<
        ::pastel::container::newtonian_sph_particles<
          dimension_, MobilityTag,
          num_integration_vectors_, num_property_vectors_, num_property_scalars_,
          Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>>
      {
        template <
          typename TargetParticles, typename BoundaryParticles,
          typename Allocator, typename Size>
        static void call(
          TargetParticles& target_particles,
          ::pastel::container::newtonian_sph_particles<
            dimension_, MobilityTag,
            num_integration_vectors_, num_property_vectors_, num_property_scalars_,
            Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator> const& particles,
          BoundaryParticles const& boundary_particles,
          std::vector< ::pastel::system::index_with_origin<typename ::pastel::container::meta::size_of<TargetParticles>::type>, Allocator > const& particle_indices_for_boundary,
          Size first, Size last)
        {
          auto const positions_data
            = particles.template data< ::pastel::particle::tags::position >();
          auto const velocities_data
            = particles.template data< ::pastel::particle::tags::velocity >();
          auto const masses_data
            = particles.template data< ::pastel::particle::tags::mass >();
          auto const densities_data
            = particles.template data< ::pastel::particle::tags::density >();
          auto const sph_divergences_data
            = particles.template data< ::pastel::particle::tags::sph_divergence >();
          auto const pressures_data
            = particles.template data< ::pastel::particle::tags::pressure >();

          for (auto particle_index = first; particle_index < last; ++particle_index)
          {
            auto const index = particle_indices_for_boundary[particle_index].index();
            auto const origin = particle_indices_for_boundary[particle_index].origin();
            if (origin == ::pastel::system::origin::particles)
            {
              target_particles.emplace_back(
                positions_data[index], velocities_data[index], Vector{}, masses_data[index],
                densities_data[index], sph_divergences_data[index], pressures_data[index]);

              ::pastel::utility::for_<std::size_t, 0u, num_property_vectors_, ::pastel::container::detail::copy_nth_property_vector>::call(
                particles, index, target_particles, ::pastel::container::num_particles(target_particles)-1u);
              ::pastel::utility::for_<std::size_t, 0u, num_property_scalars_, ::pastel::container::detail::copy_nth_property_scalar>::call(
                particles, index, target_particles, ::pastel::container::num_particles(target_particles)-1u);
            }
            else // if (origin == ::pastel::system::origin::boundary_particles)
            {
              target_particles.emplace_back(
                ::pastel::container::get< ::pastel::particle::tags::position >(boundary_particles, index),
                ::pastel::container::get< ::pastel::particle::tags::velocity >(boundary_particles, index),
                Vector{},
                ::pastel::container::get< ::pastel::particle::tags::mass >(boundary_particles, index),
                ::pastel::container::get< ::pastel::particle::tags::density >(boundary_particles, index),
                ::pastel::container::get< ::pastel::particle::tags::sph_divergence >(boundary_particles, index),
                ::pastel::container::get< ::pastel::particle::tags::pressure >(boundary_particles, index));

              ::pastel::utility::for_<std::size_t, 0u, num_property_vectors_, ::pastel::container::detail::copy_nth_property_vector>::call(
                boundary_particles, index, target_particles, ::pastel::container::num_particles(target_particles)-1u);
              ::pastel::utility::for_<std::size_t, 0u, num_property_scalars_, ::pastel::container::detail::copy_nth_property_scalar>::call(
                boundary_particles, index, target_particles, ::pastel::container::num_particles(target_particles)-1u);
            }
          }
        }
      }; // struct add_particles< ::pastel::container::newtonian_sph_particles<dimension_, MobilityTag, num_integration_vectors_, num_property_vectors_, num_property_scalars_, Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator> >

      template <
        std::size_t dimension_, typename MobilityTag,
        std::size_t num_integration_vectors_, std::size_t num_property_vectors_, std::size_t num_property_scalars_,
        typename Value, typename Point, typename Vector,
        typename PointAllocator, typename VectorAllocator, typename ScalarAllocator>
      struct copy_particles<
        ::pastel::container::newtonian_sph_particles<
          dimension_, MobilityTag,
          num_integration_vectors_, num_property_vectors_, num_property_scalars_,
          Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>>
      {
        template <
          typename TargetMobilityTag, std::size_t target_num_integration_vectors_,
          typename BoundaryMobilityTag, std::size_t boundary_num_integration_vectors_,
          typename Allocator, typename Size>
        static void call(
          ::pastel::container::newtonian_sph_particles<
            dimension_, TargetMobilityTag,
            target_num_integration_vectors_, num_property_vectors_, num_property_scalars_,
            Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator>& target_particles,
          ::pastel::container::newtonian_sph_particles<
            dimension_, MobilityTag,
            num_integration_vectors_, num_property_vectors_, num_property_scalars_,
            Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator> const& particles,
          ::pastel::container::newtonian_sph_particles<
            dimension_, BoundaryMobilityTag,
            boundary_num_integration_vectors_, num_property_vectors_, num_property_scalars_,
            Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator> const& boundary_particles,
          std::vector< ::pastel::system::index_with_origin<Size>, Allocator > const& particle_indices_for_boundary,
          Size first, Size last)
        {
          auto const positions_data
            = particles.template data< ::pastel::particle::tags::position >();
          auto const velocities_data
            = particles.template data< ::pastel::particle::tags::velocity >();
          auto const masses_data
            = particles.template data< ::pastel::particle::tags::mass >();
          auto const densities_data
            = particles.template data< ::pastel::particle::tags::density >();
          auto const sph_divergences_data
            = particles.template data< ::pastel::particle::tags::sph_divergence >();
          auto const pressures_data
            = particles.template data< ::pastel::particle::tags::pressure >();
          auto const boundary_positions_data
            = boundary_particles.template data< ::pastel::particle::tags::position >();
          auto const boundary_velocities_data
            = boundary_particles.template data< ::pastel::particle::tags::velocity >();
          auto const boundary_masses_data
            = boundary_particles.template data< ::pastel::particle::tags::mass >();
          auto const boundary_densities_data
            = boundary_particles.template data< ::pastel::particle::tags::density >();
          auto const boundary_sph_divergences_data
            = boundary_particles.template data< ::pastel::particle::tags::sph_divergence >();
          auto const boundary_pressures_data
            = boundary_particles.template data< ::pastel::particle::tags::pressure >();
          auto const target_positions_data
            = target_particles.template data< ::pastel::particle::tags::position >();
          auto const target_velocities_data
            = target_particles.template data< ::pastel::particle::tags::velocity >();
          auto const target_masses_data
            = target_particles.template data< ::pastel::particle::tags::mass >();
          auto const target_densities_data
            = target_particles.template data< ::pastel::particle::tags::density >();
          auto const target_sph_divergences_data
            = target_particles.template data< ::pastel::particle::tags::sph_divergence >();
          auto const target_pressures_data
            = target_particles.template data< ::pastel::particle::tags::pressure >();

          for (auto particle_index = first; particle_index < last; ++particle_index)
          {
            auto const index = particle_indices_for_boundary[particle_index].index();
            auto const origin = particle_indices_for_boundary[particle_index].origin();
            if (origin == ::pastel::system::origin::particles)
            {
              target_positions_data[particle_index] = positions_data[index];
              target_velocities_data[particle_index] = velocities_data[index];
              target_masses_data[particle_index] = masses_data[index];
              target_densities_data[particle_index] = densities_data[index];
              target_sph_divergences_data[particle_index] = sph_divergences_data[index];
              target_pressures_data[particle_index] = pressures_data[index];

              ::pastel::utility::for_<std::size_t, 0u, num_property_vectors_, ::pastel::container::detail::copy_nth_property_vector>::call(
                particles, index, target_particles, particle_index);
              ::pastel::utility::for_<std::size_t, 0u, num_property_scalars_, ::pastel::container::detail::copy_nth_property_scalar>::call(
                particles, index, target_particles, particle_index);
            }
            else // if (origin == ::pastel::system::origin::boundary_particles)
            {
              target_positions_data[particle_index] = boundary_positions_data[index];
              target_velocities_data[particle_index] = boundary_velocities_data[index];
              target_masses_data[particle_index] = boundary_masses_data[index];
              target_densities_data[particle_index] = boundary_densities_data[index];
              target_sph_divergences_data[particle_index] = boundary_sph_divergences_data[index];
              target_pressures_data[particle_index] = boundary_pressures_data[index];

              ::pastel::utility::for_<std::size_t, 0u, num_property_vectors_, ::pastel::container::detail::copy_nth_property_vector>::call(
                boundary_particles, index, target_particles, particle_index);
              ::pastel::utility::for_<std::size_t, 0u, num_property_scalars_, ::pastel::container::detail::copy_nth_property_scalar>::call(
                boundary_particles, index, target_particles, particle_index);
            }
          }
        }
      }; // struct copy_particles< ::pastel::container::newtonian_sph_particles<dimension_, MobilityTag, num_integration_vectors_, num_property_vectors_, num_property_scalars_, Value, Point, Vector, PointAllocator, VectorAllocator, ScalarAllocator> >
    } // namespace dispatch
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_NEWTONIAN_SPH_PARTICLES_HPP

