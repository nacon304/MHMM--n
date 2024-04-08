import random
from operator import mul
from functools import reduce
from Crypto.Util import number
from sympy.ntheory.modular import crt

def generate_large_prime(bit_length):
    prime_candidate = number.getPrime(bit_length)
    hex_representation = hex(prime_candidate)[2:]  # Bỏ qua ký tự '0x'
    return hex_representation

def hex_to_bit_length(hex_string):
    decimal_number = int(hex_string, 16)

    bit_length = decimal_number.bit_length()

    return bit_length

def generate_prime(column):
    crt_column = []

    for enc in column:
        bit_length_default = hex_to_bit_length(enc)

        while True:
            bit_length = bit_length_default + random.randint(1, 8)
            prime_hex = generate_large_prime(bit_length)

            if prime_hex not in crt_column:
                break

        crt_column.append(prime_hex)

    print("------- Generate Prime Successfully!  -------")

    return crt_column

def chinese_remainder_theorem(a_hex, m_hex):
    a = [int(x, 16) for x in a_hex]
    m = [int(x, 16) for x in m_hex]

    n = reduce(mul, m)

    Ni = [n // mi for mi in m]
    yi = [pow(Ni[i], -1, mi) for i, mi in enumerate(m)]

    x = sum(ai * Ni[i] * yi[i] for i, ai in enumerate(a)) % n
    result = hex(x)[2:]
    
    with open("../Key/CRT.txt", "w") as file:
        file.write(result)

    print("------- Calculate CRT Successfully!   -------")

    return result  # Trả về dạng hex mà không có tiền tố '0x'