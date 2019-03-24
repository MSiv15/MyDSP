/*
 * Filter.hpp
 *
 *  Created on: 2018/09/02
 *      Author: Shibasaki
 *
 * 離散時間フィルタ
 * サンプル列に対する一括処理はstd::transformで行うことを想定
 */

#ifndef MYDSP_FILTER_HPP_
#define MYDSP_FILTER_HPP_

#include "Internal/IndexSequence.hpp"
#include "Internal/ZeroInitializer.hpp"
#include <cstddef>

namespace MyDSP
{
  namespace Internal
  {
    // 従属型双二次IIRフィルタ(直接型I)
    // 型に依存しない共通部分の実装
    template <class T1, class T2, std::size_t NumStages>
    class BiquadDF1Base
    {
    protected:
      T1 state[NumStages+1][2];
      const T2 coeffs[NumStages][5];

    private:
      // コンストラクタ本体(移譲専用)
      // index_sequenceを用いて係数配列を初期化
      template <std::size_t... Seq>
      BiquadDF1Base(decltype((coeffs)) coeffs, IndexSequence<Seq...>) :
        state{},
        coeffs{{coeffs[Seq][0],coeffs[Seq][1],coeffs[Seq][2],coeffs[Seq][3],coeffs[Seq][4]}...}
      {}

    protected:
      // コンストラクタ(フィルタ係数の配列で初期化)
      explicit BiquadDF1Base(const T2 (&coeffs)[NumStages][5]) :
        BiquadDF1Base(coeffs, MakeIndexSequence<NumStages>())
      {}

    public:
      // 状態変数の初期化
      void Clear(void)
      {
        for (auto &block : state)
        {
          for (auto &element : block)
          {
            element = ZeroInitializer<T1>();
          }
        }
      }

      // フィルタ係数の取得
      decltype((coeffs)) GetCoeffs(void) const
      {
        return coeffs;
      }

      // フィルタ処理本体
      T1 operator()(const T1 & in)
      {
        T1 out;     // 出力
        T1 Xn = in; // 中間入力

        for (std::size_t stage = 0; stage < NumStages; ++stage)
        {
          // フィルタ係数
          const T2 &b0 = coeffs[stage][0];
          const T2 &b1 = coeffs[stage][1];
          const T2 &b2 = coeffs[stage][2];
          const T2 &a1 = coeffs[stage][3];
          const T2 &a2 = coeffs[stage][4];

          // 状態変数
          T1 &Xn1 = state[stage][0];
          T1 &Xn2 = state[stage][1];
          T1 &Yn1 = state[stage+1][0];
          T1 &Yn2 = state[stage+1][1];

          /* y[n] = b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2] */
          out = (b0 * Xn) + (b1 * Xn1) + (b2 * Xn2) + (a1 * Yn1) + (a2 * Yn2);

          // 状態の更新
          Xn2 = Xn1;
          Xn1 = Xn;
          Xn  = out;
        }
        T1 &Yn1 = state[NumStages][0];
        T1 &Yn2 = state[NumStages][1];
        Yn2 = Yn1;
        Yn1 = out;

        return out;
      }
    };

    // 従属型双二次IIRフィルタ(直接型II転置構成)
    // 型に依存しない共通部分の実装
    template <class T1, class T2, std::size_t NumStages>
    class BiquadDF2TBase
    {
    protected:
      T1 state[NumStages][2];
      const T2 coeffs[NumStages][5];

    private:
      // コンストラクタ本体(移譲専用)
      // index_sequenceを用いて係数配列を初期化
      template <std::size_t... Seq>
      BiquadDF2TBase(decltype((coeffs)) coeffs, IndexSequence<Seq...>) :
        state{},
        coeffs{{coeffs[Seq][0],coeffs[Seq][1],coeffs[Seq][2],coeffs[Seq][3],coeffs[Seq][4]}...}
      {}

    protected:
      // コンストラクタ(フィルタ係数の配列で初期化)
      explicit BiquadDF2TBase(const T2 (&coeffs)[NumStages][5]) :
        BiquadDF2TBase(coeffs, MakeIndexSequence<NumStages>())
      {}

    public:
      // 状態変数の初期化
      void Clear(void)
      {
        for (auto &block : state)
        {
          for (auto &element : block)
          {
            element = ZeroInitializer<T1>();
          }
        }
      }

      // フィルタ係数の取得
      decltype((coeffs)) GetCoeffs(void) const
      {
        return coeffs;
      }

      // フィルタ処理本体
      T1 operator()(const T1 & in)
      {
        T1 out;                    // 出力
        T1 Xn = in;                // 中間入力
        T1 p0, p1, p2, p3, p4, A1; // 中間変数

        for (std::size_t stage = 0; stage < NumStages; ++stage)
        {
          // フィルタ係数
          const T2 &b0 = coeffs[stage][0];
          const T2 &b1 = coeffs[stage][1];
          const T2 &b2 = coeffs[stage][2];
          const T2 &a1 = coeffs[stage][3];
          const T2 &a2 = coeffs[stage][4];

          // 状態変数
          T1 &d1 = state[stage][0];
          T1 &d2 = state[stage][1];

          /*  y[n] = b0 * x[n] + d1[n-1]             */
          /* d1[n] = b1 * x[n] + a1 * y[n] + d2[n-1] */
          /* d2[n] = b2 * x[n] + a2 * y[n]           */
          p0 = b0 * Xn;
          p1 = b1 * Xn;
          out = p0 + d1;
          p3 = a1 * out;
          p2 = b2 * Xn;
          A1 = p1 + p3;
          p4 = a2 * out;
          d1 = A1 + d2;
          d2 = p2 + p4;

          Xn = out;
        }
        return out;
      }
    };

    // FIRフィルタ
    // 型に依存しない共通部分の実装
    template <class T1, class T2, std::size_t NumTaps>
    class FIRBase
    {
    protected:
      T1 state[NumTaps*2]; // リングバッファの代わりにタップ長の2倍の長さの領域を使用
      T2 coeffs[NumTaps];  // 適応フィルタに使えるよう非constで宣言
      std::size_t state_top = 0; // ディレイラインの先頭を指すインデックス番号

    private:
      // コンストラクタ本体(移譲専用)
      // index_sequenceを用いて係数配列を初期化
      template <std::size_t... Seq>
      FIRBase(const T2 (&coeffs)[NumTaps], IndexSequence<Seq...>) :
        state{},
        coeffs{coeffs[Seq]...},
        state_top(0)
      {}

    protected:
      // コンストラクタ(フィルタ係数の配列で初期化)
      explicit FIRBase(const T2 (&coeffs)[NumTaps]) :
        FIRBase(coeffs, MakeIndexSequence<NumTaps>())
      {}

    public:
      // 状態変数の初期化
      void Clear(void)
      {
        for (auto &element : state)
        {
            element = ZeroInitializer<T1>();
        }
      }

      // フィルタ係数の取得
      auto GetCoeffs(void) const -> const T2 (&)[NumTaps]
      {
        return coeffs;
      }

      // フィルタ係数の再設定
      void SetCoeffs(const T2 (&coeffs_new)[NumTaps])
      {
        for (std::size_t tap_cnt = 0; tap_cnt < NumTaps; ++tap_cnt)
        {
          coeffs[tap_cnt] = coeffs_new[tap_cnt];
        }
      }

      // フィルタ処理本体
      T1 operator()(const T1 & in)
      {
        T1 out = ZeroInitializer<T1>(); // 出力

        // ディレイラインの更新
        state[state_top] = in;
        state[state_top+NumTaps] = in;
        state_top = (state_top + 1u) % NumTaps;

        // 積和演算の実行
        for (std::size_t tap_cnt = 0; tap_cnt < NumTaps; ++tap_cnt)
        {
          out += coeffs[tap_cnt] * state[state_top+tap_cnt];
        }

        return out;
      }
    };

  } /* namespace Internal */

  // 従属型双二次IIRフィルタ(直接型I)
  // スカラ型およびstd::complex用
  template <class T1, class T2, std::size_t NumStages>
  class IIRBiquadCascadeDF1 : public Internal::BiquadDF1Base<T1,T2,NumStages>
  {
  private:
    using Base = Internal::BiquadDF1Base<T1,T2,NumStages>;
  public:
    // コンストラクタ(フィルタ係数の配列で初期化)
    explicit IIRBiquadCascadeDF1(const T2 (&coeffs)[NumStages][5]) : Base(coeffs) {}
  };

  // 従属型双二次IIRフィルタ(直接型II転置構成)
  // スカラ型およびstd::complex用
  template <class T1, class T2, std::size_t NumStages>
  class IIRBiquadCascadeDF2T : public Internal::BiquadDF2TBase<T1,T2,NumStages>
  {
  private:
    using Base = Internal::BiquadDF2TBase<T1,T2,NumStages>;
  public:
    // コンストラクタ(フィルタ係数の配列で初期化)
    explicit IIRBiquadCascadeDF2T(const T2 (&coeffs)[NumStages][5]) : Base(coeffs) {}
  };

  // FIRフィルタ
  // スカラ型およびstd::complex用
  template <class T1, class T2, std::size_t NumTaps>
  class FIR : public Internal::FIRBase<T1,T2,NumTaps>
  {
  private:
    using Base = Internal::FIRBase<T1,T2,NumTaps>;
  public:
    // コンストラクタ(フィルタ係数の配列で初期化)
    explicit FIR(const T2 (&coeffs)[NumTaps]) : Base(coeffs) {}
  };

#ifdef EIGEN_WORLD_VERSION
  // 従属型双二次IIRフィルタ(直接型I)
  // Eigen::Matrix用
  template <class T1, int... MatParams, class T2, std::size_t NumStages>
  class IIRBiquadCascadeDF1<Eigen::Matrix<T1,MatParams...>,T2,NumStages>
    : public Internal::BiquadDF1Base<Eigen::Matrix<T1,MatParams...>,T2,NumStages>
  {
  private:
    using Base = Internal::BiquadDF1Base<Eigen::Matrix<T1,MatParams...>,T2,NumStages>;
  public:
    // コンストラクタ(フィルタ係数の配列で初期化)
    explicit IIRBiquadCascadeDF1(const T2 (&coeffs)[NumStages][5]) : Base(coeffs)
    {
      this->Clear();
    }
  };

  // 従属型双二次IIRフィルタ(直接型II転置構成)
  // Eigen::Matrix用
  template <class T1, int... MatParams, class T2, std::size_t NumStages>
  class IIRBiquadCascadeDF2T<Eigen::Matrix<T1,MatParams...>,T2,NumStages>
    : public Internal::BiquadDF2TBase<Eigen::Matrix<T1,MatParams...>,T2,NumStages>
  {
  private:
    using Base = Internal::BiquadDF2TBase<Eigen::Matrix<T1,MatParams...>,T2,NumStages>;
  public:
    // コンストラクタ(フィルタ係数の配列で初期化)
    explicit IIRBiquadCascadeDF2T(const T2 (&coeffs)[NumStages][5]) : Base(coeffs)
    {
      this->Clear();
    }
  };

  // FIRフィルタ
  // Eigen::Matrix用
  template <class T1, int... MatParams, class T2, std::size_t NumTaps>
  class FIR<Eigen::Matrix<T1,MatParams...>,T2,NumTaps>
    : public Internal::FIRBase<Eigen::Matrix<T1,MatParams...>,T2,NumTaps>
  {
  private:
    using Base = Internal::FIRBase<Eigen::Matrix<T1,MatParams...>,T2,NumTaps>;
  public:
    // コンストラクタ(フィルタ係数の配列で初期化)
    explicit FIR(const T2 (&coeffs)[NumTaps]) : Base(coeffs)
    {
      this->Clear();
    }
  };

#endif /* #ifdef EIGEN_WORLD_VERSION */

} /* namespace MyDSP */


#endif /* MYDSP_FILTER_HPP_ */
