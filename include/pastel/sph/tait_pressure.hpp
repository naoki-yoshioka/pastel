#ifndef PASTEL_SPH_TAIT_PRESSURE_HPP
# define PASTEL_SPH_TAIT_PRESSURE_HPP


namespace pastel
{
  namespace sph
  {
    // https://en.wikipedia.org/wiki/Tait_equation
    // This class actually accepts an EOS in Monaghan (2005)
    template <typename Value = double>
    class tait_pressure
    {
      Value reference_density_;
      Value sound_speed_;
      Value exponent_;

      Value inverse_reference_density_;
      Value reference_pressure_;

     public:
      constexpr tait_pressure(Value const& reference_density, Value const& sound_speed, Value const& exponent)
        : reference_density_{reference_density},
          sound_speed_{sound_speed},
          exponent_{exponent},
          inverse_reference_density_{Value{1} / reference_density},
          reference_pressure_{sound_speed * sound_speed * reference_density / exponent}
      { }

      constexpr Value const& reference_density() const { return reference_density_; }
      void reference_density(Value const& new_reference_density)
      {
        reference_density_ = new_reference_density;
        inverse_reference_density_ = Value{1} / new_reference_density;
        reference_pressure_ = sound_speed_ * sound_speed_ * new_reference_density / exponent_;
      }

      constexpr Value const& sound_speed() const { return sound_speed_; }
      void sound_speed(Value const& new_sound_speed)
      {
        sound_speed_ = new_sound_speed;
        reference_pressure_ = new_sound_speed * new_sound_speed * reference_density_ / exponent_;
      }

      constexpr Value const& exponent() const { return exponent_; }
      void exponent(Value const& new_exponent)
      {
        exponent_ = new_exponent;
        reference_pressure_ = sound_speed_ * sound_speed_ * reference_density_ / new_exponent;
      }

      // P = P_0 [(rho/rho_0)^gamma - 1], P_0 = c^2 rho_0 / gamma
      template <typename Density>
      Value operator()(Density const& density) const
      {
        using std::pow;
        return reference_pressure_ * (pow(density * inverse_reference_density_, exponent_) - Value{1});
      }
    }; // class tait_pressure<Value>
  } // namespace sph
} // namespace pastel


#endif // PASTEL_SPH_TAIT_PRESSURE_HPP

