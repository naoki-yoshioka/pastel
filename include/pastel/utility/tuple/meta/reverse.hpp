#ifndef PASTEL_UTILITY_TUPLE_META_REVERSE_HPP
# define PASTEL_UTILITY_TUPLE_META_REVERSE_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        namespace reverse_detail
        {
          template <typename Result, typename... Args>
          struct reverse;

          template <typename... ResultArgs, typename Type, typename... Args>
          struct reverse<std::tuple<ResultArgs...>, Type, Args...>
            : ::pastel::utility::tuple::meta::reverse_detail::reverse<std::tuple<Type, ResultArgs...>, Args...>
          { };

          template <typename Result>
          struct reverse<Result>
          { using type = Result; };
        } // namespace reverse_detail


        template <typename Tuple>
        struct reverse;

        template <typename... Args>
        struct reverse<std::tuple<Args...>>
          : ::pastel::utility::tuple::meta::reverse_detail::reverse<std::tuple<>, Args...>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_REVERSE_HPP

