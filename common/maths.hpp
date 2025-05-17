#pragma once

#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>

class Maths {
public:
	static glm::mat4 translate(const glm::vec3& v);
	static glm::mat4 scale(const glm::vec3& v);
	static float radians(float angle); //converts angles to radians
	static glm::mat4 rotate(const float& angle, glm::vec3 v);
	static glm::mat4 customLookAt(const glm::vec3 eye, glm::vec3 target, glm::vec3 worldUp);
	static glm::mat4 customPerspective(float fov, float aspect, float near, float far);
};
