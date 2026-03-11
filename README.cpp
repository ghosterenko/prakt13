#include <iostream>
#include <Windows.h>

struct Player {
public:
    long health = 500000;
    int damage = 12000;
    int specialDamage = 30000;
    int attackCooldown = 2;
    int specialCooldown = 5;
    int defense = 20;
    int dodgeChanse = 15;
    char name[64];
};

struct Bayum {
    long health = 900000000000;
    int resist = 44;
    int damage = 73843;
    int specialDamage = 150000;
    int attackCooldown = 5;
    int specialCooldown = 10;

public:
    void BossDefultAttack(Player player) {
        player.health -= this->damage;
    }
    void BossSpecialAttack(Player* players, const int COUNT) {
        for (int i = 0; i < COUNT; i++)
        {
            if (COUNT > 1) {
                int damage = this->specialDamage * (1 - 0.05) * (COUNT - 1);
                players[i].damage -= damage;
            }
        }
    }
    void PlayerAttack(Player player) {
        int damage = player.damage * (100 - this->resist) / 100;
        this->health -= damage;
    }
};
const int SIZE = 10;
int count = 1;
Player players[SIZE];
HANDLE playersThread[SIZE];



void Logic() {
    Bayum boss = Bayum();
    do {



    } while (boss.health != 0);
}

bool Menu() {
    char m;
    std::cout << "1. Начать игру" << std::endl;
    std::cout << "2. Выйти" << std::endl;
    std::cin >> m;
    std::cin.ignore();
    switch (m)
    {
    case '1':
    {
        char m2;
        std::cout << "1. Начать бой" << std::endl;
        std::cout << "2. Добавить игрока" << std::endl;
        std::cout << "3. Выйти из игры" << std::endl;
        std::cin >> m2;
        std::cin.ignore();
        switch (m2)
        {
        case '1':
        {
            Logic();
            break;
        }
        case '2': 
        {
            std::cout << "Задайте имя персонажа" << std::endl;
            char Name[64];
            std::cin.getline(Name, 64);
            players[count] = Player();
            for (int i = 0; i < 64; i++)
            {
                if(Name[i] != '\0')
                    players[count].name[i] = Name[i];
            }
            std::cin.ignore();
            playersThread[count] = CreateThread(NULL, 0, NULL, NULL, NULL, NULL);
            if (playersThread[count] == NULL) {
                std::cout << "Ошибка" << std::endl;
                break;
            }
            count++;
            std::cout << "Персонаж добавлен" << std::endl;
            break;

        }
        case '3':
            return false;
        default:
            break;
        }
        break;
    }
    case '2':
        return false;

    default:
        break;
    }
}

int main()
{
    bool f = true;
    while (f)
    {
        f = Menu();
    }
}
