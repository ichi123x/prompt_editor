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

---

## ビルド

ビルドは必ずリポジトリ直下の `build.ps1` を使うこと。素の `msbuild` は使わない
（このPCには VS2026 も併存しており、どちらの MSBuild が動くか不定になるため）。

```powershell
.\build.ps1                        # Debug ビルド
.\build.ps1 -Configuration Release # Release ビルド
.\build.ps1 -Target Rebuild        # リビルド
.\build.ps1 -Run                   # ビルド成功後に exe を起動
```

- `build.ps1` は vswhere で「17.x かつ MFC/ATL を含む」VS2022 だけを特定して MSBuild を呼ぶ
- ビルドログは `build.log` に **UTF-8** で出力される。**失敗時はまず `build.log` を読んで原因を特定すること**
- 出力先：`x64\Debug\prompt_editor.exe`（Release は `x64\Release\`）
- 構成は `Debug|x64` / `Release|x64` のみ。**Win32 構成は存在しない**
- プラットフォームツールセットは **v143 固定**。`v145` 等が混入した場合は `build.ps1` が警告を出す
- 実行ポリシーで弾かれる場合は `powershell -ExecutionPolicy Bypass -File .\build.ps1` を使う

---

## MFC 固有の行動規約（厳守）

### リソースファイルは直接編集しない

- **`vs_proj_files/prompt_editor.rc` と `vs_proj_files/resource.h` をテキスト編集しないこと**
- ダイアログ・コントロール・リソースIDの追加や変更は、**Visual Studio のリソースエディタ**で行う
- 直接編集すると、リソースIDの重複や `.aps`（エディタのキャッシュ）との不整合でリソースが壊れる
- リソースの変更が必要な場合は、**何をどう変更すべきかを日本語で提示し、利用者にVS上での操作を依頼すること**

### 文字コード

- ソースは **UTF-8（BOM付き）** で保存する
- `.rc` 先頭の `#pragma code_page(65001)` を絶対に削除・変更しないこと
- `CLAUDE.md` など Markdown は **UTF-8（BOMなし）/ CRLF**

### コーディング

- 文字セットは **Unicode**（`CString` は `CStringW`）。`char*` / `std::string` との混在時は明示的に変換する
- 文字列リテラルは `_T("...")` または `L"..."` を使う
- MFC は **共有DLL版**（`UseOfMfc = Dynamic`）
- C++ 標準は **C++17**（`stdcpp17`）、`ConformanceMode` 有効
- プリコンパイル済みヘッダは `pch.h`。新規 .cpp の先頭では必ず `#include "pch.h"` を最初に書く

### ファイル追加時

- 新しい `.cpp` / `.h` を追加したら、**`vs_proj_files/prompt_editor.vcxproj` の該当 `ItemGroup` にも追記すること**
  - `.cpp` → `<ClCompile Include="..." />`
  - `.h`   → `<ClInclude Include="..." />`
- 追記を忘れるとビルド対象から漏れ、リンクエラーになる

---

## 触ってはいけないファイル・ディレクトリ

| 対象 | 理由 |
|------|------|
| `.vs/` | VS のローカルキャッシュ。読む必要も編集する必要もない |
| `x64/`, `vs_proj_files/x64/` | ビルド中間・成果物 |
| `*.aps` | リソースエディタのキャッシュ。触ると `.rc` が壊れる |
| `*.user` | 個人のデバッグ設定 |
| `build.log` | ビルド出力（読むのは可、編集は不可） |

### PowerShell スクリプト（.ps1）の文字コード

- **日本語を含む `.ps1` は必ず UTF-8 (BOM付き) で保存すること**
- Windows PowerShell 5.1 (`powershell.exe`) は BOM が無い .ps1 を Shift-JIS として読むため、
  BOMなしUTF-8だと日本語コメントが文字化けし、壊れたバイト列が引用符や `&` として
  誤解釈されて構文エラーになる
- VS Code の既定は「BOMなし UTF-8」なので、.ps1 を編集・新規作成したら
  右下のエンコーディング表示 →「Save with Encoding」→「UTF-8 with BOM」を必ず確認する
- 改行は CRLF に揃える

---

## テスト

テストフレームワークは **Microsoft Native Unit Test**（VS2022 同梱）。追加インストールは不要。

```powershell
.\build.ps1 -Test                        # Debug をビルドしてテスト実行
.\build.ps1 -Configuration Release -Test  # Release でテスト実行
```

VS Code では `Ctrl+Shift+P` →「Tasks: Run Test Task」でも実行できる。

### 構成

| 場所 | 内容 |
|------|------|
| `tests/prompt_editor.tests.vcxproj` | テストDLL（MFC共有DLL・v143・x64） |
| `tests/MdGeneratorTests.cpp` | `CMdGenerator` のテスト |

- テストプロジェクトは本体の `.cpp` を**直接取り込んでコンパイル**する（ライブラリ化不要）
- プリコンパイル済みヘッダは使わない（`PrecompiledHeader=NotUsing`）
- テスト対象の .cpp を増やす場合は `tests/prompt_editor.tests.vcxproj` の `ItemGroup` に
  `<ClCompile Include="..\vs_proj_files\XXX.cpp" />` を追加する

### テストを書くときの方針

- **UI（`CDialog` / `CWinApp` 派生）に依存しないロジックだけをテストする**
- リソース（`FindResource` / `LoadResource`）を使う関数はテストDLL単体では動かない。
  例：`GeneratePreview(data, 0)` は `IDR_CLAUDE_TEMPLATE` を読むため対象外
- 新機能を実装するときは、**まずロジックを UI から切り離してからテストを書く**
- テストメソッド名は ASCII（日本語識別子は使わない）。説明は Assert の第2引数に日本語で書く
