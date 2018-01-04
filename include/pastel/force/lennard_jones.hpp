#ifndef PASTEL_FORCE_LENNARD_JONES_HPP
# define PASTEL_FORCE_LENNARD_JONES_HPP

# include <cassert>
# include <utility>

# include <pastel/force/tags.hpp>
# include <pastel/geometry/squared_norm.hpp>
# include <pastel/utility/is_nothrow_swappable.hpp>


namespace pastel
{
  namespace force
  {
    template <bool has_cutoff = true, bool has_diameter = true, typename Value = double>
    class lennard_jones;

    template <typename Value>
    class lennard_jones<false, true, Value>
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
      static constexpr bool has_cutoff = false;

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
      { assert(energy_depth > Value{0} && diameter > Value{0}); }

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
        assert(new_energy_depth > Value{0});
        energy_depth_ = new_energy_depth;
        force_coefficient_ = Value{24} * new_energy_depth;
        energy_coefficient_ = Value{4} * new_energy_depth;
      }

      constexpr Value const& diameter() const { return diameter_; }
      void diameter(Value const& new_diameter)
      {
        assert(new_diameter > Value{0});
        diameter_ = new_diameter;
        squared_diameter_ = new_diameter * new_diameter;
      }

     private:
      template <typename Value_>
      friend class ::pastel::force::lennard_jones<true, true, Value_>;

      template <typename Vector>
      auto calculate_force(Vector const& difference, Value squared_distance) const
        -> decltype(force_coefficient_ * difference / squared_distance)
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
        -> decltype(this->calculate_force(difference, ::pastel::geometry::squared_norm(difference)))
      { return this->calculate_force(difference, ::pastel::geometry::squared_norm(difference)); }

     public:
      template <typename Point>
      auto operator()(Point const& position1, Point const& position2) const
        -> decltype(this->calculate_force(position1 - position2))
      { return this->calculate_force(position1 - position2); }

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
    }; // lennard_jones<false, true, Value>


    template <typename Value>
    class lennard_jones<true, true, Value>
    {
      using bare_force_type = ::pastel::force::lennard_jones<false, true, Value>;
      bare_force_type bare_force_;
      Value cutoff_length_;

      Value squared_cutoff_length_;
      Value energy_shift_;

     public:
      using value_type = Value;
      using required_arguments_category = ::pastel::force::tags::requires_position;
      static constexpr bool has_potential_energy = true;
      static constexpr bool has_cutoff = true;

      constexpr lennard_jones() noexcept
        : bare_force_{},
          cutoff_length_{Value{3} * bare_force_.diameter()}
          squared_cutoff_length_{cutoff_length_ * cutoff_length_},
          energy_shift_{-bare_force_.calculate_energy(squared_cutoff_length_)}
      { }

      ~lennard_jones() noexcept = default;
      lennard_jones(lennard_jones const&) = default;
      lennard_jones(lennard_jones&&) noexcept = default;
      lennard_jones& operator=(lennard_jones const&) = default;
      lennard_jones& operator=(lennard_jones&&) noexcept = default;

      constexpr lennard_jones(Value const& energy_depth, Value const& diameter, Value const& cutoff_length = Value{3} * diameter) noexcept
        : bare_force_{energy_depth, diameter},
          cutoff_length_{cutoff_length},
          squared_cutoff_length_{cutoff_length * cutoff_length},
          energy_shift_{-bare_force_.calculate_energy(squared_cutoff_length_)}
      { assert(energy_depth > Value{0} && diameter > Value{0} && cutoff_length > diameter); }

      constexpr bool operator==(lennard_jones const& other) const
      {
# ifdef NDEBUG
        return bare_froce_ == other.bare_force_ && cutoff_length_ == other.cutoff_length_;
# else
        return bare_froce_ == other.bare_force_ && cutoff_length_ == other.cutoff_length_
          && squared_cutoff_length_ == other.squared_cutoff_length_
          && energy_shift_ == other.energy_shift_;
# endif
      }

      void swap(lennard_jones& other)
        noexcept(
          ::pastel::utility::is_nothrow_swappable<bare_force_type>::value
          && ::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(bare_force_, other.bare_force_);
        swap(cutoff_length_, other.cutoff_length_);
        swap(squared_cutoff_length_, other.squared_cutoff_length_);
        swap(energy_shift_, other.energy_shift_);
      }

      constexpr Value const& energy_depth() const { return bare_force_.energy_depth(); }
      void energy_depth(Value const& new_energy_depth)
      {
        assert(new_energy_depth > Value{0});
        bare_force_.energy_depth(new_energy_depth);
        energy_shift_ = -bare_force_.calculate_energy(squared_cutoff_length_);
      }

      constexpr Value const& diameter() const { return bare_force_.diameter(); }
      void diameter(Value const& new_diameter)
      {
        assert(new_diameter > Value{0} && cutoff_length_ > new_diameter);
        bare_force_.diameter(new_diameter);
        energy_shift_ = -bare_force_.calculate_energy(squared_cutoff_length_);
      }

      constexpr Value const& cutoff_length() const { return cutoff_length_; }
      void cutoff_length(Value const& new_cutoff_length)
      {
        assert(new_cutoff_length > Value{0} && new_cutoff_length > bare_force_.diameter());
        cutoff_length_ = new_cutoff_length;
        squared_cutoff_length_ = cutoff_length_ * cutoff_length_;
        energy_shift_ = -bare_force_.calculate_energy(squared_cutoff_length_);
      }

      void change_length(Value const& new_diameter, Value const& new_cutoff_length)
      {
        assert(new_diameter > Value{0} && new_cutoff_length > Value{0} && new_cutoff_length > new_diameter);
        bare_force_.diameter(new_diameter);
        cutoff_length_ = new_cutoff_length;
        squared_cutoff_length_ = cutoff_length_ * cutoff_length_;
        energy_shift_ = -bare_force_.calculate_energy(squared_cutoff_length_);
      }

      template <typename Point>
      auto operator()(Point const& position1, Point const& position2)
        -> decltype(brute_force_.calculate_force(position1 - position2, ::pastel::geometry::squared_norm(position1 - position2)))
      {
        auto const difference = position1 - position2;
        auto const squared_distance = ::pastel::geometry::squared_norm(difference);
        if (squared_distance > squared_cutoff_length_)
          return decltype(bare_force_.calculate_force(difference, squared_distance)){}
        return bare_force_.calculate_force(difference, squared_distance);
      }

      template <typename Point>
      Value energy(Point const& position1, Point const& position2) const
      {
        auto const squared_distance = ::pastel::geometry::squared_norm(position1 - position2);
        if (squared_distance > squared_cutoff_length_)
          return Value{};
        return bare_force_.calculate_energy(squared_distance) + energy_shift_;
      }
    }; // lennard_jones<true, true, Value>


    template <typename Value>
    class lennard_jones<false, false, Value>
    {
      Value energy_depth_; // e

      Value force_coefficient_; // 24e
      Value energy_coefficient_; // 4e

     public:
      using value_type = Value;
      using required_arguments_category = ::pastel::force::tags::requires_position;
      static constexpr bool has_potential_energy = true;
      static constexpr bool has_cutoff = false;

      constexpr lennard_jones() noexcept
        : energy_depth_{Value{1}},
          force_coefficient_{Value{24}},
          energy_coefficient_{Value{4}}
      { }

      ~lennard_jones() noexcept = default;
      lennard_jones(lennard_jones const&) = default;
      lennard_jones(lennard_jones&&) noexcept = default;
      lennard_jones& operator=(lennard_jones const&) = default;
      lennard_jones& operator=(lennard_jones&&) noexcept = default;

      explicit constexpr lennard_jones(Value const& energy_depth) noexcept
        : energy_depth_{energy_depth},
          force_coefficient_{Value{24} * energy_depth},
          energy_coefficient_{Value{4} * energy_depth}
      { assert(energy_depth > Value{0}); }

      constexpr bool operator==(lennard_jones const& other) const
      {
# ifdef NDEBUG
        return energy_depth_ == other.energy_depth_;
# else
        return energy_depth_ == other.energy_depth_
          && force_coefficient_ == other.force_coefficient_
          && energy_coefficient_ == other.energy_coefficient_;
# endif
      }

      void swap(lennard_jones& other)
        noexcept(::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(energy_depth_, other.energy_depth_);
        swap(force_coefficient_, other.force_coefficient_);
        swap(energy_coefficient_, other.energy_coefficient_);
      }

      constexpr Value const& energy_depth() const { return energy_depth_; }
      void energy_depth(Value const& new_energy_depth)
      {
        assert(new_energy_depth > Value{0});
        energy_depth_ = new_energy_depth;
        force_coefficient_ = Value{24} * new_energy_depth;
        energy_coefficient_ = Value{4} * new_energy_depth;
      }

     private:
      template <typename Value_>
      friend class ::pastel::force::lennard_jones<true, false, Value_>;

      template <typename Vector>
      auto calculate_force(Vector const& difference, Value squared_distance, Value squared_diameter) const
        -> decltype(force_coefficient_ * difference / squared_distance)
      {
        // 1/|r_{ij}|^2, r_{ij} = r_i - r_j
        auto const inverse_squared_distance = Value{1} / squared_distance;
        // s^2/|r_{ij}|^2
        auto const squared_diameter_distance_ratio
          = squared_diameter * inverse_squared_distance;
        // s^6/|r_{ij}|^6
        auto const diameter_distance_ratio_6
          = squared_diameter_distance_ratio * squared_diameter_distance_ratio * squared_diameter_distance_ratio;

        // 4e (12s^{12}/|r_{ij}|^{13} - 6s^6/|r_{ij}|^7) n_{ij} = [24e s^6/|r_{ij}|^6 (2 s^6/|r_{ij}|^6 - 1) / |r_{ij}|^2] r_{ij}
        // n_{ij} = r_{ij} / |r_{ij}|
        return (force_coefficient_ * diameter_distance_ratio_6 * (Value{2} * diameter_distance_ratio_6 - Value{1}) * inverse_squared_distance) * difference;
      }

      template <typename Vector>
      auto calculate_force(Vector const& difference, Value diameter) const
        -> decltype(this->calculate_force(difference, ::pastel::geometry::squared_norm(difference), diameter * diameter))
      { return this->calculate_force(difference, ::pastel::geometry::squared_norm(difference), diameter * diameter); }

     public:
      template <typename Point>
      auto operator()(Point const& position1, Value const& diameter1, Point const& position2, Value const& diameter2) const
        -> decltype(calculate_force(position1 - position2), (diameter1 + diameter2) / Value{2})
      {
        assert(diameter1 > Value{0} && diameter2 > Value{0});
        return this->calculate_force(position1 - position2, (diameter1 + diameter2) / Value{2});
      }

     private:
      Value calculate_energy(Value squared_distance, Value diameter) const
      {
        // s^2/|r_{ij}|^2, r_{ij} = r_i - r_j
        auto const squared_diameter_distance_ratio = diameter * diameter / squared_distance;
        // s^6/|r_{ij}|^6
        auto const diameter_distance_ratio_6
          = squared_diameter_distance_ratio * squared_diameter_distance_ratio * squared_diameter_distance_ratio;

        // 4e [(s/|r_{ij}|)^{12} - (s/|r_{ij}|)^6] = 4e (s/|r_{ij}|)^6 [(s/|r_{ij}|)^6 - 1]
        return energy_coefficient_ * diameter_distance_ratio_6 * (diameter_distance_ratio_6 - Value{1});
      }

     public:
      template <typename Point>
      Value energy(Point const& position1, Value const& diameter1, Point const& position2, Value const& diameter2) const
      {
        assert(diameter1 > Value{0} && diameter2 > Value{0});
        return this->energy(::pastel::geometry::squared_norm(position1 - position2), (diameter1 + diameter2) / Value{2});
      }
    }; // lennard_jones<false, false, Value>


    template <typename Value>
    class lennard_jones<true, false, Value>
    {
      using bare_force_type = ::pastel::force::lennard_jones<false, false, Value>;
      bare_force_type bare_force_;
      Value cutoff_length_;

      Value squared_cutoff_length_;

     public:
      using value_type = Value;
      using required_arguments_category = ::pastel::force::tags::requires_position;
      static constexpr bool has_potential_energy = true;
      static constexpr bool has_cutoff = true;

      constexpr lennard_jones() noexcept
        : bare_force_{},
          cutoff_length_{Value{3}}
          squared_cutoff_length_{cutoff_length_ * cutoff_length_}
      { }

      ~lennard_jones() noexcept = default;
      lennard_jones(lennard_jones const&) = default;
      lennard_jones(lennard_jones&&) noexcept = default;
      lennard_jones& operator=(lennard_jones const&) = default;
      lennard_jones& operator=(lennard_jones&&) noexcept = default;

      explicit constexpr lennard_jones(Value const& energy_depth) noexcept
        : lennard_jones{energy_depth, Value{3}}
      { }

      constexpr lennard_jones(Value const& energy_depth, Value const& cutoff_length) noexcept
        : bare_force_{energy_depth},
          cutoff_length_{cutoff_length},
          squared_cutoff_length_{cutoff_length * cutoff_length}
      { assert(energy_depth > Value{0} && cutoff_length > Value{0}); }

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
          ::pastel::utility::is_nothrow_swappable<bare_force_type>::value
          && ::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(bare_force_, other.bare_force_);
        swap(cutoff_length_, other.cutoff_length_);
        swap(squared_cutoff_length_, other.squared_cutoff_length_);
      }

      constexpr Value const& energy_depth() const { return bare_force_.energy_depth(); }
      void energy_depth(Value const& new_energy_depth)
      {
        assert(new_energy_depth > Value{0});
        bare_force_.energy_depth(new_energy_depth);
      }

      constexpr Value const& cutoff_length() const { return cutoff_length_; }
      void cutoff_length(Value const& new_cutoff_length)
      {
        assert(new_cutoff_length > Value{0});
        cutoff_length_ = new_cutoff_length;
        squared_cutoff_length_ = cutoff_length_ * cutoff_length_;
      }

      template <typename Point>
      auto operator()(Point const& position1, Value const& diameter1, Point const& position2, Value const& diameter2)
        -> decltype(brute_force_.calculate_force(position1 - position2, (diameter1 + diameter2) / Value{2}))
      {
        auto const difference = position1 - position2;
        auto const squared_distance = ::pastel::geometry::squared_norm(difference);
        if (squared_distance > squared_cutoff_length_)
          return decltype(bare_force_.calculate_force(difference, squared_distance, diameter1 * diameter2)){}

        auto const diameter = (diameter1 + diameter2) / Value{2};
        return bare_force_.calculate_force(difference, squared_distance, diameter * diameter);
      }

      template <typename Point>
      Value energy(Point const& position1, Value const& diameter1, Point const& position2, Value const& diameter2) const
      {
        auto const squared_distance = ::pastel::geometry::squared_norm(position1 - position2);
        if (squared_distance > squared_cutoff_length_)
          return Value{};

        auto const diameter = (diameter1 + diameter2) / Value{2};
        return bare_force_.calculate_energy(squared_distance, diameter) - bare_force_.calculate_energy(squared_cutoff_length_, diameter);
      }
    }; // lennard_jones<true, false, Value>


    template <bool has_cutoff, bool has_diameter, typename Value>
    inline constexpr bool operator!=(
      ::pastel::force::lennard_jones<has_cutoff, has_diameter, Value> const& lhs,
      ::pastel::force::lennard_jones<has_cutoff, has_diameter, Value> const& rhs)
    { return !(lhs == rhs); }

    template <bool has_cutoff, bool has_diameter, typename Value>
    inline void swap(
      ::pastel::force::lennard_jones<has_cutoff, has_diameter, Value>& lhs,
      ::pastel::force::lennard_jones<has_cutoff, has_diameter, Value>& rhs)
      noexcept(noexcept(lhs.swap(rhs)))
    { lhs.swap(rhs); }
  } // namespace force
} // namespace pastel


#endif // PASTEL_FORCE_LENNARD_JONES_HPP

