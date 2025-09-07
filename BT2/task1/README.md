**CẤU HÌNH 2 NHÓM LED Ở CHẾ ĐỘ OUTPUT, LED SẼ NHẤP NHÁY VỚI CHU KỲ 1000MS**
 
**A. Chương trình sử dụng các chân GPIO ở 2 cổng A và B trên STM32F103C8T6 để điều khiển led nhấp nháy**

**Cụ thể:** 

**Phần cứng:** Sử dụng PA2 - led xanh lá, PB0 -  led vàng, 2 điện trở 220R hạn dòng, các dây nối từ pin đến led

**Kết quả:**
+ Ban đầu 2 led sáng cùng lúc
+ Sau 1000ms thì 2 led đều tắt
+ Sau 1000ms tiếp thì 2 led lại sáng
+ Lặp lại liên tục

  
--> 2 led sáng tắt liên tục cùng lúc

**B. QUY TRÌNH THỰC HIỆN:**

**1. Cấp Clock cho GPIOA và GPIOB và cấu hình cho 2 chân PA2 và PB0**

<img width="308" height="166" alt="image" src="https://github.com/user-attachments/assets/d08b4256-2969-4e69-a07f-56f01cdc59d9" />

+ Bật clock cho GPIOA (bit 2) và GPIOB (bit 3), để có thể dùng chân của 2 port này
+ Xoá cấu hình cũ rồi cấu hình PA2 thành Output push-pull, tốc độ 2 MHz
+ Tương tự, cấu hình PB0 thành Output push-pull, 2 MHz

**2. Tạo delay đơn giản bằng for**

  <img width="260" height="217" alt="image" src="https://github.com/user-attachments/assets/72aeac4b-d4b8-4e8e-b261-261cb461906d" />

+ Tạo vòng lặp rỗng để tạo trễ giả lập (delay).
+ Vì HSI mặc định 8 MHz, nên khoảng t*8000 lệnh vòng lặp ≈ t mili giây

**3. Tạo hàm thực thi main để nhấp nháy 2 led**

 <img width="260" height="217" alt="image" src="https://github.com/user-attachments/assets/07dadc7a-2146-455f-8b88-2f01d591de57" />

+ Gọi hàm cấu hình GPIO
+ Bật PA2 và PB0 lên mức 1 (sáng LED), sau đó delay 1 giây
+ Tắt cả hai chân (LED tắt), delay 1 giây, rồi lặp lại
 
**C. Kết luận:**

- Chương trình đã cấu hình thành công GPIOA pin 2 (PA2) và GPIOB pin 0 (PB0) ở chế độ Output push-pull, 2 MHz bằng cách bật clock và thiết lập thanh ghi CRL.

- Trong vòng lặp vô hạn while(1), hai chân được đưa mức cao (1) → LED sáng, sau đó đưa mức thấp (0) → LED tắt, với khoảng trễ 1000ms giữa hai trạng thái.

- Kết quả: hai LED nối với PA2 và PB0 sẽ sáng cùng lúc trong 1 giây, tắt cùng lúc trong 1 giây, và lặp lại liên tục.

- Hạn chế: Độ trễ có thể không hoàn toàn chính xác do hàm for không tối ưu và khả năng đáp ứng từ phần cứng như máy tính, complier, vi điều khiển, led, dây dẫn, điện trở, và môi trường




