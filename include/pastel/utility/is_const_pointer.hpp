#ifndef PASTEL_UTILITY_IS_CONST_POINTER_HPP
# define PASTEL_UTILITY_IS_CONST_POINTER_HPP

# include <type_traits>
# include <memory>


namespace pastel
{
  namespace utility
  {
    template <typename Pointer>
    struct is_const_pointer
      : std::is_same<
          Pointer,
          typename std::pointer_traits<Pointer>::template rebind<
            typename std::remove_const<
              typename std::pointer_traits<Pointer>::element_type>::type const>>
    { };
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_IS_CONST_POINTER_HPP

