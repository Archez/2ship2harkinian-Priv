#pragma once

#include "Resource.h"
#include "ResourceFactory.h"

namespace LUS {
class TextureAnimationFactory : public ResourceFactory {
  public:
    std::shared_ptr<IResource> ReadResource(std::shared_ptr<ResourceInitData> initData,
                                            std::shared_ptr<BinaryReader> reader) override;

};

class TextureAnimationFactoryV0 : public ResourceVersionFactory {
  public:
    void ParseFileBinary(std::shared_ptr<BinaryReader> reader, std::shared_ptr<IResource> resource) override;
};
}; // namespace LUS