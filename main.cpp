#include "Semi_honest_security.h"

int main() {

    // 初始化半诚实安全下的操作
    init_semi_honest_security();

    // 半诚实安全条件下获取排序
    semi_honest_sort();

    // 半诚实安全条件下应用排序
    semi_honest_apply_sort();

    // 校验获取排序的正确性
    bool flag = true;

    vector<long long> ans(N), t(N);
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
        printf("经过校验得出结论：半诚实安全下<获取>置换是<正确>的!\n");
    } else {
        printf("经过校验得出结论：半诚实安全下<获取>置换是<错误>的!\n");
    }

    // 校验应用排序的正确性
    flag = true;
    for (int i = 0; i < N; i++) {
        ans[i] = 0;
        for (int j = 0; j < L; j++) {
            ans[i] = ans[i] * 2 + (p1.x1[i][j] + p1.x2[i][j] + p2.x2[i][j]);
        }
    }
    stable_sort(data.begin(), data.end());
    for (int i = 0; i < N; i++) {
        if (ans[i] != data[i]) {
            flag = false;
            break;
        }
    }

    if (flag) {
        printf("经过校验得出结论：半诚实安全下<应用>置换是<正确>的!\n");
    } else {
        printf("经过校验得出结论：半诚实安全下<应用>置换是<错误>的!\n");
    }

    return 0;
}
