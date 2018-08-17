#ifndef PASTEL_UTILITY_FOLDL_HPP
# define PASTEL_UTILITY_FOLDL_HPP

# include <utility>


namespace pastel
{
  namespace utility
  {
    template <typename Function, typename Value1, typename Value2>
    inline auto foldl(Function&& function, Value1&& value1, Value2&& value2)
      -> decltype(function(std::forward<Value1>(value1), std::forward<Value2>(value2)))
    { return function(std::forward<Value1>(value1), std::forward<Value2>(value2)); }

    template <typename Function, typename Value, typename... Values>
    inline auto foldl(Function&& function, Value&& value, Values&&... values)
      -> decltype(function(std::forward<Value>(value), ::pastel::utility::foldl(std::forward<Values>(values)...)))
    { return function(std::forward<Value>(value), ::pastel::utility::foldl(std::forward<Values>(values)...)); }
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_FOLDL_HPP

