from cryptography.hazmat.primitives import serialization, hashes
from cryptography.hazmat.primitives.asymmetric import rsa, padding
from cryptography.hazmat.backends import default_backend

def encrypt_data(public_key_file, plaintext_file, ciphertext_file):
    # Đọc khóa công khai từ tệp pub.pem
    with open(public_key_file, "rb") as key_file:
        public_key = serialization.load_pem_public_key(
            key_file.read(),
            backend=default_backend()
        )

    # Đọc dữ liệu từ tệp bản rõ
    with open(plaintext_file, "rb") as plain_file:
        plaintext = plain_file.read()

    # Mã hóa dữ liệu với khóa công khai
    ciphertext = public_key.encrypt(
        plaintext,
        padding.PKCS1v15(),
    )

    # Ghi bản mã vào tệp cipher
    with open(ciphertext_file, "wb") as cipher_file:
        cipher_file.write(ciphertext)

if __name__ == "__main__":
    public_key_file = "../pub.pem"
    plaintext_file = "../plain.txt"
    ciphertext_file = "../cipher.txt"

    encrypt_data(public_key_file, plaintext_file, ciphertext_file)