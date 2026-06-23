// TabDialog.cpp
#include "pch.h"
#include "TabDialog.h"
#include "PromptEditorDlg.h"

BEGIN_MESSAGE_MAP(CTabDialog, CDialog)
    ON_WM_VSCROLL()
    ON_WM_MOUSEWHEEL()
    ON_WM_SIZE()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CPreviewDialog, CTabDialog)
    // CBN_SELCHANGE はリストボックスがまだ閉じる前に発火するため不安定。
    // リストボックス破棄完了後に発火する CBN_CLOSEUP を使用する。
    ON_CBN_CLOSEUP(IDC_COMBO_PREVIEW_FILE, &CPreviewDialog::OnCbnSelchangePreviewFile)
    ON_WM_SIZE()
END_MESSAGE_MAP()

CTabDialog::CTabDialog(UINT nIDD, CWnd* pParent)
    : CDialog(nIDD, pParent)
{}

CPreviewDialog::CPreviewDialog(UINT nIDD, CPromptEditorDlg* pMainDlg)
    : CTabDialog(nIDD, nullptr)
    , m_pMainDlg(pMainDlg)
    , m_bInitRectsSaved(false)
{}

void CPreviewDialog::OnCbnSelchangePreviewFile()
{
    // CBN_CLOSEUP はコンボのドロップダウンリストボックスが破棄された直後に
    // 発火する。ただし Windows 側のフォーカス／キャプチャ履歴には、破棄済み
    // リストボックスへの参照が残ることがある。これを放置すると、後の
    // WM_ACTIVATE 処理で DefWindowProc が破棄済みハンドルに SendMessage を
    // 試みて user32 内で滞留 → メッセージポンプ停止 → ウィンドウが見えない／
    // 反応しない、という不具合を引き起こす（debugger で OnActivate→user32 の
    // ハングとして確認済み）。
    //
    // 対策：明示的にキャプチャ解放とコンボへのフォーカス再設定を行い、
    // フォーカス／キャプチャ履歴を「フレッシュ」な状態にしてから更新を
    // 遅延ポストする。
    if (::GetCapture() != NULL)
        ::ReleaseCapture();

    CWnd* pCombo = GetDlgItem(IDC_COMBO_PREVIEW_FILE);
    if (pCombo) pCombo->SetFocus();

    if (m_pMainDlg)
        m_pMainDlg->PostMessage(WM_APP_REFRESH_PREVIEW, 0, 0);
}

CTabDialog::~CTabDialog() {}

BOOL CTabDialog::OnInitDialog()
{
    CDialog::OnInitDialog();
    RecalculateScrollInfo();
    return TRUE;
}

BOOL CPreviewDialog::OnInitDialog()
{
    // CTabDialog::OnInitDialog ではなく CDialog::OnInitDialog を直接呼び、
    // スクロールバー設定（RecalculateScrollInfo）をスキップする。
    // プレビュータブはエディット自身がスクロールバーを持つため、ダイアログレベルのスクロールは不要。
    CDialog::OnInitDialog();

    CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PREVIEW_FILE);
    if (pCombo)
    {
        pCombo->AddString(_T("CLAUDE.md"));
        pCombo->AddString(_T(".steering/product.md"));
        pCombo->AddString(_T(".steering/structure.md"));
        pCombo->AddString(_T(".steering/decisions.md"));
        pCombo->AddString(_T("tasks/tasklist.md"));
        pCombo->SetCurSel(0);

        // リソーステンプレートの初期座標を記憶
        pCombo->GetWindowRect(&m_rcInitCombo);
        ScreenToClient(&m_rcInitCombo);
    }

    CWnd* pEdit = GetDlgItem(IDC_EDIT_PREVIEW_CONTENT);
    if (pEdit)
    {
        pEdit->GetWindowRect(&m_rcInitEdit);
        ScreenToClient(&m_rcInitEdit);
    }

    // ダイアログレベルのスクロールバーを無効化
    ShowScrollBar(SB_VERT, FALSE);

    m_bInitRectsSaved = true;
    return TRUE;
}

// Called explicitly by parent (ShowTab) when this tab becomes visible.
BOOL CTabDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
    if (bShow)
        RecalculateScrollInfo();
    return TRUE;
}

// Recalculate and update the vertical scrollbar range/page/position.
void CTabDialog::RecalculateScrollInfo()
{
    CRect rcClient;
    GetClientRect(&rcClient);
    if (rcClient.Height() <= 0) return;

    // Compute content height, corrected for current scroll offset.
    int nCurPos = GetScrollPos(SB_VERT);
    int nContentH = 0;
    for (CWnd* p = GetWindow(GW_CHILD); p; p = p->GetNextWindow())
    {
        CRect rc;
        p->GetWindowRect(&rc);
        ScreenToClient(&rc);
        nContentH = max(nContentH, rc.bottom + nCurPos);
    }
    nContentH += 8;

    SCROLLINFO si = {};
    si.cbSize = sizeof(si);
    si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_DISABLENOSCROLL;
    si.nMin   = 0;
    si.nMax   = nContentH;
    si.nPage  = (UINT)rcClient.Height();
    si.nPos   = nCurPos;
    SetScrollInfo(SB_VERT, &si);
}

// Scroll to the specified absolute position, moving all child controls.
void CTabDialog::ScrollToPos(int nNewPos)
{
    SCROLLINFO si = {};
    si.cbSize = sizeof(si);
    si.fMask  = SIF_ALL;
    GetScrollInfo(SB_VERT, &si);

    int nMax = max(0, si.nMax - (int)si.nPage);
    nNewPos = max(0, min(nNewPos, nMax));

    int nDelta = nNewPos - si.nPos;
    if (nDelta == 0) return;

    for (CWnd* p = GetWindow(GW_CHILD); p; p = p->GetNextWindow())
    {
        CRect rc;
        p->GetWindowRect(&rc);
        ScreenToClient(&rc);
        p->MoveWindow(rc.left, rc.top - nDelta, rc.Width(), rc.Height(), FALSE);
    }

    si.nPos  = nNewPos;
    si.fMask = SIF_POS;
    SetScrollInfo(SB_VERT, &si);
    Invalidate();
    UpdateWindow();
}

void CTabDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    SCROLLINFO si = {};
    si.cbSize = sizeof(si);
    si.fMask  = SIF_ALL;
    GetScrollInfo(SB_VERT, &si);

    int nNewPos = si.nPos;
    int nPage   = (int)si.nPage;

    switch (nSBCode)
    {
    case SB_TOP:           nNewPos = 0;                   break;
    case SB_BOTTOM:        nNewPos = si.nMax;              break;
    case SB_LINEUP:        nNewPos -= 20;                  break;
    case SB_LINEDOWN:      nNewPos += 20;                  break;
    case SB_PAGEUP:        nNewPos -= nPage;               break;
    case SB_PAGEDOWN:      nNewPos += nPage;               break;
    case SB_THUMBTRACK:
    case SB_THUMBPOSITION: nNewPos = (int)si.nTrackPos;   break;
    default:               return;
    }

    ScrollToPos(nNewPos);
}

BOOL CTabDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    SCROLLINFO si = {};
    si.cbSize = sizeof(si);
    si.fMask  = SIF_POS;
    GetScrollInfo(SB_VERT, &si);

    ScrollToPos(si.nPos - (zDelta / WHEEL_DELTA) * 40);
    return TRUE;
}

void CTabDialog::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
    if (cy <= 0) return;

    SCROLLINFO si = {};
    si.cbSize = sizeof(si);
    si.fMask  = SIF_ALL;
    GetScrollInfo(SB_VERT, &si);

    int nMax = max(0, si.nMax - (int)si.nPage);
    if (si.nPos > nMax)
        ScrollToPos(nMax);

    RecalculateScrollInfo();
}

void CPreviewDialog::OnSize(UINT nType, int cx, int cy)
{
    // CTabDialog::OnSize（スクロールロジック）をバイパスし、CDialog::OnSizeを直接呼ぶ
    CDialog::OnSize(nType, cx, cy);
    if (cx <= 0 || cy <= 0) return;
    if (!m_bInitRectsSaved) return;

    // スクロールバーが復活しないよう毎回明示的に無効化
    ShowScrollBar(SB_VERT, FALSE);

    const int MARGIN = 6;

    // コンボボックス：初期X/Yを維持、幅はクライアント幅追従、高さは初期値（ドロップダウン領域込み）固定
    CWnd* pCombo = GetDlgItem(IDC_COMBO_PREVIEW_FILE);
    if (pCombo)
    {
        int nX = m_rcInitCombo.left;
        int nY = m_rcInitCombo.top;
        int nH = m_rcInitCombo.Height();
        if (nH < 60) nH = 200;  // 念のためドロップダウン用に最低高さ確保
        int nW = max(50, cx - nX - MARGIN);
        // クライアント幅が極小でX位置がはみ出す場合のフォールバック
        if (nX + 50 > cx)
            nX = max(MARGIN, cx - 50 - MARGIN);
        pCombo->MoveWindow(nX, nY, nW, nH, TRUE);
        pCombo->ShowWindow(SW_SHOW);
    }

    // エディット：初期X/Yを維持し、幅と高さをクライアント領域に追従
    CWnd* pEdit = GetDlgItem(IDC_EDIT_PREVIEW_CONTENT);
    if (pEdit)
    {
        int nX = m_rcInitEdit.left;
        int nY = m_rcInitEdit.top;
        int nW = max(50, cx - nX - MARGIN);
        int nH = max(50, cy - nY - MARGIN);
        pEdit->MoveWindow(nX, nY, nW, nH, TRUE);
        pEdit->ShowWindow(SW_SHOW);
    }

    // 親側の描画残りを次の WM_PAINT で消化させる（UpdateWindow による同期描画は
    // メッセージポンプを長時間ブロックし、アクティベート処理と競合するため使わない）
    Invalidate();
}
