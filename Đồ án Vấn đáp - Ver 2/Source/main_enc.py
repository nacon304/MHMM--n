import sys
import time
from process_csv import read_csv, update_csv
from generate_key import gen_key
from encode import encrypt_column
from crt_algo import generate_prime, chinese_remainder_theorem_step1, chinese_remainder_theorem_step2
from equation import gen_x

start_time = time.time()
end_time = start_time

def print_time():
    global start_time, end_time
    
    end_time = time.time()
    elapsed_time = end_time - start_time
    start_time = end_time
    print(f"Thời gian chạy: {elapsed_time} giây \n")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Sử dụng: python main_enc.py <đường_dẫn_file_csv> <chỉ_mục_cột>")
        sys.exit(1)

    file_path = sys.argv[1]

    try:
        column_index = int(sys.argv[2])
    except ValueError:
        print("Chỉ mục cột phải là một số nguyên.")
        sys.exit(1)

    # Nạp database
    col = read_csv(file_path, column_index)
    print_time()

    # Tạo khóa
    gen_key()

    # Mã hóa
    en_c = encrypt_column(col)
    print_time()

    # CRT
    crt_c = generate_prime(en_c)
    print_time()

    crts = chinese_remainder_theorem_step1(en_c, crt_c)
    print_time()

    # Ánh xạ qua tập x
    x_hex = gen_x(crts)
    
    prime_x = generate_prime(x_hex)
    with open("../Key/prime_s2.txt", "w") as file:
        for p in prime_x:
            file.write(p + "\n")

    crt_x = chinese_remainder_theorem_step2(x_hex, prime_x)
    print_time()

    # Cập nhật CSDL
    output_file = file_path.replace('.csv', '_encode.csv')
    update_csv(file_path, column_index, crt_c, output_file)
    print_time()

# python main_enc.py "../Database/Distribution Channel.csv" 13