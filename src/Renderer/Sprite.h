#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <memory>

namespace Renderer {

	class Texture2D;
	class ShaderProgram;

	class Sprite {
		Sprite(const std::shared_ptr<Texture2D> pTexture,
			   const std::shared_ptr<ShaderProgram> pShaderProgram,
			   const glm::vec2& position = glm::vec2(0.f),
			   const glm::vec2& size = glm::vec2(1.f),
			   const float rotation = 0.f);

		~Sprite();

		Sprite(const Sprite&) = delete;
		Sprite& operator = (const Sprite&) = delete;

		void render() const;
		void setPosition(const glm::vec2& position);
		void setSize(const glm::vec2& size);
		void setRotation(const float rotation);

	private:
		const std::shared_ptr<Texture2D> pTexture;
		const std::shared_ptr<ShaderProgram> pShaderProgram;
	};
}