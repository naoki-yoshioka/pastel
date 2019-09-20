#ifndef PASTEL_NEIGHBOR_BOUNDARY_INEXCLUSIVE_CELLS_UPDATER_HPP
# define PASTEL_NEIGHBOR_BOUNDARY_INEXCLUSIVE_CELLS_UPDATER_HPP

# include <cassert>
# include <cstddef>
# include <cmath>
# include <array>
# include <vector>
# include <iterator>
# include <algorithm>
# include <functional>
# include <type_traits>
# include <memory>

# include <pastel/neighbor/inexclusive_cells_updater.hpp>
# include <pastel/neighbor/force.hpp>
# include <pastel/neighbor/meta/force_of.hpp>
# include <pastel/geometry/squared_distance.hpp>
# include <pastel/geometry/meta/dimension_of.hpp>
# include <pastel/geometry/meta/value_of.hpp>
# include <pastel/particle/tags.hpp>
# include <pastel/container/get.hpp>
# include <pastel/container/num_particles.hpp>
# include <pastel/container/maximal_speed.hpp>
# include <pastel/container/meta/is_data_accessible.hpp>
# include <pastel/container/meta/dimension_of.hpp>
# include <pastel/force/cutoff_length.hpp>
# include <pastel/force/meta/has_cutoff.hpp>
# include <pastel/utility/prod.hpp>
# include <pastel/utility/intpow.hpp>


namespace pastel
{
  namespace neighbor
  {
    namespace boundary_inexclusive_cells_updater_detail
    {
      template <typename NeighborList, typename BulkParticles, typename BoundaryParticles, typename Indices, typename Value, typename Position, typename Difference>
      inline Value update_status(
        NeighborList const& neighbor_list, BulkParticles const& bulk_particles, BoundaryParticles const& boundary_particles,
        Indices const& near_boundary_bulk_particle_indices,
        Value time_step, Value buffer_length,
        Position const& lower_bound, Position const& upper_bound, Difference const& cell_vector)
      {
        static constexpr auto dimension = ::pastel::container::meta::dimension_of<BulkParticles>::value;
        static_assert(
          dimension == ::pastel::container::meta::dimension_of<BoundaryParticles>::value
            && dimension == ::pastel::geometry::meta::dimension_of<Position>::value
            && dimension == ::pastel::geometry::meta::dimension_of<Difference>::value,
          "Dimensions of types of particles and geometric quantities must be the same.");

        return buffer_length - Value{2} * time_step * std::max(
          ::pastel::container::maximal_speed(bulk_particles, near_boundary_bulk_particle_indices),
          ::pastel::container::maximal_speed(boundary_particles));
      }


      template <bool is_data_accessible>
      struct is_near_boundary;

      template <>
      struct is_near_boundary<true>
      {
        template <typename BulkParticles, typename Index, typename Point, typename Vector>
        static bool call(
          BulkParticles const& bulk_particles, Index const index,
          Point const& lower_bound, Point const& upper_bound, Vector const& cell_vector)
        {
          auto const positions_data = bulk_particles.template data< ::pastel::particle::tags::position >();

          auto constexpr dimension = ::pastel::geometry::meta::dimension_of<Point>::value;
          static_assert(
            dimension == ::pastel::container::meta::dimension_of<BulkParticles>::value
              && dimension == ::pastel::geometry::meta::dimension_of<Vector>::value,
            "Dimensions of types of particles and geometric quantities must be the same.");

          for (auto dim = decltype(dimension){0}; dim < dimension; ++dim)
            if (positions_data[index][dim] <= lower_bound[dim] + cell_vector[dim]
                || positions_data[index][dim] >= upper_bound[dim] - cell_vector[dim])
              return true;

          return false;
        }
      }; // struct is_near_boundary<true>

      template <>
      struct is_near_boundary<false>
      {
        template <typename BulkParticles, typename Index, typename Point, typename Vector>
        static bool call(
          BulkParticles const& bulk_particles, Index const index,
          Point const& lower_bound, Point const& upper_bound, Vector const& cell_vector)
        {
          auto constexpr dimension = ::pastel::geometry::meta::dimension_of<Point>::value;
          static_assert(
            dimension == ::pastel::container::meta::dimension_of<BulkParticles>::value
              && dimension == ::pastel::geometry::meta::dimension_of<Vector>::value,
            "Dimensions of types of particles and geometric quantities must be the same.");

          for (auto dim = decltype(dimension){0}; dim < dimension; ++dim)
            if (::pastel::container::get< ::pastel::particle::tags::position >(bulk_particles, index)[dim] <= lower_bound[dim] + cell_vector[dim]
                || ::pastel::container::get< ::pastel::particle::tags::position >(bulk_particles, index)[dim] >= upper_bound[dim] - cell_vector[dim])
              return true;

          return false;
        }
      }; // struct is_near_boundary<false>


      template <bool is_data_accessible>
      struct generate_cell_index;

      template <>
      struct generate_cell_index<true>
      {
        template <typename Particles, typename Index, typename Point, typename Vector, std::size_t dimension>
        static std::size_t call(
          Particles const& particles, Index const index,
          Point const& lower_bound, Vector const& cell_vector,
          std::array<std::size_t, dimension> const& num_cells)
        {
          static_assert(::pastel::geometry::meta::dimension_of<Vector>::value == dimension, "dimension_of<Vector> must be equal to dimension");

          auto const boundary_lower_bound = lower_bound - cell_vector;

          auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
          auto const position_vector_from_boundary_lower_bound = positions_data[index] - boundary_lower_bound;

          auto result = std::size_t{0u};
          for (auto dim = std::size_t{0u}; dim < dimension; ++dim)
          {
            auto cell_coordinate
              = static_cast<std::size_t>(std::floor(position_vector_from_boundary_lower_bound[dim] / cell_vector[dim]));
            for (auto dim_ = std::size_t{0u}; dim_ < dim; ++dim_)
              cell_coordinate *= num_cells[dim_];
            result += cell_coordinate;
          }

          return result;
        }
      }; // struct generate_cell_index<true>

      template <>
      struct generate_cell_index<false>
      {
        template <typename Particles, typename Index, typename Point, typename Vector, std::size_t dimension>
        static std::size_t call(
          Particles const& particles, Index const index,
          Point const& lower_bound, Vector const& cell_vector,
          std::array<std::size_t, dimension> const& num_cells)
        {
          static_assert(::pastel::geometry::meta::dimension_of<Vector>::value == dimension, "dimension_of<Vector> must be equal to dimension");

          auto const boundary_lower_bound = lower_bound - cell_vector;

          auto const position_vector_from_boundary_lower_bound
            = ::pastel::container::get< ::pastel::particle::tags::position >(particles, index) - boundary_lower_bound;

          auto result = std::size_t{0u};
          for (auto dim = std::size_t{0u}; dim < dimension; ++dim)
          {
            auto cell_coordinate
              = static_cast<std::size_t>(std::floor(position_vector_from_boundary_lower_bound[dim] / cell_vector[dim]));
            for (auto dim_ = std::size_t{0u}; dim_ < dim; ++dim_)
              cell_coordinate *= num_cells[dim_];
            result += cell_coordinate;
          }

          return result;
        }
      }; // struct generate_cell_index<false>


      template <typename Index, std::size_t dimension_>
      inline Index cell_coordinate_to_cell_index(
        std::array<Index, dimension_> const& cell_coordinate, std::array<std::size_t, dimension_> const& num_cells)
      { return ::pastel::neighbor::inexclusive_cells_updater_detail::cell_coordinate_to_cell_index(cell_coordinate, num_cells); }

      template <std::size_t rindex, std::size_t dimension_>
      struct generate_near_boundary_cell_indices_impl
      {
        static void call(
          std::array<std::size_t, dimension_>& present_cell_coordinate,
          std::vector<std::size_t>& near_boundary_cell_indices,
          std::array<std::size_t, dimension_> const& num_cells)
        {
          constexpr auto dim = dimension_ - rindex - std::size_t{1u};
          for (present_cell_coordinate[dim] = std::size_t{0u}; present_cell_coordinate[dim] < num_cells[dim]; ++present_cell_coordinate[dim])
            ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::generate_near_boundary_cell_indices_impl<rindex+1u, dimension_>::call(
              present_cell_coordinate, near_boundary_cell_indices, num_cells);
        }
      }; // struct generate_near_boundary_cell_indices_impl<rindex, dimension_>

      template <std::size_t dimension_>
      struct generate_near_boundary_cell_indices_impl<dimension_, dimension_>
      {
        static void call(
          std::array<std::size_t, dimension_>& present_cell_coordinate,
          std::vector<std::size_t>& near_boundary_cell_indices,
          std::array<std::size_t, dimension_> const& num_cells)
        {
          for (auto dim = std::size_t{0u}; dim < dimension_; ++dim)
            if (present_cell_coordinate[dim] <= std::size_t{1} || present_cell_coordinate[dim] >= num_cells[dim] - std::size_t{2})
            {
              near_boundary_cell_indices.push_back(
                ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::cell_coordinate_to_cell_index(
                  present_cell_coordinate, num_cells));
              break;
            }
        }
      }; // struct generate_near_boundary_cell_indices_impl<dimension_, dimension_>

      struct bulk_multiplies
      {
        template <typename Value1, typename Value2>
        auto operator()(Value1&& value1, Value2&& value2) const
          -> decltype(std::forward<Value1>(value1) * (std::forward<Value2>(value2) - Value2{2}))
        { return std::forward<Value1>(value1) * (std::forward<Value2>(value2) - Value2{2}); }
      };

      template <std::size_t dimension_>
      inline std::vector<std::size_t> generate_near_boundary_cell_indices(
        std::array<std::size_t, dimension_> const& num_cells, std::size_t const total_num_cells)
      {
        std::vector<std::size_t> near_boundary_cell_indices;
        near_boundary_cell_indices.reserve(
          total_num_cells - ::pastel::utility::foldl(::pastel::neighbor::boundary_inexclusive_cells_updater_detail::bulk_multiplies{}, num_cells));

        auto present_cell_coordinate = std::array<std::size_t, dimension_>{};
        ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::generate_near_boundary_cell_indices_impl<0u, dimension_>::call(
          present_cell_coordinate, near_boundary_cell_indices, num_cells);

        return near_boundary_cell_indices;
      }


      template <typename Index, std::size_t dimension_>
      inline std::array<Index, dimension_> cell_index_to_cell_coordinate(
        Index cell_index, std::array<std::size_t, dimension_> const& num_cells)
      {
        auto result = std::array<Index, dimension_>{};

        for (auto dim = std::size_t{0u}; dim < dimension_; ++dim)
        {
          result[dim] = cell_index % num_cells[dim];
          cell_index /= num_cells[dim];
        }

        return result;
      }


      template <std::size_t dim, std::size_t dimension_>
      struct generate_neighbor_cell_indices_impl
      {
        template <std::size_t N>
        static void call(
          std::array<std::size_t, N>& possible_neighbor_cell_indices,
          std::size_t& array_size,
          std::array<std::size_t, dimension_> const& cell_coordinate,
          std::array<std::size_t, dimension_> const& num_cells)
        {
          if (cell_coordinate[dim]+1u < num_cells[dim])
          {
            auto const index_distance_to_neighbor_cell = std::accumulate(std::begin(num_cells), std::begin(num_cells)+dim, std::size_t{1u}, std::multiplies<std::size_t>{});

            for (auto index = std::size_t{0u}; index < array_size; ++index)
              possible_neighbor_cell_indices[array_size + index] = possible_neighbor_cell_indices[index] + index_distance_to_neighbor_cell;

            array_size *= std::size_t{2u};
          }

          ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::generate_neighbor_cell_indices_impl<dim+1u, dimension_>::call(
            possible_neighbor_cell_indices, array_size, cell_coordinate, num_cells);
        }
      }; // struct generate_neighbor_cell_indices_impl<dim, dimension_>

      template <std::size_t dimension_>
      struct generate_neighbor_cell_indices_impl<dimension_, dimension_>
      {
        template <std::size_t N>
        static void call(
          std::array<std::size_t, N>&,
          std::size_t&,
          std::array<std::size_t, dimension_> const&,
          std::array<std::size_t, dimension_> const&)
        { }
      }; // struct generate_neighbor_cell_indices_impl<dimension_, dimension_>

      template <typename Iterator, typename IteratorAllocator, std::size_t dimension_>
      inline void generate_neighbor_cell_indices(
        std::vector<Iterator, IteratorAllocator>& neighbor_cell_indices_firsts,
        std::vector<std::size_t> const& near_boundary_cell_indices,
        std::array<std::size_t, dimension_> const& num_cells)
      {
        auto const num_near_boundary_cells = near_boundary_cell_indices.size();

        for (auto index = decltype(num_near_boundary_cells){0u}; index < num_near_boundary_cells - decltype(num_near_boundary_cells){1u}; ++index)
        {
          auto const cell_index = near_boundary_cell_indices[index];

          auto const cell_coordinate
            = ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::cell_index_to_cell_coordinate(cell_index, num_cells);
          auto possible_neighbor_cell_indices = std::array<std::size_t, ::pastel::utility::intpow(2u, dimension_)>{cell_index};
          auto array_size = std::size_t{1u};

          ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::generate_neighbor_cell_indices_impl<0u, dimension_>::call(
            possible_neighbor_cell_indices, array_size, cell_coordinate, num_cells);

          auto const next_cell_index = near_boundary_cell_indices[index+1u];
          std::fill(
            std::begin(neighbor_cell_indices_firsts) + cell_index + 1u,
            std::begin(neighbor_cell_indices_firsts) + next_cell_index + 1u,
            std::copy(
              std::begin(possible_neighbor_cell_indices) + 1u, std::begin(possible_neighbor_cell_indices) + array_size,
              neighbor_cell_indices_firsts[cell_index]));
        }

        auto const cell_index = near_boundary_cell_indices.back();
        auto const cell_coordinate
          = ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::cell_index_to_cell_coordinate(cell_index, num_cells);
        auto possible_neighbor_cell_indices = std::array<std::size_t, ::pastel::utility::intpow(2u, dimension_)>{cell_index};
        auto array_size = std::size_t{1u};

        ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::generate_neighbor_cell_indices_impl<0u, dimension_>::call(
          possible_neighbor_cell_indices, array_size, cell_coordinate, num_cells);

        neighbor_cell_indices_firsts[cell_index+1u]
          = std::copy(
              std::begin(possible_neighbor_cell_indices) + 1u, std::begin(possible_neighbor_cell_indices) + array_size,
              neighbor_cell_indices_firsts[cell_index]);
      }


      template <
        typename Index, typename IndexAllocator, typename Iterator, typename IteratorAllocator,
        typename Point, typename Vector, std::size_t dimension_, typename BulkParticles, typename BoundaryParticles>
      inline void update_cell_data(
        std::vector<Index, IndexAllocator>& particle_indices_in_each_cell,
        std::vector<Iterator, IteratorAllocator>& particle_indices_in_each_cell_firsts,
        std::vector<Index, IndexAllocator>& particle_index_to_cell_index,
        std::vector<Index, IndexAllocator>& num_particles_in_each_cell,
        std::vector<Index>& near_boundary_bulk_particle_indices,
        Point const& lower_bound, Point const& upper_bound, Vector const& cell_vector,
        std::array<std::size_t, dimension_> const& num_cells,
        std::size_t const total_num_cells,
        BulkParticles const& bulk_particles, BoundaryParticles const& boundary_particles)
      {
        auto const num_bulk_particles = ::pastel::container::num_particles(bulk_particles);
        auto const num_boundary_particles = ::pastel::container::num_particles(boundary_particles);
        auto const total_num_particles = num_bulk_particles + num_boundary_particles;
        assert(particle_indices_in_each_cell.size() == total_num_particles);
        assert(particle_indices_in_each_cell_firsts.size() == total_num_cells+1);
        assert(particle_index_to_cell_index.size() == total_num_particles);
        assert(num_particles_in_each_cell.size() == total_num_cells);
        assert(
          std::accumulate(
            std::begin(num_cells), std::end(num_cells), 1u, std::multiplies<std::size_t>{})
          == total_num_cells);
        assert(near_boundary_bulk_particle_indices.empty());

        std::fill(std::begin(num_particles_in_each_cell), std::end(num_particles_in_each_cell), Index{0});

        static constexpr bool is_bulk_data_accessible
          = ::pastel::container::meta::is_data_accessible<BulkParticles>::value;
        for (auto particle_index = Index{0}; particle_index < num_bulk_particles; ++particle_index)
          if (::pastel::neighbor::boundary_inexclusive_cells_updater_detail::is_near_boundary<is_bulk_data_accessible>::call(
                bulk_particles, particle_index, lower_bound, upper_bound, cell_vector))
          {
            auto const cell_index
              = ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::generate_cell_index<is_bulk_data_accessible>::call(
                  bulk_particles, particle_index, lower_bound, cell_vector, num_cells);
            particle_index_to_cell_index[particle_index] = cell_index;
            ++num_particles_in_each_cell[cell_index];
            near_boundary_bulk_particle_indices.push_back(particle_index);
          }
        static constexpr bool is_boundary_data_accessible
          = ::pastel::container::meta::is_data_accessible<BoundaryParticles>::value;
        for (auto particle_index = Index{num_bulk_particles}; particle_index < total_num_particles; ++particle_index)
        {
          auto const cell_index
            = ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::generate_cell_index<is_boundary_data_accessible>::call(
                boundary_particles, particle_index-num_bulk_particles, lower_bound, cell_vector, num_cells);
          particle_index_to_cell_index[particle_index] = cell_index;
          ++num_particles_in_each_cell[cell_index];
        }

        particle_indices_in_each_cell_firsts.front() = std::begin(particle_indices_in_each_cell);
        for (auto cell_index = Index{0}; cell_index < total_num_cells; ++cell_index)
          particle_indices_in_each_cell_firsts[cell_index+Index{1}]
            = particle_indices_in_each_cell_firsts[cell_index] + num_particles_in_each_cell[cell_index];

        auto particle_indices_in_each_cell_iters = particle_indices_in_each_cell_firsts;
        for (auto particle_index: near_boundary_bulk_particle_indices)
          *particle_indices_in_each_cell_iters[particle_index_to_cell_index[particle_index]]++ = particle_index;
        for (auto particle_index = Index{num_bulk_particles}; particle_index < total_num_particles; ++particle_index)
          *particle_indices_in_each_cell_iters[particle_index_to_cell_index[particle_index]]++ = particle_index;
      }


      // Note that every cell doesn't contain both of bulk_particles and boundary_particles
      template <
        typename NeighborList, typename Size,
        typename Value, typename Index,
        typename ParticleIndicesIterator, typename Allocator1,
        typename NextCellIndicesIterator, typename Allocator2,
        typename SquaredDistanceFunction>
      inline void do_update_neighbor_list(
        NeighborList& neighbor_list, Size const num_bulk_particles,
        Value const& squared_search_length, Index const total_num_cells,
        std::vector<ParticleIndicesIterator, Allocator1> const& particle_indices_in_each_cell_firsts,
        std::vector<NextCellIndicesIterator, Allocator2> const& neighbor_cell_indices_firsts,
        std::vector<Index> const& near_boundary_cell_indices,
        SquaredDistanceFunction&& squared_distance_function)
      {
        assert(particle_indices_in_each_cell_firsts.size() == total_num_cells + Index{1});
        assert(neighbor_cell_indices_firsts.size() == total_num_cells + Index{1});

        neighbor_list.clear();

        for (auto cell_index: near_boundary_cell_indices)
        {
          auto const particle_index_first = particle_indices_in_each_cell_firsts[cell_index];
          auto const particle_index_last = particle_indices_in_each_cell_firsts[cell_index+1];
          auto const neighbor_cell_index_first = neighbor_cell_indices_firsts[cell_index];
          auto const neighbor_cell_index_last = neighbor_cell_indices_firsts[cell_index+1];

          for (auto particle_index1_iter = particle_index_first;
               particle_index1_iter != particle_index_last; ++particle_index1_iter)
          {
            auto const particle_index1 = *particle_index1_iter;
            if (particle_index1 < num_bulk_particles) // particle1 is a bulk particle
            {
              auto const bulk = particle_index1;

              for (auto neighbor_cell_index_iter = neighbor_cell_index_first;
                   neighbor_cell_index_iter != neighbor_cell_index_last; ++neighbor_cell_index_iter)
              {
                auto const neighbor_cell_index = *neighbor_cell_index_iter;
                auto const particle_index2_first = particle_indices_in_each_cell_firsts[neighbor_cell_index];
                auto const particle_index2_last = particle_indices_in_each_cell_firsts[neighbor_cell_index+1];

                for (auto particle_index2_iter = particle_index2_first;
                     particle_index2_iter != particle_index2_last; ++particle_index2_iter)
                {
                  auto const particle_index2 = *particle_index2_iter;
                  if (particle_index2 < num_bulk_particles) // particle2 must be a boundary particle
                    continue;

                  auto const boundary = particle_index2 - num_bulk_particles;
                  if (squared_distance_function(bulk, boundary) >= squared_search_length)
                    continue;
                  neighbor_list.emplace(bulk, boundary);
                }
              }
            }
            else // particle1 is a boundary particle
            {
              auto const boundary = particle_index1 - num_bulk_particles;

              for (auto neighbor_cell_index_iter = neighbor_cell_index_first;
                   neighbor_cell_index_iter != neighbor_cell_index_last; ++neighbor_cell_index_iter)
              {
                auto const neighbor_cell_index = *neighbor_cell_index_iter;
                auto const particle_index2_first = particle_indices_in_each_cell_firsts[neighbor_cell_index];
                auto const particle_index2_last = particle_indices_in_each_cell_firsts[neighbor_cell_index+1];

                for (auto particle_index2_iter = particle_index2_first;
                     particle_index2_iter != particle_index2_last; ++particle_index2_iter)
                {
                  auto const particle_index2 = *particle_index2_iter;
                  if (particle_index2 >= num_bulk_particles) // particle2 must be a bulk particle
                    continue;

                  auto const bulk = particle_index2;
                  if (squared_distance_function(bulk, boundary) >= squared_search_length)
                    continue;
                  neighbor_list.emplace(bulk, boundary);
                }
              }
            }
          }
        }
      }

      template <bool is_bulk_data_accessible, bool is_boundary_data_accessible>
      struct update_neighbor_list;

      template <>
      struct update_neighbor_list<true, true>
      {
        template <
          typename NeighborList, typename BulkParticles, typename BoundaryParticles,
          typename Value, typename Index,
          typename ParticleIndicesIterator, typename Allocator1,
          typename NextCellIndicesIterator, typename Allocator2>
        static void call(
          NeighborList& neighbor_list, BulkParticles const& bulk_particles, BoundaryParticles const& boundary_particles,
          Value const& squared_search_length, Index const total_num_cells,
          std::vector<ParticleIndicesIterator, Allocator1> const& particle_indices_in_each_cell_firsts,
          std::vector<NextCellIndicesIterator, Allocator2> const& neighbor_cell_indices_firsts,
          std::vector<Index> const& near_boundary_cell_indices)
        {
          auto const bulk_positions_data = bulk_particles.template data< ::pastel::particle::tags::position >();
          auto const boundary_positions_data = boundary_particles.template data< ::pastel::particle::tags::position >();
          ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::do_update_neighbor_list(
            neighbor_list, ::pastel::container::num_particles(bulk_particles),
            squared_search_length, total_num_cells,
            particle_indices_in_each_cell_firsts, neighbor_cell_indices_firsts, near_boundary_cell_indices,
            [bulk_positions_data, boundary_positions_data](Index const bulk, Index const boundary)
            { return ::pastel::geometry::squared_distance(bulk_positions_data[bulk], boundary_positions_data[boundary]); });
        }
      }; // struct update_neighbor_list<true, true>

      template <>
      struct update_neighbor_list<true, false>
      {
        template <
          typename NeighborList, typename BulkParticles, typename BoundaryParticles,
          typename Value, typename Index,
          typename ParticleIndicesIterator, typename Allocator1,
          typename NextCellIndicesIterator, typename Allocator2>
        static void call(
          NeighborList& neighbor_list, BulkParticles const& bulk_particles, BoundaryParticles const& boundary_particles,
          Value const& squared_search_length, Index const total_num_cells,
          std::vector<ParticleIndicesIterator, Allocator1> const& particle_indices_in_each_cell_firsts,
          std::vector<NextCellIndicesIterator, Allocator2> const& neighbor_cell_indices_firsts,
          std::vector<Index> const& near_boundary_cell_indices)
        {
          auto const bulk_positions_data = bulk_particles.template data< ::pastel::particle::tags::position >();
          ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::do_update_neighbor_list(
            neighbor_list, ::pastel::container::num_particles(bulk_particles),
            squared_search_length, total_num_cells,
            particle_indices_in_each_cell_firsts, neighbor_cell_indices_firsts, near_boundary_cell_indices,
            [bulk_positions_data, &boundary_particles](Index const bulk, Index const boundary)
            {
              return ::pastel::geometry::squared_distance(
                bulk_positions_data[bulk],
                ::pastel::container::get< ::pastel::particle::tags::position >(boundary_particles, boundary));
            });
        }
      }; // struct update_neighbor_list<true, false>

      template <>
      struct update_neighbor_list<false, true>
      {
        template <
          typename NeighborList, typename BulkParticles, typename BoundaryParticles,
          typename Value, typename Index,
          typename ParticleIndicesIterator, typename Allocator1,
          typename NextCellIndicesIterator, typename Allocator2>
        static void call(
          NeighborList& neighbor_list, BulkParticles const& bulk_particles, BoundaryParticles const& boundary_particles,
          Value const& squared_search_length, Index const total_num_cells,
          std::vector<ParticleIndicesIterator, Allocator1> const& particle_indices_in_each_cell_firsts,
          std::vector<NextCellIndicesIterator, Allocator2> const& neighbor_cell_indices_firsts,
          std::vector<Index> const& near_boundary_cell_indices)
        {
          auto const boundary_positions_data = boundary_particles.template data< ::pastel::particle::tags::position >();
          ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::do_update_neighbor_list(
            neighbor_list, ::pastel::container::num_particles(bulk_particles),
            squared_search_length, total_num_cells,
            particle_indices_in_each_cell_firsts, neighbor_cell_indices_firsts, near_boundary_cell_indices,
            [&bulk_particles, boundary_positions_data](Index const bulk, Index const boundary)
            {
              return ::pastel::geometry::squared_distance(
                ::pastel::container::get< ::pastel::particle::tags::position >(bulk_particles, bulk),
                boundary_positions_data[boundary]);
            });
        }
      }; // struct update_neighbor_list<false, true>

      template <>
      struct update_neighbor_list<false, false>
      {
        template <
          typename NeighborList, typename BulkParticles, typename BoundaryParticles,
          typename Value, typename Index,
          typename ParticleIndicesIterator, typename Allocator1,
          typename NextCellIndicesIterator, typename Allocator2>
        static void call(
          NeighborList& neighbor_list, BulkParticles const& bulk_particles, BoundaryParticles const& boundary_particles,
          Value const& squared_search_length, Index const total_num_cells,
          std::vector<ParticleIndicesIterator, Allocator1> const& particle_indices_in_each_cell_firsts,
          std::vector<NextCellIndicesIterator, Allocator2> const& neighbor_cell_indices_firsts,
          std::vector<Index> const& near_boundary_cell_indices)
        {
          ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::do_update_neighbor_list(
            neighbor_list, ::pastel::container::num_particles(bulk_particles),
            squared_search_length, total_num_cells,
            particle_indices_in_each_cell_firsts, neighbor_cell_indices_firsts, near_boundary_cell_indices,
            [&bulk_particles, &boundary_particles](Index const bulk, Index const boundary)
            {
              return ::pastel::geometry::squared_distance(
                ::pastel::container::get< ::pastel::particle::tags::position >(bulk_particles, bulk),
                ::pastel::container::get< ::pastel::particle::tags::position >(boundary_particles, boundary));
            });
        }
      }; // struct update_neighbor_list<false, false>
    } // namespace boundary_inexclusive_cells_updater_detail


    template <typename Point>
    class boundary_inexclusive_cells_updater
    {
     public:
      using point_type = Point;
      using vector_type = decltype(std::declval<Point>() - std::declval<Point>());
      static constexpr std::size_t dimension = ::pastel::geometry::meta::dimension_of<point_type>::value;
      using value_type = typename ::pastel::geometry::meta::value_of<point_type>::type;
      using boundary_updater_type = boundary_inexclusive_cells_updater;

     private:
      value_type search_length_;
      value_type squared_search_length_;
      value_type buffer_length_;

      point_type lower_bound_;
      point_type upper_bound_;
      std::array<std::size_t, dimension> num_cells_;
      std::size_t total_num_cells_;
      vector_type cell_vector_;

      using indices_type = std::vector<std::size_t>;
      indices_type particle_indices_in_each_cell_; // particle_indices_in_each_cell_.size() == num_particles
      std::vector<typename indices_type::iterator> particle_indices_in_each_cell_firsts_; // particle_indices_in_each_cell_firsts_.size() == total_num_cells+1
      indices_type particle_index_to_cell_index_; // particle_index_to_cell_index_.size() == num_particles
      indices_type num_particles_in_each_cell_; // num_particles_in_each_cell_.size() == total_num_cells

      indices_type near_boundary_bulk_particle_indices_;

      indices_type neighbor_cell_indices_; // neighbor_cell_indices_.size() <= total_num_cells * (intpow(2, dimension) - 1)
      std::vector<typename indices_type::iterator> neighbor_cell_indices_firsts_; // neighbor_cell_indices_firsts_.size() == total_num_cells+1
      indices_type near_boundary_cell_indices_; // near_boundary_cell_indices_.size() == total_num_cells_ - std::accumulate(std::begin(num_cells_), std::end(num_cells_), 1, [](auto partial, auto value) { return partial * (value - 2); });


     public:
      boundary_inexclusive_cells_updater()
        : search_length_{value_type{1}},
          squared_search_length_{search_length_ * search_length_},
          buffer_length_{value_type{-1}},
          lower_bound_{},
          upper_bound_{},
          num_cells_{},
          total_num_cells_{},
          cell_vector_{},
          particle_indices_in_each_cell_{},
          particle_indices_in_each_cell_firsts_(1u, particle_indices_in_each_cell_.begin()),
          particle_index_to_cell_index_{},
          num_particles_in_each_cell_{},
          near_boundary_bulk_particle_indices_{},
          neighbor_cell_indices_{},
          neighbor_cell_indices_firsts_(1u, neighbor_cell_indices_.begin()),
          near_boundary_cell_indices_{}
      { }

      boundary_inexclusive_cells_updater(::pastel::neighbor::inexclusive_cells_updater<Point> const& bulk_updater)
        : search_length_{bulk_updater.search_length_},
          squared_search_length_{bulk_updater.squared_search_length_},
          buffer_length_{bulk_updater.buffer_length_},
          lower_bound_{bulk_updater.lower_bound_},
          upper_bound_{bulk_updater.upper_bound_},
          num_cells_{generate_boundary_num_cells(bulk_updater)},
          total_num_cells_{::pastel::utility::prod(num_cells_)},
          cell_vector_{bulk_updater.cell_vector_},
          particle_indices_in_each_cell_{},
          particle_indices_in_each_cell_firsts_(total_num_cells_+1u, particle_indices_in_each_cell_.begin()),
          particle_index_to_cell_index_{},
          num_particles_in_each_cell_(total_num_cells_, 0u),
          near_boundary_bulk_particle_indices_{},
          neighbor_cell_indices_((::pastel::utility::intpow(2u, dimension) - 1u) * total_num_cells_),
          neighbor_cell_indices_firsts_(total_num_cells_+1u, neighbor_cell_indices_.begin()),
          near_boundary_cell_indices_{
            ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::generate_near_boundary_cell_indices(
              num_cells_, total_num_cells_)}
      {
        ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::generate_neighbor_cell_indices(
          neighbor_cell_indices_firsts_, near_boundary_cell_indices_, num_cells_);
        neighbor_cell_indices_.resize(neighbor_cell_indices_firsts_.back() - neighbor_cell_indices_firsts_.front());
      }

     private:
      std::array<std::size_t, dimension> generate_boundary_num_cells(
        ::pastel::neighbor::inexclusive_cells_updater<Point> const& bulk_updater)
      {
        auto result = std::array<std::size_t, dimension>{};
        for (std::size_t dim = 0u; dim < dimension; ++dim)
          result[dim] = bulk_updater.num_cells_[dim] + 2u;

        return result;
      }

     public:
      value_type const& search_length() const { return search_length_; }
      void search_length(value_type new_search_length)
      {
        buffer_length_ += new_search_length - search_length_;
        search_length_ = new_search_length;
        squared_search_length_ = search_length_ * search_length_;
      }

      value_type const& buffer_length() const { return buffer_length_; }

      bool is_valid() const { return buffer_length_ > value_type{0}; }
      bool is_invalid() const { return !this->is_valid(); }


      // key_particles is always bulk_particles, and partner_particles is always boundary_particles.
      // See pastel::system::initialize_detail::reset_boundary_status in pastel/system/initialize.hpp
      template <typename NeighborList, typename BulkParticles, typename BoundaryParticles, typename Time>
      void reset_status(NeighborList const& neighbor_list, BulkParticles const& bulk_particles, BoundaryParticles const& boundary_particles, Time time_step)
      {
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList const>::type>::value,
          "Force of NeighborList must have cutoff");

        buffer_length_
          = ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::update_status(
              neighbor_list, bulk_particles, boundary_particles, near_boundary_bulk_particle_indices_, time_step,
              search_length_ - ::pastel::force::cutoff_length(::pastel::neighbor::force(neighbor_list)),
              lower_bound_, upper_bound_, cell_vector_);
      }


      // key_particles is always bulk_particles, and partner_particles is always boundary_particles.
      // See pastel::integrate::detail::update_detail::update_boundary_status in pastel/integrate/detail/update.hpp
      template <typename NeighborList, typename BulkParticles, typename BoundaryParticles, typename Time>
      void update_status(NeighborList const& neighbor_list, BulkParticles const& bulk_particles, BoundaryParticles const& boundary_particles, Time time_step)
      {
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList const>::type>::value,
          "Force of NeighborList must have cutoff");

        buffer_length_
          = ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::update_status(
              neighbor_list, bulk_particles, boundary_particles, near_boundary_bulk_particle_indices_, time_step, buffer_length_,
              lower_bound_, upper_bound_, cell_vector_);
      }


      // key_particles is always bulk_particles, and partner_particles is always boundary_particles.
      // See pastel::system::initialize_detail::update_boundary_neighbor_list in pastel/system/initialize.hpp
      template <typename NeighborList, typename BulkParticles, typename BoundaryParticles>
      void update_neighbor_list(NeighborList& neighbor_list, BulkParticles const& bulk_particles, BoundaryParticles const& boundary_particles)
      {
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList>::type>::value,
          "Force of NeighborList must have cutoff");

        auto const num_particles
          = ::pastel::container::num_particles(bulk_particles) + ::pastel::container::num_particles(boundary_particles);
        particle_indices_in_each_cell_.resize(num_particles);
        particle_index_to_cell_index_.resize(num_particles);
        near_boundary_bulk_particle_indices_.clear();
        near_boundary_bulk_particle_indices_.reserve(::pastel::container::num_particles(bulk_particles));
        ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::update_cell_data(
          particle_indices_in_each_cell_, particle_indices_in_each_cell_firsts_,
          particle_index_to_cell_index_, num_particles_in_each_cell_,
          near_boundary_bulk_particle_indices_,
          lower_bound_, upper_bound_, cell_vector_, num_cells_, total_num_cells_,
          bulk_particles, boundary_particles);

        static constexpr bool is_bulk_data_accessible
          = ::pastel::container::meta::is_data_accessible<BulkParticles>::value;
        static constexpr bool is_boundary_data_accessible
          = ::pastel::container::meta::is_data_accessible<BoundaryParticles>::value;
        using update_neighbor_list_func
          = ::pastel::neighbor::boundary_inexclusive_cells_updater_detail::update_neighbor_list<is_bulk_data_accessible, is_boundary_data_accessible>;
        update_neighbor_list_func::call(
          neighbor_list, bulk_particles, boundary_particles, squared_search_length_, total_num_cells_,
          particle_indices_in_each_cell_firsts_, neighbor_cell_indices_firsts_, near_boundary_cell_indices_);
      }
    }; // class boundary_inexclusive_cells_updater<Cell, CellAllocator>
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_BOUNDARY_INEXCLUSIVE_CELLS_UPDATER_HPP

