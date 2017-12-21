#ifndef PASTEL_INTEGRATE_DETAIL_UPDATE_POSITIONS_ORDER1_HPP
# define PASTEL_INTEGRATE_DETAIL_UPDATE_POSITIONS_ORDER1_HPP

# include <pastel/container/num_particles.hpp>
# include <pastel/container/get.hpp>
# include <pastel/container/is_mobile.hpp>
# include <pastel/container/mobility_tags.hpp>
# include <pastel/container/meta/is_data_accessible.hpp>
# include <pastel/container/meta/mobility_tag_of.hpp>
# include <pastel/particle/tags.hpp>


namespace pastel
{
  namespace integrate
  {
    namespace detail
    {
      namespace update_positions_order1_detail
      {
        template <bool is_data_accessible>
        struct update_positions;

        template <>
        struct update_positions<true>
        {
          template <typename Particles, typename Time>
          static void call(Particles&&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles&& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              positions_data[index] += velocities_data[index] * time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles&& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                positions_data[index] += velocities_data[index] * time_step;
          }
        }; // struct update_positions<true>

        template <>
        struct update_positions<false>
        {
          template <typename Particles, typename Time>
          static void call(Particles&&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles&& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              ::pastel::container::get< ::pastel::particle::tags::position >(particles, index)
                += ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) * time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles&& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                ::pastel::container::get< ::pastel::particle::tags::position >(particles, index)
                  += ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) * time_step;
          }
        }; // struct update_positions<false>
      } // namespace update_positions_order1_detail


      template <typename Particles, typename Time>
      inline void update_positions_order1(Particles& particles, Time time_step)
      {
        static constexpr bool is_data_accessible
          = ::pastel::container::meta::is_data_accessible<Particles>::value;
        using update_positions_func
          = ::pastel::integrate::detail::update_positions_order1_detail::update_positions<is_data_accessible>;
        using mobility_tag
          = typename ::pastel::container::meta::mobility_tag_of<Particles>::type;
        update_positions_func::call(particles, time_step, mobility_tag{});
      }
    } // namespace detail
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_DETAIL_UPDATE_POSITIONS_ORDER1_HPP

