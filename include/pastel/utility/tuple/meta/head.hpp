#ifndef PASTEL_UTILITY_TUPLE_META_HEAD_HPP
# define PASTEL_UTILITY_TUPLE_META_HEAD_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      template <typename Tuple>
      struct head
        : std::tuple_element<Tuple, 0u>
      { };
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_HEAD_HPP

