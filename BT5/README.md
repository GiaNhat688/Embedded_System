**BÀI THỰC HÀNH 5**

**TRUYỀN THÔNG NỐI TIẾP USART**

**A. CHƯƠNG TRÌNH SỬ DỤNG STM32F103C8T6 VÀ USB TO TTL ĐỂ SỬ DỤNG USART HIỂN THỊ CHUỖI KÝ TỰ LÊN MÁY TÍNH**

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

**B. QUY TRÌNH THỰC HIỆN:**

**1. Cấu hình chân :**

<img width="326" height="274" alt="image" src="https://github.com/user-attachments/assets/f1343a23-9f1c-481a-bebc-dd16b502cdd2" />

**2. Cấu hình USART2:**

<img width="255" height="271" alt="image" src="https://github.com/user-attachments/assets/7a0cbc02-727b-49a9-8bc1-706de9169ac2" />

- Bật Clock cho GPIOA (sử dụng chân PA2-TX và PA3-RX) và USART2 để tương thích với 2 chân kia
- Cấu hình PA2 (Transmiss) ở chế độ Alternate Function Push-Pull vì sử dụng ngoại vi USART với tốc độ 50MHZ
- Cầu hình PA3 (Receive) ở chế độ Input with pull-up/pull-down với tốc độ là 50MHZ
- Cấu hình Baud rate (Tốc độ truyền dữ liệu) là 9600 theo công thức tính toán dưới đây:
     <img width="486" height="74" alt="image" src="https://github.com/user-attachments/assets/5c21db1d-ad6b-47f7-b8a5-ad498cffcd72" />

- Bật USART2, chức năng Transmitter và Receiver

**3. Gửi từng ký tự:**

<img width="311" height="84" alt="image" src="https://github.com/user-attachments/assets/c6a16bff-6e53-4909-bb69-2ac8bd86f0e5" />

- Để hiển thị được một chuỗi thì cần hiển thị từng ký tự trong chuỗi đó
- Chờ cho đến khi thanh ghi TXE được set có nghĩa là dữ liệu trước đã được truyền đến shift register thì có thể thêm dữ liệu mới vào
- Sau đó thêm ký tự mới từ chuỗi vào thanh ghi DR

**4. Gửi chuỗi:**

<img width="290" height="91" alt="image" src="https://github.com/user-attachments/assets/91b6b012-03b5-40da-846b-310400c2f93e" />

- Lấy địa chỉ của từng ký tự trong chuỗi cho đến cuối chuỗi thì ta sẽ có được một chuỗi hoàn chỉnh

**5. Thực thi chương trình:**

<img width="388" height="162" alt="image" src="https://github.com/user-attachments/assets/caa1ef51-e0ed-4bd2-a798-f80812ef5d08" />

- Gọi hàm cấu hình USART và TIM2 trước đó
- Gửi chuỗi cần hiển thị ra và chờ 1s sau mỗi lần gửi để trỏ đến đầu dòng tiếp theo để gủi tiếp chuỗi tiếp theo

**C. KẾT LUẬN:**

**1. Đánh giá kết quả đạt được:**
- Hiển thị chuỗi khá chính xác
- Đôi khi bị delay do phần cứng với baud rate có thể chưa chính xác

**2. Hướng cải tiến:**
- Có thể cấu hình chi tiết các thành phần như word lengh, parity, stop bit, ... để tương thích với nhiều yêu cầu hơn
