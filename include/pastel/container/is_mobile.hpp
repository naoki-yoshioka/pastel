#ifndef PASTEL_CONTAINER_IS_MOBILE_HPP
# define PASTEL_CONTAINER_IS_MOBILE_HPP

# include <pastel/container/mobility_tags.hpp>
# include <pastel/container/meta/mobility_tag_of.hpp>


namespace pastel
{
  namespace container
  {
    namespace is_mobile_detail
    {
      template <typename Particles>
      inline bool call(Particles const&, typename Particles::size_type, ::pastel::container::mobility_tags::mobile)
      { return true; }

      template <typename Particles>
      inline bool call(Particles const&, typename Particles::size_type, ::pastel::container::mobility_tags::immobile)
      { return false; }

      /*
      template <typename Particles>
      inline bool call(Particles const& particles, typename Particles::size_type index, ::pastel::container::mobility_tags::partially_mobile)
      ;
      */
    } // namespace is_mobile_detail

    template <typename Particles>
    inline bool is_mobile(Particles const& particles, typename Particles::size_type index)
    { return ::pastel::container::is_mobile_detail::call(particles, index, typename ::pastel::container::meta::mobility_tag_of<Particles>::type()); }
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_IS_MOBILE_HPP

