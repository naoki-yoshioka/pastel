#ifndef PASTEL_UTILITY_TUPLE_META_COUNT_HPP
# define PASTEL_UTILITY_TUPLE_META_COUNT_HPP

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
        namespace count_detail
        {
          template <std::size_t result, typename Type, typename T, typename... Args>
          struct count
            : ::pastel::utility::tuple::meta::count_detail::count<
                std::is_same<Type, T>::value ? result+1u : result,
                Type, Args...>
          { };

          template <std::size_t result, typename Type>
          struct count<result, Type>
            : std::integral_constant<std::size_t, result>
          { };
        } // namespace count_detail


        template <typename Tuple, typename Type>
        struct count;

        template <typename Type, typename... Args>
        struct count<std::tuple<Args...>, Type>
          : ::pastel::utility::tuple::meta::count_detail::count<0u, Type, Args...>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_COUNT_HPP

