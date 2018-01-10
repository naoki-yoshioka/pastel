#ifndef PASTEL_UTILITY_FOR_HPP
# define PASTEL_UTILITY_FOR_HPP

# include <utility>


namespace pastel
{
  namespace utility
  {
    template <typename Index, Index index, Index last, template <Index> typename Function>
    struct for_
    {
      template <typename... Args>
      static void call(Args&&... args)
      {
        Function<index>::call(args...);
        Function<index+1>::call(args...);
        Function<index+2>::call(args...);
        Function<index+3>::call(args...);
        for_<Index, index+4, last, Function>::call(std::forward<Args>(args)...);
      }
    }; // struct for_<Index, index, last, Function>

    template <typename Index, Index last, template <Index> typename Function>
    struct for_<Index, last-3, last, Function>
    {
      template <typename... Args>
      static void call(Args&&... args)
      {
        Function<last-3>::call(args...);
        Function<last-2>::call(args...);
        Function<last-1>::call(args...);
      }
    }; // struct for_<Index, last-3, last, Function>

    template <typename Index, Index last, template <Index> typename Function>
    struct for_<Index, last-2, last, Function>
    {
      template <typename... Args>
      static void call(Args&&... args)
      {
        Function<last-2>::call(args...);
        Function<last-1>::call(args...);
      }
    }; // struct for_<Index, last-2, last, Function>

    template <typename Index, Index last, template <Index> typename Function>
    struct for_<Index, last-1, last, Function>
    {
      template <typename... Args>
      static void call(Args&&... args)
      {
        Function<last-1>::call(args...);
      }
    }; // struct for_<Index, last-1, last, Function>

    template <typename Index, Index last, template <Index> typename Function>
    struct for_<Index, last, last, Function>
    {
      template <typename... Args>
      static void call(Args&&...) { }
    }; // struct for_<Index, last, last, Function>
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_FOR_HPP

