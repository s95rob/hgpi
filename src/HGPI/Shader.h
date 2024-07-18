#pragma once

#include "HGPI/Ref.h"

#include <vector>

namespace hgpi {

	enum class DataType {
		Float, Int, Uint
	};

	struct VertexAttrib {
		DataType Type;
		uint32_t ComponentCount;
		bool Normalized;

		VertexAttrib() = default;

		size_t Size() const {
			switch (Type) {
			case DataType::Float: return sizeof(float) * ComponentCount;
			case DataType::Int:
			case DataType::Uint: return sizeof(uint32_t) * ComponentCount;
			}

			return size_t();
		}
	};

	struct VertexLayout {
		VertexLayout() = default;

		VertexLayout& AddAttrib(VertexAttrib& attrib) {
			m_Attribs.push_back(attrib);
			return *this;
		}

		VertexLayout& AddAttrib(DataType type, uint32_t componentCount, bool normalized = false) {
			m_Attribs.push_back({ type, componentCount, normalized });
			return *this;
		}

		size_t Size() const { return m_Attribs.size(); }
		const VertexAttrib* Data() const { return m_Attribs.data(); }

		auto& begin() { return m_Attribs.begin(); }
		auto& end() { return m_Attribs.end(); }
		const auto& cbegin() { return m_Attribs.cbegin(); }
		const auto& cend() { return m_Attribs.cend(); }

		const VertexAttrib& operator[](size_t index) const { return m_Attribs[index]; }

	private:
		std::vector<VertexAttrib> m_Attribs;
	};

	struct ShaderModuleDesc {
		uint32_t Size;
		void* Data;
		const char* EntryName;
	};

	struct ShaderProgramDesc {
		ShaderModuleDesc VertexShader;
		union {
			ShaderModuleDesc FragmentShader, PixelShader;
		};
		VertexLayout InputLayout;
	};

	class IShaderProgram {
	public:
		virtual ~IShaderProgram() = default;
	};

	typedef Ref<IShaderProgram> ShaderProgramHandle;

}