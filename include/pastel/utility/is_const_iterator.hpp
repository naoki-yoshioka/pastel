#ifndef PASTEL_UTILITY_IS_CONST_ITERATOR_HPP
# define PASTEL_UTILITY_IS_CONST_ITERATOR_HPP

# include <iterator>

# include <pastel/utility/is_const_pointer.hpp>


namespace pastel
{
  namespace utility
  {
    template <typename Iterator>
    struct is_const_iterator
      : ::pastel::utility::is_const_pointer<typename std::iterator_traits<Iterator>::pointer>
    { };
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_IS_CONST_ITERATOR_HPP

