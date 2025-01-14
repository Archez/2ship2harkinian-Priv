#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include "Resource.h"
#include "SceneCommand.h"
// #include <libultraship/libultra/types.h>
#include "2s2h/resource/type/Path.h"

namespace LUS {

class SetPathways : public SceneCommand<PathData*> {
  public:
    using SceneCommand::SceneCommand;

    PathData** GetPointer();
    size_t GetPointerSize();

    uint32_t numPaths;
    std::vector<PathData*> paths;
};

class SetPathwaysMM : public SceneCommand<PathDataMM*> {
  public:
    using SceneCommand::SceneCommand;

    PathDataMM** GetPointer();
    size_t GetPointerSize();

    uint32_t numPaths;
    std::vector<PathDataMM*> paths;
};

}; // namespace LUS
