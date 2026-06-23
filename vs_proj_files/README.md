# prompt_editor

スペック駆動開発用のMarkdownファイル群を生成する **C++/MFC ダイアログアプリ**（VS2022）。

## 概要

新規プロジェクト開始時に、Claude Codeのスペック駆動開発テンプレート（CLAUDE.md / product.md / structure.md / decisions.md / tasklist.md）を GUI から入力して一括生成します。

## 生成されるファイル

```
{出力先フォルダ}/
├── CLAUDE.md
├── .steering/
│   ├── product.md
│   ├── structure.md
│   └── decisions.md
└── tasks/
    └── tasklist.md
```

## ビルド方法

1. VS2022 で `prompt_editor.sln` を開く
2. ソリューションエクスプローラーで `src/` 以下のファイルをプロジェクトに追加
3. 構成：`Release / x64` でビルド

### 必要なVSコンポーネント
- C++ によるデスクトップ開発ワークロード
- MFC (C++ MFC for v143 build tools)

## 使い方

1. ツールを起動する
2. プロジェクト名・目的・担当者を入力
3. ラジオボタンでプラットフォームを選択
4. 各タブ（要件定義／設計／タスク管理／CLAUDE.md）に情報を入力
5. 出力先フォルダを指定（「参照...」ボタン）
6. 「生成」ボタンをクリック

## ソース構成

| ファイル | 役割 |
|----------|------|
| `resource.h` | リソースID定義 |
| `prompt_editor.rc` | ダイアログレイアウト（リソーススクリプト） |
| `PromptEditorApp.h/.cpp` | MFCアプリケーションクラス |
| `PromptEditorDlg.h/.cpp` | メインダイアログ（UI制御） |
| `MdGenerator.h/.cpp` | Markdown生成ロジック |

## プラットフォーム対応

選択したプラットフォームに応じて `structure.md` の技術スタック欄が自動補完されます。

| 選択 | 自動補完内容 |
|------|-------------|
| C#/WinForms | C# / .NET 8 / WinForms |
| C#/WPF | C# / .NET 8 / WPF / MVVM |
| C++/MFC | C++ / MFC / Unicode / x64 |
| C++/Win32 | C++ / Win32 API / TCHAR / x64 |
| Web | HTML / CSS / JS / npm |
