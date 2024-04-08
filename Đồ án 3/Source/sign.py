from Crypto.PublicKey import RSA

def fast_mod_exp(base, exponent, modulus):
    result = 1
    base = base % modulus

    while exponent > 0:
        if exponent & 1:
            result = (result * base) % modulus
        exponent = exponent >> 1
        base = (base * base) % modulus

    return result

def pkcs1v5_padding(message, em_len):
    ps_len = em_len - len(message) - 3
    ps = b'\xff' * ps_len
    em = b'\x00\x01' + ps + b'\x00' + message
    return em

def sign_message(private_key_path, message_path, signature_path):
    with open(private_key_path, 'rb') as private_key_file:
        private_key_data = private_key_file.read()
        private_key = RSA.import_key(private_key_data)

    with open(message_path, 'rb') as message_file:
        message = message_file.read()

    em_len = private_key.size_in_bytes()
    padded_message = pkcs1v5_padding(message, em_len)
    message_int = int.from_bytes(padded_message, byteorder='big')
    signature_int = fast_mod_exp(message_int, private_key.d, private_key.n)

    signature_bytes = signature_int.to_bytes((signature_int.bit_length() + 7) // 8, byteorder='big')
    with open(signature_path, 'wb') as signature_file:
        signature_file.write(signature_bytes)

private_key_path = '../priv.pem'
message_path = '../mess.txt'
signature_path = '../sign.txt'

sign_message(private_key_path, message_path, signature_path)