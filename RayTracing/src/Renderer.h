#pragma once

#include "Walnut/Image.h"
#include <memory>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer() {}

	void OnResize(int width, int height);
	void Render();
	
	std::shared_ptr<Walnut::Image> GetFinalImage() { return m_outputImage; }
	glm::vec4 PerPixel(glm::vec2 texcord);

	float aspectRatio = 1.0f;

private:
	
	std::shared_ptr<Walnut::Image> m_outputImage;
	uint32_t* m_imageData = nullptr;
};