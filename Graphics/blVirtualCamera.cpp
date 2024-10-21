#include "blVirtualCamera.h"
#include <cmath>
#include <assert.h>

namespace BoulderLeaf::Graphics
{
	VirtualCamera::VirtualCamera(float nearPlane, float farPlane, float viewAngle, float aspectRatio) :
		m_nearPlane(nearPlane),
		m_farPlane(farPlane),
		m_viewAngle(viewAngle),
		m_aspectRatio(aspectRatio)
	{
		assert(aspectRatio <= 1 && aspectRatio > 0);
		assert(nearPlane < farPlane);
		assert(nearPlane > 0);
		assert(viewAngle > 0);
	}

	VirtualCamera::VirtualCamera():VirtualCamera(1, 10, 0.5f, 0.5f)
	{

	}

	float VirtualCamera::GetHorizontalViewAngle() const
	{
		return 2.0f * atanf(m_aspectRatio * tan(m_viewAngle / 2.0f));
	}

	bool VirtualCamera::IsInsideFustrum(const Vector4 point) const
	{
		return point.x > -1 && point.x < 1
			&& point.y > -1 && point.y < 1
			&& point.z > 0 && point.z < 1;
	}

	bool VirtualCamera::IsInsideViewSpaceFustrum(const Vector4 point) const
	{
		return point.x > -m_aspectRatio && point.x < m_aspectRatio
			&& point.y > -1 && point.y < 1
			&& point.z > m_nearPlane && point.z < m_farPlane;
	}

	Vector4 VirtualCamera::ProjectVertice(const Vector4 point) const
	{
		return Vector4::Point3D(
			point.x / (m_aspectRatio * point.z * tan(m_viewAngle / 2)),
			point.y / (point.z * tan(m_viewAngle / 2)),
			1 - ((m_farPlane - point.z) / (m_farPlane - m_nearPlane))
		);
	}

	Vector4 VirtualCamera::ProjectVerticeViewSpace(const Vector4 point) const
	{
		return Vector4::Point3D(
			point.x / (point.z * tan(m_viewAngle / 2)),
			point.y / (point.z * tan(m_viewAngle / 2)),
			point.z
		);
	}

	Matrix4x4 VirtualCamera::GetProjectionMatrix() const
	{
		return Matrix4x4(
			1 / (m_aspectRatio * tan(m_viewAngle / 2)), 0, 0, 0,
			0, 1 / (tan(m_viewAngle / 2)), 0, 0, 
			0, 0, m_farPlane / (m_farPlane - m_nearPlane), 1,
			0, 0, (-1 * (m_nearPlane * m_farPlane)) / (m_farPlane - m_nearPlane), 0
		);
	}

	Vector4 VirtualCamera::ApplyProjectionMatrix(const Matrix4x4& matrix, const Vector4 point) const
	{
		return (point * matrix) / (point.z);
	}

	float VirtualCamera::GetNearPlane() const
	{
		return m_nearPlane;
	}

	float VirtualCamera::GetFarPlane() const
	{
		return m_farPlane;
	}

	float VirtualCamera::GetViewAngle() const
	{
		return m_viewAngle;
	}

	float VirtualCamera::GetAspectRatio() const
	{
		return m_aspectRatio;
	}
}