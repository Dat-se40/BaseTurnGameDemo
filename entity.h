#pragma once 
#include "properties.h"
#include<iostream>
#include<functional>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std ; 

class Character{
protected: 
    Stat HP ; 
    Stat ATK ; 
    Stat DEF ; 
    Stat MANA ; 
    string name ;
    //se them speed 
    vector<Skill> skills;
    vector<Effect> EffectCarrying;
    unordered_map<string,vector<function<void(Character& user, Character& opponent,float)>>> eventHandle;
public: 
    Character() ; 
    Character(float h , float a , float d, float m, string n  ) ;
    virtual void TakenDamange(float dmg) ;
    bool isAlive() ;
    void ShowStatus() ; 

    Stat& GetHPStat() ;
    Stat& GetATKStat() ;
    Stat& GetDEFStat() ;
    Stat& GetManaStat() ;
    string& GetName() ; 

    friend class Skill ; 
    void AddSkill( Skill newSkill) ;
    Skill& GetSkill( int index) ;
    void UseSkill( int index, Character& opponnent ) ; 
 
    void UpdateStatus(Character& user , Character& opponnet) ; 

    friend class Effect ; 
    void AddEffect( Effect newEffect);

};

class Caster : public Character{
public:  
    Caster() ; 
};
class Lancer : public Character{
public : 
    Lancer() ; 
};
class Beast : public Character{
public :
    Beast() ;
};
