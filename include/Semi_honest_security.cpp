/**
* @author: WTY
* @date: 2024/100/19
* @description: Implement sorting under semi-honest and safe conditions
*/

#include "Semi_honest_security.h"

// 数据量的大小
int N = 0;

// 数据的比特长度
int L = 32;

// 原始数据
vector<long long> data;

// 服务器
Sever p1, p2, p3;

// 共享的数据备份
vector<vector<long long>> x1_backups, x2_backups, x3_backups;

/**
 * @Method initSever 初始化云服务器
 * @return void
 */
void initSever() {
    p1.x1.resize(N);
    p1.x2.resize(N);
    p1.π1.resize(N);
    p1.π2.resize(N);
    p1.π1_inv.resize(N);
    p1.π2_inv.resize(N);
    p1.ρ1.resize(N);
    p1.ρ2.resize(N);
    p1.σ1.resize(N);
    p1.σ2.resize(N);
    p2.x1.resize(N);
    p2.x2.resize(N);
    p2.π1.resize(N);
    p2.π2.resize(N);
    p2.π1_inv.resize(N);
    p2.π2_inv.resize(N);
    p2.ρ1.resize(N);
    p2.ρ2.resize(N);
    p2.σ1.resize(N);
    p2.σ2.resize(N);
    p3.x1.resize(N);
    p3.x2.resize(N);
    p3.π1.resize(N);
    p3.π2.resize(N);
    p3.π1_inv.resize(N);
    p3.π2_inv.resize(N);
    p3.ρ1.resize(N);
    p3.ρ2.resize(N);
    p3.σ1.resize(N);
    p3.σ2.resize(N);
}

/**
 * @Method readDataFromFile 从文件中读取数据
 * @param filename 文件名
 * @return void
 */
void readDataFromFile(const char* filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "无法打开文件: " << filename << endl;
        return; // 退出函数
    }

    string line;

    // 读取第一行，获取数据量大小 N
    if (getline(file, line)) {
        istringstream(line) >> N; // 直接读取为 long long N

        // 读取第二行 N 个数字
        if (getline(file, line)) {
            istringstream iss(line);
            long long number; // 修改为 long long
            while (iss >> number) {
                data.push_back(number); // 将无符号数存入 vector
            }
        }
    }

    file.close();
}

/**
 * @Method decimalToBinary 将十进制数转换为二进制数
 * @param num 要转换的十进制数
 * @return vector<long long> 二进制数
 */
vector<long long> decimalToBinary(long long num) {
    vector<long long> bits;

    // 检查每一位
    for (int i = L - 1; i >= 0; --i) {
        bits.push_back((num >> i) & 1); // 将当前位添加到向量中
    }

    return bits;
}

/**
 * @Method bitDecomposition 执行位分解
 * @param index 索引
 * @param a, b, c 分解的比特
 * return void
 */
void bitDecomposition(int index, long long a, long long b, long long c) {
    // 将a，b，c转换为二进制
    vector<long long> a_bits = decimalToBinary(a);
    vector<long long> b_bits = decimalToBinary(b);
    vector<long long> c_bits = decimalToBinary(c);

    p1.x1[index].resize(L);
    p1.x2[index].resize(L);
    p2.x1[index].resize(L);
    p2.x2[index].resize(L);
    p3.x1[index].resize(L);
    p3.x2[index].resize(L);

    long long a1, a2, a3, b1, b2, b3, c1, c2, c3;
    long long x1, x2, x3;

    // 进位标识初始化
    long long carry = 0;

    // 逐位位分解
    for (int i = L - 1; i >= 0; --i) {
        a1 = generateRandomInteger(0, 100);
        a2 = generateRandomInteger(0, 100);
        a3 = a_bits[i] - a1 - a2;
        b1 = generateRandomInteger(0, 100);
        b2 = generateRandomInteger(0, 100);
        b3 = b_bits[i] - b1 - b2;
        c1 = generateRandomInteger(0, 100);
        c2 = generateRandomInteger(0, 100);
        c3 = c_bits[i] - c1 - c2;
        // 计算x1, x2, x3
        x1 = (a1 + b1 + c1 + carry) % MOD;
        x2 = (a2 + b2 + c2) % MOD;
        x3 = (a3 + b3 + c3) % MOD;

        if (x1 + x2 + x3 >= 4) {
            carry = 2;
            x1 = x1 - 4;
        } else if (x1 + x2 + x3 >= 2) {
            carry = 1;
            x1 = x1 - 2;
        } else {
            carry = 0;
        }

        // 更新服务器数据
        p1.x1[index][i] = x1;
        p1.x2[index][i] = x2;
        p2.x1[index][i] = x2;
        p2.x2[index][i] = x3;
        p3.x1[index][i] = x3;
        p3.x2[index][i] = x1;
    }
}

/**
 * @Method generateRandomInteger 生成一个指定范围内的随机整数
 * @param min 最小值
 * @param max 最大值
 * @return long long 随机整数
 */
long long generateRandomInteger(long long min, long long max) {
    random_device rd; // 获取随机数种子
    mt19937 gen(rd()); // 初始化随机数生成器
    uniform_int_distribution<long long> dis(min, max); // 定义均匀分布
    return dis(gen); // 生成并返回随机整数
}

/**
 * @Method Generateshuffle 生成一个随机置换
 * @return vector<long long> 随机置换
 */
vector<long long> Generateshuffle() {
    // 创建一个包含 1 到 N 的序列
    vector<long long> sequence(N);
    for (int i = 0; i < N; ++i) {
        sequence[i] = i + 1; // 填充序列
    }

    // 随机打乱序列
    random_device rd; // 获取随机数种子
    mt19937 g(rd()); // 使用 Mersenne Twister 作为随机数引擎
    shuffle(sequence.begin(), sequence.end(), g);

    return sequence; // 返回打乱后的序列
}

/**
 * @Method generateSharedShuffle 生成共享的随机置换
 * @return void
 */
void generateSharedShuffle() {
    p1.π1 = Generateshuffle();
    p1.π2 = Generateshuffle();
    p2.π1 = p1.π2;
    p2.π2 = Generateshuffle();
    p3.π1 = p2.π2;
    p3.π2 = p1.π1;
}

/**
 * @Method singleShuffle 对数据单次置换
 * @param x 数据
 * @param π 置换
 * @param index 索引
 * @return void
 */
void singleShuffle(vector<vector<long long>> &x, vector<long long> &π, int index) {
    vector<long long> tmp(N);
    for (int i = 0; i < N; ++i) {
        tmp[π[i] - 1] = x[i][index];
    }
    for (int i = 0; i < N; ++i) {
        x[i][index] = tmp[i]; // 将置换后的结果存入 x
    }
}

/**
 * @Method singleShuffle 对数据单次置换
 * @param ρ 置换ρ
 * @param π 置换π
 * @return void
 */
void singleShuffle(vector<long long> &ρ, vector<long long> &π) {
    vector<long long> tmp(N);
    for (int i = 0; i < N; ++i) {
        tmp[π[i] - 1] = ρ[i];
    }
    for (int i = 0; i < N; ++i) {
        ρ[i] = tmp[i]; // 将置换后的结果存入 ρ
    }
}

/**
 * @Method shufflingProtocol 完整的shuffling协议
 * @param index 索引
 * @return void
 */
void shufflingProtocol(int index) {
    // 服务器p1和p3联合做置换，置换后的数据发给p2
    singleShuffle(p1.x1, p1.π1, index);
    singleShuffle(p1.x2, p1.π1, index);
    singleShuffle(p3.x1, p3.π2, index);
    singleShuffle(p3.x2, p3.π2, index);
    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    long long r1 = generateRandomInteger(0, 100);
    long long r2 = generateRandomInteger(0, 100);
    long long r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        p1.x1[i][index] = (p1.x1[i][index] + r1) % MOD;
        p1.x2[i][index] = (p1.x2[i][index] + r2) % MOD;
        p3.x1[i][index] = (p3.x1[i][index] + r3) % MOD;
        p3.x2[i][index] = (p3.x2[i][index] + r1) % MOD;
    }
    // 将置换后的新数据发送给p2
    p2.x1 = p1.x2;
    p2.x2 = p3.x1;

    // 服务器p1和p2联合做置换，置换后的数据发给p3
    singleShuffle(p1.x1, p1.π2, index);
    singleShuffle(p1.x2, p1.π2, index);
    singleShuffle(p2.x1, p2.π1, index);
    singleShuffle(p2.x2, p2.π1, index);
    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    r1 = generateRandomInteger(0, 100);
    r2 = generateRandomInteger(0, 100);
    r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        p1.x1[i][index] = (p1.x1[i][index] + r1) % MOD;
        p1.x2[i][index] = (p1.x2[i][index] + r2) % MOD;
        p2.x1[i][index] = (p2.x1[i][index] + r2) % MOD;
        p2.x2[i][index] = (p2.x2[i][index] + r3) % MOD;
    }
    // 将置换后的新数据发送给p3
    p3.x1 = p2.x2;
    p3.x2 = p1.x1;

    // 服务器p2和p3联合做置换，置换后的数据发给p1
    singleShuffle(p2.x1, p2.π2, index);
    singleShuffle(p2.x2, p2.π2, index);
    singleShuffle(p3.x1, p3.π1, index);
    singleShuffle(p3.x2, p3.π1, index);
    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    r1 = generateRandomInteger(0, 100);
    r2 = generateRandomInteger(0, 100);
    r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        p2.x1[i][index] = (p2.x1[i][index] + r2) % MOD;
        p2.x2[i][index] = (p2.x2[i][index] + r3) % MOD;
        p3.x1[i][index] = (p3.x1[i][index] + r3) % MOD;
        p3.x2[i][index] = (p3.x2[i][index] + r1) % MOD;
    }
    // 将置换后的新数据发送给p1
    p1.x1 = p3.x2;
    p1.x2 = p2.x1;
}

/**
 * @Method shufflingProtocol 完整的shuffling协议，对ρ进行洗牌
 * @return void
 */
void shufflingProtocol_ρ() {
    // 服务器p1和p3联合做置换，置换后的数据发给p2
    singleShuffle(p1.ρ1, p1.π1);
    singleShuffle(p1.ρ2, p1.π1);
    singleShuffle(p3.ρ1, p3.π2);
    singleShuffle(p3.ρ2, p3.π2);

    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    long long r1 = generateRandomInteger(0, 100);
    long long r2 = generateRandomInteger(0, 100);
    long long r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        p1.ρ1[i] = (p1.ρ1[i] + r1) % MOD;
        p1.ρ2[i] = (p1.ρ2[i] + r2) % MOD;
        p3.ρ1[i] = (p3.ρ1[i] + r3) % MOD;
        p3.ρ2[i] = (p3.ρ2[i] + r1) % MOD;
    }
    // 将置换后的新数据发送给p2
    p2.ρ1 = p1.ρ2;
    p2.ρ2 = p3.ρ1;

    // 服务器p1和p2联合做置换，置换后的数据发给p3
    singleShuffle(p1.ρ1, p1.π2);
    singleShuffle(p1.ρ2, p1.π2);
    singleShuffle(p2.ρ1, p2.π1);
    singleShuffle(p2.ρ2, p2.π1);

    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    r1 = generateRandomInteger(0, 100);
    r2 = generateRandomInteger(0, 100);
    r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        p1.ρ1[i] = (p1.ρ1[i] + r1) % MOD;
        p1.ρ2[i] = (p1.ρ2[i] + r2) % MOD;
        p2.ρ1[i] = (p2.ρ1[i] + r2) % MOD;
        p2.ρ2[i] = (p2.ρ2[i] + r3) % MOD;
    }
    // 将置换后的新数据发送给p3
    p3.ρ1 = p2.ρ2;
    p3.ρ2 = p1.ρ1;

    // 服务器p2和p3联合做置换，置换后的数据发给p1
    singleShuffle(p2.ρ1, p2.π2);
    singleShuffle(p2.ρ2, p2.π2);
    singleShuffle(p3.ρ1, p3.π1);
    singleShuffle(p3.ρ2, p3.π1);

    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    r1 = generateRandomInteger(0, 100);
    r2 = generateRandomInteger(0, 100);
    r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        p2.ρ1[i] = (p2.ρ1[i] + r2) % MOD;
        p2.ρ2[i] = (p2.ρ2[i] + r3) % MOD;
        p3.ρ1[i] = (p3.ρ1[i] + r3) % MOD;
        p3.ρ2[i] = (p3.ρ2[i] + r1) % MOD;
    }
    // 将置换后的新数据发送给p1
    p1.ρ1 = p3.ρ2;
    p1.ρ2 = p2.ρ1;
}

/**
 * @Method shufflingProtocol 完整的shuffling协议，对σ进行洗牌
 * @return void
 */
void shufflingProtocol_σ() {
    // 服务器p1和p3联合做置换，置换后的数据发给p2
    singleShuffle(p1.σ1, p1.π1);
    singleShuffle(p1.σ2, p1.π1);
    singleShuffle(p3.σ1, p3.π2);
    singleShuffle(p3.σ2, p3.π2);
    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    long long r1 = generateRandomInteger(0, 100);
    long long r2 = generateRandomInteger(0, 100);
    long long r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        p1.σ1[i] = (p1.σ1[i] + r1) % MOD;
        p1.σ2[i] = (p1.σ2[i] + r2) % MOD;
        p3.σ1[i] = (p3.σ1[i] + r3) % MOD;
        p3.σ2[i] = (p3.σ2[i] + r1) % MOD;
    }
    // 将置换后的新数据发送给p2
    p2.σ1 = p1.σ2;
    p2.σ2 = p3.σ1;

    // 服务器p1和p2联合做置换，置换后的数据发给p3
    singleShuffle(p1.σ1, p1.π2);
    singleShuffle(p1.σ2, p1.π2);
    singleShuffle(p2.σ1, p2.π1);
    singleShuffle(p2.σ2, p2.π1);
    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    r1 = generateRandomInteger(0, 100);
    r2 = generateRandomInteger(0, 100);
    r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        p1.σ1[i] = (p1.σ1[i] + r1) % MOD;
        p1.σ2[i] = (p1.σ2[i] + r2) % MOD;
        p2.σ1[i] = (p2.σ1[i] + r2) % MOD;
        p2.σ2[i] = (p2.σ2[i] + r3) % MOD;
    }
    // 将置换后的新数据发送给p3
    p3.σ1 = p2.σ2;
    p3.σ2 = p1.σ1;

    // 服务器p2和p3联合做置换，置换后的数据发给p1
    singleShuffle(p2.σ1, p2.π2);
    singleShuffle(p2.σ2, p2.π2);
    singleShuffle(p3.σ1, p3.π1);
    singleShuffle(p3.σ2, p3.π1);
    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    r1 = generateRandomInteger(0, 100);
    r2 = generateRandomInteger(0, 100);
    r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        p2.σ1[i] = (p2.σ1[i] + r2) % MOD;
        p2.σ2[i] = (p2.σ2[i] + r3) % MOD;
        p3.σ1[i] = (p3.σ1[i] + r3) % MOD;
        p3.σ2[i] = (p3.σ2[i] + r1) % MOD;
    }
    // 将置换后的新数据发送给p1
    p1.σ1 = p3.σ2;
    p1.σ2 = p2.σ1;
}

/**
 * @Method generateSharedInverseShuffle 生成共享的逆置换
 * @return void
 */
void generateSharedInverseShuffle() {
    // 生成共享的逆置换
    for (int i = 0; i < N; ++i) {
        p1.π1_inv[p1.π1[i] - 1] = i + 1;
        p1.π2_inv[p1.π2[i] - 1] = i + 1;
        p2.π1_inv[p2.π1[i] - 1] = i + 1;
        p2.π2_inv[p2.π2[i] - 1] = i + 1;
        p3.π1_inv[p3.π1[i] - 1] = i + 1;
        p3.π2_inv[p3.π2[i] - 1] = i + 1;
    }
}

/**
 * @Method unShufflingProtocol 完整的unShuffling协议
 * @param index 索引
 * @return void
 */
void unShufflingProtocol(int index) {
    // 生成对应的逆置换
    generateSharedInverseShuffle();

    // 服务器p2和p3联合做置换，置换后的数据发给p1
    singleShuffle(p2.x1, p2.π2_inv, index);
    singleShuffle(p2.x2, p2.π2_inv, index);
    singleShuffle(p3.x1, p3.π1_inv, index);
    singleShuffle(p3.x2, p3.π1_inv, index);
    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    long long r1 = generateRandomInteger(0, 100);
    long long r2 = generateRandomInteger(0, 100);
    long long r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        p2.x1[i][index] = (p2.x1[i][index] + r2) % MOD;
        p2.x2[i][index] = (p2.x2[i][index] + r3) % MOD;
        p3.x1[i][index] = (p3.x1[i][index] + r3) % MOD;
        p3.x2[i][index] = (p3.x2[i][index] + r1) % MOD;
    }
    // 将置换后的新数据发送给p1
    p1.x1 = p3.x2;
    p1.x2 = p2.x1;

    // 服务器p1和p2联合做置换，置换后的数据发给p3
    singleShuffle(p1.x1, p1.π2_inv, index);
    singleShuffle(p1.x2, p1.π2_inv, index);
    singleShuffle(p2.x1, p2.π1_inv, index);
    singleShuffle(p2.x2, p2.π1_inv, index);
    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    r1 = generateRandomInteger(0, 100);
    r2 = generateRandomInteger(0, 100);
    r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        p1.x1[i][index] = (p1.x1[i][index] + r1) % MOD;
        p1.x2[i][index] = (p1.x2[i][index] + r2) % MOD;
        p2.x1[i][index] = (p2.x1[i][index] + r2) % MOD;
        p2.x2[i][index] = (p2.x2[i][index] + r3) % MOD;
    }
    // 将置换后的新数据发送给p3
    p3.x1 = p2.x2;
    p3.x2 = p1.x1;

    // 服务器p1和p3联合做置换，置换后的数据发给p2
    singleShuffle(p1.x1, p1.π1_inv, index);
    singleShuffle(p1.x2, p1.π1_inv, index);
    singleShuffle(p3.x1, p3.π2_inv, index);
    singleShuffle(p3.x2, p3.π2_inv, index);
    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    r1 = generateRandomInteger(0, 100);
    r2 = generateRandomInteger(0, 100);
    r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        p1.x1[i][index] = (p1.x1[i][index] + r1) % MOD;
        p1.x2[i][index] = (p1.x2[i][index] + r2) % MOD;
        p3.x1[i][index] = (p3.x1[i][index] + r3) % MOD;
        p3.x2[i][index] = (p3.x2[i][index] + r1) % MOD;
    }
    // 将置换后的新数据发送给p2
    p2.x1 = p1.x2;
    p2.x2 = p3.x1;
}

/**
 * @Method unShufflingProtocol 完整的unShuffling协议，对ρ进行逆洗牌
 * @return void
 */
void unShufflingProtocol_ρ() {
    // 生成对应的逆置换
    generateSharedInverseShuffle();

    // 服务器p2和p3联合做置换，置换后的数据发给p1
    singleShuffle(p2.ρ1, p2.π2_inv);
    singleShuffle(p2.ρ2, p2.π2_inv);
    singleShuffle(p3.ρ1, p3.π1_inv);
    singleShuffle(p3.ρ2, p3.π1_inv);
    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    long long r1 = generateRandomInteger(0, 100);
    long long r2 = generateRandomInteger(0, 100);
    long long r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        p2.ρ1[i] = (p2.ρ1[i] + r2) % MOD;
        p2.ρ2[i] = (p2.ρ2[i] + r3) % MOD;
        p3.ρ1[i] = (p3.ρ1[i] + r3) % MOD;
        p3.ρ2[i] = (p3.ρ2[i] + r1) % MOD;
    }
    // 将置换后的新数据发送给p1
    p1.ρ1 = p3.ρ2;
    p1.ρ2 = p2.ρ1;

    // 服务器p1和p2联合做置换，置换后的数据发给p3
    singleShuffle(p1.ρ1, p1.π2_inv);
    singleShuffle(p1.ρ2, p1.π2_inv);
    singleShuffle(p2.ρ1, p2.π1_inv);
    singleShuffle(p2.ρ2, p2.π1_inv);
    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    r1 = generateRandomInteger(0, 100);
    r2 = generateRandomInteger(0, 100);
    r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        p1.ρ1[i] = (p1.ρ1[i] + r1) % MOD;
        p1.ρ2[i] = (p1.ρ2[i] + r2) % MOD;
        p2.ρ1[i] = (p2.ρ1[i] + r2) % MOD;
        p2.ρ2[i] = (p2.ρ2[i] + r3) % MOD;
    }
    // 将置换后的新数据发送给p3
    p3.ρ1 = p2.ρ2;
    p3.ρ2 = p1.ρ1;

    // 服务器p1和p3联合做置换，置换后的数据发给p2
    singleShuffle(p1.ρ1, p1.π1_inv);
    singleShuffle(p1.ρ2, p1.π1_inv);
    singleShuffle(p3.ρ1, p3.π2_inv);
    singleShuffle(p3.ρ2, p3.π2_inv);
    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    r1 = generateRandomInteger(0, 100);
    r2 = generateRandomInteger(0, 100);
    r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        p1.ρ1[i] = (p1.ρ1[i] + r1) % MOD;
        p1.ρ2[i] = (p1.ρ2[i] + r2) % MOD;
        p3.ρ1[i] = (p3.ρ1[i] + r3) % MOD;
        p3.ρ2[i] = (p3.ρ2[i] + r1) % MOD;
    }
    // 将置换后的新数据发送给p2
    p2.ρ1 = p1.ρ2;
    p2.ρ2 = p3.ρ1;
}

/**
 * @Method genBitPerm 生成共享的单比特置换
 * @param index 索引
 * @return void
 */
void genBitPerm(int index) {
    vector<long long> f0_1(N), f0_2(N), f0_3(N), f1_1(N), f1_2(N), f1_3(N), s0_1(N), s0_2(N), s0_3(N), s1_1(N), s1_2(N), s1_3(N), ρ_1(N), ρ_2(N), ρ_3(N);

    // 计算共享的f0和f1
    for (int i = 0; i < N; ++i) {
        f0_1[i] = 1 - p1.x1[i][index];
        f0_2[i] = -p1.x2[i][index];
        f0_3[i] = -p2.x2[i][index];
        f1_1[i] = p1.x1[i][index];
        f1_2[i] = p1.x2[i][index];
        f1_3[i] = p2.x2[i][index];
    }

    // 计算共享的s0
    long long sum1 = 0, sum2 = 0, sum3 = 0;
    for (int i = 0; i < N; ++i) {
        // sum1 += f0_1[i];
        sum1 = (sum1 + f0_1[i]) % MOD; // 防止溢出
        s0_1[i] = sum1;
        // sum2 += f0_2[i];
        sum2 = (sum2 + f0_2[i]) % MOD; // 防止溢出
        s0_2[i] = sum2;
        // sum3 += f0_3[i];
        sum3 = (sum3 + f0_3[i]) % MOD; // 防止溢出
        s0_3[i] = sum3;
    }

    // 计算共享的s1
    for (int i = 0; i < N; ++i) {
        // sum1 += f1_1[i];
        sum1 = (sum1 + f1_1[i]) % MOD; // 防止溢出
        s1_1[i] = sum1;
        // sum2 += f1_2[i];
        sum2 = (sum2 + f1_2[i]) % MOD; // 防止溢出
        s1_2[i] = sum2;
        // sum3 += f1_3[i];
        sum3 = (sum3 + f1_3[i]) % MOD; // 防止溢出
        s1_3[i] = sum3;
    }

    long long k = 0;

    // 计算共享的置换ρ
    for (int i = 0; i < N; ++i) {
        k = (p1.x1[i][index] + p1.x2[i][index] + p2.x2[i][index]) % MOD; // 计算k的指定位
        ρ_1[i] = (((1 - k) * s0_1[i]) % MOD + (k * s1_1[i]) % MOD) % MOD;
        ρ_2[i] = (((1 - k) * s0_2[i]) % MOD + (k * s1_2[i]) % MOD) % MOD;
        ρ_3[i] = (((1 - k) * s0_3[i]) % MOD + (k * s1_3[i]) % MOD) % MOD;
    }

    // 更新数据
    p1.ρ1 = ρ_1;
    p1.ρ2 = ρ_2;
    p2.ρ1 = ρ_2;
    p2.ρ2 = ρ_3;
    p3.ρ1 = ρ_3;
    p3.ρ2 = ρ_1;
}

/**
 * @Method applyPerm 应用共享的单比特置换σ
 * @param index 索引
 * @return void
 */
void applyPerm(int index) {
    // 用组合的置换σ更新ρ的信息
    p1.ρ1 = p1.σ1;
    p1.ρ2 = p1.σ2;
    p2.ρ1 = p2.σ1;
    p2.ρ2 = p2.σ2;
    p3.ρ1 = p3.σ1;
    p3.ρ2 = p3.σ2;

    // 生成共享的随机置换π
    generateSharedShuffle();

    // 对共享的ρ进行洗牌
    shufflingProtocol_ρ();

    // 对k的指定位进行洗牌
    shufflingProtocol(index);

    // 揭示洗牌后的ρ
    vector<long long> ρ_vector(N);
    for (int i = 0; i < N; ++i) {
        ρ_vector[i] = (p1.ρ1[i] + p1.ρ2[i] + p2.ρ2[i]) % MOD;
    }

    // 使用洗牌后的ρ对k的指定位进行置换
    singleShuffle(p1.x1, ρ_vector, index);
    singleShuffle(p1.x2, ρ_vector, index);
    singleShuffle(p2.x1, ρ_vector, index);
    singleShuffle(p2.x2, ρ_vector, index);
    singleShuffle(p3.x1, ρ_vector, index);
    singleShuffle(p3.x2, ρ_vector, index);
}

/**
 * @Method composePerm 组合两个置换
 * @return void
 */
void composePerm() {
    // ρ的信息不能变，保存ρ的信息
    vector<long long> ρ1_vector(p1.ρ1), ρ2_vector(p1.ρ2), ρ3_vector(p2.ρ2);

    // 生成共享的随机置换π
    generateSharedShuffle();

    // 对共享的σ进行洗牌
    shufflingProtocol_σ();

    // 揭示洗牌后的σ
    vector<long long> σ_vector(N);
    for (int i = 0; i < N; ++i) {
        σ_vector[i] = (p1.σ1[i] + p1.σ2[i] + p2.σ2[i]) % MOD;
    }

    // 求洗牌后的σ的逆置换
    vector<long long> σ_inv_vector(N);
    for (int i = 0; i < N; ++i) {
        σ_inv_vector[σ_vector[i] - 1] = i + 1;
    }

    // 利用σ_inv_vector对ρ进行置换
    singleShuffle(p1.ρ1, σ_inv_vector);
    singleShuffle(p1.ρ2, σ_inv_vector);
    singleShuffle(p2.ρ1, σ_inv_vector);
    singleShuffle(p2.ρ2, σ_inv_vector);
    singleShuffle(p3.ρ1, σ_inv_vector);
    singleShuffle(p3.ρ2, σ_inv_vector);

    // 对ρ进行逆置换
    unShufflingProtocol_ρ();

    // 更新数据，ρ用于存储当前位的置换，σ用于存储组合的置换
    p1.σ1 = p1.ρ1;
    p1.σ2 = p1.ρ2;
    p2.σ1 = p2.ρ1;
    p2.σ2 = p2.ρ2;
    p3.σ1 = p3.ρ1;
    p3.σ2 = p3.ρ2;

    // 恢复ρ的信息
    p1.ρ1 = ρ1_vector;
    p1.ρ2 = ρ2_vector;
    p2.ρ1 = ρ2_vector;
    p2.ρ2 = ρ3_vector;
    p3.ρ1 = ρ3_vector;
    p3.ρ2 = ρ1_vector;
}

/**
 * @Method genPerm 生成共享的稳定置换
 * @return void
 */
void genPerm() {
    // 生成最低位的单比特置换
    genBitPerm(L - 1);

    // 更新σ置换
    p1.σ1 = p1.ρ1;
    p1.σ2 = p1.ρ2;
    p2.σ1 = p2.ρ1;
    p2.σ2 = p2.ρ2;
    p3.σ1 = p3.ρ1;
    p3.σ2 = p3.ρ2;

    for (int i = L - 2; i >= 0; --i) {
        // 在当前位应用单比特置换
        applyPerm(i);

        // 获取当前位的置换
        genBitPerm(i);

        // 组合置换
        composePerm();
    }
    // 最终的置换是σ
}

/**
 * @Method: init_semi_honest_security 初始化半诚实安全下的操作
 * @return void
 */
void init_semi_honest_security() {
    // 读取数据
    char* filename = "C:/Users/uu/Desktop/data.txt"; // 文件名
    readDataFromFile(filename);

    // 初始化云服务器p1,p2,p3
    initSever();

    auto start_time = high_resolution_clock::now(); // 记录函数开始时间

    // 将数据拆分为三份，并求数据的3方秘密共享
    for (int i = 0; i < N; i++) {
        long long a = generateRandomInteger(0, data[i] >> 1); // 生成随机数a
        long long b = generateRandomInteger(0, a >> 1); // 生成随机数b
        long long c = data[i] - a - b; // 计算c
        // 对每个数据进行比特分解
        bitDecomposition(i, a, b, c);
    }

    auto end_time = high_resolution_clock::now(); // 记录函数结束时间
    duration<double, milli> total_duration = end_time - start_time;
    printf("比特分解所需时间是：%f 毫秒\n", total_duration.count());
    fflush(stdout);

    // 拷贝原始共享的数据
    x1_backups.resize(N);
    x2_backups.resize(N);
    x3_backups.resize(N);
    x1_backups = p1.x1;
    x2_backups = p1.x2;
    x3_backups = p2.x2;
}

/**
 * @Method: semi_honest_sort 半诚实安全条件下进行排序
 * @return void
 */
void semi_honest_sort() {
    auto start_time = high_resolution_clock::now(); // 记录函数开始时间

    // 获取排序
    genPerm();

    auto end_time = high_resolution_clock::now(); // 记录函数结束时间
    duration<double, milli> total_duration = end_time - start_time;
    printf("获取排序所需时间是：%f 毫秒\n", total_duration.count());
    fflush(stdout);

    cout << "获取的排序结果为：" << endl;
    // 揭示排序结果(测试打印排序的结果)
    for (int i = 0; i < N; i++) {
        cout << p1.σ1[i] + p1.σ2[i] + p2.σ2[i] << " ";
    }
    cout << endl;
}

/**
 * @Method singleShuffle 对数据的整体进行单次置换
 * @param x 数据
 * @param π 置换
 * @return void
 */
void singleShuffle(vector<vector<long long>> &x, vector<long long> &π) {
    vector<vector<long long>> temp(N, vector<long long>(L)); // 临时数组，用于存储置换后的数据
    for (int i = 0; i < N; i++) {
        temp[π[i] - 1] = x[i];
    }
    for (int i = 0; i < N; i++) {
        x[i] = temp[i];
    }
}

/**
 * @Method shufflingProtocol 完整的shuffling协议，对整体进行洗牌
 * @return void
 */
void shufflingProtocol() {
    // 服务器p1和p3联合做置换，置换后的数据发给p2
    singleShuffle(p1.x1, p1.π1);
    singleShuffle(p1.x2, p1.π1);
    singleShuffle(p3.x1, p3.π2);
    singleShuffle(p3.x2, p3.π2);
    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    long long r1 = generateRandomInteger(0, 100);
    long long r2 = generateRandomInteger(0, 100);
    long long r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < L; ++j) {
            p1.x1[i][j] = (p1.x1[i][j] + r1) % MOD;
            p1.x2[i][j] = (p1.x2[i][j] + r2) % MOD;
            p3.x1[i][j] = (p3.x1[i][j] + r3) % MOD;
            p3.x2[i][j] = (p3.x2[i][j] + r1) % MOD;
        }
    }
    // 将置换后的新数据发送给p2
    p2.x1 = p1.x2;
    p2.x2 = p3.x1;

    // 服务器p1和p2联合做置换，置换后的数据发给p3
    singleShuffle(p1.x1, p1.π2);
    singleShuffle(p1.x2, p1.π2);
    singleShuffle(p2.x1, p2.π1);
    singleShuffle(p2.x2, p2.π1);
    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    r1 = generateRandomInteger(0, 100);
    r2 = generateRandomInteger(0, 100);
    r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < L; ++j) {
            p1.x1[i][j] = (p1.x1[i][j] + r1) % MOD;
            p1.x2[i][j] = (p1.x2[i][j] + r2) % MOD;
            p2.x1[i][j] = (p2.x1[i][j] + r2) % MOD;
            p2.x2[i][j] = (p2.x2[i][j] + r3) % MOD;
        }
    }
    // 将置换后的新数据发送给p3
    p3.x1 = p2.x2;
    p3.x2 = p1.x1;

    // 服务器p2和p3联合做置换，置换后的数据发给p1
    singleShuffle(p2.x1, p2.π2);
    singleShuffle(p2.x2, p2.π2);
    singleShuffle(p3.x1, p3.π1);
    singleShuffle(p3.x2, p3.π1);
    // 模拟reshare过程
    // 生成随机数r1,r2,r3，满足r1+r2+r3=0
    r1 = generateRandomInteger(0, 100);
    r2 = generateRandomInteger(0, 100);
    r3 = -r1 - r2;
    // 更新数据
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < L; ++j) {
            p2.x1[i][j] = (p2.x1[i][j] + r2) % MOD;
            p2.x2[i][j] = (p2.x2[i][j] + r3) % MOD;
            p3.x1[i][j] = (p3.x1[i][j] + r3) % MOD;
            p3.x2[i][j] = (p3.x2[i][j] + r1) % MOD;
        }
    }
    // 将置换后的新数据发送给p1
    p1.x1 = p3.x2;
    p1.x2 = p2.x1;
}

/**
 * @Method: semi_honest_apply_sort 半诚实安全条件下应用排序
 * @return void
 */
void semi_honest_apply_sort() {
    // 复原原始的共享数据
    p1.x1 = x1_backups;
    p1.x2 = x2_backups;
    p2.x1 = x2_backups;
    p2.x2 = x3_backups;
    p3.x1 = x3_backups;
    p3.x2 = x1_backups;

    auto start_time = high_resolution_clock::now(); // 记录函数开始时间

    // 用组合的置换σ更新ρ的信息
    p1.ρ1 = p1.σ1;
    p1.ρ2 = p1.σ2;
    p2.ρ1 = p2.σ1;
    p2.ρ2 = p2.σ2;
    p3.ρ1 = p3.σ1;
    p3.ρ2 = p3.σ2;

    // 生成共享的随机置换π
    generateSharedShuffle();

    // 对共享的ρ进行洗牌
    shufflingProtocol_ρ();

    // 对共享的x进行洗牌
    shufflingProtocol();

    // 揭示洗牌后的ρ
    vector<long long> ρ_vector(N);
    for (int i = 0; i < N; ++i) {
        ρ_vector[i] = (p1.ρ1[i] + p1.ρ2[i] + p2.ρ2[i]) % MOD;
    }

    // 使用洗牌后的ρ对k的指定位进行置换
    singleShuffle(p1.x1, ρ_vector);
    singleShuffle(p1.x2, ρ_vector);
    singleShuffle(p2.x1, ρ_vector);
    singleShuffle(p2.x2, ρ_vector);
    singleShuffle(p3.x1, ρ_vector);
    singleShuffle(p3.x2, ρ_vector);

    auto end_time = high_resolution_clock::now(); // 记录函数结束时间
    duration<double, milli> total_duration = end_time - start_time;
    printf("应用排序所需时间是：%f 毫秒\n", total_duration.count());
    fflush(stdout);
}