#pragma once
// TabDialog.h

#include "afxdialogex.h"

class CTabDialog : public CDialog
{
public:
    CTabDialog(UINT nIDD, CWnd* pParent = NULL);
    virtual ~CTabDialog();

protected:
    virtual BOOL OnInitDialog();
    virtual BOOL OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    DECLARE_MESSAGE_MAP()

private:
    void RecalculateScrollInfo();
    void ScrollToPos(int nPos);
};
