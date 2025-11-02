**BÀI THỰC HÀNH 14**

---

**BÀI SỐ 4: TASK IDLE TỰ ĐỘNG CHUYỂN VỀ SLEEP MODE SỬ DỤNG VỚI UART VÀ BLINK LED**

---

**A. QUY TRÌNH THỰC HIỆN:**

**1. GPIO_Init**

<img width="291" height="163" alt="image" src="https://github.com/user-attachments/assets/500b9526-7c70-4c84-a103-8e9d6e1b2f80" />

Bật clock cho cổng A, đặt PA0 làm output (LED).

**2. UART_Init**

<img width="408" height="161" alt="image" src="https://github.com/user-attachments/assets/69c798d5-4481-47e7-aa91-4dd42acd4bde" />

Bật clock USART1, cấu hình PA9 (TX), PA10 (RX), đặt baudrate 9600, bật TX+RX

**3. UART_SendString**

<img width="342" height="153" alt="image" src="https://github.com/user-attachments/assets/859f470b-2ec2-4afa-94a2-ba80ff454fb9" />

Gửi chuỗi ra UART, dùng polling, khóa scheduler khi gửi.

**4. vBlinkTask**

<img width="328" height="107" alt="image" src="https://github.com/user-attachments/assets/495e1524-e04f-424b-aac7-18a3bfe230c7" />

Task nháy LED mỗi 0.5 giây.

**5. vUARTTask**

<img width="539" height="111" alt="image" src="https://github.com/user-attachments/assets/014140eb-b417-4459-8c40-da9c6f163b00" />

Task gửi chuỗi qua UART mỗi 2 giây.

**6. vApplicationIdleHook**

<img width="287" height="78" alt="image" src="https://github.com/user-attachments/assets/d9d87ef4-9267-43d5-943f-3122624875cf" />

Khi CPU rảnh, vào chế độ Sleep (__WFI()).

**7. main**

<img width="472" height="163" alt="image" src="https://github.com/user-attachments/assets/bf0988c7-a092-4ae8-b2e8-2cffec3d7c5c" />

Gọi init, tạo 2 task, khởi chạy FreeRTOS scheduler.

---

**B. KẾT LUẬN:**

- LED nháy → UART in thông báo → CPU ngủ khi rảnh để tiết kiệm năng lượng.

- Chương trình hoạt động ổn định đôi khi do thời gian dãn chưa đủ lớn nên có thể bị nhiễu

---

_**Link demo: [Idle_Sleep](https://drive.google.com/file/d/1se6_Z_Y5luYfzLWhH-1S99_t1UVZEOhS/view?usp=drive_link)**_
