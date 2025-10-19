**BÁO CÁO THỰC HÀNH**

**QUẢN LÝ TRẠNG THÁI TASK DÙNG QUEUE TRONG FREERTOS SỬ DỤNG VI ĐIỀU KHIỂN STM32F103C8T6**

---

**CHƯƠNG TRÌNH SỬ DỤNG QUEUE TRONG FREERTOS ĐỂ NHẤP NHÁY LED TẠI CÁC TẦN SỐ KHÁC NHAU**

---

**A. QUY TRÌNH THỰC HIỆN:**

**1. Khai báo & include:**

<img width="200" height="73" alt="image" src="https://github.com/user-attachments/assets/2a1f38ae-190e-4fb8-9f47-398a8b23937e" />

→ Gọi thư viện thanh ghi STM32 và các file cần cho FreeRTOS (Task, Queue).

**2. Cấu trúc dữ liệu:**

<img width="160" height="73" alt="image" src="https://github.com/user-attachments/assets/88185262-3604-4887-8075-042183816c39" />

→ Gói tần số (freq) và độ rộng xung (duty) để gửi qua hàng đợi (Queue).

**3. Hàm cấu hình GPIO:**

<img width="350" height="102" alt="image" src="https://github.com/user-attachments/assets/10f7c2ea-c467-45ff-8a51-3dc33db57c94" />

→ Bật clock cho PORTA và cấu hình PA5 làm output (LED).

**4. Task 1 – vTask_LED:**

<img width="576" height="283" alt="image" src="https://github.com/user-attachments/assets/841d9bcc-c566-4512-ae86-771560608202" />

→ Nhận dữ liệu freq & duty từ Queue,
→ Tính chu kỳ và thời gian bật/tắt LED,
→ Chớp LED theo tần số và duty đó.

**5. Task 2 – vTask_Generator:**

<img width="461" height="241" alt="image" src="https://github.com/user-attachments/assets/d8129e39-bc77-4764-876b-dd7e7b89d4a2" />

→ Mỗi 1 giây thay đổi cặp (tần số, duty)
→ Gửi dữ liệu đó sang Queue cho Task LED dùng.

**6. main():**

<img width="492" height="175" alt="image" src="https://github.com/user-attachments/assets/3ef47ec2-82d0-40a4-8752-cabc4765f5b9" />

→ Cấu hình GPIO
→ Tạo Queue (lưu tối đa 5 phần tử)
→ Tạo 2 Task
→ Bắt đầu FreeRTOS Scheduler để cả 2 Task chạy song song.

---

**B. TỔNG KẾT:**

- Task 1: đọc dữ liệu → blink LED.
- Task 2: tạo dữ liệu → gửi sang Queue.
→ LED sẽ nhấp nháy với tần số và duty thay đổi liên tục theo thời gian.

---

**C. HƯỚNG PHÁT TRIỂN:**

**1. Nâng cấp điều khiển LED:**
- Dùng PWM thật (TIMx) thay vì vTaskDelay → LED sáng mượt, chính xác tần số & duty.
- Task 1 chỉ cần cập nhật TIMx->ARR (chu kỳ) và TIMx->CCR1 (duty).
- Có thể mở rộng điều khiển động cơ DC, servo, LED RGB.

**2. Truyền thông giữa các Task:**
- Thêm UART Task để gửi thông tin freq & duty ra terminal (debug hoặc hiển thị).
- Thêm Task nhận lệnh UART → thay đổi tần số/duty theo người dùng nhập.
→ Ví dụ: gõ “F5 D50” → tần số 5Hz, duty 50%.

---

_**Link mô phỏng của nhóm: [FreeRTOS_Queue](https://drive.google.com/drive/folders/1fJC_7aZNZXzkxWVUYC7e5eSyV4-Zhvk2)**_
