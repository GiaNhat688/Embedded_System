**BÀI THỰC HÀNH SỐ 9: TRUYỀN DỮ LIỆU TRỰC TIẾP VÀO BỘ NHỚ (DMA)**

**HIỂU CƠ CHẾ DMA VÀ GIẢI PHÓNG CPU KHỎI CÁC TÁC VỤ TRUYỀN DỮ LIỆU**

**A. CHƯƠNG TRÌNH SỬ DỤNG STM32F103C8T6 VỚI DỮ LIỆU ĐƯỢC LẤY TỪ ADC QUA BIẾN TRỞ VÀ TRUYỀN QUA DMA ĐỂ GHI TRỰC TIẾP VÀO RAM** 

**1. Phần cứng:**

- STM32F103C8T6 x1
- ST-Link V2 x1
- Biến trở 10k ôm x1
- USB TO UART x1
- Các dây nối jumper

**2. Phần mềm:**

- KeilC
- Hercules

**3. Kết quả mong đợi:**

- Hiển thị được dữ liệu từ RAM lên terminal qua DMA với dữ liệu ban đầu từ ADC qua biến trở
- Hiểu được cơ chế DMA và giảm tải cho CPU

**B. QUY TRÌNH THỰC HIỆN:**

**1. Khai báo nguyên mẫu:**

<img width="345" height="268" alt="image" src="https://github.com/user-attachments/assets/58376da7-443b-4f02-82c0-c036b5d309f8" />

- Cho chương trình được sạch và truy cập ổn định

**2. Cấu hình chi tiết:**

**2.1. Cấu hình GPIO:**

<img width="228" height="126" alt="image" src="https://github.com/user-attachments/assets/5d1ae86c-0ec3-4469-b4fc-ad21e27f2cf6" />

- Chân PA1 ở chế độ analog mode để nhận điện áp từ biến trở

**2.2. Cấu hình Timer và tạo delay:**

<img width="232" height="206" alt="image" src="https://github.com/user-attachments/assets/e2c2ad28-7f26-4781-b2b0-5cf7fbc0a7c0" />

- Delay đúng 1us mỗi t

**2.3. Cấu hình USART:**

<img width="362" height="289" alt="image" src="https://github.com/user-attachments/assets/ba9e910c-724c-42e3-98ee-eb3c51a1fb6b" />

- USART1 để nhận và truyền dữ liệu

**2.4. Cấu hình ADC:**

<img width="224" height="320" alt="image" src="https://github.com/user-attachments/assets/e9c87d15-0855-4e5f-b288-0a0896079870" />

- Chọn ADC1 để có thể lấy được giá trị analog qua biến trở

**2.5. Cấu hình DMA và tạo ngắt ngoài:**

<img width="564" height="318" alt="image" src="https://github.com/user-attachments/assets/5a2efc59-acb5-403a-a043-aa8c510b3541" />

- Dữ liệu từ ADC sẽ qua DMA và vào thẳng RAM mà không cần phải qua CPU cop từng byte một
- Hàm ngắt để hiển thị dữ liệu lên terminal

**3. Gửi chuỗi ký tự lên terminal:**

<img width="347" height="180" alt="image" src="https://github.com/user-attachments/assets/6dfea60d-576c-46f0-9199-297316ba0605" />

- Chuỗi là tổng hợp của các ký tự nên để gửi chuỗi thì cần phải gửi từng ký tự

**4. Hàm thực thi ngắt:**

<img width="757" height="219" alt="image" src="https://github.com/user-attachments/assets/274c8dc4-9a7d-414a-8a85-cc7fad0332b1" />

- Set cờ TCIF khi đã truyền xong vào RAM và hiển thị dữ liệu lên terminal với định dạng cho trước

**5. Thực thi chương trình:**

<img width="216" height="190" alt="image" src="https://github.com/user-attachments/assets/b81f0982-f230-4fc6-9181-7d9d0f97c581" />

- Gọi các hàm đã khởi tạo trước đó
- Vòng lặp không làm gì cả vì chỉ cần hiển thị dữ liệu lên terminal

**C. KẾT LUẬN:**

**1. Đánh giá kết quả đạt được:**

- Kết quả hiển thị lên terminal với độ chính xác cao
- CPU có thể rảnh để làm việc khác mà dữ liệu vẫn có thể được truyền và hiển thị tốt
- Hiểu được cơ chế DMA và kết hợp được các kiến thức đã học để tạo chương trình này

**2. Hướng cải tiến:**

- Có thể dùng cảm biến để hiển thị thêm dữ liệu

Video bài làm: [ADC_DMA_RAM](
