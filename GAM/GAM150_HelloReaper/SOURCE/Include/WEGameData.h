/******************************************************************************/
/*!
\file   WEGameData.h
\author Matt Casanova 
\par    email: mcasanov\@digipen.edu
\par    class:
\par    Assignment:Simple 2D Game Engine
\date   2012/12/5

This is a struct that you can define your own game specific game data in. This
is data that needs to be shared across states, such as player lives or game
score.  You must initilize the values in main before calling 
WEAppilcationInit.
*/
/******************************************************************************/
#ifndef WEGAMEDATA_H
#define WEGAMEDATA_H

/*! This struct holds game data that is shared between states. This struct must
have at least one data member in it.*/
struct WEGameData
{
    //Player Info
    int PlayerShield;
    int PlayerEnergy;
    int PlayerBullet;

    int PlayerUpgrades[7];
    //0 = health
    //1 = energy
    //2 = health - 1 energy + 2
    //3 = line dash
    //4 = Invincible Time while dashing
    //5 = Add Awesomeness(camera shake & hitstop)
    //6 = Increase Player Dash
    
    //Level Info
    int CurrentLevelID;//What level is the player headed?

    /*Options*/
    float BGMVolume;
    float SFXVolume;

    //Where are we in the screen array?
    int ScreenNumber;

    bool FullScreen;
    //Out of game info
    float PlayTime;
    //You may add here
};

#endif 