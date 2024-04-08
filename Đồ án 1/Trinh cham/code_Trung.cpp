#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <ostream>
#include <random>
#include <fstream>
#define MAX 512
#define ERR_INVALID_ARGS 1
#define ERR_CANNOT_OPEN_FILE 2
using namespace std;

typedef bitset<MAX> uint512;

static const uint512 UINT512_MAX = uint512(string(MAX, '1'));
static const short base_primes[30] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
                                      31, 37, 41, 43, 47, 53, 59, 61, 67,
                                      71, 73, 79, 83, 89, 97, 101, 103, 107,
                                      109, 113};

bool operator<(const uint512 &a, const uint512 &b);
bool operator>(const uint512 &a, const uint512 &b);
bool operator<=(const uint512 &a, const uint512 &b);
bool operator>=(const uint512 &a, const uint512 &b);

uint512 operator+(const uint512 &a, const uint512 &b);
uint512 operator-(const uint512 &a, const uint512 &b);
uint512 operator%(const uint512 &a, const uint512 &b);

uint512 operator*(const uint512 &a, const uint512 &b);

uint512 hexStringToUint512(string hex_string);
short findMSB(const uint512 &a);

uint512 addMod(const uint512 &a, const uint512 &b, const uint512 &mod);
uint512 subMod(const uint512 &a, const uint512 &b, const uint512 &mod);
uint512 mulMod(const uint512 &a, const uint512 &b, const uint512 &mod);
uint512 powMod(const uint512 &base, const uint512 &exp, const uint512 &mod);

bool isPrimeMillerRabin(const uint512 &n, short base = 2);
bool isPrimeMillerRabinMultiBases(const uint512 &n, short n_bases = 4);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Error, main() requires 2 arguments: <input_file>, <output_file> but " << argc - 1 << " were given" << endl;
        return ERR_INVALID_ARGS;
    }

    ifstream i_file;
    i_file.open(argv[1], ios::in);
    if (!i_file.is_open())
    {
        cerr << "Error, could not open file to read: " << argv[1] << endl;
        return ERR_CANNOT_OPEN_FILE;
    }

    string hex_str;
    getline(i_file, hex_str);

    i_file.close();

    uint512 n = hexStringToUint512(hex_str);

    bool result = isPrimeMillerRabinMultiBases(n, 3);

    ofstream o_file;
    o_file.open(argv[2], ios::out);
    if (!o_file.is_open())
    {
        cerr << "Error, could not open file to write: " << argv[2] << endl;
        return ERR_CANNOT_OPEN_FILE;
    }
    // cout << result << endl;
    o_file << result;
    o_file.close();
    return 0;
}

bool operator<(const uint512 &a, const uint512 &b)
{
    short a_count = a.count();
    short b_count = b.count();
    for (int i = MAX - 1; i >= 0; i--)
    {

        if (a[i] < b[i])
        {
            return true;
        }
        else if (a[i] > b[i])
        {
            return false;
        }
    }
    return false;
}

bool operator>(const uint512 &a, const uint512 &b)
{
    for (int i = MAX - 1; i >= 0; i--)
    {
        if (a[i] > b[i])
        {
            return true;
        }
        else if (a[i] < b[i])
        {
            return false;
        }
    }
    return false;
}

bool operator<=(const uint512 &a, const uint512 &b)
{
    for (int i = MAX - 1; i >= 0; i--)
    {
        if (a[i] < b[i])
        {
            return true;
        }
        else if (a[i] > b[i])
        {
            return false;
        }
    }
    return true;
}

bool operator>=(const uint512 &a, const uint512 &b)
{
    for (int i = MAX - 1; i >= 0; i--)
    {
        if (a[i] > b[i])
        {
            return true;
        }
        else if (a[i] < b[i])
        {
            return false;
        }
    }
    return true;
}

uint512 operator+(const uint512 &a, const uint512 &b)
{
    uint512 result = a;
    bool carry = 0;
    short ab_count = a.count() + b.count();
    for (int i = 0; i < MAX; i++)
    {
        bool a_bit = a[i];
        bool b_bit = b[i];
        result[i] = a_bit ^ b_bit ^ carry;
        carry = (a_bit & b_bit) | (a_bit & carry) | (b_bit & carry);
        ab_count -= (a_bit + b_bit);
        if ((ab_count == 0) && (carry == 0))
        {
            break;
        }
    }
    result[MAX] = carry;
    return result;
}

uint512 operator-(const uint512 &a, const uint512 &b)
{
    uint512 result = a;
    bool carry = 0;
    short ab_count = b.count();
    for (int i = 0; i < MAX; i++)
    {
        bool a_bit = a[i];
        bool b_bit = b[i];
        result[i] = a_bit ^ b_bit ^ carry;
        carry = (!a_bit & b_bit) | (!a_bit & carry) | (b_bit & carry);
        ab_count -= (b_bit);
        if ((ab_count == 0) && (carry == 0))
        {
            break;
        }
    }
    return result;
}

uint512 operator%(const uint512 &a, const uint512 &b)
{
    if (b == 0)
    {
        throw std::invalid_argument("Division by zero error");
    }
    if (b == 1)
    {
        return 0;
    }
    if (a == 1)
    {
        return 1;
    }

    uint512 result = a;
    uint512 b_shifted = b << (findMSB(a) - findMSB(b));
    while (result >= b)
    {
        if (result >= b_shifted)
        {
            result = result - b_shifted;
        }
        b_shifted = b_shifted >> 1;
    }

    return result;
}

uint512 operator*(const uint512 &a, const uint512 &b)
{
    uint512 result = 0;
    uint512 a_exp2 = a;
    for (int i = 0; i < MAX; i++)
    {
        if (b[i])
        {

            result = result + (a_exp2);
        }
        a_exp2 = a_exp2 << 1;
    }
    return result;
}

uint512 hexStringToUint512(string hex_string)
{
    uint512 result(0);
    for (int i = hex_string.length() - 1; i >= 0; i--)
    {
        char c = hex_string[i];
        int val = 0;
        if (('0' <= c) && (c <= '9'))
        {
            val = c - '0';
        }
        else if (('a' <= c) && (c <= 'f'))
        {
            val = c - 'a' + 10;
        }
        else if (('A' <= c) && (c <= 'F'))
        {
            val = c - 'A' + 10;
        }
        result = result << 4;
        result |= val;
    }
    return result;
}

short findMSB(const uint512 &a)
{
    for (int i = MAX - 1; i >= 0; i--)
    {
        if (a[i])
        {
            return i;
        }
    }
    return -1;
}

uint512 addMod(const uint512 &a, const uint512 &b, const uint512 &mod)
{
    uint512 result = (a + b);
    if ((result < a) | (result < b)) // Overflow
    {
        // 1abcd mod m = (10000 + abcd) mod m = (1111 mod m) + (1 mod m) + (abcd mod m)
        result = result + (UINT512_MAX % mod) + (1 % mod);
    }
    return result % mod;
}

uint512 subMod(const uint512 &a, const uint512 &b, const uint512 &mod)
{
    uint512 a_temp = a % mod;
    uint512 b_temp = b % mod;
    if (a_temp >= b_temp)
    {
        return a_temp - b_temp;
    }
    else
    {
        return mod - b_temp + a_temp;
    }
}

uint512 mulMod(const uint512 &a, const uint512 &b, const uint512 &mod)
{
    uint512 result = 0;
    uint512 b_shifted = b % mod;
    short a_count = a.count();
    bool overflow_happened = false;
    uint512 remainder_overflow;
    for (int i = 0; i < MAX; i++)
    {
        if (a[i])
        {
            result = addMod(result, b_shifted, mod);
            a_count--;
        }

        if (a_count == 0)
        {
            break;
        }
        if (b_shifted[MAX - 1])
        {
            if (!overflow_happened)
            {
                overflow_happened = true;
                remainder_overflow = (UINT512_MAX % mod);
            }
            b_shifted = ((b_shifted << 1) % mod) + remainder_overflow + (1 % mod);
        }
        else
        {
            b_shifted = b_shifted << 1;
        }
        b_shifted = b_shifted % mod;
    }
    return result;
}

uint512 powMod(const uint512 &base, const uint512 &exp, const uint512 &mod)
{
    uint512 result = 1;

    for (int i = MAX - 1; i >= 0; i--)
    {
        if (result != 1)
        {
            result = mulMod(result, result, mod);
        }
        if (exp[i])
        {
            result = mulMod(result, base, mod);
        }
    }

    return result;
}

bool isPrimeMillerRabin(const uint512 &n, short base)
{
    // (n - 1) = 2^r * m
    uint512 m = n;
    uint512 n_minus_1 = n - 1;
    m[0] = 0;

    short r = 0;
    while (m[0] != 1)
    {
        m = m >> 1;
        r++;
    }

    m = m << 1;
    uint512 pow = powMod(base, m, n); // pow = base^m (mod n)
    if ((pow == 1) || (pow == (n - 1)))
    {
        return true;
    }

    bool trivial = false;
    for (int i = 2; i <= r; i++)
    {
        pow = mulMod(pow, pow, n);
        if ((pow == n_minus_1) && (i == r))
        {
            return false;
        }
        if ((pow == 1) || (pow == n_minus_1))
        {
            return true;
        }
    }
    return false;
}

bool isPrimeMillerRabinMultiBases(const uint512 &n, short n_bases)
{
    for (int i = 0; i < n_bases; i++)
    {
        short base = base_primes[i];
        if (!isPrimeMillerRabin(n, base))
        {
            return false;
        }
    }
    return true;
}
