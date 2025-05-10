#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
using namespace std;

struct Unit {
    string name;
    int speed;
    int hp;
    bool isPlayer;
    Unit* left;
    Unit* right;
    Unit(string n, int s, int h, bool player = false) : name(n), speed(s), hp(h), isPlayer(player), left(nullptr), right(nullptr) {}
};

// Insert into BST based on speed
Unit* insert(Unit* root, Unit* newUnit) {
    if (!root) return newUnit;
    if (newUnit->speed < root->speed)
        root->left = insert(root->left, newUnit);
    else
        root->right = insert(root->right, newUnit);
    return root;
}

// Reverse inorder traversal to get turn order
void fillTurnOrder(Unit* root, vector<Unit*>& order) {
    if (!root) return;
    fillTurnOrder(root->right, order);
    order.push_back(root);
    fillTurnOrder(root->left, order);
}

bool isAnyPlayerAlive(vector<Unit*>& units) {
    for (auto u : units) {
        if (u->isPlayer && u->hp > 0) return true;
    }
    return false;
}

void printStatus(vector<Unit*>& units) {
    cout << "\n-- Current HP --\n";
    for (auto u : units) {
        cout << u->name << ": " << u->hp << " HP" << endl;
    }
}

int main() {
    // Create units
    Unit* enemy = new Unit("Enemy", 80, 100);
    Unit* hero = new Unit("Hero", 120, 100, true);
    Unit* mage = new Unit("Mage", 100, 80, true);
    Unit* tank = new Unit("Tank", 60, 150, true);
    Unit* archer = new Unit("Archer", 90, 90, true);

    // Build BST
    Unit* root = enemy;
    root = insert(root, hero);
    root = insert(root, mage);
    root = insert(root, tank);
    root = insert(root, archer);

    // Prepare turn order
    vector<Unit*> turnOrder;
    fillTurnOrder(root, turnOrder);

    stack<string> actionHistory;
    queue<string> statusEffects;

    int round = 1;
    while (enemy->hp > 0 && isAnyPlayerAlive(turnOrder)) {
        cout << "\n=== Round " << round++ << " ===\n";
        for (Unit* u : turnOrder) {
            if (enemy->hp <= 0 || !isAnyPlayerAlive(turnOrder)) break;
            if (u->hp <= 0) continue; // Skip dead units

            if (u == enemy) {
                // Attack a random alive player (for simplicity, attack first alive)
                for (Unit* target : turnOrder) {
                    if (target->isPlayer && target->hp > 0) {
                        cout << u->name << " attacks " << target->name << "!\n";
                        target->hp -= 10;
                        actionHistory.push(u->name + " attacked " + target->name + " (-10 HP)");
                        break;
                    }
                }
            } else {
                int choice;
                cout << u->name << "'s turn! Choose action: 1) Attack  2) Skip\n> ";
                cin >> choice;
                if (choice == 1) {
                    cout << u->name << " attacks Enemy!\n";
                    enemy->hp -= 15;
                    actionHistory.push(u->name + " attacked Enemy (-15 HP)");
                    statusEffects.push(u->name + " gains +5 DEF for 1 turn");
                } else {
                    cout << u->name << " skipped their turn.\n";
                    actionHistory.push(u->name + " skipped their turn");
                }
            }
        }
        printStatus(turnOrder);
    }

    if (enemy->hp <= 0) cout << "\nEnemy defeated!\n";
    else cout << "\nAll players defeated!\n";

    cout << "\n-- Action History --\n";
    while (!actionHistory.empty()) {
        cout << actionHistory.top() << endl;
        actionHistory.pop();
    }

    cout << "\n-- Status Effects --\n";
    while (!statusEffects.empty()) {
        cout << statusEffects.front() << endl;
        statusEffects.pop();
    }

    return 0;
}
