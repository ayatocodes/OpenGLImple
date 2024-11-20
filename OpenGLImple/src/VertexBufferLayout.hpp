#pragma once
#include <vector>
#include "glew.h"

struct LayoutElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeofType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		}
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<LayoutElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout() : m_Stride(0) {}

	template <typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template <>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += LayoutElement::GetSizeofType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += LayoutElement::GetSizeofType(GL_UNSIGNED_INT) * count;
	}

	inline const std::vector<LayoutElement> GetElements() const { return m_Elements; };
	inline unsigned int GetStride() const { return m_Stride; };
};

