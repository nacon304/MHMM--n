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

def verify_signature(public_key_path, message_path, signature_path):
    with open(public_key_path, 'rb') as public_key_file:
        public_key_data = public_key_file.read()
        public_key = RSA.import_key(public_key_data)

    with open(message_path, 'rb') as message_file:
        message = message_file.read()

    with open(signature_path, 'rb') as signature_file:
        signature_data = signature_file.read()
        signature_int = int.from_bytes(signature_data, byteorder='big')

    em_len = public_key.size_in_bytes()
    padded_message = pkcs1v5_padding(message, em_len)
    message_int = int.from_bytes(padded_message, byteorder='big')
    decrypted_signature_int = fast_mod_exp(signature_int, public_key.e, public_key.n)

    if decrypted_signature_int == message_int:
        print("Signature Verified Successfully")
    else:
        print("Signature Verification Failure")

public_key_path = '../pub.pem'
message_path = '../mess.txt'
signature_path = '../sign.txt'

verify_signature(public_key_path, message_path, signature_path)