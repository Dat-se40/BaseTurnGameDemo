#include"entity.h"
int main(){
    Caster Cassandra ;
    Lancer Romulus ; 
    Romulus.ShowStatus() ; 
    Cassandra.ShowStatus() ; 
    Romulus.UseSkill(2,Romulus);
    Cassandra.UseSkill(1,Romulus);
    Cassandra.UpdateStatus(Cassandra,Romulus);
    Romulus.UpdateStatus(Romulus,Cassandra);
    Romulus.ShowStatus() ; 
    Cassandra.ShowStatus() ; 
    return 0 ; 
}