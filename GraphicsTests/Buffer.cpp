#include <gtest/gtest.h>
#include <blBuffer.h>
#include <vector>
#include <string>

using namespace BoulderLeaf::Graphics;

TEST(InlineBufferDescription, IteratorWorks)
{
	BufferDescription desc;
	desc.format = BufferFormat::BoulderLeaf;
	desc.elementDescriptions = { {"A", BufferElementType::Float}, {"B", BufferElementType::Float2} };

	uint64_t totalSize = InlineBufferDescription::CalculateTotalSize(desc.elementDescriptions);
	std::vector<byte> buffer(totalSize);
	InlineBufferDescription* inlineDesc = reinterpret_cast<InlineBufferDescription*>(buffer.data());
	inlineDesc->WriteDescription(desc);

	EXPECT_EQ(inlineDesc->mHeader.format, desc.format);
	EXPECT_EQ(inlineDesc->mHeader.numElementDescriptions, desc.elementDescriptions.size());

	InlineBufferDescription::Iterator iterator(*inlineDesc);

	size_t idx = 0;
	for (auto it = iterator.begin(); it != iterator.end(); ++it) {
		InlineBufferElementDescription& e = *it;
		EXPECT_EQ(e.mHeader.ElementType, desc.elementDescriptions[idx].ElementType);
		EXPECT_EQ(std::string(e.GetName()), desc.elementDescriptions[idx].Name);
		++idx;
	}
	EXPECT_EQ(idx, desc.elementDescriptions.size());
}
