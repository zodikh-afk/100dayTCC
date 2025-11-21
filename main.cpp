#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

double round05(double x){
    int t = (int)(x * 2 + 0.5);
    return t / 2.0;
}

double rnd(double a, double b){
    double r = rand() / (double)RAND_MAX;
    return a + r * (b - a);
}

class Item{
public:
    string name;
    Item(string n) : name(n) {}
    virtual ~Item() {}
    virtual void showInfo() { cout << name; }
};

class ExpeditionItem : public Item{
public:
    int survivalBonus;
    int badEndingChance;
    string badEndingText;

    ExpeditionItem(string n, int bonus, int badChance, string endText)
        : Item(n), survivalBonus(bonus), badEndingChance(badChance), badEndingText(endText) {}

    void showInfo() override {
        cout << name << " (+" << survivalBonus << "% шанс повернення)";
    }
};

class HomeItem : public Item {
public:
    string effect;
    HomeItem(string n, string e) : Item(n), effect(e) {}
    void showInfo() override { cout << name << " (ефект: " << effect << ")"; }
};

class Player {
public:
    vector<ExpeditionItem> expeditionItems;
    vector<HomeItem> homeItems;

    void showInventory() {
        cout << "\n--- Інвентар гравця ---\n";
        cout << "Предмети для вилазок:\n";
        if (expeditionItems.empty()) cout << "  (немає)\n";
        else {
            for (int i = 0; i < expeditionItems.size(); i++) {
                cout << "  " << i+1 << ") ";
                expeditionItems[i].showInfo();
                cout << endl;
            }
        }

        cout << "Домашні предмети:\n";
        if (homeItems.empty()) cout << "  (немає)\n";
        else {
            for (auto &item : homeItems) {
                cout << "  - ";
                item.showInfo();
                cout << endl;
            }
        }
    }
};

class Game {
private:
    int day;
    double water;
    double food;
    vector<ExpeditionItem> allExpeditionItems;

public:
    Player player;

    Game(int difficulty) {
        srand(time(0));
        day = 1;

        if (difficulty == 1) {
            water = round05(rnd(4.5, 6.0));
            food  = round05(rnd(4.5, 6.0));
        } 
        else if (difficulty == 2) {
            water = round05(rnd(2.5, 4.0));
            food  = round05(rnd(2.5, 4.0));
        } 
        else {
            water = round05(rnd(1.0, 1.5));
            food  = round05(rnd(1.0, 1.5));
        }


        allExpeditionItems.push_back(
            ExpeditionItem("Перцовий балончик", 50, 30,
                           "Ви застосували балончик, але вітер змінив напрямок і ви втратили свідомість.")
        );

        allExpeditionItems.push_back(
            ExpeditionItem("Карта місцевості", 25, 5,
                           "Карта порвалась і ти заблукав назавжди.")
        );

        allExpeditionItems.push_back(
            ExpeditionItem("Фальшивий резерв+", 35, 15,
                           "Підробку швидко викрили, і більше вас ніхто не бачив.")
        );

        allExpeditionItems.push_back(
            ExpeditionItem("Фальшиве посвідчення СБУ", 25, 20,
                           "Перевірка виявила фальшивку — вас забрали на допит, з якого ви не повернулись.")
        );

        allExpeditionItems.push_back(
            ExpeditionItem("Травматичний пістолет", 45, 30,
                           "Пістолет заклинив у найгірший момент, і це стало вашою останньою помилкою.")
        );

        allExpeditionItems.push_back(
            ExpeditionItem("Гроші для хабаря", 65, 40,
                           "Дядя тцк не захотів брати хабаря та затримав вас")
        );

        generateRandomStartItems();
    }

    void generateRandomStartItems() {
        vector<int> idx(allExpeditionItems.size());
        for (int i = 0; i < idx.size(); i++) idx[i] = i;
        random_shuffle(idx.begin(), idx.end());
        for (int i = 0; i < 3; i++) {
            player.expeditionItems.push_back(allExpeditionItems[idx[i]]);
        }
    }

    void goOnExpedition() {
        cout << "\n=== Вилазка ===\n";

        int choice = 0;
        if (player.expeditionItems.empty()) {
            cout << "У тебе немає предметів. Підеш з пустими руками? (1 - так, 0 - ні): ";
            cin >> choice;
            cin.ignore();
            if (choice == 0) return;

            int survivalChance = 50;
            int roll = rand() % 100 + 1;
            if (roll <= survivalChance) {
                cout << "Ти повернувся з вилазки.\n";
                lootFind();
            } else {
                cout << "Ти заблукав і не повернувся додому.\n";
                exit(0);
            }
            return;
        }

        cout << "Вибери предмет (0 - без предмета):\n";
        for (int i = 0; i < player.expeditionItems.size(); i++) {
            cout << i+1 << ") ";
            player.expeditionItems[i].showInfo();
            cout << endl;
        }

        cin >> choice;
        cin.ignore();

        int survivalChance = 50;
        int badChance = 0;
        ExpeditionItem* itemPtr = nullptr;

        if (choice == 0) {
            cout << "Ти пішов без предметів.\n";
        } 
        else if (choice > 0 && choice <= player.expeditionItems.size()) {
            itemPtr = &player.expeditionItems[choice-1];
            survivalChance += itemPtr->survivalBonus;
            badChance = itemPtr->badEndingChance;
            cout << "Ти взяв: " << itemPtr->name << endl;
        } 
        else return;

        if (survivalChance > 95) survivalChance = 95;

        int rollBad = rand() % 100 + 1;
        if (rollBad <= badChance) {
            cout << itemPtr->badEndingText << endl;
            exit(0);
        }

        int roll = rand() % 100 + 1;
        if (roll <= survivalChance) {
            cout << "Ти успішно повернувся.\n";
            lootFind();
        } else {
            if (itemPtr) cout << itemPtr->badEndingText << endl;
            else cout << "Ти заблукав і не повернувся.\n";
            exit(0);
        }
    }

    void lootFind() {
        int chance = rand() % 100 + 1;
        if (chance <= 40) {
            double found = rnd(0.5, 1.5);
            water += found;
            
            water = round05(water);
            cout << "Ти знайшов " << water << " води!\n";
        } 
        else if (chance <= 80) {
            double found = rnd(0.5, 1.5);
            food += found;
            food = round05(food);
            cout << "Ти знайшов " << food << " їжі!\n";
        }
        else {
            cout << "Ти нічого не знайшов.\n";
        }
    }

    void showStatus() {
        cout << "\n=============================\n";
        cout << "     День " << day << endl;
        cout << "=============================\n";
        cout << "Вода: " << water << endl;
        cout << "Їжа : " << food  << endl;
        player.showInventory();
    }

    void nextDay() {
        water -= 0.5;
        food -= 0.5;
        if (water <= 0) { cout << "\nТи помер від зневоднення...\n"; exit(0); }
        if (food <= 0) { cout << "\nТи помер від голоду...\n"; exit(0); }
        day++;
    }
};

int main() {
    srand(time(0));
    int diff;

    cout << "Виберіть складність:\n1 - Легко\n2 - Нормально\n3 - Складно\n> ";
    cin >> diff;
    cin.ignore();

    Game game(diff);

    while (true) {
        game.showStatus();
        cout << "\n1 - Вийти на вилазку\nEnter - наступний день\n> ";
        string input;
        getline(cin, input);

        if (input == "1") game.goOnExpedition();
        else game.nextDay();
    }
}
