#pragma once

#include "../../EngineIncludes.h"

namespace gde::component {
    struct Render {
        std::shared_ptr<Model> model; // TODO: swap to shared component architecture
    };
}