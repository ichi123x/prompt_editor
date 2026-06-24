// MdGenerator.cpp — Markdownファイル生成クラス実装
#include "pch.h"
#include "MdGenerator.h"
#include <direct.h>

CMdGenerator::CMdGenerator() {}
CMdGenerator::~CMdGenerator() {}

// ============================================================
// 全mdファイルを一括生成する
// ============================================================
BOOL CMdGenerator::GenerateAll(const ProjectData& data)
{
    // 出力先フォルダ構成
    // {OutputPath}/
    // ├── CLAUDE.md
    // ├── .steering/product.md
    // ├── .steering/structure.md
    // ├── .steering/decisions.md
    // └── tasks/tasklist.md

    CString strBase = data.strOutputPath;
    if (strBase.Right(1) != _T("\\") && strBase.Right(1) != _T("/"))
        strBase += _T("\\");

    // サブフォルダ作成
    if (!EnsureDirectory(strBase + _T(".steering"))) return FALSE;
    if (!EnsureDirectory(strBase + _T("tasks")))     return FALSE;

    // 各ファイル生成・書き出し
    if (!WriteFileUtf8(strBase + _T("CLAUDE.md"),                 GenerateCLAUDE(data)))    return FALSE;
    if (!WriteFileUtf8(strBase + _T(".steering\\product.md"),     GenerateProduct(data)))   return FALSE;
    if (!WriteFileUtf8(strBase + _T(".steering\\structure.md"),   GenerateStructure(data))) return FALSE;
    if (!WriteFileUtf8(strBase + _T(".steering\\decisions.md"),   GenerateDecisions(data))) return FALSE;
    if (!WriteFileUtf8(strBase + _T("tasks\\tasklist.md"),        GenerateTasklist(data)))  return FALSE;

    return TRUE;
}

// ============================================================
// CLAUDE.md 生成
// ============================================================
CString CMdGenerator::GenerateCLAUDE(const ProjectData& data)
{
    CString s;
    CString strPlatform = GetPlatformName(data.platform);

    // --- ヘッダ・プロジェクト概要 ---
    CString strHead;
    strHead.Format(
        _T("# CLAUDE.md — プロジェクト指示書\n\n")
        _T("## プロジェクト概要\n\n")
        _T("- **プロジェクト名**：%s\n")
        _T("- **目的**：%s\n")
        _T("- **担当者**：%s\n")
        _T("- **実装言語**：%s\n\n")
        _T("---\n\n"),
        (LPCTSTR)data.strProjectName,
        (LPCTSTR)data.strPurpose,
        (LPCTSTR)data.strOwner,
        (LPCTSTR)strPlatform
    );
    s += strHead;

    // --- Claudeへの基本指示 ---
    s += _T("## Claudeへの基本指示\n\n");
    s += _T("- 応答・コメントは**日本語**で統一すること\n");
    s += _T("- コードのコメントも日本語で記述すること\n");
    s += _T("- 不明点があれば作業前に必ず確認すること\n");
    s += _T("- 破壊的変更（ファイル削除・上書き）は事前に報告し、承認を得てから実行すること\n");
    s += _T("- タスク完了時は `tasks/tasklist.md` の該当項目をチェック済みに更新すること\n");
    if (!data.strClaudeBasic.IsEmpty())
        s += _T("- ") + data.strClaudeBasic + _T("\n");
    s += _T("\n");

    // --- 実装言語別 行動規約（重要） ---
    CString strGuidelines = GetPlatformGuidelines(data.platform);
    if (!strGuidelines.IsEmpty())
    {
        s += _T("### ") + strPlatform + _T("作業時の行動規約（重要）\n\n");
        s += strGuidelines;
        s += _T("\n");
    }

    s += _T("---\n\n");

    // --- ビルド環境・前提 ---
    CString strBuildEnv = GetBuildEnvironment(data.platform);
    if (!strBuildEnv.IsEmpty())
    {
        s += _T("## ビルド環境・前提（変更不可）\n\n");
        s += strBuildEnv;
        s += _T("\n---\n\n");
    }

    // --- 実装言語別 コーディング規約 ---
    CString strCodingRules = GetCodingRules(data.platform);
    if (!strCodingRules.IsEmpty())
    {
        s += _T("## ") + strPlatform + _T("コーディング規約\n\n");
        s += strCodingRules;
        s += _T("\n> 注：実ファイル（リソース定義・各クラス）の内容はこのファイルに転記しない（古くなると害になるため）。\n");
        s += _T("> 作業のたびに実ファイルを参照すること。\n\n");
        s += _T("---\n\n");
    }

    // --- ディレクトリ構成 ---
    s += _T("## ディレクトリ構成\n\n");
    s += _T("```\n");
    s += _T(".\n");
    s += _T("├── CLAUDE.md              # この指示ファイル\n");
    s += _T("├── .steering/             # スペック駆動開発の仕様書群\n");
    s += _T("│   ├── product.md         # プロダクト要件・ゴール定義\n");
    s += _T("│   ├── structure.md       # アーキテクチャ・技術スタック\n");
    s += _T("│   └── decisions.md       # 設計判断の記録（ADR）\n");
    s += _T("├── tasks/\n");
    s += _T("│   └── tasklist.md        # タスク一覧・進捗管理\n");
    s += _T("├── docs/                  # 設計書・仕様書・メモ\n");
    s += _T("└── src/                   # ソースコード\n");
    s += _T("```\n\n");
    s += _T("---\n\n");

    // --- 開発フロー ---
    s += _T("## 開発フロー（スペック駆動）\n\n");
    s += _T("```\n");
    s += _T("1. 要件定義  → .steering/product.md を更新\n");
    s += _T("2. 設計      → .steering/structure.md を更新\n");
    s += _T("3. タスク分解 → tasks/tasklist.md にタスクを追加\n");
    s += _T("4. 実装      → タスクを上から順に実施（作業時の行動規約を厳守）\n");
    s += _T("5. 確認      → タスクをチェック済みに更新\n");
    s += _T("6. 設計判断  → .steering/decisions.md に記録\n");
    s += _T("```\n");

    // --- 禁止事項（ユーザー入力がある場合のみ） ---
    if (!data.strClaudeProhibited.IsEmpty())
    {
        s += _T("\n---\n\n");
        s += _T("## 禁止事項\n\n");
        s += _T("- ") + data.strClaudeProhibited + _T("\n");
    }

    return s;
}

// ============================================================
// product.md 生成
// ============================================================
CString CMdGenerator::GenerateProduct(const ProjectData& data)
{
    CString s;
    s.Format(
        _T("# product.md — プロダクト要件定義\n\n")
        _T("> **用途**：このプロジェクトが「何を・誰のために・なぜ作るか」を定義する。\n")
        _T("> Claudeはタスク実施前にこのファイルを参照し、目的から外れた実装をしないこと。\n\n")
        _T("---\n\n")
        _T("## ゴール\n\n%s\n\n")
        _T("## ターゲットユーザー\n\n%s\n\n")
        _T("## 解決したい課題\n\n%s\n\n")
        _T("## 成功の定義（完了条件）\n\n%s\n\n")
        _T("---\n\n")
        _T("## スコープ\n\n")
        _T("### In Scope（対象）\n%s\n\n")
        _T("### Out of Scope（対象外）\n%s\n\n")
        _T("---\n\n")
        _T("## 制約条件\n\n")
        _T("| 項目 | 内容 |\n")
        _T("|------|------|\n")
        _T("| 言語・環境 | %s |\n")
        _T("| 使用ライブラリ | （未記入） |\n")
        _T("| 納期 | （未記入） |\n")
        _T("| その他 | （未記入） |\n"),
        data.strGoal.IsEmpty()           ? _T("（未記入）") : (LPCTSTR)data.strGoal,
        data.strTargetUser.IsEmpty()     ? _T("（未記入）") : (LPCTSTR)data.strTargetUser,
        data.strProblem.IsEmpty()        ? _T("（未記入）") : (LPCTSTR)data.strProblem,
        data.strSuccessCriteria.IsEmpty()? _T("- [ ] （未記入）") : (LPCTSTR)data.strSuccessCriteria,
        data.strInScope.IsEmpty()        ? _T("- （未記入）") : (LPCTSTR)data.strInScope,
        data.strOutScope.IsEmpty()       ? _T("- （未記入）") : (LPCTSTR)data.strOutScope,
        (LPCTSTR)GetPlatformName(data.platform)
    );
    return s;
}

// ============================================================
// structure.md 生成
// ============================================================
CString CMdGenerator::GenerateStructure(const ProjectData& data)
{
    CString s;
    s.Format(
        _T("# structure.md — アーキテクチャ・技術スタック\n\n")
        _T("> **用途**：技術的な構成・設計方針を定義する。\n")
        _T("> Claudeは実装前にこのファイルを確認し、方針に従ったコードを書くこと。\n\n")
        _T("---\n\n")
        _T("## 技術スタック\n\n")
        _T("%s\n\n")
        _T("%s\n\n")
        _T("---\n\n")
        _T("## 命名規則\n\n")
        _T("| 対象 | 規則 | 例 |\n")
        _T("|------|------|----|  \n")
        _T("%s\n\n")
        _T("---\n\n")
        _T("## コーディング規約\n\n")
        _T("%s\n\n")
        _T("---\n\n")
        _T("## データフロー\n\n")
        _T("```\n%s\n```\n\n")
        _T("---\n\n")
        _T("## 外部依存・連携\n\n")
        _T("| サービス／ファイル | 用途 | 備考 |\n")
        _T("|--------------------|------|------|\n")
        _T("%s\n"),
        (LPCTSTR)GetTechStackTemplate(data.platform),
        data.strTechStackNote.IsEmpty() ? _T("") : (LPCTSTR)(CString(_T("### 補足\n\n")) + data.strTechStackNote),
        data.strNamingRules.IsEmpty()   ? _T("| ファイル名 | （未記入） | |\n| 関数名 | （未記入） | |\n| 変数名 | （未記入） | |") : (LPCTSTR)data.strNamingRules,
        data.strCodingRules.IsEmpty()   ? _T("- （未記入）") : (LPCTSTR)data.strCodingRules,
        data.strDataFlow.IsEmpty()      ? _T("（入力） → （処理） → （出力）") : (LPCTSTR)data.strDataFlow,
        data.strExternalDeps.IsEmpty()  ? _T("| （未記入） | | |") : (LPCTSTR)data.strExternalDeps
    );
    return s;
}

// ============================================================
// decisions.md 生成
// ============================================================
CString CMdGenerator::GenerateDecisions(const ProjectData& data)
{
    CString s;
    s =
        _T("# decisions.md — 設計判断の記録（ADR）\n\n")
        _T("> **用途**：「なぜこの設計にしたか」を記録する。\n")
        _T("> 後から見返したとき・Claudeが判断に迷ったときの根拠として参照する。\n\n")
        _T("---\n\n")
        _T("## 記録フォーマット\n\n")
        _T("```\n")
        _T("## ADR-XXX: タイトル\n")
        _T("- **日付**：YYYY-MM-DD\n")
        _T("- **状態**：提案中 / 採用 / 却下 / 廃止\n")
        _T("- **背景**：なぜこの判断が必要だったか\n")
        _T("- **決定内容**：何を選んだか\n")
        _T("- **却下した選択肢**：何を選ばなかったか・理由\n")
        _T("- **影響**：この決定による制約・副作用\n")
        _T("```\n\n")
        _T("---\n\n")
        _T("## ADR-001: プラットフォームの選択\n\n")
        _T("- **日付**：（記入してください）\n")
        _T("- **状態**：採用\n")
        _T("- **背景**：プロジェクトのターゲット環境を決める必要があった\n");

    CString strDecision;
    strDecision.Format(
        _T("- **決定内容**：%s を採用\n")
        _T("- **却下した選択肢**：（未記入）\n")
        _T("- **影響**：%s に最適化したコードを書くこと\n\n")
        _T("---\n\n")
        _T("<!-- 以下に新しいADRを追加していく -->\n"),
        (LPCTSTR)GetPlatformName(data.platform),
        (LPCTSTR)GetPlatformName(data.platform)
    );
    s += strDecision;
    return s;
}

// ============================================================
// tasklist.md 生成
// ============================================================
CString CMdGenerator::GenerateTasklist(const ProjectData& data)
{
    CString s;
    s =
        _T("# tasklist.md — タスク一覧\n\n")
        _T("> **運用ルール**\n")
        _T("> - タスク完了時は `[ ]` を `[x]` に変更する\n")
        _T("> - 新しいタスクは該当フェーズの末尾に追加する\n")
        _T("> - 優先度：🔴 高 / 🟡 中 / 🟢 低\n\n")
        _T("---\n\n")
        _T("## フェーズ1：仕様定義\n\n")
        _T("- [ ] 🔴 `.steering/product.md` にゴール・要件を記入する\n")
        _T("- [ ] 🔴 `.steering/structure.md` に技術スタックを記入する\n")
        _T("- [ ] 🟡 スコープ（In / Out）を確定する\n")
        _T("- [ ] 🟡 完了条件を具体的に定義する\n");

    if (!data.strTasksPhase1.IsEmpty())
        s += data.strTasksPhase1 + _T("\n");

    s +=
        _T("\n## フェーズ2：設計\n\n")
        _T("- [ ] 🔴 ディレクトリ構成を確定し `structure.md` に反映する\n")
        _T("- [ ] 🔴 データフローを図示する\n")
        _T("- [ ] 🟡 命名規則・コーディング規約を決める\n")
        _T("- [ ] 🟢 `decisions.md` に初期設計判断を記録する\n");

    if (!data.strTasksPhase2.IsEmpty())
        s += data.strTasksPhase2 + _T("\n");

    s +=
        _T("\n## フェーズ3：実装\n\n")
        _T("- [ ] （実装タスクをここに追加する）\n");

    if (!data.strTasksPhase3.IsEmpty())
        s += data.strTasksPhase3 + _T("\n");

    s +=
        _T("\n## フェーズ4：テスト・確認\n\n")
        _T("- [ ] 動作確認（手動）\n")
        _T("- [ ] エラーハンドリングの確認\n")
        _T("- [ ] ドキュメント最終更新\n\n")
        _T("## フェーズ5：完了\n\n")
        _T("- [ ] `product.md` の成功条件をすべて満たしていることを確認\n")
        _T("- [ ] READMEを作成・更新する\n")
        _T("- [ ] GitHubにプッシュ\n\n")
        _T("---\n\n")
        _T("## 完了済み\n\n")
        _T("<!-- 完了タスクをここに移動（任意） -->\n");

    return s;
}

// ============================================================
// プレビュー生成（インデックスで出力ファイルを選択）
// ============================================================
CString CMdGenerator::GeneratePreview(const ProjectData& data, int nIndex)
{
    switch (nIndex)
    {
    case 0: return GenerateCLAUDE(data);
    case 1: return GenerateProduct(data);
    case 2: return GenerateStructure(data);
    case 3: return GenerateDecisions(data);
    case 4: return GenerateTasklist(data);
    default: return _T("");
    }
}

// ============================================================
// プラットフォーム名を返す
// ============================================================
CString CMdGenerator::GetPlatformName(PlatformType pt)
{
    switch (pt)
    {
    case PlatformType::WinForms: return _T("C# / WinForms");
    case PlatformType::WPF:      return _T("C# / WPF");
    case PlatformType::MFC:      return _T("C++ / MFC");
    case PlatformType::Win32:    return _T("C++ / Win32 API");
    case PlatformType::Web:      return _T("Web (HTML/CSS/JS)");
    default:                     return _T("（未選択）");
    }
}

// ============================================================
// プラットフォーム別技術スタックテンプレートを返す
// ============================================================
CString CMdGenerator::GetTechStackTemplate(PlatformType pt)
{
    switch (pt)
    {
    case PlatformType::WinForms:
        return
            _T("| 項目 | 採用技術 | 理由 |\n")
            _T("|------|----------|------|\n")
            _T("| 言語 | C# | .NET標準 |\n")
            _T("| フレームワーク | WinForms (.NET 8以降) | シンプルなデスクトップUI |\n")
            _T("| 主要ライブラリ | （未記入） | |\n")
            _T("| 実行環境 | Windows 10/11 | |\n")
            _T("| バージョン管理 | Git / GitHub | |");

    case PlatformType::WPF:
        return
            _T("| 項目 | 採用技術 | 理由 |\n")
            _T("|------|----------|------|\n")
            _T("| 言語 | C# | .NET標準 |\n")
            _T("| フレームワーク | WPF (.NET 8以降) | XAML/MVVMによるモダンUI |\n")
            _T("| アーキテクチャ | MVVM | データバインディング活用 |\n")
            _T("| 主要ライブラリ | （未記入） | |\n")
            _T("| 実行環境 | Windows 10/11 | |\n")
            _T("| バージョン管理 | Git / GitHub | |");

    case PlatformType::MFC:
        return
            _T("| 項目 | 採用技術 | 理由 |\n")
            _T("|------|----------|------|\n")
            _T("| 言語 | C++ | 組み込み系との親和性 |\n")
            _T("| フレームワーク | MFC (VS2022) | ダイアログベースUI |\n")
            _T("| 文字コード | Unicode (_UNICODE) | 日本語対応 |\n")
            _T("| ビルド | x64 Release | |\n")
            _T("| 実行環境 | Windows 10/11 | |\n")
            _T("| バージョン管理 | Git / GitHub | |");

    case PlatformType::Win32:
        return
            _T("| 項目 | 採用技術 | 理由 |\n")
            _T("|------|----------|------|\n")
            _T("| 言語 | C++ | 軽量・低依存 |\n")
            _T("| フレームワーク | Win32 API | 依存なし・最小構成 |\n")
            _T("| 文字コード | Unicode (TCHAR/WCHAR) | 日本語対応 |\n")
            _T("| ビルド | x64 Release | |\n")
            _T("| 実行環境 | Windows 10/11 | |\n")
            _T("| バージョン管理 | Git / GitHub | |");

    case PlatformType::Web:
        return
            _T("| 項目 | 採用技術 | 理由 |\n")
            _T("|------|----------|------|\n")
            _T("| 言語 | HTML / CSS / JavaScript | Web標準 |\n")
            _T("| フレームワーク | （未記入） | |\n")
            _T("| パッケージ管理 | npm / yarn | |\n")
            _T("| ビルドツール | （未記入） | |\n")
            _T("| 実行環境 | ブラウザ / Node.js | |\n")
            _T("| バージョン管理 | Git / GitHub | |");

    default:
        return
            _T("| 項目 | 採用技術 | 理由 |\n")
            _T("|------|----------|------|\n")
            _T("| 言語 | （未記入） | |\n")
            _T("| フレームワーク | （未記入） | |\n")
            _T("| バージョン管理 | Git / GitHub | |");
    }
}

// ============================================================
// プラットフォーム別 作業時の行動規約を返す（CLAUDE.md用）
// ============================================================
CString CMdGenerator::GetPlatformGuidelines(PlatformType pt)
{
    switch (pt)
    {
    case PlatformType::MFC:
        return
            _T("- **コードを提案・編集する前に、必ず該当の `.rc` と `resource.h` を読むこと**。コントロールIDを推測で書かない\n")
            _T("- ダイアログクラスを編集するときは、対応するヘッダ（メンバ変数とDDX定義）も併せて確認すること\n")
            _T("- 新規のリソースID（`IDC_` / `IDD_` / `IDR_`）を勝手に発番せず、既存IDを使うこと。新規発番が必要な場合は理由とともに確認を求めること\n")
            _T("- メッセージマップ（`BEGIN_MESSAGE_MAP`〜`END_MESSAGE_MAP`）は既存ブロックを確認してから追記し、マクロの省略・重複をしないこと\n")
            _T("- ビルド構成・プロジェクト設定（プラットフォーム、文字セット、MFCリンク方式）を変更しないこと\n")
            _T("- ビルドはClaudeが実行せず、VS2022側で行う前提とする（エラーが出たらテキストを貼って共有する運用）\n");

    case PlatformType::Win32:
        return
            _T("- ウィンドウプロシージャ（`WndProc`）を編集するときは、既存のメッセージ分岐を確認し、`default:` の `DefWindowProc` 呼び出しを必ず維持すること\n")
            _T("- リソースID（`IDR_` / `IDM_` / `IDC_`）を勝手に発番せず、既存IDを使うこと\n")
            _T("- ハンドル（`HWND` / `HDC` / `HBRUSH` 等）の生成と破棄を対で行い、`GetDC`/`ReleaseDC`、`SelectObject` 後の旧オブジェクト復元などを徹底すること\n")
            _T("- Win32 API呼び出しの戻り値・`GetLastError()` を必要に応じて確認すること\n")
            _T("- ビルド構成・プロジェクト設定（文字セット、ターゲットアーキテクチャ）を変更しないこと\n")
            _T("- ビルドはClaudeが実行せず、VS2022側で行う前提とする\n");

    case PlatformType::WinForms:
        return
            _T("- Designer自動生成コード（`*.Designer.cs`）を手書きで編集しないこと。UI変更はデザイナ側で行う前提\n")
            _T("- イベントハンドラの追加・削除はDesigner経由で行うか、命名規則（`コントロール名_イベント名`）を維持すること\n")
            _T("- `.csproj` のターゲットフレームワーク・出力設定を勝手に変更しないこと\n")
            _T("- ビルドはClaudeが実行せず、VS2022側で行う前提とする\n");

    case PlatformType::WPF:
        return
            _T("- XAMLとコードビハインドの責務分離を維持すること。ロジックは可能な限りViewModelに寄せ、View側に書かない\n")
            _T("- MVVMパターンに従い、UIスレッド外からのUIアクセスは `Dispatcher.Invoke` 経由とすること\n")
            _T("- リソースディクショナリ・スタイルを編集するときは、対象スコープ（App/Window/Control）を確認すること\n")
            _T("- `.csproj` のターゲットフレームワーク・出力設定を勝手に変更しないこと\n")
            _T("- ビルドはClaudeが実行せず、VS2022側で行う前提とする\n");

    case PlatformType::Web:
        return
            _T("- パッケージ追加・更新は事前に確認を求めること（`package.json` の自動編集は不可）\n")
            _T("- ビルドツール（webpack / vite 等）の設定を勝手に変更しないこと\n")
            _T("- 外部CDN・サードパーティスクリプトの追加は、用途と取得元を明示してから行うこと\n")
            _T("- XSS / CSRF などの基本的なセキュリティに配慮したコードを書くこと\n");

    default:
        return _T("");
    }
}

// ============================================================
// プラットフォーム別 ビルド環境テーブルを返す（CLAUDE.md用）
// ============================================================
CString CMdGenerator::GetBuildEnvironment(PlatformType pt)
{
    switch (pt)
    {
    case PlatformType::MFC:
        return
            _T("| 項目 | 設定 |\n")
            _T("|------|------|\n")
            _T("| IDE / ビルド | VS2022 + MSBuild |\n")
            _T("| MFCリンク方式 | 共有DLL（static MFCではない） |\n")
            _T("| 文字セット | Unicode（`_UNICODE` / `UNICODE` 定義済み）。`CString` は実質 `CStringW` |\n")
            _T("| ターゲットアーキテクチャ | x64 |\n")
            _T("| C++標準 | C++17 |\n");

    case PlatformType::Win32:
        return
            _T("| 項目 | 設定 |\n")
            _T("|------|------|\n")
            _T("| IDE / ビルド | VS2022 + MSBuild |\n")
            _T("| 文字セット | Unicode（`_UNICODE` / `UNICODE` 定義済み）。文字列は `TCHAR` / `WCHAR` |\n")
            _T("| ターゲットアーキテクチャ | x64 |\n")
            _T("| C++標準 | C++17 |\n");

    case PlatformType::WinForms:
        return
            _T("| 項目 | 設定 |\n")
            _T("|------|------|\n")
            _T("| IDE / ビルド | VS2022 + MSBuild / `dotnet build` |\n")
            _T("| フレームワーク | .NET 8 以降 |\n")
            _T("| 言語バージョン | C# 12 |\n")
            _T("| ターゲットアーキテクチャ | x64 |\n");

    case PlatformType::WPF:
        return
            _T("| 項目 | 設定 |\n")
            _T("|------|------|\n")
            _T("| IDE / ビルド | VS2022 + MSBuild / `dotnet build` |\n")
            _T("| フレームワーク | .NET 8 以降 |\n")
            _T("| 言語バージョン | C# 12 |\n")
            _T("| アーキテクチャ方針 | MVVM |\n")
            _T("| ターゲットアーキテクチャ | x64 |\n");

    case PlatformType::Web:
        return
            _T("| 項目 | 設定 |\n")
            _T("|------|------|\n")
            _T("| ランタイム | Node.js（LTS） |\n")
            _T("| パッケージ管理 | npm / yarn（プロジェクト方針に従う） |\n")
            _T("| ビルドツール | （プロジェクトで決定） |\n")
            _T("| 実行環境 | モダンブラウザ |\n");

    default:
        return _T("");
    }
}

// ============================================================
// プラットフォーム別 コーディング規約を返す（CLAUDE.md用）
// ============================================================
CString CMdGenerator::GetCodingRules(PlatformType pt)
{
    switch (pt)
    {
    case PlatformType::MFC:
        return
            _T("- 文字列リテラルは `_T(\"...\")` または `L\"...\"` で統一する。生の `\"...\"` は使わない\n")
            _T("- DDX/DDVの向きに注意する\n")
            _T("  - `UpdateData(TRUE)`：画面 → 変数（取得）\n")
            _T("  - `UpdateData(FALSE)`：変数 → 画面（設定）\n")
            _T("- リソースID命名規則を守る：`IDD_`（ダイアログ）／`IDC_`（コントロール）／`IDR_`（リソース）\n")
            _T("- `new` した `CWnd` 派生オブジェクトの破棄責任・親子ウィンドウの所有権を明示的に管理する\n")
            _T("- エラー処理の方針はプロジェクト内で統一する（MFC例外 `CException` 派生 / 標準例外のどちらかに寄せる）\n")
            _T("- ファイル出力はUTF-8（BOMの有無はプロジェクト方針に合わせる）\n");

    case PlatformType::Win32:
        return
            _T("- 文字列リテラルは `_T(\"...\")` または `L\"...\"` で統一する。生の `\"...\"` は使わない\n")
            _T("- ハンドル変数は使用後に必ず `Close*` / `Destroy*` / `Release*` で解放する\n")
            _T("- `GetDC` / `BeginPaint` は対応する `ReleaseDC` / `EndPaint` と必ずペアにする\n")
            _T("- メッセージ処理関数の戻り値（`LRESULT`）を正しく返す\n")
            _T("- API戻り値・`GetLastError()` を確認し、失敗時の挙動を明示する\n");

    case PlatformType::WinForms:
        return
            _T("- 命名規則は .NET 標準（型・メソッド・プロパティは PascalCase、フィールドは `_camelCase`）\n")
            _T("- I/O・長時間処理は `async` / `await` を使い、UIスレッドをブロックしない\n")
            _T("- nullable 参照型を有効化し、null 安全を意識する\n")
            _T("- リソース（`Form` / `Stream` / `IDisposable`）は `using` で確実に破棄する\n")
            _T("- ファイル出力はUTF-8（BOMの有無はプロジェクト方針に合わせる）\n");

    case PlatformType::WPF:
        return
            _T("- 命名規則は .NET 標準（型・メソッド・プロパティは PascalCase、フィールドは `_camelCase`）\n")
            _T("- ViewModelは `INotifyPropertyChanged` を実装し、ロジックはここに集約する\n")
            _T("- `ICommand`（`RelayCommand` 等）でユーザー操作をバインドする\n")
            _T("- I/O・長時間処理は `async` / `await`、UI更新は `Dispatcher` 経由で行う\n")
            _T("- リソース（`Stream` / `IDisposable`）は `using` で確実に破棄する\n");

    case PlatformType::Web:
        return
            _T("- ES2020以降の構文を使用する。`var` は使わず `const` / `let` を使う\n")
            _T("- 命名規則：変数・関数は camelCase、コンポーネント・クラスは PascalCase、定数は UPPER_SNAKE_CASE\n")
            _T("- 非同期処理は `async` / `await` を使用する\n")
            _T("- 入力値のサニタイズ・XSS対策を行う\n")
            _T("- ソースの文字コードはUTF-8\n");

    default:
        return _T("");
    }
}

// ============================================================
// UTF-8（BOMなし）でファイルに書き出す
// ============================================================
BOOL CMdGenerator::WriteFileUtf8(const CString& strPath, const CString& strContent)
{
    // CString (UTF-16) → UTF-8 変換
    int nLen = WideCharToMultiByte(
        CP_UTF8, 0,
        strContent, strContent.GetLength(),
        nullptr, 0, nullptr, nullptr);

    if (nLen <= 0) return FALSE;

    std::vector<char> buf(nLen);
    WideCharToMultiByte(
        CP_UTF8, 0,
        strContent, strContent.GetLength(),
        buf.data(), nLen, nullptr, nullptr);

    // ファイル書き込み
    FILE* fp = nullptr;
    if (_wfopen_s(&fp, strPath, L"wb") != 0 || fp == nullptr)
    {
        AfxMessageBox(_T("ファイルの書き込みに失敗しました：\n") + strPath, MB_OK | MB_ICONERROR);
        return FALSE;
    }
    fwrite(buf.data(), 1, nLen, fp);
    fclose(fp);
    return TRUE;
}

// ============================================================
// フォルダが存在しない場合は作成する
// ============================================================
BOOL CMdGenerator::EnsureDirectory(const CString& strPath)
{
    if (CreateDirectory(strPath, nullptr)) return TRUE;
    if (GetLastError() == ERROR_ALREADY_EXISTS) return TRUE;

    AfxMessageBox(_T("フォルダの作成に失敗しました：\n") + strPath, MB_OK | MB_ICONERROR);
    return FALSE;
}
