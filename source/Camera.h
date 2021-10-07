#pragma once
#include "structs.h"

class Camera final
{
	public:
		Camera(const Elite::FPoint3& position, const float angleDegrees);
		~Camera();

		Camera(const Camera&) = delete;
		Camera(Camera&&) noexcept = delete;
		Camera& operator=(const Camera&) = delete;
		Camera& operator=(Camera&&) noexcept = delete;

		void Initialize(const uint32_t screenWidth, const uint32_t screenHeight);
		void Update(const float elapsedSec);

		void MoveForward(const float z, const float y);
		void MoveUp(const float x);
		void MoveRotation(const float y, const float x);

		Elite::FMatrix4 GetInverseONB() const 
		{ return Elite::Inverse(m_ONB); };
		Elite::FMatrix4 GetWVPMatrix() const 
		{ return (m_ProjectionMatrix * Elite::Inverse(m_ONB) * m_WorldMatrix); }
		Elite::FMatrix4 GetWorldMatrix() const
		{ return m_WorldMatrix; }

		Elite::FVector4 GetInverseViewDirection(const Elite::FPoint4& worldPosition) const
		{
			Elite::FVector4 viewDirection{ (Elite::FPoint4(m_Position, 1) - worldPosition) };
			return Elite::GetNormalized(viewDirection);
		}
	private:
		float m_Near;
		float m_Far;
		float m_AngleRadians;
		float m_FOV;
		float m_AspectRatio;
		float m_CurrentRotationAngle;
		Screensize m_Screensize;
		Elite::FPoint3 m_Position;
		Elite::FVector3 m_Forward;
		Elite::FVector3 m_Right;
		Elite::FVector3 m_Up;
		Elite::FVector3 m_WorldUp;
		Elite::FMatrix4 m_ONB;
		Elite::FMatrix4 m_ProjectionMatrix;
		Elite::FMatrix4 m_WorldMatrix;

		void CreateONB();
		void CreateProjectionMatrix();
		void CreateWorldMatrix();
};