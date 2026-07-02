#pragma once

#include <blBuffer.h>

#include <blMatrix4x4.inl>

namespace BoulderLeaf::Graphics
{
	struct blStandardObjectConstants
	{
		Math::Matrix4x4 WorldViewProj = Math::Matrix4x4::Identity();
	};

	struct blStandardObjectConstantsDefinition : public BufferDefinitionTemplate<blStandardObjectConstants, BufferFormat::BoulderLeaf>
	{

	};

	using blStandardObjectConstantsBuffer = blDataBuffer<blStandardObjectConstantsDefinition>;
}

BL_RESOURCE(blStandardObjectConstantsBuffer, eResourceType::DataBuffer);