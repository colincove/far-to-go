#pragma once

#include <windows.h>
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

    inline UINT EnforceByteStrideLength(UINT byteSize, UINT strideLength)
    {
		// Example: Suppose byteSize = 300 that must be a multiple of 256.
		// (300 + 255) & ~255
		// 555 & ~255
		// 0x022B & ~0x00ff
		// 0x022B & 0xff00
		// 0x0200
		// 512
        return (byteSize + (strideLength -1)) & ~(strideLength - 1);
    }
}