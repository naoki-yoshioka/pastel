#ifndef PASTEL_FORCE_META_REQUIRED_ARGUMENTS_CATEGORY_OF_HPP
# define PASTEL_FORCE_META_REQUIRED_ARGUMENTS_CATEGORY_OF_HPP


namespace pastel
{
  namespace force
  {
    namespace meta
    {
      template <typename Force>
      struct required_arguments_category_of
      { using type = typename Force::required_arguments_category; };
    } // namespace meta
  } // namespace force
} // namespace pastel


#endif // PASTEL_FORCE_META_REQUIRED_ARGUMENTS_CATEGORY_OF_HPP

