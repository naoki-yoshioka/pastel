#ifndef PASTEL_SYSTEM_SIMPLE_SYSTEM_HPP
# define PASTEL_SYSTEM_SIMPLE_SYSTEM_HPP

# include <vector>
# include <tuple>
# include <utility>

# include <pastel/system/particles.hpp>
# include <pastel/system/neighbor_list.hpp>
# include <pastel/system/external_force.hpp>
# include <pastel/system/boundary_particles.hpp>
# include <pastel/system/boundary_neighbor_list.hpp>
# include <pastel/system/particle_indices_for_boundary.hpp>
# include <pastel/system/boundary_particle_indices.hpp>
# include <pastel/system/initialize.hpp>
# include <pastel/system/update_forces.hpp>
# include <pastel/system/no_boundary.hpp>
# include <pastel/system/index_with_origin.hpp>
# include <pastel/system/meta/boundary_particles_of.hpp>
# include <pastel/system/meta/boundary_neighbor_list_of.hpp>
# include <pastel/system/meta/particle_indices_for_boundary_of.hpp>
# include <pastel/system/meta/boundary_particle_indices_of.hpp>
# include <pastel/container/meta/dimension_of.hpp>
# include <pastel/container/meta/boundary_container_of.hpp>
# include <pastel/neighbor/meta/interaction_pair_of.hpp>
# include <pastel/neighbor/meta/boundary_neighbor_list_of.hpp>
# include <pastel/extforce/no_force.hpp>
# include <pastel/utility/tuple/meta/fill_n.hpp>


namespace pastel
{
  namespace system
  {
    template <
      typename Particles, typename NeighborList,
      typename Boundary
        = typename ::pastel::utility::tuple::meta::fill_n<
            ::pastel::container::meta::dimension_of<Particles>::value,
            ::pastel::system::no_boundary>::type,
      typename ExternalForce = ::pastel::extforce::no_force>
    class simple_system;

    namespace simple_system_detail
    {
      template <std::size_t index, typename Particles>
      struct particles_type;

      template <typename Particles>
      struct particles_type<0u, Particles>
      { using type = Particles; };

      template <std::size_t index, typename NeighborList>
      struct neighbor_list_type;

      template <typename NeighborList>
      struct neighbor_list_type<0u, NeighborList>
      { using type = NeighborList; };

      template <std::size_t index, typename ExternalForce>
      struct external_force_type;

      template <typename ExternalForce>
      struct external_force_type<0u, ExternalForce>
      { using type = ExternalForce; };

      template <std::size_t index, typename BoundaryParticles>
      struct boundary_particles_type;

      template <typename BoundaryParticles>
      struct boundary_particles_type<0u, BoundaryParticles>
      { using type = BoundaryParticles; };

      template <std::size_t index, typename BoundaryNeighborList>
      struct boundary_neighbor_list_type;

      template <typename BoundaryNeighborList>
      struct boundary_neighbor_list_type<0u, BoundaryNeighborList>
      { using type = BoundaryNeighborList; };

      template <std::size_t index, typename ParticleIndicesForBoundary>
      struct particle_indices_for_boundary_type;

      template <typename ParticleIndicesForBoundary>
      struct particle_indices_for_boundary_type<0u, ParticleIndicesForBoundary>
      { using type = ParticleIndicesForBoundary; };
    } // namespace simple_system_detail


    template <typename Particles, typename NeighborList, typename Boundary, typename ExternalForce>
    class simple_system
    {
      using interaction_pair_type = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
      static_assert(
        interaction_pair_type::first == 0u && interaction_pair_type::second == 0u,
        "Interaction pair of NeighborList must be <0, 0>");
      static_assert(
        ::pastel::container::meta::dimension_of<Particles>::value == std::tuple_size<Boundary>::value,
        "Size of Boundary must be equal to dimension of system");

      Particles particles_;
      NeighborList neighbor_list_;
      ExternalForce external_force_;

      Boundary boundary_;

      using boundary_particles_type_
        = typename ::pastel::container::meta::boundary_container_of<Particles>::type;
      boundary_particles_type_ boundary_particles_;

      using boundary_neighbor_list_type_
        = typename ::pastel::neighbor::meta::boundary_neighbor_list_of<NeighborList, 0u>::type;
      boundary_neighbor_list_type_ boundary_neighbor_list_;

      using size_type = typename ::pastel::container::meta::size_of<Particles>::type;
      using particle_indices_for_boundary_type_
        = std::vector< ::pastel::system::index_with_origin<size_type> >;
      particle_indices_for_boundary_type_ particle_indices_for_boundary_;

      using boundary_particle_indices_type_ = std::tuple<size_type, size_type, size_type>;
      boundary_particle_indices_type_ boundary_particle_indices_array_[ ::pastel::container::meta::dimension_of<Particles>::value ];

     public:
      static constexpr std::size_t dimension = ::pastel::container::meta::dimension_of<Particles>::value;

      template <std::size_t index>
      using particles_type = typename ::pastel::system::simple_system_detail::particles_type<index, Particles>::type;
      template <std::size_t index>
      using neighbor_list_type = typename ::pastel::system::simple_system_detail::neighbor_list_type<index, NeighborList>::type;
      template <std::size_t index>
      using external_force_type = typename ::pastel::system::simple_system_detail::external_force_type<index, ExternalForce>::type;
      static constexpr std::size_t particles_tuple_size = 1u;
      static constexpr std::size_t neighbor_list_tuple_size = 1u;

      using boundary_type = Boundary;
      template <std::size_t index>
      using boundary_particles_type = typename ::pastel::system::simple_system_detail::boundary_particles_type<index, boundary_particles_type_>::type;
      template <std::size_t index>
      using boundary_neighbor_list_type = typename ::pastel::system::simple_system_detail::boundary_neighbor_list_type<index, boundary_neighbor_list_type_>::type;
      template <std::size_t index>
      using particle_indices_for_boundary_type = typename ::pastel::system::simple_system_detail::particle_indices_for_boundary_type<index, particle_indices_for_boundary_type_>::type;
      using boundary_particle_indices_type = boundary_particle_indices_type_;
      static constexpr std::size_t boundary_particles_tuple_size = 1u;
      static constexpr std::size_t boundary_neighbor_list_tuple_size = 1u;

      simple_system() = delete;

      template <typename Time>
      explicit simple_system(Time time_step)
        : simple_system{Particles{}, NeighborList{}, Boundary{}, ExternalForce{}, time_step}
      { }

      template <typename Particles_, typename NeighborList_, typename Time>
      simple_system(Particles_&& particles, NeighborList_&& neighbor_list, Time time_step)
        : simple_system{
            std::forward<Particles_>(particles), std::forward<NeighborList_>(neighbor_list),
            Boundary{}, ExternalForce{}, time_step}
      { }

      template <typename Particles_, typename NeighborList_, typename Time>
      simple_system(Particles_&& particles, NeighborList_&& neighbor_list, Boundary const& boundary, Time time_step)
        : simple_system{
            std::forward<Particles_>(particles), std::forward<NeighborList_>(neighbor_list),
            boundary, ExternalForce{}, time_step}
      { }

      template <typename Particles_, typename NeighborList_, typename Time>
      simple_system(Particles_&& particles, NeighborList_&& neighbor_list, Boundary&& boundary, Time time_step)
        : simple_system{
            std::forward<Particles_>(particles), std::forward<NeighborList_>(neighbor_list),
            std::move(boundary), ExternalForce{}, time_step}
      { }

      template <typename Particles_, typename NeighborList_, typename Time>
      simple_system(Particles_&& particles, NeighborList_&& neighbor_list, ExternalForce const& external_force, Time time_step)
        : simple_system{
            std::forward<Particles_>(particles), std::forward<NeighborList_>(neighbor_list),
            Boundary{}, external_force, time_step}
      { }

      template <typename Particles_, typename NeighborList_, typename Time>
      simple_system(Particles_&& particles, NeighborList_&& neighbor_list, ExternalForce&& external_force, Time time_step)
        : simple_system{
            std::forward<Particles_>(particles), std::forward<NeighborList_>(neighbor_list),
            Boundary{}, std::move(external_force), time_step}
      { }

      template <
        typename Particles_, typename NeighborList_,
        typename Boundary_, typename ExternalForce_, typename Time>
      simple_system(
        Particles_&& particles, NeighborList_&& neighbor_list,
        Boundary_&& boundary, ExternalForce_&& external_force, Time time_step)
        : particles_{std::forward<Particles_>(particles)},
          neighbor_list_{std::forward<NeighborList_>(neighbor_list)},
          external_force_{std::forward<ExternalForce_>(external_force)},
          boundary_{std::forward<Boundary_>(boundary)},
          boundary_particles_{},
          boundary_neighbor_list_{neighbor_list_.force(), neighbor_list_.updater(), neighbor_list_.get_index_allocator()},
          particle_indices_for_boundary_{},
          boundary_particle_indices_array_{}
      {
        ::pastel::system::initialize(*this, time_step);
        ::pastel::system::update_forces(*this);
      }


      Particles const& particles() const { return particles_; }
      Particles& particles() { return particles_; }
      template <typename Particles_>
      void particles(Particles_&& new_particles) { particles_ = std::forward<Particles_>(new_particles); }

      NeighborList const& neighbor_list() const { return neighbor_list_; }
      NeighborList& neighbor_list() { return neighbor_list_; }
      template <typename NeighborList_>
      void neighbor_list(NeighborList_&& new_neighbor_list) { neighbor_list_ = std::forward<NeighborList_>(new_neighbor_list); }

      ExternalForce const& external_force() const { return external_force_; }
      ExternalForce& external_force() { return external_force_; }
      template <typename ExternalForce_>
      void external_force(ExternalForce_&& new_external_force) { external_force_ = std::forward<ExternalForce_>(new_external_force); }

      Boundary const& boundary() const { return boundary_; }
      Boundary& boundary() { return boundary_; }

      boundary_particles_type_ const& boundary_particles() const { return boundary_particles_; }
      boundary_particles_type_& boundary_particles() { return boundary_particles_; }

      boundary_neighbor_list_type_ const& boundary_neighbor_list() const { return boundary_neighbor_list_; }
      boundary_neighbor_list_type_& boundary_neighbor_list() { return boundary_neighbor_list_; }

      particle_indices_for_boundary_type_ const& particle_indices_for_boundary() const { return particle_indices_for_boundary_; }
      particle_indices_for_boundary_type_& particle_indices_for_boundary() { return particle_indices_for_boundary_; }

      template <std::size_t dimension>
      boundary_particle_indices_type_ const& boundary_particle_indices() const { return boundary_particle_indices_array_[dimension]; }
      template <std::size_t dimension>
      boundary_particle_indices_type_& boundary_particle_indices() { return boundary_particle_indices_array_[dimension]; }
    }; // class simple_system<Particles, NeighborList, Boundary, ExternalForce>


    namespace dispatch
    {
      template <typename Particles, typename NeighborList, typename Boundary, typename ExternalForce>
      struct call_particles<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>
      {
        static Particles const& call(
          ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce> const& system)
        { return system.particles(); }

        static Particles& call(
          ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>& system)
        { return system.particles(); }

        template <typename Particles_>
        static void call(
          ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>& system,
          Particles_&& new_particles)
        { return system.particles(std::forward<Particles_>(new_particles)); }
      }; // struct call_particles<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>


      template <typename Particles, typename NeighborList, typename Boundary, typename ExternalForce>
      struct call_neighbor_list<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>
      {
        static NeighborList const& call(
          ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce> const& system)
        { return system.neighbor_list(); }

        static NeighborList& call(
          ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>& system)
        { return system.neighbor_list(); }

        template <typename NeighborList_>
        static void call(
          ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>& system,
          NeighborList_&& new_neighbor_list)
        { return system.neighbor_list(std::forward<NeighborList_>(new_neighbor_list)); }
      }; // struct call_neighbor_list<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>


      template <typename Particles, typename NeighborList, typename Boundary, typename ExternalForce>
      struct call_external_force<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>
      {
        static ExternalForce const& call(
          ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce> const& system)
        { return system.external_force(); }

        static ExternalForce& call(
          ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>& system)
        { return system.external_force(); }

        template <typename ExternalForce_>
        static void call(
          ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>& system,
          ExternalForce_&& new_external_force)
        { return system.external_force(std::forward<ExternalForce_>(new_external_force)); }
      }; // struct call_external_force<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>


      template <typename Particles, typename NeighborList, typename Boundary, typename ExternalForce>
      struct call_boundary_particles<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>
      {
        static typename ::pastel::system::meta::boundary_particles_of<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>::type const& call(
          ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce> const& system)
        { return system.boundary_particles(); }

        static typename ::pastel::system::meta::boundary_particles_of<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>::type& call(
          ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>& system)
        { return system.boundary_particles(); }
      }; // struct call_boundary_particles<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>


      template <typename Particles, typename NeighborList, typename Boundary, typename ExternalForce>
      struct call_boundary_neighbor_list<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>
      {
        static typename ::pastel::system::meta::boundary_neighbor_list_of<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>::type const& call(
          ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce> const& system)
        { return system.boundary_neighbor_list(); }

        static typename ::pastel::system::meta::boundary_neighbor_list_of<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>::type& call(
          ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>& system)
        { return system.boundary_neighbor_list(); }
      }; // struct call_boundary_neighbor_list<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>


      template <typename Particles, typename NeighborList, typename Boundary, typename ExternalForce>
      struct call_particle_indices_for_boundary<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>
      {
        static typename ::pastel::system::meta::particle_indices_for_boundary_of<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>::type const& call(
          ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce> const& system)
        { return system.particle_indices_for_boundary(); }

        static typename ::pastel::system::meta::particle_indices_for_boundary_of<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>::type& call(
          ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>& system)
        { return system.particle_indices_for_boundary(); }
      }; // struct call_particle_indices_for_boundary<0u, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>


      template <std::size_t dimension, typename Particles, typename NeighborList, typename Boundary, typename ExternalForce>
      struct call_boundary_particle_indices<0u, dimension, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>
      {
        static typename ::pastel::system::meta::boundary_particle_indices_of< ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce> >::type const& call(
          ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce> const& system)
        { return system.template boundary_particle_indices<dimension>(); }

        static typename ::pastel::system::meta::boundary_particle_indices_of< ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce> >::type& call(
          ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>& system)
        { return system.template boundary_particle_indices<dimension>(); }
      }; // struct call_boundary_particle_indices<0u, dimension, ::pastel::system::simple_system<Particles, NeighborList, Boundary, ExternalForce>>
    } // namespace dispatch
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_SIMPLE_SYSTEM_HPP

