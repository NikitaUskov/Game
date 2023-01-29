#ifndef OOP_SEM5_HELP_FUNC_H
#define OOP_SEM5_HELP_FUNC_H

#include <iostream>
//#include <fstream>
//#include <unordered_map>
#include <string>
#include <vector>
//#include <cstdlib>
//#include <ctime>
#include <random>
#include <cmath>

namespace RPG {

using coordinate = std::pair<int, int>;

static bool operator==(const coordinate &frst, const coordinate &scnd) {
    return ((frst.first == scnd.first) && (frst.second == scnd.second));
}

static bool operator!=(const coordinate &frst, const coordinate &scnd) {
    return ((frst.first != scnd.first) || (frst.second != scnd.second));
}


static coordinate& operator+=(coordinate& frst, coordinate& scnd) {
    frst.first = frst.first + scnd.first;
    frst.second = frst.second + scnd.second;
    return frst;
}

static coordinate operator+(coordinate& frst, coordinate& scnd) {
    coordinate new_pos = std::make_pair(0, 0);
    new_pos += frst;
    new_pos += scnd;
    return new_pos;
}

static coordinate& operator-=(coordinate& frst, coordinate& scnd) {
    frst.first = frst.first - scnd.first;
    frst.second = frst.second - scnd.second;
    return frst;
}

static coordinate operator-(coordinate& frst, coordinate& scnd) {
    coordinate new_pos = std::make_pair(0, 0);
    new_pos += frst;
    new_pos -= scnd;
    return new_pos;
}

static coordinate operator*(coordinate& frst, int& scnd) {
    coordinate new_pos = frst;
    new_pos.first *= scnd;
    new_pos.second *= scnd;
    return new_pos;
}

static std::pair<coordinate, int> coord2Dir(coordinate& coor) {
    if (coor.first == 0 && coor.second != 0) {
        return std::make_pair(std::make_pair(coor.first, coor.second / abs(coor.second)), abs(coor.second));
    }
    else if (coor.first != 0 && coor.second == 0) {
        return std::make_pair(std::make_pair(coor.first / abs(coor.first), coor.second), abs(coor.first));
    }
    else {
        return std::make_pair(coor, -1);
    }
}

struct mhash {
    std::size_t operator()(std::pair <int, int> const &coor) const noexcept {
        std::size_t h1 = std::hash < int > {}(coor.first);
        std::size_t h2 = std::hash < int > {}(coor.second);
        return h1 ^ (h2 << 1);
    }
};

static int GetRandomNumber(int min, int max)
{
    std::random_device rd;
    std::mt19937 mersenne(rd());
    int num = min + (int) (mersenne() % (max - min + 1));
    return num;
}

static const double Pi = 3.1415926535;
//  Массивы заполненные с градацией по 3 градуса. 3 * 120 = 360
static const int graduate = 3;
static const int numOfRays = 360 / graduate;
static double Cosinus[numOfRays];
static double Sinus[numOfRays];

static void GenerateTables() {
    for (int i = 0; i < numOfRays; ++i)
    {
        Cosinus[i] = cos(i * graduate * Pi / 180);
        Sinus[i] = sin(i * graduate * Pi / 180);
    }
}

static RPG::coordinate CastRay(int x, int y, int r, int angle, int height, int width) {
    RPG::coordinate c;
    c.first = (int) round(x + r * Cosinus[(int) angle / graduate]);
    c.second = (int) round(y + r * Sinus[(int) angle / graduate]);
    c.first = c.first >= width ? width - 1 : (c.first < 0 ? 0 : c.first);
    c.second = c.second >= height ? height - 1 : (c.second < 0 ? 0 : c.second);
    return c;
}



static int Sgn(int x) {
    if (x == 0) {
        return 0;
    }
    else {
        return x > 0 ? 1 : -1;
    }
}

template <class T>
static int getNum(T &a){ //get int number
    std::cin >> a;
    if (!std::cin.good())
        return -1;
    return 1;
}
static inline int get_int(const char *msg, int &n, int min, int max){
    const char *pr = "";
    do{
        std::cout << pr << std::endl;
        std::cout << msg;
        pr = "You are wrong, repeat please!";
        if (getNum(n) < 0) // input Error
            return -1;
    } while (n < min || n > max);
    return n;
}

} // RPG

#endif //OOP_SEM5_HELP_FUNC_H
