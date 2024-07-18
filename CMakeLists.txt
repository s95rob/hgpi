cmake_minimum_required(VERSION 3.16)

project(hgpi)

add_library(hgpi-base INTERFACE)
target_include_directories(hgpi-base
INTERFACE
	"src"
)
if (WIN32)
	target_compile_definitions(hgpi-base INTERFACE HGPI_PLATFORM_WINDOWS)
endif(WIN32)

file(GLOB_RECURSE HGPI_D3D11_SRCS "src/Backend/D3D11/*.cpp" "src/Backend/D3D11/*.h")
add_library(hgpi-d3d11 ${HGPI_D3D11_SRCS})
target_link_libraries(hgpi-d3d11
PUBLIC
	hgpi-base
	dxgi
	d3d11
)

file(GLOB_RECURSE HGPI_SRCS "src/HGPI/*.cpp" "src/HGPI/*.h")
add_library(hgpi ${HGPI_SRCS})
target_link_libraries(hgpi
PUBLIC
	hgpi-d3d11
)