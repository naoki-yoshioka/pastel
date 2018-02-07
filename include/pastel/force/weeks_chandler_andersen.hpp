#ifndef PASTEL_FORCE_WEEKS_CHANDLER_ANDERSEN_HPP
# define PASTEL_FORCE_WEEKS_CHANDLER_ANDERSEN_HPP

# include <cassert>
# include <cmath>
# include <utility>

# include <pastel/force/lennard_jones.hpp>
# include <pastel/force/tags.hpp>
# include <pastel/geometry/squared_norm.hpp>
# include <pastel/utility/is_nothrow_swappable.hpp>


namespace pastel
{
  namespace force
  {
    template <bool has_diameter = true, typename Value = double>
    class weeks_chandler_andersen;

    namespace weeks_chandler_andersen_detail
    {
      template <typename Value>
      inline Value cutoff_length_coefficient()
      {
        using std::pow;
        return pow(Value{2}, Value{1}/Value{6});
      }
    } // namespace weeks_chandler_andersen_detail

    template <typename Value>
    class weeks_chandler_andersen<true, Value>
    {
      using bare_force_type = ::pastel::force::lennard_jones<false, true, Value>;
      bare_force_type bare_force_;
      Value cutoff_length_;

      Value squared_cutoff_length_;

     public:
      using value_type = Value;
      using required_arguments_category = ::pastel::force::tags::requires_position;
      static constexpr bool has_potential_energy = true;
      static constexpr bool has_cutoff = true;

      weeks_chandler_andersen()
        : bare_force_{},
          cutoff_length_{::pastel::force::weeks_chandler_andersen_detail::cutoff_length_coefficient<Value>() * bare_force_.diameter()},
          squared_cutoff_length_{cutoff_length_ * cutoff_length_}
      { }

      ~weeks_chandler_andersen() noexcept = default;
      weeks_chandler_andersen(weeks_chandler_andersen const&) = default;
      weeks_chandler_andersen(weeks_chandler_andersen&&) noexcept = default;
      weeks_chandler_andersen& operator=(weeks_chandler_andersen const&) = default;
      weeks_chandler_andersen& operator=(weeks_chandler_andersen&&) noexcept = default;

      weeks_chandler_andersen(Value const& energy_depth, Value const& diameter) noexcept
        : bare_force_{energy_depth, diameter},
          cutoff_length_{::pastel::force::weeks_chandler_andersen_detail::cutoff_length_coefficient<Value>() * diameter},
          squared_cutoff_length_{cutoff_length_ * cutoff_length_}
      { assert(energy_depth > Value{0} && diameter > Value{0}); }

      constexpr bool operator==(weeks_chandler_andersen const& other) const
      {
# ifdef NDEBUG
        return bare_force_ == other.bare_force_ && cutoff_length_ == other.cutoff_length_;
# else
        return bare_force_ == other.bare_force_ && cutoff_length_ == other.cutoff_length_
          && squared_cutoff_length_ == other.squared_cutoff_length_;
# endif
      }

      void swap(weeks_chandler_andersen& other)
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

      constexpr Value const& diameter() const { return bare_force_.diameter(); }
      void diameter(Value const& new_diameter)
      {
        assert(new_diameter > Value{0} && cutoff_length_ > new_diameter);
        bare_force_.diameter(new_diameter);
        cutoff_length_ = ::pastel::force::weeks_chandler_andersen_detail::cutoff_length_coefficient<Value>() * new_diameter;
        squared_cutoff_length_ = cutoff_length_ * cutoff_length_;
      }

      constexpr Value const& cutoff_length() const { return cutoff_length_; }

      template <typename Point>
      auto operator()(Point const& position1, Point const& position2) const
        -> decltype(bare_force_.calculate_force(position1 - position2, ::pastel::geometry::squared_norm(position1 - position2)))
      {
        auto const difference = position1 - position2;
        auto const squared_distance = ::pastel::geometry::squared_norm(difference);
        if (squared_distance > squared_cutoff_length_)
          return decltype(bare_force_.calculate_force(difference, squared_distance)){};
        return bare_force_.calculate_force(difference, squared_distance);
      }

      template <typename Point>
      Value energy(Point const& position1, Point const& position2) const
      {
        auto const squared_distance = ::pastel::geometry::squared_norm(position1 - position2);
        if (squared_distance > squared_cutoff_length_)
          return Value{};
        return bare_force_.calculate_energy(squared_distance) + bare_force_.energy_depth();
      }
    }; // weeks_chandler_andersen<true, Value>


    template <typename Value>
    class weeks_chandler_andersen<false, Value>
    {
      using bare_force_type = ::pastel::force::lennard_jones<false, false, Value>;
      bare_force_type bare_force_;

      Value max_cutoff_length_;
      Value cutoff_length_coefficient_;

      Value squared_cutoff_length_coefficient_;

     public:
      using value_type = Value;
      using required_arguments_category = ::pastel::force::tags::requires_position;
      static constexpr bool has_potential_energy = true;
      static constexpr bool has_cutoff = true;

      weeks_chandler_andersen()
        : bare_force_{},
          max_cutoff_length_{::pastel::force::weeks_chandler_andersen_detail::cutoff_length_coefficient<Value>()},
          cutoff_length_coefficient_{max_cutoff_length_},
          squared_cutoff_length_coefficient_{cutoff_length_coefficient_ * cutoff_length_coefficient_}
      { }

      ~weeks_chandler_andersen() noexcept = default;
      weeks_chandler_andersen(weeks_chandler_andersen const&) = default;
      weeks_chandler_andersen(weeks_chandler_andersen&&) noexcept = default;
      weeks_chandler_andersen& operator=(weeks_chandler_andersen const&) = default;
      weeks_chandler_andersen& operator=(weeks_chandler_andersen&&) noexcept = default;

      explicit weeks_chandler_andersen(Value const& energy_depth)
        : weeks_chandler_andersen{energy_depth, Value{1}}
      { }

      constexpr weeks_chandler_andersen(Value const& energy_depth, Value const& max_diameter)
        : bare_force_{energy_depth},
          max_cutoff_length_{max_diameter * ::pastel::force::weeks_chandler_andersen_detail::cutoff_length_coefficient<Value>()},
          cutoff_length_coefficient_{::pastel::force::weeks_chandler_andersen_detail::cutoff_length_coefficient<Value>()},
          squared_cutoff_length_coefficient_{cutoff_length_coefficient_ * cutoff_length_coefficient_}
      { assert(energy_depth > Value{0} && max_diameter > Value{0}); }

      constexpr bool operator==(weeks_chandler_andersen const& other) const
      {
# ifdef NDEBUG
        return bare_force_ == other.bare_force_ && max_cutoff_length_ == other.max_cutoff_length_
          && cutoff_length_coefficient_ == other.cutoff_length_coefficient_;
# else
        return bare_force_ == other.bare_force_ && max_cutoff_length_ == other.max_cutoff_length_
          && cutoff_length_coefficient_ == other.cutoff_length_coefficient_
          && squared_cutoff_length_coefficient_ == other.squared_cutoff_length_coefficient_;
# endif
      }

      void swap(weeks_chandler_andersen& other)
        noexcept(
          ::pastel::utility::is_nothrow_swappable<bare_force_type>::value
          && ::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(bare_force_, other.bare_force_);
        swap(max_cutoff_length_, other.max_cutoff_length_);
        swap(cutoff_length_coefficient_, other.cutoff_length_coefficient_);
        swap(squared_cutoff_length_coefficient_, other.squared_cutoff_length_coefficient_);
      }

      constexpr Value const& energy_depth() const { return bare_force_.energy_depth(); }
      void energy_depth(Value const& new_energy_depth)
      {
        assert(new_energy_depth > Value{0});
        bare_force_.energy_depth(new_energy_depth);
      }

      constexpr Value const& cutoff_length() const { return max_cutoff_length_; }
      void cutoff_length(Value const& new_cutoff_length)
      {
        assert(new_cutoff_length > Value{0});
        max_cutoff_length_ = new_cutoff_length;
      }

      constexpr Value const& cutoff_length_coefficient() const { return cutoff_length_coefficient_; }

      template <typename Point>
      auto operator()(Point const& position1, Value const& diameter1, Point const& position2, Value const& diameter2) const
        -> decltype(bare_force_.calculate_force(position1 - position2, (diameter1 + diameter2) / Value{2}))
      {
        auto const diameter = (diameter1 + diameter2) / Value{2};
        auto const squared_diameter = diameter * diameter;
        auto const difference = position1 - position2;
        auto const squared_distance = ::pastel::geometry::squared_norm(difference);
        if (squared_distance > squared_cutoff_length_coefficient_ * squared_diameter)
          return decltype(bare_force_.calculate_force(difference, squared_distance, squared_diameter)){};

        return bare_force_.calculate_force(difference, squared_distance, squared_diameter);
      }

      template <typename Point>
      Value energy(Point const& position1, Value const& diameter1, Point const& position2, Value const& diameter2) const
      {
        auto const diameter = (diameter1 + diameter2) / Value{2};
        auto const squared_cutoff_length = squared_cutoff_length_coefficient_ * diameter * diameter;
        auto const squared_distance = ::pastel::geometry::squared_norm(position1 - position2);
        if (squared_distance > squared_cutoff_length)
          return Value{};

        return bare_force_.calculate_energy(squared_distance, diameter) - bare_force_.calculate_energy(squared_cutoff_length, diameter);
      }
    }; // weeks_chandler_andersen<false, Value>


    template <bool has_diameter, typename Value>
    inline constexpr bool operator!=(
      ::pastel::force::weeks_chandler_andersen<has_diameter, Value> const& lhs,
      ::pastel::force::weeks_chandler_andersen<has_diameter, Value> const& rhs)
    { return !(lhs == rhs); }

    template <bool has_diameter, typename Value>
    inline void swap(
      ::pastel::force::weeks_chandler_andersen<has_diameter, Value>& lhs,
      ::pastel::force::weeks_chandler_andersen<has_diameter, Value>& rhs)
      noexcept(noexcept(lhs.swap(rhs)))
    { lhs.swap(rhs); }
  } // namespace force
} // namespace pastel


#endif // PASTEL_FORCE_WEEKS_CHANDLER_ANDERSEN_HPP

