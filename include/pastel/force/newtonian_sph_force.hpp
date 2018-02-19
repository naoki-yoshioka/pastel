#ifndef PASTEL_FORCE_NEWTONIAN_SPH_FORCE_HPP
# define PASTEL_FORCE_NEWTONIAN_SPH_FORCE_HPP

# include <cassert>
# include <cmath>
# include <utility>

# include <pastel/force/tags.hpp>
# include <pastel/geometry/squared_norm.hpp>
# include <pastel/utility/is_nothrow_swappable.hpp>


namespace pastel
{
  namespace force
  {
    template <typename Kernel, typename Value = double>
    class newtonian_sph_force
    {
      Kernel kernel_;

      Value cutoff_length_;
      Value inverse_cutoff_length_;
      Value squared_cutoff_length_;

      Value shear_viscosity_;
      Value viscous_coefficient_;

     public:
      using value_type = Value;
      using required_arguments_category = ::pastel::force::tags::newtonian_sph_requirement;
      static constexpr bool has_potential_energy = false;
      static constexpr bool has_cutoff = true;

      template <typename Kernel_>
      explicit constexpr newtonian_sph_force(Kernel_&& kernel)
        : kernel_{std::forward<Kernel_>(kernel)},
          cutoff_length_{Value{1}},
          inverse_cutoff_length_{Value{1}},
          squared_cutoff_length_{Value{1}},
          shear_viscosity_{Value{1}},
          viscous_coefficient_{Value{2}}
      { }

      template <typename Kernel_>
      constexpr newtonian_sph_force(Kernel_&& kernel, Value const& cutoff_length, Value const& shear_viscosity)
        : kernel_{std::forward<Kernel_>(kernel)},
          cutoff_length_{cutoff_length},
          inverse_cutoff_length_{Value{1} / cutoff_length},
          squared_cutoff_length_{cutoff_length * cutoff_length},
          shear_viscosity_{shear_viscosity},
          viscous_coefficient_{Value{2} * shear_viscosity}
      { assert(cutoff_length > Value{0} && shear_viscosity > Value{0}); }

      constexpr bool operator==(newtonian_sph_force const& other) const
      {
# ifdef NDEBUG
        return kernel_ == other.kernel_
          && cutoff_length_ == other.cutoff_length_
          && shear_viscosity_ == other.shear_viscosity_;
# else
        return kernel_ == other.kernel_
          && cutoff_length_ == other.cutoff_length_
          && inverse_cutoff_length_ == other.inverse_cutoff_length_
          && squared_cutoff_length_ == other.squared_cutoff_length_
          && shear_viscosity_ == other.shear_viscosity_
          && viscous_coefficient_ == other.viscous_coefficient_;
# endif
      }

      void swap(newtonian_sph_force& other)
        noexcept(::pastel::utility::is_nothrow_swappable<Kernel>::value && ::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(kernel_, other.kernel_);
        swap(cutoff_length_, other.cutoff_length_);
        swap(inverse_cutoff_length_, other.inverse_cutoff_length_);
        swap(squared_cutoff_length_, other.squared_cutoff_length_);
        swap(shear_viscosity_, other.shear_viscosity_);
        swap(viscous_coefficient_, other.viscous_coefficient_);
      }

      constexpr Kernel const& kernel() const { return kernel_; }
      template <typename Kernel_>
      void kernel(Kernel_&& new_kernel) { kernel_ = std::forward<Kernel_>(new_kernel); }

      constexpr Value const& cutoff_length() const { return cutoff_length_; }
      void cutoff_length(Value const& new_cutoff_length)
      {
        assert(new_cutoff_length > Value{0});
        cutoff_length_ = new_cutoff_length;
        inverse_cutoff_length_ = Value{1} / new_cutoff_length;
        squared_cutoff_length_ = new_cutoff_length * new_cutoff_length;
      }

      constexpr Value const& shear_viscosity() const { return shear_viscosity_; }
      void shear_viscosity(Value const& new_shear_viscosity)
      {
        assert(new_shear_viscosity > Value{0});
        shear_viscosity_ = new_shear_viscosity;
        viscous_coefficient_ == Value{2} * new_shear_viscosity;
      }

      // (m_i m_j/|r_{ij}|) W'(|r_{ij}|, h) [-(P_i/rho_i^2 + P_j/rho_j^2) r_{ij} + (2mu / rho_i rho_j) v_{ij}]
      template <typename Point, typename Vector>
      Vector operator()(
        Point const& position1, Vector const& velocity1, Value const& mass1, Value const& density1, Value const& pressure1,
        Point const& position2, Vector const& velocity2, Value const& mass2, Value const& density2, Value const& pressure2) const
      {
        auto const difference = position1 - position2;
        auto const squared_distance = ::pastel::geometry::squared_norm(difference);

        if (squared_distance > squared_cutoff_length_)
          return Vector{};

        using std::sqrt;
        auto const distance = sqrt(squared_distance);

        auto const coefficient = mass1 * mass2 / distance * kernel_.derivative(distance * inverse_cutoff_length_);

        auto const inverse_density1 = Value{1} / density1;
        auto const inverse_density2 = Value{1} / density2;
        return (coefficient * (pressure1 * inverse_density1 * inverse_density1 + pressure2 * inverse_density2 * inverse_density2)) * difference
          + (coefficient * viscous_coefficient_ * inverse_density1 * inverse_density2) * (velocity1 - velocity2);
      }
    }; // class newtonian_sph_force
  } // namespace force
} // namespace pastel


#endif // PASTEL_FORCE_NEWTONIAN_SPH_FORCE_HPP

