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

    // Windowsのゴーストウィンドウ機能を無効化。
    // メッセージ処理が一時的に滞った際に、システムが本物に見える応答しないダミー
    // ウィンドウ（ゴースト）を作る挙動を抑止する。
    // 長時間運用後にクリックしても前面に来ない／操作不能になる不具合の根本対策。
    ::DisableProcessWindowsGhosting();

    // COMの初期化（SHBrowseForFolderに必要）
    CoInitialize(nullptr);

    // メインダイアログを表示
    CPromptEditorDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    CoUninitialize();
    return FALSE;  // DoModal後はFALSEを返してアプリを終了
}
