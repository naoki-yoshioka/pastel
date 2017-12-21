#ifndef PASTEL_UTILITY_TUPLE_META_REPLACE_COPY_HPP
# define PASTEL_UTILITY_TUPLE_META_REPLACE_COPY_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        namespace replace_copy_detail
        {
          template <typename Result, typename FromTuple, typename ToTuple, typename OldType, typename NewType>
          struct replace_copy;

          template <typename... ResultArgs, typename FromType, typename... FromArgs, typename ToType, typename... ToArgs, typename OldType, typename NewType>
          struct replace_copy<std::tuple<ResultArgs...>, std::tuple<FromType, FromArgs...>, std::tuple<ToType, ToArgs...>, OldType, NewType>
            : ::pastel::utility::tuple::meta::replace_copy_detail::replace_copy<std::tuple<ResultArgs..., FromType>, std::tuple<FromArgs...>, std::tuple<ToArgs...>, OldType, NewType>
          { };

          template <typename... ResultArgs, typename... FromArgs, typename ToType, typename... ToArgs, typename OldType, typename NewType>
          struct replace_copy<std::tuple<ResultArgs...>, std::tuple<OldType, FromArgs...>, std::tuple<ToType, ToArgs...>, OldType, NewType>
            : ::pastel::utility::tuple::meta::replace_copy_detail::replace_copy<std::tuple<ResultArgs.., NewType>, std::tuple<FromArgs...>, std::tuple<ToArgs...>, OldType, NewType>
          { };

          template <typename... ResultArgs, typename ToType, typename... ToArgs, typename OldType, typename NewType>
          struct replace_copy<std::tuple<ResultArgs...>, std::tuple<>, std::tuple<ToType, ToArgs...>, OldType, NewType>
            : ::pastel::utility::tuple::meta::replace_copy_detail::replace_copy<std::tuple<ResultArgs..., ToType>, std::tuple<>, std::tuple<ToArgs...>, OldType, NewType>
          { };

          template <typename... ResultArgs, typename FromType, typename... FromArgs, typename OldType, typename NewType>
          struct replace_copy<std::tuple<ResultArgs...>, std::tuple<FromType, FromArgs...>, std::tuple<>, OldType, NewType>
            : ::pastel::utility::tuple::meta::replace_copy_detail::replace_copy<std::tuple<ResultArgs..., FromType>, std::tuple<FromArgs...>, std::tuple<>, OldType, NewType>
          { };

          template <typename... ResultArgs, typename... FromArgs, typename OldType, typename NewType>
          struct replace_copy<std::tuple<ResultArgs...>, std::tuple<Type, FromArgs...>, std::tuple<>, OldType, NewType>
            : ::pastel::utility::tuple::meta::replace_copy_detail::replace_copy<std::tuple<ResultArgs..., NewType>, std::tuple<FromArgs...>, std::tuple<>, OldType, NewType>
          { };

          template <typename Result, typename OldType, typename NewType>
          struct replace_copy<Result, std::tuple<>, std::tuple<>, OldType, NewType>
          { using type = Result; };
        } // namespace replace_copy_detail


        template <typename FromTuple, typename ToTuple, typename OldType, typename NewType>
        struct replace_copy;

        template <typename... FromArgs, typename... ToArgs, typename OldType, typename NewType>
        struct replace_copy<std::tuple<FromArgs...>, std::tuple<ToArgs...>, OldType, NewType>
          : ::pastel::utility::tuple::meta::replace_copy_detail::replace_copy<std::tuple<>, std::tuple<FromArgs...>, std::tuple<ToArgs...>, OldType, NewType>
        { };
      } // namespace meta
    } // namespace utility
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_REPLACE_COPY_HPP

