#include <common/camera.hpp>

Camera::Camera(const glm::vec3 Eye, const glm::vec3 Target) {
	eye = Eye;
	target = Target;
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f); 
	yaw = -Maths::radians(-90.0f); 
	pitch = 0.0f;
	calculateCameraVectors();
}

void Camera::calculateMatrices() {
	// Calculate the view matrix
	view = Maths::customLookAt(eye, target, worldUp);

	// Calculate the projection matrix
	projection = Maths::customPerspective(fov, aspect, near, far);
}

void Camera::calculateCameraVectors()
{
	front = glm::normalize(glm::vec3(
		cos(yaw) * cos(pitch),
		sin(pitch),
		-sin(yaw) * cos(pitch) 
	));
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}