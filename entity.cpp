#include"entity.h"
// ==== Character ==== //
//[ stat ]//
Character::Character(): HP() , ATK() , DEF() , MANA() , name("") {}

Character::Character(float h , float a , float d, float m, string n  ): 
    HP(h) , ATK(a) , DEF(d) , MANA(m) , name(n) {
        AddSkill(Skill("Normal Attack", 0 , 0, 
           {
            [](Character&, Character&){ return true ; }
           },{
            [](Character&user, Character&opponnet){
                opponnet.TakenDamange( user.ATK.GetCurrVal() );
                user.MANA.Restore( user.MANA.GetMaxVal()*0.3);
               }
           }
        ));
    }
Stat& Character::GetHPStat() { return HP; }
Stat& Character::GetATKStat() { return ATK; }
Stat& Character::GetDEFStat() { return DEF; }
Stat& Character::GetManaStat() { return MANA; }
string& Character::GetName(){return name ;};
bool Character::isAlive(){ 
    return  this->HP.GetCurrVal() > 0 ;
}   
void Character::TakenDamange(float dmg){
    //Se up date cai cong thuc cho nay 
    float taken = dmg - this->DEF.GetCurrVal()/2 ; 
    float before = HP.GetCurrVal() ; 
    HP.Decrease( (taken > 0 ) ? taken : 0 ); 
    cout<<this->name<<" is taken "<<before - HP.GetCurrVal()<<" damage"<<endl;

}
//[ skill ]//
void Character::AddSkill(Skill newSkill ){ 
    skills.push_back(newSkill) ;
}
Skill& Character::GetSkill( int index){
    return skills[index];
}
void Character::UseSkill(int index , Character& opponnent){
    skills[index].Use(*this , opponnent);
}
void Character::ShowStatus(){
    cout<<"===============\n";
    cout<<"Character: "<<this->name<<"\n" ; 
    cout<<"ATK: "<<this->ATK.GetCurrVal()<<"\n";
    cout<<"HP: "<<this->HP.GetCurrVal()<<"\n";
    cout<<"DEF: "<<this->DEF.GetCurrVal()<<"\n";
    cout<<"MANA: "<<this->MANA.GetCurrVal()<<"\n";
}
void Character::UpdateStatus(Character& user , Character & opponnet){
    //Update cooldown ;
    for( auto& S : skills){
        if( S.isOnCoolDown()) S.coolDown.Decrease(1);
    }

    vector<Effect> activeEffects;
    
    for (auto& E : EffectCarrying) {
        if (E.isVail()) {
            E.Act(user,opponnet);
            activeEffects.push_back(E);
        }
    }
    
    // Cập nhật danh sách hiệu ứng
    EffectCarrying = activeEffects;
}
void Character::AddEffect(Effect newEffect){
    EffectCarrying.push_back(newEffect) ;
}

// ===== Caster ====== ///
Caster::Caster() : Character(50,10,3,20,"Caster") {
    this->skills[0].effect.push_back(
        [](Character& user, Character& opponent) {
            opponent.GetManaStat().Decrease(opponent.GetManaStat().GetCurrVal() / 10);
        }
    );
    this->AddSkill( Skill("Blazing Flame", 4 , 1,
        {//Conditions
            [](Character& user, Character& opponent) {
                return true ; // Dang khong co dieu kien
            }
        },{//Effect
            [](Character& user , Character& opponent){
                float damage = user.GetATKStat().GetCurrVal()*1.2 + 
                            opponent.GetManaStat().GetCurrVal()*0.2 ; 
                opponent.TakenDamange(damage) ;
                // Them cai burn voi nua ;C
                cout<<user.GetName()<<" set Burn effect on "<<opponent.GetName()<<"\n";
                opponent.AddEffect(Debuff("Burn",3,3,{//impt
                    [](Character& taken, Character& source){
                        cout<<"Taken: "<<taken.GetName()<<"\n";
                        Stat& damage = taken.GetHPStat();
                        taken.TakenDamange(taken.GetHPStat().GetMaxVal()*0.3);
                    }
                }));
            }


        }
    ));
}

// ===== Lancer ===== //
Lancer::Lancer() : Character(55,11,4,15,"Lancer"){
    this->AddSkill( Skill("Lightning-style",5,0,
        {
            [](Character& user , Character& opponnet){
                return true ;
            }
        },{
            [](Character& user , Character& opponnet){
                float damage = user.GetATKStat().GetCurrVal()*1.7 
                + user.GetATKStat().GetMaxVal();
                opponnet.TakenDamange(damage);
            }
        }
    ));

    this->AddSkill( Skill("Concentrate",5,2,
        {
            [](Character& user , Character& opponent){
                return true ;
            }
        },{// Thay mac gom =))
            [](Character& user , Character& opponent){
                user.AddEffect( Buff("Charisma",3,2,{
                    [](Character& source, Character& destination){
                        Stat& increment = source.GetATKStat() ; 
                        increment.Increase( increment.GetMaxVal()*0.3) ;
                    }
                }));
            },
            [](Character& user, Character& opponent){
                user.AddEffect(Buff("Counter",3,1,{
                    [](Character& source, Character& destination){
                        Stat& target = source.GetHPStat() ; 
                        if (target.GetCurrVal() < target.GetMaxVal() ){
                            cout<<source.GetName()<<" counter attack to "<<source.GetName()<<endl; 
                            destination.TakenDamange( source.GetATKStat().GetCurrVal()*0.3);
                        }
                    }
                }));
            }
            
        }
    ));
}  
// ===== Beast ===== //
Beast::Beast(): Character(80,4,2,20,"Beast of Disire") {
    this->AddSkill( Skill("Hate Spike",0,0,
        {
            [](Character& user , Character& opponent){
                return true ;
            }
        },{
            [](Character& user , Character& opponent){
                float damage = user.GetATKStat().GetCurrVal() ; 
                opponent.TakenDamange(damage);
            }
        }
    ));
    this->AddSkill( Skill("Allure",5,3,
    {
        [](Character& user , Character& opponent){
            return true ;
        }
    },{
        [](Character& user , Character& opponent){
            opponent.AddEffect(Debuff("Burn",3,3,{//impt
                [](Character& taken, Character& source){
                    taken.GetDEFStat().Decrease(taken.GetDEFStat().GetCurrVal()*0.8);
                }
            }));
        }
    }));
    //Passive 
    this->AddEffect( TriggerEffect(
        "Molt",
        {
            [](Character& user )->bool{
                return user.GetHPStat().GetCurrVal() < user.GetHPStat().GetMaxVal()*0.5 ;
            },
        },{
            [](Character& user, Character& opponent){
                cout<<user.GetName()<<"'s HP: "<<user.GetHPStat().GetCurrVal()<<"\n";
                user.GetATKStat().Increase(opponent.GetATKStat().GetCurrVal()*0.4);
                user.UseSkill(1,opponent);
            }
        }
    ));
}
