#ifndef PASTEL_UTILITY_TUPLE_PARTIAL_SUM_HPP
# define PASTEL_UTILITY_TUPLE_PARTIAL_SUM_HPP

# include <tuple>
# include <utility>

# include <pastel/utility/tuple/push_back.hpp>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace partial_sum_detail
      {
        template <std::size_t index>
        struct partial_sum
        {
          using next_partial_sum_type = ::pastel::utility::tuple::partial_sum_detail::partial_sum<index-4u>;

          template <typename... Args>
          static constexpr auto call(std::tuple<Args...> const& tuple)
            -> decltype(::pastel::utility::tuple::push_back(
                 next_partial_sum_type::call(tuple),
                 std::get<index-4u>(next_partial_sum_type::call(tuple)) + std::get<index-3u>(tuple),
                 std::get<index-4u>(next_partial_sum_type::call(tuple)) + std::get<index-3u>(tuple) + std::get<index-2u>(tuple),
                 std::get<index-4u>(next_partial_sum_type::call(tuple)) + std::get<index-3u>(tuple) + std::get<index-2u>(tuple) + std::get<index-1u>(tuple),
                 std::get<index-4u>(next_partial_sum_type::call(tuple)) + std::get<index-3u>(tuple) + std::get<index-2u>(tuple) + std::get<index-1u>(tuple) + std::get<index>(tuple)))
          {
            return ::pastel::utility::tuple::push_back(
              next_partial_sum_type::call(tuple),
              std::get<index-4u>(next_partial_sum_type::call(tuple)) + std::get<index-3u>(tuple),
              std::get<index-4u>(next_partial_sum_type::call(tuple)) + std::get<index-3u>(tuple) + std::get<index-2u>(tuple),
              std::get<index-4u>(next_partial_sum_type::call(tuple)) + std::get<index-3u>(tuple) + std::get<index-2u>(tuple) + std::get<index-1u>(tuple),
              std::get<index-4u>(next_partial_sum_type::call(tuple)) + std::get<index-3u>(tuple) + std::get<index-2u>(tuple) + std::get<index-1u>(tuple) + std::get<index>(tuple));
          }

          template <typename... Args, typename Function>
          static constexpr auto call(std::tuple<Args...> const& tuple, Function const& function)
            -> decltype(::pastel::utility::tuple::push_back(
                 next_partial_sum_type::call(tuple, function),
                 function(
                   std::get<index-4u>(next_partial_sum_type::call(tuple, function)),
                   std::get<index-3u>(tuple)),
                 function(
                   function(
                     std::get<index-4u>(next_partial_sum_type::call(tuple, function)),
                     std::get<index-3u>(tuple)),
                   std::get<index-2u>(tuple)),
                 function(
                   function(
                     function(
                       std::get<index-4u>(next_partial_sum_type::call(tuple, function)),
                       std::get<index-3u>(tuple)),
                     std::get<index-2u>(tuple)),
                   std::get<index-1u>(tuple)),
                 function(
                   function(
                     function(
                       function(
                         std::get<index-4u>(next_partial_sum_type::call(tuple, function)),
                         std::get<index-3u>(tuple)),
                       std::get<index-2u>(tuple)),
                     std::get<index-1u>(tuple)),
                   std::get<index>(tuple))))
          {
            return ::pastel::utility::tuple::push_back(
              next_partial_sum_type::call(tuple, function),
              function(
                std::get<index-4u>(next_partial_sum_type::call(tuple, function)),
                std::get<index-3u>(tuple)),
              function(
                function(
                  std::get<index-4u>(next_partial_sum_type::call(tuple, function)),
                  std::get<index-3u>(tuple)),
                std::get<index-2u>(tuple)),
              function(
                function(
                  function(
                    std::get<index-4u>(next_partial_sum_type::call(tuple, function)),
                    std::get<index-3u>(tuple)),
                  std::get<index-2u>(tuple)),
                std::get<index-1u>(tuple)),
              function(
                function(
                  function(
                    function(
                      std::get<index-4u>(next_partial_sum_type::call(tuple, function)),
                      std::get<index-3u>(tuple)),
                    std::get<index-2u>(tuple)),
                  std::get<index-1u>(tuple)),
                std::get<index>(tuple)));
          }

          template <typename... Args, template <std::size_t> typename Function>
          static constexpr auto call(std::tuple<Args...> const& tuple, Function const& function)
            -> decltype(::pastel::utility::tuple::push_back(
                 next_partial_sum_type::call(tuple, function),
                 function<index-3u>(
                   std::get<index-4u>(next_partial_sum_type::call(tuple, function)),
                   std::get<index-3u>(tuple)),
                 function<index-2u>(
                   function<index-3u>(
                     std::get<index-4u>(next_partial_sum_type::call(tuple, function)),
                     std::get<index-3u>(tuple)),
                   std::get<index-2u>(tuple)),
                 function<index-1u>(
                   function<index-2u>(
                     function<index-3u>(
                       std::get<index-4u>(next_partial_sum_type::call(tuple, function)),
                       std::get<index-3u>(tuple)),
                     std::get<index-2u>(tuple)),
                   std::get<index-1u>(tuple)),
                 function<index>(
                   function<index-1u>(
                     function<index-2u>(
                       function<index-3u>(
                         std::get<index-4u>(next_partial_sum_type::call(tuple, function)),
                         std::get<index-3u>(tuple)),
                       std::get<index-2u>(tuple)),
                     std::get<index-1u>(tuple)),
                   std::get<index>(tuple))))
          {
            return ::pastel::utility::tuple::push_back(
              next_partial_sum_type::call(tuple, function),
              function<index-3u>(
                std::get<index-4u>(next_partial_sum_type::call(tuple, function)),
                std::get<index-3u>(tuple)),
              function<index-2u>(
                function<index-3u>(
                  std::get<index-4u>(next_partial_sum_type::call(tuple, function)),
                  std::get<index-3u>(tuple)),
                std::get<index-2u>(tuple)),
              function<index-1u>(
                function<index-2u>(
                  function<index-3u>(
                    std::get<index-4u>(next_partial_sum_type::call(tuple, function)),
                    std::get<index-3u>(tuple)),
                  std::get<index-2u>(tuple)),
                std::get<index-1u>(tuple)),
              function<index>(
                function<index-1u>(
                  function<index-2u>(
                    function<index-3u>(
                      std::get<index-4u>(next_partial_sum_type::call(tuple, function)),
                      std::get<index-3u>(tuple)),
                    std::get<index-2u>(tuple)),
                  std::get<index-1u>(tuple)),
                std::get<index>(tuple)));
          }
        }; // struct partial_sum<index>

        template <>
        struct partial_sum<3u>
        {
          template <typename Type0, typename Type1, typename Type2, typename Type3, typename... Args>
          static constexpr std::tuple<
            Type0,
            decltype(std::declval<Type0>() + std::declval<Type1>()),
            decltype(std::declval<Type0>() + std::declval<Type1>() + std::declval<Type2>()),
            decltype(std::declval<Type0>() + std::declval<Type1>() + std::declval<Type2>() + std::declval<Type3>())>
          call(std::tuple<Type0, Type1, Type2, Type3, Args...> const& tuple)
          {
            return std::make_tuple(
              std::get<0u>(tuple),
              std::get<0u>(tuple) + std::get<1u>(tuple),
              std::get<0u>(tuple) + std::get<1u>(tuple) + std::get<2u>(tuple),
              std::get<0u>(tuple) + std::get<1u>(tuple) + std::get<2u>(tuple) + std::get<3u>(tuple));
          }

          template <typename... Args, typename Function>
          static constexpr auto call(std::tuple<Args...> const& tuple, Function&& function)
            -> decltype(std::make_tuple(
                 std::get<0u>(tuple),
                 function(std::get<0u>(tuple), std::get<1u>(tuple)),
                 function(function(std::get<0u>(tuple), std::get<1u>(tuple)), std::get<2u>(tuple)),
                 function(function(function(std::get<0u>(tuple), std::get<1u>(tuple)), std::get<2u>(tuple)), std::get<3u>(tuple))))
          {
            return std::make_tuple(
              std::get<0u>(tuple),
              function(std::get<0u>(tuple), std::get<1u>(tuple)),
              function(function(std::get<0u>(tuple), std::get<1u>(tuple)), std::get<2u>(tuple)),
              function(function(function(std::get<0u>(tuple), std::get<1u>(tuple)), std::get<2u>(tuple)), std::get<3u>(tuple)));
          }

          template <typename... Args, template <std::size_t> typename Function>
          static constexpr auto call(std::tuple<Args...> const& tuple, Function&& function)
            -> decltype(std::make_tuple(
                 std::get<0u>(tuple),
                 function<1u>(std::get<0u>(tuple), std::get<1u>(tuple)),
                 function<2u>(function<1u>(std::get<0u>(tuple), std::get<1u>(tuple)), std::get<2u>(tuple)),
                 function<3u>(function<2u>(function<1u>(std::get<0u>(tuple), std::get<1u>(tuple)), std::get<2u>(tuple)), std::get<3u>(tuple))))
          {
            return std::make_tuple(
              std::get<0u>(tuple),
              function<1u>(std::get<0u>(tuple), std::get<1u>(tuple)),
              function<2u>(function<1u>(std::get<0u>(tuple), std::get<1u>(tuple)), std::get<2u>(tuple)),
              function<3u>(function<2u>(function<1u>(std::get<0u>(tuple), std::get<1u>(tuple)), std::get<2u>(tuple)), std::get<3u>(tuple)));
          }
        }; // struct partial_sum<3u>

        template <>
        struct partial_sum<2u>
        {
          template <typename Type0, typename Type1, typename Type2, typename... Args>
          static constexpr std::tuple<
            Type0,
            decltype(std::declval<Type0>() + std::declval<Type1>()),
            decltype(std::declval<Type0>() + std::declval<Type1>() + std::declval<Type2>())>
          call(std::tuple<Type0, Type1, Type2, Args...> const& tuple)
          {
            return std::make_tuple(
              std::get<0u>(tuple),
              std::get<0u>(tuple) + std::get<1u>(tuple),
              std::get<0u>(tuple) + std::get<1u>(tuple) + std::get<2u>(tuple));
          }

          template <typename... Args, typename Function>
          static constexpr auto call(std::tuple<Args...> const& tuple, Function&& function)
            -> decltype(std::make_tuple(
                 std::get<0u>(tuple),
                 function(std::get<0u>(tuple), std::get<1u>(tuple)),
                 function(function(std::get<0u>(tuple), std::get<1u>(tuple)), std::get<2u>(tuple))))
          {
            return std::make_tuple(
              std::get<0u>(tuple),
              function(std::get<0u>(tuple), std::get<1u>(tuple)),
              function(function(std::get<0u>(tuple), std::get<1u>(tuple)), std::get<2u>(tuple)));
          }

          template <typename... Args, template <std::size_t> typename Function>
          static constexpr auto call(std::tuple<Args...> const& tuple, Function&& function)
            -> decltype(std::make_tuple(
                 std::get<0u>(tuple),
                 function<1u>(std::get<0u>(tuple), std::get<1u>(tuple)),
                 function<2u>(function<1u>(std::get<0u>(tuple), std::get<1u>(tuple)), std::get<2u>(tuple))))
          {
            return std::make_tuple(
              std::get<0u>(tuple),
              function<1u>(std::get<0u>(tuple), std::get<1u>(tuple)),
              function<2u>(function<1u>(std::get<0u>(tuple), std::get<1u>(tuple)), std::get<2u>(tuple)));
          }
        }; // struct partial_sum<2u>

        template <>
        struct partial_sum<1u>
        {
          template <typename Type0, typename Type1, typename... Args>
          static constexpr std::tuple<Type0, decltype(std::declval<Type0>() + std::declval<Type1>())> call(std::tuple<Type0, Type1, Args...> const& tuple)
          {
            return std::make_tuple(
              std::get<0u>(tuple),
              std::get<0u>(tuple) + std::get<1u>(tuple));
          }

          template <typename... Args, typename Function>
          static constexpr auto call(std::tuple<Args...> const& tuple, Function&& function)
            -> decltype(std::make_tuple(
                 std::get<0u>(tuple),
                 function(std::get<0u>(tuple), std::get<1u>(tuple))))
          {
            return std::make_tuple(
              std::get<0u>(tuple),
              function(std::get<0u>(tuple), std::get<1u>(tuple)));
          }

          template <typename... Args, template <std::size_t> typename Function>
          static constexpr auto call(std::tuple<Args...> const& tuple, Function&& function)
            -> decltype(std::make_tuple(
                 std::get<0u>(tuple),
                 function<1u>(std::get<0u>(tuple), std::get<1u>(tuple))))
          {
            return std::make_tuple(
              std::get<0u>(tuple),
              function<1u>(std::get<0u>(tuple), std::get<1u>(tuple)));
          }
        }; // struct partial_sum<1u>

        template <>
        struct partial_sum<0u>
        {
          template <typename Type, typename... Args>
          static constexpr std::tuple<Type> call(std::tuple<Type, Args...> const& tuple)
          { return std::make_tuple(std::get<0u>(tuple)); }

          template <typename Type, typename... Args, typename Function>
          static constexpr std::tuple<Type> call(std::tuple<Type, Args...> const& tuple, Function&&)
          { return std::make_tuple(std::get<0u>(tuple)); }

          template <typename Type, typename... Args, template <std::size_t> typename Function>
          static constexpr std::tuple<Type> call(std::tuple<Type, Args...> const& tuple, Function&&)
          { return std::make_tuple(std::get<0u>(tuple)); }
        }; // struct partial_sum<0u>
      } // namespace partial_sum_detail


      template <typename... Args>
      inline constexpr auto partial_sum(std::tuple<Args...> const& tuple)
      { return ::pastel::utility::tuple::partial_sum_detail::partial_sum<sizeof...(Args)-1u>::call(tuple); }

      template <typename... Args, typename Function>
      inline constexpr auto partial_sum(std::tuple<Args...> const& tuple, Function&& function)
      { return ::pastel::utility::tuple::partial_sum_detail::partial_sum<sizeof...(Args)-1u>::call(tuple, std::forward<Function>(function)); }

      template <typename... Args, template <std::size_t> typename Function>
      inline constexpr auto partial_sum(std::tuple<Args...> const& tuple, Function&& function)
      { return ::pastel::utility::tuple::partial_sum_detail::partial_sum<sizeof...(Args)-1u>::call(tuple, std::forward<Function>(function)); }
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_PARTIAL_SUM_HPP

