#ifndef PASTEL_INTEGRATE_DETAIL_UPDATE_POSITIONS_HPP
# define PASTEL_INTEGRATE_DETAIL_UPDATE_POSITIONS_HPP

# include <cstddef>
# include <tuple>

# include <pastel/integrate/detail/derivative_coefficients.hpp>
# include <pastel/integrate/detail/derivative_data_array.hpp>
# include <pastel/container/num_particles.hpp>
# include <pastel/container/get.hpp>
# include <pastel/container/tags.hpp>
# include <pastel/container/is_mobile.hpp>
# include <pastel/container/mobility_tags.hpp>
# include <pastel/container/meta/is_data_accessible.hpp>
# include <pastel/container/meta/has_mass.hpp>
# include <pastel/container/meta/mobility_tag_of.hpp>
# include <pastel/particle/tags.hpp>
# include <pastel/utility/is_integral_constant.hpp>
# include <pastel/utility/tuple/meta/is_tuple.hpp>
# include <pastel/utility/tuple/meta/all_of.hpp>


namespace pastel
{
  namespace integrate
  {
    namespace detail
    {
      namespace update_positions_detail
      {
        template <std::size_t order, bool is_data_accessible, bool has_mass, typename IntegrationVectorIndexTuple>
        struct update_positions;


        // x_i(t+dt) = x_i(t)
        template <typename IntegrationVectorIndexTuple>
        struct update_positions<0u, true, true, IntegrationVectorIndexTuple>
        {
          template <typename Particles, typename Time, typename MobilityTag>
          static void call(Particles const&, Time, MobilityTag)
          { }
        }; // update_positions<0u, true, true, IntegrationVectorIndexTuple

        template <typename IntegrationVectorIndexTuple>
        struct update_positions<0u, true, false, IntegrationVectorIndexTuple>
          : update_positions<0u, true, true, IntegrationVectorIndexTuple>
        { }; // update_positions<0u, true, false, IntegrationVectorIndexTuple

        template <typename IntegrationVectorIndexTuple>
        struct update_positions<0u, false, true, IntegrationVectorIndexTuple>
          : update_positions<0u, true, true, IntegrationVectorIndexTuple>
        { }; // update_positions<0u, false, true, IntegrationVectorIndexTuple

        template <typename IntegrationVectorIndexTuple>
        struct update_positions<0u, false, false, IntegrationVectorIndexTuple>
          : update_positions<0u, true, true, IntegrationVectorIndexTuple>
        { }; // update_positions<0u, false, false, IntegrationVectorIndexTuple


        // x_i(t+dt) = x_i(t) + v_i(t) dt
        template <typename IntegrationVectorIndexTuple>
        struct update_positions<1u, true, true, IntegrationVectorIndexTuple>
        {
          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              positions_data[index] += velocities_data[index] * time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                positions_data[index] += velocities_data[index] * time_step;
          }
        }; // struct update_positions<1u, true, true, IntegrationVectorIndexTuple>

        template <typename IntegrationVectorIndexTuple>
        struct update_positions<1u, true, false, IntegrationVectorIndexTuple>
          : update_positions<1u, true, true, IntegrationVectorIndexTuple>
        { }; // struct update_positions<1u, true, false, IntegrationVectorIndexTuple>

        template <typename IntegrationVectorIndexTuple>
        struct update_positions<1u, false, true, IntegrationVectorIndexTuple>
        {
          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              ::pastel::container::get< ::pastel::particle::tags::position >(particles, index)
                += ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) * time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                ::pastel::container::get< ::pastel::particle::tags::position >(particles, index)
                  += ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) * time_step;
          }
        }; // struct update_positions<1u, false, true, IntegrationVectorIndexTuple>

        template <typename IntegrationVectorIndexTuple>
        struct update_positions<1u, false, false, IntegrationVectorIndexTuple>
          : update_positions<1u, false, true, IntegrationVectorIndexTuple>
        { }; // struct update_positions<1u, false, false, IntegrationVectorIndexTuple>


        // x_i(t+dt) = x_i(t) + v_i(t) dt + [f_i(t) / m_i] dt^2/2
        template <typename IntegrationVectorIndexTuple>
        struct update_positions<2u, true, true, IntegrationVectorIndexTuple>
        {
          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              positions_data[index] += velocities_data[index] * time_step + (forces_data[index] / masses_data[index]) * half_squared_time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                positions_data[index] += velocities_data[index] * time_step + (forces_data[index] / masses_data[index]) * half_squared_time_step;
          }
        }; // struct update_positions<2u, true, true, IntegrationVectorIndexTuple>

        template <typename IntegrationVectorIndexTuple>
        struct update_positions<2u, false, true, IntegrationVectorIndexTuple>
        {
          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              ::pastel::container::get< ::pastel::particle::tags::position >(particles, index)
                += ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) * time_step
                   + (::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                      / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index))
                     * half_squared_time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                ::pastel::container::get< ::pastel::particle::tags::position >(particles, index)
                  += ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) * time_step
                     + (::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                        / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index))
                       * half_squared_time_step;
          }
        }; // struct update_positions<2u, false, true, IntegrationVectorIndexTuple>

        template <typename IntegrationVectorIndexTuple>
        struct update_positions<2u, true, false, IntegrationVectorIndexTuple>
        {
          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              positions_data[index] += velocities_data[index] * time_step + forces_data[index] * half_squared_time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                positions_data[index] += velocities_data[index] * time_step + forces_data[index] * half_squared_time_step;
          }
        }; // struct update_positions<2u, true, false, IntegrationVectorIndexTuple>

        template <typename IntegrationVectorIndexTuple>
        struct update_positions<2u, false, false, IntegrationVectorIndexTuple>
        {
          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              ::pastel::container::get< ::pastel::particle::tags::position >(particles, index)
                += ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) * time_step
                   + ::pastel::container::get< ::pastel::particle::tags::force >(particles, index) * half_squared_time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                ::pastel::container::get< ::pastel::particle::tags::position >(particles, index)
                  += ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) * time_step
                     + ::pastel::container::get< ::pastel::particle::tags::force >(particles, index) * half_squared_time_step;
          }
        }; // struct update_positions<2u, false, false, IntegrationVectorIndexTuple>


        // x_i(t+dt) = x_i(t) + v_i(t) dt + [f_i(t) / m_i] dt^2/2 + b_i(t) dt^3/6
        template <typename IntegrationVectorIndexTuple>
        struct update_positions<3u, true, true, IntegrationVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<IntegrationVectorIndexTuple>::value == 1u,
            "Size of IntegrationVectorIndexTuple must be equal to 1u");

          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};
            auto const order3_coefficient = half_squared_time_step * time_step / Time{3};

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const order3_derivative_data
              = particles.template data<
                  ::pastel::container::tags::nth_integration_vector<
                    std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value > >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              positions_data[index]
                += velocities_data[index] * time_step
                   + (forces_data[index] / masses_data[index]) * half_squared_time_step
                   + order3_derivative_data[index] * order3_coefficient;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};
            auto const order3_coefficient = half_squared_time_step * time_step / Time{3};

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const order3_derivative_data
              = particles.template data<
                  ::pastel::container::tags::nth_integration_vector<
                    std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value > >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                positions_data[index]
                  += velocities_data[index] * time_step
                     + (forces_data[index] / masses_data[index]) * half_squared_time_step
                     + order3_derivative_data[index] * order3_coefficient;
          }
        }; // struct update_positions<3u, true, true, IntegrationVectorIndexTuple>

        template <typename IntegrationVectorIndexTuple>
        struct update_positions<3u, false, true, IntegrationVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<IntegrationVectorIndexTuple>::value == 1u,
            "Size of IntegrationVectorIndexTuple must be equal to 1u");

          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};
            auto const order3_coefficient = half_squared_time_step * time_step / Time{3};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              ::pastel::container::get< ::pastel::particle::tags::position >(particles, index)
                += ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) * time_step
                   + (::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                      / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index))
                     * half_squared_time_step
                   + ::pastel::container::get<
                       ::pastel::container::tags::nth_integration_vector<
                         std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value > >(particles, index)
                     * order3_coefficient;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};
            auto const order3_coefficient = half_squared_time_step * time_step / Time{3};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                ::pastel::container::get< ::pastel::particle::tags::position >(particles, index)
                  += ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) * time_step
                     + (::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                        / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index))
                       * half_squared_time_step
                     + ::pastel::container::get<
                         ::pastel::container::tags::nth_integration_vector<
                           std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value > >(particles, index)
                       * order3_coefficient;
          }
        }; // struct update_positions<3u, false, true, IntegrationVectorIndexTuple>

        template <typename IntegrationVectorIndexTuple>
        struct update_positions<3u, true, false, IntegrationVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<IntegrationVectorIndexTuple>::value == 1u,
            "Size of IntegrationVectorIndexTuple must be equal to 1u");

          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};
            auto const order3_coefficient = half_squared_time_step * time_step / Time{3};

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const order3_derivative_data
              = particles.template data<
                  ::pastel::container::tags::nth_integration_vector<
                    std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value > >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              positions_data[index]
                += velocities_data[index] * time_step
                   + forces_data[index] * half_squared_time_step
                   + order3_derivative_data[index] * order3_coefficient;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};
            auto const order3_coefficient = half_squared_time_step * time_step / Time{3};

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const order3_derivative_data
              = particles.template data<
                  ::pastel::container::tags::nth_integration_vector<
                    std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value > >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                positions_data[index]
                  += velocities_data[index] * time_step
                     + forces_data[index] * half_squared_time_step
                     + order3_derivative_data[index] * order3_coefficient;
          }
        }; // struct update_positions<3u, true, false, IntegrationVectorIndexTuple>

        template <typename IntegrationVectorIndexTuple>
        struct update_positions<3u, false, false, IntegrationVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<IntegrationVectorIndexTuple>::value == 1u,
            "Size of IntegrationVectorIndexTuple must be equal to 1u");

          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};
            auto const order3_coefficient = half_squared_time_step * time_step / Time{3};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              ::pastel::container::get< ::pastel::particle::tags::position >(particles, index)
                += ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) * time_step
                   + ::pastel::container::get< ::pastel::particle::tags::force >(particles, index) * half_squared_time_step
                   + ::pastel::container::get<
                       ::pastel::container::tags::nth_integration_vector<
                         std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value > >(particles, index)
                     * order3_coefficient;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};
            auto const order3_coefficient = half_squared_time_step * time_step / Time{3};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                ::pastel::container::get< ::pastel::particle::tags::position >(particles, index)
                  += ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) * time_step
                     + ::pastel::container::get< ::pastel::particle::tags::force >(particles, index) * half_squared_time_step
                     + ::pastel::container::get<
                         ::pastel::container::tags::nth_integration_vector<
                           std::tuple_element<0u, IntegrationVectorIndexTuple>::type::value > >(particles, index)
                       * order3_coefficient;
          }
        }; // struct update_positions<3u, false, false, IntegrationVectorIndexTuple>


        template <std::size_t index, std::size_t last>
        struct update_positions_impl1
        {
          static_assert(index >= 3u, "index >= 3u");

          template <typename PositionsData, typename DerivativeDataArray, typename DerivativeCoefficients>
          static constexpr void call(
            PositionsData& positions_data,
            DerivativeDataArray const& derivative_data_array,
            DerivativeCoefficients const& derivative_coefficients,
            std::size_t particle_index)
          {
            positions_data[particle_index]
              += derivative_data_array[index-3u][particle_index]
                 * derivative_coefficients[index];

            ::pastel::integrate::detail::update_positions_detail::update_positions_impl1<index+1u, last>::call(
              positions_data, derivative_data_array, derivative_coefficients, particle_index);
          }
        }; // struct update_positions_impl1<index, last>

        template <std::size_t last>
        struct update_positions_impl1<last, last>
        {
          template <typename PositionsData, typename DerivativeDataArray, typename DerivativeCoefficients>
          static constexpr void call(PositionsData&, DerivativeDataArray const&, DerivativeCoefficients const&, std::size_t)
          { }
        }; // struct update_positions_impl1<last, last>

        template <std::size_t index, std::size_t last, typename IntegrationVectorIndexTuple>
        struct update_positions_impl2
        {
          static_assert(index >= 3u, "index >= 3u");

          static constexpr auto derivative_index
            = std::tuple_element<index-3u, IntegrationVectorIndexTuple>::type::value;

          template <typename Particles, typename DerivativeCoefficients>
          static constexpr void call(
            Particles& particles,
            DerivativeCoefficients const& derivative_coefficients,
            std::size_t particle_index)
          {
            ::pastel::container::get< ::pastel::particle::tags::position >(particles, particle_index)
              += ::pastel::container::get< ::pastel::container::tags::nth_integration_vector<derivative_index> >(particles, particle_index)
                 * derivative_coefficients[index];

            ::pastel::integrate::detail::update_positions_detail::update_positions_impl2<index+1u, last, IntegrationVectorIndexTuple>::call(
              particles, derivative_coefficients, particle_index);
          }
        }; // struct update_positions_impl2<index, last, IntegrationVectorIndexTuple>

        template <std::size_t last, typename IntegrationVectorIndexTuple>
        struct update_positions_impl2<last, last, IntegrationVectorIndexTuple>
        {
          template <typename Particles, typename DerivativeCoefficients>
          static constexpr void call(Particles&, DerivativeCoefficients const&, std::size_t)
          { }
        }; // struct update_positions_impl2<last, last, IntegrationVectorIndexTuple>

        // x_i(t+dt) = x_i(t) + v_i(t) dt + [f_i(t) / m_i] dt^2/2 + b_i(t) dt^3/6 + c_i(t) dt^4/24 + ...
        template <std::size_t order, typename IntegrationVectorIndexTuple>
        struct update_positions<order, true, true, IntegrationVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<IntegrationVectorIndexTuple>::value == order - 2u,
            "Size of IntegrationVectorIndexTuple must be equal to order - 2u");

          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const derivative_data_array
              = ::pastel::integrate::detail::derivative_data_array<order-2u, IntegrationVectorIndexTuple, Particles>{particles};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              positions_data[index]
                += velocities_data[index] * derivative_coefficients[1u]
                   + (forces_data[index] / masses_data[index]) * derivative_coefficients[2u];

              ::pastel::integrate::detail::update_positions_detail::update_positions_impl1<3u, order+1u>::call(
                positions_data, derivative_data_array, derivative_coefficients, index);
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const derivative_data_array
              = ::pastel::integrate::detail::derivative_data_array<order-2u, IntegrationVectorIndexTuple, Particles>{particles};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                positions_data[index]
                  += velocities_data[index] * derivative_coefficients[1u]
                     + (forces_data[index] / masses_data[index]) * derivative_coefficients[2u];

                ::pastel::integrate::detail::update_positions_detail::update_positions_impl1<3u, order+1u>::call(
                  positions_data, derivative_data_array, derivative_coefficients, index);
              }
          }
        }; // struct update_positions<order, true, true, IntegrationVectorIndexTuple>

        template <std::size_t order, typename IntegrationVectorIndexTuple>
        struct update_positions<order, false, true, IntegrationVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<IntegrationVectorIndexTuple>::value == order - 2u,
            "Size of IntegrationVectorIndexTuple must be equal to order - 2u");

          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              ::pastel::container::get< ::pastel::particle::tags::position >(particles, index)
                += ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) * derivative_coefficients[1u]
                   + (::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                      / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index))
                     * derivative_coefficients[2u];

              ::pastel::integrate::detail::update_positions_detail::update_positions_impl2<3u, order+1u, IntegrationVectorIndexTuple>::call(
                particles, derivative_coefficients, index);
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                ::pastel::container::get< ::pastel::particle::tags::position >(particles, index)
                  += ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) * derivative_coefficients[1u]
                     + (::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                        / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index))
                       * derivative_coefficients[2u];

                ::pastel::integrate::detail::update_positions_detail::update_positions_impl2<3u, order+1u, IntegrationVectorIndexTuple>::call(
                  particles, derivative_coefficients, index);
              }
          }
        }; // struct update_positions<order, false, true, IntegrationVectorIndexTuple>

        template <std::size_t order, typename IntegrationVectorIndexTuple>
        struct update_positions<order, true, false, IntegrationVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<IntegrationVectorIndexTuple>::value == order - 2u,
            "Size of IntegrationVectorIndexTuple must be equal to order - 2u");

          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const derivative_data_array
              = ::pastel::integrate::detail::derivative_data_array<order-2u, IntegrationVectorIndexTuple, Particles>{particles};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              positions_data[index]
                += velocities_data[index] * derivative_coefficients[1u]
                   + forces_data[index] * derivative_coefficients[2u];

              ::pastel::integrate::detail::update_positions_detail::update_positions_impl1<3u, order+1u>::call(
                positions_data, derivative_data_array, derivative_coefficients, index);
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const derivative_data_array
              = ::pastel::integrate::detail::derivative_data_array<order-2u, IntegrationVectorIndexTuple, Particles>{particles};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                positions_data[index]
                  += velocities_data[index] * derivative_coefficients[1u]
                     + forces_data[index] * derivative_coefficients[2u];

                ::pastel::integrate::detail::update_positions_detail::update_positions_impl1<3u, order+1u>::call(
                  positions_data, derivative_data_array, derivative_coefficients, index);
              }
          }
        }; // struct update_positions<order, true, false, IntegrationVectorIndexTuple>

        template <std::size_t order, typename IntegrationVectorIndexTuple>
        struct update_positions<order, false, false, IntegrationVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<IntegrationVectorIndexTuple>::value == order - 2u,
            "Size of IntegrationVectorIndexTuple must be equal to order - 2u");

          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              ::pastel::container::get< ::pastel::particle::tags::position >(particles, index)
                += ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) * derivative_coefficients[1u]
                   + ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                     * derivative_coefficients[2u];

              ::pastel::integrate::detail::update_positions_detail::update_positions_impl2<3u, order+1u, IntegrationVectorIndexTuple>::call(
                particles, derivative_coefficients, index);
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                ::pastel::container::get< ::pastel::particle::tags::position >(particles, index)
                  += ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index) * derivative_coefficients[1u]
                     + ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                       * derivative_coefficients[2u];

                ::pastel::integrate::detail::update_positions_detail::update_positions_impl2<3u, order+1u, IntegrationVectorIndexTuple>::call(
                  particles, derivative_coefficients, index);
              }
          }
        }; // struct update_positions<order, false, false, IntegrationVectorIndexTuple>
      } // namespace update_positions_detail


      // IntegrationVectorIndexTuple: indices for b(t), c(t), ..., but not for a(t) (accelerations)
      template <std::size_t order, typename IntegrationVectorIndexTuple, typename Particles, typename Time>
      inline void update_positions(Particles& particles, Time time_step)
      {
        static_assert(
          ::pastel::utility::tuple::meta::is_tuple<IntegrationVectorIndexTuple>::value
          && ::pastel::utility::tuple::meta::all_of<
               IntegrationVectorIndexTuple, ::pastel::utility::is_integral_constant>::value,
          "IntegrationVectorIndexTuple must be a tuple whose type is std::integral_constant");

        static constexpr bool is_data_accessible
          = ::pastel::container::meta::is_data_accessible<Particles>::value;
        static constexpr bool has_mass
          = ::pastel::container::meta::has_mass<Particles>::value;
        using update_positions_func
          = ::pastel::integrate::detail::update_positions_detail::update_positions<order, is_data_accessible, has_mass, IntegrationVectorIndexTuple>;
        using mobility_tag
          = typename ::pastel::container::meta::mobility_tag_of<Particles>::type;
        update_positions_func::call(particles, time_step, mobility_tag{});
      }
    } // namespace detail
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_DETAIL_UPDATE_POSITIONS_HPP

