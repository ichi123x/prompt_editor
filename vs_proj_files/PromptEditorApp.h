#pragma once
// PromptEditorApp.h — MFCアプリケーションクラス宣言

#include <afxwin.h>
#include <afxext.h>

class CPromptEditorApp : public CWinApp
{
public:
    CPromptEditorApp();
    virtual BOOL InitInstance() override;

    DECLARE_MESSAGE_MAP()
};

extern CPromptEditorApp theApp;
