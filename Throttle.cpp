//
// Created by fenghl on 2020/3/7.
//

#include "Throttle.h"
#include <assert.h>
#include <iostream>

int64_t Throttle::take(int64_t c) {
    if(0 == max) return 0;
    {
        std::lock_guard<std::mutex> l(lock);
        count += c;
    }
    return count;
}

bool Throttle::get(int64_t c) {
    if(0 == max) {
        return false;
    }
    assert(c > 0);

    bool waited = false;
    {
        std::unique_lock<std::mutex> l(lock);
        if(_should_wait(c))
        {
            waited = true;
            conds.wait(l, [this, c](){return (!_should_wait(c));});
        }
        count += c;
    }
    return waited;
}

bool Throttle::get_or_fail(int64_t c) {
    if(0 == max) {
        return true;
    }
    assert(c > 0);

    std::lock_guard<std::mutex> l(lock);
    if(_should_wait(c)) {
        return false;
    }else {
        count += c;
    }

    return true;
}

int64_t Throttle::put(int64_t c) {
    if(0 == max) {
        return 0;
    }
    std::lock_guard<std::mutex> l(lock);
    if(0 != c)
    {
        assert(count >= c);
        count -= c;
        conds.notify_one();
    }
    return count;
}