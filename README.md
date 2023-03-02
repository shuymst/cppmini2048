# 3 × 3盤面の2048の完全解析 (Strongly Solving 2048 on 3 × 3 Board)

## 2048
2048は $4 \times 4$ 盤面上で行われるパズルゲームです。https://play2048.co でプレイすることが出来ます。

## $3 \times 3$盤面の2048の完全解析
2048は状態数の多さから完全解析が困難です。
このリポジトリは2048を $3 \times 3$ 盤面に縮小したゲームであるmini2048を考え、その完全解析を行ったものです。
成果として投稿した論文は[こちら](https://ipsj.ixsq.nii.ac.jp/ej/?action=pages_view_main&active_action=repository_view_main_item_detail&item_id=221984&item_no=1&page_id=13&block_id=8)になります。

## 環境
Ubuntu20.0.4, 22.04での動作を確認しています。
コンパイラはgccを推奨しています。それ以外の場合はCMakeLists.txtのコンパイルオプションを適宜編集してください。

## コンパイル方法
```
cd src/
mkdir build && cd build
cmake ..
make
```
これによりsrc/build以下に各実行ファイルが生成されます。
## 合法な全ての状態の列挙
```
./enumeration
```
これにより初期状態から遷移可能な全ての状態の列挙を行います。
各状態は符号なし64bit整数で表され、src/build/found_statesに記録されます。
実行時間は環境によりますが5分以内に終わるかと思います。
## 列挙した状態の価値計算
```
./retrograde
```
後退解析により全ての合法な状態の価値を計算します。ここでいう価値とは、「その状態から最善手に従い続ければ平均的に何点取れるか」を表します。詳細は[論文](https://ipsj.ixsq.nii.ac.jp/ej/?action=pages_view_main&active_action=repository_view_main_item_detail&item_id=221984&item_no=1&page_id=13&block_id=8)を参照ください。
実行時間は環境によりますが5時間ほどかかるかと思います。