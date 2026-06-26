//{{NO_DEPENDENCIES}}
// resource.h — リソースID定義
// prompt_editor プロジェクト

#pragma once

// ダイアログID
#define IDD_PROMPT_EDITOR_DIALOG        102

// タブ内子ダイアログID
#define IDD_TAB_REQUIREMENTS            110   // 要件定義タブ
#define IDD_TAB_DESIGN                  111   // 設計タブ
#define IDD_TAB_TASKS                   112   // タスク管理タブ
#define IDD_TAB_CLAUDE                  113   // CLAUDE.mdタブ

// メインダイアログ コントロールID
#define IDC_EDIT_PROJECT_NAME           1001  // プロジェクト名
#define IDC_EDIT_PURPOSE                1002  // 目的
#define IDC_EDIT_OWNER                  1003  // 担当者

// プラットフォーム選択ラジオボタン
#define IDC_RADIO_WINFORMS              1010
#define IDC_RADIO_WPF                   1011
#define IDC_RADIO_MFC                   1012
#define IDC_RADIO_WIN32                 1013
#define IDC_RADIO_WEB                   1014

// タブコントロール
#define IDC_TAB_MAIN                    1020

// 出力先
#define IDC_EDIT_OUTPUT_PATH            1030
#define IDC_BUTTON_BROWSE               1031

// ボタン
#define IDC_BUTTON_GENERATE             1040
#define IDC_BUTTON_CLOSE                1041

// ---- 要件定義タブ (IDD_TAB_REQUIREMENTS) ----
#define IDC_EDIT_GOAL                   2001  // ゴール
#define IDC_EDIT_TARGET_USER            2002  // ターゲットユーザー
#define IDC_EDIT_PROBLEM                2003  // 解決したい課題
#define IDC_EDIT_SUCCESS_CRITERIA       2004  // 成功の定義
#define IDC_EDIT_IN_SCOPE               2005  // In Scope
#define IDC_EDIT_OUT_SCOPE              2006  // Out of Scope

// ---- 設計タブ (IDD_TAB_DESIGN) ----
#define IDC_EDIT_TECH_STACK             3001  // 技術スタック補足
#define IDC_EDIT_NAMING_RULES           3002  // 命名規則
#define IDC_EDIT_CODING_RULES           3003  // コーディング規約
#define IDC_EDIT_DATA_FLOW              3004  // データフロー
#define IDC_EDIT_EXTERNAL_DEPS          3005  // 外部依存

// ---- タスク管理タブ (IDD_TAB_TASKS) ----
#define IDC_EDIT_TASKS_PHASE1           4001  // フェーズ1タスク
#define IDC_EDIT_TASKS_PHASE2           4002  // フェーズ2タスク
#define IDC_EDIT_TASKS_PHASE3           4003  // フェーズ3タスク

// ---- CLAUDE.mdタブ (IDD_TAB_CLAUDE) ----
#define IDC_EDIT_CLAUDE_BASIC           5001  // 基本指示
#define IDC_EDIT_CLAUDE_PROHIBITED      5002  // 禁止事項

// ---- プレビュータブ (IDD_TAB_PREVIEW) ----
#define IDD_TAB_PREVIEW                 114
#define IDC_COMBO_PREVIEW_FILE          6001  // 表示ファイル選択
#define IDC_EDIT_PREVIEW_CONTENT        6002  // プレビュー表示エリア
#define IDC_BUTTON_PREVIEW_REFRESH      6003  // 更新ボタン

// アイコン・アプリID
#define IDR_MAINFRAME                   128

// 埋め込みテンプレート（RCDATA）
#define IDR_CLAUDE_TEMPLATE             200   // 参照CLAUDE.md（生成ベース）

// Next default values for new objects
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        201
#define _APS_NEXT_COMMAND_VALUE         32771
#define _APS_NEXT_CONTROL_VALUE         6000
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
