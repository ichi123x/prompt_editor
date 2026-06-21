#pragma once
// PromptEditorDlg.h — メインダイアログクラス宣言

#include "afxdialogex.h"
#include "resource.h"
#include "MdGenerator.h"

// ============================================================
// CPromptEditorDlg — メインダイアログ
// ============================================================
class CPromptEditorDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CPromptEditorDlg)

public:
    CPromptEditorDlg(CWnd* pParent = nullptr);
    virtual ~CPromptEditorDlg();

    enum { IDD = IDD_PROMPT_EDITOR_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual BOOL OnInitDialog() override;
    virtual void OnOK() override {}   // Enterキーで閉じないようにする
    DECLARE_MESSAGE_MAP()

private:
    // ---- コントロール変数 ----

    // 共通入力
    CEdit   m_editProjectName;
    CEdit   m_editPurpose;
    CEdit   m_editOwner;

    // プラットフォームラジオボタン（グループの先頭変数で管理）
    int     m_nPlatform;   // 0=WinForms 1=WPF 2=MFC 3=Win32 4=Web

    // タブコントロール
    CTabCtrl m_tabMain;

    // 出力先
    CEdit   m_editOutputPath;

    // ---- タブ別 入力EditBox ----

    // 要件定義タブ
    CEdit   m_editGoal;
    CEdit   m_editTargetUser;
    CEdit   m_editProblem;
    CEdit   m_editSuccessCriteria;
    CEdit   m_editInScope;
    CEdit   m_editOutScope;

    // 設計タブ
    CEdit   m_editTechStackNote;
    CEdit   m_editNamingRules;
    CEdit   m_editCodingRules;
    CEdit   m_editDataFlow;
    CEdit   m_editExternalDeps;

    // タスク管理タブ
    CEdit   m_editTasksPhase1;
    CEdit   m_editTasksPhase2;
    CEdit   m_editTasksPhase3;

    // CLAUDE.mdタブ
    CEdit   m_editClaudeBasic;
    CEdit   m_editClaudeProhibited;

    // ---- タブ内子ダイアログ ----
    CDialog* m_pTabDlg[5];   // 各タブの子ダイアログポインタ（+プレビュー）
    int      m_nCurrentTab;  // 現在表示中のタブインデックス

    // ---- 内部メソッド ----
    void InitTabControl();                     // タブコントロール初期化
    void ShowTab(int nTab);                    // 指定タブを表示
    void CollectData(ProjectData& data);       // 入力値を ProjectData に収集
    BOOL ValidateInput(const ProjectData& d);  // 入力バリデーション
    void RefreshPreview();                     // プレビュータブ内容を更新

    // ---- イベントハンドラ ----
    afx_msg void OnBnClickedBrowse();          // フォルダ参照ボタン
    afx_msg void OnBnClickedGenerate();        // 生成ボタン
    afx_msg void OnBnClickedClose();           // 閉じるボタン
    afx_msg void OnBnClickedPreviewRefresh();  // 更新ボタン
    afx_msg void OnTcnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult); // タブ切り替え
    afx_msg void OnSize(UINT nType, int cx, int cy);                     // サイズ変更
};
