#ifndef PASTEL_UTILITY_TUPLE_META_FILL_N_HPP
# define PASTEL_UTILITY_TUPLE_META_FILL_N_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        namespace fill_n_detail
        {
          template <typename Result, std::size_t n, typename Type>
          struct fill_n;

          template <typename... ResultArgs, std::size_t n, typename Type>
          struct fill_n<std::tuple<ResultArgs...>, n, Type>
            : ::pastel::utility::tuple::meta::fill_n_detail::fill_n<std::tuple<ResultArgs..., Type>, n-1u, Type>
          { };

          template <typename... ResultArgs, typename Type>
          struct fill_n<std::tuple<ResultArgs...>, 0u, Type>
          { using type = std::tuple<ResultArgs...>; };
        } // namespace fill_n_detail


        template <std::size_t n, typename Type>
        struct fill_n
          : ::pastel::utility::tuple::meta::fill_n_detail::fill_n<std::tuple<>, n, Type>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_FILL_N_HPP

