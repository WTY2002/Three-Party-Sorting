#include "Semi_honest_security.h"
#include <chrono>
#include <cstdio>
using namespace std::chrono;


int main() {

    auto start_time = high_resolution_clock::now(); // 记录函数开始时间

    // 初始化半诚实安全下的操作
    init_semi_honest_security();

    auto end_time = high_resolution_clock::now(); // 记录函数结束时间
    duration<double, milli> total_duration = end_time - start_time;
    printf("初始化半诚实安全下的操作所需时间是：%f 毫秒\n", total_duration.count());
    fflush(stdout);

    start_time = high_resolution_clock::now(); // 记录函数开始时间

    // 半诚实安全条件下获取排序
    semi_honest_sort();

    end_time = high_resolution_clock::now(); // 记录函数结束时间
    total_duration = end_time - start_time;
    printf("半诚实安全条件下获取排序的时间是：%f 毫秒\n", total_duration.count());
    fflush(stdout);

    // 校验置换的正确性
    bool flag = true;

    vector<int> ans(N), t(N);
    for (int i = 0; i < N; i++) {
        ans[i] = i + 1;
    }

    stable_sort(ans.begin(), ans.end(), [&](int a, int b) { return data[a - 1] < data[b - 1]; });

    for (int i = 0; i < N; i++) {
        t[ans[i] - 1] = i + 1;
    }
    for (int i = 0; i < N; i++) {
        if (p1.σ1[i] + p1.σ2[i] + p2.σ2[i] != t[i]) {
            flag = false;
            break;
        }
    }

    if (flag) {
        printf("半诚实安全下的置换是正确的!\n");
    } else {
        printf("半诚实安全下的置换是错误的!\n");
    }

    return 0;
}
