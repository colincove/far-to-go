#pragma once

#include <blVector4.inl>

namespace BoulderLeaf::Graphics
{
	// Colour is a 4D vector, with the last component being the alpha channel.
	using Colour = Math::Vector4;
	// Predefined colours using DirectX::Colors
	namespace Colours
	{
		constexpr Colour White = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr Colour Black = { 0.0f, 0.0f, 0.0f, 1.0f };
		constexpr Colour Red = { 1.0f, 0.0f, 0.0f, 1.0f };
		constexpr Colour Green = { 0.0f, 1.0f, 0.0f, 1.0f };
		constexpr Colour Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
		constexpr Colour Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
		constexpr Colour Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
		constexpr Colour Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
	}
} // namespace BoulderLeaf::Graphics