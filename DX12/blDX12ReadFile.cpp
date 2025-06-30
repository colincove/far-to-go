#include <blDX12ReadFile.h>
#include "d3dcompiler.h"
#include <fstream>
#include <ios>
#include <filesystem>

namespace BoulderLeaf::Graphics::DX12
{
	ComPtr<ID3DBlob> LoadBinary(const std::wstring filePath)
	{
		auto currDir = std::filesystem::current_path();
		std::ifstream fs(filePath, std::ios::binary);
		fs.seekg(0, std::ios_base::end);
		std::fstream::pos_type size = (int) fs.tellg();
		fs.seekg(0, std::ios_base::beg);

		ComPtr<ID3DBlob> blob;
		D3DCreateBlob(size, blob.GetAddressOf());

		fs.read((char*)blob->GetBufferPointer(), size);
		fs.close();

		return blob;
	}
}