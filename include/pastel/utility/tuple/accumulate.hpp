#ifndef PASTEL_UTILITY_TUPLE_ACCUMULATE_HPP
# define PASTEL_UTILITY_TUPLE_ACCUMULATE_HPP

# include <tuple>
# include <utility>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace accumulate_detail
      {
        template <std::size_t index, std::size_t num>
        struct accumulate
        {
          template <typename... Args, typename Value>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value)
          {
            return ::pastel::utility::tuple::accumulate_detail::accumulate<index+4u, num-4u>::call(
              tuple, (((value + std::get<index>(tuple)) + std::get<index+1u>(tuple)) + std::get<index+2u>(tuple)) + std::get<index+3u>(tuple))
          }

          template <typename... Args, typename Value, typename Function>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value, Function&& function)
          {
            return ::pastel::utility::tuple::accumulate_detail::accumulate<index+4u, num-4u>::call(
              tuple,
              function(
                function(
                  function(
                    function(value, std::get<index>(tuple)),
                    std::get<index+1u>(tuple)),
                  std::get<index+2u>(tuple)),
                std::get<index+3u>(tuple)),
              std::forward<Function>(function));
          }
        }; // struct accumulate<index, num>

        template <std::size_t index>
        struct accumulate<index, 3u>
        {
          template <typename... Args, typename Value>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value)
          { return ((value + std::get<index>(tuple)) + std::get<index+1u>(tuple)) + std::get<index+2u>(tuple); }

          template <typename... Args, typename Value, typename Function>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value, Function&& function)
          {
            return function(
              function(
                function(value, std::get<index>(tuple)),
                std::get<index+1u>(tuple)),
              std::get<index+2u>(tuple));
          }
        }; // struct accumulate<index, 3u>

        template <std::size_t index>
        struct accumulate<index, 2u>
        {
          template <typename... Args, typename Value>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value)
          { return (value + std::get<index>(tuple)) + std::get<index+1u>(tuple); }

          template <typename... Args, typename Value, typename Function>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value, Function&& function)
          {
            return function(
              function(value, std::get<index>(tuple)),
              std::get<index+1u>(tuple));
          }
        }; // struct accumulate<index, 2u>

        template <std::size_t index>
        struct accumulate<index, 1u>
        {
          template <typename... Args, typename Value>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value)
          { return value + std::get<index>(tuple); }

          template <typename... Args, typename Value, typename Function>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value, Function&& function)
          { return function(value, std::get<index>(tuple)); }
        }; // struct accumulate<index, 1u>

        template <std::size_t index>
        struct accumulate<index, 0u>
        {
          template <typename... Args, typename Value>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value)
          { return value; }

          template <typename... Args, typename Value, typename Function>
          static constexpr Value call(std::tuple<Args...> const&, Value value, Function&&)
          { return value; }
        }; // struct accumulate<index, 0u>
      } // namespace accumulate_detail


      template <typename... Args, typename Value>
      inline constexpr Value accumulate(std::tuple<Args...> const& tuple, Value init)
      { return ::pastel::utility::tuple::accumulate_detail::accumulate<0u, sizeof...(Args)>::call(tuple, init); }

      template <typename... Args, typename Value, typename Function>
      inline constexpr Value accumulate(std::tuple<Args...> const& tuple, Value init, Function&& function)
      { return ::pastel::utility::tuple::accumulate_detail::accumulate<0u, sizeof...(Args)>::call(tuple, init, std::forward<Function>(function)); }
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_ACCUMULATE_HPP

