#include "Camera.h"

// Elite includes
#include "EMath.h"

Camera::Camera(const Elite::FPoint3& position, const float angleDegrees)
	  : m_AngleRadians{ angleDegrees * (float)E_TO_RADIANS }
	  , m_FOV{}
	  , m_CurrentRotationAngle{}
	  , m_AspectRatio{}
	  , m_Screensize{}
	  , m_Position{ position }
	  , m_Forward{0, 0, 1}
	  , m_Right{1, 0, 0}
	  , m_Up{0, 1, 0}
	  , m_WorldUp{0, 1, 0}
	  , m_Near{ 0.1f }
	  , m_Far{ 100.f }
	  , m_ONB{ m_ONB.Identity() }
	  , m_ProjectionMatrix{ m_ProjectionMatrix.Identity() }
	  , m_WorldMatrix{ m_WorldMatrix.Identity() }
{}
Camera::~Camera()
{}
void Camera::Initialize(const uint32_t screenWidth, const uint32_t screenHeight)
{
	m_Screensize.width = screenWidth;
	m_Screensize.height = screenHeight;

	m_AspectRatio = (float)screenWidth / screenHeight;
	m_FOV = tan(m_AngleRadians / 2);

	CreateONB();
	CreateProjectionMatrix();
	CreateWorldMatrix();
}
void Camera::Update(const float elapsedSec)
{
	m_CurrentRotationAngle += (elapsedSec * 0.9f);
	m_WorldMatrix = Elite::MakeRotationY(m_CurrentRotationAngle);
}

void Camera::MoveForward(const float z, const float y)
{
	Elite::FMatrix3 RotationMatrixY{ Elite::MakeRotation(y / 250, m_WorldUp) };		// Move around Y-axis	
	m_Forward = Elite::GetNormalized(RotationMatrixY * m_Forward);
	
	Elite::FMatrix4 TranslationMatrix{ Elite::MakeTranslation(m_Forward * z) };
	m_Position = Elite::FPoint3{ TranslationMatrix * Elite::FPoint4{ m_Position, 1 } };
	
	CreateONB();
}
void Camera::MoveUp(const float x)
{
	Elite::FMatrix4 TranslationMatrix{ Elite::MakeTranslation((m_Up)*x) };
	m_Position = Elite::FPoint3{ TranslationMatrix * Elite::FPoint4{ m_Position, 1 } };
}
void Camera::MoveRotation(const float y, const float x)
{
	Elite::FMatrix3 RotationMatrixY{ Elite::MakeRotation(y, m_Up) };
	Elite::FMatrix3 RotationMatrixX{ Elite::MakeRotation(x, m_Right) };
	m_Forward = Elite::GetNormalized(RotationMatrixY * RotationMatrixX * m_Forward);

	CreateONB();
}

void Camera::CreateONB()
{
	m_Right = Elite::GetNormalized(Cross(m_WorldUp, m_Forward));
	m_Up = Elite::GetNormalized(Cross(m_Forward, m_Right));

	m_ONB = { m_Right, m_Up, m_Forward, Elite::FVector4(Elite::FVector3(m_Position), 1.f) };
}
void Camera::CreateProjectionMatrix()
{
	m_ProjectionMatrix[0][0] = 1 / (m_AspectRatio * m_FOV);
	m_ProjectionMatrix[1][1] = 1 / m_FOV;
	m_ProjectionMatrix[2][2] = m_Far / (m_Near - m_Far);
	m_ProjectionMatrix[3][2] = ((m_Far * m_Near) / (m_Near - m_Far));
	m_ProjectionMatrix[2][3] = -1;
	m_ProjectionMatrix[3][3] = 0;
}
void Camera::CreateWorldMatrix()
{
	m_WorldMatrix = m_WorldMatrix.Identity();
}