#pragma once
#include <bitset>

using namespace std;

const int MAX_BIT = 515;

class BigIntegerBinary{
  bitset<MAX_BIT> bits;
  int sign;  // sign == 1 is positive, sign == -1 is negative
public:
  BigIntegerBinary();
  BigIntegerBinary(int);
  BigIntegerBinary(string);
  BigIntegerBinary(const BigIntegerBinary& a) : bits(a.bits), sign(a.sign) { }

  BigIntegerBinary& operator=(const BigIntegerBinary& a) { bits = a.bits; sign = a.sign; return *this;}

public:
  friend ostream& operator<<(ostream&, const BigIntegerBinary&);
  string to_string() const;

public: 
  BigIntegerBinary operator+ (const BigIntegerBinary& a);
  BigIntegerBinary operator- (BigIntegerBinary a);
  BigIntegerBinary operator ++ (int i) { BigIntegerBinary temp = *this; *this = *this + BigIntegerBinary("1"); return temp; }
  BigIntegerBinary operator ++ () { *this = *this + BigIntegerBinary("1"); return *this; }
  bool             operator == (const BigIntegerBinary& a) const { return bits == a.bits && sign == a.sign; }
  bool             operator <  (const BigIntegerBinary& a);
  bool             operator <=  (const BigIntegerBinary& a);
  bool             operator >  (const BigIntegerBinary& a);
  bool             operator >=  (const BigIntegerBinary& a);
  bool operator [](int i) const;
  void set(int i, bool value) { bits.set(i, value); }
  BigIntegerBinary operator >> (int i);
  BigIntegerBinary operator %    (const BigIntegerBinary& a);
  BigIntegerBinary operator %=   (const BigIntegerBinary& a);

public:
  BigIntegerBinary powMod(BigIntegerBinary d, BigIntegerBinary n);
  BigIntegerBinary mulMod(BigIntegerBinary d, BigIntegerBinary n);
  bool             none();
  int              count();
  int              max_bit();
};

bitset<MAX_BIT> operator+   (const bitset<MAX_BIT>& a, const bitset<MAX_BIT>& b);
bitset<MAX_BIT> operator-   (const bitset<MAX_BIT>& a, const bitset<MAX_BIT>& b);

bool            operator <  (const bitset<MAX_BIT>& a, const bitset<MAX_BIT>& b);
bool            operator <= (const bitset<MAX_BIT>& a, const bitset<MAX_BIT>& b);
bool            operator >  (const bitset<MAX_BIT>& a, const bitset<MAX_BIT>& b);
bool            operator >= (const bitset<MAX_BIT>& a, const bitset<MAX_BIT>& b);
