#pragma once
// PromptEditorDlg.h — メインダイアログクラス宣言

#include "afxdialogex.h"
#include "resource.h"
#include "MdGenerator.h"

// プレビュー更新を遅延実行するためのユーザー定義メッセージ
// （コンボボックスのドロップダウン破棄処理と競合しないようにするため）
#define WM_APP_REFRESH_PREVIEW (WM_APP + 1)

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

    // フォアグラウンド復帰時に再ポストすべきプレビュー更新が保留中かどうか
    BOOL     m_bPendingPreviewRefresh;

    // ---- 内部メソッド ----
    void InitTabControl();                     // タブコントロール初期化
    void ShowTab(int nTab);                    // 指定タブを表示
    void CollectData(ProjectData& data);       // 入力値を ProjectData に収集
    BOOL ValidateInput(const ProjectData& d);  // 入力バリデーション
    void RefreshPreview();                     // プレビュータブ内容を更新

    friend class CPreviewDialog;

    // ---- イベントハンドラ ----
    afx_msg void OnDestroy();                   // ダイアログ破棄時（設定保存）
    afx_msg void OnBnClickedBrowse();          // フォルダ参照ボタン
    afx_msg void OnBnClickedGenerate();        // 生成ボタン
    afx_msg void OnBnClickedClose();           // 閉じるボタン
    afx_msg void OnBnClickedPreviewRefresh();  // 更新ボタン
    afx_msg void OnTcnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult); // タブ切り替え
    afx_msg void OnSize(UINT nType, int cx, int cy);                     // サイズ変更
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized); // Z-order救済
    afx_msg LRESULT OnRefreshPreviewMessage(WPARAM wParam, LPARAM lParam);        // プレビュー遅延更新
};
