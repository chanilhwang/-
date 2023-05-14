#include "MyBridge.h"
#include "gl/glut.h"

MyBridge::MyBridge(cyclone::ParticleWorld* world)
{
    //init movers
    for (int i = 0; i < 12; i++) {
        Mover* m = new Mover(cyclone::Vector3(5 - 2 * (i / 2), 8, (i % 2 == 1) ? -1 : 1));
        m->m_particle->setMass(5.0);
        m->m_particle->setDamping(0.8);
        m->m_particle->clearAccumulator();
        m_movers.push_back(m);
    }

    //gravity
    m_gravity = new cyclone::ParticleGravity(cyclone::Vector3::GRAVITY);

    //particle world
    for (int i = 0; i < m_movers.size(); i++) {
        world->getParticles().push_back(m_movers[i]->m_particle);
        world->getForceRegistry().add(m_movers[i]->m_particle, m_gravity);
    }

    //cable
    for (int i = 0; i < 10; i++) {
        cyclone::ParticleCable* c = new cyclone::ParticleCable();
        c->particle[0] = m_movers[i]->m_particle;
        c->particle[1] = m_movers[i + 2]->m_particle;
        c->maxLength = 3.0f;
        c->restitution = 1.0f;
        world->getContactGenerators().push_back(c);
        m_cables.push_back(c);
    }

    //rod
    for (int i = 0; i < 6; i++) {
        cyclone::ParticleRod* c = new cyclone::ParticleRod();
        c->particle[0] = m_movers[i * 2]->m_particle;
        c->particle[1] = m_movers[i * 2 + 1]->m_particle;
        c->length = 2;
        world->getContactGenerators().push_back(c);
        m_rods.push_back(c);
    }

    //cable constraint
    for (int i = 0; i < 12; i++) {
        cyclone::ParticleCableConstraint* c = new cyclone::ParticleCableConstraint();
        c->particle = m_movers[i]->m_particle;
        c->anchor = cyclone::Vector3(5 - 2 * (i / 2), 8, (i % 2 == 1) ? -1 : 1);
        c->maxLength = 3.0f + (0.5 * ((i < 6) ? i / 2 : (11 - i) / 2));
        c->restitution = 0.5f;
        world->getContactGenerators().push_back(c);
        m_supports.push_back(c);
    }

    //collision
    for (int i = 0; i < m_movers.size() - 2; i++) {
        cyclone::ParticleCollision* c = new cyclone::ParticleCollision(0.5);
        c->particle[0] = m_movers[i]->m_particle;
        c->particle[1] = m_movers[i + 2]->m_particle;
        world->getContactGenerators().push_back(c);
        m_collisions.push_back(c);
    }
}

const std::vector<Mover*>& MyBridge::getMovers() const
{
    return m_movers;
}

void MyBridge::draw(int shadow)
{
    glLineWidth(3.0);

    if (shadow)
        glColor3f(0.2, 0.2, 0.2);
    else
        glColor3f(0.8, 0, 0);

    int name = 1;
    for each (Mover * m in m_movers) {
        cyclone::Particle* particle = m->m_particle;
        const cyclone::Vector3& pos = particle->getPosition();
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        if (!shadow)
            glLoadName(name);
        glutSolidSphere(0.2f, 20, 10);
        glPopMatrix();
        name++;
    }

    glBegin(GL_LINES);

    if (shadow)
        glColor3f(0.2, 0.2, 0.2);
    else
        glColor3f(0, 0, 1);
    for (unsigned i = 0; i < m_rods.size(); i++)
    {
        cyclone::Particle** particles = m_rods[i]->particle;
        const cyclone::Vector3& p0 = particles[0]->getPosition();
        const cyclone::Vector3& p1 = particles[1]->getPosition();
        glVertex3f(p0.x, p0.y, p0.z);
        glVertex3f(p1.x, p1.y, p1.z);
    }

    if (shadow)
        glColor3f(0.2, 0.2, 0.2);
    else
        glColor3f(0, 1, 0);
    for (unsigned i = 0; i < m_cables.size(); i++)
    {
        cyclone::Particle** particles = m_cables[i]->particle;
        const cyclone::Vector3& p0 = particles[0]->getPosition();
        const cyclone::Vector3& p1 = particles[1]->getPosition();
        glVertex3f(p0.x, p0.y, p0.z);
        glVertex3f(p1.x, p1.y, p1.z);
    }

    if (shadow)
        glColor3f(0.2, 0.2, 0.2);
    else
        glColor3f(0.7f, 0.7f, 0.7f);
    for (unsigned i = 0; i < m_supports.size(); i++)
    {
        const cyclone::Vector3& p0 = m_supports[i]->particle->getPosition();
        const cyclone::Vector3& p1 = m_supports[i]->anchor;
        glVertex3f(p0.x, p0.y, p0.z);
        glVertex3f(p1.x, p1.y, p1.z);
    }
    glEnd();

    glLineWidth(1.0);

    if (shadow)
        glColor3f(0.1, 0.1, 0.1);
    else
        glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_QUADS);
    for (unsigned i = 0; i < m_movers.size() / 2 - 1; i++)
    {
        const cyclone::Vector3& p0 = m_movers[2 * i]->m_particle->getPosition();
        const cyclone::Vector3& p1 = m_movers[2 * i + 2]->m_particle->getPosition();
        const cyclone::Vector3& p2 = m_movers[2 * i + 3]->m_particle->getPosition();;
        const cyclone::Vector3& p3 = m_movers[2 * i + 1]->m_particle->getPosition();;
        glVertex3f(p0.x, p0.y, p0.z);
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
        glVertex3f(p3.x, p3.y, p3.z);
    }
    glEnd();
}
