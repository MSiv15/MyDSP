/*
 * ZeroInitializer.hpp
 *
 *  Created on: 2018/09/20
 *      Author: Shibasaki
 *
 * ゼロ初期化処理の統一
 * だいたいEigenのせい
 */

#ifndef MYDSP_INTERNAL_ZEROINITIALIZER_HPP_
#define MYDSP_INTERNAL_ZEROINITIALIZER_HPP_

namespace MyDSP
{
  namespace Internal
  {
    template <class T>
    struct ZeroInitializerImpl
    {
      using type = T;
      static constexpr type GetValue(void)
      {
        return type();
      }
    };

#ifdef EIGEN_WORLD_VERSION
    template <class Scalar, int... Params>
    struct ZeroInitializerImpl<Eigen::Matrix<Scalar,Params...>>
    {
      using type = Eigen::Matrix<Scalar,Params...>;
      static constexpr type GetValue(void)
      {
        return type::Zero();
      }
    };

    template <class Scalar, int... Params>
    struct ZeroInitializerImpl<Eigen::Array<Scalar,Params...>>
    {
      using type = Eigen::Array<Scalar,Params...>;
      static constexpr type GetValue(void)
      {
        return type::Zero();
      }
    };

#endif /* #ifdef EIGEN_WORLD_VERSION */

    template <class T>
    static constexpr T ZeroInitializer(void)
    {
      return ZeroInitializerImpl<T>::GetValue();
    }

  } /* namespace Internal */
} /* namespace MyDSP */


#endif /* MYDSP_INTERNAL_ZEROINITIALIZER_HPP_ */
