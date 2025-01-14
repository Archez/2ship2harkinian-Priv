#include "SetPathways.h"

namespace LUS {
PathData** SetPathways::GetPointer() {
    return paths.data();
}

size_t SetPathways::GetPointerSize() {
    return paths.size() * sizeof(PathData*);
}

PathDataMM** SetPathwaysMM::GetPointer() {
    return paths.data();
}
size_t SetPathwaysMM::GetPointerSize() {
    return paths.size() * sizeof(PathData*);
}
} // namespace LUS
