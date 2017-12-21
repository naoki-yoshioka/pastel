#ifndef PASTEL_UTILITY_TUPLE_META_PUSH_FRONT_HPP
# define PASTEL_UTILITY_TUPLE_META_PUSH_FRONT_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        template <typename Tuple, typename Type>
        struct push_front;

        template <typename Type, typename... Args>
        struct push_front<std::tuple<Args...>, Type>
        { using type = std::tuple<Type, Args...>; };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_PUSH_FRONT_HPP

