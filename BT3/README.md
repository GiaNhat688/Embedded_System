  **BÀI THỰC HÀNH 3**
  
  **ĐIỀU KHIỂN NGẮT NGOÀI (EXTI)**

**A. CHƯƠNG TRÌNH ĐIỀU KHIỂN LED THAY ĐỔI TRẠNG THÁI BẰNG NÚT NHẤN SỬ DỤNG NGẮT NGOÀI TRONG KHI MỘT LED KHÁC VẪN NHẤP NHÁY VỚI CHU KỲ LÀ 1HZ**

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
- Khi nhấn nút, trạng thái của LED sẽ đảo ngược.
- Trong quá trình đó 1 LED khác vẫn nhấp nháy với chu kỳ 1Hz

**B. QUY TRÌNH THỰC HIỆN:**

**1. Khởi tạo Timer2 để làm delay:**

<img width="234" height="124" alt="image" src="https://github.com/user-attachments/assets/26d6f54c-ecc6-4c28-bf69-46024fc92b94" />

- Bật CLock cho TIM2 (TIM2EN)
- Chia xung Timer
- Auto-Reload mỗi khi timer cập nhật sẽ tương đương với 1ms
- Bật counter(CEN bit) để Timer chạy

**2. Tạo Delay bằng TIMER2:**

<img width="329" height="118" alt="image" src="https://github.com/user-attachments/assets/a439d42f-6a73-4131-8563-5e0473d76422" />

- Vòng lặp lặp t lần, mỗi lần chờ cờ UIF -> tương ứng 1ms
- TIM2->CNT = 0; reset counter trước mỗi lần đếm để đảm bảo bắt đầu từ 0
- while((TIM2->SR & (1<<0)) == 0); poll đến khi UIF = 1 (update event)
- TIM2->SR &= ~(1<<0); xóa cờ UIF (bằng cách clear bit)

**3. Xử lý ngắt ngoài:**

<img width="314" height="163" alt="image" src="https://github.com/user-attachments/assets/e32a358b-22b0-4fb3-9ab4-1033d93fb225" />

- if (EXTI->PR & (1<<0)) kiểm tra có pending flag trên EXTI line 0 hay không
- EXTI->PR = (1<<0); ghi 1 vào PR để xóa pending flag (đây là cách chuẩn trên EXTI)
- delay_ms(20); chờ 20 ms để chống dội (debounce)
- if (!(GPIOB->IDR & (1<<0))) đọc lại chân PB0: vì đang dùng pull-up, khi nhấn chân xuống mức 0 → nếu vẫn 0 sau delay thì coi là nhấn thật
- GPIOA->ODR ^= (1<<2); đảo trạng thái PA2 (LED)

**4. Hàm thực thi main:**

**a. Bật Clock cho AFIO, GPIOA, GPIOB:**

<img width="401" height="48" alt="image" src="https://github.com/user-attachments/assets/e21fc81b-d649-4d19-8cb1-123a0d73609f" />

- (1<<0) = AFIOEN (Alternate Function I/O)
- (1<<2) = IOPAEN (GPIOA)
- (1<<3) = IOPBEN (GPIOB)

**b. Cấu hình PBO làm input pull-up:**

<img width="253" height="174" alt="image" src="https://github.com/user-attachments/assets/324e2649-5893-4b92-bf6d-0a3c53aaa18c" />

-  CRL điều khiển pin 0..7, mỗi pin 4 bit (CNF[1:0] | MODE[1:0])
- 0x8 << 0 tương ứng CNF = 10 và MODE = 00 → input with pull-up/pull-down
- GPIOB->ODR |= (1<<0); khi ở chế độ input pull-up/pull-down, ghi 1 vào ODR sẽ kích pull-up (ghi 0 sẽ kích pull-down)

**c. Cầu hình PA2 làm output (LED điều khiển bởi nút):**

<img width="220" height="68" alt="image" src="https://github.com/user-attachments/assets/7862b353-f56e-4cb9-a0af-3e5886beb67b" />

- 0x1 << 8 → MODE = 01 (output 10 MHz), CNF = 00 (push-pull) → output push-pull
- GPIOA->ODR |= (1<<2); khởi tạo PA2 = 1 (LED sáng nếu nối theo kiểu active-high)

**d. Cấu hình PA5 làm output (LED nháy):**

<img width="237" height="53" alt="image" src="https://github.com/user-attachments/assets/1dd0cb10-07d2-4103-9ed7-9b3ee08b8628" />

- Tương tự: PA5 output push-pull 10MHz

**e. Chọn nguồn EXTI0 từ PB0 (AFIO):**

<img width="271" height="48" alt="image" src="https://github.com/user-attachments/assets/0f2d415d-547c-43d0-9a83-a01450c5f54c" />

- EXTICR[0] thấp 4 bit chọn EXTI0 nguồn port: 0000 = PA, 0001 = PB, 0010 = PC, ...
- 0x1 → chọn Port B cho EXTI0 (tức EXTI line 0 sẽ theo PB0)

**f. Cấu hình EXTI (cho phép và chọn cạnh):**

<img width="239" height="64" alt="image" src="https://github.com/user-attachments/assets/70f5af5d-8ac7-4825-a771-8ed4b0ebb310" />

- IMR |= (1<<0) unmask line 0 → cho phép ngắt từ line 0
- FTSR |= (1<<0) chọn falling edge trigger (ghi nhận sườn xuống)
- NVIC_EnableIRQ(EXTI0_IRQn) bật ngắt trong NVIC (CMSIS helper)

**g. Khởi tạo TIM2 và vòng lặp chính:**

<img width="226" height="116" alt="image" src="https://github.com/user-attachments/assets/60da3dd5-5b5b-41e1-8e92-2fdb094ef7eb" />

- Timer khởi tạo để xài trong delay_ms
- Vòng lặp: đảo PA5 mỗi 1000 ms → LED PA5 nháy 1Hz (thực tế 1s on, 1s off => 0.5 Hz toggles? Thường gọi blink 1Hz nghĩa đổi trạng thái mỗi 1s)

**C. KẾT LUẬN:**

**1. Đánh giá về kết quả đạt được:**
- Chương trình đã hoàn thành yêu cầu cơ bản:
  - LED PA5 nhấp nháy với tần số ~1Hz (delay 1000ms)
  - LED PA2 đảo trạng thái khi nhấn nút PB0, có chống dội phím
  - Sử dụng GPIO ở cả 2 cổng A và B, cùng EXTI và Timer2
- Tuy nhiên vẫn còn hạn chế:
  - Hàm delay_ms() là blocking → chiếm CPU, không tận dụng được ưu thế ngắt và timer
  - Gọi delay_ms() trong ISR (ngắt EXTI) không an toàn, dễ gây xung đột và làm chậm xử lý
 
**2. Hướng cải tiến:**
- Không dùng delay trong ISR: ISR chỉ nên set cờ, xử lý debounce trong main hoặc bằng một timer riêng
- Dùng timer ngắt thay cho delay để tạo nhịp LED nhấp nháy, giúp CPU rảnh làm việc khác
- Tách riêng timer: một timer cho delay/blink, một timer cho debounce để tránh xung đột


