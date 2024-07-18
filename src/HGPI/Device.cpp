#include "Backend/D3D11/D3D11Device.h"

namespace hgpi {

	DeviceHandle CreateDevice(const DeviceDesc& desc) {
		return MakeRef<D3D11Device>(desc);
	}

}