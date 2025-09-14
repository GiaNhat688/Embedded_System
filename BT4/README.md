**BÀI THỰC HÀNH 4**

**HẸN GIỜ BẰNG TIMER**

**A. CHƯƠNG TRÌNH  SỬ DỤNG STM32F103C8T6, LED LD2 (giả sử nối ở PA5) nhấp nháy bằng ngắt Timer2 với chu kỳ 1000ms (1Hz)**

**1. Phần cứng:**
- STM32F103C8T6 x1
- Led đơn (xanh, vàng) x2
- ST-Link V2 x1
- Điện trở 220R x2
- Nút nhấn x1
- Dây cái cái x4
- Dây đực đực x5

**2. Phần mềm:**
- IDE KeilC

**3. Kết quả mong đợi:**
- Cấu hình được Timer tạo ngắt mỗi 500ms
- Led sẽ tắt mỗi 500ms và sáng mỗi 500ms -> Tổng thời gian tương đương 1 chu kỳ là 1000ms (1Hz)

**B. QUY TRÌNH THỰC HIỆN:**

**1. Khai báo thư viện và cấu hình hàm xử lý ngắt của Timer2:**

<img width="260" height="137" alt="image" src="https://github.com/user-attachments/assets/21580442-96eb-4cc8-9e5f-bd8dabc23bb5" />

- Thư viện CMSIS / device header cho STM32F1: định nghĩa các thanh ghi (RCC, TIM2, GPIOA, EXTI...), các hằng số, kiểu dữ liệu
- Bắt buộc để truy cập trực tiếp các thanh ghi như TIM2->PSC, RCC->APB1ENR, ...
- if (TIM2->SR & 0x1)
  - Kiểm tra bit UIF (Update Interrupt Flag)
  - Nếu bit này = 1 → Timer vừa tạo event (update)
- TIM2->SR &= ~0x1
  - Xóa bit UIF bằng cách clear bit trong thanh ghi SR
- GPIOA->ODR ^= (1 << 5)
  - Đảo trạng thái cổng PA5 (nếu đang 1 → 0, 0 → 1)

**2. Cấu hình Timer2:**

<img width="259" height="147" alt="image" src="https://github.com/user-attachments/assets/856ea08e-8c95-48e0-bda5-a575ccde46ab" />

- RCC->APB1ENR |= (1 << 0)
  - Bật clock cho TIM2 (bit TIM2EN). Nếu không bật clock thì không thể cấu hình timer
- TIM2->PSC = 7200 - 1
    -Prescaler = 7199. Với giả sử clock timer = 72 000 000 Hz, sau prescaler:
     72 000 000 / (7199 + 1) = 72 000 000 / 7200 = 10 000 Hz
    → 1 tick = 1 / 10 000 = 0.0001 s = 0.1 ms
- TIM2->ARR = 5000 - 1
  - Auto-reload = 4999. Timer sẽ cập nhật khi đếm tới giá trị ARR. Số tick cần = 5000
    5000 × 0.1 ms = 500 ms → update event mỗi 500 ms
- TIM2->DIER |= (1 << 0);
  - Bật ngắt cập nhật (UIE) để mỗi khi update xảy ra, TIM2 sinh ngắt
- TIM2->CR1 |= (1 << 0)
  - Bật counter (CEN) để timer bắt đầu chạy
- NVIC_EnableIRQ(TIM2_IRQn)
  - Cho phép ngắt TIM2 trong NVIC (bật đường ngắt ở CPU)

**3. Hàm thực thi Main:**

**a. Cầu hình GPIO PA5:**

<img width="280" height="95" alt="image" src="https://github.com/user-attachments/assets/559bff62-f737-47c6-a1b8-027122e98f29" />

- RCC->APB2ENR |= (1 << 2)
  - Bật clock cho GPIOA (IOPAEN)
- GPIOA->CRL &= ~(0xF << 20); GPIOA->CRL |= (0x2 << 20)
  - Cấu hình pin PA5 (vì mỗi pin chiếm 4 bit trong CRL; pin5 bắt đầu tại bit 20)
  - 0x2 (bits 10) cho MODE = 10 (output 2 MHz), CNF = 00 (push-pull) → PA5 là output push-pull 2 MHz

**b. Khởi động Timer:**

<img width="144" height="84" alt="image" src="https://github.com/user-attachments/assets/becb6491-55fb-4b21-8352-7741da2d4690" />

- TIM2_Init() khởi tạo và bật timer + ngắt
- while (1) {} vòng lặp chính rỗng -> CPU rảnh, mọi thứ điều khiển bởi ngắt TIM2

**C. KẾT LUẬN:**

**1. Đánh giá kết quả đạt được:**

- LED tại PA5 nhấp nháy đúng tần số 1 Hz nhờ sử dụng ngắt Timer2 → chương trình gọn, CPU rảnh để làm việc khác
- Cấu hình trực tiếp thanh ghi giúp tối ưu tốc độ và kiểm soát chi tiết

**2. Hướng cải tiến:**

- Thêm cấu hình ưu tiên ngắt (NVIC_SetPriority) nếu có nhiều nguồn ngắt
- Viết hàm khởi tạo GPIO và Timer thành module riêng để tái sử dụng
