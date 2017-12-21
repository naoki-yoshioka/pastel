#ifndef PASTEL_UTILITY_TUPLE_META_JOIN_HPP
# define PASTEL_UTILITY_TUPLE_META_JOIN_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        template <typename Tuple1, typename Tuple2>
        struct join;

        template <typename... Args1, typename... Args2>
        struct join<std::tuple<Args1...>, std::tuple<Args2...>>
        { using type = std::tuple<Args1..., Args2...>; };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_JOIN_HPP

