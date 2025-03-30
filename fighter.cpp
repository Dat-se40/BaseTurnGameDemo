#include "fighter.h"

// ======================== Skill Methods ========================
bool Skill::CanUse(Fighter& user, Fighter& opponent) {
    if (user.currMANA < manaCost) {
        cout << user.GetName() << " does not have enough mana for " << name << "!\n";
        return false;
    }

    for (auto& condition : conditions) {
        if (!condition(user, opponent)) {
            cout << user.GetName() << " does not meet the conditions for " << name << "!\n";
            return false;
        }
    }

    return true;
}

void Skill::Use(Fighter& user, Fighter& opponent) {
    if (CanUse(user, opponent)) {
        cout << user.GetName() << " used " << name << "!\n";
        user.currMANA -= manaCost;
        effect(user, opponent);
    }
}

// ======================== Fighter Methods ========================
Fighter::Fighter(int h, int a, int d, int m, string s)
    : currHP(h), maxHP(h), currATK(a), maxATK(a), currDEF(d), maxDEF(d), currMANA(m), maxMANA(m), name(s) {}

void Fighter::NormalAttack(Fighter& opponent) {
    int damage = currATK - opponent.GetDEF();
    if (damage > 0) opponent.TakenDamage(damage);
}

void Fighter::TakenDamage(int damage) {
    currHP -= damage;
}

void Fighter::ManaRegain(int amount) {
    currMANA = min(maxMANA, currMANA + amount);
}

void Fighter::AddSkill(Skill skill) {
    skills.push_back(skill);
}

void Fighter::UseSkill(int index, Fighter& opponent) {
    if (index >= 0 && index < skills.size()) {
        skills[index].Use(*this, opponent);
    } else {
        cout << "Invalid skill index!\n";
    }
}

void Fighter::CurrStatus() {
    cout << name << "'s Status\n";
    cout << "HP: " << currHP << "/" << maxHP << "\n";
    cout << "ATK: " << currATK << "\n";
    cout << "DEF: " << currDEF << "\n";
    cout << "MANA: " << currMANA << "/" << maxMANA << "\n";
    cout << "--------------------------\n";
}

int Fighter::GetMANA(){ return currMANA ;};
int Fighter::GetATK(){ return currHP ; }
int Fighter::GetHP() { return currHP; }
int Fighter::GetDEF() { return currDEF; }
string Fighter::GetName() { return name; }
bool Fighter::IsAlive() { return currHP > 0; }

// ======================== Assassin Methods ========================
Assassin::Assassin() : Fighter(30, 8, -1, 6, "Assassin") {
    AddSkill(Skill("Bloody Attack", 2,
        {
            [](Fighter& user, Fighter& opponent) { return opponent.GetHP() > opponent.GetHP() * 0.7; },
            [](Fighter& user, Fighter& opponent) { return user.GetHP() > user.GetHP() * 0.5; }
        },
        [](Fighter& user, Fighter& opponent) {
            int damage = user.GetATK() * 1.5;
            opponent.TakenDamage(damage);
            cout << "Dealt " << damage << " bonus damage!\n";
        }
    ));
}

// ======================== Berserker Methods ========================
Berserker::Berserker() : Fighter(20, 10, -2, 6, "Berserker") {
    AddSkill(Skill("Charging Attack", 3,
        {
            [](Fighter& user, Fighter& opponent) { return user.GetHP() > user.GetHP() * 0.3; }
        },
        [](Fighter& user, Fighter& opponent) {
            int damage = user.GetATK() * 1.2 + user.GetHP() * 0.2;
            opponent.TakenDamage(damage);
            cout << "Berserker charged and dealt " << damage << " damage!\n";
        }
    ));
}

// ======================== Beast Methods ========================
Beast::Beast() : Fighter(50, 5, 2, 10, "Beast") {
    AddSkill(Skill("Starve!", 4,
        {
            [](Fighter& user, Fighter& opponent) { return user.GetMANA() > 3; }
        },
        [](Fighter& user, Fighter& opponent) {
            int damage = user.GetHP() * 0.2;
            opponent.TakenDamage(damage);
            cout << "Beast consumed the enemy, dealing " << damage << " damage!\n";
        }
    ));
}
