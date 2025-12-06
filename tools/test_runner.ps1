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

# 进入 build 目录
Set-Location -Path "build"

# 运行所有测试
$Success = Test-All
if ($Success) {
    exit 0
} else {
    exit 1
}