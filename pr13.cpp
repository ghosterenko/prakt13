#include <iostream>
#include <Windows.h>
#include <string>

CRITICAL_SECTION cs;
const int COUNTS = 10;
int count = 0;
HANDLE playersThread[COUNTS];
DWORD playersThreadID[COUNTS];
HANDLE bossThread;
DWORD bossThreadId;

std::string namePlayer[] = {
    "боб",
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

VOID WINAPI FightPlayer();
VOID WINAPI FightBoss();

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
Bayum boss = Bayum();
Player players[COUNTS];

VOID WINAPI FightPlayer() {
    srand(time(NULL));
    do {
        Sleep(200);
        EnterCriticalSection(&cs);

        Sleep(1000);
        LeaveCriticalSection(&cs);

    } while (boss.health != 0);
    
}
VOID WINAPI FightBoss() {
    srand(time(NULL));
    do {
        Sleep(200);
        EnterCriticalSection(&cs);
        int hit = rand() % 5;
        switch (hit) {
        case 1:
        case 2:
        case 3:
        case 4:
        {

            int playerid = rand() % count;
            players[playerid].health -= boss.damage;
            std::cout << playerid << "Босс ударил игрока " << players[playerid].name << " - текущее здоровье " << players[playerid].health << std::endl;
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
        LeaveCriticalSection(&cs);
        }
    } while (boss.health != 0);

}

void start() {
    Bayum boss = Bayum();
    for (int i = 0; i < count; i++)
    {
        players[i] = Player();
        players[i].name = namePlayer[i];
        playersThread[count] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FightPlayer, NULL, 0, &playersThreadID[count]);
        if (playersThread[count] == NULL) {
            std::cout << "Ошибка" << std::endl;
        }
    }
    bossThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FightBoss, NULL, 0, &bossThreadId);
    if (bossThread == NULL) {
        std::cout << "Ошибка" << std::endl;
    }

    WaitForSingleObject(bossThread, INFINITE);
    WaitForMultipleObjects(count, playersThread, TRUE, INFINITE);
    CloseHandle(bossThread);
    CloseHandle(playersThread);
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
            start();
            break;
        }
        case '2':
        {
            std::cout << "Введите количество игроков до 10" << std::endl;
            std::cin >> count;
            std::cin.ignore();
            if (count > 10) {
                std::cout << "Количество игроков не должно привышать 10" << std::endl;
                break;
            }
            else {
                std::cout << "Успешно" << std::endl;
                break;
            }

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
    setlocale(LC_ALL, "ru");
    InitializeCriticalSection(&cs);
    bool f = true;
    while (f)
    {
        f = Menu();
    }
    DeleteCriticalSection(&cs);
}
