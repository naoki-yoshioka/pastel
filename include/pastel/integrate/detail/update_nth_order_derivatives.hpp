#ifndef PASTEL_INTEGRATE_DETAIL_UPDATE_NTH_ORDER_ACCELERATIONS_HPP
# define PASTEL_INTEGRATE_DETAIL_UPDATE_NTH_ORDER_ACCELERATIONS_HPP

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
      namespace update_nth_order_derivatives_detail
      {
        template <std::size_t n, std::size_t index, std::size_t last>
        struct update_nth_order_derivatives_impl1
        {
          static_assert(index >= 1u, "index >= 1u");

          // derivative_data_array: 0 => b (r^3), 1 => c (r^4), 2 => d (r^5), ...
          // derivative_coefficients: 0 => dt^0, 1 => dt^1, 2 => dt^2/2, ...
          template <typename DerivativeDataArray, typename DerivativeCoefficients>
          static constexpr void call(
            DerivativeDataArray& derivative_data_array,
            DerivativeCoefficients const& derivative_coefficients,
            std::size_t particle_index)
          {
            derivative_data_array[n-3u][particle_index]
              += derivative_data_array[(n-3u)+index][particle_index]
                 * derivative_coefficients[index];

            ::pastel::integrate::detail::update_nth_order_derivatives_detail::update_nth_order_derivatives_impl1<n, index+1u, last>::call(
              derivative_data_array, derivative_coefficients, particle_index);
          }
        }; // struct update_nth_order_derivatives_impl1<n, index, last>

        template <std::size_t n, std::size_t last>
        struct update_nth_order_derivatives_impl1<n, last, last>
        {
          template <typename DerivativeDataArray, typename DerivativeCoefficients>
          static constexpr void call(DerivativeDataArray const&, DerivativeCoefficients const&, std::size_t)
          { }
        }; // struct update_nth_order_derivatives_impl1<n, last, last>

        template <std::size_t n, std::size_t index, std::size_t last, typename AdditionalVectorIndexTuple>
        struct update_nth_order_derivatives_impl2
        {
          static_assert(index >= 1u, "index >= 1u");

          static constexpr auto nth_order_derivative_index
            = std::tuple_element<n-3u, AdditionalVectorIndexTuple>::type::value;
          static constexpr auto derivative_index
            = std::tuple_element<(n-3u)+index, AdditionalVectorIndexTuple>::type::value;

          // derivative_data_array: 0 => b (r^3), 1 => c (r^4), 2 => d (r^5), ...
          // derivative_coefficients: 0 => dt^0, 1 => dt^1, 2 => dt^2/2, ...
          template <typename Particles, typename DerivativeCoefficients>
          static constexpr void call(
            Particles& particles,
            DerivativeCoefficients const& derivative_coefficients,
            std::size_t particle_index)
          {
            ::pastel::container::get< ::pastel::container::tags::nth_additional_vector<nth_order_derivative_index> >(particles, particle_index)
              += ::pastel::container::get< ::pastel::container::tags::nth_additional_vector<derivative_index> >(particles, particle_index)
                 * derivative_coefficients[index];

            ::pastel::integrate::detail::update_nth_order_derivatives_detail::update_nth_order_derivatives_impl2<n, index+1u, last, AdditionalVectorIndexTuple>::call(
              particles, derivative_coefficients, particle_index);
          }
        }; // struct update_nth_order_derivatives_impl2<n, index, last, AdditionalVectorIndexTuple>

        template <std::size_t n, std::size_t last, typename AdditionalVectorIndexTuple>
        struct update_nth_order_derivatives_impl2<n, last, last, AdditionalVectorIndexTuple>
        {
          template <typename Particles, typename DerivativeCoefficients>
          static constexpr void call(Particles&, DerivativeCoefficients const&, std::size_t)
          { }
        }; // struct update_nth_order_derivatives_impl2<n, last, last, AdditionalVectorIndexTuple>

        // r^n_i(t+dt) = r^n_i(t) + r^{n+1}_i(t) dt + r^{n+2}_i(t) dt^2/2 + ...
        //   b_i(t) = r^3_i(t), c_i(t) = r^4_i(t), ...
        template <std::size_t n, std::size_t order, bool is_data_accessible, typename AdditionalVectorIndexTuple>
        struct update_nth_order_derivatives
        {
          static_assert(n >= 3u, "n >= 3u");
          static_assert(
            std::tuple_size<AdditionalVectorIndexTuple>::value == (n-2u)+order,
            "Size of AdditionalVectorIndexTuple must be equal to (n-2u)+order");

          template <typename Particles, typename Time>
          static void call(Particles const&, Time, ::pastel::container::mobility_tags::immobile)
          { }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const derivative_data_array
              = ::pastel::integrate::detail::derivative_data_array<(n-2u)+order, AdditionalVectorIndexTuple, Particles>{particles};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              ::pastel::integrate::detail::update_nth_order_derivatives_detail::update_nth_order_derivatives_impl1<n, 1u, order+1u>::call(
                derivative_data_array, derivative_coefficients, index);
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const derivative_data_array
              = ::pastel::integrate::detail::derivative_data_array<(n-2u)+order, AdditionalVectorIndexTuple, Particles>{particles};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                ::pastel::integrate::detail::update_nth_order_derivatives_detail::update_nth_order_derivatives_impl1<n, 1u, order+1u>::call(
                  derivative_data_array, derivative_coefficients, index);
          }
        }; // struct update_nth_order_derivatives<n, order, is_data_accessible, AdditionalVectorIndexTuple>

        template <std::size_t n, std::size_t order, typename AdditionalVectorIndexTuple>
        struct update_nth_order_derivatives<n, order, false, AdditionalVectorIndexTuple>
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
              ::pastel::integrate::detail::update_nth_order_derivatives_detail::update_nth_order_derivatives_impl2<n, 1u, order+1u, AdditionalVectorIndexTuple>::call(
                particles, derivative_coefficients, index);
          }

          template <typename Particles, typename Time>
          static void call(Particles& particles, Time time_step, ::pastel::container::mobility_tags::partially_mobile)
          {
            auto const derivative_coefficients
              = ::pastel::integrate::detail::derivative_coefficients<order, Time>{time_step};

            auto const num_particles = ::pastel::container::num_particles(particles);
            for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
              if (::pastel::container::is_mobile(particles, index))
                ::pastel::integrate::detail::update_nth_order_derivatives_detail::update_nth_order_derivatives_impl2<n, 1u, order+1u, AdditionalVectorIndexTuple>::call(
                  particles, derivative_coefficients, index);
          }
        }; // struct update_nth_order_derivatives<n, order, false, AdditionalVectorIndexTuple>
      } // namespace update_nth_order_derivatives_detail


      // AdditionalVectorIndexTuple: indices for b(t), c(t), ..., but not for a(t) (accelerations)
      template <std::size_t n, std::size_t order, typename AdditionalVectorIndexTuple, typename Particles, typename Time>
      inline void update_nth_order_derivatives(Particles& particles, Time time_step)
      {
        static_assert(
          ::pastel::utility::tuple::meta::is_tuple<AdditionalVectorIndexTuple>::value
          && ::pastel::utility::tuple::meta::all_of<
               AdditionalVectorIndexTuple, ::pastel::utility::is_integral_constant>::value,
          "AdditionalVectorIndexTuple must be a tuple whose type is std::integral_constant");

        static constexpr bool is_data_accessible
          = ::pastel::container::meta::is_data_accessible<Particles>::value;
        using update_nth_order_derivatives_func
          = ::pastel::integrate::detail::update_nth_order_derivatives_detail::update_nth_order_derivatives<
              n, order, is_data_accessible, AdditionalVectorIndexTuple>;
        using mobility_tag
          = typename ::pastel::container::meta::mobility_tag_of<Particles>::type;
        update_nth_order_derivatives_func::call(particles, time_step, mobility_tag{});
      }
    } // namespace detail
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_DETAIL_UPDATE_NTH_ORDER_ACCELERATIONS_HPP

