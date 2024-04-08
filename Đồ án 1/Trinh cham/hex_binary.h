#include <string>
#include<bitset>
#include<cmath>
using namespace std;

string hex_to_bin(string a_hex) {
    string a_bin;
    for (int i = 0; i < int(a_hex.size()); i++) {
        if (a_hex[i] >= '0' && a_hex[i] <= '9') {
            a_bin += bitset<4>(a_hex[i] - '0').to_string();
        } else {
            a_bin += bitset<4>(a_hex[i] - 'A' + 10).to_string();
        }
    }
    return a_bin;
}

string bin_to_hex(string a_bin) {
    string a_hex;
    for (int i = 0; i < int(a_bin.size()); i += 4) {
        int num = 0;
        for (int j = 0; j < 4; j++) {
            num += (a_bin[i + j] - '0') * pow(2, 3 - j);
        }
        if (num >= 0 && num <= 9) {
            a_hex += to_string(num);
        } else {
            a_hex += (char)(num - 10 + 'A');
        }
    }
    return a_hex;
}

string int_to_binary(int x) {
    string a_bin;
    string temp = "";
    if(x < 0) {
        temp = "-";
        x = -x;
    }
    while (x > 0) {
        a_bin = to_string(x % 2) + a_bin;
        x /= 2;
    }
    return temp + a_bin;
}