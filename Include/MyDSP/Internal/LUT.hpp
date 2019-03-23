/*
 * LUT.hpp
 *
 *  Created on: 2018/09/04
 *      Author: Shibasaki
 *
 * ルックアップテーブル
 */

#ifndef MYDSP_INTERNAL_LUT_HPP_
#define MYDSP_INTERNAL_LUT_HPP_

#include "IndexSequence.hpp"
#include "../Const.hpp"
#include <limits>
#include <cmath>
#include <cstddef>

namespace MyDSP
{
  namespace Internal
  {
    static constexpr std::size_t sin_table_size = 512;

#if defined(__GNUC__) && (__GNUC__ * 10 + __GNUC_MINOR__ >= 46)
    // cmathがconstexprに対応している場合、それを使って三角関数テーブルをコンパイル時に生成する
    // N: 0から2πまでの分割数(1以上)
    // I: 生成される要素数-1
    template <class T, std::size_t N, std::size_t I>
    struct SinCosTableGenerator
    {
      static_assert(N > 0, "Template parameter 'N' shouldn't be zero");
      static constexpr T dx = TwoPi<T>() / N;
      T sin_vals[I+1];
      T cos_vals[I+1];
      template <std::size_t... Seq>
      constexpr SinCosTableGenerator(IndexSequence<Seq...>) :
        sin_vals{0, std::sin(dx*(1+Seq))...},
        cos_vals{1, std::cos(dx*(1+Seq))...}
      {}
      constexpr SinCosTableGenerator() :
        SinCosTableGenerator(MakeIndexSequence<I>())
      {}
    };

#else
    // マクローリン展開によるsin(x)の近似
    template <class T>
    static constexpr T SinMaclaurin(T x, int m_max, int m = 1)
    {
      return (m <= 0) ? 0
      :  (m >= m_max) ? x
      :  x - (x * x)/(2 * m * (2 * m + 1)) * SinMaclaurin(x, m_max, m + 1) ;
    }

    // マクローリン展開によるcos(x)の近似
    template <class T>
    static constexpr T CosMaclaurin(T x, int m_max, int m = 1)
    {
      return (m <= 0) ? 0
      :  (m >= m_max) ? 1
      :  1 - (x * x)/(2 * m * (2 * m - 1)) * CosMaclaurin(x, m_max, m + 1) ;
    }

    // マクローリン展開と加法定理を用いて三角関数テーブルをコンパイル時に生成する
    // N: 0から2πまでの分割数(1以上)
    // I: 生成される要素数-1
    template <class T, std::size_t N, std::size_t I>
    struct SinCosTableGenerator
    {
      T sin_vals[I+1];
      T cos_vals[I+1];
      template <std::size_t... Seq1, std::size_t... Seq2>
      constexpr SinCosTableGenerator(SinCosTableGenerator<T,N,(I+1)/2>&& prev, IndexSequence<Seq1...>, IndexSequence<Seq2...>) :
        sin_vals{
          0,
          prev.sin_vals[1+Seq1]...,
          (prev.sin_vals[(I+1)/2] * prev.cos_vals[1+Seq2] + prev.cos_vals[(I+1)/2] * prev.sin_vals[1+Seq2])...
        },
        cos_vals{
          1,
          prev.cos_vals[1+Seq1]...,
          (prev.cos_vals[(I+1)/2] * prev.cos_vals[1+Seq2] - prev.sin_vals[(I+1)/2] * prev.sin_vals[1+Seq2])...
        }
      {}
      constexpr SinCosTableGenerator() :
        SinCosTableGenerator(SinCosTableGenerator<T,N,(I+1)/2>(), MakeIndexSequence<(I+1)/2>(), MakeIndexSequence<I/2>())
      {}
    };

    template <class T, std::size_t N>
    struct SinCosTableGenerator<T,N,1>
    {
      static_assert(N > 0, "Template parameter 'N' shouldn't be zero");
      static constexpr T dx = TwoPi<T>() / N;
      static constexpr T sin_dx = SinMaclaurin(dx, 5);
      static constexpr T cos_dx = CosMaclaurin(dx, 6);
      T sin_vals[2];
      T cos_vals[2];
      constexpr SinCosTableGenerator() : sin_vals{0, sin_dx}, cos_vals{1, cos_dx} {}
    };

    template <class T, std::size_t N>
    struct SinCosTableGenerator<T,N,0>
    {
      T sin_vals[1];
      T cos_vals[1];
      constexpr SinCosTableGenerator() : sin_vals{0}, cos_vals{1} {}
    };

#endif /* defined(__GNUC__) && (__GNUC__ * 10 + __GNUC_MINOR__ >= 46) */

    // 三角関数テーブルを結合して1周期の正弦波にする
    template <class T, std::size_t N>
    struct SinTableImpl
    {
      static_assert(N%8 == 0, "Template parameter 'N' should be multiple of 8");
      T values[N+1];
      template <std::size_t... Seq>
      constexpr SinTableImpl(SinCosTableGenerator<long double,N,N/8>&& table, IndexSequence<Seq...>) :
        values{
          static_cast<T>(table.sin_vals[Seq])...,
          static_cast<T>(table.cos_vals[N/8-Seq])...,
          static_cast<T>(table.cos_vals[Seq])...,
          static_cast<T>(table.sin_vals[N/8-Seq])...,
          -static_cast<T>(table.sin_vals[Seq])...,
          -static_cast<T>(table.cos_vals[N/8-Seq])...,
          -static_cast<T>(table.cos_vals[Seq])...,
          -static_cast<T>(table.sin_vals[N/8-Seq])...,
          0
        }
      {}
      constexpr SinTableImpl() :
        SinTableImpl(SinCosTableGenerator<long double,N,N/8>(), MakeIndexSequence<N/8>())
      {}
    };

    // 正弦波テーブルの実体
    template <class T>
    struct SinTable
    {
      static constexpr SinTableImpl<T,sin_table_size> instance{};
      static constexpr auto& values = instance.values;
    };
    template <class T>
    constexpr SinTableImpl<T,sin_table_size> SinTable<T>::instance;

  } /* namespace Internal */
} /* namespace MyDSP */


#endif /* MYDSP_INTERNAL_LUT_HPP_ */
