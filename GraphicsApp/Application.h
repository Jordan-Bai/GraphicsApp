#pragma once
#include "ShaderProgram.h"
#include <vector>
#include <map>

class GameObject;
class Camera;

class Application
{
	GLFWwindow* m_window;

	glm::vec2 m_mousePos;
	glm::vec2 m_lastMousePos;

	float m_width = 1280;
	float m_height = 720;

	//std::map<ShaderProgram*, std::vector<GameObject*>> m_gameObjects;
	std::vector<GameObject*> m_gameObjects;
	std::multimap<ShaderProgram*, GameObject*> m_renderedObjects;
	Camera* m_currentCamera;

public:
	~Application();

	int Initialize();
	std::vector<GameObject*> GetObjects();
	void AddObject(GameObject* object);
	Camera* GetCurrentCamera();
	void SetCurrentCamera(Camera* camera);
	GLFWwindow* GetWindow();

	glm::vec2 GetMousePos();
	glm::vec2 GetMouseDelta();
	bool GetKeyDown(int key);
	bool GetMouseButtonDown(int button);

	glm::mat4 GetVPMatrix();

	void Update(float delta);
	void Draw();
};