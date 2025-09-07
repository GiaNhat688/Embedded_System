CẤU HÌNH 2 NHÓM LED Ở CHẾ ĐỘ OUTPUT, LED SẼ NHẤP NHÁY VỚI CHU KỲ 1000MS
 
Chương trình sử dụng các chân GPIO ở 2 cổng A và B trên STM32F103C8T6 để điều khiển led nhấp nháy

Cụ thể:
- Phần cứng: Sử dụng PA2 - led xanh lá, PB0 -  led vàng, 2 điện trở 220R hạn dòng, các dây nối từ pin đến led
- Kết quả:
+ Ban đầu 2 led sáng cùng lúc
+ Sau 1000ms thì 2 led đều tắt
+ Sau 1000ms tiếp thì 2 led lại sáng
+ Lặp lại liên tục
--> 2 led sáng tắt liên tục cùng lúc

QUY TRÌNH THỰC HIỆN:
- Bước 1: Cấp Clock cho GPIOA và GPIOB và cấu hình cho 2 chân PA2 và PB0

<img width="308" height="166" alt="image" src="https://github.com/user-attachments/assets/d08b4256-2969-4e69-a07f-56f01cdc59d9" />

- Bước 2: Tạo delay đơn giản bằng for

  <img width="260" height="217" alt="image" src="https://github.com/user-attachments/assets/72aeac4b-d4b8-4e8e-b261-261cb461906d" />

- Bước 3: Tạo hàm thực thi main để nhấp nháy 2 led

  ![Uploading image.png…]()



