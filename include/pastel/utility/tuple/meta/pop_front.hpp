#ifndef PASTEL_UTILITY_TUPLE_META_POP_FRONT_HPP
# define PASTEL_UTILITY_TUPLE_META_POP_FRONT_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        template <typename Tuple>
        struct pop_front;

        template <typename Type, typename... Args>
        struct pop_front<std::tuple<Type, Args...>>
        { using type = std::tuple<Args...>; };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_POP_FRONT_HPP

