#pragma once

#include <blMesh.h>
#include <blColour.h>
#include <array>

namespace BoulderLeaf::Graphics
{
	const StandardMesh& Cube()
	{
		using namespace Math;

		static std::array<StandardVertex, 8> vertices =
		{
			StandardVertex({ Vector3(-1.0f, -1.0f, -1.0f), Vector3(), Vector4(Colours::White), Vector2() }),
			StandardVertex({ Vector3(-1.0f, +1.0f, -1.0f), Vector3(), Vector4(Colours::Black), Vector2()}),
			StandardVertex({ Vector3(+1.0f, +1.0f, -1.0f), Vector3(), Vector4(Colours::Red), Vector2() }),
			StandardVertex({ Vector3(+1.0f, -1.0f, -1.0f), Vector3(), Vector4(Colours::Green), Vector2() }),
			StandardVertex({ Vector3(-1.0f, -1.0f, +1.0f), Vector3(), Vector4(Colours::Blue), Vector2() }),
			StandardVertex({ Vector3(-1.0f, +1.0f, +1.0f), Vector3(), Vector4(Colours::Yellow), Vector2() }),
			StandardVertex({ Vector3(+1.0f, +1.0f, +1.0f), Vector3(), Vector4(Colours::Cyan), Vector2() }),
			StandardVertex({ Vector3(+1.0f, -1.0f, +1.0f), Vector3(), Vector4(Colours::Magenta), Vector2()
		})
		};

		static std::array<blMeshStorage::index, 36> indices =
		{
			// front face
			0, 1, 2,
			0, 2, 3,
			// back face
			4, 6, 5,
			4, 7, 6,
			// left face
			4, 5, 1,
			4, 1, 0,
			// right face
			3, 2, 6,
			3, 6, 7,
			// top face
			1, 5, 6,
			1, 6, 2,
			// bottom face
			4, 0, 3,
			4, 3, 7
		};

		static StandardMesh cube(vertices.data(), vertices.size(), indices.data(), indices.size());

		return cube;
	}
}