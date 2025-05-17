#include <common/maths.hpp>

glm::mat4 Maths::translate(const glm::vec3& v) //for translation
{
    glm::mat4 translate(1.0f);
    translate[3][0] = v.x, translate[3][1] = v.y, translate[3][2] = v.z;
    return translate;
}

glm::mat4 Maths::scale(const glm::vec3& v) //for scaling
{
	glm::mat4 scale(1.0f);
	scale[0][0] = v.x, scale[1][1] = v.y, scale[2][2] = v.z;
	return scale;
}

float Maths::radians(float angle)
{
    return angle * 3.1416f / 180.0f;
}

glm::mat4 Maths::rotate(const float& angle, glm::vec3 v)
{
    v = glm::normalize(v);
    float c = cos(angle);
    float s = sin(angle);
    float x2 = v.x * v.x, y2 = v.y * v.y, z2 = v.z * v.z;
    float xy = v.x * v.y, xz = v.x * v.z, yz = v.y * v.z;
    float xs = v.x * s, ys = v.y * s, zs = v.z * s;

    glm::mat4 rotate;
    rotate[0][0] = (1 - c) * x2 + c;
    rotate[0][1] = (1 - c) * xy + zs;
    rotate[0][2] = (1 - c) * xz - ys;
    rotate[1][0] = (1 - c) * xy - zs;
    rotate[1][1] = (1 - c) * y2 + c;
    rotate[1][2] = (1 - c) * yz + xs;
    rotate[2][0] = (1 - c) * xz + ys;
    rotate[2][1] = (1 - c) * yz - xs;
    rotate[2][2] = (1 - c) * z2 + c;

    return rotate;
}

//My Custom LookAt function following lecture notes
glm::mat4 Maths::customLookAt(const glm::vec3 eye, glm::vec3 target, glm::vec3 worldUp){
	glm::mat4 translation = translate(-eye); //first thing is to translate by -eye so whole worldspace moves and camera is at origin (0-0-0)
	glm:: vec3 front = ((target - eye)/(glm::length(target - eye)));
	glm::vec3 right = (glm::cross(front, worldUp) / (glm::length(glm::cross(front, worldUp))));
    glm::vec3 up = (glm::cross(right, front));
	glm::mat4 rotation(1.0f); // --> rotates the whole worldspace so eye is at origin and front is looking down -Z axis
	rotation[0][0] = right.x, rotation[0][1] = right.y, rotation[0][2] = right.z;
	rotation[1][0] = up.x, rotation[1][1] = up.y, rotation[1][2] = up.z;
	rotation[2][0] = -front.x, rotation[2][1] = -front.y, rotation[2][2] = -front.z;
    glm::mat4 view = rotation * translation;
	return view;
}

//My custom perspective projection following the lecture notes
glm::mat4 Maths::customPerspective(float fov, float aspect, float near, float far){
    glm::mat4 projection(0.0f);
    float  top = near * tan(fov / 2);
    float right = top * aspect;
    projection[0][0] = (near / right);
    projection[1][1] = (near / top);
    projection[2][2] = -((far + near) / (far - near));
    projection[2][3] = -1.0f;
    projection[3][2] = -((2 * far * near) / (far - near));
	return projection;
}
