#ifndef PASTEL_SYSTEM_META_BOUNDARY_OF_HPP
# define PASTEL_SYSTEM_META_BOUNDARY_OF_HPP


namespace pastel
{
  namespace system
  {
    namespace meta
    {
      template <typename System>
      struct boundary_of
      { using type = typename System::boundary_type; };
    } // namespace meta
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_META_BOUNDARY_OF_HPP

