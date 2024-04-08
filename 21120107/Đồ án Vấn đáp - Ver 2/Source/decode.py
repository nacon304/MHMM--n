from Cryptodome.Cipher import ChaCha20
import binascii
import math
import random
import numpy as np
import sympy as sp

def chacha20_decrypt(key, ciphertext):
    nonce = ciphertext[:8]
    ciphertext = ciphertext[8:]
    cipher = ChaCha20.new(key=key, nonce=nonce)
    plaintext = cipher.decrypt(ciphertext)
    return plaintext

def mod_hex(a_hex, b_hex):
    a = int(a_hex, 16)
    b = int(b_hex, 16)
    result = a % b
    return hex(result)[2:]

def tinh_nhom_tai_vi_tri(pos, N):
    if N < 100:
        return 0
    
    so_luong_nhom = int(math.sqrt(N))
    so_luong_so_trong_nhom = N // so_luong_nhom
    so_thu_tu_nhom = min(pos // so_luong_so_trong_nhom, so_luong_nhom - 1)
    
    return so_thu_tu_nhom

def vander_matrix(tap_x, increasing=True):
    n = len(tap_x)
    A = np.zeros((n, n), dtype=object)

    for i in range(n):
        for j in range(n):
            A[i, j] = tap_x[i] ** j if increasing else tap_x[i] ** (n - 1 - j)

    return sp.Matrix(A)

def giai_he_phuong_trinh(tap_x, B):
    x = sp.symbols('x0:' + str(len(B)))

    X = sp.Matrix([int(hex_str, 16) for hex_str in tap_x])
    A = vander_matrix(X, increasing=True)
    B = sp.Matrix([int(hex_str, 16) for hex_str in B])

    he_phuong_trinh = sp.Eq(A * sp.Matrix(x), B)
    nghiem = sp.solve(he_phuong_trinh, x)
    nghiem_hex = [hex(int(value))[2:] for value in nghiem.values()]

    return nghiem_hex

def find_crt(length, row_index):
    # Tìm x
    with open("../Key/CRT.txt", "r") as file:
        crt = file.read()
    with open("../Key/prime_s2.txt", "r") as file:
        primes = file.readlines()
    primes = [hex_str.strip() for hex_str in primes]

    X = []
    for p in primes:
        X.append(mod_hex(crt, p))

    # Tìm crts
    with open("../Key/res.txt", "r") as file:
        Y = file.readlines()
    Y = [hex_str.strip() for hex_str in Y]
    crts = giai_he_phuong_trinh(X, Y)

    # Trả ra crt
    return crts[tinh_nhom_tai_vi_tri(row_index, length)]

def decrypt_row(length, row_index, encrypted_hex):
    with open("../Key/key.txt", "rb") as file:
        key = file.read()
    
    block_size = 32
    crt = find_crt(length, row_index)

    encrypted_value = mod_hex(crt, encrypted_hex)
    if len(encrypted_value) % 2 != 0:
        encrypted_value = '0' + encrypted_value

    # Giải mã xâu đã được mã hóa
    ciphertext_bytes = binascii.unhexlify(encrypted_value)
    decrypted_text_blocks = []
    for i in range(0, len(ciphertext_bytes), block_size+8):
        ciphertext_block = ciphertext_bytes[i:i+block_size+8]
        decrypted_block = chacha20_decrypt(key, ciphertext_block)
        decrypted_text_blocks.append(decrypted_block)

    # Kết hợp các khối giải mã thành một xâu văn bản hoàn chỉnh
    decrypted_text = b"".join(decrypted_text_blocks).decode("utf-8")

    return decrypted_text