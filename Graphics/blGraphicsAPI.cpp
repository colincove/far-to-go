#include <blGraphicsAPI.h>
#include <xutility>

namespace BoulderLeaf::Graphics
{
	API::API(std::shared_ptr<Core::blWindow> window) : 
		mWindow(window), mInitialized(false)
	{

	}

	void API::RecieveWindowMessage(MSG msg)
	{
		switch (msg.message)
		{
		case WM_EXITSIZEMOVE:
		case CB_SETDROPPEDWIDTH:
			OnResize();
			break;
		}

		OnWindowMessage(msg);
	}

	void API::OnWindowMessage(MSG msg)
	{

	}

	void API::OnResize()
	{
	}

	void API::InitializeGroup(const blRenderGroupId& group)
	{
		InitializeGroupInternal(group);
		RenderGroupData& groupData = renderGroupData[group];
		groupData.initialized = true;
	}

	void API::Update(const Metrics::blTime& time)
	{
		UpdateInternal(time);
	}

	void API::StartFrame() 
	{
		StartFrameInternal();
	}

	void API::EndFrame()
	{
		EndFrameInternal();
	}

	void API::Initialize()
	{
		assert(!mInitialized);
		InitializeBegin();

		for (blRenderGroupId group : blRenderGroups::Iterator())
		{
			RenderGroupData& groupData = renderGroupData[group];
			assert(!groupData.initialized);
			InitializeGroup(group);
		}

		InitializeFinish();
		mInitialized = true;
	}
}