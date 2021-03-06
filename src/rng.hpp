#ifndef RNG_HPP
#define RNG_HPP

#include "pos.hpp"

// Simple multiply-with-carry RNG, basically straight from wikipedia

class RNG
{
private:
    unsigned int _w, _z;

public:
    int Integer(int n);
    int Integer(int a, int b);

    float Float();
    float Float(float max);
    float Float(float min, float max);

    float Gaussian();
    float Gaussian(float mean, float sd);

    bool Bool();
    bool Bool(float prob);

    Pos Position(int rows, int cols);
    int Dir();
    int Bit();
    int Bit(float prob);

    int Sign();

    RNG();

    void Seed(int i);

    friend std::ostream& operator<<(std::ostream& os, const RNG& p);
    friend std::istream& operator>>(std::istream& is, RNG& p);
    friend class RNGWidget;
};

std::ostream& operator<<(std::ostream& os, const RNG& p);
std::istream& operator>>(std::istream& is, RNG& p);


#endif // RNG_HPP
