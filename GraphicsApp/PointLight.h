#pragma once

#include "GameObject.h"

class PointLight : public GameObject
{
public:
	glm::vec3 m_col;
	float m_bright;

	PointLight();
	PointLight(Mesh* mesh, Material* material);
	PointLight(glm::vec3 position, glm::vec3 colour, float brightness);

	void SetColour(glm::vec3 colour, float brightness);
	glm::vec3 GetColour();

	virtual void Draw() override;
};