#include <blGraphicsAPI.h>
#include <xutility>

namespace BoulderLeaf::Graphics
{
	API::API(std::shared_ptr<Core::blWindow> window) : 
		mWindow(window)
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

	void API::InitializeGroupInternal(const blRenderGroupId& group)
	{
		InitializeGroup(group);
	}

	void API::PreDrawRenderItemInternal(const RenderData& renderItem, const blSceneResourcePtr& scene)
	{
		RenderGroupData& groupData = renderGroupData[renderItem.group];

		if (groupData.initialized == false)
		{
			InitializeGroupInternal(renderItem.group);
			groupData.initialized = true;
		}

		PreDrawRenderItem(renderItem, scene);
	}

	void API::DrawScene(const RenderItemBuffer& renderItems, const blSceneResourcePtr& scene)
	{
		//note: dynamic memory. bad. 
		//std::vector<DynamicInstanceRenderingData> instancedItems;
		// reserve 25% of the render items for instancing. This is just a guess and can be adjusted as needed.
		//instancedItems.reserve((size_t)(renderItems.size() * 0.25f));

		for (const RenderItemVariant& item : renderItems)
		{
			std::visit([this, &scene/*,&instancedItems*/](auto&& item)
				{
					using T = std::decay_t<decltype(item)>;
					PreDrawRenderItemInternal(static_cast<const RenderData&>(item), scene);

					if constexpr (std::is_same_v<T, RenderMeshData>)
					{
						/*
						const RenderMeshData& renderData = item;

						if (renderData.material.expired() 
							|| renderData.mesh.expired())
						{
							return;
						}

						const blMaterialResourcePtr materialPtr = renderData.material.lock();
						const blMeshBaseResourcePtr meshPtr = renderData.mesh.lock();
						const blMaterial& material = materialPtr->GetData();

						if (material.IsInstanced())
						{
							auto renderMeshDataInstanced = std::find_if(instancedItems.begin(), instancedItems.end(),
								[&renderData, &materialPtr, &meshPtr](const RenderMeshDataInstanced& existingItem)
								{

									return materialPtr->GetId() == existingItem.material->GetId()
										&& meshPtr->GetId() == existingItem.mesh->GetId() && 
										existingItem.group == renderData.group;
								});

							if (renderMeshDataInstanced == instancedItems.end())
							{
								DynamicInstanceRenderingData dynamicInstantRenderingData;
								dynamicInstantRenderingData.instanceDataBuffer = new blBasicDataBuffer(
									renderData.constantBuffer->GetData().GetDataElementDescriptions(),

								);


								instancedItems->push_back(
									{
										RenderMeshDataInstanced
										{
											renderData.group,
											std::vector<Math::Matrix4x4>{ renderData.transform },
											meshPtr,
											materialPtr,
										}
									});
							}

							if (renderMeshDataInstanced != instancedItems.end())
							{
								renderMeshDataInstanced->instances.push_back({ renderData.transform });
							}
							else
							{
								instancedItems.push_back(RenderMeshDataInstanced
								{
									renderData.group,
									std::vector<Math::Matrix4x4>{ renderData.transform },
									meshPtr,
									materialPtr,
								});
							}
						}
						else
						{
							DrawMesh(item, scene);
						}
						*/

						// add support for dynaminc instancing later
						assert(false);
					}
					else if constexpr (std::is_same_v<T, RenderMeshDataInstanced>)
					{
						DrawMeshInstanced(item, scene);
					}
					else
					{
						assert(false);
					}

				}, item);
		}

		/*for (const RenderMeshDataInstanced& item : instancedItems)
		{
			DrawMeshInstanced(item, scene);
		}*/
	}
}