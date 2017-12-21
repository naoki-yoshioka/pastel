#ifndef PASTEL_SYSTEM_SIMPLE_SYSTEM_HPP
# define PASTEL_SYSTEM_SIMPLE_SYSTEM_HPP

# include <utility>

# include <pastel/system/particles.hpp>
# include <pastel/system/neighbor_list.hpp>
# include <pastel/system/external_force.hpp>
# include <pastel/extforce/no_force.hpp>


namespace pastel
{
  namespace system
  {
    template <typename Particles, typename NeighborList, typename ExternalForce = ::pastel::extforce::no_force>
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
    } // namespace simple_system_detail


    template <typename Particles, typename NeighborList, typename ExternalForce>
    class simple_system
    {
      Particles particles_;
      NeighborList neighbor_list_;
      ExternalForce external_force_;

     public:
      template <std::size_t index>
      using particles_type = typename ::pastel::system::simple_system_detail::particles_type<index, Particles>::type;
      template <std::size_t index>
      using neighbor_list_type = typename ::pastel::system::simple_system_detail::neighbor_list_type<index, NeighborList>::type;
      template <std::size_t index>
      using external_force_type = typename ::pastel::system::simple_system_detail::external_force_type<index, ExternalForce>::type;
      static constexpr std::size_t particles_tuple_size = 1u;
      static constexpr std::size_t neighbor_list_tuple_size = 1u;

      simple_system() = default;

      template <typename Particles_, typename NeighborList_>
      simple_system(Particles_&& particles, NeighborList_&& neighbor_list)
        : particles_{std::forward<Particles_>(particles)},
          neighbor_list_{std::forward<NeighborList_>(neighbor_list)},
          external_force_{}
      { }

      template <typename Particles_, typename NeighborList_, typename ExternalForce_>
      simple_system(Particles_&& particles, NeighborList_&& neighbor_list, ExternalForce_&& external_force)
        : particles_{std::forward<Particles_>(particles)},
          neighbor_list_{std::forward<NeighborList_>(neighbor_list)},
          external_force_{std::forward<ExternalForce_>(external_force)}
      { }

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
    }; // class simple_system<Particles, NeighborList, ExternalForce>


    namespace dispatch
    {
      template <typename Particles, typename NeighborList, typename ExternalForce>
      struct call_particles<0u, ::pastel::system::simple_system<Particles, NeighborList, ExternalForce>>
      {
        static Particles const& call(
          ::pastel::system::simple_system<Particles, NeighborList, ExternalForce> const& system)
        { return system.particles(); }

        static Particles& call(
          ::pastel::system::simple_system<Particles, NeighborList, ExternalForce>& system)
        { return system.particles(); }

        template <typename Particles_>
        static void call(
          ::pastel::system::simple_system<Particles, NeighborList, ExternalForce>& system,
          Particles_&& new_particles)
        { return system.particles(std::forward<Particles_>(new_particles)); }
      }; // struct call_particles<0u, Particles, NeighborList, ExternalForce>


      template <typename Particles, typename NeighborList, typename ExternalForce>
      struct call_neighbor_list<0u, ::pastel::system::simple_system<Particles, NeighborList, ExternalForce>>
      {
        static NeighborList const& call(
          ::pastel::system::simple_system<Particles, NeighborList, ExternalForce> const& system)
        { return system.neighbor_list(); }

        static NeighborList& call(
          ::pastel::system::simple_system<Particles, NeighborList, ExternalForce>& system)
        { return system.neighbor_list(); }

        template <typename NeighborList_>
        static void call(
          ::pastel::system::simple_system<Particles, NeighborList, ExternalForce>& system,
          NeighborList_&& new_neighbor_list)
        { return system.neighbor_list(std::forward<NeighborList_>(new_neighbor_list)); }
      }; // struct call_neighbor_list<0u, Particles, NeighborList, ExternalForce>


      template <typename Particles, typename NeighborList, typename ExternalForce>
      struct call_external_force<0u, ::pastel::system::simple_system<Particles, NeighborList, ExternalForce>>
      {
        static ExternalForce const& call(
          ::pastel::system::simple_system<Particles, NeighborList, ExternalForce> const& system)
        { return system.external_force(); }

        static ExternalForce& call(
          ::pastel::system::simple_system<Particles, NeighborList, ExternalForce>& system)
        { return system.external_force(); }

        template <typename ExternalForce_>
        static void call(
          ::pastel::system::simple_system<Particles, NeighborList, ExternalForce>& system,
          ExternalForce_&& new_external_force)
        { return system.external_force(std::forward<ExternalForce_>(new_external_force)); }
      }; // struct call_external_force<0u, Particles, NeighborList, ExternalForce>
    } // namespace dispatch
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_SIMPLE_SYSTEM_HPP

