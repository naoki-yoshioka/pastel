#ifndef PASTEL_FORCE_LINEAR_SPRING_HPP
# define PASTEL_FORCE_LINEAR_SPRING_HPP

# include <utility>

# include <pastel/force/tags.hpp>
# include <pastel/geometry/norm.hpp>
# include <pastel/geometry/squared_norm.hpp>
# include <pastel/utility/is_nothrow_swappable.hpp>


namespace pastel
{
  namespace force
  {
    template <bool has_natural_length = false, typename Value = double>
    class linear_spring;

    template <typename Value>
    class linear_spring<false, Value>
    {
      Value spring_coefficient_;
      Value half_spring_coefficient_;

     public:
      using value_type = Value;
      using required_arguments_category = ::pastel::force::tags::requires_position;
      static constexpr bool has_potential_energy = true;
      static constexpr bool has_cutoff = false;

      constexpr linear_spring() noexcept
        : spring_coefficient_{Value{1}}, half_spring_coefficient_{Value{1} / Value{2}}
      { }

      ~linear_spring() noexcept = default;
      linear_spring(linear_spring const&) = default;
      linear_spring(linear_spring&&) noexcept = default;
      linear_spring& operator=(linear_spring const&) = default;
      linear_spring& operator=(linear_spring&&) noexcept = default;

      explicit constexpr linear_spring(Value const& spring_coefficient) noexcept
        : spring_coefficient_{spring_coefficient},
          half_spring_coefficient_{spring_coefficient / Value{2}}
      { }

      constexpr bool operator==(linear_spring const& other) const
      {
# ifdef NDEBUG
        return spring_coefficient_ == other.spring_coefficient_;
# else
        return spring_coefficient_ == other.spring_coefficient_
          && half_spring_coefficient_ == other.half_spring_coefficient_;
# endif
      }

      void swap(linear_spring& other)
        noexcept(::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(spring_coefficient_, other.spring_coefficient_);
        swap(half_spring_coefficient_, other.half_spring_coefficient_);
      }

      constexpr Value const& spring_coefficient() const { return spring_coefficient_; }
      void spring_coefficient(Value const& new_spring_coefficient)
      {
        spring_coefficient_ = new_spring_coefficient;
        half_spring_coefficient_ = new_spring_coefficient / Value{2};
      }

      template <typename Point>
      auto operator()(Point const& position1, Point const& position2) const
        -> decltype(spring_coefficient_ * (position2 - position1))
      {
        // -k r_{ij} = k (r_j - r_i), r_{ij} = r_i - r_j
        return spring_coefficient_ * (position2 - position1);
      }

      template <typename Point>
      Value energy(Point const& position1, Point const& position2) const
      {
        // k |r_{ij}|^2 / 2
        return half_spring_coefficient_ * ::pastel::geometry::squared_norm(position1 - position2);
      }
    }; // linear_spring<false, Value>


    template <typename Value>
    class linear_spring<true, Value>
    {
      Value spring_coefficient_;
      Value natural_length_;

      Value half_spring_coefficient_;

     public:
      using value_type = Value;
      using required_arguments_category = ::pastel::force::tags::requires_position;
      static constexpr bool has_potential_energy = true;
      static constexpr bool has_cutoff = false;

      constexpr linear_spring() noexcept
        : spring_coefficient_{Value{1}},
          natural_length_{Value{0}},
          half_spring_coefficient_{Value{1} / Value{2}}
      { }

      ~linear_spring() noexcept = default;
      linear_spring(linear_spring const&) = default;
      linear_spring(linear_spring&&) noexcept = default;
      linear_spring& operator=(linear_spring const&) = default;
      linear_spring& operator=(linear_spring&&) noexcept = default;

      constexpr linear_spring(Value const& spring_coefficient, Value const& natural_length) noexcept
        : spring_coefficient_{spring_coefficient},
          natural_length_{natural_length},
          half_spring_coefficient_{spring_coefficient / Value{2}}
      { }

      constexpr bool operator==(linear_spring const& other) const
      {
# ifdef NDEBUG
        return spring_coefficient_ == other.spring_coefficient_
          && natural_length_ == other.natural_length_;
# else
        return spring_coefficient_ == other.spring_coefficient_
          && natural_length_ == other.natural_length_
          && half_spring_coefficient_ == other.half_spring_coefficient_;
# endif
      }

      void swap(linear_spring& other)
        noexcept(::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(spring_coefficient_, other.spring_coefficient_);
        swap(natural_length_, other.natural_length_);
        swap(half_spring_coefficient_, other.half_spring_coefficient_);
      }

      Value const& spring_coefficient() const { return spring_coefficient_; }
      void spring_coefficient(Value const& new_spring_coefficient)
      {
        spring_coefficient_ = new_spring_coefficient;
        half_spring_coefficient_ = new_spring_coefficient / Value{2};
      }

      Value const& natural_length() const { return natural_length_; }
      void natural_length(Value const& new_natural_length) { natural_length_ = new_natural_length; }

     private:
      template <typename Vector>
      auto calculate_force(Vector const& difference) const
        -> decltype(spring_coefficient_ * difference)
      {
        // -k (|r_{ij}| - l) n_{ij} = k (l/|r_{ij}| - 1) r_{ij}, r_{ij} = r_i - r_j
        auto const distance = ::pastel::geometry::norm(difference);
        return (spring_coefficient_ * (natural_length_ / distance - Value{1})) * difference;
      }

     public:
      template <typename Point>
      auto operator()(Point const& position1, Point const& position2) const
        -> decltype(this->calculate_force(position1 - position2))
      { return this->calculate_force(position1 - position2); }

      template <typename Point>
      Value energy(Point const& position1, Point const& position2) const
      {
        // k (|r_{ij}| - l)^2 / 2
        auto const distance = ::pastel::geometry::norm(position1 - position2);
        auto const energy_length = distance - natural_length_;
        auto const squared_energy_length = energy_length * energy_length;
        return half_spring_coefficient_ * squared_energy_length;
      }
    }; // linear_spring<true, Value>


    template <bool has_natural_length, typename Value>
    inline constexpr bool operator!=(
      ::pastel::force::linear_spring<has_natural_length, Value> const& lhs,
      ::pastel::force::linear_spring<has_natural_length, Value> const& rhs)
    { return !(lhs == rhs); }

    template <bool has_natural_length, typename Value>
    inline void swap(
      ::pastel::force::linear_spring<has_natural_length, Value>& lhs,
      ::pastel::force::linear_spring<has_natural_length, Value>& rhs)
      noexcept(noexcept(lhs.swap(rhs)))
    { lhs.swap(rhs); }
  } // namespace force
} // namespace pastel


#endif // PASTEL_FORCE_LINEAR_SPRING_HPP

