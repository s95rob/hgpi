#pragma once

#include <dxgi.h>
#include <d3d11.h>

#include <cassert>
#define DX_ASSERT(expression) assert(expression == S_OK)

#include <exception>
#define DX_THROW(expression) if (expression != S_OK) throw std::exception();