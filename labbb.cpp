#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

int coins;
int bob_coins = 0;
int tom_coins = 0;
int dead_man_coins = 0;
mutex mtx;

void coin_sharing(string name, int& thief_coins, int& companion_coins) {
    while (coins > 1) {  // Останавливаемся когда осталась 1 монета
        mtx.lock();

        if (coins > 1) {
            if (thief_coins <= companion_coins) {
                coins--;
                thief_coins++;
                cout << name << " vzyal monetu. U " << name << ": " << thief_coins
                    << ", u kompaniona: " << companion_coins
                    << ", ostalos: " << coins << endl;
            }
        }

        mtx.unlock();
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}

int main() {
    cout << "=== Lab 3: Razdel monet s pokojnikom ===" << endl;
    cout << "Vvedite kolichestvo monet: ";
    cin >> coins;

    int initial_coins = coins;

    cout << "Nachalnoe kolichestvo monet: " << coins << endl;
    cout << "Nachalo deleja..." << endl << endl;

    thread bob(coin_sharing, "Bob", ref(bob_coins), ref(tom_coins));
    thread tom(coin_sharing, "Tom", ref(tom_coins), ref(bob_coins));

    bob.join();
    tom.join();

    // Если осталась 1 монета (нечетное количество) - отдаем покойнику
    if (coins == 1) {
        dead_man_coins = 1;
        coins = 0;
        cout << "Poslednyaya moneta dostalas pokojniku!" << endl;
    }

    cout << endl << "=== Itogi deleja ===" << endl;
    cout << "U Boba monet: " << bob_coins << endl;
    cout << "U Toma monet: " << tom_coins << endl;
    cout << "U pokojnika monet: " << dead_man_coins << endl;
    cout << "Ostalos monet: " << coins << endl;
    cout << "Vsego raspredeleno: " << (bob_coins + tom_coins + dead_man_coins) << endl;
    cout << "Raznica mezhdu vorami: " << abs(bob_coins - tom_coins) << endl;

    // Проверка корректности
    if (bob_coins + tom_coins + dead_man_coins == initial_coins) {
        cout << "Raspredelenie vypolneno korrektno!" << endl;
    }

    // Анализ распределения
    cout << endl << "=== Analiz raspredeleniya ===" << endl;
    if (dead_man_coins > 0) {
        cout << "Pri nechetnom kolichestve (" << initial_coins << "):" << endl;
        cout << "- Vory poluchili porovnu: " << bob_coins << " monet" << endl;
        cout << "- Pokojnik poluchil: 1 monetu" << endl;
        cout << "- Itogo: " << bob_coins << " + " << tom_coins << " + 1 = " << initial_coins << endl;
    }
    else {
        cout << "Pri chetnom kolichestve (" << initial_coins << "):" << endl;
        cout << "- Vory poluchili porovnu: " << bob_coins << " monet" << endl;
        cout << "- Pokojnik ne poluchil monet" << endl;
        cout << "- Itogo: " << bob_coins << " + " << tom_coins << " = " << initial_coins << endl;
    }

    system("pause");
    return 0;
}