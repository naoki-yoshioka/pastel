#ifndef SPH_EXTFORCE_HPP
# define SPH_EXTFORCE_HPP

# include <utility>

# include <pastel/geometry/get.hpp>
# include <pastel/particle/tags.hpp>
# include <pastel/container/get.hpp>
# include <pastel/container/meta/size_of.hpp>
# include <pastel/container/meta/has_mass.hpp>


namespace sph_extforce_detail
{
  template <bool has_mass>
  struct force
  {
    template <typename Particles, typename Vector>
    static Vector const& call(
      Particles const& particles,
      typename ::pastel::container::meta::size_of<Particles const>::type index,
      Vector const& gravitational_acceleration)
    { return ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index) * gravitational_acceleration; }
  }; // struct force<has_mass>

  template <>
  struct force<false>
  {
    template <typename Particles, typename Vector>
    static Vector const& call(
      Particles const&,
      typename ::pastel::container::meta::size_of<Particles const>::type,
      Vector const& gravitational_acceleration)
    { return gravitational_acceleration; }
  }; // struct force<false>
} // namespace sph_extforce_detail


template <typename Pressure, typename Vector, std::size_t wall_dimension, typename Value = double>
class sph_extforce
{
  Pressure pressure_;
  Vector gravitational_acceleration_;

  Value wall_position_;

 public:
  static constexpr bool is_sph_external_force = true;

  sph_extforce(Pressure const& pressure, Vector const& gravitational_acceleration, Value const& wall_position) noexcept
    : pressure_{pressure},
      gravitational_acceleration_{gravitational_acceleration},
      wall_position_{wall_position}
  { }

  Pressure const& pressure() const { return pressure_; }

  Vector const& gravitational_acceleration() const { return gravitational_acceleration_; }
  template <typename Vector_>
  void gravitational_acceleration(Vector_&& new_gravitational_acceleration)
  { gravitational_acceleration_ = std::forward<Vector_>(new_gravitational_acceleration); }

  template <typename Particles>
  Vector operator()(
    Particles const& particles,
    typename ::pastel::container::meta::size_of<Particles const>::type index) const
  {
    static constexpr bool has_mass
      = ::pastel::container::meta::has_mass<Particles>::value;
    auto const gravitational_force
      = sph_extforce_detail::force<has_mass>::call(
        particles, index, gravitational_acceleration_);

    auto const& position = pastel::geometry::get<wall_dimension>(pastel::container::get<pastel::particle::tags::position>(particles, index));
    auto const distance = position - wall_position_;
    auto wall_force = Vector{};
    pastel::geometry::get<wall_dimension>(wall_force) = distance > Value{0} ? Value{0} : -Value{1000} * distance;
    return gravitational_force + wall_force;
  }
}; // class sph_extforce<Vector>


#endif // SPH_EXTFORCE_HPP

