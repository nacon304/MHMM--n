from Cryptodome.Random import get_random_bytes

def gen_key():
    key = get_random_bytes(32)

    with open("../Key/key.txt", "wb") as file:
        file.write(key)