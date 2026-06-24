#pragma once
// MdGenerator.h — Markdownファイル生成クラス宣言

#include <afxwin.h>

// プラットフォーム種別
enum class PlatformType
{
    WinForms = 0,
    WPF,
    MFC,
    Win32,
    Web
};

// ユーザー入力データをまとめる構造体
struct ProjectData
{
    // 共通
    CString strProjectName;     // プロジェクト名
    CString strPurpose;         // 目的
    CString strOwner;           // 担当者
    PlatformType platform;      // プラットフォーム種別
    CString strOutputPath;      // 出力先フォルダ

    // 要件定義タブ
    CString strGoal;            // ゴール
    CString strTargetUser;      // ターゲットユーザー
    CString strProblem;         // 解決したい課題
    CString strSuccessCriteria; // 成功の定義
    CString strInScope;         // In Scope
    CString strOutScope;        // Out of Scope

    // 設計タブ
    CString strTechStackNote;   // 技術スタック補足
    CString strNamingRules;     // 命名規則
    CString strCodingRules;     // コーディング規約
    CString strDataFlow;        // データフロー
    CString strExternalDeps;    // 外部依存

    // タスク管理タブ
    CString strTasksPhase1;     // フェーズ1タスク
    CString strTasksPhase2;     // フェーズ2タスク
    CString strTasksPhase3;     // フェーズ3タスク

    // CLAUDE.mdタブ
    CString strClaudeBasic;     // 基本指示（追加分）
    CString strClaudeProhibited;// 禁止事項（追加分）
};

// Markdown生成クラス
class CMdGenerator
{
public:
    CMdGenerator();
    ~CMdGenerator();

    // 全mdファイルを一括生成する（メインエントリポイント）
    BOOL GenerateAll(const ProjectData& data);

    // プレビュー生成（0=CLAUDE.md / 1=product / 2=structure / 3=decisions / 4=tasklist）
    CString GeneratePreview(const ProjectData& data, int nIndex);

private:
    // 各mdファイルの内容生成
    CString GenerateCLAUDE(const ProjectData& data);
    CString GenerateProduct(const ProjectData& data);
    CString GenerateStructure(const ProjectData& data);
    CString GenerateDecisions(const ProjectData& data);
    CString GenerateTasklist(const ProjectData& data);

    // プラットフォーム名を文字列で返す
    CString GetPlatformName(PlatformType pt);

    // プラットフォーム別の技術スタックテンプレートを返す
    CString GetTechStackTemplate(PlatformType pt);

    // プラットフォーム別の作業時行動規約を返す（CLAUDE.md用）
    CString GetPlatformGuidelines(PlatformType pt);

    // プラットフォーム別のビルド環境テーブルを返す（CLAUDE.md用）
    CString GetBuildEnvironment(PlatformType pt);

    // プラットフォーム別のコーディング規約を返す（CLAUDE.md用）
    CString GetCodingRules(PlatformType pt);

    // UTF-8（BOMなし）でファイルに書き出す
    BOOL WriteFileUtf8(const CString& strPath, const CString& strContent);

    // フォルダが存在しない場合は作成する
    BOOL EnsureDirectory(const CString& strPath);
};
