#include "ObjectType.h"
#include <iostream>

Variant::Variant()
	: mesh(nullptr), mat(nullptr)//, scale(1)
{
}

Variant::Variant(Mesh* _mesh, Material* _mat)
	: mesh(_mesh), mat(_mat)//, scale(1)
{
}


ObjectType::ObjectType()
	: rad(1), exclusionRad(1), spawnAttempts(5), rotate(false), minOverlap(0), maxOverlap(1), maxRotation(1), scale(1)
{
}


float ObjectType::GetBestHeight(glm::vec2 pos, TextureData& heightMap)
{
	glm::vec2 heightRange = HeightRange(glm::vec2(pos.x, pos.y), rad, heightMap);

	return heightRange.x + minOverlap;
}

bool ObjectType::CanSpawn(glm::vec2 pos, TextureData& heightMap)
{
	if (rotate)
	{
		glm::vec3 rot = GetRotation(pos, rad, heightMap);
		if (abs(rot.x) > maxRotation || abs(rot.z) > maxRotation)
		{
			return false;
		}
	}

	glm::vec2 heightRange = HeightRange(glm::vec2(pos.x, pos.y), rad, heightMap);
	float overlapHeight = heightRange.y - heightRange.x;
	//overlapHeight = Remap(overlapHeight, 0, 1, 0, 3);
	if (overlapHeight > maxOverlap - minOverlap)
	{
		return false;
	}

	return true;
}

GameObject* ObjectType::GenerateObject(glm::vec3 pos)
{
	Variant variant;
	if (objectVariants.size() == 0)
	{
		std::cout << "ERROR(ObjectType::GenerateObject): Object type does not have variants";
	}
	else
	{
		int index = rand() % objectVariants.size();
		variant = objectVariants[index];
	}
	GameObject* obj = new GameObject(variant.mesh, variant.mat);
	obj->m_scale = scale;
	obj->m_pos = pos;

	return obj;
}

GameObject* ObjectType::GenerateObject(glm::vec3 pos, TextureData& heightMap)
{
	Variant variant;
	if (objectVariants.size() == 0)
	{
		std::cout << "ERROR(ObjectType::GenerateObject): Object type does not have variants";
	}
	else
	{
		int index = rand() % objectVariants.size();
		variant = objectVariants[index];
	}
	GameObject* obj = new GameObject(variant.mesh, variant.mat);
	obj->m_scale = scale;
	obj->m_pos = pos;
	if (rotate)
	{
		obj->m_rot = GetRotation(glm::vec2(pos.x, pos.z), rad, heightMap);
	}

	return obj;
}
