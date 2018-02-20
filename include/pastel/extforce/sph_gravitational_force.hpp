#ifndef PASTEL_EXTFORCE_SPH_GRAVITATIONAL_FORCE_HPP
# define PASTEL_EXTFORCE_SPH_GRAVITATIONAL_FORCE_HPP

# include <utility>

# include <pastel/particle/tags.hpp>
# include <pastel/container/get.hpp>
# include <pastel/container/meta/size_of.hpp>
# include <pastel/container/meta/has_mass.hpp>


namespace pastel
{
  namespace extforce
  {
    namespace sph_gravitational_force_detail
    {
      template <bool has_mass>
      struct force
      {
        template <typename Particles>
        static Vector const& call(
          Particles const& particles,
          typename ::pastel::container::meta::size_of<Particles const>::type index,
          Vector const& gravitational_acceleration)
        { return ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index) * gravitational_acceleration; }
      }; // struct force<has_mass>

      template <>
      struct force<false>
      {
        template <typename Particles>
        static Vector const& call(
          Particles const&,
          typename ::pastel::container::meta::size_of<Particles const>::type,
          Vector const& gravitational_acceleration)
        { return gravitational_acceleration; }
      }; // struct force<false>
    } // namespace gravitational_force_detail


    template <typename Pressure, typename Vector>
    class sph_gravitational_force
    {
      Pressure pressure_;
      Vector gravitational_acceleration_;

     public:
      static constexpr bool is_sph_external_force = false;

      gravitational_force(Pressure const& pressure, Vector const& gravitational_acceleration) noexcept
        : pressure_{pressure},
          gravitational_acceleration_{gravitational_acceleration}
      { }

      Pressure const& pressure() const { return pressure_; }

      Vector const& gravitational_acceleration() const { return gravitational_acceleration_; }
      template <typename Vector_>
      void gravitational_acceleration(Vector_&& new_gravitational_acceleration)
      { gravitational_acceleration_ = std::forward<Vector_>(new_gravitational_acceleration); }

      template <typename Particles>
      Vector const& operator()(
        Particles const& particles,
        typename ::pastel::container::meta::size_of<Particles const>::type index) const
      {
        static constexpr bool has_mass
          = ::pastel::container::meta::has_mass<Particles>::value;
        return ::pastel::extforce::gravitational_force_detail::force<has_mass>::call(
          particles, index, gravitational_acceleration_);
      }
    }; // class sph_gravitational_force<Vector>
  } // namespacd extforce
} // namespace pastel


#endif // PASTEL_EXTFORCE_SPH_GRAVITATIONAL_FORCE_HPP

