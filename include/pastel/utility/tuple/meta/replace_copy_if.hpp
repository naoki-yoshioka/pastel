#ifndef PASTEL_UTILITY_TUPLE_META_REPLACE_COPY_IF_HPP
# define PASTEL_UTILITY_TUPLE_META_REPLACE_COPY_IF_HPP

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
        namespace replace_copy_if_detail
        {
          template <typename Result, typename FromTuple, typename ToTuple, template <typename> typename Predicate, typename NewType>
          struct replace_copy_if;

          template <typename... ResultArgs, typename FromType, typename... FromArgs, typename ToType, typename... ToArgs, template <typename> typename Predicate, typename NewType>
          struct replace_copy_if<std::tuple<ResultArgs...>, std::tuple<FromType, FromArgs...>, std::tuple<ToType, ToArgs...>, Predicate, NewType>
            : std::conditional<
                Predicate<FromType>::value,
                typename ::pastel::utility::tuple::meta::replace_copy_if_detail::replace_copy_if<
                  std::tuple<ResultArgs..., NewType>, std::tuple<FromArgs...>, std::tuple<ToArgs...>, Predicate, NewType>::type,
                typename ::pastel::utility::tuple::meta::replace_copy_if_detail::replace_copy_if<
                  std::tuple<ResultArgs..., FromType>, std::tuple<FromArgs...>, std::tuple<ToArgs...>, Predicate, NewType>::type>::type
          { };

          template <typename... ResultArgs, typename ToType, typename... ToArgs, template <typename> typename Predicate, typename NewType>
          struct replace_copy_if<std::tuple<ResultArgs...>, std::tuple<>, std::tuple<ToType, ToArgs...>, Predicate, NewType>
            : ::pastel::utility::tuple::meta::replace_copy_if_detail::replace_copy_if<std::tuple<ResultArgs..., ToType>, std::tuple<>, std::tuple<ToArgs...>, Predicate, NewType>
          { };

          template <typename... ResultArgs, typename FromType, typename... FromArgs, template <typename> typename Predicate, typename NewType>
          struct replace_copy_if<std::tuple<ResultArgs...>, std::tuple<FromType, FromArgs...>, std::tuple<>, Predicate, NewType>
            : std::conditional<
                Predicate<FromType>::value,
                typename ::pastel::utility::tuple::meta::replace_copy_if_detail::replace_copy_if<
                  std::tuple<ResultArgs..., NewType>, std::tuple<FromArgs...>, std::tuple<>, Predicate, NewType>::type,
                typename ::pastel::utility::tuple::meta::replace_copy_if_detail::replace_copy_if<
                  std::tuple<ResultArgs..., FromType>, std::tuple<FromArgs...>, std::tuple<>, Predicate, NewType>::type>::type
          { };

          template <typename Result, template <typename> typename Predicate, typename NewType>
          struct replace_copy_if<Result, std::tuple<>, std::tuple<>, Predicate, NewType>
          { using type = Result; };
        } // namespace replace_copy_if_detail


        template <typename FromTuple, typename ToTuple, template <typename> typename Predicate, typename NewType>
        struct replace_copy_if;

        template <typename... FromArgs, typename... ToArgs, template <typename > typename Predicate, typename NewType>
        struct replace_copy_if<std::tuple<FromArgs...>, std::tuple<ToArgs...>, Predicate, NewType>
          : ::pastel::utility::tuple::meta::replace_copy_if_detail::replace_copy_if<std::tuple<>, std::tuple<FromArgs...>, std::tuple<ToArgs...>, Predicate, NewType>
        { };
      } // namespace meta
    } // namespace utility
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_REPLACE_COPY_IF_HPP

