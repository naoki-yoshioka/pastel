#ifndef PASTEL_UTILITY_IS_NOTHROW_SWAPPABLE_HPP
# define PASTEL_UTILITY_IS_NOTHROW_SWAPPABLE_HPP

# include <utility>
# include <type_traits>


namespace pastel
{
  namespace utility
  {
    namespace is_nothrow_swappable_detail
    {
      using std::swap;
      template <typename T>
      struct test
      {
        static constexpr bool value
          = noexcept(swap(std::declval<T&>(), std::declval<T&>()));
      }; // struct test<T>
    } // namespace is_nothrow_swappable_detail

    template <typename T>
    using is_nothrow_swappable
      = std::integral_constant<bool, ::pastel::utility::is_nothrow_swappable_detail::test<T>::value>;
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_IS_NOTHROW_SWAPPABLE_HPP

