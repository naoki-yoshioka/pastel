#ifndef PASTEL_SYSTEM_META_DIMENSION_OF_HPP
# define PASTEL_SYSTEM_META_DIMENSION_OF_HPP

# include <cstddef>
# include <type_traits>


namespace pastel
{
  namespace system
  {
    namespace meta
    {
      template <typename System>
      struct dimension_of
        : std::integral_constant<std::size_t, System::dimension>
      { };
    } // namespace meta
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_META_DIMENSION_OF_HPP

