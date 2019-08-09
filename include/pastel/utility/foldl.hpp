#ifndef PASTEL_UTILITY_FOLDL_HPP
# define PASTEL_UTILITY_FOLDL_HPP

# include <utility>
# include <array>


namespace pastel
{
  namespace utility
  {
    template <typename Function, typename Value1, typename Value2>
    inline auto foldl(Function&& function, Value1&& value1, Value2&& value2)
      -> decltype(function(std::forward<Value1>(value1), std::forward<Value2>(value2)))
    { return function(std::forward<Value1>(value1), std::forward<Value2>(value2)); }

    template <typename Function, typename Value1, typename Value2, typename... Values>
    inline auto foldl(Function&& function, Value1&& value1, Value2&& value2, Values&&... values)
      -> decltype(::pastel::utility::foldl(
           function(std::forward<Value1>(value1), std::forward<Value2>(value2)),
           std::forward<Values>(values)...))
    {
      return ::pastel::utility::foldl(
        std::forward<Function>(function),
        function(std::forward<Value1>(value1), std::forward<Value2>(value2)),
        std::forward<Values>(values)...);
    }


    namespace foldl_detail
    {
      template <std::size_t index, std::size_t N>
      struct array_foldl
      {
        template <typename Function, typename Value>
        static Value call(Function&& function, Value const value, std::array<Value, N> const& array)
        {
          return ::pastel::utility::foldl_detail::array_foldl<index+1u, N>::call(
            std::forward<Function>(function), function(value, array[index]), array);
        }
      };

      template <std::size_t N>
      struct array_foldl<N, N>
      {
        template <typename Function, typename Value>
        static Value call(Function&&, Value const value, std::array<Value, N> const&)
        { return value; }
      };


      template <std::size_t N>
      struct foldl
      {
        template <typename Function, typename Value>
        static Value call(Function&& function, std::array<Value, N> const& array)
        { return ::pastel::utility::foldl_detail::array_foldl<1u, N>::call(std::forward<Function>(function), array[0u], array); }
      };

      template <>
      struct foldl<0u>
      {
        template <typename Function, typename Value>
        static Value call(Function&&, std::array<Value, 0u> const&)
        { return Value{}; }
      };
    }

    template <typename Function, typename Value, std::size_t N>
    inline Value foldl(Function&& function, std::array<Value, N> const& array)
    { return ::pastel::utility::foldl_detail::foldl<N>::call(std::forward<Function>(function), array); }
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_FOLDL_HPP

