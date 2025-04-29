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

	//std::vector<GameObject*> m_gameObjects;
	std::multimap<ShaderProgram*, GameObject*> m_gameObjects;
	std::vector<ShaderProgram*> m_shaders;
	Camera* m_currentCamera;

protected:
	static Application* s_instance; // Application is a singleton

	Application();
	~Application();

	Application(const Application& other) = delete;
	Application& operator=(const Application& other) = delete;

public:
	static Application* Get();

	int Initialize();

	//std::vector<GameObject*> GetObjects();
	void AddObject(GameObject* object);
	Camera* GetCurrentCamera();
	void SetCurrentCamera(Camera* camera);
	GLFWwindow* GetWindow();

	void BindUniformInAllShaders(std::string uniformName, int value); // Also includes binding samplers
	void BindUniformInAllShaders(std::string uniformName, float value);
	void BindUniformInAllShaders(std::string uniformName, glm::vec3 value);
	void BindUniformInAllShaders(std::string uniformName, glm::mat4 value);

	void SetUniformInAllShaders(std::string uniformName, int value); // Also includes setting samplers
	void SetUniformInAllShaders(std::string uniformName, float value);
	void SetUniformInAllShaders(std::string uniformName, glm::vec3 value);
	void SetUniformInAllShaders(std::string uniformName, glm::mat4 value);

	void ReloadShaders();
	void ApplyAllUniforms();

	glm::vec2 GetMousePos();
	glm::vec2 GetMouseDelta();
	bool GetKeyDown(int key);
	bool GetMouseButtonDown(int button);

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double xPos, double yPos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetVPMatrix();
	float GetAspectRatio();

	void Update(float delta);
	void Draw();
};

//template<typename T, type>
//inline void Application::SetUniformInAllShaders(std::string uniformName, T const& t)
//{
//}
