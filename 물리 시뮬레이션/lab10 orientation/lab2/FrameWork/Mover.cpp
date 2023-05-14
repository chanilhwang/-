
#include <iostream>
#include "Mover.h"
#include "gl/glut.h"
#include "timing.h"


float size = 1.5f;


const float DEGREES_TO_RADIAN = 3.141592 / 180.0f;
const float RADIANS_TO_DEGREES = 180.0f / 3.141592;



///////////////////////////////////////////////////////////////////
Mover::Mover(cyclone::Vector3 & p)
{
	m_particle = new cyclone::Particle();
	m_particle->setPosition(p);
	m_orientation = cyclone::Quaternion(1, 0, 0, 0);
	transformMatrix.setOrientationAndPos(m_orientation, p);
	m_angular_damping = 0.8;
}



Mover::~Mover()
{
}

void Mover::update(float duration)
{
	//ori, ang_vel, ang_acc
	m_angular_velocity.addScaledVector(m_angular_acceleration, duration);
	m_angular_velocity *= real_pow(m_angular_damping, duration);
	m_orientation.addScaledVector(m_angular_velocity, duration);
	m_orientation.normalise();

	//vel, acc, pos
	m_particle->integrate(duration);

	//transformMatrix for graphics
	transformMatrix.setOrientationAndPos(m_orientation, m_particle->getPosition());
}


void Mover::getGLTransform(float matrix[16])
{
	
	
	matrix[0] = (float)transformMatrix.data[0];
	matrix[1] = (float)transformMatrix.data[4];
	matrix[2] = (float)transformMatrix.data[8];
	matrix[3] = 0;

	matrix[4] = (float)transformMatrix.data[1];
	matrix[5] = (float)transformMatrix.data[5];
	matrix[6] = (float)transformMatrix.data[9];
	matrix[7] = 0;

	matrix[8] = (float)transformMatrix.data[2];
	matrix[9] = (float)transformMatrix.data[6];
	matrix[10] = (float)transformMatrix.data[10];
	matrix[11] = 0;

	matrix[12] = (float)transformMatrix.data[3];
	matrix[13] = (float)transformMatrix.data[7];
	matrix[14] = (float)transformMatrix.data[11];
	matrix[15] = 1;
}


/** Draws the bone. */
void draw(int shadow)
{
	
}

void Mover::draw(int shadow)
{
	// Get the OpenGL transformation
	GLfloat mat[16];
	getGLTransform(mat);


	if (!shadow) {
		glPushMatrix();
		glMultMatrixf(mat);

		glLineWidth(3.0f);
		glBegin(GL_LINES);
		glColor3f(1, 0, 0);

		glVertex3f(0, 0.1, 0);
		glVertex3f(0, 10, 0);

		glColor3f(0, 1, 0);

		glVertex3f(0, 0.1, 0);
		glVertex3f(10, 0.1, 0);

		glColor3f(0, 0, 1);

		glVertex3f(0, 0.1, 0);
		glVertex3f(0, 0.1, 10);
		glEnd();
		glPopMatrix();
		glLineWidth(1.0f);
	}
	

	

	if (shadow) {
		glColor3f(0.2f, 0.2f, 0.2f);
	}
	else {

		glColor3f(1, 0., 0);
	}


	glPushMatrix();
	glMultMatrixf(mat);

	glutSolidCube(size*2);
	glPopMatrix();
		

}

void Mover::setOrientation(cyclone::Quaternion q)
{
	this->m_orientation = q;
	transformMatrix.setOrientationAndPos(q, m_particle->getPosition());
}


//////////////////////////////

MoverConnection::MoverConnection()
{


	Mover * a = new Mover(cyclone::Vector3(0, 6, 0));
	a->m_angular_velocity = cyclone::Vector3(0, 0, 0);
	a->m_angular_acceleration = cyclone::Vector3(0, 0.3, 0);

	m_movers.push_back(a);

}

MoverConnection::~MoverConnection()
{


}


void MoverConnection::update(float duration)
{
	
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		m_movers[i]->update(duration);
	}
}


void MoverConnection::draw(int shadow)
{

	for (unsigned int i = 0; i < m_movers.size(); i++) {
		if (!shadow)
			glLoadName(i + 1);
		m_movers[i]->draw(shadow);

	}

	
}