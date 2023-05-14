#include "MyGlWindow.h"

MyGlWindow::MyGlWindow(int w, int h) : m_width(w), m_height(h)
{
	setupBuffer();
}

ShaderProgram* shaderProgram;
GLuint vaoHandle; // VAO handler
GLuint vboPosition; // VBO handler
GLuint vboColor;

void MyGlWindow::setupBuffer() 
{
	shaderProgram = new ShaderProgram(); // 쉐이더 오브젝트 생성
	shaderProgram->initFromFiles("simple.vert", "simple.frag");
	
	const float vertexPosition[] = {
		-0.2f, -0.2f, 0.f,
		0.2f, -0.2f, 0.f,
		0.2f, 0.2f, 0.f
	};

	const float vertexColor[] = {
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f
	};



	//VAO 생성
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle); // 여기에 작업할거다 activate 의미

		//VBO 생성 - 포지션
		glGenBuffers(1, &vboPosition);
		glBindBuffer(GL_ARRAY_BUFFER, vboPosition); // vbo 이용
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPosition), vertexPosition, GL_STATIC_DRAW);
		glVertexAttribPointer(
			0, // attrib 번호
			3, // # of data for each vertex
			GL_FLOAT, // data type
			GL_FALSE, // normalized
			0,
			0
		);
		glEnableVertexAttribArray(0);
	
		//VBO 생성 - 컬러
		glGenBuffers(1, &vboColor);
		glBindBuffer(GL_ARRAY_BUFFER, vboColor);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColor), vertexColor, GL_STATIC_DRAW);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			0
		);
		glEnableVertexAttribArray(1);


	glBindVertexArray(0); // VAO를 close
}

void MyGlWindow::draw() 
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, m_width, m_height);
	glEnable(GL_DEPTH_TEST);

	//그려줌
	shaderProgram->use(); // vertex shader 호출

		glBindVertexArray(vaoHandle);

		//normal draw
		glDrawArrays(GL_TRIANGLES, 0, 3);

	shaderProgram->disable(); // vertex shader close
}

void MyGlWindow::setSize(int w, int h)
{
	m_width = w;
	m_height = h;
}