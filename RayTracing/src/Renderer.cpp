#include "Renderer.h"
#include "Walnut/Random.h"

void Renderer::OnResize(int width, int height)
{
	if (m_outputImage)
	{
		// No resizing necessary.
		if (m_outputImage->GetWidth() == width && m_outputImage->GetHeight() == height)
			return;

		m_outputImage->Resize(width, height);
	}
	else
	{
		m_outputImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_imageData;
	m_imageData = new uint32_t[width * height];
}

void Renderer::Render()
{
	// Render every Pixel.
	for (uint32_t y = 0; y < m_outputImage->GetHeight(); ++y)
	{
		for (uint32_t x = 0; x < m_outputImage->GetWidth(); ++x)
		{
			glm::vec2 uvCoord = {(float)x/(float)m_outputImage->GetWidth(), (float)y /(float)m_outputImage->GetHeight() };

			// Remapping the coord to (-1, 1) range.
			uvCoord = uvCoord * 2.0f - 1.0f;

			m_imageData[x + y * m_outputImage->GetWidth()] = PerPixel(uvCoord);	
		}
	}
	m_outputImage->SetData(m_imageData);
}

uint32_t Renderer::PerPixel(glm::vec2 texcord)
{
	glm::vec3 ray_origin{ 0.0f, 0.0f, 2.0f };
	glm::vec3 ray_direction{ texcord.x, texcord.y, -1.0f };
	float radius = 0.5f;

	// Solving Quadratic equation.
	// (bx^2 + by^2)t^2 + (axbx + ayby)2t + (ax^2 + ay^2 + r^2)
	//  a = Ray origin.
	//  b = Ray direction.
	//  r = Radius of sphere.
	//  t = hit distance.

	// discrminant.
	// d = (b^2 - 4 * a * c)
	float a = glm::dot(ray_direction , ray_direction);
	float b = 2.0f * glm::dot(ray_origin, ray_direction);
	float c = glm::dot(ray_origin, ray_origin) - radius * radius;

	float disc = b * b - (4.0f * a * c);

	if(disc >= 0)
		return 0xffff00ff;

	
	return 0xff000000;
	
	//uint8_t r = (uint8_t)(abs(texcord.x) * 255.0f);
	//uint8_t g = (uint8_t)(abs(texcord.y) * 255.0f);

	//return 0xff000000 | (g << 8) | r;
}
