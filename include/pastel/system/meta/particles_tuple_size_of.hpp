#ifndef PASTEL_SYSTEM_META_PARTICLES_TUPLE_SIZE_OF_HPP
# define PASTEL_SYSTEM_META_PARTICLES_TUPLE_SIZE_OF_HPP

# include <cstddef>
# include <type_traits>


namespace pastel
{
  namespace system
  {
    namespace meta
    {
      template <typename System>
      struct particles_tuple_size_of
        : std::integral_constant<std::size_t, System::particles_tuple_size>
      { };
    } // namespace meta
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_META_PARTICLES_TUPLE_SIZE_OF_HPP

