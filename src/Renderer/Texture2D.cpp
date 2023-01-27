#include "Texture2D.h"

namespace Renderer {

	Texture2D::Texture2D(GLuint widht, GLuint height,
						const unsigned char* data,
						const unsigned int channels,
						const GLenum filter,
						const GLenum wrapmode)
						:m_width(widht), m_height(height) {
		
		switch (channels){
			case 4: 
				m_mode = GL_RGBA;
			case 3:
				m_mode = GL_RGB;
			default:
				m_mode = GL_RGBA;
				break;
		}

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapmode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapmode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture2D& Texture2D::operator = (Texture2D&& texture2d) {
		glDeleteTextures(1, &m_ID);
		m_ID = texture2d.m_ID;
		texture2d.m_ID = 0;
		m_mode = texture2d.m_mode;
		m_width = texture2d.m_width;
		m_height = texture2d.m_height;
		return *this;
	}

	Texture2D::Texture2D(Texture2D&& texture2d) {
		m_ID = texture2d.m_ID;
		texture2d.m_ID = 0;
		m_mode = texture2d.m_mode;
		m_width = texture2d.m_width;
		m_height = texture2d.m_height;
	}

	Texture2D::~Texture2D() {
		glDeleteTextures(1, &m_ID);
	}

	void Texture2D::bind() const {
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
}