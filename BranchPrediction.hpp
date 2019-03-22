/*
 * BranchPrediction.hpp
 *
 *  Created on: 2018/09/06
 *      Author: Shibasaki
 *
 * 分岐予測補助
 * GCCとClangで動作確認済み
 */

#ifndef MYDSP_BRANCHPREDICTION_HPP_
#define MYDSP_BRANCHPREDICTION_HPP_

namespace MyDSP
{
#if defined(__GNUC__)
  // 論理が真の場合を優先して最適化
  static constexpr __attribute__((always_inline))
  bool likely(bool x) {return __builtin_expect(!!x, 1);}
  // 論理が偽の場合を優先して最適化
  static constexpr __attribute__((always_inline))
  bool unlikely(bool x) {return __builtin_expect(!!x, 0);}
#elif defined(_MSC_VER)
  // 論理が真の場合を優先して最適化
  static constexpr __forceinline
  bool likely(bool x) {return __assume(!!x);}
  // 論理が偽の場合を優先して最適化
  static constexpr __forceinline
  bool unlikely(bool x) {return __assume(!x);}
#else
  static constexpr bool likely(bool x)   {return x;}
  static constexpr bool unlikely(bool x) {return x;}
#endif

} /* namespace MyDSP */


#endif /* MYDSP_BRANCHPREDICTION_HPP_ */
