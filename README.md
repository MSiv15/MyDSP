# MyDSP
ヘッダオンリー軽量DSPライブラリ

## Features
- c++11/14
- ヘッダオンリー
- 信号処理クラスとしてIIR/FIRフィルタ、離散PIDコントローラを提供(浮動小数点型のみ)
- 算術関数として分数関数によるatan/atan2の近似計算、テーブル参照によるsin/cosの近似計算などを提供
- コンパイラによる最適化を前提とした実装
- [Eigen](http://eigen.tuxfamily.org)ライブラリで提供される行列型をサポート

## Install
[MyDSP](Include/MyDSP)ディレクトリを丸ごとプロジェクトのインクルードディレクトリにコピーすれば使用可能です。
```bash
$ git clone https://github.com/MSiv15/MyDSP.git
$ cp -r MyDSP/Include/MyDSP SomeProject/Include
```

## Usage
機能ごとにファイルが分かれているので、使用する機能に対応するヘッダファイルをインクルードします。  
Eigenライブラリと連携する場合は本ライブラリより前にEigenをインクルードしてください。
```c++
#include <iostream>
#include "Eigen/Core"
#include "MyDSP/Filter.hpp"

int main(void)
{
  constexpr double filtercoeff[1][5] = {{0,0.1,0,0.9,0}}; // {{b0,b1,b2,a1,a2}}
  MyDSP::IIRBiquadCascadeDF2T<Eigen::RowVector2d,double,1> filter(filtercoeff);
  Eigen::RowVector2d in(1,2);
  Eigen::RowVector2d out;
  
  for (int i = 0; i < 50; ++i)
  {
    out = filter(in);
    std::cout << i << "\t" << out(0) << "\t" << out(1) << std::endl;
  }
  return 0;
}
```

## License
This library is released under the MIT License, see [LICENSE](LICENSE).

## Author
[Masaki Shibasaki](https://github.com/MSiv15)
