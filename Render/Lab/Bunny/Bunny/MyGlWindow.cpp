#include "MyGlWindow.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

static float DEFAULT_VIEW_POINT[3] = { 5,5,5 };
static float DEFAULT_VIEW_CENTER[3] = { 0,0,0 };
static float DEFAULT_UP_VECTOR[3] = { 0,1,0 };

glm::mat4 lookAt(const glm::vec3& pos, const glm::vec3& look, const glm::vec3& up)
{
	glm::vec3 zaxis = glm::normalize(pos - look);
	glm::vec3 xaxis = glm::normalize(glm::cross(up, zaxis));
	glm::vec3 yaxis = glm::normalize(glm::cross(zaxis, xaxis));

	glm::mat4 R;

	R[0] = glm::vec4(xaxis.x, yaxis.x, zaxis.x, 0.0f);
	R[1] = glm::vec4(xaxis.y, yaxis.y, zaxis.y, 0.0f);
	R[2] = glm::vec4(xaxis.z, yaxis.z, zaxis.z, 0.0f);
	R[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 T;

	T[0] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	T[1] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	T[2] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	T[3] = glm::vec4(-pos.x, -pos.y, -pos.z, 1.0f);

	return R * T;
}

glm::mat4 perspective(const float& fov, const float& aspect, const float& n, const float& f)
{
	glm::mat4 P(0.0f);
	const float tanHalfFOV = tan(glm::radians(fov) / 2.0f);

	float A = (-f - n) / (f - n);
	float B = (2 * (n * f)) / (n - f);

	P[0] = glm::vec4(1.0f / (aspect * tanHalfFOV), 0, 0, 0);
	P[1] = glm::vec4(0, 1.0 / tanHalfFOV, 0, 0);
	P[2] = glm::vec4(0, 0, A, -1.0f);
	P[3] = glm::vec4(0, 0, B, 0.0f);

	return P;
}

MyGlWindow::MyGlWindow(int w, int h)
{
	m_width = w;
	m_height = h;
	m_aspect = w / static_cast<float>(h);

	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	m_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.f, m_aspect);
	Initilize();
}

void MyGlWindow::Initilize()
{
	shaderProgram = new ShaderProgram(); // 쉐이더 오브젝트 생성
	shaderProgram->initFromFiles("simple.vert", "simple.frag");

	texture = new BunnyTexture();

	shaderProgram->addUniform("Light.Position");
	shaderProgram->addUniform("Light.La");
	shaderProgram->addUniform("Light.Ld");
	shaderProgram->addUniform("Light.Ls");
	shaderProgram->addUniform("Material.Ka");
	shaderProgram->addUniform("Material.Kd");
	shaderProgram->addUniform("Material.Ks");
	shaderProgram->addUniform("Material.Shiness");

	shaderProgram->addUniform("normalMatrix");
	shaderProgram->addUniform("mvp");
	shaderProgram->addUniform("modelView");
	shaderProgram->addUniform("view");

}

void MyGlWindow::draw() 
{
	glm::mat4 model(1.0);

	glm::vec3 eye = m_viewer->getViewPoint();
	glm::vec3 look = m_viewer->getViewCenter();
	glm::vec3 up = m_viewer->getUpVector();
	glm::mat4 view = lookAt(eye, look, up);

	glm::mat4 projection = perspective(45.f, m_aspect, 0.1f, 500.f);

	glm::mat4 modelview = view * model;
	glm::mat4 mvp = projection * view * model;

	glm::vec4 lightLocation(10, 10, 10, 1);
	glm::vec3 Ka(0.0, 0.1, 0.06);
	glm::vec3 Kd(0.0, 0.50980392, 0.50980392);
	glm::vec3 Ks(0.50196078, 0.50196078, 0.50196078);
	GLfloat shiness = 1;

	glm::vec3 La(0.1, 0.1, 0.1);
	glm::vec3 Ld(0.5, 0.5, 0.5);
	glm::vec3 Ls(0.6, 0.6, 0.6);

	glm::mat4 iv = glm::inverse(modelview);
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(iv));

	//버퍼 초기화
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, m_width, m_height);
	glEnable(GL_DEPTH_TEST);

	//그려줌
	shaderProgram->use(); // vertex shader 호출

	glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(lightLocation));
	glUniform3fv(shaderProgram->uniform("Light.La"), 1, glm::value_ptr(La));
	glUniform3fv(shaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(Ld));
	glUniform3fv(shaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(Ls));
	glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(Ka));
	glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(Kd));
	glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(Ks));
	glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &shiness);

	glUniformMatrix3fv(shaderProgram->uniform("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniformMatrix4fv(shaderProgram->uniform("modelView"), 1, GL_FALSE, glm::value_ptr(modelview));
	glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(shaderProgram->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));

	if(texture) texture->draw();

	shaderProgram->disable();
}

void MyGlWindow::setSize(int w, int h)
{
	m_width = w;
	m_height = h;
}

void MyGlWindow::setAspect(float a)
{
	m_aspect = a;
}
