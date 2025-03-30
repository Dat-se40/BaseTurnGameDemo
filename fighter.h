#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <functional>

using namespace std;

class Fighter; // Khai báo trước

class Skill {
public:
    string name;
    int manaCost;
    vector<function<bool(Fighter&, Fighter&)>> conditions;
    function<void(Fighter&, Fighter&)> effect;

    Skill(string n, int cost,
          vector<function<bool(Fighter&, Fighter&)>> conds,
          function<void(Fighter&, Fighter&)> eff)
        : name(n), manaCost(cost), conditions(conds), effect(eff) {}

    bool CanUse(Fighter& user, Fighter& opponent);
    void Use(Fighter& user, Fighter& opponent);

    friend class Fighter;
};

class Fighter {
protected:
    int currHP, maxHP;
    int currATK, maxATK;
    int currDEF, maxDEF;
    int currMANA, maxMANA;
    string name;
    vector<Skill> skills;

    friend class Skill;

public:
    Fighter(int h, int a, int d, int m, string s);
    virtual void NormalAttack(Fighter& opponent);
    void TakenDamage(int damage);
    void CurrStatus();
    bool IsAlive();
    void ManaRegain(int amount);
    void AddSkill(Skill skill);
    void UseSkill(int index, Fighter& opponent);

    int GetMANA() ;
    int GetATK();
    int GetHP();
    int GetDEF();
    string GetName();
};

class Assassin : public Fighter {
public:
    Assassin();
};

class Berserker : public Fighter {
public:
    Berserker();
};

class Beast : public Fighter {
public:
    Beast();
};
