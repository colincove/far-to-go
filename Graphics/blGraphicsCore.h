#pragma once

#include <blImageFormat.h>
#include <string>
#include <minwindef.h>

namespace BoulderLeaf::Graphics
{
	enum class eInputClassification
	{
		PER_VERTEX_DATA = 0,
		PER_INSTANCE_DATA = 1
	};

    struct InputElementDesc
    {
        std::string SemanticName;
        UINT SemanticIndex;
        eImageFormat Format;
        UINT InputSlot;
        UINT AlignedByteOffset;
        eInputClassification InputSlotClass;
        UINT InstanceDataStepRate;
    };
}