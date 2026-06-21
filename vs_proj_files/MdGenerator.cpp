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
    s.Format(
        _T("# CLAUDE.md — プロジェクト指示書\n\n")
        _T("## プロジェクト概要\n\n")
        _T("- **プロジェクト名**：%s\n")
        _T("- **目的**：%s\n")
        _T("- **担当者**：%s\n")
        _T("- **プラットフォーム**：%s\n\n")
        _T("---\n\n")
        _T("## Claudeへの基本指示\n\n")
        _T("- 応答・コメントは**日本語**で統一すること\n")
        _T("- コードのコメントも日本語で記述すること\n")
        _T("- 不明点があれば作業前に必ず確認すること\n")
        _T("- 破壊的変更（ファイル削除・上書き）は事前に報告し、承認を得てから実行すること\n")
        _T("- タスク完了時は `tasks/tasklist.md` の該当項目をチェック済みに更新すること\n")
        _T("%s\n\n")
        _T("---\n\n")
        _T("## ディレクトリ構成\n\n")
        _T("```\n")
        _T(".\n")
        _T("├── CLAUDE.md              # この指示ファイル\n")
        _T("├── .steering/             # スペック駆動開発の仕様書群\n")
        _T("│   ├── product.md         # プロダクト要件・ゴール定義\n")
        _T("│   ├── structure.md       # アーキテクチャ・技術スタック\n")
        _T("│   └── decisions.md       # 設計判断の記録（ADR）\n")
        _T("├── tasks/\n")
        _T("│   └── tasklist.md        # タスク一覧・進捗管理\n")
        _T("├── docs/                  # 設計書・仕様書・メモ\n")
        _T("└── src/                   # ソースコード\n")
        _T("```\n\n")
        _T("---\n\n")
        _T("## 開発フロー（スペック駆動）\n\n")
        _T("```\n")
        _T("1. 要件定義  → .steering/product.md を更新\n")
        _T("2. 設計      → .steering/structure.md を更新\n")
        _T("3. タスク分解 → tasks/tasklist.md にタスクを追加\n")
        _T("4. 実装      → タスクを上から順に実施\n")
        _T("5. 確認      → タスクをチェック済みに更新\n")
        _T("6. 設計判断  → .steering/decisions.md に記録\n")
        _T("```\n\n")
        _T("---\n\n")
        _T("## 禁止事項\n\n")
        _T("- `.steering/` 内のファイルを無断で書き換えないこと\n")
        _T("- `tasks/tasklist.md` のタスクを無断で削除しないこと\n")
        _T("- 未確認の外部ライブラリを追加しないこと\n")
        _T("%s\n"),
        (LPCTSTR)data.strProjectName,
        (LPCTSTR)data.strPurpose,
        (LPCTSTR)data.strOwner,
        (LPCTSTR)GetPlatformName(data.platform),
        data.strClaudeBasic.IsEmpty() ? _T("") : (LPCTSTR)(CString(_T("- ")) + data.strClaudeBasic),
        data.strClaudeProhibited.IsEmpty() ? _T("") : (LPCTSTR)(CString(_T("- ")) + data.strClaudeProhibited)
    );
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
