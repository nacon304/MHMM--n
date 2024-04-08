from Cryptodome.Cipher import ChaCha20
import binascii

def chacha20_encrypt(key, plaintext):
    cipher = ChaCha20.new(key=key)
    ciphertext = cipher.encrypt(plaintext)
    return cipher.nonce + ciphertext

def encrypt_data(plaintext, key):
    # Chia nhỏ đoạn văn bản thành các khối nhỏ hơn
    block_size = 32
    plaintext_blocks = [plaintext[i:i+block_size] for i in range(0, len(plaintext), block_size)]

    # Mã hóa từng khối nhỏ bằng ChaCha20
    ciphertext_blocks = []
    for block in plaintext_blocks:
        block_bytes = block
        ciphertext_block = chacha20_encrypt(key, block_bytes)
        ciphertext_blocks.append(ciphertext_block)

    # Kết hợp các khối mã hóa thành một xâu mã hóa hoàn chỉnh
    ciphertext = b"".join(ciphertext_blocks)

    # Chuyển đổi xâu mã hóa thành dạng hex
    ciphertext_hex = binascii.hexlify(ciphertext).decode("utf-8")

    return ciphertext_hex

def encrypt_column(column):
    with open("../Key/key.txt", "rb") as file:
        key = file.read()

    encrypted_column = []
    for value in column:
        ciphertext = encrypt_data(value.encode("utf-8"), key)
        encrypted_column.append(ciphertext)

    print("------- Encode Database Successfully! -------")

    return encrypted_column