#ifndef PASTEL_INTEGRATE_DETAIL_UPDATE_VELOCITIES_ORDER1_HPP
# define PASTEL_INTEGRATE_DETAIL_UPDATE_VELOCITIES_ORDER1_HPP

# include <pastel/container/num_particles.hpp>
# include <pastel/container/get.hpp>
# include <pastel/container/is_mobile.hpp>
# include <pastel/container/mobility_tags.hpp>
# include <pastel/container/meta/has_mass.hpp>
# include <pastel/container/meta/is_data_accessible.hpp>
# include <pastel/container/meta/mobility_tag_of.hpp>
# include <pastel/particle/tags.hpp>


namespace pastel
{
  namespace integrate
  {
    namespace detail
    {
      namespace update_velocities_order1_detail
      {
        template <bool is_data_accessible, bool has_mass>
        struct update_velocities;

        template <>
        struct update_velocities<true, true>
        {
          template <typename Particles, typename Time>
          static void call(Particles&&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles&& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              velocities_data[index] += (forces_data[index] / masses_data[index]) * time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles&& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                velocities_data[index] += (forces_data[index] / masses_data[index]) * time_step;
          }
        }; // struct update_velocities<true, true>

        template <>
        struct update_velocities<true, false>
        {
          template <typename Particles, typename Time>
          static void call(Particles&&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles&& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              velocities_data[index] += forces_data[index] * time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles&& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                velocities_data[index] += forces_data[index] * time_step;
          }
        }; // struct update_velocities<true, false>

        template <>
        struct update_velocities<false, true>
        {
          template <typename Particles, typename Time>
          static void call(Particles&&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles&& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index)
                += (::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                    / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index))
                   * time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles&& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index)
                  += (::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                      / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index))
                     * time_step;
          }
        }; // struct update_velocities<false, true>

        template <>
        struct update_velocities<false, false>
        {
          template <typename Particles, typename Time>
          static void call(Particles&&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles&& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index)
                += ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                   * time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles&& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index)
                  += ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                     * time_step;
          }
        }; // struct update_velocities<false, false>
      } // namespace update_velocities_order1_detail


      template <typename Particles, typename Time>
      inline void update_velocities_order1(Particles& particles, Time time_step)
      {
        static constexpr bool is_data_accessible
          = ::pastel::container::meta::is_data_accessible<Particles>::value;
        static constexpr bool has_mass
          = ::pastel::container::meta::has_mass<Particles>::value;
        using update_velocities_func
          = ::pastel::integrate::detail::update_velocities_order1_detail::update_velocities<is_data_accessible, has_mass>;
        using mobility_tag
          = typename ::pastel::container::meta::mobility_tag_of<Particles>::type;
        update_velocities_func::call(particles, time_step, mobility_tag{});
      }
    } // namespace detail
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_DETAIL_UPDATE_VELOCITIES_ORDER1_HPP
