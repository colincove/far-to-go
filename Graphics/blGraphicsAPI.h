#pragma once

/*after adding the include #include <windef.h> I started to get this compile error
#maybe because this is a library.And it does not know what architecture to compile in ?

#C : \Program Files(x86)\Windows Kits\10\Include\10.0.26100.0\um\winnt.h
#define PROBE_ALIGNMENT( _s ) TYPE_ALIGNMENT( DWORD )
#elif !defined(RC_INVOKED)
#error "No Target Architecture"
#endif

#answer
#include <windows.h> */
#include <windows.h>
#include <blWindow.h>
#include <memory>
#include <blRenderScene.h>
#include <RenderItems/blRenderItemMesh.h>
#include <blShader.h>
#include <Resources/blResourceManager.h>
#include <blRenderGroup.h>

namespace BoulderLeaf::Graphics
{
	class API
	{
		struct RenderGroupData
		{
			bool initialized = false;
		};

	private:
		std::shared_ptr<Core::blWindow> mWindow;
		RenderGroupData renderGroupData[blRenderGroups::MaxValue];
		bool mInitialized;
	public:
		API() = default;
		API(std::shared_ptr<Core::blWindow> window);
	public:
		void RecieveWindowMessage(MSG msg);
		virtual LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { return true; }
		virtual void DrawMesh(const RenderMeshData& renderData, const blSceneResourcePtr scene) = 0;
		virtual void DrawMeshInstanced(const RenderMeshDataInstanced& renderData, const blSceneResourcePtr scene) = 0;
		virtual void DrawCompositeMeshInstanced(const RenderCompositeMeshDataInstanced& renderData, const blSceneResourcePtr scene) = 0;
		virtual void MarkResourceDirty(const blResourceId resourceId) = 0;
		void StartFrame();
		void EndFrame();
		void Initialize();
	protected:
		virtual void InitializeFinish() {};
		virtual void InitializeBegin() {};
		virtual void StartFrameInternal() {};
		virtual void EndFrameInternal() {};
		virtual void OnWindowMessage(MSG msg);
		virtual void OnResize();
		virtual void InitializeGroupInternal(const blRenderGroupId& group) {};
	private:
		void InitializeGroup(const blRenderGroupId& group);
	};
}