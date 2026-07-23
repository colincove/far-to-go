#include <blVirtualCamera.h>
#include <cmath>
#include <assert.h>

namespace BoulderLeaf::Graphics
{
	VirtualCamera::VirtualCamera(float nearPlane, float farPlane, float viewAngle, float aspectRatio) :
		m_nearPlane(nearPlane),
		m_farPlane(farPlane),
		m_viewAngle(viewAngle),
		m_aspectRatio(aspectRatio),
		mRotation(0.0f, 1.0f, 0.0f, 0.0f)
	{
		assert(aspectRatio > 0);
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

	Matrix4x4 VirtualCamera::GetTransform() const
	{
		const Matrix4x4 translation = Matrix4x4::TranslationMatrix(mTranslation.x, mTranslation.y, mTranslation.z);
		const Matrix4x4 rotation = Quaternion::RotationMatrix4x4(mRotation);
		return rotation * translation;
	}

	Matrix4x4 VirtualCamera::GetView() const
	{
		/*const Matrix4x4 transform = GetTransform();
		const Matrix4x4 transformInverse = transform.Inverse();
		return Matrix4x4(transformInverse.r0, transformInverse.r1, transformInverse.r2, transform.r3);*/

		const Matrix4x4 rotation = Quaternion::RotationMatrix4x4(mRotation);
		const Matrix4x4 rotationInv = rotation.Inverse();
		const Matrix4x4 rotationInvView = Matrix4x4(rotationInv.r0, rotationInv.r1, rotationInv.r2, rotation.r3);

		//const Matrix4x4 transform = GetTransform();
		//const Matrix4x4 transformInv = transform.Inverse();
		const Matrix4x4 translation = Matrix4x4::TranslationMatrix(mTranslation.x, mTranslation.y, mTranslation.z);
		const Matrix4x4 translationInv = translation.Inverse();

		return (rotation).Inverse();
		//return rotationInv * translationInv;

		//const Matrix4x4 transform = GetTransform();
		//const Matrix4x4 transformInverse = transform.Inverse();
		//return Matrix4x4(transformInverse.r0, transformInverse.r1, transformInverse.r2, transform.r3);
	}

	Vector4 VirtualCamera::ApplyProjectionMatrix(const Matrix4x4& matrix, const Vector4 point) const
	{
		return (point * matrix) / (point.z);
	}

	void VirtualCamera::LookAt(Math::Vector4 point)
	{
		// I am worried about the handedness here. Need some testing. 
		// I am using this in place of 		
		// const Matrix4x4 lookAtMatrix = Matrix4x4::LookAtLH(mTranslation, point, arbitraryUp);	
		
		mRotation = Quaternion::LookAt(
			Vector3(mTranslation.x, mTranslation.y, mTranslation.z),
			Vector3(point.x, point.y, point.z));

		const Matrix4x4 lookAtMatrix = Matrix4x4::LookAtLH(mTranslation, point, arbitraryUp);	
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