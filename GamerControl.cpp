#include "GamerControl.h"

void GamerControl::ChooseCharacter(Fighter*& character) {
    cout << "\nChoose your character's class:";
    cout << "\n1. Assassin";
    cout << "\n2. Berserker";
    int n;
    cout << "\nEnter your choice: ";
    cin >> n;
    switch (n) {
    case 1:
        character = new Assassin();
        break;
    case 2:
        character = new Berserker();
        break;
    default:
        cout << "Invalid choice! Defaulting to Assassin.\n";
        character = new Assassin();
        break;
    }
}

void GamerControl::CharacterTurn(Fighter*& attacker, Fighter*& defenser) {
    int choice;
    cout << "\n--------------------------\n";
    cout << " 1. Normal attack\n";
    cout << " 2. Special attack\n";
    cout << "--------------------------";

    bool actionDone = false;
    do {
        cout << "\nYour choice: ";
        cin >> choice;
        if (choice == 1) {
            cout << attacker->GetName() << " uses Normal Attack on " << defenser->GetName() << "!\n";
            attacker->NormalAttack(*defenser);
            actionDone = true;
        } 
        else if (choice == 2) {
            int skillIndex;
            cout << "\nEnter the skill number: ";
            cin >> skillIndex;
            attacker->UseSkill(skillIndex, *defenser);
            actionDone = true;
        } 
        else {
            cout << "Invalid choice! Try again.\n";
        }
    } while (!actionDone);
}

void GamerControl::RunGame() {
    cout << "Choose class of computer's character:";
    ChooseCharacter(ComputerChar);
    cout << "Choose class of player's character:";
    ChooseCharacter(PlayerChar);

    int round = 1, pausecmd;
    while (PlayerChar->IsAlive() && ComputerChar->IsAlive()) {
        cout << "===== ROUND: " << round << " =====";
        
        cout << "\nComputer's turn:";
        CharacterTurn(ComputerChar, PlayerChar);

        if (!PlayerChar->IsAlive()) {
            cout << "\nPlayer lost! Computer wins!\n";
            break;
        }

        cout << "\nPlayer's turn:";
        CharacterTurn(PlayerChar, ComputerChar);

        if (!ComputerChar->IsAlive()) {
            cout << "\nComputer lost! Player wins!\n";
            break;
        }

        cout << "\nDo you want to continue the game?\nPress 0 to stop, 1 to continue, 2 to show status: ";
        cin >> pausecmd;
        if (pausecmd == 0) break;
        else if (pausecmd == 2) {
            cout << "Computer's character:";
            ComputerChar->CurrStatus();
            cout << "Player's character:";
            PlayerChar->CurrStatus();
        }
        round++;
    }
}
