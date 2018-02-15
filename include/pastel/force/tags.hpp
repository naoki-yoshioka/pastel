#ifndef PASTEL_FORCE_TAGS_HPP
# define PASTEL_FORCE_TAGS_HPP


namespace pastel
{
  namespace force
  {
    namespace tags
    {
      struct requires_position { };
      struct requires_position_velocity { };
      struct requires_position_orientation { };
      struct dem_requirement { };
      struct newtonian_sph_requirement { };
    } // namespace tags
  } // namespace force
} // namespace pastel


#endif // PASTEL_FORCE_TAGS_HPP

