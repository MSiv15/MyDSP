/*
 * IndexSequence.hpp
 *
 *  Created on: 2018/09/14
 *      Author: Shibasaki
 *
 * C++14のindex_sequenceをC++11でも使えるようにする
 */

#ifndef MYDSP_INTERNAL_INDEXSEQUENCE_HPP_
#define MYDSP_INTERNAL_INDEXSEQUENCE_HPP_

#include <cstddef>
#if __cplusplus > 201103L
  #include <utility>
#endif

namespace MyDSP
{
  namespace Internal
  {
#if __cplusplus > 201103L // C++14以降ならば、標準ライブラリのものをそのまま使う

    template <std::size_t... I>
    using IndexSequence = std::index_sequence<I...>;

    template <std::size_t N>
    using MakeIndexSequence = std::make_index_sequence<N>;

    template <class... T>
    using IndexSequenceFor = std::index_sequence_for<T...>;

#else // C++11ならば、再帰テンプレートを使って同様のものを実現する

    template <std::size_t... I>
    struct IndexSequence{};

    template <class T1, class T2>
    struct IndexSequenceConcatenator;

    template <std::size_t... I1, std::size_t... I2>
    struct IndexSequenceConcatenator<IndexSequence<I1...>, IndexSequence<I2...>>
    {
      using type = IndexSequence<I1..., (sizeof...(I1) + I2)...>;
    };

    template <std::size_t N>
    struct IndexSequenceGenerator
    {
      using type = typename IndexSequenceConcatenator<
          typename IndexSequenceGenerator<N/2>::type,
          typename IndexSequenceGenerator<N-N/2>::type
        >::type;
    };

    template <>
    struct IndexSequenceGenerator<1>
    {
      using type = IndexSequence<0>;
    };

    template <>
    struct IndexSequenceGenerator<0>
    {
      using type = IndexSequence<>;
    };

    template <std::size_t N>
    using MakeIndexSequence = typename IndexSequenceGenerator<N>::type;

    template <class... T>
    using IndexSequenceFor = MakeIndexSequence<sizeof...(T)>;

#endif /* __cplusplus > 201103L */

  } /* namespace Internal */
} /* namespace MyDSP */


#endif /* MYDSP_INTERNAL_INDEXSEQUENCE_HPP_ */
