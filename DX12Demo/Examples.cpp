#include "BoxExample.h"
#include "Examples.h"
#include "IndexBufferExample.h"
#include "VertexBufferLineStripExample.h"
#include "ImguiExample.h"
#include "ShapesExample.h"
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
		{ [](std::shared_ptr <DX12> dx12) { return new ImguiExample(dx12); } },
		{ [](std::shared_ptr <DX12> dx12) { return new ShapesExample(dx12); } }
	};
	
	constexpr size_t k_numberOfExamples = Count(k_exampleDefinitions);
	static_assert(k_numberOfExamples == static_cast<size_t>(Example::Count));
}

namespace BoulderLeaf::Graphics::DX12
{
	AbstractExample* GetNewExample(std::shared_ptr <DX12> dx12, const Example example)
	{
		const size_t index = static_cast<size_t>(example);
		assert(index < k_numberOfExamples);
		return k_exampleDefinitions[index](dx12);
	}

	Example Next(Example current)
	{
		size_t index = static_cast<size_t>(current) + 1;
		if (index >= static_cast<size_t>(Example::Count))
		{
			index = 0;
		}

		return static_cast<Example>(index);
	}

	Example Prev(Example current)
	{
		size_t index = static_cast<size_t>(current) - 1;
		if (index < 0)
		{
			index = static_cast<size_t>(Example::Count) - 1;
		}

		return static_cast<Example>(index);
	}

	void DrawImgui()
	{

	}
}