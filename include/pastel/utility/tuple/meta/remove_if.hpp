#ifndef PASTEL_UTILITY_TUPLE_META_REMOVE_IF_HPP
# define PASTEL_UTILITY_TUPLE_META_REMOVE_IF_HPP

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
        namespace remove_if_detail
        {
          template <typename Result, template <typename> typename Predicate, typename... Args>
          struct remove_if;

          template <typename... ResultArgs, template <typename> typename Predicate, typename Type, typename... Args>
          struct remove_if<std::tuple<ResultArgs...>, Predicate, Type, Args...>
            : std::conditional<
                Predicate<Type>::value,
                typename ::pastel::utility::tuple::meta::remove_if_detail::remove_if<std::tuple<ResultArgs...>, Predicate, Args...>::type,
                typename ::pastel::utility::tuple::meta::remove_if_detail::remove_if<std::tuple<ResultArgs..., Type>, Predicate, Args...>::type>::type
          { };

          template <typename Result, template <typename> typename Predicate>
          struct remove_if<Result, Predicate>
          { using type = Result; };
        } // namespace remove_if_detail


        template <typename Tuple, template <typename> typename Predicate>
        struct remove_if;

        template <typename... Args, template <typename> typename Predicate>
        struct remove_if<std::tuple<Args...>, Predicate>
          : ::pastel::utility::tuple::meta::remove_if_detail::remove_if<std::tuple<>, Predicate, Args...>
        { };
      } // namespace meta
    } // namespace utility
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_REMOVE_IF_HPP

