#pragma once
#include <vector>
#include "structs.h"

class Camera;
class TriangleMesh;
namespace Elite
{
	class Renderer;
	class Timer;
}
class Scenegraph final
{		
	public:
		Scenegraph();
		~Scenegraph();

		Scenegraph(const Scenegraph&) = delete;
		Scenegraph(Scenegraph&&) noexcept = delete;
		Scenegraph& operator=(const Scenegraph&) = delete;
		Scenegraph& operator=(Scenegraph&&) noexcept = delete;

		void Initialize(const uint32_t screenWidth, const uint32_t screenHeight, const Elite::FPoint3& camPos);
		void Update(const float elapsedSec, const bool UpdateMesh);

		float GetDepthValueAtIndex(const size_t index) const { return m_DepthBuffer[index]; }
		void SetDepthBufferAtIndex(const int index, const float depth) { m_DepthBuffer[index] = depth; }

		Camera* GetCamera() const { return m_pCamera; }

		void AddMeshToGraph(TriangleMesh* mesh) { m_Meshes.push_back(mesh); }
		const std::vector<TriangleMesh*>& GetTriangleMeshVector() const { return m_Meshes; }
		TriangleMesh* GetMeshAtIndex(const size_t index) const { return m_Meshes[index]; }

	private:
		// Variables
		Camera* m_pCamera;
		Screensize m_ScreenSize;

		std::vector<float> m_DepthBuffer;
		std::vector<TriangleMesh*> m_Meshes;
		
		void Tag_RemoveDupplicates();
		void FixIBuffer();
};