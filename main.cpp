#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

#include "Throttle.h"

#include <mutex>

std::mutex lock;

Throttle op_throttle(12);

void task(int64_t i){

    {
        std::lock_guard<std::mutex> l(lock);
        std::cout << "started... The task id is " << i << std::endl;
    }

    if(!op_throttle.get_or_fail(3)) {
        op_throttle.get(3);
    }

    {
        std::lock_guard<std::mutex> l(lock);
        std::cout << "processing... The task id is " << i << ",current hold:" << op_throttle.get_count() << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1+rand()&7));

    {
        std::lock_guard<std::mutex> l(lock);
        std::cout << "finished... The task id is " << i << ",current hold:" << op_throttle.put(3) << std::endl;
    }
}

int main() {
    srand((int)time(0));
    std::thread t1(task,1);
    std::thread t2(task,2);
    std::thread t3(task,3);
    std::thread t4(task,4);
    std::thread t5(task,5);
    std::thread t6(task,6);
    std::thread t7(task,7);
    std::thread t8(task,8);
    std::thread t9(task,9);
    std::thread t10(task,10);
    std::thread t11(task,11);

    t11.join();t10.join();t9.join();t8.join();
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    return 0;
}
