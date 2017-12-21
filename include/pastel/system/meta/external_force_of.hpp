#ifndef PASTEL_SYSTEM_META_EXTERNAL_FORCE_OF_HPP
# define PASTEL_SYSTEM_META_EXTERNAL_FORCE_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace system
  {
    namespace meta
    {
      template <std::size_t index, typename System>
      struct external_force_of
      { using type = typename System::template external_force_type<index>; };

      template <std::size_t index, typename System>
      struct external_force_of<index, System const>
      { using type = typename System::template external_force_type<index> const; };
    } // namespace meta
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_META_EXTERNAL_FORCE_OF_HPP

