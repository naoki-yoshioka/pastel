#ifndef PASTEL_UTILITY_IS_INTEGRAL_CONSTANT_HPP
# define PASTEL_UTILITY_IS_INTEGRAL_CONSTANT_HPP

# include <type_traits>


namespace pastel
{
  namespace utility
  {
    template <typename T>
    struct is_integral_constant
      : std::false_type
    { };

    template <typename T, T n>
    struct is_integral_constant<std::integral_constant<T, n>>
      : std::true_type
    { };
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_IS_INTEGRAL_CONSTANT_HPP

