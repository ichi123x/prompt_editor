# structure.md — アーキテクチャ・技術スタック

## 技術スタック

| 項目 | 採用技術 | 理由 |
|------|----------|------|
| 言語 | C++ | 組み込み系の延長線・MFCとの相性 |
| フレームワーク | MFC（Microsoft Foundation Classes） | VS2022標準・ダイアログベースUI |
| UIスタイル | ダイアログベース + CTabCtrl | シンプルで保守しやすい |
| 文字コード | Unicode（_UNICODE） | 日本語対応 |
| ビルド環境 | VS2022 / x64 | 開発機環境に合わせる |
| バージョン管理 | Git / GitHub | 標準 |

---

## ディレクトリ詳細設計

```
src/
├── prompt_editor.rc         # ダイアログ・タブ・コントロールのリソース定義
├── resource.h               # リソースID定義（IDC_xxx / IDD_xxx）
├── PromptEditorApp.h        # CWinAppクラス宣言
├── PromptEditorApp.cpp      # CWinAppクラス実装・エントリポイント
├── PromptEditorDlg.h        # メインダイアログクラス宣言
├── PromptEditorDlg.cpp      # メインダイアログ実装
│                            #   - タブ切り替え制御
│                            #   - ラジオボタン選択処理
│                            #   - フォルダブラウズ
│                            #   - 生成ボタン処理
└── MdGenerator.h/.cpp       # Markdown生成ロジック
                             #   - GenerateCLAUDE()
                             #   - GenerateProduct()
                             #   - GenerateStructure()
                             #   - GenerateDecisions()
                             #   - GenerateTasklist()
                             #   - WriteFile()
```

---

## UIレイアウト

```
┌─────────────────────────────────────────────────┐
│ prompt_editor                                    │
│                                                  │
│ プロジェクト名: [____________________________]   │
│ 目的         : [____________________________]   │
│ 担当者       : [____________________________]   │
│                                                  │
│ プラットフォーム:                                │
│  ○ C#/WinForms  ○ C#/WPF  ○ C++/MFC           │
│  ○ C++/Win32    ○ Web(HTML/CSS/JS)              │
│                                                  │
│ ┌──────────┬────────┬──────────┬────────────┐  │
│ │ 要件定義 │  設計  │タスク管理│  CLAUDE.md │  │
│ ├──────────┴────────┴──────────┴────────────┤  │
│ │                                            │  │
│ │  （タブ別入力フォーム）                    │  │
│ │                                            │  │
│ └────────────────────────────────────────────┘  │
│                                                  │
│ 出力先: [C:\work\claude\project____] [参照...]   │
│                                    [生成] [閉じる]│
└─────────────────────────────────────────────────┘
```

---

## 命名規則

| 対象 | 規則 | 例 |
|------|------|----|
| クラス名 | PascalCase + プレフィックス C | `CPromptEditorDlg` |
| メンバ変数 | m_ プレフィックス | `m_strProjectName` |
| リソースID | IDC_ / IDD_ / IDR_ | `IDC_EDIT_PROJECT_NAME` |
| メソッド名 | PascalCase | `GenerateMarkdown()` |
| ローカル変数 | camelCase | `strOutput` |

---

## コーディング規約

- MFC標準のDDX/DDVを使ってコントロールとメンバ変数をバインドする
- ファイル出力は `CStdioFile` を使用（UTF-8 BOMなし）
- エラー処理は `AfxMessageBox` で日本語メッセージを表示
- タブページは `CTabCtrl` + 子ダイアログ方式（各タブを別ダイアログで管理）

---

## データフロー

```
ユーザー入力
（プロジェクト名・目的・担当者・プラットフォーム選択・各タブ入力）
    ↓
CPromptEditorDlg::OnBnClickedGenerate()
    ↓
CMdGenerator::Generate( 入力データ構造体 )
    ↓
各Generate関数でCString生成
    ↓
CMdGenerator::WriteFile( パス, 内容 )
    ↓
指定フォルダに md ファイル一式出力
```
