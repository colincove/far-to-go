#include <filesystem>
#include <shlobj.h>
#include "blPIX.h"

namespace
{
    auto wide_to_char(const WCHAR* source) 
    {
        const auto wide_char_file_path_length = wcslen(source);
        auto destination_buffer = std::make_unique<char[]>(wide_char_file_path_length + 1);

        auto array_index = 0;
        while (source[array_index] != '\0') {
            destination_buffer.get()[array_index] = static_cast<CHAR>(source[array_index]);
            array_index++;
        }

        destination_buffer.get()[array_index] = '\0';
        return destination_buffer;
    }
}

namespace BoulderLeaf::Metrics
{
    std::wstring GetLatestWinPixGpuCapturerPath()
    {
        LPWSTR programFilesPath = nullptr;
        SHGetKnownFolderPath(FOLDERID_ProgramFiles, KF_FLAG_DEFAULT, NULL, &programFilesPath);

        std::filesystem::path pixInstallationPath = programFilesPath;
        pixInstallationPath /= "Microsoft PIX";

        std::wstring newestVersionFound;

        for (auto const& directory_entry : std::filesystem::directory_iterator(pixInstallationPath))
        {
            if (directory_entry.is_directory())
            {
                if (newestVersionFound.empty() || newestVersionFound < directory_entry.path().filename().c_str())
                {
                    newestVersionFound = directory_entry.path().filename().c_str();
                }
            }
        }

        if (newestVersionFound.empty())
        {
            // TODO: Error, no PIX installation found
        }

        return pixInstallationPath / newestVersionFound / L"WinPixGpuCapturer.dll";
    }

    bool LoadPIX()
    {
        // Check to see if a copy of WinPixGpuCapturer.dll has already been injected into the application.
        // This may happen if the application is launched through the PIX UI. 
        auto didLoaBeforedPix = GetModuleHandle("WinPixGpuCapturer.dll");
        if (GetModuleHandle("WinPixGpuCapturer.dll") == 0)
        {
            auto path = GetLatestWinPixGpuCapturerPath();
            auto charArray = wide_to_char(path.c_str());
            LoadLibrary(charArray.get());
        }

        return GetModuleHandle("WinPixGpuCapturer.dll");
    }
}