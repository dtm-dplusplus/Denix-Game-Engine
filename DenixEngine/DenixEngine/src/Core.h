#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Core/DeLog.h"
#include "Core/File.h"

template <typename T>
using Ref = std::shared_ptr<T>;


template <typename T>
using Scope = std::unique_ptr<T>;
