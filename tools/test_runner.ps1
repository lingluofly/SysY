# test_runner.ps1
function Run-Test {
    param(
        [string]$TestFile
    )
    
    try {
        # 获取基础文件名
        $BaseName = [System.IO.Path]::GetFileNameWithoutExtension($TestFile)
        
        # 使用编译器编译
        Write-Host "编译 $TestFile..."
        $CompileResult = Start-Process -FilePath ".\sysy_compiler.exe" -ArgumentList @($TestFile) -Wait -NoNewWindow -PassThru
        
        if ($CompileResult.ExitCode -ne 0) {
            Write-Host "✗ 编译失败 $TestFile" -ForegroundColor Red
            return $false
        } else {
            Write-Host "✓ $TestFile 编译成功" -ForegroundColor Green
            return $true
        }
    }
    catch {
        Write-Host "✗ 运行 $TestFile 时出错: $_" -ForegroundColor Red
        return $false
    }
}

function Test-All {
    # 测试用例列表 - 相对于项目根目录
    $TestFiles = @(
        "..\tests\work1_test\basic_test.sy",
        "..\tests\work1_test\array_loop_test.sy",
        "..\tests\work1_test\condition_test.sy",
        "..\tests\work1_test\function_test.sy",
        "..\tests\work1_test\nested_loop_test.sy",
        "..\tests\work1_test\variable_test.sy",
        "..\tests\work1_test\while_loop_test.sy"
    )
    
    $Passed = 0
    $Total = $TestFiles.Count
    
    Write-Host "开始运行测试..." -ForegroundColor Yellow
    Write-Host ""
    
    foreach ($TestFile in $TestFiles) {
        if (Run-Test -TestFile $TestFile) {
            $Passed++
        }
    }
    
    Write-Host ""
    Write-Host "测试完成: $Passed/$Total 通过" -ForegroundColor Yellow
    
    if ($Passed -eq $Total) {
        return $true
    } else {
        return $false
    }
}

# 检查当前目录是否已经是 build 目录，或者是否存在 build 目录
$CurrentDirectory = (Get-Location).Path
$IsInBuildDirectory = $CurrentDirectory -match "\\build$" -or $CurrentDirectory -eq "build"

if (-not $IsInBuildDirectory) {
    # 检查是否存在 build 目录
    if (-not (Test-Path -Path "build" -PathType Container)) {
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
        Write-Host "5. 运行 CMake：cmake .. -G \"MinGW Makefiles\""`n        Write-Host "6. 编译项目：mingw32-make"
        exit 1
    }
    
    # 进入 build 目录
    Set-Location -Path "build"
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
    Write-Host "5. 运行 CMake：cmake .. -G \"MinGW Makefiles\""`n    Write-Host "6. 编译项目：mingw32-make"
    exit 1
}

# 运行所有测试
$Success = Test-All
if ($Success) {
    exit 0
} else {
    exit 1
}