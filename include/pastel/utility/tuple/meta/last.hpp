#ifndef PASTEL_UTILITY_TUPLE_META_LAST_HPP
# define PASTEL_UTILITY_TUPLE_META_LAST_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      template <typename Tuple>
      struct last
        : std::tuple_element<Tuple, std::tuple_size<Tuple>::value-1u>
      { };
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_LAST_HPP

