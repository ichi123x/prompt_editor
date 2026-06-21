// TabDialog.cpp
#include "pch.h"
#include "TabDialog.h"

BEGIN_MESSAGE_MAP(CTabDialog, CDialog)
    ON_WM_VSCROLL()
    ON_WM_MOUSEWHEEL()
    ON_WM_SIZE()
END_MESSAGE_MAP()

CTabDialog::CTabDialog(UINT nIDD, CWnd* pParent)
    : CDialog(nIDD, pParent)
{}

CTabDialog::~CTabDialog() {}

BOOL CTabDialog::OnInitDialog()
{
    CDialog::OnInitDialog();
    RecalculateScrollInfo();
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
