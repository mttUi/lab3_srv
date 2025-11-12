#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

int coins = 101;
int Bob_coins = 0;
int Tom_coins = 0;
std::mutex mtx;

void coin_sharing(const std::string& thief_name) {
    while (coins > 0) {
        
        mtx.lock();

        setlocale(LC_ALL, "Russian");
       
        if (coins > 0) {
            if (thief_name == "Bob") {
                if (Bob_coins <= Tom_coins) {
                    coins--;
                    Bob_coins++;
                    std::cout << "Bob взял монету. У Боба: " << Bob_coins
                        << ", у Тома: " << Tom_coins
                        << ", осталось: " << coins << std::endl;
                }
            }
            else { 
                if (Tom_coins <= Bob_coins) {
                    coins--;
                    Tom_coins++;
                    std::cout << "Tom взял монету. У Боба: " << Bob_coins
                        << ", у Тома: " << Tom_coins
                        << ", осталось: " << coins << std::endl;
                }
            }
        }

        
        mtx.unlock();

        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    std::cout << "=== Лабораторная работа №3: 100 и 1 монета ===" << std::endl;
    std::cout << "Начальное количество монет: " << coins << std::endl;
    std::cout << "Начинаем дележ..." << std::endl << std::endl;

    
    std::thread bob_thread(coin_sharing, "Bob");
    std::thread tom_thread(coin_sharing, "Tom");

    
    bob_thread.join();
    tom_thread.join();

    
    std::cout << std::endl << "=== Итоги дележа ===" << std::endl;
    std::cout << "У Боба монет: " << Bob_coins << std::endl;
    std::cout << "У Тома монет: " << Tom_coins << std::endl;
    std::cout << "Осталось монет: " << coins << std::endl;
    std::cout << "Всего распределено: " << (Bob_coins + Tom_coins) << std::endl;

    
    if (Bob_coins + Tom_coins == 101) {
        std::cout << "Распределение выполнено корректно!" << std::endl;
    }
    else {
        std::cout << "Ошибка в распределении монет!" << std::endl;
    }

    system("pause");
    return 0;
}