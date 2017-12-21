#ifndef PASTEL_UTILITY_IS_INPUT_ITERATOR_HPP
# define PASTEL_UTILITY_IS_INPUT_ITERATOR_HPP

# include <iterator>
# include <type_traits>


namespace pastel
{
  namespace utility
  {
    template <typename Iterator>
    using is_input_iterator = std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::input_iterator_tag>;
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_IS_INPUT_ITERATOR_HPP

