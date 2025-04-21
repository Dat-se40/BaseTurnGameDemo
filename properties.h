#pragma once
#include<iostream>
#include<functional>

using namespace std ; 

class Character ; 


class Stat{
protected: 
    float currentVal; 
    float maxVal ;
public: 
    Stat();
    Stat(float val ) ;

    void Increase( float val ) ;
    void Decrease( float val) ;
    void Restore( float val ) ; 
    float GetCurrVal() ; 
    float GetMaxVal() ; 
};

class Skill{
public: 
    string name ;// Ten ki nang 
    float manaCost ; // Gia tri su dung
    Stat coolDown ;

    vector<function<bool(Character&, Character&)>> conditions ; //Dieu kien kich hoat skill
    vector<function<void(Character&, Character&)>> effect ; // Gay sat thuong, gay hieu ung
    Skill(string n , float cost, int cooldown ,
        vector<function<bool(Character&, Character&)>> condi , 
        vector<function<void(Character&, Character&)>> effect   
    );

    bool CanUse(Character& user ,Character& opponnent) ; 
    void Use(Character& user, Character& opponnet) ;
    bool isOnCoolDown() ;  
};

class Effect{
protected:
    string name ;
    Stat coolDown ;
    Stat number_Activations ;
    // bool isExclusive (cho may cac buff/debuff) khong trung nhau
    vector<function<void(Character& , Character&) >> impact ; 
public : 
    Effect(string n , int cd , int NA,
        vector<function<void(Character& , Character&) >> imt 
    ) ;
    friend class Character ; 
    bool isVail() ; 
    // Kiểu người gây(source) ra người hứng chịu(destination)
    virtual void Act(Character& source , Character& destination)   ; 
   
};

class Buff : public Effect{
// so lan kich hoat
public: 
    Buff(string n , int cd , int NA,
    vector<function<void(Character& , Character&) >> imt 
    ) ;
    void Act(Character& source , Character& destination) override;
};

class Debuff : public Effect{
public:
    Debuff(string n , int cd , int NA,
        vector<function<void(Character& , Character&) >> imt 
        ) ;
    void Act(Character& taken , Character& source) override;
} ; 