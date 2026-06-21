// PromptEditorApp.cpp — MFCアプリケーションクラス実装
#include "pch.h"
#include "PromptEditorApp.h"
#include "PromptEditorDlg.h"

// アプリケーションシングルトン
CPromptEditorApp theApp;

BEGIN_MESSAGE_MAP(CPromptEditorApp, CWinApp)
END_MESSAGE_MAP()

CPromptEditorApp::CPromptEditorApp() {}

BOOL CPromptEditorApp::InitInstance()
{
    CWinApp::InitInstance();

    // COMの初期化（SHBrowseForFolderに必要）
    CoInitialize(nullptr);

    // メインダイアログを表示
    CPromptEditorDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    CoUninitialize();
    return FALSE;  // DoModal後はFALSEを返してアプリを終了
}
