#include "BigIntegerBinary.h"

#include <iostream>
#include <bitset>
#include <assert.h>

using namespace std;

BigIntegerBinary::BigIntegerBinary()
{
  bits = bitset<MAX_BIT>();
  sign = 1;
}

BigIntegerBinary::BigIntegerBinary(int x)
{
  if(x < 0) {
    sign = -1;
    x = -x;
  } else {
    sign = 1;
  }

  bits = bitset<MAX_BIT>(x);
}

BigIntegerBinary::BigIntegerBinary(string s)
{
  if(s[0] == '-') {
    sign = -1;
    s = s.substr(1);
  } else {
    sign = 1;
  }
  bits = bitset<MAX_BIT>(s);
}

string BigIntegerBinary::to_string() const
{
  string result = bits.to_string();
  while(result[0] == '0') {
    result = result.substr(1);
  }
  if(result == "") {
    result = "0";
  }
  if(sign == -1) {
    result = "-" + result;
  }
  return result;
}

BigIntegerBinary BigIntegerBinary::operator+(const BigIntegerBinary &a)
{
  BigIntegerBinary result;
  if(sign == a.sign) {
    result.sign = sign;
    result.bits = bits + a.bits;
  } else {
    if(bits > a.bits) {
      result.sign = sign;
      result.bits = bits - a.bits;
    } else {
      result.sign = a.sign;
      result.bits = a.bits - bits;
    }
  }

  return result;
}

BigIntegerBinary BigIntegerBinary::operator-(BigIntegerBinary a)
{
  a.sign = -a.sign;
  return *this + a;
}

bool BigIntegerBinary::operator<(const BigIntegerBinary &a)
{
  if(sign == a.sign) {
    if(sign == 1) {
      return bits < a.bits;
    } else {
      return bits > a.bits;
    }
  } else {
    return sign == -1;
  }
}

bool BigIntegerBinary::operator<=(const BigIntegerBinary &a)
{
  if(sign == a.sign) {
    if(sign == 1) {
      return bits <= a.bits;
    } else {
      return bits >= a.bits;
    }
  } else {
    return sign == -1;
  }
}

bool BigIntegerBinary::operator>(const BigIntegerBinary &a)
{
  if(sign == a.sign) {
    if(sign == 1) {
      return bits > a.bits;
    } else {
      return bits < a.bits;
    }
  } else {
    return sign == 1;
  }
}

bool BigIntegerBinary::operator>=(const BigIntegerBinary &a)
{
  if(sign == a.sign) {
    if(sign == 1) {
      return bits >= a.bits;
    } else {
      return bits <= a.bits;
    }
  } else {
    return sign == 1;
  }
}

bool BigIntegerBinary::operator[](int i) const
{
  return bits[i];
}

BigIntegerBinary BigIntegerBinary::operator>>(int i)
{
  this->bits = this->bits >> i; 
  return *this;
}

BigIntegerBinary BigIntegerBinary::operator%(const BigIntegerBinary &a)
{
  BigIntegerBinary result;
  result.sign = sign;
  result.bits = bits;
  while(result > a) {
    result = result - a;
  }
  return result;
}

BigIntegerBinary BigIntegerBinary::operator%=(const BigIntegerBinary &a)
{
  while((*this) > a) {
    (*this) = (*this) - a;
  }
  return *this;
}

BigIntegerBinary BigIntegerBinary::powMod(BigIntegerBinary d, BigIntegerBinary n)
{
  // Không xử lý với mũ âm
  assert(d.sign == 1);

  if(d.none()) {
    return BigIntegerBinary("1");
  }
  
  if(d[0] == 1 && d.count() == 1) {
    return *this;
  }

  BigIntegerBinary result;
  result.set(0, 1);
  
  if(sign == 1) {
    result.sign = 1;
  } else {
    result.sign = d.bits[0] == 1 ? -1 : 1;
  }

  int maxx = 0;
  for(int i = 0; i < MAX_BIT; i++) {
    if(d[i] == 1) maxx = i;
  }

  BigIntegerBinary x = *this;
  for(int i = 0; i <= maxx; i++) {
    if(d[i] == 1) {
      result = result.mulMod(x, n);
      result %= n;
    }
    x = x.mulMod(x, n);
    x %= n;
  }
  return result; 
}

BigIntegerBinary BigIntegerBinary::mulMod(BigIntegerBinary d, BigIntegerBinary n)
{
  BigIntegerBinary result;
  result.sign = sign * d.sign;
  
  // result.bits = bits * d.bits;
  BigIntegerBinary x = *this;

  int maxx = 0;
  for(int i = 0; i < MAX_BIT; i++) {
    if(d[i] == 1) maxx = i;
  }

  for(int i = 0; i <= maxx; i++) {
    if(d[i] == 1) {
      result.bits = result.bits + x.bits;
      result %= n;
    }
    x.bits = x.bits << 1;
    x %= n;
  }
  return result;
}

bool BigIntegerBinary::none()
{
  return bits.none();
}

int BigIntegerBinary::count()
{
  return bits.count();
}

int BigIntegerBinary::max_bit()
{
  for(int i = MAX_BIT - 1; i >= 0; i--) {
    if(bits[i] == 1) {
      return i;
    }
  }
  return 0;
}

ostream &operator<<(ostream &out, const BigIntegerBinary &a)
{
  // TODO: insert return statement here
  out << a.to_string();
  return out;
}

bitset<MAX_BIT> operator+(const bitset<MAX_BIT> &a, const bitset<MAX_BIT> &b)
{
  bitset<MAX_BIT> result;
  bool carry = 0;

  for(int i = 0; i < MAX_BIT; i++) {
    char sum = a[i] + b[i] + carry;
    result[i] = sum % 2;
    carry = sum / 2;
  }

  return result;
}

bitset<MAX_BIT> operator-(const bitset<MAX_BIT> &a, const bitset<MAX_BIT> &b)
{
  bitset<MAX_BIT> result;
  bool carry = 0;

  for(int i = 0; i < MAX_BIT; i++) {
    char sum = a[i] - b[i] - carry;

    if(sum < 0) {
      sum += 2;
      carry = 1;
    } else {
      carry = 0;
    }

    result[i] = sum;
  }
  return result;
}

bool operator <(const bitset<MAX_BIT> &a, const bitset<MAX_BIT> &b)
{
  for(int i = MAX_BIT - 1; i >= 0; i--) {
    if(a[i] < b[i]) {
      return true;
    } else if(a[i] > b[i]) {
      return false;
    }
  }
  return false;
}

bool operator<=(const bitset<MAX_BIT> &a, const bitset<MAX_BIT> &b)
{
  for(int i = MAX_BIT - 1; i >= 0; i--) {
    if(a[i] < b[i]) {
      return true;
    } else if(a[i] > b[i]) {
      return false;
    }
  }
  return true;
}

bool operator >(const bitset<MAX_BIT> &a, const bitset<MAX_BIT> &b)
{
  for(int i = MAX_BIT - 1; i >= 0; i--) {
    if(a[i] > b[i]) {
      return true;
    } else if(a[i] < b[i]) {
      return false;
    }
  }
  return false;
}

bool operator>=(const bitset<MAX_BIT> &a, const bitset<MAX_BIT> &b)
{
  for(int i = MAX_BIT - 1; i >= 0; i--) {
    if(a[i] > b[i]) {
      return true;
    } else if(a[i] < b[i]) {
      return false;
    }
  }
  return true;
}