#ifndef PASTEL_UTILITY_FOR_HPP
# define PASTEL_UTILITY_FOR_HPP

# include <utility>


namespace pastel
{
  namespace utility
  {
    namespace for_detail
    {
      template <typename Index, Index index, int num, template <Index> typename Function>
      struct unrolled_for
      {
        template <typename... Args>
        static void call(Args&&... args)
        {
          Function<index>::call(args...);
          Function<index+1>::call(args...);
          Function<index+2>::call(args...);
          Function<index+3>::call(args...);
          ::pastel::utility::for_detail::unrolled_for<Index, index+4, num-4, Function>::call(std::forward<Args>(args)...);
        }
      }; // struct unrolled_for<Index, index, num, Function>

      template <typename Index, Index index, template <Index> typename Function>
      struct unrolled_for<Index, index, 3, Function>
      {
        template <typename... Args>
        static void call(Args&&... args)
        {
          Function<index>::call(args...);
          Function<index+1>::call(args...);
          Function<index+2>::call(std::forward<Args>(args)...);
        }
      }; // struct unrolled_for<Index, index, 3, Function>

      template <typename Index, Index index, template <Index> typename Function>
      struct unrolled_for<Index, index, 2, Function>
      {
        template <typename... Args>
        static void call(Args&&... args)
        {
          Function<index>::call(args...);
          Function<index+1>::call(std::forward<Args>(args)...);
        }
      }; // struct unrolled_for<Index, index, 2, Function>

      template <typename Index, Index index, template <Index> typename Function>
      struct unrolled_for<Index, index, 1, Function>
      {
        template <typename... Args>
        static void call(Args&&... args)
        { Function<index>::call(std::forward<Args>(args)...); }
      }; // struct unrolled_for<Index, index, 1, Function>

      template <typename Index, Index index, template <Index> typename Function>
      struct unrolled_for<Index, index, 0, Function>
      {
        template <typename... Args>
        static void call(Args&&... args)
        { }
      }; // struct unrolled_for<Index, index, 0, Function>
    } // namespace for_detail

    template <typename Index, Index index, Index last, template <Index> typename Function>
    struct for_
    {
      static_assert(last >= index, "last >= index");

      template <typename... Args>
      static void call(Args&&... args)
      { ::pastel::utility::for_detail::unrolled_for<Index, index, last-index, Function>::call(std::forward<Args>(args)...); }
    }; // struct for_<Index, index, last, Function>
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_FOR_HPP

