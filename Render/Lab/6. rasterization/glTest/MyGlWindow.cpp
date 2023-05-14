#include "MyGlWindow.h"

static float DEFAULT_VIEW_POINT[3] = { 5,5,5 };
static float DEFAULT_VIEW_CENTER[3] = { 0,0,0 };
static float DEFAULT_UP_VECTOR[3] = { 0,1,0 };

glm::mat4 lookAt(const glm::vec3& camPos, const glm::vec3& look, const glm::vec3& up)
{
	glm::mat4 result = glm::mat4(1.0);
	glm::vec3 xAxis, yAxis, zAxis;

	zAxis = glm::normalize(look - camPos);
	xAxis = glm::normalize(glm::cross(up, zAxis));
	yAxis = glm::normalize(glm::cross(zAxis, xAxis));

	result[0] = glm::vec4(xAxis.x, yAxis.x, zAxis.x, -glm::dot(xAxis, look));
	result[1] = glm::vec4(xAxis.y, yAxis.y, zAxis.y, -glm::dot(yAxis, look));
	result[2] = glm::vec4(xAxis.z, yAxis.z, zAxis.z, -glm::dot(zAxis, look));

	return result;
}

glm::mat4 perspective(const float& fovY, const float& aspect, const float& n, const float& f)
{
	glm::mat4 result = glm::mat4(1.0);
	float tanFovYHalf = glm::tan(fovY / 2.f);
	result[0] = glm::vec4(1.f / (aspect * tanFovYHalf), 0, 0, 0);
	result[1] = glm::vec4(0, 1.f / tanFovYHalf, 0, 0);
	result[2] = glm::vec4(0, 0, -(f + n) / (f - n), -1.f);
	result[3] = glm::vec4(0, 0, -(2.f * f * n) / (f - n), 0);

	return result;
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

ShaderProgram* shaderProgram;

void MyGlWindow::Initilize()
{
	shaderProgram = new ShaderProgram(); // 쉐이더 오브젝트 생성
	shaderProgram->initFromFiles("simple.vert", "simple.frag");

	m_cube = new ColorCube();

	shaderProgram->addUniform("mvp");
}

void MyGlWindow::draw() 
{
	glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(0));

	glm::vec3 eye = m_viewer->getViewPoint();
	glm::vec3 look = m_viewer->getViewCenter();
	glm::vec3 up = m_viewer->getUpVector();
	glm::mat4 view = glm::lookAt(eye, look, up);

	glm::mat4 projection = glm::perspective(45.f, m_aspect, 0.1f, 500.f);

	glm::mat4 mvp = projection * view * model;

	//버퍼 초기화
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, m_width, m_height);
	glEnable(GL_DEPTH_TEST);

	//그려줌
	shaderProgram->use(); // vertex shader 호출

	glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	if(m_cube) m_cube->draw();

	shaderProgram->disable(); // vertex shader close
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
