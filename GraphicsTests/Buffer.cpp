#include <gtest/gtest.h>
#include <blBuffer.h>
#include <Resources/blResourcesExprimental.h>	
#include <vector>
#include <string>
#include <blVector2.inl>

using namespace BoulderLeaf;

TEST(Buffer, DescriptionResources)
{
	using namespace Graphics;

	blResourceContainerPool pool;
	blResourceContainer* container = pool.GetContainer(
		1000000 * 256, // 256mb
		10000
	);

	std::vector<BufferDescription> descs =
	{
		{
			BufferFormat::DX12,
			{ {"A", BufferElementType::Float}, {"B", BufferElementType::Float2} }
		},
		{
			BufferFormat::BoulderLeaf,
			{ {"CC", BufferElementType::Matrix3x3}, {"DD", BufferElementType::Matrix3x3} }
		}
	};

	std::vector<blResourceHandleOfType<blBufferDescriptionResource>> handles;

	for (const BufferDescription& desc : descs)
	{
		handles.push_back(container->CreateResourceOfTypeWithDynamicSize<blBufferDescriptionResource>(L"BufferDescriptionResource", desc));
	}

	for (int i = 0; i < descs.size(); ++i)
	{
		const Graphics::BufferDescription& desc = descs[i];
		const blResourceHandleOfType<Graphics::blBufferDescriptionResource> handle = handles[i];

		const Graphics::blBufferDescriptionResource& resource = *handle;

		EXPECT_EQ(desc.format, resource.mFormat);

		for (int j = 0; j < desc.elementDescriptions.size(); ++j)
		{
			EXPECT_EQ(desc.elementDescriptions[j].ElementType, resource.Get(j).mElementType);
			EXPECT_EQ(desc.elementDescriptions[j].Name, resource.Get(j).GetName());

			EXPECT_EQ(desc.elementDescriptions[j].ElementType, resource[j].mElementType);
			EXPECT_EQ(desc.elementDescriptions[j].Name, resource[j].GetName());
		}

		int k = 0;
		for (const Graphics::blBufferElementDescriptionResource& elementResource : Graphics::blBufferDescriptionResource::ConstIterator(&resource))
		{
			EXPECT_EQ(desc.elementDescriptions[k].ElementType, elementResource.mElementType);
			EXPECT_EQ(desc.elementDescriptions[k].Name, elementResource.GetName());
			++k;
		}
	}
}


TEST(Buffer, DataBufferResources)
{
	using namespace Graphics;

	blResourceContainerPool pool;
	blResourceContainer* container = pool.GetContainer(
		1000000 * 256, // 256mb
		10000
	);

	Graphics::BufferDescription desc =
	{
		Graphics::BufferFormat::DX12,
		{ {"A", Graphics::BufferElementType::Float}, {"B", Graphics::BufferElementType::Float2} }
	};

	struct TestStruct
	{
		float A;
		Math::Vector2 B;
	};

	const std::vector<TestStruct> testVector =
	{
		{
			54.4f,
			Math::Vector2(1.0f, 2.0f)
		},
		{
			0.1f,
			Math::Vector2(100.0f, 23.0f)
		},
		{
			10,
			Math::Vector2(11.0f, 24.0f)
		}
	};

	blResourceHandleOfType<blBufferDescriptionResource> descResource =
		container->CreateResourceOfTypeWithDynamicSize<blBufferDescriptionResource>(L"BufferDescriptionResource", desc);

	blResourceHandleOfType<blArrayBufferResource> resource = CreateArrayBufferResource<TestStruct>(
		container,
		L"TestStruct Array Buffer",
		descResource.GetRef(),
		testVector.data(),
		(uint32_t) testVector.size()
	);

	blArrayBufferResource& resourceResult = *resource;
	blListResource& bufferResourceHandle = *container->CreateHandleFromRefOfType<blListResource>(resourceResult.mBufferResourceRef);

	for (int i = 0; i < testVector.size(); ++i)
	{
		EXPECT_EQ(testVector[i].A, bufferResourceHandle.Get<TestStruct>(i).A);
		EXPECT_EQ(testVector[i].B, bufferResourceHandle.Get<TestStruct>(i).B);
	}
}