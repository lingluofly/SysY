# test_runner.ps1

# 命令行参数定义
param(
    [switch]$Help,
    [string]$Work = "all"
)

# 显示帮助信息
if ($Help) {
    Write-Host "=====================================================" -ForegroundColor Cyan
    Write-Host "SysY 编译器自动化测试程序" -ForegroundColor Cyan
    Write-Host "=====================================================" -ForegroundColor Cyan
    Write-Host "用法: test_runner.ps1 [-Help] [-Work work_number]" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "参数:"
    Write-Host "  -Help: 显示此帮助信息"
    Write-Host "  -Work: 指定要运行的work测试（1, 2, 3, 4或all）"
    Write-Host ""
    Write-Host "示例:"
    Write-Host "  .\test_runner.ps1            # 运行所有测试"
    Write-Host "  .\test_runner.ps1 -Work 3    # 只运行work3的测试"
    Write-Host "  .\test_runner.ps1 -Help      # 显示帮助信息"
    exit 0
}

function Run-Test {
    param(
        [string]$TestFile,
        [bool]$ShouldFail = $false
    )
    
    try {
        # 获取基础文件名
        $BaseName = [System.IO.Path]::GetFileNameWithoutExtension($TestFile)
        
        # 使用编译器编译并捕获输出（避免直接输出导致重复）
        Write-Host "运行 $TestFile..."
        $Output = & .\sysy_compiler.exe $TestFile 2>&1
        $ExitCode = $LASTEXITCODE
        
        # 只显示一次捕获的输出
        if ($Output) {
            $Output | ForEach-Object { Write-Host "  $_" }
        }
        Write-Host "退出码: $ExitCode"
        
        if ($ShouldFail) {
            if ($ExitCode -ne 0) {
                Write-Host " ✓ [预期失败]" -ForegroundColor Green
                return $true
            } else {
                Write-Host " ✗ [预期失败但通过]" -ForegroundColor Red
                return $false
            }
        } else {
            if ($ExitCode -eq 0) {
                Write-Host " ✓ [通过]" -ForegroundColor Green
                return $true
            } else {
                Write-Host " ✗ [失败]" -ForegroundColor Red
                return $false
            }
        }
    }
    catch {
        Write-Host " ✗ [运行错误]" -ForegroundColor Red
        Write-Host "  错误: $_" -ForegroundColor DarkGray
        return $false
    }
}

function Test-All {
    param(
        [string]$Work = "all"
    )
    
    # 测试用例配置 - 包含所有work任务的测试
    $TestConfig = @(
        # Work 1 测试用例 - 词法分析
        @{
            Work = "Work 1";
            Description = "词法分析测试";
            Tests = @(
                "..\tests\work1_test\basic_test.sy",
                "..\tests\work1_test\condition_test.sy",
                "..\tests\work1_test\function_test.sy",
                "..\tests\work1_test\variable_test.sy",
                "..\tests\work1_test\while_loop_test.sy"
            );
            ShouldFail = $false
        },
        
        # Work 1 错误测试用例
        @{
            Work = "Work 1";
            Description = "词法分析错误测试";
            Tests = @(
                "..\tests\work1_test\array_loop_test.sy",
                "..\tests\work1_test\nested_loop_test.sy"
            );
            ShouldFail = $true
        },
        
        # Work 2 测试用例 - 语法分析
        @{
            Work = "Work 2";
            Description = "语法分析测试";
            Tests = @(
                "..\tests\work2_test\example2.sy",
                "..\tests\work2_test\example3.sy"
            );
            ShouldFail = $false
        },
        
        # Work 2 错误测试用例
        @{
            Work = "Work 2";
            Description = "语法分析错误测试";
            Tests = @(
                "..\tests\work2_test\example1.sy",
                "..\tests\work2_test\example3_error1.sy",
                "..\tests\work2_test\example3_error2.sy"
            );
            ShouldFail = $true
        },
        
        # Work 3 测试用例 - 语法分析
        @{
            Work = "Work 3";
            Description = "语法分析测试";
            Tests = @(
                "..\tests\work3_test\correct_syntax.sy",
                "..\tests\work3_test\basic_variables.sy",
                "..\tests\work3_test\array_program.sy",
                "..\tests\work3_test\function_program.sy",
                "..\tests\work3_test\multiple_declarations.sy",
                "..\tests\work3_test\control_flow.sy"
            );
            ShouldFail = $false
        },
        
        # Work 3 错误测试用例
        @{
            Work = "Work 3";
            Description = "语法分析错误测试";
            Tests = @(
                "..\tests\work3_test\missing_semicolon.sy",
                "..\tests\work3_test\mismatched_brackets.sy"
            );
            ShouldFail = $true
        },
        
        # Work 4 测试用例 - 语义分析
        @{
            Work = "Work 4";
            Description = "语义分析测试";
            Tests = @(
                "..\tests\work4_test\type_mismatch_return.sy",
                "..\tests\work4_test\type_mismatch_operands.sy",
                "..\tests\work4_test\undefined_variable.sy",
                "..\tests\work4_test\undefined_function.sy",
                "..\tests\work4_test\redefined_variable.sy",
                "..\tests\work4_test\redefined_function.sy",
                "..\tests\work4_test\wrong_argument_count.sy",
                "..\tests\work4_test\non_integer_array_index.sy",
                "..\tests\work4_test\type_mismatch_assignment.sy"
            );
            ShouldFail = $false
        },
        
        # Work 4 错误测试用例
        @{
            Work = "Work 4";
            Description = "语义分析错误测试";
            Tests = @(
                "..\tests\work4_test\const_assignment_error.sy"
            );
            ShouldFail = $true
        }
    )
    
    $GlobalPassed = 0
    $GlobalTotal = 0
    $WorkResults = @()
    
    Write-Host "=====================================================" -ForegroundColor Cyan
    Write-Host "SysY 编译器自动化测试程序" -ForegroundColor Cyan
    Write-Host "=====================================================" -ForegroundColor Cyan
    Write-Host ""
    
    # 运行每个work的测试用例
    foreach ($Config in $TestConfig) {
        # 根据指定的work过滤测试
        if ($Work -ne "all" -and $Config.Work -notmatch "Work $Work") {
            continue
        }
        Write-Host "[$($Config.Work)] $($Config.Description)" -ForegroundColor Yellow
        Write-Host "-----------------------------------------------------" -ForegroundColor Yellow
        
        $WorkPassed = 0
        $WorkTotal = $Config.Tests.Count
        $GlobalTotal += $WorkTotal
        
        foreach ($TestFile in $Config.Tests) {
            if (Run-Test -TestFile $TestFile -ShouldFail $Config.ShouldFail) {
                $WorkPassed++
            }
        }
        
        # 保存work的测试结果
        $WorkResults += @{
            Work = $Config.Work;
            Description = $Config.Description;
            Passed = $WorkPassed;
            Total = $WorkTotal
        }
        
        Write-Host "-----------------------------------------------------" -ForegroundColor Yellow
        Write-Host "[$($Config.Work)] $($Config.Description): $WorkPassed/$WorkTotal 通过"
        Write-Host ""
        
        $GlobalPassed += $WorkPassed
    }
    
    # 打印汇总报告
    Write-Host "=====================================================" -ForegroundColor Cyan
    Write-Host "测试汇总报告" -ForegroundColor Cyan
    Write-Host "=====================================================" -ForegroundColor Cyan
    
    foreach ($Result in $WorkResults) {
        $Percentage = [math]::Round(($Result.Passed / $Result.Total) * 100, 2)
        Write-Host "[$($Result.Work)] $($Result.Description): $($Result.Passed)/$($Result.Total) ($Percentage%)"
    }
    
    Write-Host "-----------------------------------------------------" -ForegroundColor Cyan
    $GlobalPercentage = [math]::Round(($GlobalPassed / $GlobalTotal) * 100, 2)
    Write-Host "总测试结果: $GlobalPassed/$GlobalTotal ($GlobalPercentage%) 通过" -ForegroundColor Cyan
    Write-Host "=====================================================" -ForegroundColor Cyan
    
    if ($GlobalPassed -eq $GlobalTotal) {
        return $true
    } else {
        return $false
    }
}

# 检查当前目录是否已经是 build 目录，或者是否存在 build 目录
$CurrentDirectory = (Get-Location).Path
$IsInBuildDirectory = $CurrentDirectory -match "\\build$" -or $CurrentDirectory -eq "build"

if (-not $IsInBuildDirectory) {
    # 检查项目根目录下是否存在 build 目录
    $ProjectRoot = Join-Path $CurrentDirectory ".."
    $BuildDirectory = Join-Path $ProjectRoot "build"
    
    if (-not (Test-Path -Path $BuildDirectory -PathType Container)) {
        Write-Host "✗ 错误：build 目录不存在，请先执行构建步骤！" -ForegroundColor Red
        Write-Host "环境要求："
        Write-Host "- Windows系统"
        Write-Host "- PowerShell 5.0及以上"
        Write-Host "- CMake 4.2.0及以上"
        Write-Host "- C++编译器（推荐安装Visual Studio 2019及以上版本，或MinGW-w64）"
        Write-Host ""
        Write-Host "构建步骤："
        Write-Host "使用 Visual Studio："
        Write-Host "1. 安装 CMake"
        Write-Host "2. 创建 build 目录：mkdir build"
        Write-Host "3. 进入 build 目录：cd build"
        Write-Host "4. 运行 CMake：cmake .."
        Write-Host "5. 编译项目：cmake --build ."
        Write-Host ""
        Write-Host "使用 MinGW-w64："
        Write-Host "1. 安装 CMake 和 MinGW-w64"
        Write-Host "2. 确保 MinGW-w64 的 bin 目录已添加到系统 PATH"
        Write-Host "3. 创建 build 目录：mkdir build"
        Write-Host "4. 进入 build 目录：cd build"
        Write-Host "5. 运行 CMake：cmake .. -G \"MinGW Makefiles\""
        Write-Host "6. 编译项目：mingw32-make"
        exit 1
    }
    
    # 进入项目根目录下的 build 目录
    Set-Location -Path $BuildDirectory
}

# 检查编译器是否存在
$CompilerPath = ".\sysy_compiler.exe"
if (-not (Test-Path -Path $CompilerPath -PathType Leaf)) {
    Write-Host "✗ 错误：编译器 $CompilerPath 不存在，请先执行构建步骤！" -ForegroundColor Red
    Write-Host "环境要求："
    Write-Host "- Windows系统"
    Write-Host "- PowerShell 5.0及以上"
    Write-Host "- CMake 4.2.0及以上"
    Write-Host "- C++编译器（推荐安装Visual Studio 2019及以上版本，或MinGW-w64）"
    Write-Host ""
    Write-Host "构建步骤："
    Write-Host "使用 Visual Studio："
    Write-Host "1. 安装 CMake"
    Write-Host "2. 创建 build 目录：mkdir build"
    Write-Host "3. 进入 build 目录：cd build"
    Write-Host "4. 运行 CMake：cmake .."
    Write-Host "5. 编译项目：cmake --build ."
    Write-Host ""
    Write-Host "使用 MinGW-w64："
    Write-Host "1. 安装 CMake 和 MinGW-w64"
    Write-Host "2. 确保 MinGW-w64 的 bin 目录已添加到系统 PATH"
    Write-Host "3. 创建 build 目录：mkdir build"
    Write-Host "4. 进入 build 目录：cd build"
    Write-Host "5. 运行 CMake：cmake .. -G \"MinGW Makefiles\""
    Write-Host "6. 编译项目：mingw32-make"
    exit 1
}



# 运行测试
$Success = Test-All -Work $Work
if ($Success) {
    Write-Host ""
    Write-Host "🎉 所有测试通过！" -ForegroundColor Green
    exit 0
} else {
    Write-Host ""
    Write-Host "❌ 部分测试失败！" -ForegroundColor Red
    exit 1
}