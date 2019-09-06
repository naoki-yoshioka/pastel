#ifndef PASTEL_UTILITY_PROD_HPP
# define PASTEL_UTILITY_PROD_HPP

# include <utility>

# include <pastel/utility/foldl.hpp>


namespace pastel
{
  namespace utility
  {
    namespace prod_detail
    {
      struct multiplies
      {
        template <typename Value1, typename Value2>
        auto operator()(Value1&& value1, Value2&& value2) const
          -> decltype(std::forward<Value1>(value1) * std::forward<Value2>(value2))
        { return std::forward<Value1>(value1) * std::forward<Value2>(value2); }
      };
    } // namespace prod_detail

    template <typename... Values>
    inline auto prod(Values&&... values)
      -> decltype(::pastel::utility::foldl(::pastel::utility::prod_detail::multiplies{}, std::forward<Values>(values)...))
    { return ::pastel::utility::foldl(::pastel::utility::prod_detail::multiplies{}, std::forward<Values>(values)...); }

    template <typename Value, std::size_t N>
    inline auto prod(std::array<Value, N> const& array)
      -> decltype(::pastel::utility::foldl(::pastel::utility::prod_detail::multiplies{}, array))
    { return ::pastel::utility::foldl(::pastel::utility::prod_detail::multiplies{}, array); }
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_PROD_HPP

