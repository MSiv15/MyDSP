/*
 * Const.hpp
 *
 *  Created on: 2018/09/03
 *      Author: Shibasaki
 *
 * 数学定数
 * https://keisan.casio.jp/calculator で得られる値からそれぞれの型にキャストする
 */

#ifndef MYDSP_CONST_HPP_
#define MYDSP_CONST_HPP_

namespace MyDSP
{
  // pi
  template <class T>
  static constexpr T Pi() {
    return static_cast<T>(3.1415926535897932384626433832795028841971693993751L);
  }
  static constexpr float  pi_f = Pi<float>();
  static constexpr double pi_d = Pi<double>();

  // pi * 2
  template <class T>
  static constexpr T TwoPi() {
    return static_cast<T>(6.2831853071795864769252867665590057683943387987502L);
  }
  static constexpr float  two_pi_f = TwoPi<float>();
  static constexpr double two_pi_d = TwoPi<double>();

  // pi * 4
  template <class T>
  static constexpr T FourPi() {
    return static_cast<T>(12.5663706143591729538505735331180115367886775975L);
  }
  static constexpr float  four_pi_f = FourPi<float>();
  static constexpr double four_pi_d = FourPi<double>();

  // pi / 2
  template <class T>
  static constexpr T HalfPi() {
    return static_cast<T>(1.5707963267948966192313216916397514420985846996876L);
  }
  static constexpr float  half_pi_f = HalfPi<float>();
  static constexpr double half_pi_d = HalfPi<double>();

  // pi / 3
  template <class T>
  static constexpr T ThirdPi() {
    return static_cast<T>(1.047197551196597746154214461093167628065723133125L);
  }
  static constexpr float  third_pi_f = ThirdPi<float>();
  static constexpr double third_pi_d = ThirdPi<double>();

  // pi * 2 / 3
  template <class T>
  static constexpr T TwoThirdsPi() {
    return static_cast<T>(2.0943951023931954923084289221863352561314462662501L);
  }
  static constexpr float  two_thirds_pi_f = TwoThirdsPi<float>();
  static constexpr double two_thirds_pi_d = TwoThirdsPi<double>();

  // pi / 4
  template <class T>
  static constexpr T QuarterPi() {
    return static_cast<T>(0.78539816339744830961566084581987572104929234984378L);
  }
  static constexpr float  quarter_pi_f = QuarterPi<float>();
  static constexpr double quarter_pi_d = QuarterPi<double>();

  // pi * 3 / 4
  template <class T>
  static constexpr T ThreeQuartersPi() {
    return static_cast<T>(2.3561944901923449288469825374596271631478770495313L);
  }
  static constexpr float  three_quarters_pi_f = ThreeQuartersPi<float>();
  static constexpr double three_quarters_pi_d = ThreeQuartersPi<double>();

  // pi / 6
  template <class T>
  static constexpr T SixthPi() {
    return static_cast<T>(0.52359877559829887307710723054658381403286156656252L);
  }
  static constexpr float  sixth_pi_f = SixthPi<float>();
  static constexpr double sixth_pi_d = SixthPi<double>();

  // pi * 5 / 6
  template <class T>
  static constexpr T FiveSixthsPi() {
    return static_cast<T>(2.6179938779914943653855361527329190701643078328126L);
  }
  static constexpr float  five_sixths_pi_f = FiveSixthsPi<float>();
  static constexpr double five_sixths_pi_d = FiveSixthsPi<double>();

  // pi / 30
  template <class T>
  static constexpr T PiByThirty() {
    return static_cast<T>(0.1047197551196597746154214461093167628065723133125L);
  }
  static constexpr float  pi_by_thirty_f = PiByThirty<float>();
  static constexpr double pi_by_thirty_d = PiByThirty<double>();

  // pi / 180
  template <class T>
  static constexpr T PiByOneEighty() {
    return static_cast<T>(0.017453292519943295769236907684886127134428718885417L);
  }
  static constexpr float  pi_by_one_eighty_f = PiByOneEighty<float>();
  static constexpr double pi_by_one_eighty_d = PiByOneEighty<double>();

  // 1 / pi
  template <class T>
  static constexpr T ByPi() {
    return static_cast<T>(0.31830988618379067153776752674502872406891929148091L);
  }
  static constexpr float  by_pi_f = ByPi<float>();
  static constexpr double by_pi_d = ByPi<double>();

  // 1 / (pi * 2)
  template <class T>
  static constexpr T ByTwoPi() {
    return static_cast<T>(0.15915494309189533576888376337251436203445964574046L);
  }
  static constexpr float  by_two_pi_f = ByTwoPi<float>();
  static constexpr double by_two_pi_d = ByTwoPi<double>();

  // 30 / pi
  template <class T>
  static constexpr T ThirtyByPi() {
    return static_cast<T>(9.5492965855137201461330258023508617220675787444274L);
  }
  static constexpr float  thirty_by_pi_f = ThirtyByPi<float>();
  static constexpr double thirty_by_pi_d = ThirtyByPi<double>();

  // 180 / pi
  template <class T>
  static constexpr T OneEightyByPi() {
    return static_cast<T>(57.295779513082320876798154814105170332405472466564L);
  }
  static constexpr float  one_eighty_by_pi_f = OneEightyByPi<float>();
  static constexpr double one_eighty_by_pi_d = OneEightyByPi<double>();

  // sqrt(2)
  template <class T>
  static constexpr T SqrtTwo() {
    return static_cast<T>(1.414213562373095048801688724209698078569671875377L);
  }
  static constexpr float  sqrt_two_f = SqrtTwo<float>();
  static constexpr double sqrt_two_d = SqrtTwo<double>();

  // sqrt(3)
  template <class T>
  static constexpr T SqrtThree() {
    return static_cast<T>(1.7320508075688772935274463415058723669428052538104L);
  }
  static constexpr float  sqrt_three_f = SqrtThree<float>();
  static constexpr double sqrt_three_d = SqrtThree<double>();

  // 1 / sqrt(2)
  template <class T>
  static constexpr T BySqrtTwo() {
    return static_cast<T>(0.70710678118654752440084436210484903928483593768847L);
  }
  static constexpr float  by_sqrt_two_f = BySqrtTwo<float>();
  static constexpr double by_sqrt_two_d = BySqrtTwo<double>();

  // 1 / sqrt(3)
  template <class T>
  static constexpr T BySqrtThree() {
    return static_cast<T>(0.57735026918962576450914878050195745564760175127013L);
  }
  static constexpr float  by_sqrt_three_f = BySqrtThree<float>();
  static constexpr double by_sqrt_three_d = BySqrtThree<double>();

} /* namespace MyDSP */


#endif /* MYDSP_CONST_HPP_ */
