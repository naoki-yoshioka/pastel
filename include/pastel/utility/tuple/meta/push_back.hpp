#ifndef PASTEL_UTILITY_TUPLE_META_PUSH_BACK_HPP
# define PASTEL_UTILITY_TUPLE_META_PUSH_BACK_HPP

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
        struct push_back;

        template <typename Type, typename... Args>
        struct push_back<std::tuple<Args...>, Type>
        { using type = std::tuple<Args..., Type>; };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_PUSH_BACK_HPP

