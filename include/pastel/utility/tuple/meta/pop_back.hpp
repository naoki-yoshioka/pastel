#ifndef PASTEL_UTILITY_TUPLE_META_POP_BACK_HPP
# define PASTEL_UTILITY_TUPLE_META_POP_BACK_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        namespace pop_back_detail
        {
          template <typename Result, typename... Args>
          struct pop_back;

          template <typename... ResultArgs, typename Type, typename... Args>
          struct pop_back<std::tuple<ResultArgs...>, Type, Args...>
            : ::pastel::utility::tuple::meta::pop_back_detail::pop_back<std::tuple<ResultArgs..., Type>, Args...>
          { };

          template <typename... ResultArgs, typename Type>
          struct pop_back<std::tuple<ResultArgs...>, Type>
          { using type = std::tuple<ResultArgs...>; };
        } // namespace pop_back_detail


        template <typename Tuple>
        struct pop_back;

        template <typename Type, typename... Args>
        struct pop_back<std::tuple<Type, Args...>>
          : ::pastel::utility::tuple::meta::pop_back_detail::pop_back<std::tuple<Type>, Args...>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_POP_BACK_HPP

