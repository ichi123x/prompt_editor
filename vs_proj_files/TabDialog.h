#pragma once
// TabDialog.h

#include "afxdialogex.h"

class CPromptEditorDlg;

// ============================================================
// CTabDialog — タブ内共通ダイアログ
// ============================================================
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

// ============================================================
// CPreviewDialog — プレビュータブ
// ============================================================
class CPreviewDialog : public CTabDialog
{
public:
    CPreviewDialog(UINT nIDD, CPromptEditorDlg* pMainDlg);

protected:
    virtual BOOL OnInitDialog() override;
    afx_msg void OnCbnSelchangePreviewFile();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    DECLARE_MESSAGE_MAP()

private:
    CPromptEditorDlg* m_pMainDlg;

    // リソース定義時の初期座標（ピクセル）— OnSizeでのドリフトを防ぐため記憶
    CRect m_rcInitCombo;
    CRect m_rcInitEdit;
    bool  m_bInitRectsSaved;
};
