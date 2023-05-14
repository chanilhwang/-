/******************************************************************************
FileName: particle.h
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#ifndef PARTICLE_H

#include "hreaper_general.h"

class particle
{
public:
    point Pos;
    point Vel;
    point Scale;
    color Color;
};

class emitter
{
public:
    point Pos;
    bool Visible;
    particle* Particles;
    int ParticleNum;

    void Initialize(int _ParticleNum);
    void CreateParticle(particle *Particle);
    void UpdateParticles(float dt);
    void DeleteParticles(void);
};

#define PARTICLE_H
#endif