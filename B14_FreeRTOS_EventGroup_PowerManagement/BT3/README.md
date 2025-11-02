**BÀI THỰC HÀNH 14**

---

**BÀI SỐ 3: SO SÁNH ĐIỆN NĂNG TIÊU THỤ KHI GỬI VÀ KHI STANDBY SỬ DỤNG UART**

---

**A. QUY TRÌNH THỰC HIỆN:**

**1. UART_Init():**

<img width="413" height="218" alt="image" src="https://github.com/user-attachments/assets/644f2eb7-5e9d-4fdd-b0c7-8ed51a58d699" />

Cấu hình UART1 để truyền dữ liệu:

Bật clock cho GPIOA và USART1.

PA9 → chân TX (AF Push-Pull, output 50 MHz).

PA10 → chân RX (Input floating).

Đặt tốc độ baud = 9600 bps.

Bật USART, TX, RX.

**2. UART_SendString:**

<img width="351" height="125" alt="image" src="https://github.com/user-attachments/assets/514cdf50-bb41-4733-bb71-d9c72e26efb6" />

Gửi chuỗi ký tự qua UART:

Gửi từng ký tự khi thanh ghi TX trống.

Chờ cho đến khi toàn bộ dữ liệu gửi xong.

**3. RTC_Init_Wakeup(seconds):**

<img width="559" height="333" alt="image" src="https://github.com/user-attachments/assets/cd12a801-0a4c-4dc8-8174-48b765998bcd" />

Cấu hình RTC (đồng hồ thời gian thực) để báo thức sau seconds giây:

Bật clock cho PWR và BKP.

Cho phép truy cập backup domain.

Bật thạch anh LSE (32.768 kHz).

Chọn LSE làm nguồn clock cho RTC.

Prescaler = 32767 → tick mỗi 1 s.

Đặt báo thức sau seconds giây.

Bật ngắt báo thức.

Khóa lại backup domain.

**4. Enter_Standby_Mode:**

<img width="275" height="105" alt="image" src="https://github.com/user-attachments/assets/12e736bc-9a70-406d-94f6-91803d5e8d26" />

Đưa MCU vào chế độ Standby (ngủ sâu):

Xóa cờ wake-up.

Chọn chế độ Standby (PDDS = 1).

Bật Deep Sleep.

Lệnh __WFI() → CPU dừng hoạt động, tiêu thụ cực thấp.

**5. main:**

<img width="500" height="251" alt="image" src="https://github.com/user-attachments/assets/beae71a7-766a-48c0-ba6e-9efed8d02b42" />

Luồng chính của chương trình:

Bật clock cho PWR.

Kiểm tra nếu vừa thức dậy từ Standby → gửi “Woke up from Standby!”.

Nếu khởi động lần đầu → gửi “First boot...”.

Cấu hình RTC báo thức sau 5 giây.

Gửi thông báo “Going to Standby...”.

Vào Standby để chờ RTC đánh thức.

---

**B. KẾT LUẬN:**

- MCU khởi động → gửi chuỗi UART → ngủ trong 5 giây → RTC đánh thức → gửi chuỗi “Woke up from Standby!”.

- Hoạt động ổn định đôi lúc bị delay do phần cứng

---

_**Link demo: [UART_Standby](https://drive.google.com/file/d/1g7c5CqakCstw6SpEBVzKoLL6gzaalbB0/view?usp=drive_link)**_
