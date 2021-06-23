#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

//疑似乱数による乱数生成法（rand()を使った方法）
//rand()が環境によって値が異なるために再現性がなく、統計的に信用性がない
double urand() {

	double m, a;

	//rand()に0.5を足してRAND_MAXに1を足すことで偏りがなくなる
	m = RAND_MAX + 1;
	a = (rand() + 0.5) / m;
	a = (rand() + a) / m;
	return (rand() + a) / m;

}

//Xorshift法に使うフィールド変数
static uint32_t xorshift_X = 123456789;
static uint32_t xorshift_Y = 362436069;
static uint32_t xorshift_Z = 521288629;
static uint32_t xorshift_W = 88675123;;

void InitializeRandom(uint32_t seed) {

	do {
		seed = seed * 1812433253 + 1; seed ^= seed << 13; seed ^= seed >> 17;
		xorshift_X = 123464980 ^ seed;
		seed = seed * 1812433253 + 1; seed ^= seed << 13; seed ^= seed >> 17;
		xorshift_Y = 3447902351 ^ seed;
		seed = seed * 1812433253 + 1; seed ^= seed << 13; seed ^= seed >> 17;
		xorshift_Z = 2859490775 ^ seed;
		seed = seed * 1812433253 + 1; seed ^= seed << 13; seed ^= seed >> 17;
		xorshift_W = 47621719 ^ seed;
	} while (xorshift_X == 0 && xorshift_Y == 0 && xorshift_Z == 0 && xorshift_W == 0);

}

//Xorshift法の一様乱数
//性質のいい疑似乱数を作れる（メイセンヌツイスタ法より目おとりするけど、モンテカロには使える）
double Xorshift() {

	uint32_t t;
	t = xorshift_X ^ (xorshift_X << 11);
	xorshift_X = xorshift_Y;
	xorshift_Y = xorshift_Z;
	xorshift_Z = xorshift_W;
	xorshift_W ^= t ^ (t >> 8) ^ (xorshift_W >> 19);
	return ((xorshift_X + 0.5) / 4294967296.0 + xorshift_W) / 4294967296.0;

}

//正規乱数による乱数(Box-Muller法)
//平均0、分散１の正規分布に従う乱数生成
double BoxMullerRand() {

	return sqrt(-2 * log(urand())) * cos(2 * M_PI *urand());
}

//正規乱数による乱数(Marsaglia polar法)
//Box-Muller法より速い
double MarsagliaPollarRand() {

	double x, y, r;
	do {
		x = 2 * urand() - 1;
		y = 2 * urand() - 1;
		r = x * x + y * y;

	} while (r >= 1);

	return sqrt(-2 * log(r) / r)*x;

}

//指数分布に従う乱数（逆関数法）
double ExpRand(double lamda) {

	return -lamda * log(urand());
}

//ポアソン分布に従う乱数
double PoasonRand(double lamda) {

	double a, b;
	int k = 0;
	a = urand();
	b = exp(-lamda);
	while (a >= b) {
		a *= urand();
		k++;
	}

	return k;
}

int main()
{
	int i;
	srand((unsigned int)time(NULL));
	InitializeRandom((unsigned int)time(NULL));

	printf("疑似乱数\n");
	for (i = 0; i < 10; i++) {

		printf("%f\n", urand());
	}

	printf("一様乱数（Xorshift法）\n");
	for (i = 0; i < 10; i++)
	{
		printf("%f\n", Xorshift());
	}

	printf("Box-Muller法\n");
	for (i = 0; i < 10; i++) {

		printf("%f\n", BoxMullerRand());
	}

	printf("MarsagliaPollar法\n");
	for (i = 0; i < 10; i++) {
		printf("%f\n", MarsagliaPollarRand());
	}

	printf("指数分布（逆関数法）\n");
	for (i = 0; i < 10; i++) {
		printf("%f\n", ExpRand(1.0));
	}

	printf("ポアソン分布に従う乱数\n");
	for (i = 0; i < 10; i++) {
		printf("%f\n", PoasonRand(1.0));
	}

	return 0;
}
