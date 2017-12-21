#ifndef PASTEL_UTILITY_TUPLE_META_TAIL_HPP
# define PASTEL_UTILITY_TUPLE_META_TAIL_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      template <typename Tuple>
      struct tail;

      template <typename Type, typename... Args>
      struct tail<std::tuple<Type, Args...>>
      { using type = std::tuple<Args...>; };
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_TAIL_HPP

