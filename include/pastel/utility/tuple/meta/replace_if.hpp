#ifndef PASTEL_UTILITY_TUPLE_META_REPLACE_IF_HPP
# define PASTEL_UTILITY_TUPLE_META_REPLACE_IF_HPP

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
        namespace replace_if_detail
        {
          template <typename Result, template <typename> typename Predicate, typename NewType, typename... Args>
          struct replace_if;

          template <typename... ResultArgs, template <typename> typename Predicate, typename NewType, typename Type, typename... Args>
          struct replace_if<std::tuple<ResultArgs...>, Predicate, NewType, Type, Args...>
            : std::conditional<
                Predicate<Type>::value,
                typename ::pastel::utility::tuple::meta::replace_if_detail::replace_if<
                  std::tuple<ResultArgs..., NewType>, Predicate, NewType, Args...>::type,
                typename ::pastel::utility::tuple::meta::replace_if_detail::replace_if<
                  std::tuple<ResultArgs..., Type>, Predicate, NewType, Args...>::type>::type
          { };

          template <typename Result, template <typename> typename Predicate, typename NewType>
          struct replace_if<Result, Predicate, NewType>
          { using type = Result; };
        } // namespace replace_if_detail


        template <typename Tuple, template <typename> typename Predicate, typename NewType>
        struct replace_if;

        template <typename... Args, template <typename> typename Predicate, typename NewType>
        struct replace_if<std::tuple<Args...>, Predicate, NewType>
          : ::pastel::utility::tuple::meta::replace_if_detail::replace_if<std::tuple<>, Predicate, NewType, Args...>
        { };
      } // namespace meta
    } // namespace utility
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_REPLACE_IF_HPP

