#ifndef PASTEL_CONTAINER_TAGS_HPP
# define PASTEL_CONTAINER_TAGS_HPP


namespace pastel
{
  namespace container
  {
    namespace tags
    {
      template <int n>
      struct nth_integration_vector { };
      template <int n>
      struct nth_property_vector { };
      template <int n>
      struct nth_property_scalar { };
    } // namespace tags
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_TAGS_HPP

