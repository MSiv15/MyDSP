/*
 * Controller.hpp
 *
 *  Created on: 2018/09/02
 *      Author: Shibasaki
 *
 * 制御システム関連
 */

#ifndef MYDSP_CONTROLLER_HPP_
#define MYDSP_CONTROLLER_HPP_

#include "Internal/ZeroInitializer.hpp"

namespace MyDSP
{
  namespace Internal
  {
    // 離散PIDコントローラ
    // 型に依存しない共通部分の実装
    template <class T1, class T2>
    class PIDBase
    {
    protected:
      T1 state[3];
      T1 &Xn1 = state[0];
      T1 &Xn2 = state[1];
      T1 &Yn1 = state[2];
      T2 b0, b1, b2;

    public:
      // デフォルトコンストラクタ
      PIDBase() : state{}, b0(), b1(), b2() {}

      // コンストラクタ(PID係数で初期化)
      PIDBase(T2 Kp, T2 Ki, T2 Kd) :
        state{},
        b0(Kp + Ki + Kd),
        b1(-Kp - 2 * Kd),
        b2(Kd)
      {}

      // コンストラクタ(PID係数の配列で初期化)
      explicit PIDBase(const T2 (&coeffs)[3]) :
        state{},
        b0(coeffs[0] + coeffs[1] + coeffs[2]),
        b1(-coeffs[0] - 2 * coeffs[2]),
        b2(coeffs[2])
      {}

      // 状態変数の初期化
      void Clear(void)
      {
        state[0] = ZeroInitializer<T1>();
        state[1] = ZeroInitializer<T1>();
        state[2] = ZeroInitializer<T1>();
      }

      // PID係数の再設定
      void SetCoeffs(T2 Kp, T2 Ki, T2 Kd)
      {
        b0 = Kp + Ki + Kd;
        b1 = -Kp - 2 * Kd;
        b2 = Kd;
      }

      // 出力値の上書き
      void SetOutput(T1 output_value)
      {
        Yn1 = output_value;
      }

      // 出力値の再取得
      const T1& GetOutput() const
      {
        return Yn1;
      }

      // PID制御
      T1 operator()(const T1 & in)
      {
        /* y[n] = b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + y[n-1] */
        T1 out = b0 * in + b1 * Xn1 + b2 * Xn2 + Yn1;

        // 状態の更新
        Xn2 = Xn1;
        Xn1 = in;
        Yn1 = out;

        return out;
      }
    };

  } /* namespace Internal */

  // 離散PIDコントローラ
  // スカラ型用
  template <class T>
  class PIDController : public Internal::PIDBase<T,T>
  {
  private:
    using Base = Internal::PIDBase<T,T>;

  public:
    // 継承コンストラクタ
    using Base::Base;
  };

#ifdef EIGEN_CORE_H
  // 離散PIDコントローラ
  // Eigen::Matrix用
  template <class T, int... MatParams>
  class PIDController<Eigen::Matrix<T,MatParams...>>
    : public Internal::PIDBase<Eigen::Matrix<T,MatParams...>,T>
  {
  private:
    using Base = Internal::PIDBase<Eigen::Matrix<T,MatParams...>,T>;

  public:
    // デフォルトコンストラクタ
    PIDController() : Base()
    {
      this->Clear();
    }

    // コンストラクタ(PID係数で初期化)
    PIDController(T Kp, T Ki, T Kd) : Base(Kp, Ki, Kd)
    {
      this->Clear();
    }

    // コンストラクタ(PID係数の配列で初期化)
    explicit PIDController(const T (&coeffs)[3]) : Base(coeffs)
    {
      this->Clear();
    }
  };

#endif /* #ifdef EIGEN_CORE_H */

} /* namespace MyDSP */


#endif /* MYDSP_CONTROLLER_HPP_ */
