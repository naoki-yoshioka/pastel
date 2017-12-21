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
        template <std::size_t index, std::size_t last>
        struct accumulate
        {
          template <typename... Args, typename Value>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value)
          {
            return ::pastel::utility::tuple::accumulate_detail::accumulate<index+4u, last>::call(
              tuple, (((value + std::get<index>(tuple)) + std::get<index+1u>(tuple)) + std::get<index+2u>(tuple)) + std::get<index+3u>(tuple))
          }

          template <typename... Args, typename Value, typename Function>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value, Function&& function)
          {
            return ::pastel::utility::tuple::accumulate_detail::accumulate<index+4u, last>::call(
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
        }; // struct accumulate<index, last>

        template <std::size_t last>
        struct accumulate<last-3u, last>
        {
          template <typename... Args, typename Value>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value)
          { return ((value + std::get<last-3u>(tuple)) + std::get<last-2u>(tuple)) + std::get<last-1u>(tuple); }

          template <typename... Args, typename Value, typename Function>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value, Function&& function)
          {
            return function(
              function(
                function(value, std::get<last-3u>(tuple)),
                std::get<last-2u>(tuple)),
              std::get<last-1u>(tuple));
          }
        }; // struct accumulate<last-3u, last>

        template <std::size_t last>
        struct accumulate<last-2u, last>
        {
          template <typename... Args, typename Value>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value)
          { return (value + std::get<last-2u>(tuple)) + std::get<last-1u>(tuple); }

          template <typename... Args, typename Value, typename Function>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value, Function&& function)
          {
            return function(
              function(value, std::get<last-2u>(tuple)),
              std::get<last-1u>(tuple));
          }
        }; // struct accumulate<last-2u, last>

        template <std::size_t last>
        struct accumulate<last-1u, last>
        {
          template <typename... Args, typename Value>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value)
          { return value + std::get<last-1u>(tuple); }

          template <typename... Args, typename Value, typename Function>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value, Function&& function)
          { return function(value, std::get<last-1u>(tuple)); }
        }; // struct accumulate<last-1u, last>

        template <std::size_t last>
        struct accumulate<last, last>
        {
          template <typename... Args, typename Value>
          static constexpr Value call(std::tuple<Args...> const& tuple, Value value)
          { return value; }

          template <typename... Args, typename Value, typename Function>
          static constexpr Value call(std::tuple<Args...> const&, Value value, Function&&)
          { return value; }
        }; // struct accumulate<last, last>
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

