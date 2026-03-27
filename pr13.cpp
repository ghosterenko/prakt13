#include <iostream>
#include <Windows.h>
#include <string>

std::string namePlayer[] = {
    "ББоб",
    "Алекс",
    "Стив",
    "Альф",
    "Бета",
    "Волк",
    "Лис",
    "Зебра",
    "Борис",
    "Геральд"
};

struct Player {
public:
    long health = 500000;
    int damage = 12000;
    int specialDamage = 30000;
    int attackCooldown = 2;
    int specialCooldown = 5;
    int defense = 20;
    int dodgeChanse = 15;
    std::string name;

};

struct Bayum {
public:
    long health = 900000000000;
    int resist = 44;
    int damage = 73843;
    int specialDamage = 150000;
    int attackCooldown = 5;
    int specialCooldown = 10;

    void PlayerAttack(Player player) {
        int damage = player.damage * (100 - resist) / 100;
        health -= damage;
    }
};
const int COUNTS = 10;
int count = 0;

HANDLE playersThread[COUNTS];
HANDLE bossThread;

Bayum boss = Bayum();
Player players[COUNTS];

VOID WINAPI FightPlayer() {
    srand(time(NULL));
    while (true) {
        Sleep(1000);

    } 
}
VOID WINAPI FightBoss() {
    srand(time(NULL));
    while(true) {
        Sleep(200);
        int hit = rand() % 5;
        switch (hit) {
        case 1:
        case 2:
        case 3:
        case 4:
        {

            int playerid = rand() % count;
            players[playerid].health -= boss.damage;
            std::cout << "Босс ударил игрока " << players[playerid].name << " - текущее здоровье " << players[playerid].health << std::endl;
            Sleep(1000);
            break;
        }
        case 5:
        {
            for (int i = 0; i < count; i++)
            {
                if (count > 1) {
                    int damage = boss.specialDamage * (1 - 0.05) * (count - 1);
                    players[i].health -= damage;
                }
            }
            std::cout << "Спец атака босса, всем игрокам нанесен урон - текущее хп игроков:" << std::endl;
            for (int i = 0; i < count; i++)
            {
                std::cout << players[i].name << " " << players[i].health << std::endl;
            }
            Sleep(1000);
            break;
        }

        }
        
    }

}

void start() {
    Bayum boss = Bayum(); 
    for (int i = 0; i < count; i++)
    {
        players[i] = Player();
        players[i].name = namePlayer[i];
        playersThread[count] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FightPlayer, NULL, 0, NULL);
        if (playersThread[count] == NULL) {
            std::cout << "Ошибка" << std::endl;
        }
    }
    bossThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FightBoss, NULL, 0, NULL);
    if (bossThread == NULL) {
        std::cout << "Ошибка" << std::endl;
    }

    WaitForSingleObject(bossThread, INFINITE);
    WaitForMultipleObjects(count, playersThread, TRUE, INFINITE);
    CloseHandle(bossThread);
    CloseHandle(playersThread);
}

int main()
{
    setlocale(LC_ALL, "ru");
    std::cout << "Введите количество игроков до 10" << std::endl;
    std::cin >> count;
    std::cin.ignore();
    if (count > 10) {
        std::cout << "Количество игроков не должно привышать 10" << std::endl;
    }
    else {
        std::cout << "Успешно" << std::endl;
        start();
    }
}
