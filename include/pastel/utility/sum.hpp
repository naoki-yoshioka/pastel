#ifndef PASTEL_UTILITY_SUM_HPP
# define PASTEL_UTILITY_SUM_HPP

# include <utility>

# include <pastel/utility/foldl.hpp>


namespace pastel
{
  namespace utility
  {
    namespace sum_detail
    {
      struct plus
      {
        template <typename Value1, typename Value2>
        auto operator()(Value1&& value1, Value2&& value2) const
          -> decltype(std::forward<Value1>(value1) + std::forward<Value2>(value2))
        { return std::forward<Value1>(value1) + std::forward<Value2>(value2); }
      };
    } // namespace sum_detail

    template <typename... Values>
    inline auto sum(Values&&... values)
      -> decltype(::pastel::utility::foldl(::pastel::utility::sum_detail::plus{}, std::forward<Values>(values)...))
    { return ::pastel::utility::foldl(::pastel::utility::sum_detail::plus{}, std::forward<Values>(values)...); }
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_SUM_HPP

