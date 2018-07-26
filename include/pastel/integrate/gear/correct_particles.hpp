#ifndef PASTEL_INTEGRATE_GEAR_CORRECT_PARTICLES_HPP
# define PASTEL_INTEGRATE_GEAR_CORRECT_PARTICLES_HPP

# include <cstddef>
# include <tuple>

# include <pastel/particle/tags.hpp>
# include <pastel/container/tags.hpp>
# include <pastel/container/get.hpp>
# include <pastel/system/for_each_container.hpp>


namespace pastel
{
  namespace integrate
  {
    namespace gear
    {
      namespace correct_particles_detail
      {
        template <std::size_t order, bool is_data_accessible, bool has_mass, std::size_t acceleration_index, typename IntegrationVectorIndexTuple>
        struct correct_particles_impl;

        /*
          Gear 4: [r_n = (1/n!) (d^n r/dt^n) dt^n]
           r^p_0(t+dt) = r_0(t) + r_1(t) +   r_2(t) +   r_3(t)
           r^p_1(t+dt) =          r_1(t) + 2 r_2(t) + 3 r_3(t)
           r^p_2(t+dt) =                     r_2(t) + 3 r_3(t)
           r^p_3(t+dt) =                                r_3(t)
             a^c(t+dt) = f({r^p(t+dt)}) / m
           r^c_2(t+dt) = a^c(t+dt) dt^2 / 2
                  dr_2 = r^c_2(t+dt) - r^p_2(t+dt)
           r^c_0(t+dt) = r^p_0(t+dt) + (1/6) dr_2
           r^c_1(t+dt) = r^p_1(t+dt) + (5/6) dr_2
           r^c_3(t+dt) = r^p_3(t+dt) + (1/3) dr_2
        */

        // REQUIRED: x, v, f, a, b (q, \omega, \tau, \alpha, \beta for DEM)
        //      da_i = f_i(t+dt) / m_i - a^p_i(t+dt)
        // x_i(t+dt) = x^p_i(t+dt) + (dt^2/12) da_i
        // v_i(t+dt) = v^p_i(t+dt) +  (5dt/12) da_i
        // b_i(t+dt) = b^p_i(t+dt) +    (1/dt) da_i
        template <std::size_t acceleration_index, typename IntegrationVectorIndexTuple>
        struct correct_particles_impl<4u, true, true, acceleration_index, IntegrationVectorIndexTuple>
        {
          static constexpr auto b_index = std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value;

          template <typename Particles, typename Time>
          static void call(Particles const& particles, Time time_step, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const position_coefficient = time_step * time_step / Time{12};
            auto const velocity_coefficient = Time{5} * time_step / Time{12};
            auto const b_coefficient = Time{1} / time_step;

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const a_data = particles.template data< ::pastel::container::tags::nth_integration_vector<acceleration_index> >();
            auto const b_data = particles.template data< ::pastel::container::tags::nth_integration_vector<b_index> >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              auto const correction = forces_data[index] / masses_data[index] - a_data[index];
              positions_data[index] += position_coefficient * correction;
              velocities_data[index] += velocity_coefficient * correction;
              b_data[index] += b_coefficient * correction;
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const position_coefficient = time_step * time_step / Time{12};
            auto const velocity_coefficient = Time{5} * time_step / Time{12};
            auto const b_coefficient = Time{1} / time_step;

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const a_data = particles.template data< ::pastel::container::tags::nth_integration_vector<acceleration_index> >();
            auto const b_data = particles.template data< ::pastel::container::tags::nth_integration_vector<b_index> >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                auto const correction = forces_data[index] / masses_data[index] - a_data[index];
                positions_data[index] += position_coefficient * correction;
                velocities_data[index] += velocity_coefficient * correction;
                b_data[index] += b_coefficient * correction;
              }
          }
        }; // struct correct_particles_impl<4u, true, true, acceleration_index, IntegrationVectorIndexTuple>

        template <std::size_t acceleration_index, typename IntegrationVectorIndexTuple>
        struct correct_particles_impl<4u, false, true, acceleration_index, IntegrationVectorIndexTuple>
        {
          static constexpr auto b_index = std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value;

          template <typename Particles, typename Time>
          static void call(Particles const& particles, Time time_step, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const position_coefficient = time_step * time_step / Time{12};
            auto const velocity_coefficient = Time{5} * time_step / Time{12};
            auto const b_coefficient = Time{1} / time_step;

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              auto const correction
                = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                    / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index)
                  - ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<acceleration_index> >(particles, index);
              ::pastel::container::get< ::pastel::particle::tags::position >(particles, index) += position_coefficient * correction;
              ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) += velocity_coefficient * correction;
              ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<b_index> >(particles, index) += b_coefficient * correction;
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const position_coefficient = time_step * time_step / Time{12};
            auto const velocity_coefficient = Time{5} * time_step / Time{12};
            auto const b_coefficient = Time{1} / time_step;

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                auto const correction
                  = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                      / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index)
                    - ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<acceleration_index> >(particles, index);
                ::pastel::container::get< ::pastel::particle::tags::position >(particles, index) += position_coefficient * correction;
                ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) += velocity_coefficient * correction;
                ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<b_index> >(particles, index) += b_coefficient * correction;
              }
          }
        }; // struct correct_particles_impl<4u, false, true, acceleration_index, IntegrationVectorIndexTuple>

        template <std::size_t acceleration_index, typename IntegrationVectorIndexTuple>
        struct correct_particles_impl<4u, true, false, acceleration_index, IntegrationVectorIndexTuple>
        {
          static constexpr auto b_index = std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value;

          template <typename Particles, typename Time>
          static void call(Particles const& particles, Time time_step, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const position_coefficient = time_step * time_step / Time{12};
            auto const velocity_coefficient = Time{5} * time_step / Time{12};
            auto const b_coefficient = Time{1} / time_step;

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const a_data = particles.template data< ::pastel::container::tags::nth_integration_vector<acceleration_index> >();
            auto const b_data = particles.template data< ::pastel::container::tags::nth_integration_vector<b_index> >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              auto const correction = forces_data[index] - a_data[index];
              positions_data[index] += position_coefficient * correction;
              velocities_data[index] += velocity_coefficient * correction;
              b_data[index] += b_coefficient * correction;
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const position_coefficient = time_step * time_step / Time{12};
            auto const velocity_coefficient = Time{5} * time_step / Time{12};
            auto const b_coefficient = Time{1} / time_step;

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const a_data = particles.template data< ::pastel::container::tags::nth_integration_vector<acceleration_index> >();
            auto const b_data = particles.template data< ::pastel::container::tags::nth_integration_vector<b_index> >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                auto const correction = forces_data[index] - a_data[index];
                positions_data[index] += position_coefficient * correction;
                velocities_data[index] += velocity_coefficient * correction;
                b_data[index] += b_coefficient * correction;
              }
          }
        }; // struct correct_particles_impl<4u, true, false, acceleration_index, IntegrationVectorIndexTuple>

        template <std::size_t acceleration_index, typename IntegrationVectorIndexTuple>
        struct correct_particles_impl<4u, false, false, acceleration_index, IntegrationVectorIndexTuple>
        {
          static constexpr auto b_index = std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value;

          template <typename Particles, typename Time>
          static void call(Particles const& particles, Time time_step, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const position_coefficient = time_step * time_step / Time{12};
            auto const velocity_coefficient = Time{5} * time_step / Time{12};
            auto const b_coefficient = Time{1} / time_step;

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              auto const correction
                = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                  - ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<acceleration_index> >(particles, index);
              ::pastel::container::get< ::pastel::particle::tags::position >(particles, index) += position_coefficient * correction;
              ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) += velocity_coefficient * correction;
              ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<b_index> >(particles, index) += b_coefficient * correction;
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const position_coefficient = time_step * time_step / Time{12};
            auto const velocity_coefficient = Time{5} * time_step / Time{12};
            auto const b_coefficient = Time{1} / time_step;

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                auto const correction
                  = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                    - ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<acceleration_index> >(particles, index);
                ::pastel::container::get< ::pastel::particle::tags::position >(particles, index) += position_coefficient * correction;
                ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) += velocity_coefficient * correction;
                ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<b_index> >(particles, index) += b_coefficient * correction;
              }
          }
        }; // struct correct_particles_impl<4u, false, false, acceleration_index, IntegrationVectorIndexTuple>


        /*
          Gear 5: [r_n = (1/n!) (d^n r/dt^n) dt^n]
           r^p_0(t+dt) = r_0(t) + r_1(t) +   r_2(t) +   r_3(t) +   r_4(t)
           r^p_1(t+dt) =          r_1(t) + 2 r_2(t) + 3 r_3(t) + 4 r_4(t)
           r^p_2(t+dt) =                     r_2(t) + 3 r_3(t) + 6 r_4(t)
           r^p_3(t+dt) =                                r_3(t) + 4 r_4(t)
           r^p_4(t+dt) =                                           r_4(t)
             a^c(t+dt) = f({r^p(t+dt)}) / m
           r^c_2(t+dt) = a^c(t+dt) dt^2 / 2
                  dr_2 = r^c_2(t+dt) - r^p_2(t+dt)
           r^c_0(t+dt) = r^p_0(t+dt) + (19/90) dr_2
           r^c_1(t+dt) = r^p_1(t+dt) +   (3/4) dr_2
           r^c_3(t+dt) = r^p_3(t+dt) +   (1/2) dr_2
           r^c_4(t+dt) = r^p_4(t+dt) +  (1/12) dr_2
        */

        // REQUIRED: x, v, f, a, b, c (q, \omega, \tau, \alpha, \beta, \gamma for DEM)
        //      da_i = f_i(t+dt) / m_i - a^p_i(t+dt)
        // x_i(t+dt) = x^p_i(t+dt) + (19dt^2/180) da_i
        // v_i(t+dt) = v^p_i(t+dt) +      (3dt/8) da_i
        // b_i(t+dt) = b^p_i(t+dt) +      (3/2dt) da_i
        // c_i(t+dt) = c^p_i(t+dt) +     (1/dt^2) da_i
        template <std::size_t acceleration_index, typename IntegrationVectorIndexTuple>
        struct correct_particles_impl<5u, true, true, acceleration_index, IntegrationVectorIndexTuple>
        {
          static constexpr auto b_index = std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value;
          static constexpr auto c_index = std::tuple_element<1u, IntegrationVectorIndexTuple>::type::value;

          template <typename Particles, typename Time>
          static void call(Particles const& particles, Time time_step, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const inverse_time_step = Time{1} / time_step;

            auto const position_coefficient = Time{19} * time_step * time_step / Time{180};
            auto const velocity_coefficient = Time{3} * time_step / Time{8};
            auto const b_coefficient = Time{3} * inverse_time_step / Time{2};
            auto const c_coefficient = inverse_time_step * inverse_time_step;

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const a_data = particles.template data< ::pastel::container::tags::nth_integration_vector<acceleration_index> >();
            auto const b_data = particles.template data< ::pastel::container::tags::nth_integration_vector<b_index> >();
            auto const c_data = particles.template data< ::pastel::container::tags::nth_integration_vector<c_index> >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              auto const correction = forces_data[index] / masses_data[index] - a_data[index];
              positions_data[index] += position_coefficient * correction;
              velocities_data[index] += velocity_coefficient * correction;
              b_data[index] += b_coefficient * correction;
              c_data[index] += c_coefficient * correction;
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const inverse_time_step = Time{1} / time_step;

            auto const position_coefficient = Time{19} * time_step * time_step / Time{180};
            auto const velocity_coefficient = Time{3} * time_step / Time{8};
            auto const b_coefficient = Time{3} * inverse_time_step / Time{2};
            auto const c_coefficient = inverse_time_step * inverse_time_step;

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const a_data = particles.template data< ::pastel::container::tags::nth_integration_vector<acceleration_index> >();
            auto const b_data = particles.template data< ::pastel::container::tags::nth_integration_vector<b_index> >();
            auto const c_data = particles.template data< ::pastel::container::tags::nth_integration_vector<c_index> >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                auto const correction = forces_data[index] / masses_data[index] - a_data[index];
                positions_data[index] += position_coefficient * correction;
                velocities_data[index] += velocity_coefficient * correction;
                b_data[index] += b_coefficient * correction;
                c_data[index] += c_coefficient * correction;
              }
          }
        }; // struct correct_particles_impl<5u, true, true, acceleration_index, IntegrationVectorIndexTuple>

        template <std::size_t acceleration_index, typename IntegrationVectorIndexTuple>
        struct correct_particles_impl<5u, false, true, acceleration_index, IntegrationVectorIndexTuple>
        {
          static constexpr auto b_index = std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value;
          static constexpr auto c_index = std::tuple_element<1u, IntegrationVectorIndexTuple>::type::value;

          template <typename Particles, typename Time>
          static void call(Particles const& particles, Time time_step, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const inverse_time_step = Time{1} / time_step;

            auto const position_coefficient = Time{19} * time_step * time_step / Time{180};
            auto const velocity_coefficient = Time{3} * time_step / Time{8};
            auto const b_coefficient = Time{3} * inverse_time_step / Time{2};
            auto const c_coefficient = inverse_time_step * inverse_time_step;

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              auto const correction
                = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                    / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index)
                  - ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<acceleration_index> >(particles, index);
              ::pastel::container::get< ::pastel::particle::tags::position >(particles, index) += position_coefficient * correction;
              ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) += velocity_coefficient * correction;
              ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<b_index> >(particles, index) += b_coefficient * correction;
              ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<c_index> >(particles, index) += c_coefficient * correction;
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const inverse_time_step = Time{1} / time_step;

            auto const position_coefficient = Time{19} * time_step * time_step / Time{180};
            auto const velocity_coefficient = Time{3} * time_step / Time{8};
            auto const b_coefficient = Time{3} * inverse_time_step / Time{2};
            auto const c_coefficient = inverse_time_step * inverse_time_step;

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                auto const correction
                  = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                      / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index)
                    - ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<acceleration_index> >(particles, index);
                ::pastel::container::get< ::pastel::particle::tags::position >(particles, index) += position_coefficient * correction;
                ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) += velocity_coefficient * correction;
                ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<b_index> >(particles, index) += b_coefficient * correction;
                ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<c_index> >(particles, index) += c_coefficient * correction;
              }
          }
        }; // struct correct_particles_impl<5u, false, true, acceleration_index, IntegrationVectorIndexTuple>

        template <std::size_t acceleration_index, typename IntegrationVectorIndexTuple>
        struct correct_particles_impl<5u, true, false, acceleration_index, IntegrationVectorIndexTuple>
        {
          static constexpr auto b_index = std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value;
          static constexpr auto c_index = std::tuple_element<1u, IntegrationVectorIndexTuple>::type::value;

          template <typename Particles, typename Time>
          static void call(Particles const& particles, Time time_step, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const inverse_time_step = Time{1} / time_step;

            auto const position_coefficient = Time{19} * time_step * time_step / Time{180};
            auto const velocity_coefficient = Time{3} * time_step / Time{8};
            auto const b_coefficient = Time{3} * inverse_time_step / Time{2};
            auto const c_coefficient = inverse_time_step * inverse_time_step;

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const a_data = particles.template data< ::pastel::container::tags::nth_integration_vector<acceleration_index> >();
            auto const b_data = particles.template data< ::pastel::container::tags::nth_integration_vector<b_index> >();
            auto const c_data = particles.template data< ::pastel::container::tags::nth_integration_vector<c_index> >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              auto const correction = forces_data[index] - a_data[index];
              positions_data[index] += position_coefficient * correction;
              velocities_data[index] += velocity_coefficient * correction;
              b_data[index] += b_coefficient * correction;
              c_data[index] += c_coefficient * correction;
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const inverse_time_step = Time{1} / time_step;

            auto const position_coefficient = Time{19} * time_step * time_step / Time{180};
            auto const velocity_coefficient = Time{3} * time_step / Time{8};
            auto const b_coefficient = Time{3} * inverse_time_step / Time{2};
            auto const c_coefficient = inverse_time_step * inverse_time_step;

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const a_data = particles.template data< ::pastel::container::tags::nth_integration_vector<acceleration_index> >();
            auto const b_data = particles.template data< ::pastel::container::tags::nth_integration_vector<b_index> >();
            auto const c_data = particles.template data< ::pastel::container::tags::nth_integration_vector<c_index> >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                auto const correction = forces_data[index] - a_data[index];
                positions_data[index] += position_coefficient * correction;
                velocities_data[index] += velocity_coefficient * correction;
                b_data[index] += b_coefficient * correction;
                c_data[index] += c_coefficient * correction;
              }
          }
        }; // struct correct_particles_impl<5u, true, false, acceleration_index, IntegrationVectorIndexTuple>

        template <std::size_t acceleration_index, typename IntegrationVectorIndexTuple>
        struct correct_particles_impl<5u, false, false, acceleration_index, IntegrationVectorIndexTuple>
        {
          static constexpr auto b_index = std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value;
          static constexpr auto c_index = std::tuple_element<1u, IntegrationVectorIndexTuple>::type::value;

          template <typename Particles, typename Time>
          static void call(Particles const& particles, Time time_step, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const inverse_time_step = Time{1} / time_step;

            auto const position_coefficient = Time{19} * time_step * time_step / Time{180};
            auto const velocity_coefficient = Time{3} * time_step / Time{8};
            auto const b_coefficient = Time{3} * inverse_time_step / Time{2};
            auto const c_coefficient = inverse_time_step * inverse_time_step;

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              auto const correction
                = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                  - ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<acceleration_index> >(particles, index);
              ::pastel::container::get< ::pastel::particle::tags::position >(particles, index) += position_coefficient * correction;
              ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) += velocity_coefficient * correction;
              ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<b_index> >(particles, index) += b_coefficient * correction;
              ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<c_index> >(particles, index) += c_coefficient * correction;
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const inverse_time_step = Time{1} / time_step;

            auto const position_coefficient = Time{19} * time_step * time_step / Time{180};
            auto const velocity_coefficient = Time{3} * time_step / Time{8};
            auto const b_coefficient = Time{3} * inverse_time_step / Time{2};
            auto const c_coefficient = inverse_time_step * inverse_time_step;

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                auto const correction
                  = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                    - ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<acceleration_index> >(particles, index);
                ::pastel::container::get< ::pastel::particle::tags::position >(particles, index) += position_coefficient * correction;
                ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) += velocity_coefficient * correction;
                ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<b_index> >(particles, index) += b_coefficient * correction;
                ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<c_index> >(particles, index) += c_coefficient * correction;
              }
          }
        }; // struct correct_particles_impl<5u, false, false, acceleration_index, IntegrationVectorIndexTuple>


        /*
          Gear 6: [r_n = (1/n!) (d^n r/dt^n) dt^n]
           r^p_0(t+dt) = r_0(t) + r_1(t) +   r_2(t) +   r_3(t) +   r_4(t) +    r_5(t)
           r^p_1(t+dt) =          r_1(t) + 2 r_2(t) + 3 r_3(t) + 4 r_4(t) +  5 r_5(t)
           r^p_2(t+dt) =                     r_2(t) + 3 r_3(t) + 6 r_4(t) + 10 r_5(t)
           r^p_3(t+dt) =                                r_3(t) + 4 r_4(t) + 10 r_5(t)
           r^p_4(t+dt) =                                           r_4(t) +  5 r_5(t)
           r^p_5(t+dt) =                                                       r_5(t)
             a^c(t+dt) = f({r^p(t+dt)}) / m
           r^c_2(t+dt) = a^c(t+dt) dt^2 / 2
                  dr_2 = r^c_2(t+dt) - r^p_2(t+dt)
           r^c_0(t+dt) = r^p_0(t+dt) +    (3/16) dr_2
           r^c_1(t+dt) = r^p_1(t+dt) + (251/360) dr_2
           r^c_3(t+dt) = r^p_3(t+dt) +   (11/18) dr_2
           r^c_4(t+dt) = r^p_4(t+dt) +     (1/6) dr_2
           r^c_5(t+dt) = r^p_5(t+dt) +    (1/60) dr_2
        */

        // REQUIRED: x, v, f, a, b, c, d (q, \omega, \tau, \alpha, \beta, \gamma, \delta for DEM)
        //      da_i = f_i(t+dt) / m_i - a^p_i(t+dt)
        // x_i(t+dt) = x^p_i(t+dt) +  (3dt^2/32) da_i
        // v_i(t+dt) = v^p_i(t+dt) + (251dt/720) da_i
        // b_i(t+dt) = b^p_i(t+dt) +    (11/6dt) da_i
        // c_i(t+dt) = c^p_i(t+dt) +    (2/dt^2) da_i
        // d_i(t+dt) = d^p_i(t+dt) +    (1/dt^3) da_i
        template <std::size_t acceleration_index, typename IntegrationVectorIndexTuple>
        struct correct_particles_impl<6u, true, true, acceleration_index, IntegrationVectorIndexTuple>
        {
          static constexpr auto b_index = std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value;
          static constexpr auto c_index = std::tuple_element<1u, IntegrationVectorIndexTuple>::type::value;
          static constexpr auto d_index = std::tuple_element<2u, IntegrationVectorIndexTuple>::type::value;

          template <typename Particles, typename Time>
          static void call(Particles const& particles, Time time_step, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const inverse_time_step = Time{1} / time_step;
            auto const inverse_squared_time_step = inverse_time_step * inverse_time_step;

            auto const position_coefficient = Time{3} * time_step * time_step / Time{32};
            auto const velocity_coefficient = Time{251} * time_step / Time{720};
            auto const b_coefficient = Time{11} * inverse_time_step / Time{6};
            auto const c_coefficient = Time{2} * inverse_squared_time_step;
            auto const d_coefficient = inverse_squared_time_step * inverse_time_step;

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const a_data = particles.template data< ::pastel::container::tags::nth_integration_vector<acceleration_index> >();
            auto const b_data = particles.template data< ::pastel::container::tags::nth_integration_vector<b_index> >();
            auto const c_data = particles.template data< ::pastel::container::tags::nth_integration_vector<c_index> >();
            auto const d_data = particles.template data< ::pastel::container::tags::nth_integration_vector<d_index> >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              auto const correction = forces_data[index] / masses_data[index] - a_data[index];
              positions_data[index] += position_coefficient * correction;
              velocities_data[index] += velocity_coefficient * correction;
              b_data[index] += b_coefficient * correction;
              c_data[index] += c_coefficient * correction;
              d_data[index] += d_coefficient * correction;
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const inverse_time_step = Time{1} / time_step;
            auto const inverse_squared_time_step = inverse_time_step * inverse_time_step;

            auto const position_coefficient = Time{3} * time_step * time_step / Time{32};
            auto const velocity_coefficient = Time{251} * time_step / Time{720};
            auto const b_coefficient = Time{11} * inverse_time_step / Time{6};
            auto const c_coefficient = Time{2} * inverse_squared_time_step;
            auto const d_coefficient = inverse_squared_time_step * inverse_time_step;

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const a_data = particles.template data< ::pastel::container::tags::nth_integration_vector<acceleration_index> >();
            auto const b_data = particles.template data< ::pastel::container::tags::nth_integration_vector<b_index> >();
            auto const c_data = particles.template data< ::pastel::container::tags::nth_integration_vector<c_index> >();
            auto const d_data = particles.template data< ::pastel::container::tags::nth_integration_vector<d_index> >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                auto const correction = forces_data[index] / masses_data[index] - a_data[index];
                positions_data[index] += position_coefficient * correction;
                velocities_data[index] += velocity_coefficient * correction;
                b_data[index] += b_coefficient * correction;
                c_data[index] += c_coefficient * correction;
                d_data[index] += d_coefficient * correction;
              }
          }
        }; // struct correct_particles_impl<6u, true, true, acceleration_index, IntegrationVectorIndexTuple>

        template <std::size_t acceleration_index, typename IntegrationVectorIndexTuple>
        struct correct_particles_impl<6u, false, true, acceleration_index, IntegrationVectorIndexTuple>
        {
          static constexpr auto b_index = std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value;
          static constexpr auto c_index = std::tuple_element<1u, IntegrationVectorIndexTuple>::type::value;
          static constexpr auto d_index = std::tuple_element<2u, IntegrationVectorIndexTuple>::type::value;

          template <typename Particles, typename Time>
          static void call(Particles const& particles, Time time_step, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const inverse_time_step = Time{1} / time_step;
            auto const inverse_squared_time_step = inverse_time_step * inverse_time_step;

            auto const position_coefficient = Time{3} * time_step * time_step / Time{32};
            auto const velocity_coefficient = Time{251} * time_step / Time{720};
            auto const b_coefficient = Time{11} * inverse_time_step / Time{6};
            auto const c_coefficient = Time{2} * inverse_squared_time_step;
            auto const d_coefficient = inverse_squared_time_step * inverse_time_step;

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              auto const correction
                = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                    / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index)
                  - ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<acceleration_index> >(particles, index);
              ::pastel::container::get< ::pastel::particle::tags::position >(particles, index) += position_coefficient * correction;
              ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) += velocity_coefficient * correction;
              ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<b_index> >(particles, index) += b_coefficient * correction;
              ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<c_index> >(particles, index) += c_coefficient * correction;
              ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<d_index> >(particles, index) += d_coefficient * correction;
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const inverse_time_step = Time{1} / time_step;
            auto const inverse_squared_time_step = inverse_time_step * inverse_time_step;

            auto const position_coefficient = Time{3} * time_step * time_step / Time{32};
            auto const velocity_coefficient = Time{251} * time_step / Time{720};
            auto const b_coefficient = Time{11} * inverse_time_step / Time{6};
            auto const c_coefficient = Time{2} * inverse_squared_time_step;
            auto const d_coefficient = inverse_squared_time_step * inverse_time_step;

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                auto const correction
                  = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                      / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index)
                    - ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<acceleration_index> >(particles, index);
                ::pastel::container::get< ::pastel::particle::tags::position >(particles, index) += position_coefficient * correction;
                ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) += velocity_coefficient * correction;
                ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<b_index> >(particles, index) += b_coefficient * correction;
                ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<c_index> >(particles, index) += c_coefficient * correction;
                ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<d_index> >(particles, index) += d_coefficient * correction;
              }
          }
        }; // struct correct_particles_impl<6u, false, true, acceleration_index, IntegrationVectorIndexTuple>

        template <std::size_t acceleration_index, typename IntegrationVectorIndexTuple>
        struct correct_particles_impl<6u, true, false, acceleration_index, IntegrationVectorIndexTuple>
        {
          static constexpr auto b_index = std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value;
          static constexpr auto c_index = std::tuple_element<1u, IntegrationVectorIndexTuple>::type::value;
          static constexpr auto d_index = std::tuple_element<2u, IntegrationVectorIndexTuple>::type::value;

          template <typename Particles, typename Time>
          static void call(Particles const& particles, Time time_step, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const inverse_time_step = Time{1} / time_step;
            auto const inverse_squared_time_step = inverse_time_step * inverse_time_step;

            auto const position_coefficient = Time{3} * time_step * time_step / Time{32};
            auto const velocity_coefficient = Time{251} * time_step / Time{720};
            auto const b_coefficient = Time{11} * inverse_time_step / Time{6};
            auto const c_coefficient = Time{2} * inverse_squared_time_step;
            auto const d_coefficient = inverse_squared_time_step * inverse_time_step;

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const a_data = particles.template data< ::pastel::container::tags::nth_integration_vector<acceleration_index> >();
            auto const b_data = particles.template data< ::pastel::container::tags::nth_integration_vector<b_index> >();
            auto const c_data = particles.template data< ::pastel::container::tags::nth_integration_vector<c_index> >();
            auto const d_data = particles.template data< ::pastel::container::tags::nth_integration_vector<d_index> >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              auto const correction = forces_data[index] - a_data[index];
              positions_data[index] += position_coefficient * correction;
              velocities_data[index] += velocity_coefficient * correction;
              b_data[index] += b_coefficient * correction;
              c_data[index] += c_coefficient * correction;
              d_data[index] += d_coefficient * correction;
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const inverse_time_step = Time{1} / time_step;
            auto const inverse_squared_time_step = inverse_time_step * inverse_time_step;

            auto const position_coefficient = Time{3} * time_step * time_step / Time{32};
            auto const velocity_coefficient = Time{251} * time_step / Time{720};
            auto const b_coefficient = Time{11} * inverse_time_step / Time{6};
            auto const c_coefficient = Time{2} * inverse_squared_time_step;
            auto const d_coefficient = inverse_squared_time_step * inverse_time_step;

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const a_data = particles.template data< ::pastel::container::tags::nth_integration_vector<acceleration_index> >();
            auto const b_data = particles.template data< ::pastel::container::tags::nth_integration_vector<b_index> >();
            auto const c_data = particles.template data< ::pastel::container::tags::nth_integration_vector<c_index> >();
            auto const d_data = particles.template data< ::pastel::container::tags::nth_integration_vector<d_index> >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                auto const correction = forces_data[index] - a_data[index];
                positions_data[index] += position_coefficient * correction;
                velocities_data[index] += velocity_coefficient * correction;
                b_data[index] += b_coefficient * correction;
                c_data[index] += c_coefficient * correction;
                d_data[index] += d_coefficient * correction;
              }
          }
        }; // struct correct_particles_impl<6u, true, false, acceleration_index, IntegrationVectorIndexTuple>

        template <std::size_t acceleration_index, typename IntegrationVectorIndexTuple>
        struct correct_particles_impl<6u, false, false, acceleration_index, IntegrationVectorIndexTuple>
        {
          static constexpr auto b_index = std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value;
          static constexpr auto c_index = std::tuple_element<1u, IntegrationVectorIndexTuple>::type::value;
          static constexpr auto d_index = std::tuple_element<2u, IntegrationVectorIndexTuple>::type::value;

          template <typename Particles, typename Time>
          static void call(Particles const& particles, Time time_step, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const inverse_time_step = Time{1} / time_step;
            auto const inverse_squared_time_step = inverse_time_step * inverse_time_step;

            auto const position_coefficient = Time{3} * time_step * time_step / Time{32};
            auto const velocity_coefficient = Time{251} * time_step / Time{720};
            auto const b_coefficient = Time{11} * inverse_time_step / Time{6};
            auto const c_coefficient = Time{2} * inverse_squared_time_step;
            auto const d_coefficient = inverse_squared_time_step * inverse_time_step;

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              auto const correction
                = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                  - ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<acceleration_index> >(particles, index);
              ::pastel::container::get< ::pastel::particle::tags::position >(particles, index) += position_coefficient * correction;
              ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) += velocity_coefficient * correction;
              ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<b_index> >(particles, index) += b_coefficient * correction;
              ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<c_index> >(particles, index) += c_coefficient * correction;
              ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<d_index> >(particles, index) += d_coefficient * correction;
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const inverse_time_step = Time{1} / time_step;
            auto const inverse_squared_time_step = inverse_time_step * inverse_time_step;

            auto const position_coefficient = Time{3} * time_step * time_step / Time{32};
            auto const velocity_coefficient = Time{251} * time_step / Time{720};
            auto const b_coefficient = Time{11} * inverse_time_step / Time{6};
            auto const c_coefficient = Time{2} * inverse_squared_time_step;
            auto const d_coefficient = inverse_squared_time_step * inverse_time_step;

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                auto const correction
                  = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                    - ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<acceleration_index> >(particles, index);
                ::pastel::container::get< ::pastel::particle::tags::position >(particles, index) += position_coefficient * correction;
                ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) += velocity_coefficient * correction;
                ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<b_index> >(particles, index) += b_coefficient * correction;
                ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<c_index> >(particles, index) += c_coefficient * correction;
                ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<d_index> >(particles, index) += d_coefficient * correction;
              }
          }
        }; // struct correct_particles_impl<6u, false, false, acceleration_index, IntegrationVectorIndexTuple>


        template <std::size_t order, std::size_t acceleration_index, typename IntegrationVectorIndexTuple, typename Time>
        struct correct_particles
        {
          static_assert(order >= 4u && order <= 6u, "order must satisfy 4u <= order <= 6u");

         private:
          Time time_step_;

         public:
          correct_particles(Time time_step) : time_step_{time_step} { }

          template <typename Particles, typename ExternalForce>
          void operator()(Particles& particles, ExternalForce&&) const
          {
            static constexpr bool is_data_accessible
              = ::pastel::container::meta::is_data_accessible<Particles>::value;
            static constexpr bool has_mass
              = ::pastel::container::meta::has_mass<Particles>::value;
            using correct_particles_impl
              = ::pastel::integrate::gear::correct_particles_detail::correct_particles_impl<
                  order, is_data_accessible, has_mass, acceleration_index, IntegrationVectorIndexTuple>;
            using mobility_tag
              = typename ::pastel::container::meta::mobility_tag_of<Particles>::type;
            correct_particles_impl::call(particles, time_step_, mobility_tag{});
          }
        }; // struct correct_particles<IntegrationVectorIndexTuple, Time>
      } // namespace correct_particles_detail


      // IntegrationVectorIndexTuple: indices for b(t), c(t), ..., but not for a(t) (accelerations)
      template <std::size_t order, std::size_t acceleration_index, typename IntegrationVectorIndexTuple, typename System, typename Time>
      inline void correct_particles(System& system, Time time_step)
      {
        static_assert(order >= 4u && order <= 6u, "order must satisfy 4u <= order <= 6u");
        using correct_particles_func
          = ::pastel::integrate::gear::correct_particles_detail::correct_particles<
              order, acceleration_index, IntegrationVectorIndexTuple, Time>;
        ::pastel::system::for_each_container(system, correct_particles_func{time_step});
      }
    } // namespace gear
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_GEAR_CORRECT_PARTICLES_HPP

