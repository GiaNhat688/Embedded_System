**BÀI THỰC HÀNH SỐ 15**

**NGĂN CHẶN VIỆC TRUY CẬP ĐỒNG THỜI VÀO TÀI NGUYÊN DÙNG CHUNG**

---

**CHƯƠNG TRÌNH SỬ DỤNG FREERTOS VÀ CƠ CHẾ MUTEX VỚI BINARY SEMAPHORE ĐỂ XỬ LÝ CÁC CHUỖI KÝ TỰ CÙNG MỨC ƯU TIÊN**

---

**A. QUY TRÌNH THỰC HIỆN:**

**1. Thêm các thư viện cần thiết:**

<img width="187" height="69" alt="image" src="https://github.com/user-attachments/assets/ab84581f-909f-48bc-b8a2-dfe8b0cd6b3e" />

- stm32f10x.h dùng để sử dụng thanh ghi
  
- FreeRTOS.h dùng để định nghĩa các dữ liệu cơ bản như BaseType_t, TickType_t, SemaphoreHandle_t,...
  
- task.h dùng để sử dụng các lệnh xTaskCreate(), vTaskStartScheduler(),...
  
- semphr.h dùng để sử dụng các lệnh như xSemaphoreGive(), xSemaphoreTake(), xSemaphoreCreateMutex(), xSemaphoreCreateBinary(),...

**2. Gọi 2 biến toàn cục kiểu con trỏ:**

<img width="290" height="38" alt="image" src="https://github.com/user-attachments/assets/44e25c79-64dc-466b-aace-2979f8a12d47" />

- Gọi kiểu biến tham chiếu Handle (ở đây là con trỏ) để tương tác với Mutex
- Tương tự để tương tác với Binary Semaphore

**3. Cấu hình USART1:**

<img width="323" height="194" alt="image" src="https://github.com/user-attachments/assets/5797ac56-3014-4a81-9625-b8d38ee47770" />

- Bật Clock cho USART1 và GPIOA

- Cấu hình chân PA9-TX(Alternate Function Output Push-Pull) tốc độ 50MHz

- Cấu hình chân PA10-RX(Input Floating) tốc độ 50MHz

- Vì sử dụng PCLK2 là 8MHz nên baudrate nhóm em để là 0x0341

- Kích hoạt Tx, Rx và USART1

**4. Hàm truyền chuỗi ký tự không có Mutex:**

<img width="392" height="103" alt="image" src="https://github.com/user-attachments/assets/58d5044c-1c2d-4ae2-a512-58b719abff42" />

- Dùng để hiển thị chuỗi ký tự bị lỗi khi không dùng cơ chế Mutex với cùng mức ưu tiên

**5. Hàm truyền chuỗi ký tự có sử dụng Mutex:**

<img width="482" height="173" alt="image" src="https://github.com/user-attachments/assets/4affba4f-b47a-4c3c-9df9-c9e093ce3bb9" />

- Khi các task trước chạy xong hết rồi thì đến task cần gửi sẽ lấy được quyền và gửi chuỗi ngay
  
- Sau đó thì trả lại quyền cho task khác

**6. Hai hàm để hiển thị lỗi:**

<img width="369" height="224" alt="image" src="https://github.com/user-attachments/assets/5e9d0fcd-e076-44cf-ad29-d716c97c091c" />

- Gửi chuỗi theo kiểu không dùng Mutex

**7. Hai hàm gửi đúng chuỗi sử dụng Mutex và Binary Semaphore:**

<img width="428" height="325" alt="image" src="https://github.com/user-attachments/assets/8b42013c-5fe8-4a33-b925-df26e66b08a6" />

- Vấn đề là task 3 và task 4 cùng mức ưu tiên và khi hiển thị lên terminal thì task 4 lại hiển thị trước

 --> Giải pháp: Chỉ hiển thị task 4 khi task 3 give Binary Semaphore cho task 4

- Gửi chuỗi cần hiển thị trước tiên và phải đặt lệnh xSemaphoreGive để được ưu tiên trước
  
- Chuỗi ở task cần hiển thị sau chỉ được hiển thị khi lấy được Binary Semaphore để in chuỗi ra
  
- Và sau đó lặp lại theo vòng for(;;) với delay 1s cho từng task mà không cần Binary Semaphore nữa

**8. Hàm thực thi chương trình:**

<img width="469" height="272" alt="image" src="https://github.com/user-attachments/assets/6b6355f1-a9b6-4257-b0fd-ad667e184a32" />

- Gọi hàm cấu hình USART1
  
- Gọi tên biến kiểu Handle là Semaphore tạo Mutex
  
- Gọi tên biến kiểu Handle là Semaphore tạo Binary Semaphore
  
- Nếu tạo được Mutex và Binary Semaphore thì thực hiện các task
  
- Tạo các task từ 1->4 theo format
  
- Gọi hàm khởi động bộ lập lịch để cho phép CPU bắt đầu chạy các task song song

---

**B. KẾT LUẬN:**
- Kết quả in ra terminal tương đối chính xác
  
- Có những lần khi mới reset thì hiển thị các ký tự rác có thể do phần cứng hoặc baudrate

---

**C. HƯỚNG PHÁT TRIỂN:**
- Có thể time out trong khi SemaphoreTake Mutex để không bị kẹt mãi khi task trước bị treo
  
- Có thể dùng mức ưu tiên cao hơn cho gọn gàng
  
- Dùng queue để tránh bị nhiều task cùng lúc sẽ ổn định và dễ mở rộng hơn

---

**_Link demo: [FreeRTOS_Mutex_Binary Semaphore](https://drive.google.com/file/d/1AAj6km06vSo-W4M_2HaHFhAce07KLJQG/view?usp=drive_link)_**
