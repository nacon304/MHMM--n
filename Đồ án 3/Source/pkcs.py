from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization

def read_key_file(file_path, is_private=True):
    with open(file_path, "rb") as key_file:
        if is_private:
            key = serialization.load_pem_private_key(key_file.read(), password=None, backend=default_backend())
        else:
            key = serialization.load_pem_public_key(key_file.read(), backend=default_backend())

    return key

def print_key_components(key, is_private=True):
    if is_private:
        print("RSA Private Key:")
        private_numbers = key.private_numbers()
        public_numbers = private_numbers.public_numbers
    else:
        print("RSA Public Key:")
        public_numbers = key.public_numbers()

    print("Modulus (n):")
    print(hex(public_numbers.n))

    if is_private:
        print("Private Exponent (d):")
        print(hex(private_numbers.d))

    print("Public Exponent (e):")
    print(hex(public_numbers.e))

    if is_private:
        print("Prime 1 (p):")
        print(hex(private_numbers.p))
        print("Prime 2 (q):")
        print(hex(private_numbers.q))
        print("Exponent 1 (dp):")
        print(hex(private_numbers.dmp1))
        print("Exponent 2 (dq):")
        print(hex(private_numbers.dmq1))
        print("Coefficient (qi):")
        print(hex(private_numbers.iqmp))

    print()

def save_key_components_to_file(file_path, key, is_private=True):
    with open(file_path, "w") as output_file:
        if is_private:
            output_file.write("RSA Private Key:\n")
            private_numbers = key.private_numbers()
            public_numbers = private_numbers.public_numbers
        else:
            output_file.write("RSA Public Key:\n")
            public_numbers = key.public_numbers()

        output_file.write("Modulus (n):\n")
        output_file.write(hex(public_numbers.n) + "\n")

        if is_private:
            output_file.write("Private Exponent (d):\n")
            output_file.write(hex(private_numbers.d) + "\n")

        output_file.write("Public Exponent (e):\n")
        output_file.write(hex(public_numbers.e) + "\n")

        if is_private:
            output_file.write("Prime 1 (p):\n")
            output_file.write(hex(private_numbers.p) + "\n")
            output_file.write("Prime 2 (q):\n")
            output_file.write(hex(private_numbers.q) + "\n")
            output_file.write("Exponent 1 (dp):\n")
            output_file.write(hex(private_numbers.dmp1) + "\n")
            output_file.write("Exponent 2 (dq):\n")
            output_file.write(hex(private_numbers.dmq1) + "\n")
            output_file.write("Coefficient (qi):\n")
            output_file.write(hex(private_numbers.iqmp) + "\n")

        output_file.write("\n")

if __name__ == "__main__":
    private_key_path = "..\priv.pem"
    public_key_path = "..\pub.pem"

    private_key = read_key_file(private_key_path)
    public_key = read_key_file(public_key_path, is_private=False)

    print_key_components(private_key, is_private=True)
    print_key_components(public_key, is_private=False)

    save_key_components_to_file("priv.txt", private_key, is_private=True)
    save_key_components_to_file("pub.txt", public_key, is_private=False)
