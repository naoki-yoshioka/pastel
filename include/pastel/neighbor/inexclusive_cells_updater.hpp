#ifndef PASTEL_NEIGHBOR_INEXCLUSIVE_CELLS_UPDATER_HPP
# define PASTEL_NEIGHBOR_INEXCLUSIVE_CELLS_UPDATER_HPP

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

# include <pastel/neighbor/force.hpp>
# include <pastel/neighbor/meta/force_of.hpp>
# include <pastel/neighbor/meta/interaction_pair_of.hpp>
# include <pastel/neighbor/detail/update_status.hpp>
# include <pastel/geometry/squared_distance.hpp>
# include <pastel/geometry/meta/dimension_of.hpp>
# include <pastel/geometry/meta/value_of.hpp>
# include <pastel/particle/tags.hpp>
# include <pastel/container/get.hpp>
# include <pastel/container/num_particles.hpp>
# include <pastel/container/maximal_speed.hpp>
# include <pastel/container/meta/is_data_accessible.hpp>
# include <pastel/force/cutoff_length.hpp>
# include <pastel/force/meta/has_cutoff.hpp>
# include <pastel/utility/prod.hpp>
# include <pastel/utility/intpow.hpp>


namespace pastel
{
  namespace neighbor
  {
    namespace inexclusive_cells_updater_detail
    {
      template <typename Vector, std::size_t dimension_>
      inline Vector generate_cell_vector(Vector system_vector, std::array<std::size_t, dimension_> const& num_cells)
      {
        static_assert(::pastel::geometry::meta::dimension_of<Vector>::value == dimension_, "dimension_of<Vector> must be equal to dimension_");

        for (auto dim = std::size_t{0u}; dim < dimension_; ++dim)
          system_vector[dim] /= num_cells[dim];

        return system_vector;
      }


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

          auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
          auto const position_vector_from_lower_bound = positions_data[index] - lower_bound;

          auto result = std::size_t{0u};
          for (auto dim = std::size_t{0u}; dim < dimension; ++dim)
          {
            auto cell_coordinate
              = static_cast<std::size_t>(std::floor(position_vector_from_lower_bound[dim] / cell_vector[dim]));
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

          auto const position_vector_from_lower_bound
            = ::pastel::container::get< ::pastel::particle::tags::position >(particles, index) - lower_bound;

          auto result = std::size_t{0u};
          for (auto dim = std::size_t{0u}; dim < dimension; ++dim)
          {
            auto cell_coordinate
              = static_cast<std::size_t>(std::floor(position_vector_from_lower_bound[dim] / cell_vector[dim]));
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
      {
        auto result = Index{0u};

        for (auto dim = std::size_t{0u}; dim < dimension_; ++dim)
        {
          auto nth_dim_cell_coordinate = cell_coordinate[dim];
          for (auto dim_ = std::size_t{0u}; dim_ < dim; ++dim_)
            nth_dim_cell_coordinate *= num_cells[dim_];
          result += nth_dim_cell_coordinate;
        }

        return result;
      }


      template <std::size_t dim, std::size_t dimension_>
      struct generate_neighbor_cell_indices_impl2
      {
        template <std::size_t N>
        static void call(
          std::array<std::size_t, N>& possible_neighbor_cell_indices,
          std::size_t& array_size,
          std::array<std::size_t, dimension_> const& present_cell_coordinate,
          std::array<std::size_t, dimension_> const& num_cells)
        {
          if (present_cell_coordinate[dim]+1u < num_cells[dim])
          {
            auto const index_distance_to_neighbor_cell = std::accumulate(std::begin(num_cells), std::begin(num_cells)+dim, std::size_t{1u}, std::multiplies<std::size_t>{});

            for (auto index = std::size_t{0u}; index < array_size; ++index)
              possible_neighbor_cell_indices[array_size + index] = possible_neighbor_cell_indices[index] + index_distance_to_neighbor_cell;

            array_size *= std::size_t{2u};
          }

          ::pastel::neighbor::inexclusive_cells_updater_detail::generate_neighbor_cell_indices_impl2<dim+1u, dimension_>::call(
            possible_neighbor_cell_indices, array_size, present_cell_coordinate, num_cells);
        }
      }; // struct generate_neighbor_cell_indices_impl2<dim, dimension_>

      template <std::size_t dimension_>
      struct generate_neighbor_cell_indices_impl2<dimension_, dimension_>
      {
        template <std::size_t N>
        static void call(
          std::array<std::size_t, N>&,
          std::size_t&,
          std::array<std::size_t, dimension_> const&,
          std::array<std::size_t, dimension_> const&)
        { }
      }; // struct generate_neighbor_cell_indices_impl2<dimension_, dimension_>

      template <std::size_t rindex, std::size_t dimension_>
      struct generate_neighbor_cell_indices_impl
      {
        template <typename Iterator, typename IteratorAllocator>
        static void call(
          std::array<std::size_t, dimension_>& present_cell_coordinate,
          std::vector<Iterator, IteratorAllocator>& neighbor_cell_indices_firsts,
          std::array<std::size_t, dimension_> const& num_cells)
        {
          constexpr auto dim = dimension_ - rindex - std::size_t{1u};
          for (present_cell_coordinate[dim] = std::size_t{0u}; present_cell_coordinate[dim] < num_cells[dim]; ++present_cell_coordinate[dim])
            ::pastel::neighbor::inexclusive_cells_updater_detail::generate_neighbor_cell_indices_impl<rindex+1u, dimension_>::call(
              present_cell_coordinate, neighbor_cell_indices_firsts, num_cells);
        }
      }; // struct generate_neighbor_cell_indices_impl<rindex, dimension_>

      template <std::size_t dimension_>
      struct generate_neighbor_cell_indices_impl<dimension_, dimension_>
      {
        template <typename Iterator, typename IteratorAllocator>
        static void call(
          std::array<std::size_t, dimension_>& present_cell_coordinate,
          std::vector<Iterator, IteratorAllocator>& neighbor_cell_indices_firsts,
          std::array<std::size_t, dimension_> const& num_cells)
        {
          auto const present_cell_index
            = ::pastel::neighbor::inexclusive_cells_updater_detail::cell_coordinate_to_cell_index(present_cell_coordinate, num_cells);
          auto possible_neighbor_cell_indices = std::array<std::size_t, ::pastel::utility::intpow(2u, dimension_)>{present_cell_index};
          auto array_size = std::size_t{1u};

          ::pastel::neighbor::inexclusive_cells_updater_detail::generate_neighbor_cell_indices_impl2<0u, dimension_>::call(
            possible_neighbor_cell_indices, array_size, present_cell_coordinate, num_cells);

          neighbor_cell_indices_firsts[present_cell_index+1u]
            = std::copy(
                std::begin(possible_neighbor_cell_indices) + 1u, std::begin(possible_neighbor_cell_indices) + array_size,
                neighbor_cell_indices_firsts[present_cell_index]);
        }
      }; // struct generate_neighbor_cell_indices_impl<dimension_, dimension_>

      template <typename Iterator, typename IteratorAllocator, std::size_t dimension_>
      inline void generate_neighbor_cell_indices(
        std::vector<Iterator, IteratorAllocator>& neighbor_cell_indices_firsts,
        std::array<std::size_t, dimension_> const& num_cells)
      {
        auto present_cell_coordinate = std::array<std::size_t, dimension_>{};
        ::pastel::neighbor::inexclusive_cells_updater_detail::generate_neighbor_cell_indices_impl<0u, dimension_>::call(
          present_cell_coordinate, neighbor_cell_indices_firsts, num_cells);
      }


      template <
        typename Index, typename IndexAllocator, typename Iterator, typename IteratorAllocator,
        typename Point, typename Vector, std::size_t dimension_, typename Particles>
      inline void update_cell_data(
        std::vector<Index, IndexAllocator>& particle_indices_in_each_cell,
        std::vector<Iterator, IteratorAllocator>& particle_indices_in_each_cell_firsts,
        std::vector<Index, IndexAllocator>& particle_index_to_cell_index,
        std::vector<Index, IndexAllocator>& num_particles_in_each_cell,
        Point const& lower_bound, Vector const& cell_vector,
        std::array<std::size_t, dimension_> const& num_cells,
        std::size_t const total_num_cells,
        Particles const& particles)
      {
        auto const num_particles = ::pastel::container::num_particles(particles);
        assert(particle_indices_in_each_cell.size() == num_particles);
        assert(particle_indices_in_each_cell_firsts.size() == total_num_cells+1);
        assert(particle_index_to_cell_index.size() == num_particles);
        assert(num_particles_in_each_cell.size() == total_num_cells);
        assert(
          std::accumulate(
            std::begin(num_cells), std::end(num_cells), 1u, std::multiplies<std::size_t>{})
          == total_num_cells);

        std::fill(std::begin(num_particles_in_each_cell), std::end(num_particles_in_each_cell), Index{0});

        static constexpr bool is_data_accessible
          = ::pastel::container::meta::is_data_accessible<Particles>::value;
        for (auto particle_index = Index{0}; particle_index < num_particles; ++particle_index)
        {
          auto const cell_index
            = ::pastel::neighbor::inexclusive_cells_updater_detail::generate_cell_index<is_data_accessible>::call(
                particles, particle_index, lower_bound, cell_vector, num_cells);
          particle_index_to_cell_index[particle_index] = cell_index;
          ++num_particles_in_each_cell[cell_index];
        }

        particle_indices_in_each_cell_firsts.front() = std::begin(particle_indices_in_each_cell);
        for (auto cell_index = Index{0}; cell_index < total_num_cells; ++cell_index)
          particle_indices_in_each_cell_firsts[cell_index+Index{1}]
            = particle_indices_in_each_cell_firsts[cell_index] + num_particles_in_each_cell[cell_index];
        assert(particle_indices_in_each_cell_firsts.back() == std::end(particle_indices_in_each_cell));

        auto particle_indices_in_each_cell_iters = particle_indices_in_each_cell_firsts;
        for (auto particle_index = Index{0}; particle_index < num_particles; ++particle_index)
          *particle_indices_in_each_cell_iters[particle_index_to_cell_index[particle_index]]++ = particle_index;
      }

      template <
        typename Index, typename IndexAllocator, typename Iterator, typename IteratorAllocator,
        typename Point, typename Vector, std::size_t dimension_, typename Particles1, typename Particles2>
      inline void update_cell_data(
        std::vector<Index, IndexAllocator>& particle_indices_in_each_cell,
        std::vector<Iterator, IteratorAllocator>& particle_indices_in_each_cell_firsts,
        std::vector<Index, IndexAllocator>& particle_index_to_cell_index,
        std::vector<Index, IndexAllocator>& num_particles_in_each_cell,
        Point const& lower_bound, Vector const& cell_vector,
        std::array<std::size_t, dimension_> const& num_cells,
        std::size_t const total_num_cells,
        Particles1 const& particles1, Particles2 const& particles2)
      {
        auto const num_particles1 = ::pastel::container::num_particles(particles1);
        auto const num_particles2 = ::pastel::container::num_particles(particles2);
        auto const total_num_particles = num_particles1 + num_particles2;
        assert(particle_indices_in_each_cell.size() == total_num_particles);
        assert(particle_indices_in_each_cell_firsts.size() == total_num_cells+1);
        assert(particle_index_to_cell_index.size() == total_num_particles);
        assert(num_particles_in_each_cell.size() == total_num_cells);
        assert(
          std::accumulate(
            std::begin(num_cells), std::end(num_cells), 1u, std::multiplies<std::size_t>{})
          == total_num_cells);

        std::fill(std::begin(num_particles_in_each_cell), std::end(num_particles_in_each_cell), Index{0});

        static constexpr bool is_data_accessible1
          = ::pastel::container::meta::is_data_accessible<Particles1>::value;
        for (auto particle_index = Index{0}; particle_index < num_particles1; ++particle_index)
        {
          auto const cell_index
            = ::pastel::neighbor::inexclusive_cells_updater_detail::generate_cell_index<is_data_accessible1>::call(
                particles1, particle_index, lower_bound, cell_vector, num_cells);
          particle_index_to_cell_index[particle_index] = cell_index;
          ++num_particles_in_each_cell[cell_index];
        }
        static constexpr bool is_data_accessible2
          = ::pastel::container::meta::is_data_accessible<Particles2>::value;
        for (auto particle_index = Index{num_particles1}; particle_index < total_num_particles; ++particle_index)
        {
          auto const cell_index
            = ::pastel::neighbor::inexclusive_cells_updater_detail::generate_cell_index<is_data_accessible2>::call(
                particles2, particle_index-num_particles1, lower_bound, cell_vector, num_cells);
          particle_index_to_cell_index[particle_index] = cell_index;
          ++num_particles_in_each_cell[cell_index];
        }

        particle_indices_in_each_cell_firsts.front() = std::begin(particle_indices_in_each_cell);
        for (auto cell_index = Index{0}; cell_index < total_num_cells; ++cell_index)
          particle_indices_in_each_cell_firsts[cell_index+Index{1}]
            = particle_indices_in_each_cell_firsts[cell_index] + num_particles_in_each_cell[cell_index];
        assert(particle_indices_in_each_cell_firsts.back() == std::end(particle_indices_in_each_cell));

        auto particle_indices_in_each_cell_iters = particle_indices_in_each_cell_firsts;
        for (auto particle_index = Index{0}; particle_index < total_num_particles; ++particle_index)
          *particle_indices_in_each_cell_iters[particle_index_to_cell_index[particle_index]]++ = particle_index;
      }


      template <
        typename NeighborList, typename Value, typename Index,
        typename ParticleIndicesIterator, typename Allocator1,
        typename NextCellIndicesIterator, typename Allocator2,
        typename SquaredDistanceFunction>
      inline void do_update_neighbor_list1(
        NeighborList& neighbor_list,
        Value const& squared_search_length, Index const total_num_cells,
        std::vector<ParticleIndicesIterator, Allocator1> const& particle_indices_in_each_cell_firsts,
        std::vector<NextCellIndicesIterator, Allocator2> const& neighbor_cell_indices_firsts,
        SquaredDistanceFunction&& squared_distance_function)
      {
        assert(particle_indices_in_each_cell_firsts.size() == total_num_cells + Index{1});
        assert(neighbor_cell_indices_firsts.size() == total_num_cells + Index{1});

        neighbor_list.clear();

        for (auto cell_index = Index{0}; cell_index < total_num_cells; ++cell_index)
        {
          auto const particle_index_first = particle_indices_in_each_cell_firsts[cell_index];
          auto const particle_index_last = particle_indices_in_each_cell_firsts[cell_index+1];
          auto const neighbor_cell_index_first = neighbor_cell_indices_firsts[cell_index];
          auto const neighbor_cell_index_last = neighbor_cell_indices_firsts[cell_index+1];

          for (auto particle_index1_iter = particle_index_first; particle_index1_iter != particle_index_last; ++particle_index1_iter)
          {
            auto const particle_index1 = *particle_index1_iter;
            for (auto particle_index2_iter = std::next(particle_index1_iter); particle_index2_iter != particle_index_last; ++particle_index2_iter)
            {
              auto const particle_index2 = *particle_index2_iter;
              if (squared_distance_function(particle_index1, particle_index2) >= squared_search_length)
                continue;
              auto const key_partner = std::minmax(particle_index1, particle_index2);
              neighbor_list.emplace(key_partner.first, key_partner.second);
            }

            for (auto neighbor_cell_index_iter = neighbor_cell_index_first;
                 neighbor_cell_index_iter != neighbor_cell_index_last; ++neighbor_cell_index_iter)
            {
              auto const neighbor_cell_index = *neighbor_cell_index_iter;
              auto const particle_index2_first = particle_indices_in_each_cell_firsts[neighbor_cell_index];
              auto const particle_index2_last = particle_indices_in_each_cell_firsts[neighbor_cell_index+1];

              for (auto particle_index2_iter = particle_index2_first; particle_index2_iter != particle_index2_last; ++particle_index2_iter)
              {
                auto const particle_index2 = *particle_index2_iter;
                if (squared_distance_function(particle_index1, particle_index2) >= squared_search_length)
                  continue;
                auto const key_partner = std::minmax(particle_index1, particle_index2);
                neighbor_list.emplace(key_partner.first, key_partner.second);
              }
            }
          }
        }
      }

      template <bool is_data_accessible>
      struct update_neighbor_list1;

      template <>
      struct update_neighbor_list1<true>
      {
        template <
          typename NeighborList, typename Particles, typename Value, typename Index,
          typename ParticleIndicesIterator, typename Allocator1,
          typename NextCellIndicesIterator, typename Allocator2>
        static void call(
          NeighborList& neighbor_list, Particles const& particles,
          Value const& squared_search_length, Index const total_num_cells,
          std::vector<ParticleIndicesIterator, Allocator1> const& particle_indices_in_each_cell_firsts,
          std::vector<NextCellIndicesIterator, Allocator2> const& neighbor_cell_indices_firsts)
        {
          auto const positions_data = particles.template data< ::pastel::particle::tags::position >();
          ::pastel::neighbor::inexclusive_cells_updater_detail::do_update_neighbor_list1(
            neighbor_list, squared_search_length, total_num_cells,
            particle_indices_in_each_cell_firsts, neighbor_cell_indices_firsts,
            [positions_data](Index const particle_index1, Index const particle_index2)
            { return ::pastel::geometry::squared_distance(positions_data[particle_index1], positions_data[particle_index2]); });
        }
      }; // struct update_neighbor_list1<true>

      template <>
      struct update_neighbor_list1<false>
      {
        template <
          typename NeighborList, typename Particles, typename Value, typename Index,
          typename ParticleIndicesIterator, typename Allocator1,
          typename NextCellIndicesIterator, typename Allocator2>
        static void call(
          NeighborList& neighbor_list, Particles const& particles,
          Value const& squared_search_length, Index const total_num_cells,
          std::vector<ParticleIndicesIterator, Allocator1> const& particle_indices_in_each_cell_firsts,
          std::vector<NextCellIndicesIterator, Allocator2> const& neighbor_cell_indices_firsts)
        {
          ::pastel::neighbor::inexclusive_cells_updater_detail::do_update_neighbor_list1(
            neighbor_list, squared_search_length, total_num_cells,
            particle_indices_in_each_cell_firsts, neighbor_cell_indices_firsts,
            [&particles](Index const particle_index1, Index const particle_index2)
            {
              return ::pastel::geometry::squared_distance(
                ::pastel::container::get< ::pastel::particle::tags::position >(particles, particle_index1),
                ::pastel::container::get< ::pastel::particle::tags::position >(particles, particle_index2));
            });
        }
      }; // struct update_neighbor_list1<false>


      template <
        typename NeighborList, typename Size,
        typename Value, typename Index,
        typename ParticleIndicesIterator, typename Allocator1,
        typename NextCellIndicesIterator, typename Allocator2,
        typename SquaredDistanceFunction>
      inline void do_update_neighbor_list2(
        NeighborList& neighbor_list, Size const num_key_particles,
        Value const& squared_search_length, Index const total_num_cells,
        std::vector<ParticleIndicesIterator, Allocator1> const& particle_indices_in_each_cell_firsts,
        std::vector<NextCellIndicesIterator, Allocator2> const& neighbor_cell_indices_firsts,
        SquaredDistanceFunction&& squared_distance_function)
      {
        assert(particle_indices_in_each_cell_firsts.size() == total_num_cells + Index{1});
        assert(neighbor_cell_indices_firsts.size() == total_num_cells + Index{1});

        neighbor_list.clear();

        for (auto cell_index = Index{0}; cell_index < total_num_cells; ++cell_index)
        {
          auto const particle_index_first = particle_indices_in_each_cell_firsts[cell_index];
          auto const particle_index_last = particle_indices_in_each_cell_firsts[cell_index+1];
          auto const neighbor_cell_index_first = neighbor_cell_indices_firsts[cell_index];
          auto const neighbor_cell_index_last = neighbor_cell_indices_firsts[cell_index+1];

          for (auto particle_index1_iter = particle_index_first;
               particle_index1_iter != particle_index_last; ++particle_index1_iter)
          {
            auto const particle_index1 = *particle_index1_iter;
            if (particle_index1 < num_key_particles) // particle1 is a key
            {
              auto const key = particle_index1;

              for (auto particle_index2_iter = std::next(particle_index1_iter);
                   particle_index2_iter != particle_index_last; ++particle_index2_iter)
              {
                auto const particle_index2 = *particle_index2_iter;
                if (particle_index2 < num_key_particles) // particle2 must be a partner
                  continue;

                auto const partner = particle_index2 - num_key_particles;
                if (squared_distance_function(key, partner) >= squared_search_length)
                  continue;
                neighbor_list.emplace(key, partner);
              }

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
                  if (particle_index2 < num_key_particles) // particle2 must be a partner
                    continue;

                  auto const partner = particle_index2 - num_key_particles;
                  if (squared_distance_function(key, partner) >= squared_search_length)
                    continue;
                  neighbor_list.emplace(key, partner);
                }
              }
            }
            else // particle1 is a partner
            {
              auto const partner = particle_index1 - num_key_particles;

              for (auto particle_index2_iter = std::next(particle_index1_iter);
                   particle_index2_iter != particle_index_last; ++particle_index2_iter)
              {
                auto const particle_index2 = *particle_index2_iter;
                if (particle_index2 >= num_key_particles) // particle2 must be a key
                  continue;

                auto const key = particle_index2;
                if (squared_distance_function(key, partner) >= squared_search_length)
                  continue;
                neighbor_list.emplace(key, partner);
              }

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
                  if (particle_index2 >= num_key_particles) // particle2 must be a key
                    continue;

                  auto const key = particle_index2;
                  if (squared_distance_function(key, partner) >= squared_search_length)
                    continue;
                  neighbor_list.emplace(key, partner);
                }
              }
            }
          }
        }
      }

      template <bool is_key_data_accessible, bool is_partner_data_accessible>
      struct update_neighbor_list2;

      template <>
      struct update_neighbor_list2<true, true>
      {
        template <
          typename NeighborList, typename KeyParticles, typename PartnerParticles,
          typename Value, typename Index,
          typename ParticleIndicesIterator, typename Allocator1,
          typename NextCellIndicesIterator, typename Allocator2>
        static void call(
          NeighborList& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles,
          Value const& squared_search_length, Index const total_num_cells,
          std::vector<ParticleIndicesIterator, Allocator1> const& particle_indices_in_each_cell_firsts,
          std::vector<NextCellIndicesIterator, Allocator2> const& neighbor_cell_indices_firsts)
        {
          auto const key_positions_data = key_particles.template data< ::pastel::particle::tags::position >();
          auto const partner_positions_data = partner_particles.template data< ::pastel::particle::tags::position >();
          ::pastel::neighbor::inexclusive_cells_updater_detail::do_update_neighbor_list2(
            neighbor_list, ::pastel::container::num_particles(key_particles),
            squared_search_length, total_num_cells,
            particle_indices_in_each_cell_firsts, neighbor_cell_indices_firsts,
            [key_positions_data, partner_positions_data](Index const key, Index const partner)
            { return ::pastel::geometry::squared_distance(key_positions_data[key], partner_positions_data[partner]); });
        }
      }; // struct update_neighbor_list2<true, true>

      template <>
      struct update_neighbor_list2<true, false>
      {
        template <
          typename NeighborList, typename KeyParticles, typename PartnerParticles,
          typename Value, typename Index,
          typename ParticleIndicesIterator, typename Allocator1,
          typename NextCellIndicesIterator, typename Allocator2>
        static void call(
          NeighborList& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles,
          Value const& squared_search_length, Index const total_num_cells,
          std::vector<ParticleIndicesIterator, Allocator1> const& particle_indices_in_each_cell_firsts,
          std::vector<NextCellIndicesIterator, Allocator2> const& neighbor_cell_indices_firsts)
        {
          auto const key_positions_data = key_particles.template data< ::pastel::particle::tags::position >();
          ::pastel::neighbor::inexclusive_cells_updater_detail::do_update_neighbor_list2(
            neighbor_list, ::pastel::container::num_particles(key_particles),
            squared_search_length, total_num_cells,
            particle_indices_in_each_cell_firsts, neighbor_cell_indices_firsts,
            [key_positions_data, &partner_particles](Index const key, Index const partner)
            {
              return ::pastel::geometry::squared_distance(
                key_positions_data[key],
                ::pastel::container::get< ::pastel::particle::tags::position >(partner_particles, partner));
            });
        }
      }; // struct update_neighbor_list2<true, false>

      template <>
      struct update_neighbor_list2<false, true>
      {
        template <
          typename NeighborList, typename KeyParticles, typename PartnerParticles,
          typename Value, typename Index,
          typename ParticleIndicesIterator, typename Allocator1,
          typename NextCellIndicesIterator, typename Allocator2>
        static void call(
          NeighborList& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles,
          Value const& squared_search_length, Index const total_num_cells,
          std::vector<ParticleIndicesIterator, Allocator1> const& particle_indices_in_each_cell_firsts,
          std::vector<NextCellIndicesIterator, Allocator2> const& neighbor_cell_indices_firsts)
        {
          auto const partner_positions_data = partner_particles.template data< ::pastel::particle::tags::position >();
          ::pastel::neighbor::inexclusive_cells_updater_detail::do_update_neighbor_list2(
            neighbor_list, ::pastel::container::num_particles(key_particles),
            squared_search_length, total_num_cells,
            particle_indices_in_each_cell_firsts, neighbor_cell_indices_firsts,
            [&key_particles, partner_positions_data](Index const key, Index const partner)
            {
              return ::pastel::geometry::squared_distance(
                ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key),
                partner_positions_data[partner]);
            });
        }
      }; // struct update_neighbor_list2<false, true>

      template <>
      struct update_neighbor_list2<false, false>
      {
        template <
          typename NeighborList, typename KeyParticles, typename PartnerParticles,
          typename Value, typename Index,
          typename ParticleIndicesIterator, typename Allocator1,
          typename NextCellIndicesIterator, typename Allocator2>
        static void call(
          NeighborList& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles,
          Value const& squared_search_length, Index const total_num_cells,
          std::vector<ParticleIndicesIterator, Allocator1> const& particle_indices_in_each_cell_firsts,
          std::vector<NextCellIndicesIterator, Allocator2> const& neighbor_cell_indices_firsts)
        {
          ::pastel::neighbor::inexclusive_cells_updater_detail::do_update_neighbor_list2(
            neighbor_list, ::pastel::container::num_particles(key_particles),
            squared_search_length, total_num_cells,
            particle_indices_in_each_cell_firsts, neighbor_cell_indices_firsts,
            [&key_particles, &partner_particles](Index const key, Index const partner)
            {
              return ::pastel::geometry::squared_distance(
                ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key),
                ::pastel::container::get< ::pastel::particle::tags::position >(partner_particles, partner));
            });
        }
      }; // struct update_neighbor_list2<false, false>
    } // namespace inexclusive_cells_updater_detail


    template <typename Point>
    class boundary_inexclusive_cells_updater;

    template <typename Point>
    class inexclusive_cells_updater
    {
      friend class ::pastel::neighbor::boundary_inexclusive_cells_updater<Point>;

     public:
      using point_type = Point;
      using vector_type = decltype(std::declval<Point>() - std::declval<Point>());
      static constexpr std::size_t dimension = ::pastel::geometry::meta::dimension_of<point_type>::value;
      using value_type = typename ::pastel::geometry::meta::value_of<point_type>::type;
      using boundary_updater_type = ::pastel::neighbor::boundary_inexclusive_cells_updater<Point>;

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

      indices_type neighbor_cell_indices_; // neighbor_cell_indices_.size() <= total_num_cells * (intpow(2, dimension) - 1)
      std::vector<typename indices_type::iterator> neighbor_cell_indices_firsts_; // neighbor_cell_indices_firsts_.size() == total_num_cells+1

     public:
      inexclusive_cells_updater()
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
          neighbor_cell_indices_{},
          neighbor_cell_indices_firsts_(1u, neighbor_cell_indices_.begin())
      { }

      template <typename Time, typename Speed, typename... NumCells>
      inexclusive_cells_updater(
        value_type search_length, value_type cutoff_length, Time time_step, Speed maximal_speed,
        point_type const& lower_bound, point_type const& upper_bound, NumCells const... num_cells)
        : search_length_{search_length},
          squared_search_length_{search_length * search_length},
          buffer_length_{search_length - cutoff_length - value_type{2} * time_step * maximal_speed},
          lower_bound_{lower_bound},
          upper_bound_{upper_bound},
          num_cells_{num_cells...},
          total_num_cells_{::pastel::utility::prod(num_cells...)},
          cell_vector_{
            ::pastel::neighbor::inexclusive_cells_updater_detail::generate_cell_vector(
                upper_bound-lower_bound, num_cells_)},
          particle_indices_in_each_cell_{},
          particle_indices_in_each_cell_firsts_(total_num_cells_+1u, particle_indices_in_each_cell_.begin()),
          particle_index_to_cell_index_{},
          num_particles_in_each_cell_(total_num_cells_, 0u),
          neighbor_cell_indices_((::pastel::utility::intpow(2u, dimension) - 1u) * total_num_cells_),
          neighbor_cell_indices_firsts_(total_num_cells_+1u, neighbor_cell_indices_.begin())
      {
        static_assert(sizeof...(NumCells) == dimension, "The number of num_cells arguments must be equal to dimension");
        assert(search_length > cutoff_length && cutoff_length > value_type{0});
        assert(time_step > Time{0});
        assert(maximal_speed > Speed{0});
        assert(lower_bound_ < upper_bound_);
        assert(std::all_of(std::begin(cell_vector_), std::end(cell_vector_), [search_length](value_type const& cell_length) { return cell_length >= search_length; }));

        ::pastel::neighbor::inexclusive_cells_updater_detail::generate_neighbor_cell_indices(neighbor_cell_indices_firsts_, num_cells_);
        neighbor_cell_indices_.resize(neighbor_cell_indices_firsts_.back() - neighbor_cell_indices_firsts_.front());
      }


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


      template <typename NeighborList, typename Particles, typename Time>
      void reset_status(NeighborList const& neighbor_list, Particles const& particles, Time time_step)
      {
        using interaction_pair_type
          = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
        static_assert(interaction_pair_type::first == interaction_pair_type::second, "NeighborList must be intraparticle");
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList const>::type>::value,
          "Force of NeighborList must have cutoff");

        buffer_length_
          = ::pastel::neighbor::detail::update_status(
              neighbor_list, particles, time_step,
              search_length_ - ::pastel::force::cutoff_length(::pastel::neighbor::force(neighbor_list)));
      }

      template <typename NeighborList, typename KeyParticles, typename PartnerParticles, typename Time>
      void reset_status(NeighborList const& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles, Time time_step)
      {
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList const>::type>::value,
          "Force of NeighborList must have cutoff");

        buffer_length_
          = ::pastel::neighbor::detail::update_status(
              neighbor_list, key_particles, partner_particles, time_step,
              search_length_ - ::pastel::force::cutoff_length(::pastel::neighbor::force(neighbor_list)));
      }


      template <typename NeighborList, typename Particles, typename Time>
      void update_status(NeighborList const& neighbor_list, Particles const& particles, Time time_step)
      {
        using interaction_pair_type
          = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
        static_assert(interaction_pair_type::first == interaction_pair_type::second, "NeighborList must be intraparticle");
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList const>::type>::value,
          "Force of NeighborList must have cutoff");

        buffer_length_ = ::pastel::neighbor::detail::update_status(neighbor_list, particles, time_step, buffer_length_);
      }

      template <typename NeighborList, typename KeyParticles, typename PartnerParticles, typename Time>
      void update_status(NeighborList const& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles, Time time_step)
      {
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList const>::type>::value,
          "Force of NeighborList must have cutoff");

        buffer_length_ = ::pastel::neighbor::detail::update_status(neighbor_list, key_particles, partner_particles, time_step, buffer_length_);
      }


      template <typename NeighborList, typename Particles>
      void update_neighbor_list(NeighborList& neighbor_list, Particles const& particles)
      {
        using interaction_pair_type
          = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
        static_assert(interaction_pair_type::first == interaction_pair_type::second, "NeighborList must be intraparticle");
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList>::type>::value,
          "Force of NeighborList must have cutoff");

        auto const num_particles = ::pastel::container::num_particles(particles);
        particle_indices_in_each_cell_.resize(num_particles);
        particle_index_to_cell_index_.resize(num_particles);
        ::pastel::neighbor::inexclusive_cells_updater_detail::update_cell_data(
          particle_indices_in_each_cell_, particle_indices_in_each_cell_firsts_,
          particle_index_to_cell_index_, num_particles_in_each_cell_,
          lower_bound_, cell_vector_, num_cells_, total_num_cells_,
          particles);

        static constexpr bool is_data_accessible
          = ::pastel::container::meta::is_data_accessible<Particles>::value;
        using update_neighbor_list_func
          = ::pastel::neighbor::inexclusive_cells_updater_detail::update_neighbor_list1<is_data_accessible>;
        update_neighbor_list_func::call(
          neighbor_list, particles, squared_search_length_, total_num_cells_,
          particle_indices_in_each_cell_firsts_, neighbor_cell_indices_firsts_);
      }

      template <typename NeighborList, typename KeyParticles, typename PartnerParticles>
      void update_neighbor_list(NeighborList& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles)
      {
        static_assert(
          ::pastel::force::meta::has_cutoff<typename ::pastel::neighbor::meta::force_of<NeighborList>::type>::value,
          "Force of NeighborList must have cutoff");

        auto const num_particles
          = ::pastel::container::num_particles(key_particles) + ::pastel::container::num_particles(partner_particles);
        particle_indices_in_each_cell_.resize(num_particles);
        particle_index_to_cell_index_.resize(num_particles);
        ::pastel::neighbor::inexclusive_cells_updater_detail::update_cell_data(
          particle_indices_in_each_cell_, particle_indices_in_each_cell_firsts_,
          particle_index_to_cell_index_, num_particles_in_each_cell_,
          lower_bound_, cell_vector_, num_cells_, total_num_cells_,
          key_particles, partner_particles);

        static constexpr bool is_key_data_accessible
          = ::pastel::container::meta::is_data_accessible<KeyParticles>::value;
        static constexpr bool is_partner_data_accessible
          = ::pastel::container::meta::is_data_accessible<PartnerParticles>::value;
        using update_neighbor_list_func
          = ::pastel::neighbor::inexclusive_cells_updater_detail::update_neighbor_list2<is_key_data_accessible, is_partner_data_accessible>;
        update_neighbor_list_func::call(
          neighbor_list, key_particles, partner_particles, squared_search_length_, total_num_cells_,
          particle_indices_in_each_cell_firsts_, neighbor_cell_indices_firsts_);
      }
    }; // class inexclusive_cells_updater<Cell, CellAllocator>
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_INEXCLUSIVE_CELLS_UPDATER_HPP

