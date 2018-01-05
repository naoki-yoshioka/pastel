#ifndef PASTEL_UTILITY_IS_CONST_POINTER_HPP
# define PASTEL_UTILITY_IS_CONST_POINTER_HPP

# include <type_traits>


namespace pastel
{
  namespace utility
  {
    template <typename Pointer>
    struct is_const_pointer;

    template <typename T>
    struct is_const_pointer<T*>
      : std::false_type
    { };

    template <typename T>
    struct is_const_pointer<T const*>
      : std::true_type
    { };
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_IS_CONST_POINTER_HPP

