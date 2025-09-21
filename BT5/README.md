**BÀI THỰC HÀNH 5**

**TRUYỀN THÔNG NỐI TIẾP USART**

**A. CHƯƠNG TRÌNH SỬ DỤNG STM32F103C8T6 VÀ USB TO TTL ĐỂ SỬ DỤNG USART HIỂN THỊ CHUỖI KÝ TỰ LÊN MÁY TÍNH VẦ BẬT/TẮT LED**

**1. Phần cứng:**
- STM32F103C8T6 x1
- USB TO TTL x1
- ST-Link V2 x1
- Dây cái cái x8

**2. Phần mềm:**
- KeilC (nạp code)
- Hercules (terminal)

**3. Kết quả mong đợi:**
- Hiển thị được chuỗi "Hello from STM32!" lên terminal trên ứng dụng Hercules
- Bật/tắt led thông qua lệnh ON/OFF

**B. QUY TRÌNH THỰC HIỆN:**

**1. Cấu hình chân cho LED và USART:**

<img width="339" height="308" alt="image" src="https://github.com/user-attachments/assets/fa3b45fe-7fa8-49a2-8986-315fe9fba9e4" />

**2. Cấu hình USART2:**

<img width="387" height="93" alt="image" src="https://github.com/user-attachments/assets/95ef6d43-db44-4140-9abf-78f844d2a893" />

- Cấu hình Baud rate (Tốc độ truyền dữ liệu) là 9600 theo công thức tính toán dưới đây:

     <img width="486" height="74" alt="image" src="https://github.com/user-attachments/assets/5c21db1d-ad6b-47f7-b8a5-ad498cffcd72" />

- Bật USART2, chức năng Transmitter và Receiver

**3. Gửi từng ký tự để tạo thành chuỗi**

<img width="306" height="185" alt="image" src="https://github.com/user-attachments/assets/d36627f1-5534-45e6-8092-f30bdefd8104" />

**4. Lấy ký tự để bật/tắt LED:**

<img width="291" height="92" alt="image" src="https://github.com/user-attachments/assets/af6d773c-6ebb-44ad-ba5c-324382a6d6b8" />

**5. Thực thi chương trình:**

<img width="340" height="556" alt="image" src="https://github.com/user-attachments/assets/4f4b6ae6-3812-40c8-9010-9cb826976d7b" />

**C. KẾT LUẬN:**

**1. Đánh giá kết quả đạt được:**
- Hiển thị chuỗi khá chính xác
- Bật/tắt led đúng như yêu cầu
- Đôi khi bị delay do phần cứng với baud rate có thể chưa chính xác

**2. Hướng cải tiến:**
- Có thể cấu hình chi tiết các thành phần như word lengh, parity, stop bit, ... để tương thích với nhiều yêu cầu hơn
