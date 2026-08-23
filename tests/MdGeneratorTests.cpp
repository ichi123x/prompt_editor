// MdGeneratorTests.cpp — CMdGenerator の単体テスト
// フレームワーク: Microsoft Native Unit Test (VS2022 同梱)

#include <afxwin.h>
#include <CppUnitTest.h>
#include "MdGenerator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// MFC を共有DLLで使うレギュラーDLLとして初期化するためのアプリオブジェクト
class CTestApp : public CWinApp {};
CTestApp theApp;

namespace PromptEditorTests
{
    TEST_CLASS(MdGeneratorTests)
    {
    private:
        // 代表的な入力値を持つ ProjectData を作る
        static ProjectData MakeSampleData()
        {
            ProjectData d;
            d.platform           = PlatformType::MFC;
            d.strProjectName     = _T("テストプロジェクト");
            d.strGoal            = _T("ゴールの内容");
            d.strTargetUser      = _T("ターゲットユーザーの内容");
            d.strProblem         = _T("課題の内容");
            d.strSuccessCriteria = _T("成功条件の内容");
            d.strInScope         = _T("対象の内容");
            d.strOutScope        = _T("対象外の内容");
            return d;
        }

        // 空の ProjectData（プラットフォームだけ設定）
        static ProjectData MakeEmptyData()
        {
            ProjectData d;
            d.platform = PlatformType::MFC;
            return d;
        }

        // s に sub が含まれることを検証する
        static void AssertContains(const CString& s, LPCTSTR sub, LPCWSTR message)
        {
            Assert::IsTrue(s.Find(sub) >= 0, message);
        }

    public:
        // product.md が生成され、入力値が反映されること
        TEST_METHOD(GeneratePreview_Product_ReflectsInput)
        {
            CMdGenerator gen;
            const CString s = gen.GeneratePreview(MakeSampleData(), 1);

            AssertContains(s, _T("# product.md"), L"product.md の見出しが無い");
            AssertContains(s, _T("ゴールの内容"), L"ゴールが反映されていない");
            AssertContains(s, _T("課題の内容"), L"課題が反映されていない");
        }

        // 未入力の項目は「（未記入）」で埋められること
        TEST_METHOD(GeneratePreview_Product_EmptyInputFallsBackToPlaceholder)
        {
            CMdGenerator gen;
            const CString s = gen.GeneratePreview(MakeEmptyData(), 1);

            AssertContains(s, _T("（未記入）"), L"未入力のプレースホルダが出ていない");
        }

        // プラットフォーム名が MFC として展開されること
        TEST_METHOD(GeneratePreview_Product_ContainsPlatformName)
        {
            CMdGenerator gen;
            const CString s = gen.GeneratePreview(MakeSampleData(), 1);

            AssertContains(s, _T("C++ / MFC"), L"プラットフォーム名が反映されていない");
        }

        // structure.md が生成されること
        TEST_METHOD(GeneratePreview_Structure_HasHeading)
        {
            CMdGenerator gen;
            const CString s = gen.GeneratePreview(MakeSampleData(), 2);

            AssertContains(s, _T("# structure.md"), L"structure.md の見出しが無い");
        }

        // tasklist.md が生成され、フェーズ見出しを含むこと
        TEST_METHOD(GeneratePreview_Tasklist_HasPhases)
        {
            CMdGenerator gen;
            const CString s = gen.GeneratePreview(MakeSampleData(), 4);

            AssertContains(s, _T("# tasklist.md"), L"tasklist.md の見出しが無い");
            AssertContains(s, _T("フェーズ1"), L"フェーズ1の見出しが無い");
        }

        // 範囲外のインデックスは空文字を返すこと
        TEST_METHOD(GeneratePreview_InvalidIndex_ReturnsEmpty)
        {
            CMdGenerator gen;
            const CString s = gen.GeneratePreview(MakeSampleData(), 99);

            Assert::IsTrue(s.IsEmpty(), L"範囲外インデックスで空文字が返っていない");
        }
    };
}
