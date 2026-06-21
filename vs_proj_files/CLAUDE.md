# CLAUDE.md — プロジェクト指示書

## プロジェクト概要

- **プロジェクト名**：prompt_editor
- **目的**：スペック駆動開発用のMarkdownファイル群を生成するプロンプトエディタ
- **担当者**：ihira
- **実装言語**：C++ / MFC（VS2022）

---

## Claudeへの基本指示

- 応答・コメントは**日本語**で統一すること
- コードのコメントも日本語で記述すること
- 不明点があれば作業前に必ず確認すること
- 破壊的変更（ファイル削除・上書き）は事前に報告し、承認を得てから実行すること
- タスク完了時は `tasks/tasklist.md` の該当項目をチェック済みに更新すること

---

## ディレクトリ構成

```
prompt_editor/
├── CLAUDE.md                    # この指示ファイル
├── .steering/
│   ├── product.md               # プロダクト要件・ゴール定義
│   ├── structure.md             # アーキテクチャ・技術スタック
│   └── decisions.md             # 設計判断の記録（ADR）
├── tasks/
│   └── tasklist.md              # タスク一覧・進捗管理
├── docs/                        # 設計書・仕様書・メモ
├── src/
│   ├── prompt_editor.rc         # ダイアログリソース定義
│   ├── resource.h               # リソースID定義
│   ├── PromptEditorApp.h/.cpp   # MFCアプリケーションクラス
│   ├── PromptEditorDlg.h/.cpp   # メインダイアログ
│   └── MdGenerator.h/.cpp       # Markdownファイル生成ロジック
└── prompt_editor.sln            # VS2022ソリューション
```

---

## 開発フロー（スペック駆動）

```
1. 要件定義  → .steering/product.md を更新
2. 設計      → .steering/structure.md を更新
3. タスク分解 → tasks/tasklist.md にタスクを追加
4. 実装      → タスクを上から順に実施
5. 確認      → タスクをチェック済みに更新
6. 設計判断  → .steering/decisions.md に記録
```

---

## 禁止事項

- `.steering/` 内のファイルを無断で書き換えないこと
- `tasks/tasklist.md` のタスクを無断で削除しないこと
- 未確認の外部ライブラリを追加しないこと
