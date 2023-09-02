#pragma once
#define NOMINMAX

// stl stuff
#include <memory>
#include <cassert>
#include <chrono>
#include <limits>
#include <fstream>

#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <set>
#include <unordered_set>

// other lib stuff

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>


// engine stuff (should only be mostly done files that won't change much)
#include "Managers/Debug/DebugManager.h"

#include "Managers/Graphics/VulkanInterface/VulkanInterface.h"

#include "../Utilities/Hash.h"