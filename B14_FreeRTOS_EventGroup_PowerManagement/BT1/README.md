**BÀI THỰC HÀNH SỐ 14**

**SỬ DỤNG EVENTGROUP TRONG FREERTOS ĐỂ QUẢN LÝ VÀ ĐIỀU PHỐI CÁC TÁC VỤ ĐƠN HOẶC ĐỒNG THỜI**

---

**A. QUY TRÌNH THỰC HIỆN:**

**1. Khai báo prototype và các thư viện:**

<img width="602" height="324" alt="image" src="https://github.com/user-attachments/assets/b42a8cff-5d4b-4064-a6e6-2b90ed659658" />

- Tạo các thư viện và prototype cho các bit bắt đầu một task và bit kết thúc tương ứng của task đó
  
- Tạo một biến kiểu Handle để điều khiển vào cơ chế EventGroup

**2. Cấu hình GPIO:**

<img width="273" height="233" alt="image" src="https://github.com/user-attachments/assets/27159f7a-0473-473a-ac3a-1ea69e049c96" />

- Bật clock
  
- Cấu hình cho pa0, pa1, pa2 là output push-pull với tốc độ là 50Mhz

**3. Tác vụ chính để điều phối:**

<img width="757" height="438" alt="image" src="https://github.com/user-attachments/assets/18d99a08-2c2a-4183-b26c-351eb65c0c54" />

- Với chế độ đơn thì để từng bit cần set riêng lẻ
  
- Đợi một lúc để cấu hình hoàn chỉnh
  
- Set các bit lên 1 tương ứng với task1, task2 và task3
  
- Tạo hàm trả về trạng thái xem đã hoàn thành task tương ứng với bit done hay chưa
  
- Với chế độ đồng thời thì trong cấu trúc hàm sẽ để cả 3 bit cùng set 1 lúc

**4. Các task con thực hiện nhiệm vụ:**

<img width="623" height="554" alt="image" src="https://github.com/user-attachments/assets/1e384614-8266-4d55-b064-6135561df25c" />

- Task 1 bit được set lên 1 từ hàm điều phối và sau đó thực thi bật/tắt led đơn
  
- Set bit done tương ứng với task1 để hàm chờ từ task điều phối nhận và xem là hoàn thành 1 task
  
- Tương tự với 2 task còn lại

**5. Hàm thực thi:**

<img width="491" height="243" alt="image" src="https://github.com/user-attachments/assets/ef63f839-0403-4877-ba4f-d52f7926ee72" />

- Khai báo hàm cấu hình
  
- Tạo EventGroup trả về là kiểu handle xMyEventGroup cho tất cả task
  
- Tạo lần lượt các task là task điều phối độ ưu tiên cao nhất, sau đó đến 3 task con với độ ưu tiên bằng nhau và thấp hơn task điều phối
  
- Kích hoạt bộ lập lịch
  
- Vòng while tránh khi bộ lập lịch bị lỗi tạo idle/timer task

---

_**Link video demo: [BTH14_FreeRTOS_B1_EventGroup](https://drive.google.com/file/d/1VQ7KeW9i42DnRqZg7gqha6Fym0rDzRv3/view?usp=drive_link)**_
