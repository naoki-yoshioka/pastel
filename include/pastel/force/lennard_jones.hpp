#ifndef PASTEL_FORCE_LENNARD_JONES_HPP
# define PASTEL_FORCE_LENNARD_JONES_HPP

# include <utility>

# include <pastel/force/tags.hpp>
# include <pastel/geometry/norm.hpp>
# include <pastel/geometry/squared_norm.hpp>
# include <pastel/utility/is_nothrow_swappable.hpp>


namespace pastel
{
  namespace force
  {
    template <bool has_cutoff = true, typename Value = double>
    class lennard_jones;

    template <typename Value>
    class lennard_jones<false, Value>
    {
      Value energy_depth_; // e
      Value diameter_; // s

      Value force_coefficient_; // 24e
      Value energy_coefficient_; // 4e
      Value squared_diameter_; // s^2

     public:
      using value_type = Value;
      using required_arguments_category = ::pastel::force::tags::requires_position;
      static constexpr bool has_potential_energy = true;

      constexpr lennard_jones() noexcept
        : energy_depth_{Value{1}},
          diameter_{Value{1}},
          force_coefficient_{Value{24}},
          energy_coefficient_{Value{4}},
          squared_diameter_{Value{1}}
      { }

      ~lennard_jones() noexcept = default;
      lennard_jones(lennard_jones const&) = default;
      lennard_jones(lennard_jones&&) noexcept = default;
      lennard_jones& operator=(lennard_jones const&) = default;
      lennard_jones& operator=(lennard_jones&&) noexcept = default;

      constexpr lennard_jones(Value const& energy_depth, Value const& diameter) noexcept
        : energy_depth_{energy_depth},
          diameter_{diameter},
          force_coefficient_{Value{24} * energy_depth},
          energy_coefficient_{Value{4} * energy_depth},
          squared_diameter_{diameter * diameter}
      { }

      constexpr bool operator==(lennard_jones const& other) const
      {
# ifdef NDEBUG
        return energy_depth_ == other.energy_depth_ && diameter_ == other.diameter_;
# else
        return energy_depth_ == other.energy_depth_ && diameter_ == other.diameter_
          && force_coefficient_ == other.force_coefficient_
          && energy_coefficient_ == other.energy_coefficient_
          && squared_diameter_ == other.squared_diameter_;
# endif
      }

      void swap(lennard_jones& other)
        noexcept(::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(energy_depth_, other.energy_depth_);
        swap(diameter_, other.diameter_);
        swap(force_coefficient_, other.force_coefficient_);
        swap(energy_coefficient_, other.energy_coefficient_);
        swap(squared_diameter_, other.squared_diameter_);
      }

      constexpr Value const& energy_depth() const { return energy_depth_; }
      void energy_depth(Value const& new_energy_depth)
      {
        energy_depth_ = new_energy_depth;
        force_coefficient_ = Value{24} * new_energy_depth;
        energy_coefficient_ = Value{4} * new_energy_depth;
      }

      constexpr Value const& diameter() const { return diameter_; }
      void diameter(Value const& new_diameter)
      {
        diameter_ = new_diameter;
        squared_diameter_ = new_diameter * new_diameter;
      }

     private:
      template <bool has_cutoff_, typename Value_>
      friend class ::pastel::force::lennard_jones<has_cutoff_, Value_>;

      template <typename Vector>
      auto calculate_force(Vector const& difference, Value squared_distance) const
        -> decltype(force_coefficient_ * difference)
      {
        // 1/|r_{ij}|^2, r_{ij} = r_i - r_j
        auto const inverse_squared_distance = Value{1} / squared_distance;
        // s^2/|r_{ij}|^2
        auto const squared_diameter_distance_ratio
          = squared_diameter_ * inverse_squared_distance;
        // s^6/|r_{ij}|^6
        auto const diameter_distance_ratio_6
          = squared_diameter_distance_ratio * squared_diameter_distance_ratio * squared_diameter_distance_ratio;

        // 4e (12s^{12}/|r_{ij}|^{13} - 6s^6/|r_{ij}|^7) n_{ij} = [24e s^6/|r_{ij}|^6 (2 s^6/|r_{ij}|^6 - 1) / |r_{ij}|^2] r_{ij}
        // n_{ij} = r_{ij} / |r_{ij}|
        return (force_coefficient_ * diameter_distance_ratio_6 * (Value{2} * diameter_distance_ratio_6 - Value{1}) * inverse_squared_distance) * difference;
      }

      template <typename Vector>
      auto calculate_force(Vector const& difference) const
        -> decltype(force_coefficient_ * difference)
      { return this->calculate_force(difference, ::pastel::geometry::squared_norm(difference)); }

     public:
      template <typename Point>
      auto operator()(Point const& position1, Point const& position2) const
        -> decltype(calculate_force(position1 - position2))
      { return this->calculate_force(position2 - position1); }

     private:
      Value calculate_energy(Value squared_distance) const
      {
        // s^2/|r_{ij}|^2, r_{ij} = r_i - r_j
        auto const squared_diameter_distance_ratio = squared_diameter_ / squared_distance;
        // s^6/|r_{ij}|^6
        auto const diameter_distance_ratio_6
          = squared_diameter_distance_ratio * squared_diameter_distance_ratio * squared_diameter_distance_ratio;

        // 4e [(s/|r_{ij}|)^{12} - (s/|r_{ij}|)^6] = 4e (s/|r_{ij}|)^6 [(s/|r_{ij}|)^6 - 1]
        return energy_coefficient_ * diameter_distance_ratio_6 * (diameter_distance_ratio_6 - Value{1});
      }

     public:
      template <typename Point>
      Value energy(Point const& position1, Point const& position2) const
      { return this->energy(::pastel::geometry::squared_norm(position1 - position2)); }
    }; // lennard_jones<false, Value>


    template <typename Value>
    class lennard_jones<true, Value>
    {
      ::pastel::force::lennard_jones<false, Value> bare_force_;
      Value cutoff_length_;

      Value squared_cutoff_length_;

     public:
      using value_type = Value;
      using required_arguments_category = ::pastel::force::tags::requires_position;
      static constexpr bool has_potential_energy = true;

      constexpr lennard_jones() noexcept
        : bare_force_{},
          cutoff_length_{Value{3} * bare_force_.diameter()}
          squared_cutoff_length_{cutoff_length_ * cutoff_length_}
      { }

      ~lennard_jones() noexcept = default;
      lennard_jones(lennard_jones const&) = default;
      lennard_jones(lennard_jones&&) noexcept = default;
      lennard_jones& operator=(lennard_jones const&) = default;
      lennard_jones& operator=(lennard_jones&&) noexcept = default;

      constexpr lennard_jones(Value const& energy_depth, Value const& diameter, Value const& cutoff_length = Value{3} * diameter) noexcept
        : bare_force_{energy_depth, diameter},
          cutoff_length_{cutoff_length},
          squared_cutoff_length_{cutoff_length * cutoff_length}
      { }

      constexpr bool operator==(lennard_jones const& other) const
      {
# ifdef NDEBUG
        return bare_froce_ == other.bare_force_ && cutoff_length_ == other.cutoff_length_;
# else
        return bare_froce_ == other.bare_force_ && cutoff_length_ == other.cutoff_length_
          && squared_cutoff_length_ == other.squared_cutoff_length_;
# endif
      }

      void swap(lennard_jones& other)
        noexcept(
          ::pastel::utility::is_nothrow_swappable< ::pastel::force::lennard_jones<false, Value> >::value
          && ::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(bare_force_, other.bare_force_);
        swap(cutoff_length_, other.cutoff_length_);
        swap(squared_cutoff_length_, other.squared_cutoff_length_);
      }

      constexpr Value const& energy_depth() const { return bare_force_.energy_depth(); }
      void energy_depth(Value const& new_energy_depth) { bare_force_.energy_depth(new_energy_depth); }

      constexpr Value const& diameter() const { return bare_force_.diameter(); }
      void diameter(Value const& new_diameter) { bare_force_.diameter(new_diameter); }

      constexpr Value const& cutoff_length() const { return cutoff_length_; }
      void cutoff_length(Value const& new_cutoff_length) const { cutoff_length_ = new_cutoff_length; }

      template <typename Point>
      auto operator()(Point const& position1, Point const& position2)
        -> decltype(this->calculate_force(position1 - position2))
      {
        auto const difference = position1 - position2;
        auto const squared_distance = ::pastel::geometry::squared_norm(difference);
        if (squared_distance > squared_cutoff_length_)
          return Vector{}
        return bare_force_.calculate_force(difference, squared_distance);
      }

      template <typename Point>
      Value energy(Point const& position1, Point const& position2) const
      {
        auto const squared_distance = ::pastel::geometry::squared_norm(position1 - position2);
        if (squared_distance > squared_cutoff_length_)
          return Vector{}
        return bare_force_.calculate_energy(squared_distance);
      }
    }; // lennard_jones<true, Value>


    template <bool has_cutoff, typename Value>
    inline constexpr bool operator!=(
      ::pastel::force::lennard_jones<has_cutoff, Value> const& lhs,
      ::pastel::force::lennard_jones<has_cutoff, Value> const& rhs)
    { return !(lhs == rhs); }

    template <bool has_natural_length, typename Value>
    inline void swap(
      ::pastel::force::lennard_jones<has_cutoff, Value>& lhs,
      ::pastel::force::lennard_jones<has_cutoff, Value>& rhs)
      noexcept(noexcept(lhs.swap(rhs)))
    { lhs.swap(rhs); }
  } // namespace force
} // namespace pastel


#endif // PASTEL_FORCE_LENNARD_JONES_HPP

