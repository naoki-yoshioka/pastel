#ifndef PASTEL_UTILITY_TUPLE_FOR_EACH_HPP
# define PASTEL_UTILITY_TUPLE_FOR_EACH_HPP

# include <tuple>
# include <utility>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace for_each_detail
      {
        template <std::size_t index, std::size_t num>
        struct for_each
        {
          template <typename Function, typename... Args>
          static constexpr void call(std::tuple<Args...>& tuple, Function&& function)
          {
            function(std::get<index>(tuple));
            function(std::get<index+1u>(tuple));
            function(std::get<index+2u>(tuple));
            function(std::get<index+3u>(tuple));
            ::pastel::utility::tuple::for_each_detail::for_each<index+4u, num-4u>::call(tuple, std::forward<Function>(function));
          }

          template <typename Function, typename... Args>
          static constexpr void call(std::tuple<Args...> const& tuple, Function&& function)
          {
            function(std::get<index>(tuple));
            function(std::get<index+1u>(tuple));
            function(std::get<index+2u>(tuple));
            function(std::get<index+3u>(tuple));
            ::pastel::utility::tuple::for_each_detail::for_each<index+4u, num-4u>::call(tuple, std::forward<Function>(function));
          }

          template <typename Function, typename... Args>
          static constexpr void call(std::tuple<Args...> const& tuple, Function&& function)
          {
            function(std::move(std::get<index>(tuple)));
            function(std::move(std::get<index+1u>(tuple)));
            function(std::move(std::get<index+2u>(tuple)));
            function(std::move(std::get<index+3u>(tuple)));
            ::pastel::utility::tuple::for_each_detail::for_each<index+4u, num-4u>::call(tuple, std::forward<Function>(function));
          }
        }; // struct for_each<index, num>

        template <std::size_t index>
        struct for_each<index, 3u>
        {
          template <typename Function, typename... Args>
          static constexpr void call(std::tuple<Args...>& tuple, Function&& function)
          {
            function(std::get<index>(tuple));
            function(std::get<index+1u>(tuple));
            function(std::get<index+2u>(tuple));
          }

          template <typename Function, typename... Args>
          static constexpr void call(std::tuple<Args...> const& tuple, Function&& function)
          {
            function(std::get<index>(tuple));
            function(std::get<index+1u>(tuple));
            function(std::get<index+2u>(tuple));
          }

          template <typename Function, typename... Args>
          static constexpr void call(std::tuple<Args...> const& tuple, Function&& function)
          {
            function(std::move(std::get<index>(tuple)));
            function(std::move(std::get<index+1u>(tuple)));
            function(std::move(std::get<index+2u>(tuple)));
          }
        }; // struct for_each<index, 3u>

        template <std::size_t index>
        struct for_each<index, 2u>
        {
          template <typename Function, typename... Args>
          static constexpr void call(std::tuple<Args...>& tuple, Function&& function)
          {
            function(std::get<index>(tuple));
            function(std::get<index+1u>(tuple));
          }

          template <typename Function, typename... Args>
          static constexpr void call(std::tuple<Args...> const& tuple, Function&& function)
          {
            function(std::get<index>(tuple));
            function(std::get<index+1u>(tuple));
          }

          template <typename Function, typename... Args>
          static constexpr void call(std::tuple<Args...> const& tuple, Function&& function)
          {
            function(std::move(std::get<index>(tuple)));
            function(std::move(std::get<index+1u>(tuple)));
          }
        }; // struct for_each<index, 2u>

        template <std::size_t index>
        struct for_each<index, 1u>
        {
          template <typename Function, typename... Args>
          static constexpr void call(std::tuple<Args...>& tuple, Function&& function)
          { function(std::get<index>(tuple)); }

          template <typename Function, typename... Args>
          static constexpr void call(std::tuple<Args...> const& tuple, Function&& function)
          { function(std::get<index>(tuple)); }

          template <typename Function, typename... Args>
          static constexpr void call(std::tuple<Args...> const& tuple, Function&& function)
          { function(std::move(std::get<index>(tuple))); }
        }; // struct for_each<index, 1u>

        template <std::size_t index>
        struct for_each<index, 0u>
        {
          template <typename Function, typename... Args>
          static constexpr void call(std::tuple<Args...>&, Function&&) { }

          template <typename Function, typename... Args>
          static constexpr void call(std::tuple<Args...> const&, Function&&) { }

          template <typename Function, typename... Args>
          static constexpr void call(std::tuple<Args...> const&, Function&&) { }
        }; // struct for_each<index, 0u>
      } // namespace for_each_detail


      template <typename Function, typename... Args>
      inline constexpr void for_each(std::tuple<Args...>& tuple, Function&& function)
      { ::pastel::utility::tuple::for_each_detail::for_each<0u, sizeof...(Args)>::call(tuple, std::forward<Function>(function)); }

      template <typename Function, typename... Args>
      inline constexpr void for_each(std::tuple<Args...> const& tuple, Function&& function)
      { ::pastel::utility::tuple::for_each_detail::for_each<0u, sizeof...(Args)>::call(tuple, std::forward<Function>(function)); }

      template <typename Function, typename... Args>
      inline constexpr void for_each(std::tuple<Args...>&& tuple, Function&& function)
      { ::pastel::utility::tuple::for_each_detail::for_each<0u, sizeof...(Args)>::call(std::move(tuple), std::forward<Function>(function)); }
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_FOR_EACH_HPP

