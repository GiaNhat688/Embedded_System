**BÀI THỰC HÀNH SỐ 13**

---

**BÀI 1**

**VIẾT CHƯƠNG TRÌNH GỒM 2 TASK: 1 TASK BLINK LED VÀ 1 TASK BẬT/TẮT LED CẢNH BÁO KHI CÓ NGẮT NGOÀI, TRONG ĐÓ TASK CẢNH BÁO ĐƯỢC KÍCH HOẠT NGAY SAU KHI HÀM NGẮT THỰC THI VÀ Ở TRẠNG THÁI BLOCK KHI CHƯA NHẤN NÚT**

---

**A. QUY TRÌNH THỰC HIỆN:**

**1. Thêm thư viện vào trong chương trình:**

<img width="185" height="83" alt="image" src="https://github.com/user-attachments/assets/7cb1a625-4126-4463-b8b6-4c8f2b287d99" />

- stm32f10x.h: Cung cấp các định nghĩa cho thanh ghi phần cứng của chip STM32 (ví dụ: GPIOA, RCC).

- FreeRTOS.h: Chứa các định nghĩa cốt lõi của FreeRTOS.

- task.h: Chứa các hàm quản lý tác vụ (task) như xTaskCreate, vTaskDelay.

- semphr.h: Chứa các hàm quản lý semaphore và mutex.

- stdbool.h: Cho phép sử dụng kiểu dữ liệu bool với hai giá trị true và false.

**2. Khai báo các biến toàn cục và macro:**

<img width="328" height="52" alt="image" src="https://github.com/user-attachments/assets/89c3a7e5-f8b0-4ce7-a7b0-669ec825e927" />

- SemaphoreHandle_t xInterruptSemaphore;: Khai báo một "handle" (biến quản lý) cho một đối tượng Semaphore. Biến này sẽ được dùng để tham chiếu đến semaphore sau khi nó được tạo ra.

-volatile bool isWarningOn = false;: Khai báo một biến toàn cục để lưu trạng thái của đèn cảnh báo.

-bool: Biến chỉ có thể là true hoặc false.

-volatile: Một từ khóa quan trọng, báo cho trình biên dịch biết rằng giá trị của biến này có thể bị thay đổi bởi các yếu tố bên ngoài (như một ngắt), vì vậy không được tối ưu hóa việc truy cập vào nó.

-#define DEBOUNCE_TIME_MS 200: Định nghĩa một hằng số cho thời gian chống dội phím là 200 mili giây.

**3. Hàm Khởi tạo Phần cứng (Hardware_Init):**

<img width="336" height="323" alt="image" src="https://github.com/user-attachments/assets/64a33e09-7d6f-4590-81d9-49ed29b5a1ec" />

- Bật Clock cho GPIOA và AFIO.
- Cấu hình chân PA0 (Input with Pull-Up), PA1 (Output Push-Pull - 50 MHz) và PA5 (Output Push-Pull - 50 MHz).
- Bật ngắt ngoài Line 0:
  - Cấu hình Mask Register.
  - Cấu hình Falling trigger (sườn xuống khi nhần nút).
- Set độ ưu tiên cho ngắt (càng thấp càng được ưu tiên).
- Bật ngăt NVIC với kênh EXTI0.

**4. Hàm nhấp nháy led cơ bản:**

<img width="328" height="106" alt="image" src="https://github.com/user-attachments/assets/e5affaa2-b35d-4ea5-b47f-9bb1049ea6e0" />

- Dùng cho task 1.

**5. Hàm bật/tắt led khi có ngắt:**

<img width="612" height="198" alt="image" src="https://github.com/user-attachments/assets/01f8c16a-52b1-4af9-abc1-94d90165192b" />

- Nếu nhận được "take" từ cơ chế Semaphore có nghĩa là ngắt xảy ra thì:
  - Thoát khỏi timeout vô hạn bằng lệnh pdTRUE.
  - Kiểm tra xem led hiện tại đang on hay off bằng cờ isOn.
  - Nếu là on thì sáng led bằng thanh ghi BSRR.
  - Nếu là off thì tắt led BRR.
- Vòng lặp vô hạn sẽ lặp lại khi có ngắt tiếp theo xảy ra.

**6. Hàm xử lý ngắt ngoài:**

<img width="643" height="246" alt="image" src="https://github.com/user-attachments/assets/7e34f75d-8c0d-439a-89f1-76f2e27f1c32" />

- Chống dội phím (debounce) bằng cách so sánh thời gian ban đầu chưa nhấn nút với thời gian khi nhấn nút có lớn hơn 200ms hay không:
  - Nếu có thì là một nút nhấn hoàn chỉnh và thực thi ngắt. (*)
  - Nếu không thì là bị dội phím và không thực thi ngắt.
- Khi đã vào được ngắt rồi thì xoá cờ ngắt đi để cho ngắt tiếp theo vào.
- (*) Khi nhấn được nút hoàn chỉnh:
  - Cho thời gian gần nhất lastTime là lúc nhấn nút currentTime để so sánh cho lần tiếp theo.
  - Ban đầu chưa có Task nào có độ ưu tiên cao cần thực hiện khi có ngắt xảy ra do Semaphore đang rỗng và Task đang ở trạng thái BLock.
  - Đưa Semaphore ra cho Task thực thi từ ISR với Handle là xInterruptSemaphore và trỏ đến HigherPriorityTaskWoken.
  - vInterruptTask chỉ được thực thi ngay khi có portYIELD_FROM_ISR, có nghĩa là yêu cầu Scheduler context switch ngay khi cờ                  xHigherPriorityTaskWoken là True.
  
**7. Hàm thực thi main():**

<img width="564" height="201" alt="image" src="https://github.com/user-attachments/assets/68bfbaca-a8d9-4468-b17f-68d2ffab13ca" />

- Gọi hàm con Hardware_Init() đã cấu hình từ trước.
- Tạo một đối tượng Semaphore nhị phân và gán Handle của nó cho biến xInterruptSemaphore.
- Kiểm tra xem Semaphore đã được tạo thành công chưa:
  - Nếu chưa được tạo thì không thực thi task.
  - Nếu dã được tạo thì:
    - Tạo Task 1 là vBlinkTask với độ ưu tiên là 1, stack length là 128 word, không truyền tham số và không handle task.
    - Tạo Task 2 là vInterruptTask với độ ưu tiên là 2, stack length là 128 word, không truyền tham số và không handle task.
    --> Bộ lập lịch bắt đầu thực thi task (FreeRTOS sẽ điều khiển CPU).

---

**B. KẾT LUẬN:**

- Chương trình chạy tương đối chính xác.

- Chương trình hoạt động ổn định, led nháy định kỳ và led cảnh báo bật/tắt ngay sau khi nhấn nút. Cơ chế chống dội nút giúp tránh nhiễu tín hiệu, tuy nhiên chương trình vẫn còn hạn chế khi xử lý trường hợp nhấn nút quá nhanh hoặc xuất hiện nhiều ngắt liên tiếp.

---

**C. HƯỚNG PHÁT TRIỂN:**

- Chương trình có thể được mở rộng bằng cách thêm nhiều nút và led để xử lý nhiều sự kiện ngắt khác nhau, tích hợp cơ chế lọc nhiễu bằng phần cứng, hoặc dùng hàng đợi (queue) để truyền dữ liệu từ ngắt đến các task xử lý phức tạp hơn.

- Ngoài ra, có thể bổ sung tính năng điều khiển thời gian bật/tắt led và ghi log sự kiện để nâng cao độ tin cậy.

---

_**Link demo: [FreeRTOS_IRQ](https://drive.google.com/file/d/112dLYg-iWgX8-oJDP7mTPcP6xbPlP00W/view?usp=drive_link)**_
