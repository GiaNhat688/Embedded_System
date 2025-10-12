**BÀI THỰC HÀNH 10**

**KHỞI TẠO TASK SỬ DỤNG FREERTOS**

**CHƯƠNG TRÌNH SỬ DỤNG STM32F103C8T6 KẾT HỢP VỚI FREERTOS ĐỂ QUẢN LÝ CÁC TASK THỰC HIỆN NHẤP NHÁY LED VỚI 3 CHÂN TƯƠNG ỨNG VỚI 3 TẦN SỐ KHÁC NHAU LẦN LƯỢT LÀ 3HZ, 10HZ, 25HZ**
---
**I. SỬ DỤNG 3 HÀM ĐỘC LẬP CHO 3 TÁC VỤ:**

**A. QUY TRÌNH THỰC HIỆN**

**1. Cấu hình GPIO:**

<img width="498" height="218" alt="image" src="https://github.com/user-attachments/assets/4a03d873-e79f-4652-b1a2-85b702a590dd" />

- Cấu hình PA1, PA2, PA3 là output push-pull tốc độ 50 MHz.

- Tắt LED lúc khởi tạo (ODR &= ~...).

**2. Task01 / Task02 / Task03:**

Công thức để tính thời gian thực thi chia làm 2 giai đoạn:
- Giai đoạn 1: Lúc bật Led thì -> T/2
- Giai đoạn 2: Lúc tắt Led thì -> T/2 còn lại
  --> Se chuan mot chu ky hoan chinh
  --> Công thức: T(exe) = 1000 / (f * 2) (ms)

<img width="384" height="433" alt="image" src="https://github.com/user-attachments/assets/75e9d8c4-eb85-4afa-a3ac-63b3114adb5d" />

- Mỗi task nháy 1 LED:

  - Task01 → PA1, 3 Hz

  - Task02 → PA2, 10 Hz

  - Task03 → PA3, 25 Hz

- vTaskDelay(ms) làm task dừng trong khoảng thời gian delay để tạo tần số nháy.

**3. Hàm thực thi:**

<img width="496" height="326" alt="image" src="https://github.com/user-attachments/assets/dc394ee1-1fbf-4ffb-8fc6-70ad919d05b0" />

- Bật HSI (8 MHz) và reset hệ thống clock.

- Khởi tạo GPIO.

- Tạo 3 task nháy LED.

- Bắt đầu scheduler FreeRTOS với vTaskStartScheduler().

**B. KẾT LUẬN:**
- Mỗi LED được điều khiển bởi 1 task riêng, dùng FreeRTOS để chạy song song và vTaskDelay để tạo tần số nháy khác nhau.
- Thời gian và tốc độ của led khá chính xác tuy nhiên vẫn cần cải thiện bằng các nguồn khác.

**II. SỬ DỤNG 1 HÀM TÁC VỤ DUY NHẤT VÀ TRUYỀN VÀO THAM SỐ ĐỂ THAY ĐỔI CHÂN NHÁY LED, TẦN SỐ NHÁY:**

**A. QUY TRÌNH THỰC HIỆN:**

**1. LED_Param_t: struct chứa thông tin LED**

<img width="181" height="170" alt="image" src="https://github.com/user-attachments/assets/e8ec7e98-1cf6-4164-bcf4-b5d639d7897e" />

- pin: chân GPIO của LED

- delay: thời gian giữa các lần nháy (ms)

**2. GPIO_Init():**

<img width="359" height="137" alt="image" src="https://github.com/user-attachments/assets/6d4b2348-5ab2-4b3a-bdc2-6ace959d3770" />

- Cấu hình PA1, PA2, PA3 là output 50 MHz.

- Tắt LED lúc khởi tạo.

**3. LED_Task():**

<img width="412" height="153" alt="image" src="https://github.com/user-attachments/assets/ceb4aea8-f471-4a94-bd02-cc385d7497bc" />

- Nhận tham số LED_Param_t*

- Toggle LED (GPIOA->ODR ^= pin)

- Dùng vTaskDelay(delay) để tạo tần số nháy.

**4. main():**

<img width="605" height="395" alt="image" src="https://github.com/user-attachments/assets/65992f41-06c8-4c9f-af0c-13300343f5d1" />

- Bật HSI, reset clock.

- Khởi tạo GPIO.

- Tạo 3 task cùng dùng LED_Task nhưng truyền tham số khác nhau cho từng LED.

- Bắt đầu scheduler FreeRTOS.

**B. KẾT LUẬN:**
- Một hàm task dùng chung, tham số quyết định LED và tốc độ nháy, giúp code ngắn gọn và dễ mở rộng.
- Sử dụng một task chung với tham số riêng cho từng LED giúp giảm trùng lặp code, dễ quản lý và mở rộng.
