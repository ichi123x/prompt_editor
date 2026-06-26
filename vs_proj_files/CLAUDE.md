# CLAUDE.md — プロジェクト指示書

## プロジェクト概要

- **プロジェクト名**：プロンプトエディター（Prompt Editor）
- **目的**：新規プロジェクト開始時に、Claude Code で作業しやすい環境一式（`CLAUDE.md` / `skills/` / `.steering/` / `tasks/` のMDファイル群）を生成するデスクトップツール。利用者が入力した「プロジェクト名・目的・担当者・プラットフォーム」等を反映した `CLAUDE.md` を出力する
- **実装言語**：C++ / MFC（VS2022）

### 本リポジトリの2階層構造（重要）

| 階層 | 場所 | 役割 |
|------|------|------|
| ツール本体 | リポジトリ直下（`vs_proj_files/` ほか） | **プロンプトエディター自身**の開発対象（MFC C++） |
| 生成テンプレート | `vs_proj_files/output_test/` | ツールが**生成する成果物のサンプル/テンプレート** |

- この `CLAUDE.md` は **ツール本体の開発ガイド**であり、生成テンプレートではない。差し込み文言（入力ボックスの値）を書かないこと
- `skills/` 配下の各 `SKILL.md` は**プラットフォームごとの知見ベース**で、壁打ちを通じて継続的に育てる（「完成」ステータスは持たない）。まず MFC C++ を固め、その後に他プラットフォームへ展開する

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
├── vs_proj_files/
│   ├── prompt_editor.rc         # ダイアログリソース定義
│   ├── resource.h               # リソースID定義
│   ├── PromptEditorApp.h/.cpp   # MFCアプリケーションクラス
│   ├── PromptEditorDlg.h/.cpp   # メインダイアログ
│   ├── TabDialog.h/.cpp         # タブダイアログ
│   ├── MdGenerator.h/.cpp       # Markdownファイル生成ロジック
│   └── pch.h/.cpp               # プリコンパイル済みヘッダ
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
