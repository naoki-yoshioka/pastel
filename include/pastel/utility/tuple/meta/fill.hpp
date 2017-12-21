#ifndef PASTEL_UTILITY_TUPLE_META_FILL_HPP
# define PASTEL_UTILITY_TUPLE_META_FILL_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        namespace fill_detail
        {
          template <typename Result, typename Type, std::size_t n>
          struct fill;

          template <typename... Args, typename Type, std::size_t n>
          struct fill<std::tuple<Args...>, Type, n>
            : ::pastel::utility::tuple::meta::fill_detail::fill<std::tuple<Args..., Type>, Type, n-1u>
          { };

          template <typename Result, typename Type>
          struct fill<Result, Type, 0u>
          { using type = Result; };
        } // namespace fill_detail


        template <typename Tuple, typename Type>
        struct fill;

        template <typename... Args, typename Type>
        struct fill<std::tuple<Args...>, Type>
          : ::pastel::utility::tuple::meta::fill_detail::fill<std::tuple<>, Type, sizeof...(Args)>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_FILL_HPP

