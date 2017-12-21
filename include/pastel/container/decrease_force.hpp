#ifndef PASTEL_CONTAINER_DECREASE_FORCE_HPP
# define PASTEL_CONTAINER_DECREASE_FORCE_HPP

# include <type_traits>

# include <pastel/container/meta/mobility_tag_of.hpp>
# include <pastel/container/meta/vector_of.hpp>
# include <pastel/container/meta/size_of.hpp>
# include <pastel/container/is_mobile.hpp>
# include <pastel/container/get.hpp>
# include <pastel/particle/tags.hpp>


namespace pastel
{
  namespace container
  {
    namespace decrease_force_detail
    {
      template < typename Particles>
      inline void decrease_force(
        Particles& particles,
        typename ::pastel::container::meta::size_of<Particles>::type index,
        typename ::pastel::container::meta::vector_of<Particles>::type const& force_decrement,
        ::pastel::container::mobility_tags::mobile)
      { ::pastel::container::get< ::pastel::particle::tags::force >(particles, index) -= force_decrement; }

      template <typename Particles>
      inline void decrease_force(
        Particles&,
        typename ::pastel::container::meta::size_of<Particles>::type,
        typename ::pastel::container::meta::vector_of<Particles>::type const&,
        ::pastel::container::mobility_tags::immobile)
      { }

      template <typename Particles>
      inline void decrease_force(
        Particles& particles,
        typename ::pastel::container::meta::size_of<Particles>::type index,
        typename ::pastel::container::meta::vector_of<Particles>::type const& force_decrement,
        ::pastel::container::mobility_tags::partially_mobile)
      {
        if (::pastel::container::is_mobile(particles, index))
          ::pastel::container::get< ::pastel::particle::tags::force >(particles, index) -= force_decrement;
      }
    } // namespace decrease_force_detail


    template < typename Particles>
    inline void decrease_force(
      Particles& particles,
      typename ::pastel::container::meta::size_of<Particles>::type index,
      typename ::pastel::container::meta::vector_of<Particles>::type const& force_decrement)
    {
      ::pastel::container::decrease_force_detail::decrease_force(
        particles, index, force_decrement,
        typename ::pastel::container::meta::mobility_tag_of<Particles>::type{});
    }
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_DECREASE_FORCE_HPP

