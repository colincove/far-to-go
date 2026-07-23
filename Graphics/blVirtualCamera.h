#pragma once
#include <blTransform.h>
#include <blVector4.inl>

namespace BoulderLeaf::Graphics
{
	using namespace Math;

	class VirtualCamera
	{
	public:
		float m_nearPlane;
		float m_farPlane;
		float m_viewAngle;
		float m_aspectRatio;
		constexpr static Vector4 arbitraryUp = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

		Math::Vector4 mTranslation;
		Math::Quaternion mRotation;
	public:
		VirtualCamera();
		VirtualCamera(float nearPlane, float farPlane, float viewAngle, float aspectRatio);

		float GetHorizontalViewAngle() const;

		bool IsInsideFustrum(const Vector4 point) const; //evaluates point as a normalized device coordinate
		bool IsInsideViewSpaceFustrum(const Vector4 point) const; //evaluates point as being in view space
		Vector4 ProjectVertice(const Vector4 point) const; //should return normalized device coordinate
		Vector4 ProjectVerticeViewSpace(const Vector4 point) const; //should return view space projection

		Matrix4x4 GetTransform() const;
		Matrix4x4 GetView() const;
		Matrix4x4 GetProjectionMatrix() const;
		Vector4 ApplyProjectionMatrix(const Matrix4x4& matrix, const Vector4 point) const;
		void LookAt(Math::Vector4 point);

		float GetNearPlane() const;
		float GetFarPlane() const;
		float GetViewAngle() const;
		float GetAspectRatio() const;


	};
}