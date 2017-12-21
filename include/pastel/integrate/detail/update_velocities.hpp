#ifndef PASTEL_INTEGRATE_DETAIL_UPDATE_VELOCITIES_HPP
# define PASTEL_INTEGRATE_DETAIL_UPDATE_VELOCITIES_HPP

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
      namespace update_velocities_detail
      {
        template <std::size_t order, bool is_data_accessible, bool has_mass, typename AdditionalVectorIndexTuple>
        struct update_velocities;


        // v_i(t+dt) = v_i(t)
        template <typename AdditionalVectorIndexTuple>
        struct update_velocities<0u, true, true, AdditionalVectorIndexTuple>
        {
          template <typename Particles, typename Time, typename MobilityTag>
          static void call(Particles const&, Time, MobilityTag)
          { }
        }; // struct update_velocities<0u, true, true, AdditionalVectorIndexTuple>

        template <typename AdditionalVectorIndexTuple>
        struct update_velocities<0u, true, false, AdditionalVectorIndexTuple>
          : update_velocities<0u, true, true, AdditionalVectorIndexTuple>
        { }; // struct update_velocities<0u, true, false, AdditionalVectorIndexTuple>

        template <typename AdditionalVectorIndexTuple>
        struct update_velocities<0u, false, true, AdditionalVectorIndexTuple>
          : update_velocities<0u, true, true, AdditionalVectorIndexTuple>
        { }; // struct update_velocities<0u, false, true, AdditionalVectorIndexTuple>

        template <typename AdditionalVectorIndexTuple>
        struct update_velocities<0u, false, false, AdditionalVectorIndexTuple>
          : update_velocities<0u, true, true, AdditionalVectorIndexTuple>
        { }; // struct update_velocities<0u, false, false, AdditionalVectorIndexTuple>


        // v_i(t+dt) = v_i(t) + [f_i(t) / m_i] dt
        template <typename AdditionalVectorIndexTuple>
        struct update_velocities<1u, true, true, AdditionalVectorIndexTuple>
        {
          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              velocities_data[index] += (forces_data[index] / masses_data[index]) * time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                velocities_data[index] += (forces_data[index] / masses_data[index]) * time_step;
          }
        }; // struct update_velocities<1u, true, true, AdditionalVectorIndexTuple>

        template <typename AdditionalVectorIndexTuple>
        struct update_velocities<1u, false, true, AdditionalVectorIndexTuple>
        {
          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index)
                += (::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                    / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index))
                   * time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index)
                  += (::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                      / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index))
                     * time_step;
          }
        }; // struct update_velocities<1u, false, true, AdditionalVectorIndexTuple>

        template <typename AdditionalVectorIndexTuple>
        struct update_velocities<1u, true, false, AdditionalVectorIndexTuple>
        {
          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              velocities_data[index] += forces_data[index] * time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                velocities_data[index] += forces_data[index] * time_step;
          }
        }; // struct update_velocities<1u, true, false, AdditionalVectorIndexTuple>

        template <typename AdditionalVectorIndexTuple>
        struct update_velocities<1u, false, false, AdditionalVectorIndexTuple>
        {
          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index)
                += ::pastel::container::get< ::pastel::particle::tags::force >(particles, index) * time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index)
                  += ::pastel::container::get< ::pastel::particle::tags::force >(particles, index) * time_step;
          }
        }; // struct update_velocities<1u, false, false, AdditionalVectorIndexTuple>


        // v_i(t+dt) = v_i(t) + [f_i(t) / m_i] dt + b_i(t) dt^2/2
        template <typename AdditionalVectorIndexTuple>
        struct update_velocities<2u, true, true, AdditionalVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<AdditionalVectorIndexTuple>::value == 1u,
            "Size of AdditionalVectorIndexTuple must be equal to 1u");

          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};

            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const order2_derivative_data
              = particles.template data<
                  ::pastel::container::tags::nth_additional_vector<
                    std::tuple_element<0u, AdditionalVectorIndexTuple>::type::value > >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              velocities_data[index]
                += (forces_data[index] / masses_data[index]) * time_step
                   + order2_derivative_data[index] * half_squared_time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};

            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const order2_derivative_data
              = particles.template data<
                  ::pastel::container::tags::nth_additional_vector<
                    std::tuple_element<0u, AdditionalVectorIndexTuple>::type::value > >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                velocities_data[index]
                  += (forces_data[index] / masses_data[index]) * time_step
                     + order2_derivative_data[index] * half_squared_time_step;
          }
        }; // struct update_velocities<2u, true, true, AdditionalVectorIndexTuple>

        template <typename AdditionalVectorIndexTuple>
        struct update_velocities<2u, false, true, AdditionalVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<AdditionalVectorIndexTuple>::value == 1u,
            "Size of AdditionalVectorIndexTuple must be equal to 1u");

          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index)
                += (::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                    / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index))
                   * time_step
                   + ::pastel::container::get<
                       ::pastel::container::tags::nth_additional_vector<
                         std::tuple_element<0u, AdditionalVectorIndexTuple>::type::value > >(particles, index)
                     * half_squared_time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index)
                  += (::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                      / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index))
                     * time_step
                     + ::pastel::container::get<
                         ::pastel::container::tags::nth_additional_vector<
                           std::tuple_element<0u, AdditionalVectorIndexTuple>::type::value > >(particles, index)
                       * half_squared_time_step;
          }
        }; // struct update_velocities<2u, false, true, AdditionalVectorIndexTuple>

        template <typename AdditionalVectorIndexTuple>
        struct update_velocities<2u, true, false, AdditionalVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<AdditionalVectorIndexTuple>::value == 1u,
            "Size of AdditionalVectorIndexTuple must be equal to 1u");

          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};

            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const order2_derivative_data
              = particles.template data<
                  ::pastel::container::tags::nth_additional_vector<
                    std::tuple_element<0u, AdditionalVectorIndexTuple>::type::value > >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              velocities_data[index]
                += forces_data[index] * time_step
                   + order2_derivative_data[index] * half_squared_time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};

            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const order2_derivative_data
              = particles.template data<
                  ::pastel::container::tags::nth_additional_vector<
                    std::tuple_element<0u, AdditionalVectorIndexTuple>::type::value > >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                velocities_data[index]
                  += forces_data[index] * time_step
                     + order2_derivative_data[index] * half_squared_time_step;
          }
        }; // struct update_velocities<2u, true, false, AdditionalVectorIndexTuple>

        template <typename AdditionalVectorIndexTuple>
        struct update_velocities<2u, false, false, AdditionalVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<AdditionalVectorIndexTuple>::value == 1u,
            "Size of AdditionalVectorIndexTuple must be equal to 1u");

          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index)
                += ::pastel::container::get< ::pastel::particle::tags::force >(particles, index) * time_step
                   + ::pastel::container::get<
                       ::pastel::container::tags::nth_additional_vector<
                         std::tuple_element<0u, AdditionalVectorIndexTuple>::type::value > >(particles, index)
                     * half_squared_time_step;
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const half_squared_time_step = time_step * time_step / Time{2};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index)
                  += ::pastel::container::get< ::pastel::particle::tags::force >(particles, index) * time_step
                     + ::pastel::container::get<
                         ::pastel::container::tags::nth_additional_vector<
                           std::tuple_element<0u, AdditionalVectorIndexTuple>::type::value > >(particles, index)
                       * half_squared_time_step;
          }
        }; // struct update_velocities<2u, false, false, AdditionalVectorIndexTuple>


        template <std::size_t index, std::size_t last>
        struct update_velocities_impl1
        {
          static_assert(index >= 2u, "index >= 2u");

          template <typename VelocitiesData, typename DerivativeDataArray, typename DerivativeCoefficients>
          static constexpr void call(
            VelocitiesData& velocities_data,
            DerivativeDataArray const& derivative_data_array,
            DerivativeCoefficients const& derivative_coefficients,
            std::size_t particle_index)
          {
            velocities_data[particle_index]
              += derivative_data_array[index-2u][particle_index]
                 * derivative_coefficients[index];

            ::pastel::integrate::detail::update_velocities_detail::update_velocities_impl1<index+1u, last>::call(
              velocities_data, derivative_data_array, derivative_coefficients, particle_index);
          }
        }; // struct update_velocities_impl1<index, last>

        template <std::size_t last>
        struct update_velocities_impl1<last, last>
        {
          template <typename VelocitiesData, typename DerivativeDataArray, typename DerivativeCoefficients>
          static constexpr void call(VelocitiesData&, DerivativeDataArray const&, DerivativeCoefficients const&, std::size_t)
          { }
        }; // struct update_velocities_impl1<last, last>

        template <std::size_t index, std::size_t last, typename AdditionalVectorIndexTuple>
        struct update_velocities_impl2
        {
          static_assert(index >= 2u, "index >= 2u");

          static constexpr auto derivative_index
            = std::tuple_element<index-2u, AdditionalVectorIndexTuple>::type::value;

          template <typename Particles, typename DerivativeCoefficients>
          static constexpr void call(
            Particles& particles,
            DerivativeCoefficients const& derivative_coefficients,
            std::size_t particle_index)
          {
            ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, particle_index)
              += ::pastel::container::get< ::pastel::container::tags::nth_additional_vector<derivative_index> >(particles, particle_index)
                 * derivative_coefficients[index];

            ::pastel::integrate::detail::update_velocities_detail::update_velocities_impl2<index+1u, last, AdditionalVectorIndexTuple>::call(
              particles, derivative_coefficients, particle_index);
          }
        }; // struct update_velocities_impl2<index, last, AdditionalVectorIndexTuple>

        template <std::size_t last, typename AdditionalVectorIndexTuple>
        struct update_velocities_impl2<last, last, AdditionalVectorIndexTuple>
        {
          template <typename Particles, typename DerivativeCoefficients>
          static constexpr void call(Particles&, DerivativeCoefficients const&, std::size_t)
          { }
        }; // struct update_velocities_impl2<last, last, AdditionalVectorIndexTuple>

        // v_i(t+dt) = v_i(t) + [f_i(t) / m_i] dt + b_i(t) dt^2/2 + c_i(t) dt^3/6 + ...
        template <std::size_t order, typename AdditionalVectorIndexTuple>
        struct update_velocities<order, true, true, AdditionalVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<AdditionalVectorIndexTuple>::value == order - 1u,
            "Size of AdditionalVectorIndexTuple must be equal to order - 1u");

          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const derivative_data_array
              = ::pastel::integrate::detail::derivative_data_array<order-1u, AdditionalVectorIndexTuple, Particles>{particles};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              velocities_data[index]
                += (forces_data[index] / masses_data[index]) * derivative_coefficients[1u];

              ::pastel::integrate::detail::update_velocities_detail::update_velocities_impl1<2u, order+1u>::call(
                velocities_data, derivative_data_array, derivative_coefficients, index);
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const derivative_data_array
              = ::pastel::integrate::detail::derivative_data_array<order-1u, AdditionalVectorIndexTuple, Particles>{particles};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                velocities_data[index]
                  += (forces_data[index] / masses_data[index]) * derivative_coefficients[1u];

                ::pastel::integrate::detail::update_velocities_detail::update_velocities_impl1<2u, order+1u>::call(
                  velocities_data, derivative_data_array, derivative_coefficients, index);
              }
          }
        }; // struct update_velocities<order, true, true, AdditionalVectorIndexTuple>

        template <std::size_t order, typename AdditionalVectorIndexTuple>
        struct update_velocities<order, false, true, AdditionalVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<AdditionalVectorIndexTuple>::value == order - 1u,
            "Size of AdditionalVectorIndexTuple must be equal to order - 1u");

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
              ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index)
                += (::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                    / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index))
                   * derivative_coefficients[1u];

              ::pastel::integrate::detail::update_velocities_detail::update_velocities_impl2<2u, order+1u, AdditionalVectorIndexTuple>::call(
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
                ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index)
                  += (::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                      / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index))
                     * derivative_coefficients[1u];

                ::pastel::integrate::detail::update_velocities_detail::update_velocities_impl2<2u, order+1u, AdditionalVectorIndexTuple>::call(
                  particles, derivative_coefficients, index);
              }
          }
        }; // struct update_velocities<order, false, true, AdditionalVectorIndexTuple>

        template <std::size_t order, typename AdditionalVectorIndexTuple>
        struct update_velocities<order, true, false, AdditionalVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<AdditionalVectorIndexTuple>::value == order - 1u,
            "Size of AdditionalVectorIndexTuple must be equal to order - 1u");

          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const derivative_data_array
              = ::pastel::integrate::detail::derivative_data_array<order-1u, AdditionalVectorIndexTuple, Particles>{particles};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              velocities_data[index]
                += forces_data[index] * derivative_coefficients[1u];

              ::pastel::integrate::detail::update_velocities_detail::update_velocities_impl1<2u, order+1u>::call(
                velocities_data, derivative_data_array, derivative_coefficients, index);
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const velocities_data = particles.template data< ::pastel::particle::tags::velocity >();
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const derivative_data_array
              = ::pastel::integrate::detail::derivative_data_array<order-2u, AdditionalVectorIndexTuple, Particles>{particles};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                velocities_data[index]
                  += forces_data[index] * derivative_coefficients[1u];

                ::pastel::integrate::detail::update_velocities_detail::update_velocities_impl1<2u, order+1u>::call(
                  velocities_data, derivative_data_array, derivative_coefficients, index);
              }
          }
        }; // struct update_velocities<order, true, false, AdditionalVectorIndexTuple>

        template <std::size_t order, typename AdditionalVectorIndexTuple>
        struct update_velocities<order, false, false, AdditionalVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<AdditionalVectorIndexTuple>::value == order - 1u,
            "Size of AdditionalVectorIndexTuple must be equal to order - 1u");

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
              ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index)
                += ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                   * derivative_coefficients[1u];

              ::pastel::integrate::detail::update_velocities_detail::update_velocities_impl2<2u, order+1u, AdditionalVectorIndexTuple>::call(
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
                ::pastel::container::get< ::pastel::particle::tags::velocity >(particles, index)
                  += ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                     * derivative_coefficients[1u];

                ::pastel::integrate::detail::update_velocities_detail::update_velocities_impl2<2u, order+1u, AdditionalVectorIndexTuple>::call(
                  particles, derivative_coefficients, index);
              }
          }
        }; // struct update_velocities<order, false, false, AdditionalVectorIndexTuple>
      } // namespace update_velocities_detail


      // AdditionalVectorIndexTuple: indices for b(t), c(t), ..., but not for a(t) (accelerations)
      template <std::size_t order, typename AdditionalVectorIndexTuple, typename Particles, typename Time>
      inline void update_velocities(Particles& particles, Time time_step)
      {
        static_assert(
          ::pastel::utility::tuple::meta::is_tuple<AdditionalVectorIndexTuple>::value
          && ::pastel::utility::tuple::meta::all_of<
               AdditionalVectorIndexTuple, ::pastel::utility::is_integral_constant>::value,
          "AdditionalVectorIndexTuple must be a tuple whose type is std::integral_constant");

        static constexpr bool is_data_accessible
          = ::pastel::container::meta::is_data_accessible<Particles>::value;
        static constexpr bool has_mass
          = ::pastel::container::meta::has_mass<Particles>::value;
        using update_velocities_func
          = ::pastel::integrate::detail::update_velocities_detail::update_velocities<order, is_data_accessible, has_mass, AdditionalVectorIndexTuple>;
        using mobility_tag
          = typename ::pastel::container::meta::mobility_tag_of<Particles>::type;
        update_velocities_func::call(particles, time_step, mobility_tag{});
      }
    } // namespace detail
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_DETAIL_UPDATE_VELOCITIES_HPP

