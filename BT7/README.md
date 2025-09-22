**BÀI THỰC HÀNH 7**

**HIỂU VÀ TRIỂN KHAI GIAO THỨC SPI**

**A. CHƯƠNG TRÌNH SỬ DỤNG STM32F103C8T6 DÙNG GIAO TIẾP SPI ĐỂ GIAO TRIẾP VỚI CẢM BIẾN ADXL345 HIỂN THỊ DỮ LIỆU CỦA CẢM BIẾN LÊN TERMINAL**

**1. Phần cứng:**
- Stm32f103c8t6 x1
- ADXL345 x1
- ST-Link V2 x1
- Các dây nối jumper

**2. Phần mềm:**
- KeilC
- Hercules

**3. Kết quả mong đợi:**
- Hiển thị được chính xác giá trị của dữ liệu của cảm biến lên terminal

**B. QUY TRÌNH THỰC HIỆN:**

**1. Thư viện và macro (khai báo đầu):**

<img width="547" height="141" alt="image" src="https://github.com/user-attachments/assets/57484388-ac52-49a5-9808-8ac398efe8fa" />

- stm32f10x.h: định nghĩa thanh ghi STM32.
- stdio.h: dùng sprintf.
- CS_LOW/HIGH: điều khiển chân CS (PA4) bằng BSRR.
- ADXL345_DEVID: địa chỉ thanh ghi ID (0x00).

**2. Hàm delay cơ bản:**

<img width="412" height="87" alt="image" src="https://github.com/user-attachments/assets/b3ee4ec4-328f-4755-b2b4-66cc8d541336" />

- Sử dụng vòng lặp trống đơn giản

**3. USART:**

**3.1. Cấu hình USART1:**

<img width="578" height="262" alt="image" src="https://github.com/user-attachments/assets/c706270d-0195-467c-b0bd-5919af2ac44c" />

- Bật clock cho GPIOA và USART1.
- Cấu hình chân:
    - PA9 (TX) → Alternate function push-pull.
    - PA10 (RX) → Input floating.
- Tốc độ baud: 9600 (BRR=0x1D4C nếu CPU 72MHz).
- Bật UART, TX, RX trong thanh ghi CR1.

**3.2. Các hàm gửi ký tự và chuỗi:**

<img width="359" height="172" alt="image" src="https://github.com/user-attachments/assets/c8e29b7c-3352-48ba-822f-c2eb5e2a6704" />

- USART1_SendChar: gửi 1 ký tự (chờ TXE).

- USART1_SendString: gửi cả chuỗi.

**4. SPI1:**

**4.1. Cầu hình SPI1:**

<img width="566" height="481" alt="image" src="https://github.com/user-attachments/assets/59e6b4b4-3d6b-4f0b-9bf3-5dacd8418fcd" />

- Bật clock GPIOA + SPI1.
- Cấu hình các chân PA4–PA7 theo chuẩn SPI.
- Đặt CS = 1 lúc khởi tạo.
- CR1: bật SPI, chọn Master, tốc độ f/16, dùng phần mềm quản lý CS, mode0 (CPOL=0, CPHA=0).

**4.2. Truyền và nhận dữ liệu:**

<img width="435" height="131" alt="image" src="https://github.com/user-attachments/assets/8d9d301c-7bf2-43f5-b9e9-5009ee363d7a" />

- SPI là full-duplex: khi gửi 1 byte thì cũng nhận về 1 byte.
- Trình tự: chờ TXE → ghi dữ liệu → chờ RXNE → đọc dữ liệu trả về.

**5. Giao tiếp ADXL345:**

<img width="480" height="164" alt="image" src="https://github.com/user-attachments/assets/9de183b0-eb64-4d9c-870a-402edcd52334" />

- ADXL345_Read(reg):
    - Hạ CS.
    - Gửi reg | 0x80 (bit7=1 để đọc).
    - Gửi 1 byte dummy (0x00) để nhận dữ liệu từ slave.
    - Nâng CS.
- Thanh ghi 0x00 chứa Device ID (thường = 0xE5).

**6. Hàm main:**

<img width="499" height="259" alt="image" src="https://github.com/user-attachments/assets/28fab9e3-af83-446f-8532-44c5773c22f4" />

- Gọi USART1_Init() và SPI1_Init().
- Trong vòng lặp:
    - Đọc ADXL345_DEVID.
    - In ra UART: "ADXL345 ID: 0xE5" hoặc "ADXL345 ID: 0x00" nều như không nhận được tín hiệu.
    - Delay 1 giây rồi lặp lại.

**C. KẾT LUẬN:**
**1. Đánh giá kết quả đạt được:**
- Chưa hoàn thiện lắm do cảm biến có thể bị sai số do yếu tô phần cứng nên hiển thị đôi lúc không chính xác.

**2. Hướng cải tiến:**
- Nếu nhóm em thay bằng cảm biến nhiệt độ hay ánh sáng thì có thể hiển thị chĩnh xác hơn.
