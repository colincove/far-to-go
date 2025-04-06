#include "BoxExample.h"
#include "Examples.h"
#include "IndexBufferExample.h"
#include "VertexBufferLineStripExample.h"
#include "ImguiExample.h"
#include <array>
#include <functional>

namespace
{
	#define Count(a) sizeof(a)/sizeof(*a)

	using namespace BoulderLeaf::Graphics::DX12;

	struct ExampleDef
	{
		std::function<AbstractExample*(std::shared_ptr <DX12> dx12)> Construct;
		AbstractExample* operator()(std::shared_ptr <DX12> dx12) const
		{
			return Construct(dx12);
		}
	};

	const ExampleDef k_exampleDefinitions[] = {
		{ [](std::shared_ptr <DX12> dx12) { return new BoxExample(dx12); }},
		{ [](std::shared_ptr <DX12> dx12) { return new VertexBufferLineStripExample(dx12); } },
		{ [](std::shared_ptr <DX12> dx12) { return new IndexBufferExample(dx12); } },
		{ [](std::shared_ptr <DX12> dx12) { return new ImguiExample(dx12); } }
	};

	constexpr size_t k_numberOfExamples = Count(k_exampleDefinitions);
}

namespace BoulderLeaf::Graphics::DX12
{
	AbstractExample* GetNewExample(std::shared_ptr <DX12> dx12, const int index)
	{
		assert(index < k_numberOfExamples);
		return k_exampleDefinitions[index](dx12);
	}

	void DrawImgui()
	{

	}
}