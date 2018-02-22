#ifndef PASTEL_KERNEL_WENDLAND_HPP
# define PASTEL_KERNEL_WENDLAND_HPP

# include <cassert>
# include <cstddef>
# include <utility>

# include <boost/math/constants/constants.hpp>

# include <pastel/utility/is_nothrow_swappable.hpp>


namespace pastel
{
  namespace kernel
  {
    template <std::size_t order = 1u, std::size_t dimension = 3u, typename Value = double>
    class wendland;

    namespace wendland_detail
    {
      // psi_{2,1}(r) = (1-r)_+^3 (1+3r)
      //  (...)_+ = max{0, ...}
      template <typename Value>
      inline Value wendland21(Value const& value)
      {
        assert(value >= Value{0});
# ifndef NDEBUG
        if (value >= Value{1})
          return Value{0};
# endif

        auto const one_minus_value = Value{1} - value;
        auto const one_minus_value_3 = one_minus_value * one_minus_value * one_minus_value;
        return one_minus_value_3 * (Value{1} + Value{3} * value);
      }

      // psi'_{2,1}(r) = -12r(1-r)_+^2
      template <typename Value>
      inline Value derivative_wendland21(Value const& value)
      {
        assert(value >= Value{0});
# ifndef NDEBUG
        if (value >= Value{1})
          return Value{0};
# endif

        auto const one_minus_value = Value{1} - value;
        auto const one_minus_value_2 = one_minus_value * one_minus_value;
        return -Value{12} * value * one_minus_value_2;
      }

      // psi_{3,1}(r) = (1-r)_+^4 (1+4r)
      template <typename Value>
      inline Value wendland31(Value const& value)
      {
        assert(value >= Value{0});
# ifndef NDEBUG
        if (value >= Value{1})
          return Value{0};
# endif

        auto const one_minus_value = Value{1} - value;
        auto const one_minus_value_2 = one_minus_value * one_minus_value;
        auto const one_minus_value_4 = one_minus_value_2 * one_minus_value_2;
        return one_minus_value_4 * (Value{1} + Value{4} * value);
      }

      // psi'_{3,1}(r) = -20r(1-r)_+^3
      template <typename Value>
      inline Value derivative_wendland31(Value const& value)
      {
        assert(value >= Value{0});
# ifndef NDEBUG
        if (value >= Value{1})
          return Value{0};
# endif

        auto const one_minus_value = Value{1} - value;
        auto const one_minus_value_3 = one_minus_value * one_minus_value * one_minus_value;
        return -Value{20} * value * one_minus_value_3;
      }


      // psi_{3,2}(r) = (1-r)_+^5 (1+5r+8r^2)
      template <typename Value>
      inline Value wendland32(Value const& value)
      {
        assert(value >= Value{0});
# ifndef NDEBUG
        if (value >= Value{1})
          return Value{0};
# endif

        auto const one_minus_value = Value{1} - value;
        auto const one_minus_value_2 = one_minus_value * one_minus_value;
        auto const one_minus_value_5 = one_minus_value_2 * one_minus_value_2 * one_minus_value;
        return one_minus_value_5 * (Value{1} + Value{5} * value + Value{8} * value * value);
      }

      // psi'_{3,2}(r) = -14r(4r+1)(1-r)_+^4
      template <typename Value>
      inline Value derivative_wendland32(Value const& value)
      {
        assert(value >= Value{0});
# ifndef NDEBUG
        if (value >= Value{1})
          return Value{0};
# endif

        auto const one_minus_value = Value{1} - value;
        auto const one_minus_value_2 = one_minus_value * one_minus_value;
        auto const one_minus_value_4 = one_minus_value_2 * one_minus_value_2;
        return -Value{14} * value * (Value{4} * value + Value{1}) * one_minus_value_4;
      }

      // psi_{4,2}(r) = (1-r)_+^6 (1+6r+(35/3)r^2)
      template <typename Value>
      inline Value wendland42(Value const& value)
      {
        assert(value >= Value{0});
# ifndef NDEBUG
        if (value >= Value{1})
          return Value{0};
# endif

        auto const one_minus_value = Value{1} - value;
        auto const one_minus_value_2 = one_minus_value * one_minus_value;
        auto const one_minus_value_6 = one_minus_value_2 * one_minus_value_2 * one_minus_value_2;
        return one_minus_value_6 * (Value{1} + Value{6} * value + Value{35} * value * value / Value{3});
      }

      // psi'_{4,2}(r) = -(56/3)r(5r+1)(1-r)_+^5
      template <typename Value>
      inline Value derivative_wendland42(Value const& value)
      {
        assert(value >= Value{0});
# ifndef NDEBUG
        if (value >= Value{1})
          return Value{0};
# endif

        auto const one_minus_value = Value{1} - value;
        auto const one_minus_value_2 = one_minus_value * one_minus_value;
        auto const one_minus_value_5 = one_minus_value_2 * one_minus_value_2 * one_minus_value;
        return -Value{56} / Value{3} * value * (Value{5} * value + Value{1}) * one_minus_value_3;
      }


      // psi_{4,3}(r) = (1-r)_+^7 (1+7r+19r^2+21r^3)
      template <typename Value>
      inline Value wendland43(Value const& value)
      {
        assert(value >= Value{0});
# ifndef NDEBUG
        if (value >= Value{1})
          return Value{0};
# endif

        auto const one_minus_value = Value{1} - value;
        auto const one_minus_value_2 = one_minus_value * one_minus_value;
        auto const one_minus_value_7 = one_minus_value_2 * one_minus_value_2 * one_minus_value_2 * one_minus_value;
        auto const value_2 = value * value;
        return one_minus_value_7 * (Value{1} + Value{7} * value + Value{19} * value_2 + Value{21} * value_2 * value);
      }

      // psi'_{4,3}(r) = -6r(35r^2+18r+3)(1-r)_+^6
      template <typename Value>
      inline Value derivative_wendland43(Value const& value)
      {
        assert(value >= Value{0});
# ifndef NDEBUG
        if (value >= Value{1})
          return Value{0};
# endif

        auto const one_minus_value = Value{1} - value;
        auto const one_minus_value_2 = one_minus_value * one_minus_value;
        auto const one_minus_value_6 = one_minus_value_2 * one_minus_value_2 * one_minus_value_2;
        return -Value{6} * value * (Value{35} * value * value + Value{18} * value + Value{3}) * one_minus_value_6;
      }

      // psi_{5,3}(r) = (1-r)_+^8 (1+8r+25r^2+32r^3)
      template <typename Value>
      inline Value wendland53(Value const& value)
      {
        assert(value >= Value{0});
# ifndef NDEBUG
        if (value >= Value{1})
          return Value{0};
# endif

        auto const one_minus_value = Value{1} - value;
        auto const one_minus_value_2 = one_minus_value * one_minus_value;
        auto const one_minus_value_4 = one_minus_value_2 * one_minus_value_2;
        auto const one_minus_value_8 = one_minus_value_4 * one_minus_value_4;
        auto const value_2 = value * value;
        return one_minus_value_8 * (Value{1} + Value{8} * value + Value{25} * value_2 + Value{32} * value_2 * value);
      }

      // psi'_{5,3}(r) = -22r(16r^2+7r+1)(1-r)_+^7
      template <typename Value>
      inline Value derivative_wendland53(Value const& value)
      {
        assert(value >= Value{0});
# ifndef NDEBUG
        if (value >= Value{1})
          return Value{0};
# endif

        auto const one_minus_value = Value{1} - value;
        auto const one_minus_value_2 = one_minus_value * one_minus_value;
        auto const one_minus_value_7 = one_minus_value_2 * one_minus_value_2 * one_minus_value_2 * one_minus_value;
        return -Value{22} * value * (Value{16} * value * value + Value{7} * value + Value{1}) * one_minus_value_7;
      }
    } // namespace wendland_detail


    // W(x, h) = H^{-1} w(|x|/H) = (C_1/H) psi_{1+1+floor(1/2),1}(|x|/H)
    // div_a W(r_{ab}, h) = C_1/H^2 psi'_{2,1}(|r_{ab}|/H) n_{ab}
    //  C_1=5/4, r_{ab} = r_a - r_b, n_{ab} = r_{ab} / |r_{ab}|
    template <typename Value>
    class wendland<1u, 1u, Value>
    {
      Value support_radius_;

      Value inverse_support_radius_;
      Value coefficient_;
      Value derivative_coefficient_;

     public:
      constexpr wendland()
        : support_radius_{Value{1}},
          inverse_support_radius_{Value{1}},
          coefficient_{Value{5} / Value{4}},
          derivative_coefficient_{coefficient_}
      { }

      explicit constexpr wendland(Value const& support_radius)
        : support_radius_{support_radius},
          inverse_support_radius_{Value{1} / support_radius},
          coefficient_{Value{5} * inverse_support_radius_ / Value{4}},
          derivative_coefficient_{coefficient_ * inverse_support_radius_}
      { assert(support_radius_ > Value{0}); }

      constexpr bool operator==(wendland const& other) const
      {
# ifdef NDEBUG
        return support_radius_ == other.support_radius_;
# else
        return support_radius_ == other.support_radius_
          && inverse_support_radius_ == other.inverse_support_radius_
          && coefficient_ == other.coefficient_
          && derivative_coefficient_ == other.derivative_coefficient_;
# endif
      }

      void swap(wendland& other)
        noexcept(::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(support_radius_, other.support_radius_);
        swap(inverse_support_radius_, other.inverse_support_radius_);
        swap(coefficient_, other.coefficient_);
        swap(derivative_coefficient_, other.derivative_coefficient_);
      }

      Value const& support_radius() const { return support_radius_; }
      void support_radius(Value const& new_support_radius)
      {
        assert(new_support_radius > Value{0});
        support_radius_ = new_support_radius;
        inverse_support_radius_ = Value{1} / new_support_radius;
        coefficient_ = Value{5} * inverse_support_radius_ / Value{4};
        derivative_coefficient_ = coefficient_ * inverse_support_radius_;
      }

      Value operator()(Value const& value) const
      {
        assert(value >= Value{0});
        return coefficient_ * ::pastel::kernel::wendland_detail::wendland21(value * inverse_support_radius_);
      }

      Value derivative(Value const& value) const
      {
        assert(value >= Value{0});
        return derivative_coefficient_ * ::pastel::kernel::wendland_detail::derivative_wendland21(value * inverse_support_radius_);
      }
    }; // class wendland<1u, 1u, Value>

    // W(x, h) = H^{-2} w(|x|/H) = (C_2/H^2) psi_{1+1+floor(2/2),1}(|x|/H)
    // div_a W(r_{ab}, h) = C_2/H^3 psi'_{3,1}(|r_{ab}|/H) n_{ab}
    //  C_2=7/pi, r_{ab} = r_a - r_b, n_{ab} = r_{ab} / |r_{ab}|
    template <typename Value>
    class wendland<1u, 2u, Value>
    {
      Value support_radius_;

      Value inverse_support_radius_;
      Value coefficient_;
      Value derivative_coefficient_;

     public:
      wendland()
        : support_radius_{Value{1}},
          inverse_support_radius_{Value{1}},
          coefficient_{Value{7} / boost::math::constants::pi<Value>()},
          derivative_coefficient_{coefficient_}
      { }

      explicit wendland(Value const& support_radius)
        : support_radius_{support_radius},
          inverse_support_radius_{Value{1} / support_radius},
          coefficient_{Value{7} * inverse_support_radius_ * inverse_support_radius_ / boost::math::constants::pi<Value>()},
          derivative_coefficient_{coefficient_ * inverse_support_radius_}
      { assert(support_radius_ > Value{0}); }

      constexpr bool operator==(wendland const& other) const
      {
# ifdef NDEBUG
        return support_radius_ == other.support_radius_;
# else
        return support_radius_ == other.support_radius_
          && inverse_support_radius_ == other.inverse_support_radius_
          && coefficient_ == other.coefficient_
          && derivative_coefficient_ == other.derivative_coefficient_;
# endif
      }

      void swap(wendland& other)
        noexcept(::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(support_radius_, other.support_radius_);
        swap(inverse_support_radius_, other.inverse_support_radius_);
        swap(coefficient_, other.coefficient_);
        swap(derivative_coefficient_, other.derivative_coefficient_);
      }

      Value const& support_radius() const { return support_radius_; }
      void support_radius(Value const& new_support_radius)
      {
        assert(new_support_radius > Value{0});
        support_radius_ = new_support_radius;
        inverse_support_radius_ = Value{1} / new_support_radius;
        coefficient_ = Value{7} * inverse_support_radius_ * inverse_support_radius_ / boost::math::constants::pi<Value>();
        derivative_coefficient_ = coefficient_ * inverse_support_radius_;
      }

      Value operator()(Value const& value) const
      {
        assert(value >= Value{0});
        return coefficient_ * ::pastel::kernel::wendland_detail::wendland31(value * inverse_support_radius_);
      }

      Value derivative(Value const& value) const
      {
        assert(value >= Value{0});
        return derivative_coefficient_ * ::pastel::kernel::wendland_detail::derivative_wendland31(value * inverse_support_radius_);
      }
    }; // class wendland<1u, 2u, Value>

    // W(x, h) = H^{-3} w(|x|/H) = (C_3/H^3) \psi_{1+1+floor(3/2),1}(|x|/H)
    // div_a W(r_{ab}, h) = C_3/H^4 psi'_{3,1}(|r_{ab}|/H) n_{ab}
    //  C_3=21/2pi, r_{ab} = r_a - r_b, n_{ab} = r_{ab} / |r_{ab}|
    template <typename Value>
    class wendland<1u, 3u, Value>
    {
      Value support_radius_;

      Value inverse_support_radius_;
      Value coefficient_;
      Value derivative_coefficient_;

     public:
      wendland()
        : support_radius_{Value{1}},
          inverse_support_radius_{Value{1}},
          coefficient_{Value{21} * boost::math::constants::one_div_two_pi<Value>()},
          derivative_coefficient_{coefficient_}
      { }

      explicit wendland(Value const& support_radius)
        : support_radius_{support_radius},
          inverse_support_radius_{Value{1} / support_radius},
          coefficient_{Value{21} * inverse_support_radius_ * inverse_support_radius_ * inverse_support_radius_ * boost::math::constants::one_div_two_pi<Value>()},
          derivative_coefficient_{coefficient_ * inverse_support_radius_}
      { assert(support_radius_ > Value{0}); }

      constexpr bool operator==(wendland const& other) const
      {
# ifdef NDEBUG
        return support_radius_ == other.support_radius_;
# else
        return support_radius_ == other.support_radius_
          && inverse_support_radius_ == other.inverse_support_radius_
          && coefficient_ == other.coefficient_
          && derivative_coefficient_ == other.derivative_coefficient_;
# endif
      }

      void swap(wendland& other)
        noexcept(::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(support_radius_, other.support_radius_);
        swap(inverse_support_radius_, other.inverse_support_radius_);
        swap(coefficient_, other.coefficient_);
        swap(derivative_coefficient_, other.derivative_coefficient_);
      }

      Value const& support_radius() const { return support_radius_; }
      void support_radius(Value const& new_support_radius)
      {
        assert(new_support_radius > Value{0});
        support_radius_ = new_support_radius;
        inverse_support_radius_ = Value{1} / new_support_radius;
        coefficient_ = Value{21} * inverse_support_radius_ * inverse_support_radius_ * inverse_support_radius_ * boost::math::constants::one_div_two_pi<Value>();
        derivative_coefficient_ = coefficient_ * inverse_support_radius_;
      }

      Value operator()(Value const& value) const
      {
        assert(value >= Value{0});
        return coefficient_ * ::pastel::kernel::wendland_detail::wendland31(value * inverse_support_radius_);
      }

      Value derivative(Value const& value) const
      {
        assert(value >= Value{0});
        return derivative_coefficient_ * ::pastel::kernel::wendland_detail::derivative_wendland31(value * inverse_support_radius_);
      }
    }; // class wendland<1u, 3u, Value>


    // W(x, h) = H^{-1} w(|x|/H) = (C_1/H) psi_{2+1+floor(1/2),2}(|x|/H)
    // div_a W(r_{ab}, h) = C_1/H^2 psi'_{3,2}(|r_{ab}|/H) n_{ab}
    //  C_1=3/2, r_{ab} = r_a - r_b, n_{ab} = r_{ab} / |r_{ab}|
    template <typename Value>
    class wendland<2u, 1u, Value>
    {
      Value support_radius_;

      Value inverse_support_radius_;
      Value coefficient_;
      Value derivative_coefficient_;

     public:
      wendland()
        : support_radius_{Value{1}},
          inverse_support_radius_{Value{1}},
          coefficient_{Value{3} / Value{2}},
          derivative_coefficient_{coefficient_}
      { }

      explicit wendland(Value const& support_radius)
        : support_radius_{support_radius},
          inverse_support_radius_{Value{1} / support_radius},
          coefficient_{Value{3} * inverse_support_radius_ / Value{2}},
          derivative_coefficient_{coefficient_ * inverse_support_radius_}
      { assert(support_radius_ > Value{0}); }

      constexpr bool operator==(wendland const& other) const
      {
# ifdef NDEBUG
        return support_radius_ == other.support_radius_;
# else
        return support_radius_ == other.support_radius_
          && inverse_support_radius_ == other.inverse_support_radius_
          && coefficient_ == other.coefficient_
          && derivative_coefficient_ == other.derivative_coefficient_;
# endif
      }

      void swap(wendland& other)
        noexcept(::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(support_radius_, other.support_radius_);
        swap(inverse_support_radius_, other.inverse_support_radius_);
        swap(coefficient_, other.coefficient_);
        swap(derivative_coefficient_, other.derivative_coefficient_);
      }

      Value const& support_radius() const { return support_radius_; }
      void support_radius(Value const& new_support_radius)
      {
        assert(new_support_radius > Value{0});
        support_radius_ = new_support_radius;
        inverse_support_radius_ = Value{1} / new_support_radius;
        coefficient_ = Value{3} * inverse_support_radius_ / Value{2};
        derivative_coefficient_ = coefficient_ * inverse_support_radius_;
      }

      Value operator()(Value const& value) const
      {
        assert(value >= Value{0});
        return coefficient_ * ::pastel::kernel::wendland_detail::wendland32(value * inverse_support_radius_);
      }

      Value derivative(Value const& value) const
      {
        assert(value >= Value{0});
        return derivative_coefficient_ * ::pastel::kernel::wendland_detail::derivative_wendland32(value * inverse_support_radius_);
      }
    }; // class wendland<2u, 1u, Value>

    // W(x, h) = H^{-2} w(|x|/H) = (C_2/H^2) psi_{2+1+floor(2/2),2}(|x|/H)
    // div_a W(r_{ab}, h) = C_2/H^3 psi'_{4,2}(|r_{ab}|/H) n_{ab}
    //  C_2=9/pi, r_{ab} = r_a - r_b, n_{ab} = r_{ab} / |r_{ab}|
    template <typename Value>
    class wendland<2u, 2u, Value>
    {
      Value support_radius_;

      Value inverse_support_radius_;
      Value coefficient_;
      Value derivative_coefficient_;

     public:
      wendland()
        : support_radius_{Value{1}},
          inverse_support_radius_{Value{1}},
          coefficient_{Value{9} / boost::math::constants::pi<Value>()},
          derivative_coefficient_{coefficient_}
      { }

      explicit wendland(Value const& support_radius)
        : support_radius_{support_radius},
          inverse_support_radius_{Value{1} / support_radius},
          coefficient_{Value{9} * inverse_support_radius_ * inverse_support_radius_ / boost::math::constants::pi<Value>()},
          derivative_coefficient_{coefficient_ * inverse_support_radius_}
      { assert(support_radius_ > Value{0}); }

      constexpr bool operator==(wendland const& other) const
      {
# ifdef NDEBUG
        return support_radius_ == other.support_radius_;
# else
        return support_radius_ == other.support_radius_
          && inverse_support_radius_ == other.inverse_support_radius_
          && coefficient_ == other.coefficient_
          && derivative_coefficient_ == other.derivative_coefficient_;
# endif
      }

      void swap(wendland& other)
        noexcept(::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(support_radius_, other.support_radius_);
        swap(inverse_support_radius_, other.inverse_support_radius_);
        swap(coefficient_, other.coefficient_);
        swap(derivative_coefficient_, other.derivative_coefficient_);
      }

      Value const& support_radius() const { return support_radius_; }
      void support_radius(Value const& new_support_radius)
      {
        assert(new_support_radius > Value{0});
        support_radius_ = new_support_radius;
        inverse_support_radius_ = Value{1} / new_support_radius;
        coefficient_ = Value{9} * inverse_support_radius_ * inverse_support_radius_ / boost::math::constants::pi<Value>();
        derivative_coefficient_ = coefficient_ * inverse_support_radius_;
      }

      Value operator()(Value const& value) const
      {
        assert(value >= Value{0});
        return coefficient_ * ::pastel::kernel::wendland_detail::wendland42(value * inverse_support_radius_);
      }

      Value derivative(Value const& value) const
      {
        assert(value >= Value{0});
        return derivative_coefficient_ * ::pastel::kernel::wendland_detail::derivative_wendland42(value * inverse_support_radius_);
      }
    }; // class wendland<2u, 2u, Value>

    // W(x, h) = H^{-3} w(|x|/H) = (C_3/H^3) \psi_{2+1+floor(3/2),2}(|x|/H)
    // div_a W(r_{ab}, h) = C_3/H^4 psi'_{4,2}(|r_{ab}|/H) n_{ab}
    //  C_3=495/32pi, r_{ab} = r_a - r_b, n_{ab} = r_{ab} / |r_{ab}|
    template <typename Value>
    class wendland<2u, 3u, Value>
    {
      Value support_radius_;

      Value inverse_support_radius_;
      Value coefficient_;
      Value derivative_coefficient_;

     public:
      wendland()
        : support_radius_{Value{1}},
          inverse_support_radius_{Value{1}},
          coefficient_{Value{495} / Value{16} * boost::math::constants::one_div_two_pi<Value>()},
          derivative_coefficient_{coefficient_}
      { }

      explicit wendland(Value const& support_radius)
        : support_radius_{support_radius},
          inverse_support_radius_{Value{1} / support_radius},
          coefficient_{Value{495} / Value{16} * inverse_support_radius_ * inverse_support_radius_ * inverse_support_radius_ * boost::math::constants::one_div_two_pi<Value>()},
          derivative_coefficient_{coefficient_ * inverse_support_radius_}
      { assert(support_radius_ > Value{0}); }

      constexpr bool operator==(wendland const& other) const
      {
# ifdef NDEBUG
        return support_radius_ == other.support_radius_;
# else
        return support_radius_ == other.support_radius_
          && inverse_support_radius_ == other.inverse_support_radius_
          && coefficient_ == other.coefficient_
          && derivative_coefficient_ == other.derivative_coefficient_;
# endif
      }

      void swap(wendland& other)
        noexcept(::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(support_radius_, other.support_radius_);
        swap(inverse_support_radius_, other.inverse_support_radius_);
        swap(coefficient_, other.coefficient_);
        swap(derivative_coefficient_, other.derivative_coefficient_);
      }

      Value const& support_radius() const { return support_radius_; }
      void support_radius(Value const& new_support_radius)
      {
        assert(new_support_radius > Value{0});
        support_radius_ = new_support_radius;
        inverse_support_radius_ = Value{1} / new_support_radius;
        coefficient_ = Value{495} / Value{16} * inverse_support_radius_ * inverse_support_radius_ * inverse_support_radius_ * boost::math::constants::one_div_two_pi<Value>();
        derivative_coefficient_ = coefficient_ * inverse_support_radius_;
      }

      Value operator()(Value const& value) const
      {
        assert(value >= Value{0});
        return coefficient_ * ::pastel::kernel::wendland_detail::wendland42(value * inverse_support_radius_);
      }

      Value derivative(Value const& value) const
      {
        assert(value >= Value{0});
        return derivative_coefficient_ * ::pastel::kernel::wendland_detail::derivative_wendland42(value * inverse_support_radius_);
      }
    }; // class wendland<2u, 3u, Value>


    // W(x, h) = H^{-1} w(|x|/H) = (C_1/H) psi_{3+1+floor(1/2),3}(|x|/H)
    // div_a W(r_{ab}, h) = C_1/H^2 psi'_{4,3}(|r_{ab}|/H) n_{ab}
    //  C_1=55/32, r_{ab} = r_a - r_b, n_{ab} = r_{ab} / |r_{ab}|
    template <typename Value>
    class wendland<3u, 1u, Value>
    {
      Value support_radius_;

      Value inverse_support_radius_;
      Value coefficient_;
      Value derivative_coefficient_;

     public:
      wendland()
        : support_radius_{Value{1}},
          inverse_support_radius_{Value{1}},
          coefficient_{Value{55} / Value{32}},
          derivative_coefficient_{coefficient_}
      { }

      explicit wendland(Value const& support_radius)
        : support_radius_{support_radius},
          inverse_support_radius_{Value{1} / support_radius},
          coefficient_{Value{55} * inverse_support_radius_ / Value{32}},
          derivative_coefficient_{coefficient_ * inverse_support_radius_}
      { assert(support_radius_ > Value{0}); }

      constexpr bool operator==(wendland const& other) const
      {
# ifdef NDEBUG
        return support_radius_ == other.support_radius_;
# else
        return support_radius_ == other.support_radius_
          && inverse_support_radius_ == other.inverse_support_radius_
          && coefficient_ == other.coefficient_
          && derivative_coefficient_ == other.derivative_coefficient_;
# endif
      }

      void swap(wendland& other)
        noexcept(::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(support_radius_, other.support_radius_);
        swap(inverse_support_radius_, other.inverse_support_radius_);
        swap(coefficient_, other.coefficient_);
        swap(derivative_coefficient_, other.derivative_coefficient_);
      }

      Value const& support_radius() const { return support_radius_; }
      void support_radius(Value const& new_support_radius)
      {
        assert(new_support_radius > Value{0});
        support_radius_ = new_support_radius;
        inverse_support_radius_ = Value{1} / new_support_radius;
        coefficient_ = Value{55} * inverse_support_radius_ / Value{32};
        derivative_coefficient_ = coefficient_ * inverse_support_radius_;
      }

      Value operator()(Value const& value) const
      {
        assert(value >= Value{0});
        return coefficient_ * ::pastel::kernel::wendland_detail::wendland43(value * inverse_support_radius_);
      }

      Value derivative(Value const& value) const
      {
        assert(value >= Value{0});
        return derivative_coefficient_ * ::pastel::kernel::wendland_detail::derivative_wendland43(value * inverse_support_radius_);
      }
    }; // class wendland<3u, 1u, Value>

    // W(x, h) = H^{-2} w(|x|/H) = (C_2/H^2) psi_{3+1+floor(2/2),3}(|x|/H)
    // div_a W(r_{ab}, h) = C_2/H^3 psi'_{5,3}(|r_{ab}|/H) n_{ab}
    //  C_2=78/7pi, r_{ab} = r_a - r_b, n_{ab} = r_{ab} / |r_{ab}|
    template <typename Value>
    class wendland<3u, 2u, Value>
    {
      Value support_radius_;

      Value inverse_support_radius_;
      Value coefficient_;
      Value derivative_coefficient_;

     public:
      wendland()
        : support_radius_{Value{1}},
          inverse_support_radius_{Value{1}},
          coefficient_{Value{78} / Value{7} / boost::math::constants::pi<Value>()},
          derivative_coefficient_{coefficient_}
      { }

      explicit wendland(Value const& support_radius)
        : support_radius_{support_radius},
          inverse_support_radius_{Value{1} / support_radius},
          coefficient_{Value{78} * inverse_support_radius_ * inverse_support_radius_ / Value{7} / boost::math::constants::pi<Value>()},
          derivative_coefficient_{coefficient_ * inverse_support_radius_}
      { assert(support_radius_ > Value{0}); }

      constexpr bool operator==(wendland const& other) const
      {
# ifdef NDEBUG
        return support_radius_ == other.support_radius_;
# else
        return support_radius_ == other.support_radius_
          && inverse_support_radius_ == other.inverse_support_radius_
          && coefficient_ == other.coefficient_
          && derivative_coefficient_ == other.derivative_coefficient_;
# endif
      }

      void swap(wendland& other)
        noexcept(::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(support_radius_, other.support_radius_);
        swap(inverse_support_radius_, other.inverse_support_radius_);
        swap(coefficient_, other.coefficient_);
        swap(derivative_coefficient_, other.derivative_coefficient_);
      }

      Value const& support_radius() const { return support_radius_; }
      void support_radius(Value const& new_support_radius)
      {
        assert(new_support_radius > Value{0});
        support_radius_ = new_support_radius;
        inverse_support_radius_ = Value{1} / new_support_radius;
        coefficient_ = Value{78} * inverse_support_radius_ * inverse_support_radius_ / Value{7} / boost::math::constants::pi<Value>();
        derivative_coefficient_ = coefficient_ * inverse_support_radius_;
      }

      Value operator()(Value const& value) const
      {
        assert(value >= Value{0});
        return coefficient_ * ::pastel::kernel::wendland_detail::wendland53(value * inverse_support_radius_);
      }

      Value derivative(Value const& value) const
      {
        assert(value >= Value{0});
        return derivative_coefficient_ * ::pastel::kernel::wendland_detail::derivative_wendland53(value * inverse_support_radius_);
      }
    }; // class wendland<3u, 2u, Value>

    // W(x, h) = H^{-3} w(|x|/H) = (C_3/H^3) \psi_{3+1+floor(3/2),3}(|x|/H)
    // div_a W(r_{ab}, h) = C_3/H^4 psi'_{5,3}(|r_{ab}|/H) n_{ab}
    //  C_3=1365/64pi, r_{ab} = r_a - r_b, n_{ab} = r_{ab} / |r_{ab}|
    template <typename Value>
    class wendland<3u, 3u, Value>
    {
      Value support_radius_;

      Value inverse_support_radius_;
      Value coefficient_;
      Value derivative_coefficient_;

     public:
      wendland()
        : support_radius_{Value{1}},
          inverse_support_radius_{Value{1}},
          coefficient_{Value{1365} / Value{32} * boost::math::constants::one_div_two_pi<Value>()},
          derivative_coefficient_{coefficient_}
      { }

      explicit wendland(Value const& support_radius)
        : support_radius_{support_radius},
          inverse_support_radius_{Value{1} / support_radius},
          coefficient_{Value{1365} / Value{32} * inverse_support_radius_ * inverse_support_radius_ * inverse_support_radius_ * boost::math::constants::one_div_two_pi<Value>()},
          derivative_coefficient_{coefficient_ * inverse_support_radius_}
      { assert(support_radius_ > Value{0}); }

      constexpr bool operator==(wendland const& other) const
      {
# ifdef NDEBUG
        return support_radius_ == other.support_radius_;
# else
        return support_radius_ == other.support_radius_
          && inverse_support_radius_ == other.inverse_support_radius_
          && coefficient_ == other.coefficient_
          && derivative_coefficient_ == other.derivative_coefficient_;
# endif
      }

      void swap(wendland& other)
        noexcept(::pastel::utility::is_nothrow_swappable<Value>::value)
      {
        using std::swap;
        swap(support_radius_, other.support_radius_);
        swap(inverse_support_radius_, other.inverse_support_radius_);
        swap(coefficient_, other.coefficient_);
        swap(derivative_coefficient_, other.derivative_coefficient_);
      }

      Value const& support_radius() const { return support_radius_; }
      void support_radius(Value const& new_support_radius)
      {
        assert(new_support_radius > Value{0});
        support_radius_ = new_support_radius;
        inverse_support_radius_ = Value{1} / new_support_radius;
        coefficient_ = Value{1365} / Value{32} * inverse_support_radius_ * inverse_support_radius_ * inverse_support_radius_ * boost::math::constants::one_div_two_pi<Value>();
        derivative_coefficient_ = coefficient_ * inverse_support_radius_;
      }

      Value operator()(Value const& value) const
      {
        assert(value >= Value{0});
        return coefficient_ * ::pastel::kernel::wendland_detail::wendland53(value * inverse_support_radius_);
      }

      Value derivative(Value const& value) const
      {
        assert(value >= Value{0});
        return derivative_coefficient_ * ::pastel::kernel::wendland_detail::derivative_wendland53(value * inverse_support_radius_);
      }
    }; // class wendland<3u, 3u, Value>


    template <std::size_t order, std::size_t dimension, typename Value>
    inline bool operator!=(
      ::pastel::kernel::wendland<order, dimension, Value> const& lhs,
      ::pastel::kernel::wendland<order, dimension, Value> const& rhs)
    { return !(lhs == rhs); }

    template <std::size_t order, std::size_t dimension, typename Value>
    inline void swap(
      ::pastel::kernel::wendland<order, dimension, Value>& lhs,
      ::pastel::kernel::wendland<order, dimension, Value>& rhs)
      noexcept(noexcept(lhs.swap(rhs)))
    { lhs.swap(rhs); }
  } // namespace kernel
} // namespace pastel


#endif // PASTEL_KERNEL_WENDLAND_HPP

