#ifndef PASTEL_UTILITY_TUPLE_META_ERASE_HPP
# define PASTEL_UTILITY_TUPLE_META_ERASE_HPP

# include <cstddef>
# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        namespace erase_detail
        {
          template <typename Result, typename... Args>
          struct erase_after;

          template <typename... ResultArgs, typename Type, typename... Args>
          struct erase_after<std::tuple<ResultArgs...>, Type, Args...>
            : ::pastel::utility::tuple::meta::erase_detail::erase_after<std::tuple<ResultArgs..., Type>, Args...>
          { };

          template <typename Result>
          struct erase_after<Result>
          { using type = Result; };


          template <typename Result, std::size_t index, typename... Args>
          struct erase_before;

          template <typename... ResultArgs, std::size_t index, typename Type, typename... Args>
          struct erase_before<std::tuple<ResultArgs...>, index, Type, Args...>
            : ::pastel::utility::tuple::meta::erase_detail::erase_before<std::tuple<ResultArgs..., Type>, index-1u, Args...>
          { };

          template <typename Result, typename Type, typename... Args>
          struct erase_before<Result, 0u, Type, Args...>
            : ::pastel::utility::tuple::meta::erase_detail::erase_after<Result, Args...>
          { };
        } // namespace erase_detail


        template <typename Tuple, std::size_t index>
        struct erase;

        template <std::size_t index, typename... Args>
        struct erase<std::tuple<Args...>, index>
          : ::pastel::utility::tuple::meta::erase_detail::erase_before<std::tuple<>, index, Args...>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_ERASE_HPP

