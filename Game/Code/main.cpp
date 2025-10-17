#include "Engine/Core/PongFramework.h"
#include <Engine/Core/Constants.h>
#include "Utilities/ErrorUtils.h"

#include <windows.h>
#include <string>

std::wstring getExePath() {
	wchar_t buf[MAX_PATH];
	DWORD n = GetModuleFileNameW(nullptr, buf, MAX_PATH);
	return std::wstring(buf, n);
}

#include <filesystem>
std::filesystem::path exeDir = std::filesystem::path(getExePath()).parent_path();

int main()
{
	err::set_log_file(exeDir.string());
	GameConstants::Init();
	PongFramework framework;

	return framework.Run();
}