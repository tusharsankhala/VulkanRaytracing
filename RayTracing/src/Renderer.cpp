#include "Renderer.h"
#include "Walnut/Random.h"

namespace Utils
{
	uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}
}

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

	aspectRatio = (float)width /(float) height;
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
			uvCoord.x *= aspectRatio;

			glm::vec4 color = PerPixel(uvCoord);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_imageData[x + y * m_outputImage->GetWidth()] = Utils::ConvertToRGBA(color);
		}
	}
	m_outputImage->SetData(m_imageData);
}

glm::vec4 Renderer::PerPixel(glm::vec2 texcord)
{	
	glm::vec3 ray_origin{ 0.0f, 0.0f, 1.0f };
	glm::vec3 ray_direction{ texcord.x, texcord.y, -1.0f };
	glm::vec3 center { 0.0f, 0.0f, 0.0f };
	glm::vec3 light_dir = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
	glm::vec3 sphereColor{ 1.0f, 0.0f, 1.0f };

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
	float c = glm::dot(ray_origin, ray_origin) - (radius * radius);
	
	float disc = b * b - (4.0f * a * c);
	
	if (disc < 0.0f)
		return glm::vec4(0, 0, 0, 1);
	
	// Hit Points = (-b +- sqrt(discriminant))/2*a
	float t0 = (-b + glm::sqrt(disc))/(2.0f * a);
	float closestHit = (-b - glm::sqrt(disc)) / (2.0f * a);

	auto hitPosClosest = ray_origin + ray_direction * closestHit;
	auto normal = glm::normalize(hitPosClosest);

	float diffuse = glm::max(glm::dot(normal, -light_dir), 0.0f);
	sphereColor *= diffuse;
	
	return glm::vec4(sphereColor, 1.0f);
}
