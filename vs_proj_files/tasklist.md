# tasklist.md — タスク一覧

> **運用ルール**
> - タスク完了時は `[ ]` を `[x]` に変更する
> - 新しいタスクは該当フェーズの末尾に追加する
> - 優先度：🔴 高 / 🟡 中 / 🟢 低

---

## フェーズ1：仕様定義

- [x] 🔴 `.steering/product.md` にゴール・要件を記入する
- [x] 🔴 `.steering/structure.md` に技術スタックを記入する
- [x] 🟡 スコープ（In / Out）を確定する
- [x] 🟡 完了条件を具体的に定義する

## フェーズ2：設計

- [x] 🔴 ディレクトリ構成を確定し `structure.md` に反映する
- [x] 🔴 データフローを図示する
- [x] 🟡 命名規則・コーディング規約を決める
- [x] 🟢 `decisions.md` に初期設計判断を記録する

## フェーズ3：実装

- [x] 🔴 VS2022でMFCダイアログベースプロジェクトを新規作成する（vcxproj/sln/pch生成済み）
- [x] 🔴 `resource.h` にリソースIDを定義する
- [x] 🔴 `prompt_editor.rc` でダイアログレイアウトを設計する
  - [x] プロジェクト名・目的・担当者の入力フィールド
  - [x] プラットフォーム選択ラジオボタン（5種類）
  - [x] タブコントロール（4タブ）
  - [x] 出力先フォルダ入力 + 参照ボタン
  - [x] 生成ボタン・閉じるボタン
- [x] 🔴 `PromptEditorApp.h/.cpp` を実装する
- [x] 🔴 `PromptEditorDlg.h/.cpp` を実装する
  - [x] `OnInitDialog()` でタブ初期化
  - [x] ラジオボタン選択イベント処理
  - [x] フォルダブラウズ（`SHBrowseForFolder`）
  - [x] 生成ボタンクリック処理
- [x] 🔴 `MdGenerator.h/.cpp` を実装する
  - [x] `GenerateCLAUDE()` — CLAUDE.md生成
  - [x] `GenerateProduct()` — product.md生成
  - [x] `GenerateStructure()` — structure.md生成（プラットフォーム別）
  - [x] `GenerateDecisions()` — decisions.md生成
  - [x] `GenerateTasklist()` — tasklist.md生成
  - [x] `WriteFileUtf8()` — UTF-8ファイル書き出し
- [x] 🟡 各タブの子ダイアログを実装する（.rcのDS_CONTROLダイアログで定義済み）
  - [x] 要件定義タブ（ゴール・課題・完了条件・スコープ）
  - [x] 設計タブ（技術スタック・命名規則・データフロー）
  - [x] タスク管理タブ（フェーズ別タスク入力）
  - [x] CLAUDE.mdタブ（基本指示・禁止事項）
- [ ] 🔴 VS2022でビルドし、エラーを修正する

## フェーズ4：テスト・確認

- [ ] 🔴 動作確認（手動）— 全プラットフォーム選択で md 生成
- [ ] 🟡 出力ファイルの文字コード確認（UTF-8 BOMなし）
- [ ] 🟡 必須項目未入力時のバリデーション確認
- [ ] 🟢 ドキュメント最終更新

## フェーズ5：完了

- [ ] `product.md` の成功条件をすべて満たしていることを確認
- [ ] READMEを作成・更新する
- [ ] GitHubにプッシュ

---

## 完了済み

- [x] 仕様定義（product.md / structure.md / decisions.md / tasklist.md 初版作成）
