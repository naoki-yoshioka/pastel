#ifndef PASTEL_UTILITY_TUPLE_META_IS_TUPLE_HPP
# define PASTEL_UTILITY_TUPLE_META_IS_TUPLE_HPP

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
        template <typename T>
        struct is_tuple
          : std::false_type
        { };

        template <typename... Args>
        struct is_tuple<std::tuple<Args...>>
          : std::true_type
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_IS_TUPLE_HPP

