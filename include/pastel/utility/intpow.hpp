#ifndef PASTEL_UTILITY_INTPOW_HPP
# define PASTEL_UTILITY_INTPOW_HPP

# include <cstdint>
# include <type_traits>


namespace pastel
{
  namespace utility
  {
    namespace intpow_detail
    {
      template <typename Value, typename Integer>
      constexpr Value intpow_impl(Value const base, Integer const exp)
      {
        return exp == Integer{1}
          ? base
          : ::pastel::utility::intpow_detail::intpow_impl(base, exp / Integer{2})
            * ::pastel::utility::intpow_detail::intpow_impl(base, exp - exp / Integer{2});
      }

      template <typename Value, typename Integer>
      constexpr Value intpow_general(Value const base, Integer const exp)
      { return exp == Integer{0} ? Value{1} : ::pastel::utility::intpow_detail::intpow_impl(base, exp); }

      template <typename Value, typename Integer>
      constexpr Value intpow2(Integer const exp)
      { return static_cast<Value>(std::uintmax_t{1u} << exp); }
    } // namespace intpow_detail

    template <typename Value, typename Integer>
    constexpr Value intpow(Value const base, Integer const exp)
    {
      static_assert(std::is_integral<Integer>::value, "Integer must be an integer type");

      return exp < Integer{0}
        ? ::pastel::utility::intpow(Value{1} / base, -exp)
        : std::is_integral<Value>::value && base == Value{2}
          ? ::pastel::utility::intpow_detail::intpow2<Value>(exp)
          : ::pastel::utility::intpow_detail::intpow_general(base, exp);
    }
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_INTPOW_HPP

