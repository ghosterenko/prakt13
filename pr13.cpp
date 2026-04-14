#include <iostream>
#include <Windows.h>

std::string namePlayer[] = {
    "Боб",
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
    long health = 600000;
    int damage = 12000;
    int specialDamage = 30000;
    int attackCooldown = 2;
    int specialCooldown = 5;
    int defense = 20;
    int dodgeChance = 15;
    std::string name;
};

struct Bayum {
    long health = 500000;
    int resist = 44;
    int damage = 73843;
    int specialDamage = 150000;
    int attackCooldown = 5;
    int specialCooldown = 10;
};

const int COUNTS = 10;
int playerCount = 0;
bool bossAlive = true;
bool game = true;
int currentPlayerId = 0;

HANDLE playersThread[COUNTS];
HANDLE bossThread;
HANDLE bossEvent;
HANDLE playerTurnEvent;
HANDLE gameOverEvent;

Bayum boss;
Player players[COUNTS];

void FightPlayer() {
    int playerId = currentPlayerId;
    currentPlayerId++;
    int lastAttackTime = 0;
    int lastSpecialTime = 0;

    while (game && bossAlive) {

        WaitForSingleObject(playerTurnEvent, INFINITE);

        int currentTime = GetTickCount64() / 1000;

        if (currentTime - lastAttackTime < players[playerId].attackCooldown) {
            SetEvent(bossEvent);
            continue;
        }

        int damage = 0;
        damage = players[playerId].damage;
        lastAttackTime = currentTime;
        std::cout << players[playerId].name << " атакует " << damage << std::endl;

        long realDamage = damage * (100 - boss.resist) / 100;
        boss.health -= realDamage;

        if (boss.health <= 0) {
            bossAlive = false;
            std::cout << "Игроки победили" << std::endl;
            game = false;
            SetEvent(gameOverEvent);
            break;
        }

        SetEvent(bossEvent);
    }

}
void FightBoss() {
    int lastAttackTime = 0;
    int lastSpecialTime = 0;

    while (game && bossAlive) {

        WaitForSingleObject(bossEvent, INFINITE);

        bool aliveExists = false;
        for (int i = 0; i < playerCount; i++) {
            if (players[i].health > 0) {
                aliveExists = true;
                break;
            }
        }

        if (!aliveExists) {
            std::cout << "Босс победил" << std::endl;
            game = false;
            SetEvent(gameOverEvent);
            break;
        }

        int currentTime = GetTickCount64() / 1000;

        if (currentTime - lastAttackTime < boss.attackCooldown) {
            SetEvent(playerTurnEvent);
            continue;
        }

        bool Special = currentTime - lastSpecialTime >= boss.specialCooldown;

        if (Special) {
            lastSpecialTime = currentTime;
            lastAttackTime = currentTime;

            int aliveCount = 0;
            for (int i = 0; i < playerCount; i++) {
                if (players[i].health > 0) 
                    aliveCount++;
            }

            int totalDamage = boss.specialDamage;
            if (aliveCount > 1) {
                totalDamage = totalDamage * (100 - 5 * (playerCount - 1)) / 100;
            }

            std::cout << "Босс: спецатака - урон " << totalDamage << std::endl;

            for (int i = 0; i < playerCount; i++) {
                if (players[i].health > 0) {
                    if (rand() % 100 < players[i].dodgeChance) {
                        std::cout << players[i].name << " увернулся" << std::endl;
                        continue;
                    }

                    int damage = totalDamage * (100 - players[i].defense) / 100;
                    players[i].health -= damage;

                    std::cout << players[i].name << " -" << damage << " хп - осталось " << players[i].health << std::endl;

                    if (players[i].health <= 0) {
                        players[i].health = 0;
                        std::cout << players[i].name << " погиб" << std::endl;
                    }
                }
            }
        }
        else {
            lastAttackTime = currentTime;

            int alive[10];
            int aliveCount = 0;
            for (int i = 0; i < playerCount; i++) {
                if (players[i].health > 0) {
                    alive[aliveCount] = i;
                    aliveCount++;
                }
            }

            if (aliveCount > 0) {
                int target = alive[rand() % aliveCount];

                if (rand() % 100 < players[target].dodgeChance) {
                    std::cout << "Босс атакует " << players[target].name << " - промах" << std::endl;
                }
                else {
                    long damageTaken = boss.damage * (100 - players[target].defense) / 100;
                    players[target].health -= damageTaken;

                    std::cout << "Босс атакует " << players[target].name << " -" << damageTaken << " хп - осталось " << players[target].health << std::endl;

                    if (players[target].health <= 0) {
                        players[target].health = 0;
                        std::cout << players[target].name << " погиб!" << std::endl;
                    }
                }
            }
        }

        SetEvent(playerTurnEvent);
    }
}

void start() {
    bossEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    playerTurnEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    gameOverEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    for (int i = 0; i < playerCount; i++) {
        players[i] = Player();
        players[i].name = namePlayer[i];
        players[i].health = 500000;
        std::cout << "Игрок " << i + 1 << ": " << players[i].name << std::endl;
    }

    std::cout << "Бой начался" << std::endl;
    std::cout << "Босс: " << boss.health << " хп" << std::endl;

    bossThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FightBoss, NULL, 0, NULL);
    if (bossThread == NULL) {
        std::cout << "Ошибка" << std::endl;
        return;
    }

    currentPlayerId = 0;
    for (int i = 0; i < playerCount; i++) {
        playersThread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FightPlayer, NULL, 0, NULL);
        if (playersThread[i] == NULL) {
            std::cout << "Ошибка" << std::endl;
            return;
        }
    }

    SetEvent(playerTurnEvent);
    WaitForSingleObject(gameOverEvent, INFINITE);

    Sleep(1000);

    std::cout << "статистика" << std::endl;
    std::cout << "Здоровье босса: " << boss.health << std::endl;

    std::cout << "Топ-3 по урону:" << std::endl;

    int tempDamage[10];
    std::string tempNames[10];
    for (int i = 0; i < playerCount; i++) {
        tempNames[i] = players[i].name;
    }

    for (int i = 0; i < playerCount; i++) {
        int maxIndex = INT_MIN;
        int maxDamage = INT_MIN;

        for (int j = 0; j < playerCount; j++) {
            if (tempDamage[j] > maxDamage) {
                maxDamage = tempDamage[j];
                maxIndex = j;
            }
        }

        if (maxIndex != -1) {
            std::cout << i + 1 << ". " << tempNames[maxIndex] << " - " << tempDamage[maxIndex] << " урона" << std::endl;
            tempDamage[maxIndex] = INT_MIN;
        }
    }
    CloseHandle(bossThread);
    for (int i = 0; i < playerCount; i++) {
        CloseHandle(playersThread[i]);
    }
    CloseHandle(bossEvent);
    CloseHandle(playerTurnEvent);
    CloseHandle(gameOverEvent);
}

int main() {
    setlocale(LC_ALL, "ru");
    srand(time(NULL));

    std::cout << "Введите количество игроков до 10: ";
    std::cin >> playerCount;

    if (playerCount > 10) {
        std::cout << "Количество игроков не должно превышать 10" << std::endl;
    }
    if (playerCount < 1) {
        std::cout << "Количество игроков должно быть больше 0" << std::endl;
    }

    std::cout << "Успешно!" << std::endl;
    start();
}
