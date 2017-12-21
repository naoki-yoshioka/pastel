#ifndef PASTEL_UTILITY_TUPLE_META_REMOVE_HPP
# define PASTEL_UTILITY_TUPLE_META_REMOVE_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        namespace remove_detail
        {
          template <typename Result, typename Type, typename... Args>
          struct remove;

          template <typename... ResultArgs, typename Type, typename T, typename... Args>
          struct remove<std::tuple<ResultArgs...>, Type, T, Args...>
            : ::pastel::utility::tuple::meta::remove_detail::remove<std::tuple<ResultArgs..., T>, Type, Args...>
          { };

          template <typename... ResultArgs, typename Type, typename... Args>
          struct remove<std::tuple<ResultArgs...>, Type, Type, Args...>
            : ::pastel::utility::tuple::meta::remove_detail::remove<std::tuple<ResultArgs...>, Type, Args...>
          { };

          template <typename Result, typename Type>
          struct remove<Result, Type>
          { using type = Result; };
        } // namespace remove_detail


        template <typename Tuple, typename Type>
        struct remove;

        template <typename... Args, typename Type>
        struct remove<std::tuple<Args...>, Type>
          : ::pastel::utility::tuple::meta::remove_detail::remove<std::tuple<>, Type, Args...>
        { };
      } // namespace meta
    } // namespace utility
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_REMOVE_HPP

