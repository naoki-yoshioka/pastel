#ifndef PASTEL_UTILITY_NEXT_POWER_OF_TWO_HPP
# define PASTEL_UTILITY_NEXT_POWER_OF_TWO_HPP

# include <type_traits>


namespace pastel
{
  namespace utility
  {
    namespace next_power_of_two_detail
    {
      template <typename T>
      inline constexpr T calculate(T value, T power_of_two)
      { return value > power_of_two ? ::pastel::utility::next_power_of_two_detail::calculate(value, power_of_two*T{2}) : power_of_two; }
    } // next_power_of_two_detail

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
    inline constexpr T next_power_of_two(T value)
    { return ::pastel::utility::next_power_of_two_detail::calculate(value, T{1}); }
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_NEXT_POWER_OF_TWO_HPP

