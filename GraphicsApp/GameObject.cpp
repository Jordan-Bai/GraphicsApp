#include "GameObject.h"
#include "ext/matrix_transform.hpp"

GameObject::GameObject()
	: m_app()
{
}

void GameObject::Init(Application* app)
{
	m_app = app;
}

glm::mat4 GameObject::GetObjectSpace()
{
	return glm::translate(glm::mat4(1), m_pos);
}
