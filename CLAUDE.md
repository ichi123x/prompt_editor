# CLAUDE.md — プロジェクト指示書

## プロジェクト概要

- **プロジェクト名**：プロジェクト入力ボックスの値をそのまま入れる
- **目的**：目的入力ボックスの値をそのまま入れる
- **担当者**：担当入力ボックスの値をそのまま入れる
- **実装言語**：プラットホームで選択された言語をそのまま入れる
---

## Claudeへの基本指示

- 応答・コメントは**日本語**で統一すること
- コードのコメントも日本語で記述すること
- 不明点があれば作業前に必ず確認すること
- 破壊的変更（ファイル削除・上書き）は事前に報告し、承認を得てから実行すること
- タスク完了時は `tasks/tasklist.md` の該当項目をチェック済みに更新すること

---

## ディレクトリ構成

- ディレクトリ（skills/.steering/tasks）がない場合は新規作成する
- ディレクトリ（skills/.steering/tasks）に該当するファイルがない場合、空で新規作成する
```
prompt_editor/
├── CLAUDE.md                     # この指示ファイル
├── skills/
│   ├── 00-common/               # 全共通
│   │   └── SKILL.md            # 作業に関するスキル
│   ├── 01-cs-winfoms/           # C#/WinForms
│   │   └── SKILL.md            # 作業に関するスキル
│   ├── 02-cs-wpf/               # C#/WPF
│   │   └── SKILL.md            # 作業に関するスキル
│   ├── 03-cpp-mfc/              # C++/MFC
│   │   └── SKILL.md            # 作業に関するスキル
│   ├── 04-cpp-win32/            # C++/Win32
│   │   └── SKILL.md            # 作業に関するスキル
│   └── 05-web/                  # Web(HTML/CSS/JS)
│        └── SKILL.md            # 作業に関するスキル
├── .steering/
│   ├── product.md               # プロダクト要件・ゴール定義
│   ├── structure.md             # アーキテクチャ・技術スタック
│   └── decisions.md             # 設計判断の記録（ADR）
├── tasks/
│   └── tasklist.md              # タスク一覧・進捗管理
├── docs/                    　   # 設計書・仕様書・メモ
├── src/
│   ├── prompt_editor.rc         # ダイアログリソース定義
│   ├── resource.h               # リソースID定義
│   ├── PromptEditorApp.h/.cpp   # MFCアプリケーションクラス
│   ├── PromptEditorDlg.h/.cpp   # メインダイアログ
│   └── MdGenerator.h/.cpp       # Markdownファイル生成ロジック
└── prompt_editor.sln 　          # VS2022ソリューション
```

---

## 開発フロー（スペック駆動）

```
1. 要件定義   → .steering/product.md を更新
2. 設計       → .steering/structure.md を更新
3. タスク分解  → tasks/tasklist.md にタスクを追加
4. 実装       → タスクを上から順に実施（MFC作業時は上記の行動規約を厳守）
5. 確認       → タスクをチェック済みに更新
6. ビルド検証  → VS2022でビルド・動作確認、エラーはテキストで共有
```
