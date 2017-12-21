#ifndef PASTEL_SYSTEM_SYSTEM_HPP
# define PASTEL_SYSTEM_SYSTEM_HPP

# include <tuple>
# include <utility>
# include <type_traits>

# include <pastel/extforce/no_force.hpp>
# include <pastel/container/meta/dimension_of.hpp>
# include <pastel/utility/tuple/meta/is_tuple.hpp>
# include <pastel/utility/tuple/meta/all_of.hpp>
# include <pastel/utility/tuple/meta/fill_n.hpp>


namespace pastel
{
  namespace system
  {
    namespace system_detail
    {
      template <typename Particles, int dimension>
      struct dimension_equal_to
        : std::integral_constant<bool, ::pastel::container::meta::dimension_of<Particles>::value == dimension>
      { };
    } // namespace system_detail


    template <
      typename ParticlesTuple, typename NeighborListTuple,
      typename ExternalForceTuple
        = typename ::pastel::utility::tuple::meta::fill_n<
            std::tuple_size<ParticlesTuple>::value,
            ::pastel::extforce::no_force>::type>
    class system
    {
      ParticlesTuple particles_tuple_;
      NeighborListTuple neighbor_list_tuple_;
      ExternalForceTuple external_force_tuple_;

      static_assert(::pastel::utility::tuple::meta::is_tuple<ParticlesTuple>::value, "ParticlesTuple must be a tuple");

     public:
      static constexpr int dimension
        = std::tuple_size<ParticlesTuple>::value > 0u
          ? ::pastel::container::meta::dimension_of<typename std::tuple_element<0u, ParticlesTuple>::type>::value
          : 0;

     private:
      template <typename Particles>
      using dimension_equal_to = ::pastel::system::system_detail::dimension_equal_to<Particles, dimension>;
      static_assert(
        ::pastel::utility::tuple::meta::all_of<ParticlesTuple, dimension_equal_to>::value,
        "Dimensions in ParticlesTuple must be equal");

      static_assert(::pastel::utility::tuple::meta::is_tuple<NeighborListTuple>::value, "NeighborListTuple must be a tuple");

      static_assert(::pastel::utility::tuple::meta::is_tuple<ExternalForceTuple>::value, "ExternalForceTuple must be a tuple");
      static_assert(
        std::tuple_size<ParticlesTuple>::value == std::tuple_size<ExternalForceTuple>::value,
        "size of ParticlesTuple must be equal to that of ExternalForceTuple");

     public:
      template <std::size_t index>
      using particles_type = typename std::tuple_element<index, ParticlesTuple>::type;
      template <std::size_t index>
      using neighbor_list_type = typename std::tuple_element<index, NeighborListTuple>::type;
      template <std::size_t index>
      using external_force_type = typename std::tuple_element<index, ExternalForceTuple>::type;

      static constexpr std::size_t particles_tuple_size = std::tuple_size<ParticlesTuple>::value;
      static constexpr std::size_t neighbor_list_tuple_size = std::tuple_size<NeighborListTuple>::value;

      system() = default;

      template <typename ParticlesTuple_, typename NeighborListTuple_>
      system(ParticlesTuple_&& particles_tuple, NeighborListTuple_&& neighbor_list_tuple)
        : particles_tuple_{std::forward<ParticlesTuple_>(particles_tuple)},
          neighbor_list_tuple_{std::forward<NeighborListTuple_>(neighbor_list_tuple)},
          external_force_tuple_{}
      { }

      template <typename ParticlesTuple_, typename NeighborListTuple_, typename ExternalForceTuple_>
      system(
        ParticlesTuple_&& particles_tuple,
        NeighborListTuple_&& neighbor_list_tuple,
        ExternalForceTuple_&& external_force_tuple)
        : particles_tuple_{std::forward<ParticlesTuple_>(particles_tuple)},
          neighbor_list_tuple_{std::forward<NeighborListTuple_>(neighbor_list_tuple)},
          external_force_tuple_{std::forward<ExternalForceTuple_>(external_force_tuple)}
      { }

      template <std::size_t index>
      typename std::tuple_element<index, ParticlesTuple>::type const& particles() const
      { return std::get<index>(particles_tuple_); }

      template <std::size_t index>
      typename std::tuple_element<index, ParticlesTuple>::type& particles()
      { return std::get<index>(particles_tuple_); }

      template <std::size_t index, typename Particles>
      void particles(Particles&& new_particles)
      { std::get<index>(particles_tuple_) = std::forward<Particles>(new_particles); }


      template <std::size_t index>
      typename std::tuple_element<index, NeighborListTuple>::type const& neighbor_list() const
      { return std::get<index>(neighbor_list_tuple_); }

      template <std::size_t index>
      typename std::tuple_element<index, NeighborListTuple>::type& neighbor_list()
      { return std::get<index>(neighbor_list_tuple_); }

      template <std::size_t index, typename NeighborList>
      void neighbor_list(NeighborList&& new_neighbor_list)
      { std::get<index>(neighbor_list_tuple_) = std::forward<NeighborList>(new_neighbor_list); }


      template <std::size_t index>
      typename std::tuple_element<index, ExternalForceTuple>::type const& external_force() const
      { return std::get<index>(external_force_tuple_); }

      template <std::size_t index>
      typename std::tuple_element<index, ExternalForceTuple>::type& external_force()
      { return std::get<index>(external_force_tuple_); }

      template <std::size_t index, typename ExternalForce>
      void external_force(ExternalForce&& new_external_force)
      { std::get<index>(external_force_tuple_) = std::forward<ExternalForce>(new_external_force); }
    }; // class system<ParticlesTuple, NeighborListTuple>
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_SYSTEM_HPP

