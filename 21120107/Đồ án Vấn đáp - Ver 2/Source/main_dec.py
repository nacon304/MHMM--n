import sys
import time
from process_csv import read_csv
from decode import decrypt_row

if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Sử dụng: python main_dec.py <đường_dẫn_file_csv> <chỉ_mục_cột> <chỉ_mục_hàng> <pass>")
        sys.exit(1)

    file_path = sys.argv[1]

    try:
        column_index = int(sys.argv[2])
    except ValueError:
        print("Chỉ mục cột phải là một số nguyên.")
        sys.exit(1)

    try:
        row_index = int(sys.argv[3])
    except ValueError:
        print("Chỉ mục hàng phải là một số nguyên.")
        sys.exit(1)
    
    pass_user = sys.argv[4]

    # Nạp database
    col = read_csv(file_path, column_index)

    # Trả kết quả
    if pass_user != col[row_index - 1]:
        print("Truy cập không thành công!")
    else:
        print("Truy cập thành công!")

        start_time = time.time()
        print(f"Kết quả: {decrypt_row(len(col), row_index, pass_user)}")
        end_time = time.time()
        elapsed_time = end_time - start_time
        start_time = end_time
        print(f"Thời gian chạy: {elapsed_time} giây \n")

# python main_dec.py "../Database/Distribution Channel_encode.csv" 13 10
