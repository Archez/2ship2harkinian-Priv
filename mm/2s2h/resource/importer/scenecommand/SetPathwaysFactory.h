#pragma once

#include "2s2h/resource/importer/scenecommand/SceneCommandFactory.h"

namespace LUS {
class SetPathwaysFactory : public SceneCommandFactory {
  public:
    std::shared_ptr<IResource>
    ReadResource(std::shared_ptr<ResourceInitData> initData, std::shared_ptr<BinaryReader> reader) override;
};

class SetPathwaysFactoryV0 : public SceneCommandVersionFactory {
  public:
    void ParseFileBinary(std::shared_ptr<BinaryReader> reader, std::shared_ptr<IResource> resource) override;
};

class SetPathwaysMMFactory : public SceneCommandFactory {
  public:
    std::shared_ptr<IResource> ReadResource(std::shared_ptr<ResourceInitData> initData,
                                            std::shared_ptr<BinaryReader> reader) override;
};

class SetPathwaysMMFactoryV0 : public SceneCommandVersionFactory {
  public:
    void ParseFileBinary(std::shared_ptr<BinaryReader> reader, std::shared_ptr<IResource> resource) override;
};

}; // namespace LUS
