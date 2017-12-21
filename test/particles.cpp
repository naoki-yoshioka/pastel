#define BOOST_TEST_MODULE particles test
#include <boost/test/included/unit_test.hpp>

#include <iterator>

#include <pastel/container/particles.hpp>
#include <pastel/container/aligned_particles.hpp>
#include <pastel/container/num_particles.hpp>
#include <pastel/container/get.hpp>
#include <pastel/container/clear_forces.hpp>
#include <pastel/container/apply_external_forces.hpp>
#include <pastel/container/increase_force.hpp>
#include <pastel/container/decrease_force.hpp>
#include <pastel/container/mobility_tags.hpp>
#include <pastel/container/meta/point_of.hpp>
#include <pastel/container/meta/vector_of.hpp>
#include <pastel/container/meta/scalar_of.hpp>
#include <pastel/container/meta/dimension_of.hpp>
#include <pastel/container/meta/is_data_accessible.hpp>
#include <pastel/container/meta/num_additional_vectors_of.hpp>
#include <pastel/container/meta/num_additional_scalars_of.hpp>
//#include <pastel/particle/particle.hpp>
//#include <pastel/particle/particle_io.hpp>
#include <pastel/particle/tags.hpp>
#include <pastel/geometry/vector_io.hpp>
#include <pastel/geometry/point_io.hpp>
#include <pastel/geometry/meta/dimension_of.hpp>
#include <pastel/extforce/constant_force.hpp>


template <typename Particles, std::size_t num_additional_vectors, std::size_t num_additional_scalars>
void test()
{
  constexpr auto num_particles = 10u;
  auto particles1 = Particles{};
  particles1.reserve(num_particles);

  using point_type = typename pastel::container::meta::point_of<Particles>::type;
  using vector_type = typename pastel::container::meta::vector_of<Particles>::type;
  using scalar_type = typename pastel::container::meta::scalar_of<Particles>::type;
  constexpr auto initial_velocity = vector_type{1.0, 0.0, 0.0};
  constexpr auto initial_force = vector_type{1.0, 0.0, 0.0};
  constexpr auto initial_mass = scalar_type{1.0};
  constexpr auto initial_diameter = scalar_type{1.0};
  for (auto index = 0u; index < num_particles; ++index)
    particles1.emplace_back(
      point_type{1.0 * index + 0.5, 1.0, 1.0},
      initial_velocity, initial_force, initial_mass, initial_diameter);

  BOOST_TEST(pastel::container::meta::dimension_of<Particles>::value == pastel::geometry::meta::dimension_of<point_type>::value);
  BOOST_TEST(pastel::container::meta::dimension_of<Particles>::value == pastel::geometry::meta::dimension_of<vector_type>::value);
  BOOST_TEST(pastel::container::meta::is_data_accessible<Particles>::value);
  BOOST_TEST(pastel::container::meta::num_additional_vectors_of<Particles>::value == num_additional_vectors);
  BOOST_TEST(pastel::container::meta::num_additional_scalars_of<Particles>::value == num_additional_scalars);

  BOOST_TEST(pastel::container::num_particles(particles1) == num_particles);
  BOOST_TEST((pastel::container::get<pastel::particle::tags::position>(particles1, 0u) == point_type{0.5, 1.0, 1.0}));
  BOOST_TEST(pastel::container::get<pastel::particle::tags::velocity>(particles1, 0u) == initial_velocity);
  BOOST_TEST(pastel::container::get<pastel::particle::tags::force>(particles1, 0u) == initial_force);
  BOOST_TEST(pastel::container::get<pastel::particle::tags::mass>(particles1, 0u) == initial_mass);
  BOOST_TEST(pastel::container::get<pastel::particle::tags::diameter>(particles1, 0u) == initial_diameter);

  pastel::container::clear_forces(particles1);
  BOOST_TEST((pastel::container::get<pastel::particle::tags::force>(particles1, 0u) == vector_type{0.0, 0.0, 0.0}));

  pastel::container::apply_external_forces(particles1, pastel::extforce::constant_force<vector_type>{initial_force});
  BOOST_TEST(pastel::container::get<pastel::particle::tags::force>(particles1, 0u) == initial_force);

  pastel::container::clear_forces(particles1);
  pastel::container::increase_force(particles1, 0u, vector_type{2.0, 0.0, 0.0});
  BOOST_TEST((pastel::container::get<pastel::particle::tags::force>(particles1, 0u) == vector_type{2.0, 0.0, 0.0}));

  pastel::container::decrease_force(particles1, 0u, vector_type{1.0, 0.0, 0.0});
  BOOST_TEST((pastel::container::get<pastel::particle::tags::force>(particles1, 0u) == vector_type{1.0, 0.0, 0.0}));


  auto particles2 = Particles{std::begin(particles1), std::end(particles1)};
  BOOST_TEST((particles2.template front<pastel::particle::tags::position>() == point_type{0.5, 1.0, 1.0}));
  auto particles3 = Particles{particles2.rbegin(), particles2.rend()};
  BOOST_TEST((particles3.template back<pastel::particle::tags::position>() == point_type{0.5, 1.0, 1.0}));
}

template <typename Particles, std::size_t num_additional_vectors, std::size_t num_additional_scalars>
void immobile_test()
{
  auto const num_particles = 10u;
  auto particles1 = Particles{};
  particles1.reserve(num_particles);

  using point_type = typename pastel::container::meta::point_of<Particles>::type;
  using vector_type = typename pastel::container::meta::vector_of<Particles>::type;
  using scalar_type = typename pastel::container::meta::scalar_of<Particles>::type;
  auto const initial_velocity = vector_type{1.0, 0.0, 0.0};
  auto const initial_force = vector_type{1.0, 0.0, 0.0};
  auto const initial_mass = scalar_type{1.0};
  auto const initial_diameter = scalar_type{1.0};
  for (auto index = 0u; index < num_particles; ++index)
    particles1.emplace_back(
      point_type{1.0 * index + 0.5, 1.0, 1.0},
      initial_velocity, initial_force, initial_mass, initial_diameter);

  BOOST_TEST(pastel::container::meta::dimension_of<Particles>::value == pastel::geometry::meta::dimension_of<point_type>::value);
  BOOST_TEST(pastel::container::meta::dimension_of<Particles>::value == pastel::geometry::meta::dimension_of<vector_type>::value);
  BOOST_TEST(pastel::container::meta::is_data_accessible<Particles>::value);
  BOOST_TEST(pastel::container::meta::num_additional_vectors_of<Particles>::value == num_additional_vectors);
  BOOST_TEST(pastel::container::meta::num_additional_scalars_of<Particles>::value == num_additional_scalars);

  BOOST_TEST(pastel::container::num_particles(particles1) == num_particles);
  BOOST_TEST((pastel::container::get<pastel::particle::tags::position>(particles1, 0u) == point_type{0.5, 1.0, 1.0}));
  BOOST_TEST(pastel::container::get<pastel::particle::tags::velocity>(particles1, 0u) == initial_velocity);
  BOOST_TEST(pastel::container::get<pastel::particle::tags::force>(particles1, 0u) == initial_force);
  BOOST_TEST(pastel::container::get<pastel::particle::tags::mass>(particles1, 0u) == initial_mass);
  BOOST_TEST(pastel::container::get<pastel::particle::tags::diameter>(particles1, 0u) == initial_diameter);

  pastel::container::clear_forces(particles1);
  BOOST_TEST((pastel::container::get<pastel::particle::tags::force>(particles1, 0u) == vector_type{0.0, 0.0, 0.0}));

  pastel::container::apply_external_forces(particles1, pastel::extforce::constant_force<vector_type>{initial_force});
  BOOST_TEST((pastel::container::get<pastel::particle::tags::force>(particles1, 0u) == vector_type{0.0, 0.0, 0.0}));

  pastel::container::clear_forces(particles1);
  pastel::container::increase_force(particles1, 0u, vector_type{2.0, 0.0, 0.0});
  BOOST_TEST((pastel::container::get<pastel::particle::tags::force>(particles1, 0u) == vector_type{0.0, 0.0, 0.0}));

  pastel::container::decrease_force(particles1, 0u, vector_type{1.0, 0.0, 0.0});
  BOOST_TEST((pastel::container::get<pastel::particle::tags::force>(particles1, 0u) == vector_type{0.0, 0.0, 0.0}));


  auto particles2 = Particles{std::begin(particles1), std::end(particles1)};
  BOOST_TEST((particles2.template front<pastel::particle::tags::position>() == point_type{0.5, 1.0, 1.0}));
  auto particles3 = Particles{particles2.rbegin(), particles2.rend()};
  BOOST_TEST((particles3.template back<pastel::particle::tags::position>() == point_type{0.5, 1.0, 1.0}));
}

BOOST_AUTO_TEST_CASE(particles_test, * boost::unit_test::tolerance(0.000001))
{ test<pastel::container::particles<3u, pastel::container::mobility_tags::mobile, 1u, 0u>, 1u, 0u>(); }

BOOST_AUTO_TEST_CASE(aligned_particles_test, * boost::unit_test::tolerance(0.000001))
{ test<pastel::container::aligned_particles<3u, pastel::container::mobility_tags::mobile, 1u, 0u>, 1u, 0u>(); }

BOOST_AUTO_TEST_CASE(immobile_particles_test, * boost::unit_test::tolerance(0.000001))
{ immobile_test<pastel::container::particles<3u, pastel::container::mobility_tags::immobile, 1u, 0u>, 1u, 0u>(); }

BOOST_AUTO_TEST_CASE(immobile_aligned_particles_test, * boost::unit_test::tolerance(0.000001))
{ immobile_test<pastel::container::aligned_particles<3u, pastel::container::mobility_tags::immobile, 1u, 0u>, 1u, 0u>(); }

