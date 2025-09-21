<img width="255" height="271" alt="image" src="https://github.com/user-attachments/assets/2bd75d82-6a0d-4c56-9603-1b4ef7e10dea" />**BÀI THỰC HÀNH 5**

**TRUYỀN THÔNG NỐI TIẾP USART**

A. CHƯƠNG TRÌNH SỬ DỤNG STM32F103C8T6 VÀ USB TO TTL ĐỂ SỬ DỤNG USART HIỂN THỊ CHUỖI KÝ TỰ LÊN MÁY TÍNH

1. Phần cứng:
- STM32F103C8T6 x1
- USB TO TTL x1
- ST-Link V2 x1
- Dây cái cái x8

2. Phần mềm:
- KeilC (nạp code)
- Hercules (terminal)

3. Kết quả mong đợi:
- Hiển thị được chuỗi "Hello from STM32!" lên terminal trên ứng dụng Hercules

B. QUY TRÌNH THỰC HIỆN:

1. Khai báo thư viện và cấu hình hàm delay bằng Timer2:

<img width="267" height="285" alt="image" src="https://github.com/user-attachments/assets/04d13a94-d383-4753-add9-01aa03040b67" />

- Tương tự như ở bài thực hành 4

2. Cấu hình USART2:

<img width="255" height="271" alt="image" src="https://github.com/user-attachments/assets/7a0cbc02-727b-49a9-8bc1-706de9169ac2" />

- Bật Clock cho GPIOA (sử dụng chân PA2-TX và PA3-RX) và USART2 để tương thích với 2 chân kia
- Cấu hình PA2 (Transmiss) ở chế độ Alternate Function Push-Pull vì sử dụng ngoại vi USART với tốc độ 50MHZ
- Cầu hình PA3 (Receive) ở chế độ Input with pull-up/pull-down với tốc độ là 50MHZ
- Cấu hình Baud rate (Tốc độ truyền dữ liệu) là 9600 theo công thức tính toán dưới đây:
     <img width="486" height="74" alt="image" src="https://github.com/user-attachments/assets/5c21db1d-ad6b-47f7-b8a5-ad498cffcd72" />

- Bật USART2, chức năng Transmitter và Receiver

3. 
