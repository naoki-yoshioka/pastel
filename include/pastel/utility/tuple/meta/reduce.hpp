#ifndef PASTEL_UTILITY_TUPLE_META_REDUCE_HPP
# define PASTEL_UTILITY_TUPLE_META_REDUCE_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        namespace reduce_detail
        {
          template <std::size_t first, std::size_t last, typename Tuple, template <typename, typename> typename Function>
          struct reduce
            : Function<
                typename ::pastel::utility::tuple::meta::reduce_detail::reduce<first, first + (last - first) / 2u, Tuple, Function>::type,
                typename ::pastel::utility::tuple::meta::reduce_detail::reduce<first + (last - first) / 2u, last, Tuple, Function>::type>
          { };

          template <std::size_t first, typename Tuple, template <typename, typename> typename Function>
          struct reduce<first, first+1u, Tuple, Function>
            : std::tuple_element<Tuple, first>
          { };

          template <std::size_t first, typename Tuple, template <typename, typename> typename Function>
          struct reduce<first, first, Tuple, Function>
          { using type = void; };
        } // namespace reduce_detail


        template <typename Tuple, template <typename, typename> typename Function>
        struct reduce;

        template <typename... Args, template <typename, typename> typename Function>
        struct reduce<std::tuple<Args...>, Function>
          : ::pastel::utility::tuple::meta::reduce_detail::reduce<0u, sizeof...(Args), std::tuple<Args...>, Function>
        { };


        template <typename Tuple, typename Init, template <typename, typename> typename Function>
        struct reduce_init;

        template <typename... Args, typename Init, template <typename, typename> typename Function>
        struct reduce_init<std::tuple<Args...>, Init, Function>
          : ::pastel::utility::tuple::meta::reduce<std::tuple<Init, Args...>, Function>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_REDUCE_HPP

