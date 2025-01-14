#include "2s2h/resource/importer/scenecommand/SetCameraSettingsFactory.h"
#include "2s2h/resource/type/scenecommand/SetCameraSettings.h"
#include "spdlog/spdlog.h"

namespace LUS {
std::shared_ptr<IResource> SetCameraSettingsFactory::ReadResource(std::shared_ptr<ResourceInitData> initData,
                                                                 std::shared_ptr<BinaryReader> reader) {
    auto resource = std::make_shared<SetCameraSettings>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
    case 0:
	    factory = std::make_shared<SetCameraSettingsFactoryV0>();
	    break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load SetCameraSettings with version {}", resource->GetInitData()->ResourceVersion);
	return nullptr;
    }

    factory->ParseFileBinary(reader, resource);

    return resource;
}

void LUS::SetCameraSettingsFactoryV0::ParseFileBinary(std::shared_ptr<BinaryReader> reader,
                                        std::shared_ptr<IResource> resource)
{
    std::shared_ptr<SetCameraSettings> setCameraSettings = std::static_pointer_cast<SetCameraSettings>(resource);
    ResourceVersionFactory::ParseFileBinary(reader, setCameraSettings);

    ReadCommandId(setCameraSettings, reader);
	// BENTODO in MM this scene command is only used as a signal to have the scene system mark an area as visted. We should make a new command factory for this but this is fine for now.
    //setCameraSettings->settings.cameraMovement = reader->ReadInt8();
    //setCameraSettings->settings.worldMapArea = reader->ReadInt32();
}

} // namespace LUS
