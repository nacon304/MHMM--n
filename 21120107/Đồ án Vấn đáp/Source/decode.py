from Cryptodome.Cipher import ChaCha20
import binascii

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

def decrypt_row(encrypted_column, encrypted_hex):
    with open("../Key/key.txt", "rb") as file:
        key = file.read()
    with open("../Key/CRT.txt", "rb") as file:
        crt = file.read()

    block_size = 32

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