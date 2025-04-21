#include "properties.h"
#include "entity.h"
// ====== STAT ====== //
Stat::Stat(float val) : currentVal(val), maxVal(val) {}
Stat::Stat() : currentVal(0), maxVal(0) {}

void Stat::Increase(float val)
{
    currentVal += val;
}

void Stat::Decrease(float val)
{
    currentVal -= val;
    if (currentVal < 0)
        currentVal = 0;
}

void Stat::Restore(float val)
{
    Increase(val);
    currentVal = (currentVal > maxVal) ? maxVal : currentVal;
}
float Stat::GetCurrVal() { return currentVal; }

float Stat::GetMaxVal() { return maxVal; }
// ==== Skill ==== ///
Skill::Skill(string n, float cost, int cooldown,
             vector<function<bool(Character &, Character &)>> condi,
             vector<function<void(Character &, Character &)>> eff)
    : name(n), manaCost(cost), conditions(condi), effect(eff), coolDown(coolDown)
{
    coolDown.Decrease(coolDown.GetMaxVal());
}

bool Skill::CanUse(Character &user, Character &opponnet)
{
    if (user.MANA.GetCurrVal() < manaCost)
    {
        cout << user.name << " doesn't have enough mana to cast " << this->name << "\n";
        return false;
    }
    else if (this->isOnCoolDown())
    {
        cout << this->name << " is on cooldow\n";
        return false;
    }
    else
    {
        for (auto &condition : conditions)
        {
            if (condition(user, opponnet) == false)
            {
                cout << "Doesn't meet conditions to cast " << this->name << "\n";
                return false;
            }
        }
    }
    return true;
}

void Skill::Use(Character &user, Character &opponnet)
{
    cout << user.name << " uses " << this->name << "\n";
    user.MANA.Decrease(this->manaCost);
    for (auto &e : effect)
    {
        e(user, opponnet);
    }
    // Set up cooldon
    this->coolDown.Increase(coolDown.GetMaxVal());
}

bool Skill::isOnCoolDown()
{
    return coolDown.GetCurrVal() > 0;
}
// ==== Effect ===== //
Effect::Effect(string n, int cd, int NA,
               vector<function<void(Character &, Character &)>> imt) : name(n), coolDown(cd), number_Activations(NA), impact(imt) {}
bool Effect::isVail()
{
    return coolDown.GetCurrVal() > 0;
}
void Effect::Act(Character &source, Character &destination)
{
    if ( number_Activations.GetCurrVal() ==  0 ) {
        coolDown.Decrease(1);
        return ; 
    }
    
    for (auto &imp : impact)
    {
        imp(source, destination);
    }
    // Giảm thời gian tồn tại của hiệu ứng
    coolDown.Decrease(1);
    number_Activations.Decrease(1);
}

// ===== Buff ====== //
Buff::Buff(string n, int cd, int NA,
           vector<function<void(Character &, Character &)>> imt)
    : Effect(n, cd, NA, imt) {}
void Buff::Act(Character &source, Character &destination)
{
    // Áp dụng buff từ source lên source (tự tăng cường)
    if ( number_Activations.GetCurrVal() ==  0 ) {
        coolDown.Decrease(1);
        return ; 
    }
    for (auto &imp : impact)
    {
        imp(source, source);
    }
    // Giảm thời gian tồn tại
    coolDown.Decrease(1);
}
// ===== Debuff ===== //
Debuff::Debuff(string n, int cd, int NA,
               vector<function<void(Character &, Character &)>> imt)
    : Effect(n, cd, NA, imt) {}
void Debuff::Act(Character &taken, Character &source)
{
    // Áp dụng debuff từ source lên destination
    if ( number_Activations.GetCurrVal() ==  0 ) {
        coolDown.Decrease(1);
        return ; 
    }
    for (auto &imp : impact)
    {   
        imp(taken, source);
    }
    // Giảm thời gian tồn tại
    coolDown.Decrease(1);
    number_Activations.Decrease(1);
}