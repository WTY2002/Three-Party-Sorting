/**
* @author: WTY
* @date: 2024/10/19
* @description: Implement sorting under semi-honest and safe conditions
*/

#ifndef SEMI_HONEST_SECURITY_H
#define SEMI_HONEST_SECURITY_H


#include <iostream>
#include <random>
#include <fstream>
#include <sstream>
#include <vector>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <cstdio>
using namespace std;
using namespace std::chrono;


// 8比特长度，最大值为2^7-1
#define MAX_8 127

// 32比特长度，最大值为2^31-1
#define MAX_32 2147483647

// 64比特长度，最大值为2^63-1
#define MAX_64 9223372036854775807LL

// 模数，用于模运算，取值为2^61-1
#define MOD 2305843009213693951LL

// 数据量的大小
extern int N;

// 数据的比特长度
extern int L;

// 原始数据
extern vector<long long> data;

// 服务器
struct Sever {
    // 服务器持有的数据份额
    vector<vector<long long>> x1, x2;
    // 服务器持有的置换份额
    vector<long long> π1;
    vector<long long> π2;
    // 服务器持有的逆置换份额
    vector<long long> π1_inv;
    vector<long long> π2_inv;
    // 单比特排序的置换
    vector<long long> ρ1, ρ2;
    // 暂存，用于后续组合两个置换
    vector<long long> σ1, σ2;
};

// 服务器
extern Sever p1, p2, p3;

// 共享的数据备份
extern vector<vector<long long>> x1_backups, x2_backups, x3_backups;

/**
 * @Method initSever 初始化云服务器
 * @return void
 */
void initSever();

/**
 * @Method readDataFromFile 从文件中读取数据
 * @param filename 文件名
 * @return void
 */
void readDataFromFile(const char* filename);

/**
 * @Method decimalToBinary 将十进制数转换为二进制数
 * @param num 要转换的十进制数
 * @return vector<long long> 二进制数
 */
vector<long long> decimalToBinary(long long num);

/**
 * @Method bitDecomposition 执行位分解
 * @param index 索引
 * @param a, b, c 分解的比特
 * return void
 */
void bitDecomposition(int index, long long a, int b, int c);

/**
 * @Method generateRandomInteger 生成一个指定范围内的随机整数
 * @param min 最小值
 * @param max 最大值
 * @return long long 随机整数
 */
long long generateRandomInteger(long long min, long long max);

/**
 * @Method Generateshuffle 生成一个随机置换
 * @return vector<long long> 随机置换
 */
vector<long long> Generateshuffle();

/**
 * @Method generateSharedShuffle 生成共享的随机置换
 * @return void
 */
void generateSharedShuffle();

/**
 * @Method singleShuffle 对数据单次置换
 * @param x 数据
 * @param π 置换
 * @param index 索引
 * @return void
 */
void singleShuffle(vector<vector<long long>> &x, vector<long long> &π, int index);

/**
 * @Method singleShuffle 对数据单次置换
 * @param ρ 置换ρ
 * @param π 置换π
 * @return void
 */
void singleShuffle(vector<long long> &ρ, vector<long long> &π);

/**
 * @Method shufflingProtocol 完整的shuffling协议
 * @param index 索引
 * @return void
 */
void shufflingProtocol(int index);

/**
 * @Method shufflingProtocol 完整的shuffling协议，对ρ进行洗牌
 * @return void
 */
void shufflingProtocol_ρ();

/**
 * @Method shufflingProtocol 完整的shuffling协议，对σ进行洗牌
 * @return void
 */
void shufflingProtocol_σ();

/**
 * @Method generateSharedInverseShuffle 生成共享的逆置换
 * @return void
 */
void generateSharedInverseShuffle();

/**
 * @Method unShufflingProtocol 完整的unShuffling协议
 * @param index 索引
 * @return void
 */
void unShufflingProtocol(int index);

/**
 * @Method unShufflingProtocol 完整的unShuffling协议，对ρ进行逆洗牌
 * @return void
 */
void unShufflingProtocol_ρ();

/**
 * @Method genBitPerm 生成共享的单比特置换
 * @param index 索引
 * @return void
 */
void genBitPerm(int index);

/**
 * @Method applyPerm 应用共享的单比特置换σ
 * @param index 索引
 * @return void
 */
void applyPerm(int index);

/**
 * @Method composePerm 组合两个置换
 * @return void
 */
void composePerm();

/**
 * @Method genPerm 生成共享的稳定置换
 * @return void
 */
void genPerm();

/**
 * @Method: init_semi_honest_security 初始化半诚实安全下的操作
 * @return void
 */
void init_semi_honest_security();

/**
 * @Method: semi_honest_sort 半诚实安全条件下进行排序
 * @return void
 */
void semi_honest_sort();

/**
 * @Method singleShuffle 对数据的整体进行单次置换
 * @param x 数据
 * @param π 置换
 * @return void
 */
void singleShuffle(vector<vector<long long>> &x, vector<long long> &π);

/**
 * @Method shufflingProtocol 完整的shuffling协议，对整体进行洗牌
 * @return void
 */
void shufflingProtocol();

/**
 * @Method: semi_honest_apply_sort 半诚实安全条件下应用排序
 * @return void
 */
void semi_honest_apply_sort();

#endif //SEMI_HONEST_SECURITY_H
