#ifndef PASTEL_INTEGRATE_GEAR_PREDICT_PARTICLES_HPP
# define PASTEL_INTEGRATE_GEAR_PREDICT_PARTICLES_HPP

# include <cstddef>
# include <tuple>

# include <pastel/integrate/detail/update_positions.hpp>
# include <pastel/integrate/detail/update_velocities.hpp>
# include <pastel/integrate/detail/update_accelerations.hpp>
# include <pastel/integrate/detail/update_nth_order_derivatives.hpp>
//# include <pastel/integrate/detail/update_orientations_order1.hpp>
//# include <pastel/integrate/detail/update_local_angular_velocities_order1.hpp>
# include <pastel/system/for_each_container.hpp>
//# include <pastel/container/modify_global_angular_velocities.hpp>


namespace pastel
{
  namespace integrate
  {
    namespace gear
    {
      namespace predict_particles_detail
      {
        template <std::size_t order, typename IntegrationVectorIndexTuple, typename Time>
        struct update_positions
        {
         private:
          Time time_step_;

         public:
          update_positions(Time time_step) : time_step_{time_step} { }

          template <typename Particles, typename ExternalForce>
          void operator()(Particles& particles, ExternalForce&&) const
          { ::pastel::integrate::detail::update_positions<order, IntegrationVectorIndexTuple>(particles, time_step_); }
        }; // struct update_positions<order, IntegrationVectorIndexTuple, Time>


        template <std::size_t order, typename IntegrationVectorIndexTuple, typename Time>
        struct update_velocities
        {
         private:
          Time time_step_;

         public:
          update_velocities(Time time_step) : time_step_{time_step} { }

          template <typename Particles, typename ExternalForce>
          void operator()(Particles& particles, ExternalForce&&) const
          { ::pastel::integrate::detail::update_velocities<order, IntegrationVectorIndexTuple>(particles, time_step_); }
        }; // struct update_velocities<order, IntegrationVectorIndexTuple, Time>


        template <std::size_t order, std::size_t acceleration_index, typename IntegrationVectorIndexTuple, typename Time>
        struct update_accelerations
        {
         private:
          Time time_step_;

         public:
          update_accelerations(Time time_step) : time_step_{time_step} { }

          template <typename Particles, typename ExternalForce>
          void operator()(Particles& particles, ExternalForce&&) const
          { ::pastel::integrate::detail::update_accelerations<order, acceleration_index, IntegrationVectorIndexTuple>(particles, time_step_); }
        }; // struct update_accelerations<order, acceleration_index, IntegrationVectorIndexTuple, Time>


        template <std::size_t n, std::size_t order, typename IntegrationVectorIndexTuple, typename Time>
        struct update_nth_order_derivatives
        {
         private:
          Time time_step_;

         public:
          update_nth_order_derivatives(Time time_step) : time_step_{time_step} { }

          template <typename Particles, typename ExternalForce>
          void operator()(Particles& particles, ExternalForce&&) const
          { ::pastel::integrate::detail::update_nth_order_derivatives<n, order, IntegrationVectorIndexTuple>(particles, time_step_); }
        }; // struct update_nth_order_derivatives<index, IntegrationVectorIndexTuple, Time>



        template <std::size_t order, std::size_t acceleration_index, typename IntegrationVectorIndexTuple>
        struct predict_particles;

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
        // x^p_i(t+dt) =       x_i(t) +         v_i(t) dt + [f_i(t) / m_i] (dt^2/2) + b_i(t) (dt^3/6)
        // v^p_i(t+dt) =       v_i(t) + [f_i(t) / m_i] dt +         b_i(t) (dt^2/2)
        // a^p_i(t+dt) = f_i(t) / m_i +         b_i(t) dt +
        // b^p_i(t+dt) =       b_i(t) +
        template <std::size_t acceleration_index, typename IntegrationVectorIndexTuple>
        struct predict_particles<4u, acceleration_index, IntegrationVectorIndexTuple>
        {
          template <typename System, typename Time>
          static void call(System& system, Time time_step)
          {
            ::pastel::system::for_each_container(
              system,
              ::pastel::integrate::gear::predict_particles_detail::update_positions<3u, IntegrationVectorIndexTuple, Time>{time_step});
            // update_orientations
            ::pastel::system::for_each_container(
              system,
              ::pastel::integrate::gear::predict_particles_detail::update_velocities<2u, IntegrationVectorIndexTuple, Time>{time_step});
            // update_local_angular_velocities, modify_global_angular_velocities
            ::pastel::system::for_each_container(
              system,
              ::pastel::integrate::gear::predict_particles_detail::update_accelerations<1u, acceleration_index, IntegrationVectorIndexTuple, Time>{time_step});
            // update_angular_accelerations
          }
        }; // struct predict_particles<4u, acceleration_index, IntegrationVectorIndexTuple>


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
        // x^p_i(t+dt) =       x_i(t) +         v_i(t) dt + [f_i(t) / m_i] (dt^2/2) + b_i(t) (dt^3/6) + c_i(t) (dt^4/24)
        // v^p_i(t+dt) =       v_i(t) + [f_i(t) / m_i] dt +         b_i(t) (dt^2/2) + c_i(t) (dt^3/6)
        // a^p_i(t+dt) = f_i(t) / m_i +         b_i(t) dt +         c_i(t) (dt^2/2)
        // b^p_i(t+dt) =       b_i(t) +         c_i(t) dt
        // c^p_i(t+dt) =       c_i(t)
        template <std::size_t acceleration_index, typename IntegrationVectorIndexTuple>
        struct predict_particles<5u, acceleration_index, IntegrationVectorIndexTuple>
        {
          template <typename System, typename Time>
          static void call(System& system, Time time_step)
          {
            ::pastel::system::for_each_container(
              system,
              ::pastel::integrate::gear::predict_particles_detail::update_positions<4u, IntegrationVectorIndexTuple, Time>{time_step});
            // update_orientations
            ::pastel::system::for_each_container(
              system,
              ::pastel::integrate::gear::predict_particles_detail::update_velocities<3u, IntegrationVectorIndexTuple, Time>{time_step});
            // update_local_angular_velocities, modify_global_angular_velocities
            ::pastel::system::for_each_container(
              system,
              ::pastel::integrate::gear::predict_particles_detail::update_accelerations<2u, acceleration_index, IntegrationVectorIndexTuple, Time>{time_step});
            // update_angular_accelerations
            ::pastel::system::for_each_container(
              system,
              ::pastel::integrate::gear::predict_particles_detail::update_nth_order_derivatives<3u, 1u, IntegrationVectorIndexTuple, Time>{time_step});
            // update_angular_integration_vectors<??>
          }
        }; // struct predict_particles<5u, acceleration_index, IntegrationVectorIndexTuple>


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
        // x^p_i(t+dt) =       x_i(t) +         v_i(t) dt + [f_i(t) / m_i] (dt^2/2) + b_i(t) (dt^3/6) + c_i(t) (dt^4/24) + d_i(t) (dt^5/120)
        // v^p_i(t+dt) =       v_i(t) + [f_i(t) / m_i] dt +         b_i(t) (dt^2/2) + c_i(t) (dt^3/6) + d_i(t) (dt^4/24)
        // a^p_i(t+dt) = f_i(t) / m_i +         b_i(t) dt +         c_i(t) (dt^2/2) + d_i(t) (dt^3/6)
        // b^p_i(t+dt) =       b_i(t) +         c_i(t) dt +         d_i(t) (dt^2/2)
        // c^p_i(t+dt) =       c_i(t) +         d_i(t) dt
        // d^p_i(t+dt) =       d_i(t)
        template <std::size_t acceleration_index, typename IntegrationVectorIndexTuple>
        struct predict_particles<6u, acceleration_index, IntegrationVectorIndexTuple>
        {
          template <typename System, typename Time>
          static void call(System& system, Time time_step)
          {
            ::pastel::system::for_each_container(
              system,
              ::pastel::integrate::gear::predict_particles_detail::update_positions<5u, IntegrationVectorIndexTuple, Time>{time_step});
            // update_orientations
            ::pastel::system::for_each_container(
              system,
              ::pastel::integrate::gear::predict_particles_detail::update_velocities<4u, IntegrationVectorIndexTuple, Time>{time_step});
            // update_local_angular_velocities, modify_global_angular_velocities
            ::pastel::system::for_each_container(
              system,
              ::pastel::integrate::gear::predict_particles_detail::update_accelerations<3u, acceleration_index, IntegrationVectorIndexTuple, Time>{time_step});
            // update_angular_accelerations
            ::pastel::system::for_each_container(
              system,
              ::pastel::integrate::gear::predict_particles_detail::update_nth_order_derivatives<3u, 2u, IntegrationVectorIndexTuple, Time>{time_step});
            // update_angular_integration_vectors<??>
            ::pastel::system::for_each_container(
              system,
              ::pastel::integrate::gear::predict_particles_detail::update_nth_order_derivatives<4u, 1u, IntegrationVectorIndexTuple, Time>{time_step});
            // update_angular_integration_vectors<??>
          }
        }; // struct predict_particles<6u, acceleration_index, IntegrationVectorIndexTuple>
      } // namespace predict_particles_detail


      // IntegrationVectorIndexTuple: indices for b(t), c(t), ..., but not for a(t) (accelerations)
      template <std::size_t order, std::size_t acceleration_index, typename IntegrationVectorIndexTuple, typename System, typename Time>
      inline void predict_particles(System& system, Time time_step)
      {
        static_assert(order >= 4u && order <= 6u, "order must satisfy 4u <= order <= 6u");
        using predict_particles_func
          = ::pastel::integrate::gear::predict_particles_detail::predict_particles<
              order, acceleration_index, IntegrationVectorIndexTuple>;
        predict_particles_func::call(system, time_step);
      }
    } // namespace gear
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_GEAR_PREDICT_PARTICLES_HPP

