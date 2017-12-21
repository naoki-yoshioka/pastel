#ifndef PASTEL_CONTAINER_META_MOBILITY_TAG_OF_HPP
# define PASTEL_CONTAINER_META_MOBILITY_TAG_OF_HPP


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct mobility_tag_of
      { using type = typename Particles::mobility_tag; };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_MOBILITY_TAG_OF_HPP

