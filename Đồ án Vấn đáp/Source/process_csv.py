import pandas as pd
import csv

def create_csv():
    # Dữ liệu về lương công nhân
    employee_data = [
        ["Tên", "Chức vụ", "Lương"],
        ["John", "Quản lý", 50000],
        ["Alice", "Nhân viên kỹ thuật", 42000],
        ["Bob", "Nhân viên sản xuất", 38000],
        ["Eva", "Nhân viên kinh doanh", 45000],
        ["David", "Nhân viên hỗ trợ", 40000]
    ]

    # Tên file CSV
    csv_file_path = '../Database/luong_cong_nhan.csv'

    # Ghi dữ liệu vào file CSV
    with open(csv_file_path, mode='w', newline='', encoding='utf-8') as file:
        writer = csv.writer(file)
        writer.writerows(employee_data)

    print(f'File CSV về lương công nhân đã được tạo thành công: {csv_file_path}')

def read_csv(file_path, column_index):
    try:
        df = pd.read_csv(file_path)

        if column_index < 0 or column_index >= len(df.columns):
            print(f'Chỉ mục cột không hợp lệ. Chỉ mục cột bắt đầu từ 0.')
            return

        column_name = df.columns[column_index]

        selected_column = df[column_name].astype(str)
        
        print("------- Read File CSV Successfully!   -------")

        return selected_column

    except FileNotFoundError:
        print(f"Không tìm thấy file CSV: {file_path}")
    except Exception as e:
        print(f"Có lỗi xảy ra: {e}")

def update_csv(file_path, column_index, new_values, output_file_path):
    try:
        df = pd.read_csv(file_path)

        if column_index < 0 or column_index >= len(df.columns):
            print(f'Chỉ mục cột không hợp lệ. Chỉ mục cột bắt đầu từ 0.')
            return

        if len(new_values) != len(df):
            print(f'Số lượng giá trị mới không khớp với số lượng dòng trong file CSV.')
            return

        column_name = df.columns[column_index]
        df[column_name] = new_values

        df.to_csv(output_file_path, index=False)

        print("------- Update File CSV Successfully! -------")

    except FileNotFoundError:
        print(f'Không tìm thấy file CSV: {file_path}')
    except Exception as e:
        print(f'Có lỗi xảy ra: {e}')