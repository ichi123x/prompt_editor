// PromptEditorDlg.cpp — メインダイアログ実装
#include "pch.h"
#include "PromptEditorDlg.h"
#include "TabDialog.h"
#include "resource.h"
#include <ShlObj.h>   // SHBrowseForFolder

#pragma comment(lib, "Shell32.lib")

IMPLEMENT_DYNAMIC(CPromptEditorDlg, CDialogEx)

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================
CPromptEditorDlg::CPromptEditorDlg(CWnd* pParent)
    : CDialogEx(IDD_PROMPT_EDITOR_DIALOG, pParent)
    , m_nPlatform(2)        // デフォルト：MFC
    , m_nCurrentTab(0)
{
    // タブ子ダイアログ初期化
    for (int i = 0; i < 4; i++)
        m_pTabDlg[i] = nullptr;
}

CPromptEditorDlg::~CPromptEditorDlg()
{
    for (int i = 0; i < 5; i++)
    {
        if (m_pTabDlg[i])
        {
            delete m_pTabDlg[i];
            m_pTabDlg[i] = nullptr;
        }
    }
}

// ============================================================
// DoDataExchange — DDX/DDVバインディング
// ============================================================
void CPromptEditorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    // 共通入力
    DDX_Control(pDX, IDC_EDIT_PROJECT_NAME, m_editProjectName);
    DDX_Control(pDX, IDC_EDIT_PURPOSE,      m_editPurpose);
    DDX_Control(pDX, IDC_EDIT_OWNER,        m_editOwner);

    // プラットフォームラジオボタン（先頭IDから連番で管理）
    DDX_Radio(pDX, IDC_RADIO_WINFORMS, m_nPlatform);

    // タブコントロール
    DDX_Control(pDX, IDC_TAB_MAIN, m_tabMain);

    // 出力先
    DDX_Control(pDX, IDC_EDIT_OUTPUT_PATH, m_editOutputPath);
}

BEGIN_MESSAGE_MAP(CPromptEditorDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_BROWSE,           &CPromptEditorDlg::OnBnClickedBrowse)
    ON_BN_CLICKED(IDC_BUTTON_GENERATE,         &CPromptEditorDlg::OnBnClickedGenerate)
    ON_BN_CLICKED(IDC_BUTTON_CLOSE,            &CPromptEditorDlg::OnBnClickedClose)
    ON_BN_CLICKED(IDC_BUTTON_PREVIEW_REFRESH,  &CPromptEditorDlg::OnBnClickedPreviewRefresh)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CPromptEditorDlg::OnTcnSelchangeTabMain)
    ON_WM_SIZE()
END_MESSAGE_MAP()

// ============================================================
// OnInitDialog — ダイアログ初期化
// ============================================================
BOOL CPromptEditorDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetWindowText(_T("prompt_editor — スペック駆動MDジェネレーター"));

    // デフォルト値設定
    m_editOwner.SetWindowText(_T("ihira"));

    // ラジオボタン初期選択（MFC）
    CheckRadioButton(IDC_RADIO_WINFORMS, IDC_RADIO_WEB, IDC_RADIO_MFC);
    m_nPlatform = 2;

    // タブコントロール初期化
    InitTabControl();

    return TRUE;
}

// ============================================================
// InitTabControl — タブコントロールにタブを追加し子ダイアログを生成
// ============================================================
void CPromptEditorDlg::InitTabControl()
{
    // タブ追加
    m_tabMain.InsertItem(0, _T("要件定義"));
    m_tabMain.InsertItem(1, _T("設計"));
    m_tabMain.InsertItem(2, _T("タスク管理"));
    m_tabMain.InsertItem(3, _T("CLAUDE.md"));
    m_tabMain.InsertItem(4, _T("プレビュー"));

    // 子ダイアログ生成（各タブのIDD_TAB_xxx）
    static const UINT arrIDD[5] = {
        IDD_TAB_REQUIREMENTS,
        IDD_TAB_DESIGN,
        IDD_TAB_TASKS,
        IDD_TAB_CLAUDE,
        IDD_TAB_PREVIEW
    };

    // タブコントロールのクライアント領域を取得
    CRect rcTab;
    m_tabMain.GetClientRect(&rcTab);
    m_tabMain.AdjustRect(FALSE, &rcTab);

    // タブコントロールをスクリーン座標→クライアント座標に変換
    CRect rcTabCtrl;
    m_tabMain.GetWindowRect(&rcTabCtrl);
    ScreenToClient(&rcTabCtrl);
    rcTab.OffsetRect(rcTabCtrl.left, rcTabCtrl.top);

    for (int i = 0; i < 5; i++)
    {
        if (i < 4)
        {
            m_pTabDlg[i] = new CTabDialog(arrIDD[i], this);
        }
        else
        {
            m_pTabDlg[i] = new CDialog(arrIDD[i], this);
        }

        m_pTabDlg[i]->Create(arrIDD[i], &m_tabMain);
        m_pTabDlg[i]->MoveWindow(&rcTab);
        m_pTabDlg[i]->ShowWindow(i == 0 ? SW_SHOW : SW_HIDE);
    }

    m_nCurrentTab = 0;
}

// ============================================================
// ShowTab — 指定タブの子ダイアログを表示
// ============================================================
void CPromptEditorDlg::ShowTab(int nTab)
{
    if (m_nCurrentTab == nTab) return;

    if (m_pTabDlg[m_nCurrentTab])
        m_pTabDlg[m_nCurrentTab]->ShowWindow(SW_HIDE);

    m_nCurrentTab = nTab;

    if (m_pTabDlg[m_nCurrentTab])
        m_pTabDlg[m_nCurrentTab]->ShowWindow(SW_SHOW);

    if (nTab == 4)
        RefreshPreview();
}

// ============================================================
// OnTcnSelchangeTabMain — タブ切り替えイベント
// ============================================================
void CPromptEditorDlg::OnTcnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult)
{
    int nSel = m_tabMain.GetCurSel();
    if (nSel >= 0 && nSel < 5)
        ShowTab(nSel);
    *pResult = 0;
}

// ============================================================
// OnBnClickedBrowse — フォルダ参照
// ============================================================
void CPromptEditorDlg::OnBnClickedBrowse()
{
    BROWSEINFO bi = {};
    bi.hwndOwner = GetSafeHwnd();
    bi.lpszTitle = _T("出力先フォルダを選択してください");
    bi.ulFlags   = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl)
    {
        TCHAR szPath[MAX_PATH] = {};
        if (SHGetPathFromIDList(pidl, szPath))
            m_editOutputPath.SetWindowText(szPath);
        CoTaskMemFree(pidl);
    }
}

// ============================================================
// CollectData — 入力値を ProjectData に収集する
// ============================================================
void CPromptEditorDlg::CollectData(ProjectData& data)
{
    // 共通
    m_editProjectName.GetWindowText(data.strProjectName);
    m_editPurpose.GetWindowText(data.strPurpose);
    m_editOwner.GetWindowText(data.strOwner);
    m_editOutputPath.GetWindowText(data.strOutputPath);

    // プラットフォーム
    UpdateData(TRUE);   // DDXでm_nPlatformを更新
    data.platform = static_cast<PlatformType>(m_nPlatform);

    // 各タブの入力値は子ダイアログから取得
    auto GetTabEdit = [&](int nTab, int nCtrlID, CString& str)
    {
        if (m_pTabDlg[nTab])
        {
            CWnd* pWnd = m_pTabDlg[nTab]->GetDlgItem(nCtrlID);
            if (pWnd) pWnd->GetWindowText(str);
        }
    };

    // 要件定義タブ (index=0)
    GetTabEdit(0, IDC_EDIT_GOAL,             data.strGoal);
    GetTabEdit(0, IDC_EDIT_TARGET_USER,      data.strTargetUser);
    GetTabEdit(0, IDC_EDIT_PROBLEM,          data.strProblem);
    GetTabEdit(0, IDC_EDIT_SUCCESS_CRITERIA, data.strSuccessCriteria);
    GetTabEdit(0, IDC_EDIT_IN_SCOPE,         data.strInScope);
    GetTabEdit(0, IDC_EDIT_OUT_SCOPE,        data.strOutScope);

    // 設計タブ (index=1)
    GetTabEdit(1, IDC_EDIT_TECH_STACK,  data.strTechStackNote);
    GetTabEdit(1, IDC_EDIT_NAMING_RULES, data.strNamingRules);
    GetTabEdit(1, IDC_EDIT_CODING_RULES, data.strCodingRules);
    GetTabEdit(1, IDC_EDIT_DATA_FLOW,    data.strDataFlow);
    GetTabEdit(1, IDC_EDIT_EXTERNAL_DEPS, data.strExternalDeps);

    // タスク管理タブ (index=2)
    GetTabEdit(2, IDC_EDIT_TASKS_PHASE1, data.strTasksPhase1);
    GetTabEdit(2, IDC_EDIT_TASKS_PHASE2, data.strTasksPhase2);
    GetTabEdit(2, IDC_EDIT_TASKS_PHASE3, data.strTasksPhase3);

    // CLAUDE.mdタブ (index=3)
    GetTabEdit(3, IDC_EDIT_CLAUDE_BASIC,      data.strClaudeBasic);
    GetTabEdit(3, IDC_EDIT_CLAUDE_PROHIBITED, data.strClaudeProhibited);
}

// ============================================================
// ValidateInput — 入力バリデーション
// ============================================================
BOOL CPromptEditorDlg::ValidateInput(const ProjectData& d)
{
    if (d.strProjectName.IsEmpty())
    {
        AfxMessageBox(_T("プロジェクト名を入力してください。"), MB_OK | MB_ICONWARNING);
        m_editProjectName.SetFocus();
        return FALSE;
    }
    if (d.strOutputPath.IsEmpty())
    {
        AfxMessageBox(_T("出力先フォルダを指定してください。"), MB_OK | MB_ICONWARNING);
        m_editOutputPath.SetFocus();
        return FALSE;
    }
    return TRUE;
}

// ============================================================
// OnBnClickedGenerate — 生成ボタン処理
// ============================================================
void CPromptEditorDlg::OnBnClickedGenerate()
{
    // 入力値収集
    ProjectData data;
    CollectData(data);

    // バリデーション
    if (!ValidateInput(data)) return;

    // Markdown生成
    CMdGenerator gen;
    if (gen.GenerateAll(data))
    {
        CString msg;
        msg.Format(
            _T("MDファイルを生成しました。\n\n出力先：%s"),
            (LPCTSTR)data.strOutputPath);
        AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
    }
}

// ============================================================
// OnBnClickedClose — 閉じるボタン処理
// ============================================================
void CPromptEditorDlg::OnBnClickedClose()
{
    CDialogEx::OnCancel();
}

// ============================================================
// RefreshPreview — プレビュータブの内容を現在の入力値で更新する
// ============================================================
void CPromptEditorDlg::RefreshPreview()
{
    CDialog* pPreviewDlg = m_pTabDlg[4];
    if (!pPreviewDlg || !pPreviewDlg->GetSafeHwnd()) return;

    CComboBox* pCombo = (CComboBox*)pPreviewDlg->GetDlgItem(IDC_COMBO_PREVIEW_FILE);
    if (!pCombo) return;

    // 初回のみコンボを初期化
    if (pCombo->GetCount() == 0)
    {
        pCombo->AddString(_T("CLAUDE.md"));
        pCombo->AddString(_T(".steering/product.md"));
        pCombo->AddString(_T(".steering/structure.md"));
        pCombo->AddString(_T(".steering/decisions.md"));
        pCombo->AddString(_T("tasks/tasklist.md"));
        pCombo->SetCurSel(0);
    }

    int nSel = pCombo->GetCurSel();
    if (nSel < 0) nSel = 0;

    // 入力値収集・プレビュー生成
    ProjectData data;
    CollectData(data);

    CMdGenerator gen;
    CString strContent = gen.GeneratePreview(data, nSel);
    strContent.Replace(_T("\n"), _T("\r\n"));

    CWnd* pEdit = pPreviewDlg->GetDlgItem(IDC_EDIT_PREVIEW_CONTENT);
    if (pEdit) pEdit->SetWindowText(strContent);
}

// ============================================================
// OnBnClickedPreviewRefresh — 更新ボタン処理
// ============================================================
void CPromptEditorDlg::OnBnClickedPreviewRefresh()
{
    RefreshPreview();
}

// ============================================================
// OnSize — ダイアログサイズ変更処理
// ============================================================
void CPromptEditorDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    if (nType == SIZE_MINIMIZED || nType == SIZE_MAXHIDE)
        return;

    // 共通入力エリアの高さ
    const int HEADER_HEIGHT = 100;
    const int FOOTER_HEIGHT = 40;
    const int MARGIN = 8;

    // タブコントロールを拡張
    if (m_tabMain.GetSafeHwnd())
    {
        CRect rcTab(MARGIN, HEADER_HEIGHT, cx - MARGIN, cy - FOOTER_HEIGHT);
        m_tabMain.MoveWindow(&rcTab, TRUE);  // TRUE を指定して即座にリドロー

        // タブのクライアント領域を取得
        CRect rcTabClient = rcTab;
        m_tabMain.AdjustRect(FALSE, &rcTabClient);

        // 各タブ子ダイアログをリサイズ
        for (int i = 0; i < 5; i++)
        {
            if (m_pTabDlg[i] && m_pTabDlg[i]->GetSafeHwnd())
            {
                m_pTabDlg[i]->MoveWindow(&rcTabClient, FALSE);
                
                // 子ダイアログの OnSize を明示的にトリガー
                // (TabDialog の OnSize で EditBox がリサイズされるため)
                m_pTabDlg[i]->SendMessage(WM_SIZE, SIZE_RESTORED, 
                    MAKELPARAM(rcTabClient.Width(), rcTabClient.Height()));
            }
        }
    }

    // 出力先エリアを移動
    if (m_editOutputPath.GetSafeHwnd())
    {
        CWnd* pLabel = GetDlgItem(IDC_STATIC + 100);  // 出力先ラベル
        CRect rcLabel(MARGIN, cy - FOOTER_HEIGHT + 2, 50, cy - FOOTER_HEIGHT + 12);
        if (pLabel) pLabel->MoveWindow(&rcLabel, FALSE);

        CRect rcEdit(MARGIN + 50, cy - FOOTER_HEIGHT, cx - 110, cy - FOOTER_HEIGHT + 14);
        m_editOutputPath.MoveWindow(&rcEdit, FALSE);

        CWnd* pBrowse = GetDlgItem(IDC_BUTTON_BROWSE);
        CRect rcBrowse(cx - 100, cy - FOOTER_HEIGHT - 2, cx - 50, cy - FOOTER_HEIGHT + 14);
        if (pBrowse) pBrowse->MoveWindow(&rcBrowse, FALSE);
    }

    // ボタンを移動
    CWnd* pGenerate = GetDlgItem(IDC_BUTTON_GENERATE);
    CWnd* pClose = GetDlgItem(IDC_BUTTON_CLOSE);

    if (pGenerate)
    {
        CRect rcGen(cx - 130, cy - 20, cx - 70, cy - 4);
        pGenerate->MoveWindow(&rcGen, FALSE);
    }

    if (pClose)
    {
        CRect rcClose(cx - 60, cy - 20, cx - MARGIN, cy - 4);
        pClose->MoveWindow(&rcClose, FALSE);
    }

    InvalidateRect(NULL);
}
