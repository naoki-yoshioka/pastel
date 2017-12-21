#ifndef PASTEL_INTEGRATE_DETAIL_UPDATE_ACCELERATIONS_HPP
# define PASTEL_INTEGRATE_DETAIL_UPDATE_ACCELERATIONS_HPP

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
      namespace update_accelerations_detail
      {
        template <
          std::size_t order, bool is_data_accessible, bool has_mass,
          std::size_t acceleration_index, typename AdditionalVectorIndexTuple>
        struct update_accelerations;


        // a_i(t+dt) = f_i(t) / m_i
        template <std::size_t acceleration_index, typename AdditionalVectorIndexTuple>
        struct update_accelerations<0u, true, true, acceleration_index, AdditionalVectorIndexTuple>
        {
          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time, ::pastel::container::mobility_tags::mobile)
          {
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const accelerations_data
              = particles.template data<
                  ::pastel::container::tags::nth_additional_vector<acceleration_index> >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              accelerations_data[index] = forces_data[index] / masses_data[index];
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const accelerations_data
              = particles.template data<
                  ::pastel::container::tags::nth_additional_vector<acceleration_index> >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                accelerations_data[index] = forces_data[index] / masses_data[index];
          }
        }; // struct update_accelerations<0u, true, true, acceleration_index, AdditionalVectorIndexTuple>

        template <std::size_t acceleration_index, typename AdditionalVectorIndexTuple>
        struct update_accelerations<0u, false, true, acceleration_index, AdditionalVectorIndexTuple>
        {
          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time, ::pastel::container::mobility_tags::mobile)
          {
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              ::pastel::container::get< ::pastel::container::tags::nth_additional_vector<acceleration_index> >(particles, index)
                = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                    / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index);
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                ::pastel::container::get< ::pastel::container::tags::nth_additional_vector<acceleration_index> >(particles, index)
                  = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                      / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index);
          }
        }; // struct update_accelerations<0u, false, true, acceleration_index, AdditionalVectorIndexTuple>

        template <std::size_t acceleration_index, typename AdditionalVectorIndexTuple>
        struct update_accelerations<0u, true, false, acceleration_index, AdditionalVectorIndexTuple>
        {
          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time, ::pastel::container::mobility_tags::mobile)
          {
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const accelerations_data
              = particles.template data<
                  ::pastel::container::tags::nth_additional_vector<acceleration_index> >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              accelerations_data[index] = forces_data[index];
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const accelerations_data
              = particles.template data<
                  ::pastel::container::tags::nth_additional_vector<acceleration_index> >();

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                accelerations_data[index] = forces_data[index];
          }
        }; // struct update_accelerations<0u, true, false, acceleration_index, AdditionalVectorIndexTuple>

        template <std::size_t acceleration_index, typename AdditionalVectorIndexTuple>
        struct update_accelerations<0u, false, false, acceleration_index, AdditionalVectorIndexTuple>
        {
          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time, ::pastel::container::mobility_tags::mobile)
          {
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              ::pastel::container::get< ::pastel::container::tags::nth_additional_vector<acceleration_index> >(particles, index)
                = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index);
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                ::pastel::container::get< ::pastel::container::tags::nth_additional_vector<acceleration_index> >(particles, index)
                  = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index);
          }
        }; // struct update_accelerations<0u, false, false, acceleration_index, AdditionalVectorIndexTuple>


        template <std::size_t index, std::size_t last>
        struct update_accelerations_impl1
        {
          static_assert(index >= 1u, "index >= 1u");

          template <typename AccelerationsData, typename DerivativeDataArray, typename DerivativeCoefficients>
          static constexpr void call(
            AccelerationsData& accelerations_data,
            DerivativeDataArray const& derivative_data_array,
            DerivativeCoefficients const& derivative_coefficients,
            std::size_t particle_index)
          {
            accelerations_data[particle_index]
              += derivative_data_array[index-1u][particle_index]
                 * derivative_coefficients[index];

            ::pastel::integrate::detail::update_accelerations_detail::update_accelerations_impl1<index+1u, last>::call(
              accelerations_data, derivative_data_array, derivative_coefficients, particle_index);
          }
        }; // struct update_accelerations_impl1<index, last>

        template <std::size_t last>
        struct update_accelerations_impl1<last, last>
        {
          template <typename AccelerationsData, typename DerivativeDataArray, typename DerivativeCoefficients>
          static constexpr void call(AccelerationsData&, DerivativeDataArray const&, DerivativeCoefficients const&, std::size_t)
          { }
        }; // struct update_accelerations_impl1<last, last>

        template <std::size_t index, std::size_t last, std::size_t acceleration_index, typename AdditionalVectorIndexTuple>
        struct update_accelerations_impl2
        {
          static_assert(index >= 1u, "index >= 1u");

          static constexpr auto derivative_index
            = std::tuple_element<index-1u, AdditionalVectorIndexTuple>::type::value;

          template <typename Particles, typename DerivativeCoefficients>
          static constexpr void call(
            Particles& particles,
            DerivativeCoefficients const& derivative_coefficients,
            std::size_t particle_index)
          {
            ::pastel::container::get< ::pastel::container::tags::nth_additional_vector<acceleration_index> >(particles, particle_index)
              += ::pastel::container::get< ::pastel::container::tags::nth_additional_vector<derivative_index> >(particles, particle_index)
                 * derivative_coefficients[index];

            ::pastel::integrate::detail::update_accelerations_detail::update_accelerations_impl2<index+1u, last, acceleration_index, AdditionalVectorIndexTuple>::call(
              particles, derivative_coefficients, particle_index);
          }
        }; // struct update_accelerations_impl2<index, last, acceleration_index, AdditionalVectorIndexTuple>

        template <std::size_t last, std::size_t acceleration_index, typename AdditionalVectorIndexTuple>
        struct update_accelerations_impl2<last, last, acceleration_index, AdditionalVectorIndexTuple>
        {
          template <typename Particles, typename DerivativeCoefficients>
          static constexpr void call(Particles&, DerivativeCoefficients const&, std::size_t)
          { }
        }; // struct update_accelerations_impl2<last, last, acceleration_index, AdditionalVectorIndexTuple>

        // a_i(t+dt) = f_i(t) / m_i + b_i(t) dt + c_i(t) dt^2/2 + ...
        template <std::size_t order, std::size_t acceleration_index, typename AdditionalVectorIndexTuple>
        struct update_accelerations<order, true, true, acceleration_index, AdditionalVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<AdditionalVectorIndexTuple>::value == order,
            "Size of AdditionalVectorIndexTuple must be equal to order");

          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const accelerations_data
              = particles.template data<
                  ::pastel::container::tags::nth_additional_vector<acceleration_index> >();
            auto const derivative_data_array
              = ::pastel::integrate::detail::derivative_data_array<order, AdditionalVectorIndexTuple, Particles>{particles};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              accelerations_data[index] = forces_data[index] / masses_data[index];

              ::pastel::integrate::detail::update_accelerations_detail::update_accelerations_impl1<1u, order+1u>::call(
                accelerations_data, derivative_data_array, derivative_coefficients, index);
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const masses_data = particles.template data< ::pastel::particle::tags::mass >();
            auto const accelerations_data
              = particles.template data<
                  ::pastel::container::tags::nth_additional_vector<acceleration_index> >();
            auto const derivative_data_array
              = ::pastel::integrate::detail::derivative_data_array<order, AdditionalVectorIndexTuple, Particles>{particles};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                accelerations_data[index] = forces_data[index] / masses_data[index];

                ::pastel::integrate::detail::update_accelerations_detail::update_accelerations_impl1<1u, order+1u>::call(
                  accelerations_data, derivative_data_array, derivative_coefficients, index);
              }
          }
        }; // struct update_accelerations<order, true, true, acceleration_index, AdditionalVectorIndexTuple>

        template <std::size_t order, std::size_t acceleration_index, typename AdditionalVectorIndexTuple>
        struct update_accelerations<order, false, true, acceleration_index, AdditionalVectorIndexTuple>
        {
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
              ::pastel::container::get< ::pastel::container::tags::nth_additional_vector<acceleration_index> >(particles, index)
                = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                    / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index);

              ::pastel::integrate::detail::update_accelerations_detail::update_accelerations_impl2<1u, order+1u, acceleration_index, AdditionalVectorIndexTuple>::call(
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
                ::pastel::container::get< ::pastel::container::tags::nth_additional_vector<acceleration_index> >(particles, index)
                  = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index)
                      / ::pastel::container::get< ::pastel::particle::tags::mass >(particles, index);

                ::pastel::integrate::detail::update_accelerations_detail::update_accelerations_impl2<1u, order+1u, acceleration_index, AdditionalVectorIndexTuple>::call(
                  particles, derivative_coefficients, index);
              }
          }
        }; // struct update_accelerations<order, false, true, acceleration_index, AdditionalVectorIndexTuple>

        template <std::size_t order, std::size_t acceleration_index, typename AdditionalVectorIndexTuple>
        struct update_accelerations<order, true, false, acceleration_index, AdditionalVectorIndexTuple>
        {
          static_assert(
            std::tuple_size<AdditionalVectorIndexTuple>::value == order,
            "Size of AdditionalVectorIndexTuple must be equal to order");

          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const accelerations_data
              = particles.template data<
                  ::pastel::container::tags::nth_additional_vector<acceleration_index> >();
            auto const derivative_data_array
              = ::pastel::integrate::detail::derivative_data_array<order, AdditionalVectorIndexTuple, Particles>{particles};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            {
              accelerations_data[index] = forces_data[index];

              ::pastel::integrate::detail::update_accelerations_detail::update_accelerations_impl1<1u, order+1u>::call(
                accelerations_data, derivative_data_array, derivative_coefficients, index);
            }
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const forces_data = particles.template data< ::pastel::particle::tags::force >();
            auto const accelerations_data
              = particles.template data<
                  ::pastel::container::tags::nth_additional_vector<acceleration_index> >();
            auto const derivative_data_array
              = ::pastel::integrate::detail::derivative_data_array<order, AdditionalVectorIndexTuple, Particles>{particles};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
              {
                accelerations_data[index] = forces_data[index];

                ::pastel::integrate::detail::update_accelerations_detail::update_accelerations_impl1<1u, order+1u>::call(
                  accelerations_data, derivative_data_array, derivative_coefficients, index);
              }
          }
        }; // struct update_accelerations<order, true, false, acceleration_index, AdditionalVectorIndexTuple>

        template <std::size_t order, std::size_t acceleration_index, typename AdditionalVectorIndexTuple>
        struct update_accelerations<order, false, false, acceleration_index, AdditionalVectorIndexTuple>
        {
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
              ::pastel::container::get< ::pastel::container::tags::nth_additional_vector<acceleration_index> >(particles, index)
                = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index);

              ::pastel::integrate::detail::update_accelerations_detail::update_accelerations_impl2<1u, order+1u, acceleration_index, AdditionalVectorIndexTuple>::call(
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
                ::pastel::container::get< ::pastel::container::tags::nth_additional_vector<acceleration_index> >(particles, index)
                  = ::pastel::container::get< ::pastel::particle::tags::force >(particles, index);

                ::pastel::integrate::detail::update_accelerations_detail::update_accelerations_impl2<1u, order+1u, acceleration_index, AdditionalVectorIndexTuple>::call(
                  particles, derivative_coefficients, index);
              }
          }
        }; // struct update_accelerations<order, false, false, acceleration_index, AdditionalVectorIndexTuple>
      } // namespace update_accelerations_detail


      // AdditionalVectorIndexTuple: indices for b(t), c(t), ..., but not for a(t) (accelerations)
      template <std::size_t order, std::size_t acceleration_index, typename AdditionalVectorIndexTuple, typename Particles, typename Time>
      inline void update_accelerations(Particles& particles, Time time_step)
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
        using update_accelerations_func
          = ::pastel::integrate::detail::update_accelerations_detail::update_accelerations<
              order, is_data_accessible, has_mass, acceleration_index, AdditionalVectorIndexTuple>;
        using mobility_tag
          = typename ::pastel::container::meta::mobility_tag_of<Particles>::type;
        update_accelerations_func::call(particles, time_step, mobility_tag{});
      }
    } // namespace detail
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_DETAIL_UPDATE_ACCELERATIONS_HPP

