# from Cryptodome.Cipher import ChaCha20
# from Cryptodome.Random import get_random_bytes
# import binascii

# def chacha20_encrypt(key, plaintext):
#     cipher = ChaCha20.new(key=key)
#     ciphertext = cipher.encrypt(plaintext)
#     return cipher.nonce + ciphertext

# def chacha20_decrypt(key, ciphertext):
#     nonce = ciphertext[:8]
#     ciphertext = ciphertext[8:]
#     cipher = ChaCha20.new(key=key, nonce=nonce)
#     plaintext = cipher.decrypt(ciphertext)
#     return plaintext

# # Tạo một khóa ngẫu nhiên
# key = get_random_bytes(32)

# # Lưu khóa vào tệp tin "key.txt"
# with open("../Key/key.txt", "wb") as file:
#     file.write(key)

# # Đoạn văn bản dài
# plaintext = "21120107".encode("utf-8")

# # Chia nhỏ đoạn văn bản thành các khối nhỏ hơn
# block_size = 16
# plaintext_blocks = [plaintext[i:i+block_size] for i in range(0, len(plaintext), block_size)]

# # Mã hóa từng khối nhỏ bằng ChaCha20
# ciphertext_blocks = []
# for block in plaintext_blocks:
#     block_bytes = block
#     ciphertext_block = chacha20_encrypt(key, block_bytes)
#     ciphertext_blocks.append(ciphertext_block)

# # Kết hợp các khối mã hóa thành một xâu mã hóa hoàn chỉnh
# ciphertext = b"".join(ciphertext_blocks)

# # Chuyển đổi xâu mã hóa thành dạng hex
# ciphertext_hex = binascii.hexlify(ciphertext).decode("utf-8")
# print("Ciphertext (hex):", ciphertext_hex)

# # Giải mã xâu đã được mã hóa
# ciphertext_bytes = binascii.unhexlify(ciphertext_hex)
# decrypted_text_blocks = []
# for i in range(0, len(ciphertext_bytes), block_size+8):
#     ciphertext_block = ciphertext_bytes[i:i+block_size+8]
#     decrypted_block = chacha20_decrypt(key, ciphertext_block)
#     decrypted_text_blocks.append(decrypted_block)

# # Kết hợp các khối giải mã thành một xâu văn bản hoàn chỉnh
# decrypted_text = b"".join(decrypted_text_blocks).decode("utf-8")
# print("Decrypted text:", decrypted_text)


import random
import numpy as np
import sympy as sp

def doc_va_tinh_gia_tri_hex_tai_tap_x(hex_numbers, tap_x):
    numbers = [int(h, 16) for h in hex_numbers]
    x = sp.symbols('x')
    
    # Tạo đa thức từ hệ số numbers
    polynomial = sum(coef * (x ** exp) for exp, coef in enumerate(numbers))
    
    # Tính giá trị đa thức tại từng x và chuyển sang chuỗi hex
    ket_qua_hex_tai_tap_x = [hex(int(polynomial.subs(x, x_val)))[2:] for x_val in tap_x]
    
    return ket_qua_hex_tai_tap_x

def vander_matrix(tap_x, increasing=True):
    n = len(tap_x)
    A = np.zeros((n, n), dtype=object)

    for i in range(n):
        for j in range(n):
            A[i, j] = tap_x[i] ** j if increasing else tap_x[i] ** (n - 1 - j)

    return sp.Matrix(A)

def giai_he_phuong_trinh(tap_x, B):
    x = sp.symbols('x0:' + str(len(B)))

    A = vander_matrix(tap_x, increasing=True)
    B = sp.Matrix([int(hex_str, 16) for hex_str in B])

    he_phuong_trinh = sp.Eq(A * sp.Matrix(x), B)

    nghiem = sp.solve(he_phuong_trinh, x)

    nghiem_hex = [hex(int(value))[2:] for value in nghiem.values()]

    return nghiem_hex

with open("../Key/CRT.txt", "r") as file:
    danh_sach_hex = file.readlines()
danh_sach_hex = [hex_str.strip() for hex_str in danh_sach_hex]

tap_x = random.sample(range(1000), len(danh_sach_hex))
ket_qua_hex_tai_tap_x = doc_va_tinh_gia_tri_hex_tai_tap_x(danh_sach_hex, tap_x)

nghiem_hex = giai_he_phuong_trinh(tap_x, ket_qua_hex_tai_tap_x)
