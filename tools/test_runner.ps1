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
    # 测试用例列表
    $TestFiles = @(
        "..\work1_test\basic_test.sy",
        "..\work1_test\array_loop_test.sy",
        "..\work1_test\condition_test.sy",
        "..\work1_test\function_test.sy",
        "..\work1_test\nested_loop_test.sy",
        "..\work1_test\variable_test.sy",
        "..\work1_test\while_loop_test.sy"
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

# 检查是否存在 build 目录
if (-not (Test-Path -Path "build" -PathType Container)) {
    Write-Host "✗ 错误：build 目录不存在，请先执行构建步骤！" -ForegroundColor Red
    Write-Host "构建步骤："
    Write-Host "1. 安装 CMake"
    Write-Host "2. 创建 build 目录：mkdir build && cd build"
    Write-Host "3. 运行 CMake：cmake .."
    Write-Host "4. 编译项目：make"
    exit 1
}

# 进入 build 目录
Set-Location -Path "build"

# 检查编译器是否存在
$CompilerPath = ".\sysy_compiler.exe"
if (-not (Test-Path -Path $CompilerPath -PathType Leaf)) {
    Write-Host "✗ 错误：编译器 $CompilerPath 不存在，请先执行构建步骤！" -ForegroundColor Red
    Write-Host "构建步骤："
    Write-Host "1. 安装 CMake"
    Write-Host "2. 创建 build 目录：mkdir build && cd build"
    Write-Host "3. 运行 CMake：cmake .."
    Write-Host "4. 编译项目：make"
    exit 1
}

# 运行所有测试
$Success = Test-All
if ($Success) {
    exit 0
} else {
    exit 1
}