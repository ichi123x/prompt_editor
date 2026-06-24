// CLI Markdown generator (cross-platform, no MFC)
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

struct ProjectData
{
    string strProjectName;
    string strPurpose;
    string strOwner;
    int platform = 2; // 0=WinForms,1=WPF,2=MFC,3=Win32,4=Web
    string strGoal;
    string strTargetUser;
    string strProblem;
    string strSuccessCriteria;
    string strInScope;
    string strOutScope;
    string strTechStackNote;
    string strNamingRules;
    string strCodingRules;
    string strDataFlow;
    string strExternalDeps;
    string strTasksPhase1;
    string strTasksPhase2;
    string strTasksPhase3;
    string strClaudeBasic;
    string strClaudeProhibited;
};

static string GetPlatformName(int pt)
{
    switch (pt)
    {
    case 0: return "C# / WinForms";
    case 1: return "C# / WPF";
    case 2: return "C++ / MFC";
    case 3: return "C++ / Win32 API";
    case 4: return "Web (HTML/CSS/JS)";
    default: return "(未選択)";
    }
}

static bool EnsureDirectory(const fs::path& p)
{
    error_code ec;
    if (fs::exists(p, ec)) return true;
    return fs::create_directories(p, ec);
}

static bool WriteFileUtf8(const fs::path& path, const string& content)
{
    ofstream ofs(path, ios::binary);
    if (!ofs) return false;
    ofs.write(content.data(), (streamsize)content.size());
    return ofs.good();
}

static string GenerateCLAUDE(const ProjectData& d)
{
    string s;
    s += "# CLAUDE.md — プロジェクト指示書\n\n";
    s += "## プロジェクト概要\n\n";
    s += "- **プロジェクト名**：" + (d.strProjectName.empty() ? string("（未記入）") : d.strProjectName) + "\n";
    s += "- **目的**：" + (d.strPurpose.empty() ? string("（未記入）") : d.strPurpose) + "\n";
    s += "- **担当者**：" + (d.strOwner.empty() ? string("（未記入）") : d.strOwner) + "\n";
    s += "- **プラットフォーム**：" + GetPlatformName(d.platform) + "\n\n";
    s += "---\n\n";
    s += "## Claudeへの基本指示\n\n";
    s += "- 応答・コメントは**日本語**で統一すること\n";
    s += "- コードのコメントも日本語で記述すること\n";
    s += "- 不明点があれば作業前に必ず確認すること\n";
    s += "- 破壊的変更（ファイル削除・上書き）は事前に報告し、承認を得てから実行すること\n";
    s += "- タスク完了時は `tasks/tasklist.md` の該当項目をチェック済みに更新すること\n";
    if (!d.strClaudeBasic.empty()) s += "- " + d.strClaudeBasic + "\n";
    s += "\n---\n\n";
    s += "## ディレクトリ構成\n\n";
    s += "ディレクトリ構成例:\n\n";
    s += "./\n";
    s += "- CLAUDE.md\n";
    s += "- .steering/\n";
    s += "  - product.md\n";
    s += "  - structure.md\n";
    s += "  - decisions.md\n";
    s += "- tasks/\n";
    s += "  - tasklist.md\n";
    s += "- docs/\n";
    s += "- src/\n\n";
    if (!d.strClaudeProhibited.empty()) s += "- " + d.strClaudeProhibited + "\n";
    return s;
}

static string GenerateProduct(const ProjectData& d)
{
    string s;
    s += "# product.md — プロダクト要件定義\n\n";
    s += "> **用途**：このプロジェクトが「何を・誰のために・なぜ作るか」を定義する。\n\n";
    s += "---\n\n";
    s += "## ゴール\n\n" + (d.strGoal.empty() ? string("（未記入）") : d.strGoal) + "\n\n";
    s += "## ターゲットユーザー\n\n" + (d.strTargetUser.empty() ? string("（未記入）") : d.strTargetUser) + "\n\n";
    s += "## 解決したい課題\n\n" + (d.strProblem.empty() ? string("（未記入）") : d.strProblem) + "\n\n";
    s += "## 成功の定義（完了条件）\n\n" + (d.strSuccessCriteria.empty() ? string("- [ ] （未記入）") : d.strSuccessCriteria) + "\n\n";
    s += "---\n\n";
    s += "## スコープ\n\n";
    s += "### In Scope（対象）\n" + (d.strInScope.empty() ? string("- （未記入）") : d.strInScope) + "\n\n";
    s += "### Out of Scope（対象外）\n" + (d.strOutScope.empty() ? string("- （未記入）") : d.strOutScope) + "\n\n";
    s += "---\n\n";
    s += "## 制約条件\n\n";
    s += "| 項目 | 内容 |\n|------|------|\n| 言語・環境 | " + GetPlatformName(d.platform) + " |\n";
    return s;
}

static string GenerateStructure(const ProjectData& d)
{
    string s;
    s += "# structure.md — アーキテクチャ・技術スタック\n\n";
    s += "> **用途**：技術的な構成・設計方針を定義する。\n\n";
    s += "---\n\n";
    s += "## 技術スタック\n\n";
    s += (d.strTechStackNote.empty() ? string("（未記入）") : d.strTechStackNote) + "\n\n";
    return s;
}

static string GenerateDecisions(const ProjectData& d)
{
    string s;
    s += "# decisions.md — 設計判断の記録（ADR）\n\n";
    s += "---\n\n";
    s += "## ADR-001: プラットフォームの選択\n\n";
    s += "- **決定内容**：" + GetPlatformName(d.platform) + " を採用\n";
    return s;
}

static string GenerateTasklist(const ProjectData& d)
{
    string s;
    s += "# tasklist.md — タスク一覧\n\n";
    s += "## フェーズ1：仕様定義\n\n";
    s += "- [ ] `.steering/product.md` にゴール・要件を記入する\n";
    if (!d.strTasksPhase1.empty()) s += d.strTasksPhase1 + "\n";
    s += "\n## フェーズ2：設計\n\n";
    if (!d.strTasksPhase2.empty()) s += d.strTasksPhase2 + "\n";
    s += "\n## フェーズ3：実装\n\n";
    if (!d.strTasksPhase3.empty()) s += d.strTasksPhase3 + "\n";
    return s;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cerr << "Usage: cli_md_generator <output_dir> [project_name] [purpose] [owner] [platformIndex]\n";
        return 1;
    }

    fs::path out = argv[1];
    ProjectData d;
    if (argc >= 3) d.strProjectName = argv[2];
    if (argc >= 4) d.strPurpose = argv[3];
    if (argc >= 5) d.strOwner = argv[4];
    if (argc >= 6) d.platform = atoi(argv[5]);

    // create directories
    if (!EnsureDirectory(out)) { cerr << "Failed to create output dir: " << out << "\n"; return 2; }
    if (!EnsureDirectory(out / ".steering")) { cerr << "Failed to create .steering\n"; return 2; }
    if (!EnsureDirectory(out / "tasks"))     { cerr << "Failed to create tasks\n"; return 2; }

    bool ok = true;
    ok &= WriteFileUtf8(out / "CLAUDE.md", GenerateCLAUDE(d));
    ok &= WriteFileUtf8(out / ".steering" / "product.md", GenerateProduct(d));
    ok &= WriteFileUtf8(out / ".steering" / "structure.md", GenerateStructure(d));
    ok &= WriteFileUtf8(out / ".steering" / "decisions.md", GenerateDecisions(d));
    ok &= WriteFileUtf8(out / "tasks" / "tasklist.md", GenerateTasklist(d));

    if (!ok) { cerr << "One or more files failed to write.\n"; return 3; }

    cout << "Generated MD files to: " << out << "\n";
    return 0;
}
