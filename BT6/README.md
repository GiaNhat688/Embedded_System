BÀI THỰC HÀNH 6

GIAO TIẾP I2C

A. CHƯƠNG TRÌNH SỬ DỤNG STM32F103C8T6 ĐỂ CẤU HÌNH I2C GIAO TIẾP ĐƯỢC VỚI CẢM BIẾN VÀ HIỂN THỊ DỮ LIỆU LÊN TERMINAL

1. Phần cứng:
- Stm32f103c8t6 x1
- Module DS1307+AT24C32 x1
- ST-Link V2 x1
- Các dây nối jumper

2. Phần mềm:
- KeilC
- Hercules

B. QUY TRÌNH THỰC HIỆN:

1. Khai báo và định nghĩa:

<img width="546" height="93" alt="image" src="https://github.com/user-attachments/assets/25f909a8-cfbd-4347-8609-0c934af62ce0" />

- Gọi thư viện chuẩn STM32F10x và stdio để dùng sprintf.
- Định nghĩa địa chỉ I2C của DS1307 (ghi = 0xD0, đọc = 0xD1).

2. USART1 (UART):

2.1. Khởi tạo USART1:

<img width="577" height="324" alt="image" src="https://github.com/user-attachments/assets/080df256-5bfe-4710-a061-9a470ac74b32" />

- Chức năng: Cấu hình USART1 để in dữ liệu ra Serial (PA9 = TX, PA10 = RX).

2.2. Gửi dữ liệu:

<img width="368" height="190" alt="image" src="https://github.com/user-attachments/assets/e50ce561-4d55-40d6-8146-dcb2a30c070a" />

- Chức năng: Gửi một ký tự hoặc chuỗi ra cổng Serial.

3. I2C1:

3.1. Khởi tạo I2C1:

<img width="642" height="373" alt="image" src="https://github.com/user-attachments/assets/977b809c-9c06-440e-8324-a31ec0147d06" />

- Chức năng: Cấu hình I2C1 trên PB6 (SCL), PB7 (SDA) để giao tiếp với DS1307.

3.2. Các thao tác I2C cơ bản:

<img width="483" height="387" alt="image" src="https://github.com/user-attachments/assets/02e8ad5f-3762-42f5-bf96-295bf66e8578" />

- Chức năng: Thao tác cơ bản để START, STOP, gửi địa chỉ, gửi dữ liệu qua I2C.

3.3. Đọc dữ liệu từ I2C:

<img width="486" height="215" alt="image" src="https://github.com/user-attachments/assets/bbeb2494-5957-45a7-bfe6-cbc795f1d985" />

- Chức năng: Đọc 1 byte từ Slave (DS1307).

4. Đọc/Ghi thanh ghi DS1307:

