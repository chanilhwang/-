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

void MyGlWindow::Initilize()
{
	shaderProgram = new ShaderProgram(); // 쉐이더 오브젝트 생성
	shaderProgram->initFromFiles("simple.vert", "simple.frag");

	m_cow = new ColorCowTexture();

	shaderProgram->addUniform("Light.Position");
	shaderProgram->addUniform("Light.La");
	shaderProgram->addUniform("Light.Ld");
	shaderProgram->addUniform("Light.Ls");
	shaderProgram->addUniform("Material.Ka");
	shaderProgram->addUniform("Material.Kd");
	shaderProgram->addUniform("Material.Ks");
	shaderProgram->addUniform("modelViewMatrix");
	shaderProgram->addUniform("normalMatrix");
	shaderProgram->addUniform("mvp");
	shaderProgram->addUniform("model");
}

void MyGlWindow::draw() 
{
	glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(0));

	glm::vec3 eye = m_viewer->getViewPoint();
	glm::vec3 look = m_viewer->getViewCenter();
	glm::vec3 up = m_viewer->getUpVector();
	glm::mat4 view = glm::lookAt(eye, look, up);

	glm::mat4 projection = glm::perspective(45.f, m_aspect, 0.1f, 500.f);

	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * mview;

	glm::vec4 lightLocation(50, 50, 50, 1);
	glm::vec3 Kd(1, 1, 0);
	glm::vec3 Id(1, 1, 1);

	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(imvp));

	//버퍼 초기화
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, m_width, m_height);
	glEnable(GL_DEPTH_TEST);

	//그려줌
	shaderProgram->use(); // vertex shader 호출

	glUniform4fv(shaderProgram->uniform("lightLocation"), 1, glm::value_ptr(lightLocation));
	glUniform3fv(shaderProgram->uniform("Kd"), 1, glm::value_ptr(Kd));
	glUniform3fv(shaderProgram->uniform("Id"), 1, glm::value_ptr(Id));

	glUniformMatrix4fv(shaderProgram->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(shaderProgram->uniform("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	if(m_cow) m_cow->draw();

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
