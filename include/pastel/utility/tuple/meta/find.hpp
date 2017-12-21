#ifndef PASTEL_UTILITY_TUPLE_META_FIND_HPP
# define PASTEL_UTILITY_TUPLE_META_FIND_HPP

# include <tuple>
# include <type_traits>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        namespace find_detail
        {
          template <std::size_t result, typename Type, typename T, typename... Args>
          struct find
            : std::conditional<
                std::is_same<Type, T>::value,
                std::integral_constant<std::size_t, result>,
                typename ::pastel::utility::tuple::meta::find_detail::find<result+1u, Type, Args...>::type>
          { };

          template <std::size_t result, typename Type>
          struct find<result, Type>
            : std::integral_constant<std::size_t, result>
          { };
        } // namespace find_detail


        template <typename Tuple, typename Type>
        struct find;

        template <typename Type, typename... Args>
        struct find<std::tuple<Args...>, Type>
          : ::pastel::utility::tuple::meta::find_detail::find<0u, Type, Args...>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_FIND_HPP

