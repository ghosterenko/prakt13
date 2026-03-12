#include <iostream>
#include <Windows.h>
#include <string>

CRITICAL_SECTION cs;
HANDLE events = CreateEventA(NULL, NULL, NULL, "event");
const int SIZE = 10;
int count = 1;
HANDLE playersThread[SIZE];
HANDLE bossThread;
std::string namePlayer[] = {
    "боб",
    "Алекс",
    "Стив",
    "Альф",
    "Бета"
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
    char name[64];

    Player() {
        playersThread[count] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FightPlayer, NULL, NULL, NULL);
        if (playersThread[count] == NULL) {
            std::cout << "Ошибка" << std::endl;
        }
    }
};

struct Bayum {
    long health = 900000000000;
    int resist = 44;
    int damage = 73843;
    int specialDamage = 150000;
    int attackCooldown = 5;
    int specialCooldown = 10;

public:
    Bayum() {
        bossThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FightBoss, NULL, 0, NULL);
        if (bossThread == NULL) {
            std::cout << "Ошибка" << std::endl;
        }
    }

    void BossDefultAttack(Player player) {
        player.health -= this->damage;
    }
    void BossSpecialAttack(Player* players, const int COUNT) {
        for (int i = 0; i < COUNT; i++)
        {
            if (COUNT > 1) {
                int damage = this->specialDamage * (1 - 0.05) * (COUNT - 1);
                players[i].health -= damage;
            }
        }
    }
    void PlayerAttack(Player player) {
        int damage = player.damage * (100 - this->resist) / 100;
        this->health -= damage;
    }
};

Player players[SIZE];

DWORD WINAPI FightPlayer(Bayum boss) {
    srand(time(NULL));
    do {
        EnterCriticalSection(&cs);

        LeaveCriticalSection(&cs);

    } while (boss.health != 0);

}
DWORD WINAPI FightBoss(Bayum boss) {
    srand(time(NULL));
    do {

    EnterCriticalSection(&cs);
    int hit = rand() % 5;
    switch (hit) {
    case 1:
    case 2:
    case 3:
    case 4:
    {
        
        int playerid = rand() % count;
        boss.BossDefultAttack(players[playerid]);
        std::cout << "Босс ударил игрока " << players[playerid].name << "- текущее здоровье " << players[playerid].health << std::endl;
        
        break;
    }
    case 5: 
    {
        boss.BossSpecialAttack(players, SIZE);
        std::cout << "Спец атака босса, всем игрокам нанесен урон - текущее хп игроков:" << std::endl;
        for (int i = 0; i < count; i++)
        {
            std::cout << players[i].name << " " << players[i].health << std::endl;
        }
        break;
    }
    LeaveCriticalSection(&cs);
    }
    } while (boss.health != 0);

}

void start() {
    Bayum boss = Bayum();
    while (true)
    {

    }
    
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
            }
            if (SIZE != 10) {
                std::cout << "Задайте имя персонажа" << std::endl;
                char Name[64];
                std::cin.getline(Name, 64);
                players[count] = Player();
                for (int i = 0; i < 64; i++)
                {
                    if (Name[i] != '\0')
                        players[count].name[i] = Name[i];
                }
                std::cin.ignore();
                
                count++;
                std::cout << "Персонаж добавлен" << std::endl;
                break;
            }
            else {
                std::cout << "Достигнут лимит по персонажам" << std::endl;
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
