#pragma once
#include "ShaderProgram.h"
#include <vector>
#include <map>
#include <type_traits>

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
	std::vector<ShaderProgram*> m_shaders;
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

	void ReloadShaders();
	void ApplyAllUniforms();

	void SetUniformInAllShaders(std::string uniformName, float value);
	void SetUniformInAllShaders(std::string uniformName, glm::vec3 value);
	void SetUniformInAllShaders(std::string uniformName, glm::mat4 value);
	void SetUniformInAllShaders(std::string uniformName, int value); // Also includes setting samplers

	void BindUniformInAllShaders(std::string uniformName, float value);
	void BindUniformInAllShaders(std::string uniformName, glm::vec3 value);
	void BindUniformInAllShaders(std::string uniformName, glm::mat4 value);
	void BindUniformInAllShaders(std::string uniformName, int value); // Also includes binding samplers

	glm::vec2 GetMousePos();
	glm::vec2 GetMouseDelta();
	bool GetKeyDown(int key);
	bool GetMouseButtonDown(int button);

	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetVPMatrix();

	void Update(float delta);
	void Draw();
};

//template<typename T, type>
//inline void Application::SetUniformInAllShaders(std::string uniformName, T const& t)
//{
//}
