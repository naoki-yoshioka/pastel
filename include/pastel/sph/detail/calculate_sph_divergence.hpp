#ifndef PASTEL_SPH_DETAIL_CALCULATE_SPH_DIVERGENCE_HPP
# define PASTEL_SPH_DETAIL_CALCULATE_SPH_DIVERGENCE_HPP

# include <cmath>

# include <pastel/force/squared_cutoff_length.hpp>
# include <pastel/geometry/squared_norm.hpp>
# include <pastel/geometry/dot_product.hpp>


namespace pastel
{
  namespace sph
  {
    namespace detail
    {
      template <typename Point, typename Vector, typename Scalar, typename Force>
      Scalar calculate_sph_divergence(
        Point const& position1, Vector const& velocity1, Scalar const& mass1,
        Point const& position2, Vector const& velocity2, Scalar const& mass2,
        Force const& force)
      {
        auto const difference = position1 - position2;
        auto const squared_distance = ::pastel::geometry::squared_norm(difference);

        if (squared_distance > ::pastel::force::squared_cutoff_length(force))
          return Scalar{};

        using std::sqrt;
        auto const distance = sqrt(squared_distance);

        return mass1 * mass2 / distance * force.kernel().derivative(distance)
          * ::pastel::geometry::dot_product(velocity1 - velocity2, difference);
      }

      template <typename Point, typename Vector, typename Scalar, typename Force>
      Scalar calculate_sph_divergence(
        Point const& position1, Vector const& velocity1,
        Point const& position2, Vector const& velocity2, Scalar const& mass2,
        Force const& force)
      {
        auto const difference = position1 - position2;
        auto const squared_distance = ::pastel::geometry::squared_norm(difference);

        if (squared_distance > ::pastel::force::squared_cutoff_length(force))
          return Scalar{};

        using std::sqrt;
        auto const distance = sqrt(squared_distance);

        return mass2 / distance * force.kernel().derivative(distance)
          * ::pastel::geometry::dot_product(velocity1 - velocity2, difference);
      }

      template <typename Point, typename Vector, typename Scalar, typename Force>
      Scalar calculate_sph_divergence(
        Point const& position1, Vector const& velocity1, Scalar const& mass1,
        Point const& position2, Vector const& velocity2,
        Force const& force)
      {
        auto const difference = position1 - position2;
        auto const squared_distance = ::pastel::geometry::squared_norm(difference);

        if (squared_distance > ::pastel::force::squared_cutoff_length(force))
          return Scalar{};

        using std::sqrt;
        auto const distance = sqrt(squared_distance);

        return mass1 / distance * force.kernel().derivative(distance)
          * ::pastel::geometry::dot_product(velocity1 - velocity2, difference);
      }

      template <typename Point, typename Vector, typename Scalar, typename Force>
      Scalar calculate_sph_divergence(
        Point const& position1, Vector const& velocity1,
        Point const& position2, Vector const& velocity2,
        Force const& force)
      {
        auto const difference = position1 - position2;
        auto const squared_distance = ::pastel::geometry::squared_norm(difference);

        if (squared_distance > ::pastel::force::squared_cutoff_length(force))
          return Scalar{};

        using std::sqrt;
        auto const distance = sqrt(squared_distance);

        return force.kernel().derivative(distance) / distance
          * ::pastel::geometry::dot_product(velocity1 - velocity2, difference);
      }
    } // namespace detail
  } // namespace sph
} // namespace pastel


#endif // PASTEL_SPH_DETAIL_CALCULATE_SPH_DIVERGENCE_HPP

