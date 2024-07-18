#pragma once

#include "HGPI/Shader.h"
#include "Backend/D3D11/D3D11Device.h"

namespace hgpi {

	class D3D11ShaderProgram : public IShaderProgram, public DeviceChild<D3D11Device> {
	public:
		D3D11ShaderProgram(D3D11Device* device, const ShaderProgramDesc& desc);
		virtual ~D3D11ShaderProgram();

		ID3D11VertexShader* GetVertexShader() { return m_VertexShader; }
		ID3D11PixelShader* GetPixelShader() { return m_PixelShader; }

		ID3D11InputLayout* GetInputLayout() { return m_Layout; }

		size_t GetInputLayoutStride() const { return m_InputLayoutStride; }
		
	private:
		ID3D11VertexShader* m_VertexShader = nullptr;
		ID3D11PixelShader* m_PixelShader = nullptr;
		ID3D10Blob* m_VSBlob = nullptr, 
			*m_PSBlob = nullptr;
		ID3D11InputLayout* m_Layout = nullptr;
		size_t m_InputLayoutStride = 0;
	};

}