/*
 * Math.hpp
 *
 *  Created on: 2018/09/03
 *      Author: Shibasaki
 *
 * 軽量算術演算関数
 */

#ifndef MYDSP_MATH_HPP_
#define MYDSP_MATH_HPP_

#include "Const.hpp"
#include "Internal/LUT.hpp"
#include "BranchPrediction.hpp"
#include <type_traits>
#include <cmath>
#include <cstdlib>
#include <cstddef>

namespace MyDSP
{
  // 符号関数
  // コンパイル時に実行できるかどうかは環境依存
  template <class T>
  static constexpr auto Sign(T val) noexcept
    -> typename std::enable_if<std::is_signed<T>::value,int>::type
  {
    return std::signbit(-val) - std::signbit(val); // float型に対しては分岐が生じないはず
  }

  // 符号関数
  // unsigned型の場合、常に1を返す
  template <class T>
  static constexpr auto Sign(T val) noexcept
    -> typename std::enable_if<!std::is_signed<T>::value,int>::type
  {
    return 1;
  }

  // 絶対値関数
  // コンパイル時に実行できるかどうかは環境依存
  template <class T>
  static constexpr auto Abs(T val) noexcept
    -> typename std::enable_if<std::is_signed<T>::value,T>::type
  {
    return std::abs(val);
  }

  // 絶対値関数
  // unsigned型の場合、引数をそのまま返す
  template <class T>
  static constexpr auto Abs(T val) noexcept
    -> typename std::enable_if<!std::is_signed<T>::value,T>::type
  {
    return val;
  }

  // ガウス求積に基づくatan(x)の近似
  // https://blogs.yahoo.co.jp/fermiumbay2/38729661.html
  template <class T>
  static constexpr auto Atan(T x) noexcept
    -> typename std::enable_if<std::is_floating_point<T>::value,T>::type
  {
    return (x >  1) ? ( HalfPi<T>() - (45*x*x+12)/(45*x*x*x+27*x)) 
    :      (x < -1) ? (-HalfPi<T>() - (45*x*x+12)/(45*x*x*x+27*x))
    :      (12*x*x*x+45*x)/(27*x*x+45) ;
  }

  // ガウス求積に基づくatan2(y,x)の近似
  template <class T>
  static constexpr auto Atan2(T y, T x) noexcept
    -> typename std::enable_if<std::is_floating_point<T>::value,T>::type
  {
    return      (x >  0) ?  Atan(y/x)
    : (y >= 0 && x <  0) ?  Pi<T>() + Atan(y/x)
    : (y <  0 && x <  0) ? -Pi<T>() + Atan(y/x)
    : (y >  0 && x == 0) ?  HalfPi<T>()
    : (y <  0 && x == 0) ? -HalfPi<T>()
    : 0 ;
  }

  // 平方根
  // 基本的にはcmathで定義されたものをそのまま呼び出す
  // コンパイル時に実行できるかどうかは環境依存
  template <class T>
  static constexpr auto Sqrt(T x) noexcept
    -> typename std::enable_if<!std::is_integral<T>::value,T>::type
  {
    return std::sqrt(x);
  }

  // 平方根(単精度浮動小数点数)
  // 引数が負の場合は0を返す
  // CMSIS環境下であればARM FPU向けに最適化
  // コンパイル時に実行できるかどうかは環境依存
  template <>
  constexpr float Sqrt<float>(float x) noexcept
  {
    return (unlikely(!(x >= 0))) ? 0 :
#if   (__FPU_USED == 1) && defined (__CC_ARM)
      __sqrtf(x);
#elif (__FPU_USED == 1) && (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
      __builtin_sqrtf(x);
#elif (__FPU_USED == 1) && defined(__GNUC__)
      __builtin_sqrtf(x);
#elif (__FPU_USED == 1) && defined (__ICCARM__) && (__VER__ >= 6040000)
      [&x]
      {
        float y;
        __ASM("VSQRT.F32 %0,%1" : "=t"(y) : "t"(x));
        return y;
      }();
#else
      std::sqrt(x);
#endif
  }

  // 平方根(整数)
  // float型にキャストして処理する
  template <class T>
  static constexpr auto Sqrt(T x) noexcept
    -> typename std::enable_if<std::is_integral<T>::value,T>::type
  {
    return Sqrt<float>(x);
  }

  // 2乗ノルム
  // std::hypotとは異なり、極めて大きな数が入力された場合に内部でオーバーフローを起こす可能性がある
  // コンパイル時に実行できるかどうかは環境依存
  template <class T>
  static constexpr T Hypot(T x, T y) noexcept
  {
    return Sqrt(x * x + y * y);
  }

  // テーブルルックアップによるsin(theta),cos(theta)の近似計算
  // CMSIS DSPではスプライン補間が行われているが、計算量を減らすため線形補間を採用
  // 入力範囲は[-pi +pi]
  template <class T>
  static inline auto SinCos(
    const T theta,
    T * p_sin_val,
    T * p_cos_val) noexcept
    -> typename std::enable_if<std::is_floating_point<T>::value,void>::type
  {
    using namespace Internal;

    T fract, in;                  // 一時変数
    std::size_t index_s, index_c; // テーブル参照のインデックス
    T s1, s2, c1, c2;             // 出力値の前後のテーブル上の値
    T findex;

    // 入力された角度theta(rad)の絶対値を求め、[0 1]で正規化する
    in = Abs(theta * ByTwoPi<T>());
    in = in - static_cast<int>(in);

    // テーブルのインデックス値を求める
    findex = static_cast<T>(sin_table_size) * in;
    index_s = static_cast<std::size_t>(findex) % sin_table_size;
    index_c = (index_s + (sin_table_size / 4)) % sin_table_size;

    // インデックスの小数点以下成分の計算
    fract = findex - static_cast<T>(index_s);

    // 入力された角度の直近2点におけるsinとcosの値をテーブルから取得する
    s1 = SinTable<T>::values[index_s+0];
    s2 = SinTable<T>::values[index_s+1];
    c1 = SinTable<T>::values[index_c+0];
    c2 = SinTable<T>::values[index_c+1];

    // sin値の算出
    *p_sin_val = Sign(theta) * ((1.0f-fract)*s1 + fract*s2);

    // cos値の算出
    *p_cos_val = (1.0f-fract)*c1 + fract*c2;
  }

} /* namespace MyDSP */


#endif /* MYDSP_MATH_HPP_ */
