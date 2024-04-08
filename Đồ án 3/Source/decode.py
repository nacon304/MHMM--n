from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import rsa, padding
from cryptography.hazmat.backends import default_backend

def decrypt_data(private_key_file, ciphertext_file, plaintext_file):
    with open(private_key_file, "rb") as key_file:
        private_key = serialization.load_pem_private_key(
            key_file.read(),
            password=None,  
            backend=default_backend()
        )

    with open(ciphertext_file, "rb") as cipher_file:
        ciphertext = cipher_file.read()

    plaintext = private_key.decrypt(
        ciphertext,
        padding.PKCS1v15()
    )

    with open(plaintext_file, "wb") as plain_file:
        plain_file.write(plaintext)

if __name__ == "__main__":
    private_key_file = "../priv.pem"
    ciphertext_file = "../cipher.txt"
    plaintext_file = "../plain_des.txt"

    decrypt_data(private_key_file, ciphertext_file, plaintext_file)