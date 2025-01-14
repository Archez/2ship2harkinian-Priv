#include "Path.h"

namespace LUS {
PathData* Path::GetPointer() {
    return pathData.data();
}

size_t Path::GetPointerSize() {
    return pathData.size() * sizeof(PathData);
}

PathDataMM* PathMM::GetPointer() {
    return pathData.data();
}
size_t PathMM::GetPointerSize() {
    return pathData.size() * sizeof(PathData);
}

} // namespace LUS
