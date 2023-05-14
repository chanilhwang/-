/******************************************************************************
FileName: particle.cpp
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "particle.h"
#include "WERandom.h"
namespace
{
}

void emitter::Initialize(int _ParticleNum)
{
    Visible = true;
    Pos = {0};
    ParticleNum = _ParticleNum; 
    Particles = new particle[ParticleNum];
}

void emitter::CreateParticle(particle *Particle)
{
    Particle->Pos.X = WERandom::GetFloat(Pos.X - 70, Pos.X + 70);
    Particle->Pos.Y = WERandom::GetFloat(Pos.Y - 70, Pos.Y + 70);
    Particle->Vel.X = WERandom::GetFloat(-7, 7);
    Particle->Vel.Y = WERandom::GetFloat(-7, 7);
    Particle->Scale.X = WERandom::GetFloat(0.5f, 2.0f);
    Particle->Scale.Y = Particle->Scale.X;
    
    int color = WERandom::GetInt(-1, 7);
    switch (color)
    {
    case 0:
    case 1:
    case 2:
        //Yellow
        Particle->Color.Red   = (unsigned char)WERandom::GetInt(230, 255);
        Particle->Color.Green = (unsigned char)WERandom::GetInt(180, 210);
        Particle->Color.Blue  = 139;
        break;
    case 3:
    case 4:
    case 5:
        //Bright Blue
        Particle->Color.Red   = (unsigned char)253;
        Particle->Color.Green = (unsigned char)WERandom::GetInt(120, 180);        
        Particle->Color.Blue  = 80;
        break;
    case 6:
        //Bright Blue
        Particle->Color.Red   = (unsigned char)70;
        Particle->Color.Green = (unsigned char)WERandom::GetInt(200, 255);        
        Particle->Color.Blue  = (unsigned char)WERandom::GetInt(160, 200);
        break;

    }

    Particle->Color.Alpha = WERandom::GetFloat(50.0f, 170.0f);
}

void emitter::UpdateParticles(float dt)
{
    if(!Visible)
        return;
    
    for(int Index = 0;
        Index < ParticleNum;
        ++Index)
    {
        Particles[Index].Pos.X += Particles[Index].Vel.X * dt;
        Particles[Index].Pos.Y += Particles[Index].Vel.Y * dt;
        Particles[Index].Color.Alpha -= dt * 0.3f;
        Particles[Index].Scale.X -= dt * 0.25f * 3.5f;
        Particles[Index].Scale.Y -= dt * 0.25f * 3.5f;

        if (Particles[Index].Scale.Y < 0 || 
            Particles[Index].Scale.Y < 0 ||
            Particles[Index].Color.Alpha <= 0)
        {
            CreateParticle(&Particles[Index]);
        }
    }
}

void emitter::DeleteParticles(void)
{
    delete[] Particles;
}