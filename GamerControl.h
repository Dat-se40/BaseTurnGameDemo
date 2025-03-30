#pragma once
#include <iostream>
#include "fighter.h"
using namespace std; 

class Computer {
protected: 
    Fighter* ComputerChar;
};

class Player {
protected: 
    Fighter* PlayerChar; 
};

class GamerControl : public Computer, public Player {
public: 
    void ChooseCharacter(Fighter*& character);
    static void CharacterTurn(Fighter*& attacker, Fighter*& defenser);
    void RunGame();
};
