//
// Created by fenghl on 2020/3/7.
//

#ifndef THROTTLE_THROTTLE_H
#define THROTTLE_THROTTLE_H

#include <atomic>
#include <mutex>
#include <condition_variable>

class Throttle {
    std::atomic<int64_t> count = {0}, max = {0};
    std::mutex lock;
    std::condition_variable conds;

public:
    Throttle(int64_t m = 0):max(m){};
    ~Throttle(){};

private:

    //返回Ture
    bool _should_wait(int64_t c) const {
        int m = max;
        int cur = count;
        return m &&   \
                ((c <= m && cur + c > m)        //满足该条件需要等待
                || (c >= m && cur > m));         //满足该条件将永远等待
    }

    bool _wait(int64_t c, std::unique_lock<std::mutex>& l);

public:

    int64_t get_count(){ return count;}

    /**
     * @param c 待取的数量
     * @return 返回已经被取的数量
     */
    int64_t take(int64_t c = 1);

    /**
     * @param c 待取的数量
     * @return 如果这个请求由于throttle被堵塞，返回ture, 否则返回false
     */
    bool get(int64_t c = 1);

    /**
     * @param c 待取的数量
     * @return 返回ture表示已经成功取到，返回fasle表示需要等待
     */
    bool get_or_fail(int64_t c = 1);

    /**
     * @param c 要返回的数量
     * @return 返回剩余请求已取的数量
     */
    int64_t put(int64_t c = 1);
};

#endif //THROTTLE_THROTTLE_H
