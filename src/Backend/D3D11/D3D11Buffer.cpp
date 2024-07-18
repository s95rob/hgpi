#include "Backend/D3D11/D3D11Buffer.h"

#include "Backend/D3D11/D3D11Util.h"

namespace hgpi {

	D3D11Buffer::D3D11Buffer(D3D11Device* device, const BufferDesc& desc, D3D11_BIND_FLAG bind) 
		: DeviceChild(device) {
		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = desc.Size;
		bd.BindFlags = bind;
		bd.Usage = D3D11UsageFromBufferUsage(desc.Usage);
		bd.CPUAccessFlags = D3D11CPUAccessFromBufferUsage(desc.Usage);
		
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = desc.Data;
		sd.SysMemPitch = desc.Size;

		ID3D11Device* dev = *device;
		DX_THROW(dev->CreateBuffer(&bd, &sd, &m_Buffer));
	}

	D3D11Buffer::~D3D11Buffer() {
		m_Buffer->Release();
	}

	D3D11Texture2D::D3D11Texture2D(D3D11Device* device, const Texture2DDesc& desc, D3D11_BIND_FLAG bindFlags) 
		: D3D11Texture(device) {
		D3D11_TEXTURE2D_DESC td = {};
		td.Width = desc.Width;
		td.Height = desc.Height;
		td.MipLevels = 1;
		td.ArraySize = 1;
		if (bindFlags == D3D11_BIND_DEPTH_STENCIL)
			td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		else
			td.Format = DXGIFormatFromComponentCount(desc.Channels);
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11UsageFromBufferUsage(desc.Usage);
		td.BindFlags = bindFlags;
		td.CPUAccessFlags = D3D11CPUAccessFromBufferUsage(desc.Usage);

		D3D11_SUBRESOURCE_DATA srd = {};
		srd.pSysMem = desc.Data;
		srd.SysMemPitch = desc.Width * desc.Channels;

		ID3D11Device* dev = *device;
		DX_THROW(dev->CreateTexture2D(&td, 
			(bindFlags == D3D11_BIND_DEPTH_STENCIL ? nullptr : &srd),
			&m_Texture));

		m_SamplerDesc = {};
		m_SamplerDesc.Filter = D3D11FilterFromFilterMode(desc.Filter);
		m_SamplerDesc.AddressU = D3D11AddressModeFromWrapMode(desc.Wrap);
		m_SamplerDesc.AddressV = D3D11AddressModeFromWrapMode(desc.Wrap);
		m_SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		HRESULT result = (dev->CreateSamplerState(&m_SamplerDesc, &m_Sampler));

		if (bindFlags != D3D11_BIND_DEPTH_STENCIL) {
			D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
			srvd.Format = td.Format;
			srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvd.Texture2D.MostDetailedMip = 0;
			srvd.Texture2D.MipLevels = 1;
			result = (dev->CreateShaderResourceView(m_Texture, &srvd, &m_TextureView));
		}
	}

	D3D11Texture2D::~D3D11Texture2D() {
		m_Texture->Release();
		m_Sampler->Release();
	}

	D3D11DepthTarget::D3D11DepthTarget(D3D11Device* device, const Texture2DDesc& desc)
		: D3D11Texture2D(device, desc, D3D11_BIND_DEPTH_STENCIL) {
		ID3D11Device* dev = *device;
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
		dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		DX_THROW(dev->CreateDepthStencilView(m_Texture, &dsvd, &m_DepthView));
	}

	D3D11DepthTarget::~D3D11DepthTarget() {
		m_DepthView->Release();
	}


}