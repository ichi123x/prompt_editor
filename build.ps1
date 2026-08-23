<#
.SYNOPSIS
    prompt_editor を Visual Studio 2022 (v143) 固定でビルド／テストします。

.DESCRIPTION
    このPCには VS2022 Professional と VS2026 Community が併存しています。
    素の `msbuild` を呼ぶとどちらが動くか不定になるため、vswhere で
    「17.x かつ MFC/ATL を持つ」インストールだけを特定して MSBuild を呼びます。

    Claude Code にはこのスクリプトを叩かせてください。
    ビルドログは build.log に UTF-8 で出力されるので、AI が読んで自己修正できます。

.EXAMPLE
    .\build.ps1                      # Debug ビルド
    .\build.ps1 -Configuration Release
    .\build.ps1 -Target Rebuild
    .\build.ps1 -Test                # ビルド後に単体テストを実行
    .\build.ps1 -Run                 # ビルド後に exe を起動
#>
[CmdletBinding()]
param(
    [ValidateSet('Debug', 'Release')]
    [string]$Configuration = 'Debug',

    [ValidateSet('Build', 'Rebuild', 'Clean')]
    [string]$Target = 'Build',

    # ビルド成功後に単体テスト (Microsoft Native Unit Test) を実行する
    [switch]$Test,

    # ビルド成功後に生成された exe を起動する
    [switch]$Run,

    # v143 以外のツールセットを検出しても警告を出さない
    [switch]$SkipToolsetCheck
)

$ErrorActionPreference = 'Stop'
$root = if ($PSScriptRoot) { $PSScriptRoot } else { (Get-Location).Path }

$Solution = Join-Path $root 'prompt_editor.sln'
$Platform = 'x64'   # このプロジェクトは x64 のみ（.sln に Win32 構成なし）

if (-not (Test-Path -LiteralPath $Solution)) {
    throw "ソリューションが見つかりません: $Solution`nこのスクリプトはリポジトリ直下に置いてください。"
}

# ---------------------------------------------------------------
# 1. VS2022 (17.x, MFC/ATL 入り) を特定
# ---------------------------------------------------------------
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
if (-not (Test-Path -LiteralPath $vswhere)) {
    throw "vswhere.exe が見つかりません: $vswhere"
}

$vsPath = & $vswhere -version '[17.0,18.0)' `
                     -requires Microsoft.VisualStudio.Component.VC.ATLMFC `
                     -property installationPath | Select-Object -First 1

if (-not $vsPath) {
    throw @"
MFC/ATL を含む Visual Studio 2022 (17.x) が見つかりません。
Visual Studio Installer で VS2022 を選び、個別コンポーネントから
「最新の v143 ビルド ツール用 C++ MFC (x86 & x64)」を追加してください。
"@
}

$msbuild = Join-Path $vsPath 'MSBuild\Current\Bin\MSBuild.exe'
if (-not (Test-Path -LiteralPath $msbuild)) {
    throw "MSBuild.exe が見つかりません: $msbuild"
}

# ---------------------------------------------------------------
# 2. プラットフォームツールセットの検証 (v143 固定)
# ---------------------------------------------------------------
if (-not $SkipToolsetCheck) {
    $bad = [System.Collections.Generic.List[object]]::new()
    $projects = @(Get-ChildItem -LiteralPath $root -Filter '*.vcxproj' -File -Recurse -ErrorAction SilentlyContinue)

    foreach ($proj in $projects) {
        $text = Get-Content -LiteralPath $proj.FullName -Raw -ErrorAction SilentlyContinue
        if (-not $text) { continue }

        foreach ($m in [regex]::Matches($text, '<PlatformToolset>(?<ts>[^<]+)</PlatformToolset>')) {
            $ts = $m.Groups['ts'].Value.Trim()
            if ($ts -ne 'v143') {
                $bad.Add([pscustomobject]@{ Project = $proj.Name; Toolset = $ts })
            }
        }
    }

    if ($bad.Count -gt 0) {
        Write-Warning 'v143 以外のプラットフォームツールセットを検出しました（VS2026 で作成された可能性があります）:'
        ($bad | Format-Table -AutoSize | Out-String).Trim() | Write-Warning
        Write-Warning 'プロジェクトのプロパティで v143 に戻してください。(-SkipToolsetCheck で無視可)'
    }
}

# ---------------------------------------------------------------
# 3. ビルド実行
# ---------------------------------------------------------------
$logFile = Join-Path $root 'build.log'

Write-Host ''
Write-Host "  Visual Studio : $vsPath"                              -ForegroundColor Cyan
Write-Host "  Solution      : prompt_editor.sln"                    -ForegroundColor Cyan
Write-Host "  Build         : $Configuration | $Platform | $Target" -ForegroundColor Cyan
Write-Host "  Log           : $logFile"                             -ForegroundColor Cyan
Write-Host ''

$msbuildArgs = @(
    $Solution
    "/t:$Target"
    "/p:Configuration=$Configuration"
    "/p:Platform=$Platform"
    '/m'
    '/nologo'
    '/v:minimal'
    '/clp:Summary'
    '/fl'
    "/flp:LogFile=$logFile;Verbosity=normal;Encoding=UTF-8"
)

& $msbuild @msbuildArgs
$exitCode = $LASTEXITCODE

Write-Host ''
if ($exitCode -eq 0) {
    Write-Host '  BUILD SUCCEEDED' -ForegroundColor Green
} else {
    Write-Host "  BUILD FAILED (exit code $exitCode)" -ForegroundColor Red
    Write-Host "  詳細ログ: $logFile" -ForegroundColor Red
    Write-Host ''
    exit $exitCode
}

$outDir = Join-Path $root "$Platform\$Configuration"
$exe    = Join-Path $outDir 'prompt_editor.exe'
if (Test-Path -LiteralPath $exe) {
    Write-Host "  出力: $exe" -ForegroundColor Green
}
Write-Host ''

# ---------------------------------------------------------------
# 4. テスト実行（-Test 指定時のみ）
# ---------------------------------------------------------------
if ($Test) {
    $vstest = Join-Path $vsPath 'Common7\IDE\CommonExtensions\Microsoft\TestWindow\vstest.console.exe'
    if (-not (Test-Path -LiteralPath $vstest)) {
        throw "vstest.console.exe が見つかりません: $vstest"
    }

    $testDll = Join-Path $outDir 'prompt_editor.tests.dll'
    if (-not (Test-Path -LiteralPath $testDll)) {
        throw "テストDLLが見つかりません: $testDll`nテストプロジェクトがビルドされていない可能性があります。"
    }

    Write-Host "  テスト実行: $testDll" -ForegroundColor Cyan
    Write-Host ''

    & $vstest $testDll '/Platform:x64' '/Logger:console;verbosity=normal'
    $testExit = $LASTEXITCODE

    Write-Host ''
    if ($testExit -eq 0) {
        Write-Host '  TESTS PASSED' -ForegroundColor Green
    } else {
        Write-Host "  TESTS FAILED (exit code $testExit)" -ForegroundColor Red
        Write-Host ''
        exit $testExit
    }
    Write-Host ''
}

# ---------------------------------------------------------------
# 5. 実行（-Run 指定時のみ）
# ---------------------------------------------------------------
if ($Run) {
    if (Test-Path -LiteralPath $exe) {
        Write-Host '  起動します...' -ForegroundColor Green
        Start-Process -FilePath $exe
    } else {
        Write-Warning "exe が見つかりません: $exe"
    }
}

exit 0
