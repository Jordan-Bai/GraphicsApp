#include "ObjectType.h"
#include "HeightMapUtils.h"
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


float ObjectType::GetBaseHeight(glm::vec2 pos, TextureData& heightMap)
{
	if (rotate)
	{
		// Return the average y
		return heightMap.GetAverage(pos, rad).x - minOverlap;

		float avgY = heightMap.GetAverage(pos, rad).x;
		glm::vec3 rot = GetRotation(pos, rad, heightMap);
		glm::vec2 heightRange = HeightRangeRot(glm::vec3(pos.x, avgY, pos.y), rad, rot, heightMap);
		return avgY + heightRange.x - minOverlap;
	}

	glm::vec2 heightRange = HeightRange(glm::vec2(pos.x, pos.y), rad, heightMap);
	//std::cout << heightRange.x - minOverlap << '\n';

	//return heightRange.x - minOverlap;
	return heightRange.x;
}

glm::vec3 ObjectType::GetHeightOffset(glm::vec3 pos, TextureData& heightMap)
{
	if (rotate)
	{
		//float avgY = heightMap.GetAverage(pos, rad).x;
		glm::vec3 rot = GetRotation(glm::vec2(pos.x, pos.z), rad, heightMap);
		glm::vec3 xAxis(cos(rot.z), sin(rot.z), 0);
		glm::vec3 zAxis(0, sin(rot.x), cos(rot.x));
		glm::vec3 perp = glm::cross(zAxis, xAxis);
		//		     | C		  | Y
		//		A    |			  |      X
		//		_____|			  |_______
		//		    /			 /
		//		   / B			/ Z

		glm::vec2 heightRange = HeightRange(pos, rad, perp, heightMap);
		return perp * heightRange.x * 0.95f;
	}
	return glm::vec3(0, -minOverlap, 0);
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
		//glm::vec2 heightRange = HeightRange(glm::vec2(pos.x, pos.y), rad, heightMap);
		//float overlap = heightMap.GetAverage(pos, rad).x - heightRange.y;
		//if (overlap > maxOverlap - minOverlap)
		//{
		//	return false;
		//}
	}
	else
	{
		glm::vec2 heightRange = HeightRange(glm::vec2(pos.x, pos.y), rad, heightMap);
		float overlapHeight = heightRange.y - heightRange.x;
		if (overlapHeight > maxOverlap - minOverlap)
		{
			return false;
		}
	}

	return true;
}

GameObject* ObjectType::GenerateObject(glm::vec3 pos)
{
	Variant* variant;
	if (objectVariants.size() == 0)
	{
		std::cout << "ERROR(ObjectType::GenerateObject): Object type does not have variants";
		return nullptr;
	}
	else
	{
		int index = rand() % objectVariants.size();
		variant = objectVariants[index];
	}
	GameObject* obj = new GameObject(variant->mesh, variant->mat);
	obj->m_scale = scale;
	obj->m_pos = pos;

	return obj;
}

GameObject* ObjectType::GenerateObject(glm::vec3 pos, TextureData& heightMap)
{
	Variant* variant;
	if (objectVariants.size() == 0)
	{
		std::cout << "ERROR(ObjectType::GenerateObject): Object type does not have variants";
		return nullptr;
	}
	else
	{
		int index = rand() % objectVariants.size();
		variant = objectVariants[index];
	}
	GameObject* obj = new GameObject(variant->mesh, variant->mat);
	obj->m_scale = scale;
	obj->m_pos = pos;
	if (rotate)
	{
		obj->m_rot = GetRotation(glm::vec2(pos.x, pos.z), rad, heightMap);
	}

	return obj;
}

GameObject* ObjectType::GenerateObject(glm::vec3 pos, TextureData& heightMap, std::mt19937& twister)
{
	Variant* variant;
	if (objectVariants.size() == 0)
	{
		std::cout << "ERROR(ObjectType::GenerateObject): Object type does not have variants";
		return nullptr;
	}
	else
	{
		std::uniform_int_distribution<int> randomInt(0, objectVariants.size() - 1);
		int index = randomInt(twister);
		variant = objectVariants[index];
	}
	GameObject* obj = new GameObject(variant->mesh, variant->mat);
	obj->m_scale = scale;
	obj->m_pos = pos;
	obj->m_pos += GetHeightOffset(pos, heightMap);
	if (rotate)
	{
		obj->m_rot = GetRotation(glm::vec2(pos.x, pos.z), rad, heightMap);
	}

	return obj;
}
