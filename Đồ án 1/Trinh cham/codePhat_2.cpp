#include <string>
#include <bitset>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

typedef long long ll;

using namespace std;

// const int MAX_BIT = 1024 + 5;
const int BIT_PER_DIGIT = 30;
const ll BASE = 1ll << BIT_PER_DIGIT;
// const int MAX_SIZE = MAX_BIT / BIT_PER_DIGIT + 1;

typedef long long ll;
typedef unsigned int uint;

class BigNumWithInteger
{
    using TYPE = int;

    vector<TYPE> digits;
    int sign; // sign == 1 is positive, sign == -1 is negative
public:
    BigNumWithInteger();
    BigNumWithInteger(ll);
    BigNumWithInteger(string);
    BigNumWithInteger(const BigNumWithInteger &a) : digits(a.digits), sign(a.sign) {}

    BigNumWithInteger &operator=(const BigNumWithInteger &a)
    {
        digits = a.digits;
        sign = a.sign;
        return *this;
    }

public:
    string to_string() const;
    friend ostream &operator<<(ostream &, const BigNumWithInteger &);

public:
    BigNumWithInteger operator+(const BigNumWithInteger &a);
    BigNumWithInteger operator-(BigNumWithInteger a);
    BigNumWithInteger operator++(int i)
    {
        BigNumWithInteger temp = *this;
        *this = *this + BigNumWithInteger("1");
        return temp;
    }
    BigNumWithInteger operator++()
    {
        *this = *this + BigNumWithInteger("1");
        return *this;
    }
    bool operator==(const BigNumWithInteger &a) const { return digits == a.digits && sign == a.sign; }
    bool operator!=(const BigNumWithInteger &a) { return !(*this == a); }
    bool operator<(const BigNumWithInteger &a);
    bool operator<=(const BigNumWithInteger &a);
    bool operator>(const BigNumWithInteger &a);
    bool operator>=(const BigNumWithInteger &a);
    TYPE &operator[](int i);
    void set(int i, TYPE value) { digits[i] = value; }
    BigNumWithInteger operator>>(int i);
    BigNumWithInteger operator<<(int i);
    BigNumWithInteger operator%(BigNumWithInteger a) const;
    BigNumWithInteger operator%=(const BigNumWithInteger a);
    BigNumWithInteger operator*(const BigNumWithInteger &a) const;

public:
    BigNumWithInteger mulMod(BigNumWithInteger d, BigNumWithInteger n);
    BigNumWithInteger powMod(BigNumWithInteger d, BigNumWithInteger n);
    BigNumWithInteger abs() const;

public:
    void set_sign(int sign) { this->sign = sign; }
    int get_sign() const { return this->sign; }

public:
    int max_bit() { return bit_size(); }
    void trim();
    int size() const;
    int bit_size() const;
    bool is_zero() const;
    bool is_one() const;
    bool is_even() const;
};

string hex_to_bin(string a_hex)
{
    string a_bin;
    for (int i = 0; i < int(a_hex.size()); i++)
    {
        if (a_hex[i] >= '0' && a_hex[i] <= '9')
        {
            a_bin += bitset<4>(a_hex[i] - '0').to_string();
        }
        else
        {
            a_bin += bitset<4>(a_hex[i] - 'A' + 10).to_string();
        }
    }
    return a_bin;
}

string bin_to_hex(string a_bin)
{
    string a_hex;
    if (a_bin[0] == '-')
    {
        a_hex += '-';
        a_bin = a_bin.substr(1);
    }
    reverse(a_bin.begin(), a_bin.end());
    while (a_bin.size() % 4 != 0)
    {
        a_bin += '0';
    }
    for (int i = 0; i < int(a_bin.size()); i += 4)
    {
        int num = 0;
        for (int j = 0; j < 4; j++)
        {
            num += (a_bin[i + j] - '0') * pow(2, j);
        }
        if (num >= 0 && num <= 9)
        {
            a_hex += to_string(num);
        }
        else
        {
            a_hex += (char)(num - 10 + 'A');
        }
    }
    reverse(a_hex.begin(), a_hex.end());
    return a_hex;
}

string int_to_binary(int x)
{
    string a_bin;
    string temp = "";
    if (x < 0)
    {
        temp = "-";
        x = -x;
    }
    while (x > 0)
    {
        a_bin = to_string(x % 2) + a_bin;
        x /= 2;
    }
    return temp + a_bin;
}

BigNumWithInteger::BigNumWithInteger()
{
    digits.clear();
    sign = 1;
}

BigNumWithInteger::BigNumWithInteger(ll s)
{
    digits.clear();
    if (s < 0)
    {
        sign = -1;
        s = -s;
    }
    else
    {
        sign = 1;
    }

    if (s == 0)
    {
        digits.push_back(0);
        return;
    }

    while (s > 0)
    {
        digits.push_back(s % BASE);
        s /= BASE;
    }
}

BigNumWithInteger::BigNumWithInteger(string s)
{
    digits.clear();
    sign = 1;
    if (s[0] == '-')
    {
        sign = -1;
        s = s.substr(1);
    }

    if (s.size() == 0)
    {
        digits.push_back(0);
        return;
    }
    reverse(s.begin(), s.end());
    int pos = 0;
    while (pos < s.size())
    {
        uint value = 0;
        for (int i = pos; i < pos + BIT_PER_DIGIT && i < s.size(); i++)
        {
            value += (s[i] - '0') * (1 << (i - pos));
        }
        digits.push_back(value);
        pos += BIT_PER_DIGIT;
    }

    this->trim();
}

string BigNumWithInteger::to_string() const
{
    if (is_zero())
        return "0";
    string binary = "";
    for (uint val : digits)
    {
        for (int i = 0; i < BIT_PER_DIGIT; i++)
        {
            binary += (val & (1 << i)) ? '1' : '0';
        }
    }

    while (binary.size() > 1 && binary.back() == '0')
    {
        binary.pop_back();
    }

    reverse(binary.begin(), binary.end());
    if (sign == -1)
    {
        binary = '-' + binary;
    }
    return binary;
}

BigNumWithInteger BigNumWithInteger::operator+(const BigNumWithInteger &a)
{
    int n = size();
    int m = a.size();
    BigNumWithInteger ans;
    ans.digits.resize(max(n, m) + 1);

    if (sign == a.sign)
    {
        ans.sign = sign;
        int carry = 0;
        ll sum = 0;
        for (int i = 0; i < max(n, m); i++)
        {
            sum = carry;
            if (i < n)
                sum += digits[i];
            if (i < m)
                sum += a.digits[i];
            ans.digits[i] = sum % BASE;
            carry = sum / BASE;
        }
        ans.digits[max(n, m)] = carry;
        ans.trim();
        return ans;
    }
    else
    {
        if (this->abs() >= a.abs())
        {
            ans.sign = sign;
            int carry = 0;
            ll sum = 0;
            for (int i = 0; i < n; i++)
            {
                sum = carry + digits[i];
                if (i < m)
                    sum -= a.digits[i];
                if (sum < 0)
                {
                    sum += BASE;
                    carry = -1;
                }
                else
                {
                    carry = 0;
                }
                ans.digits[i] = sum;
            }
            ans.trim();
            return ans;
        }
        else
        {
            ans.sign = a.sign;
            int carry = 0;
            ll sum = 0;
            for (int i = 0; i < m; i++)
            {
                sum = carry + a.digits[i];
                if (i < n)
                    sum -= digits[i];
                if (sum < 0)
                {
                    sum += BASE;
                    carry = -1;
                }
                else
                {
                    carry = 0;
                }
                ans.digits[i] = sum;
            }
            ans.trim();
            return ans;
        }
    }
}

BigNumWithInteger BigNumWithInteger::operator-(BigNumWithInteger a)
{
    a.sign = -a.sign;
    return *this + a;
}

bool BigNumWithInteger::operator<(const BigNumWithInteger &a)
{
    if (sign != a.sign)
        return sign < a.sign;

    bool is_negative = (sign == -1);

    int n = size();
    int m = a.size();
    if (n < m)
        return true ^ is_negative;
    if (n > m)
        return false ^ is_negative;

    for (int i = n - 1; i >= 0; i--)
    {
        if (digits[i] < a.digits[i])
            return true ^ is_negative;
        if (digits[i] > a.digits[i])
            return false ^ is_negative;
    }
    return false;
}

bool BigNumWithInteger::operator<=(const BigNumWithInteger &a)
{
    if (sign != a.sign)
        return sign < a.sign;
    bool is_negative = (sign == -1);

    int n = size();
    int m = a.size();
    if (n < m)
        return true ^ is_negative;
    if (n > m)
        return false ^ is_negative;
    for (int i = n - 1; i >= 0; i--)
    {
        if (digits[i] < a.digits[i])
            return true ^ is_negative;
        if (digits[i] > a.digits[i])
            return false ^ is_negative;
    }
    return true;
}

bool BigNumWithInteger::operator>(const BigNumWithInteger &a)
{
    return !(*this <= a);
}

bool BigNumWithInteger::operator>=(const BigNumWithInteger &a)
{
    return !(*this < a);
}

int &BigNumWithInteger::operator[](int i)
{
    if (i < 0 || i > size())
    {
        throw "Index out of range";
    }

    return digits[i];
}

BigNumWithInteger BigNumWithInteger::operator>>(int i)
{
    if (i < 0)
    {
        throw "Shift right must be positive";
    }

    if (i == 0)
        return *this;

    if (i > BASE)
    {
        throw "Shift right ammount is to big";
    }

    long long value = (1ll << i);

    int n = size();
    BigNumWithInteger ans;
    ans.sign = sign;
    ans.digits.resize(n);

    ll carry = 0;
    ll sum = 0;
    for (int j = n - 1; j >= 0; j--)
    {
        sum = digits[j];
        if (carry > 0)
        {
            sum += 1ll * BASE * carry;
        }
        carry = sum % value;
        ans.digits[j] = sum >> i;
    }
    ans.trim();
    return ans;
}

BigNumWithInteger BigNumWithInteger::operator<<(int i)
{
    if (i < 0)
    {
        throw "Shift left must be positive";
    }

    if (i == 0)
        return *this;

    if (i > BASE)
    {
        throw "Shift left ammount is to big";
    }

    ll carry = 0;
    ll sum = 0;
    int n = size();
    BigNumWithInteger ans;
    ans.sign = sign;
    ans.digits.resize(n + i);
    for (int j = 0; j < n; j++)
    {
        if (j == 0)
        {
            sum = digits[j] << i;
            ans.digits[j] = sum % BASE;
            carry = sum / BASE;
        }
        else
        {
            sum = digits[j] + carry;
            ans.digits[j] = sum % BASE;
            carry = sum / BASE;
        }
    }
    ans.digits[n] = carry;
    ans.trim();
    return ans;
}

BigNumWithInteger BigNumWithInteger::operator%(BigNumWithInteger a) const
{
    BigNumWithInteger ans(*this);
    a = a.abs();
    ans = ans.abs();
    if (ans < a)
    {
        ans.sign = sign;
        return ans;
    }

    while (ans >= a)
    {
        ans = ans - a;
    }
    ans.trim();
    ans.sign = sign;
    return ans;
}

BigNumWithInteger BigNumWithInteger::operator%=(BigNumWithInteger a)
{
    *this = *this % a;
    return *this;
}

BigNumWithInteger BigNumWithInteger::operator*(const BigNumWithInteger &a) const
{
    int n = size();
    int m = a.size();
    BigNumWithInteger ans;
    ans.digits.resize(n + m);
    ans.sign = sign * a.sign;

    ll carry = 0;
    ll sum = 0;
    for (int i = 0; i < n; i++)
    {
        carry = 0;
        for (int j = 0; j < m; j++)
        {
            sum = ans.digits[i + j] + 1ll * digits[i] * a.digits[j] + carry;
            ans.digits[i + j] = sum % BASE;
            carry = sum / BASE;
        }
        ans.digits[i + m] = carry;
    }

    ans.trim();
    return ans;
}

BigNumWithInteger BigNumWithInteger::abs() const
{
    BigNumWithInteger ans(*this);
    ans.sign = 1;
    return ans;
}

BigNumWithInteger BigNumWithInteger::mulMod(BigNumWithInteger d, BigNumWithInteger n)
{
    BigNumWithInteger ans;
    if (d.is_zero() || this->is_zero())
    {
        return ans;
    }

    ans.sign = this->sign * d.sign;
    d = d.abs();

    string binary = d.to_string();
    reverse(binary.begin(), binary.end());

    BigNumWithInteger temp = (*this);
    temp = temp.abs();
    temp %= n;

    for (int i = 0; i < binary.size(); i++)
    {
        if (binary[i] == '1')
        {
            ans = (ans + temp) % n;
        }
        temp = (temp + temp) % n;
    }

    return ans % n;
}

BigNumWithInteger BigNumWithInteger::powMod(BigNumWithInteger d, BigNumWithInteger n)
{
    BigNumWithInteger ans(1);
    if (this->is_zero())
        return BigNumWithInteger(0);
    if (d.is_zero())
        return ans;
    if (d.is_one())
        return *this;

    if (d.sign == -1)
    {
        throw "Exponent must be positive";
    }

    string binary = d.to_string();
    reverse(binary.begin(), binary.end());

    BigNumWithInteger x(*this);
    x = x.abs();
    x %= n;
    for (int i = binary.size() - 1; i >= 0; i--)
    {
        if (!(ans.is_zero() || ans.is_one()))
        {
            ans = ans.mulMod(ans, n);
        }

        if (binary[i] == '1')
        {
            ans = ans.mulMod(x, n);
        }

        if (ans >= n)
        {
            cerr << "ans >= n\n";
        }
    }

    ans %= n;

    // cout << "ans = " << ans << endl;
    // cout << "n   = " << n << endl;
    // cout << "ans >= n: " << (ans >= n) << endl;
    // cout << ans.size() << ' ' << n.size() << endl;
    // cout << ans.bit_size() << ' ' << n.bit_size() << endl;
    // cout << ans.digits.back() << ' ' << n.digits.back() << endl;

    if (sign == 1)
    {
        ans.sign = 1;
    }
    else
    {
        ans.sign = d[0] & 1 ? -1 : 1;
    }

    return ans;
}

ostream &operator<<(ostream &out, const BigNumWithInteger &a)
{
    out << a.to_string();
    return out;
}

void BigNumWithInteger::trim()
{
    while (digits.size() > 0 && !digits.back())
    {
        digits.pop_back();
    }
}

int BigNumWithInteger::size() const
{
    return digits.size();
}

int BigNumWithInteger::bit_size() const
{
    return this->to_string().size();
}

bool BigNumWithInteger::is_zero() const
{
    return digits.empty() || (digits.size() == 1 && digits[0] == 0);
}

bool BigNumWithInteger::is_one() const
{
    return digits.size() == 1 && digits[0] == 1;
}

bool BigNumWithInteger::is_even() const
{
    return digits.empty() || ((digits[0] & 1) == 0);
}

auto Bezout(BigNumWithInteger x, BigNumWithInteger y)
{
    struct Ans
    {
        BigNumWithInteger a, b;
        BigNumWithInteger d;
    };

    // cerr << "x = " << x << endl;
    // cerr << "y = " << y << endl;

    if (x.is_zero())
    {
        return Ans{BigNumWithInteger("0"), BigNumWithInteger("1"), y};
    }
    if (y.is_zero())
    {
        return Ans{BigNumWithInteger("1"), BigNumWithInteger("0"), x};
    }

    BigNumWithInteger _x(x);
    BigNumWithInteger _y(y);

    x = x.abs();
    y = y.abs();

    int g = 0;
    int i = 0, j = 0;
    string x_binary = x.to_string();
    if (x_binary[0] == '-')
        x_binary = x_binary.substr(1); // x_binary.erase(0, 1);
    string y_binary = y.to_string();
    if (y_binary[0] == '-')
        y_binary = y_binary.substr(1); // y_binary.erase(0, 1);
    reverse(x_binary.begin(), x_binary.end());
    reverse(y_binary.begin(), y_binary.end());

    while (i < x_binary.size() && j < y_binary.size() && x_binary[i] == '0' && y_binary[i] == '0')
    {
        g++;
        i++;
        j++;
    }

    // cerr << "g = " << g << endl;
    // cerr << "i = " << i << endl;
    // cerr << "j = " << j << endl;

    // x = x.cut(i);
    x = x >> i;
    // y = y.cut(j);
    y = y >> j;

    // cerr << "u = " << x << endl;
    // cerr << "v = " << y << endl;
    // cerr << "g = " << g << endl;
    BigNumWithInteger u(x);
    BigNumWithInteger v(y);

    BigNumWithInteger A("1");
    BigNumWithInteger B("0");
    BigNumWithInteger C("0");
    BigNumWithInteger D("1");

    while (!u.is_zero())
    {
        while (!u.is_zero() && u.is_even())
        {
            u = u >> 1;
            if (A.is_even() && B.is_even())
            {
                A = A >> 1;
                B = B >> 1;
            }
            else
            {
                A = (A + y) >> 1;
                A %= y;
                B = (B - x) >> 1;
                B %= y;
            }
            // cerr << "\tu = " << u << endl;
        }

        while (!v.is_zero() && v.is_even())
        {
            v = v >> 1;
            if (C.is_even() && D.is_even())
            {
                C = C >> 1;
                D = D >> 1;
            }
            else
            {
                C = (C + y) >> 1;
                C %= y;
                D = (D - x) >> 1;
                D %= y;
            }
            // cerr << "\tv = " << v << endl;
        }

        // cerr << "u = " << u << endl;
        // cerr << "v = " << v << endl;

        if (u >= v)
        {
            u = u - v;
            A = A - C;
            B = B - D;
        }
        else
        {
            v = v - u;
            C = C - A;
            D = D - B;
        }
    }

    BigNumWithInteger a = C;
    BigNumWithInteger b = D;
    BigNumWithInteger d = v;
    d = d << g;

    a.set_sign(a.get_sign() * _x.get_sign());
    b.set_sign(b.get_sign() * _y.get_sign());

    // cerr << "x*a + b*y = " << x*a + b*y << endl;

    while (a < BigNumWithInteger(0))
    {
        a = a + y;
        b = b - x;
        // cerr << "x*a + b*y = " << x*a + b*y << endl;
    }
    return Ans{a, b, d};
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cerr << "Usage: ./main input_file output_file\n";
        return 0;
    }
    // cerr << argv[0] << " " << argv[1] << ' ' << argv[2] << ' ' << argc << endl;

    // cerr << "open file\n";
    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w", stdout);
    // cerr << "open file OK\n";

    // Give p, q, e
    // Need to find
    //  1. n   = p * q
    //  2. phi = (p - 1) * (q - 1)
    //  3. e * d + phi * x = 1

    // cout << "Start Program\n";
    string p_hex;
    cin >> p_hex;
    reverse(p_hex.begin(), p_hex.end());
    string q_hex;
    cin >> q_hex;
    reverse(q_hex.begin(), q_hex.end());
    string e_hex;
    cin >> e_hex;
    reverse(e_hex.begin(), e_hex.end());

    string p_binary = hex_to_bin(p_hex);
    string q_binary = hex_to_bin(q_hex);
    string e_binary = hex_to_bin(e_hex);

    BigNumWithInteger p(p_binary);
    BigNumWithInteger q(q_binary);
    BigNumWithInteger e(e_binary);

    // cerr << "input OK\n";
    BigNumWithInteger n = p * q;
    BigNumWithInteger phi = (p - BigNumWithInteger("1")) * (q - BigNumWithInteger("1"));

    // cerr << "p   = " << p << "\n";
    // cerr << "q   = " << q << "\n";
    // cerr << "e   = " << e << "\n";
    // cerr << "phi = " << phi << "\n";
    // cerr << "n   = " << n << "\n";

    auto [a, b, v] = Bezout(e, phi);
    // cerr << "v = " << v << endl;

    if (!v.is_one())
    {
        cout << "-1\n";
        return 0;
    }

    // cerr << "e*a + phi*b = " << e*a + phi*b << endl;

    string a_hex = bin_to_hex(a.to_string());
    reverse(a_hex.begin(), a_hex.end());
    cout << a_hex << endl;
}