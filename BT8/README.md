**BÀI THỰC HÀNH SỐ 8: CHUYỂN ĐỔI ADC**

**ĐỌC VÀ CHUYỂN ĐỔI TÍN HIỆU ANALOG SANG DẠNG SỐ**

**A. CHƯƠNG TRÌNH SỬ DỤNG ADC TRÊN STM32F103C8T6 ĐỂ ĐỌC GIÁ TRỊ TỪ MỘT BIẾN TRỞ VÀ CHUYỂN ĐỔI GIÁ TRỊ NÀY THÀNH ĐIỆN ÁP (mV) VÀ HIỂN THỊ LÊN TERMINAL**
- werwe
              - r234

**1. Phần cứng:**
- Stm32f103c8t6 x1
- ST-Link V2 x1
- Potentionmeter x1
- USB TO UART x1
- Các dây nối jumper

**2. Phần mềm:**
- KeilC
- Hercules

**3. Kết quả mong đợi:**
- Hiển thị được chính xác ADC của biến trở và nhiệt độ tương ứng

**B. QUY TRÌNH THỰC HIỆN:**

_***Nhóm em quy ước 0-3.3V tương ứng với số mức trong ADC (12 bit) là: 0-4095**_

<img width="217" height="79" alt="image" src="https://github.com/user-attachments/assets/858ad484-8e55-456a-b5b2-19cec485c966" />

- Giá trị ADC cao nhất đo được là 4052 tương ứng với 3.27V
- Vậy công thức quy đổi là:
       
     <img width="160" height="73" alt="image" src="https://github.com/user-attachments/assets/d9a13f67-39eb-465d-99ff-2660d8d85f45" />
 
    - ADC_Value: Được đo từ biến trở
    - Vref: Điện áp tham chiếu, ở đây là 3.3V
    - 4095: Số mức tối đa đạt được (2 ^ 12 = 4096 -> 4096 - 1 = 4095)

**1. Khai báo trước các nguyên mẫu hàm (phần này chủ yếu để kiểm soát code dễ dàng hơn):**

<img width="356" height="209" alt="image" src="https://github.com/user-attachments/assets/0f3aee67-0ae2-4803-b567-6d6353bf4829" />

- Hàm cấu hình GPIOA chân PA1 để nhận tín hiệu vào từ biến trở
- Hàm cấu hình USART1 với chân PA9-TX và PA10-RX để hiển thị dữ liệu lên terminal
- Hàm cấu hình TIMER2 để tạo độ trễ chuẩn phù hợp khi cấu hình ADC
- Hàm cấu hình ADC1 để tạo ra được một conversion chuyển đổi dữ liệu (analog->digital)

**2. Cấu hình chi tiết:**

**2.1. Cấu hình chi tiết GPIOA với chân PA1:**

<img width="227" height="124" alt="image" src="https://github.com/user-attachments/assets/0dedd462-074d-45c3-991e-35152abd1b56" />

- Cấu hình ở chế độ Analog mode

**2.2. Cấu hình chi tiết cho USART1:**

<img width="307" height="221" alt="image" src="https://github.com/user-attachments/assets/3aa0a9b4-5260-4c34-9f69-e9cdef9da4b9" />

- PA9-TX ở chế độ Alternate function Push-Pull dùng cho ngoại vi như USART, SPI, I2C, etc
- PA10-RX ở chế độ Input pull-up để giữ đường truyền ở mức cao khi idle

**2.3. Cấu hình chi tiết TIMER2 và tạo hàm delay từ nó:**

<img width="256" height="211" alt="image" src="https://github.com/user-attachments/assets/6120d05a-2224-4d74-943b-dbcf5dd8783a" />

- Cho bộ chia tần là dể 1 tick tương ứng với 1us
- Cho bộ đếm với từng tick từ giá trị 0 -> 65536
- Hàm delay tạo bằng cách giữ cpu trong vòng lặp khi bộ đếm đang đếm đến giá trị thoả mãn

**2.4. Cầu hình chi tiết ADC1:**

<img width="262" height="207" alt="image" src="https://github.com/user-attachments/assets/4c81a1c3-5726-4aa8-8c29-7cf485e68fd7" />

- Cấp clock và bật ADC1 lên
- Chọn sample time để tạo thời gian lấy mẫu cho tụ để tín hiệu lấy được chính xác
- Chờ khoảng 2us để hệ thống ổn định
- Chọn channel 1 (PA1) là lần chuyển đổi đầu tiên trong chuỗi sẽ lấy giá trị từ channel 1 (PA1)
- Bật bit 1 cho ADON để bắt đầu quá trình chuyển đổi theo đúng trình tự chuẩn
- Đợi đến khi cờ EOC được bật thì quá trình chuyển đổi hoàn thành

**3. Gửi một chuỗi ký tự bằng USART1:**

<img width="354" height="173" alt="image" src="https://github.com/user-attachments/assets/82e83789-8e7d-404f-a481-0657123d4c0e" />

- Khi nào mà cờ TC(Transmission complete) bằng 1 thì có nghĩa là toàn bộ dữ liệu bao gồm cả start/stop bits đều đã được truyền đi và thanh ghi truyền TXE không còn dữ liệu nữa để cho lần nạp dữ liệu tiếp theo
- Một chuỗi trong C thì là kết hợp của từng ký tự thì chỉ cần lấy địa chỉ của từng ký tự trong bộ nhớ ghép vào thì thành một chuỗi theo yêu cầu

**4. Tạo hàm đọc giá trị ADC của biến trở:**

<img width="256" height="97" alt="image" src="https://github.com/user-attachments/assets/1346bb29-f476-4f68-b975-dab4598bf56b" />

- Bật ADC lên
- Chờ cờ EOC = 1 và trả về giá trị điện áp đọc được được quy đổi ra theo mức (0-4095)
- Cờ EOC = 0 và chờ cho lần conversion tiếp theo

**5. Thực thi chương trình:**

<img width="505" height="315" alt="image" src="https://github.com/user-attachments/assets/6291091d-1e60-4dcb-8c61-b164fc8f3bd7" />

- Gọi các hàm đã tạo từ trước
- Khởi tạo mảng chứa khoảng 32 ký tự (phù hợp cho chuỗi dài hiển thị ADC và Voltage tương ứng)
- Vòng lặp:
              - Lấy giá trị từ thanh ghi DR
  
              - Chuyển đổi sang mức điện áp tương ứng với số mức ADC như công thức ở trên
  
              - In chuỗi theo thứ tự ADC (int) trước rồi đến Voltage (float)
  
              - Dùng USART để gửi chuỗi trên lên terminal
  
              - Delay 1ms tạo độ trễ

**C. KẾT LUẬN:**

**1. Đánh giá kết quả nhận được:**

- Hiển thị khá chính xác ADC và chuyển đổi sang Voltage tương ứng
- Do hạn chế về phần cứng nên có thể vẫn đang còn nhiều sai số

**2. Hướng cải tiến:**

- Có thể dùng cảm biến ngoài để quản lý dễ dàng hơn
- Có thể tạo delay non-blocking bằng ngắt ngoài nhưng phức tạp hơn

**_Video mô phỏng: [ADC_Pot_Voltage](https://drive.google.com/drive/u/0/folders/1yncYx0cI1FtgzgAOeFypJnUEJCMZ6Jc0)_**

