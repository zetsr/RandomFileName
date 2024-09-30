#include <windows.h>
#include <shellapi.h>
#include <string>
#include <random>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem; // 使用别名简化文件系统路径

//设置随机文件名
std::string generateRandomFileName(size_t length) {
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string result;
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<size_t> dist(0, chars.size() - 1);

    for (size_t i = 0; i < length; ++i) {
        result += chars[dist(rng)];
    }
    return result;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // 设置 DPI 感知
    SetProcessDPIAware();

    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    std::string currentPath(path);
    std::string currentDir = fs::path(currentPath).parent_path().string(); // 使用 fs::path
    std::string currentFileName = fs::path(currentPath).filename().string(); // 使用 fs::path

    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<size_t> distLength(3, 16); // 随机长度范围

    size_t newNameLength = distLength(rng); // 生成随机文件名长度
    std::string newFileName = generateRandomFileName(newNameLength) + ".exe";

    std::string newPath = currentDir + "\\" + newFileName;
    if (MoveFileA(currentPath.c_str(), newPath.c_str())) {
        // 继续执行程序
        MessageBoxA(NULL, "程序运行中……", "提示", MB_OK);
    }
    else {
        std::cerr << "移动文件失败，错误代码: " << GetLastError() << std::endl;
    }

    return 0;
}