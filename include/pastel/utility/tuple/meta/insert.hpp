#ifndef PASTEL_UTILITY_TUPLE_META_INSERT_HPP
# define PASTEL_UTILITY_TUPLE_META_INSERT_HPP

# include <cstddef>:
# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        namespace insert_detail
        {
          template <typename Result, typename... Args>
          struct insert_after;

          template <typename... ResultArgs, typename T, typename... Args>
          struct insert_after<std::tuple<ResultArgs...>, T, Args...>
            : ::pastel::utility::tuple::meta::insert_detail::insert_after<std::tuple<ResultArgs..., T>, Args...>
          { };

          template <typename Result>
          struct insert_after
          { using type = Result; };


          template <typename Result, std::size_t index, typename Type, typename... Args>
          struct insert_before;

          template <typename... ResultArgs, std::size_t index, typename Type, typename T, typename... Args>
          struct insert_before<std::tuple<ResultArgs...>, index, Type, T, Args...>
            : ::pastel::utility::tuple::meta::insert_detail::insert_before<std::tuple<ResultArgs..., T>, index-1u, Type, Args...>
          { };

          template <typename... ResultArgs, typename Type, typename... Args>
          struct insert_before<std::tuple<ResultArgs...>, 0u, Type, Args...>
            : ::pastel::utility::tuple::meta::insert_detail::insert_after<std::tuple<ResultArgs..., Type>, Args...>
          { };
        } // namespace insert_detail


        template <typename Tuple, std::size_t index, typename Type>
        struct insert;

        template <std::size_t index, typename Type, typename... Args>
        struct insert<std::tuple<Args...>, index, Type>
          : ::pastel::utility::tuple::meta::insert_detail::insert_before<std::tuple<>, index, Type, Args...>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_INSERT_HPP

