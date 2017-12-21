#ifndef PASTEL_FORCE_META_VALUE_OF_HPP
# define PASTEL_FORCE_META_VALUE_OF_HPP


namespace pastel
{
  namespace force
  {
    namespace meta
    {
      template <typename Force>
      struct value_of
      { using type = typename Force::value_type; };
    } // namespace meta
  } // namespace force
} // namespace pastel


#endif // PASTEL_FORCE_META_VALUE_OF_HPP

