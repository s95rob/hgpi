#include "Backend/D3D11/D3D11Shader.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include "D3D11Util.h"

#include <iostream>

namespace hgpi {

	D3D11ShaderProgram::D3D11ShaderProgram(D3D11Device* device, const ShaderProgramDesc& desc) 
		: DeviceChild(device) {
		ID3DBlob* errorBlob = nullptr;

		if (D3DCompile(desc.VertexShader.Data, desc.VertexShader.Size, "VERTEX",
			nullptr, nullptr, desc.VertexShader.EntryName, "vs_4_0", 0, 0, &m_VSBlob, &errorBlob) != S_OK) {
			std::cout << (char*)errorBlob->GetBufferPointer() << '\n';
			errorBlob->Release();
		}
			
		if (D3DCompile(desc.PixelShader.Data, desc.PixelShader.Size, "PIXEL",
			nullptr, nullptr, desc.PixelShader.EntryName, "ps_4_0", 0, 0, &m_PSBlob, &errorBlob) != S_OK) {	
			std::cout << (char*)errorBlob->GetBufferPointer() << '\n';
			errorBlob->Release();
		}

		ID3D11Device* dev = *device;
		DX_THROW(dev->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(),
			nullptr, &m_VertexShader));
		DX_THROW(dev->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize(),
			nullptr, &m_PixelShader));

		std::vector<D3D11_INPUT_ELEMENT_DESC> ieds(desc.InputLayout.Size());
		for (int i = 0; i < desc.InputLayout.Size(); i++) {
			ieds[i] = { "ATTRIB", (UINT)i, DXGIFormatFromAttrib(desc.InputLayout[i]),
				0, (UINT)m_InputLayoutStride, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			m_InputLayoutStride += desc.InputLayout[i].Size();
		}

		DX_THROW(dev->CreateInputLayout(ieds.data(), ieds.size(),
			m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), &m_Layout));
	}

	D3D11ShaderProgram::~D3D11ShaderProgram() {
		m_VertexShader->Release();
		m_PixelShader->Release();
		m_VSBlob->Release();
		m_PSBlob->Release();
	}

}