CẤU HÌNH 1 LED Ở CHẾ ĐỘ OUTPUT VÀ 1 NÚT NHẤN Ở CHẾ ĐỘ INPUT, LED THAY ĐỔI TRẠNG THÁI MỖI LẦN NGƯỜI DÙNG NHẤN NÚT

Chương trình sử dụng các chân GPIO ở port A và port B, gốm PA2 cho led output và PB0 cho nút nhấn input

Cụ thể
- Phần cứng: STM32F103C8T6, 1 led đơn vàng, 1 nút nhấn 4 chân, các dây nối pin port với led và nút nhấn, điện trở 220R hạn dòng cho led
- Nối mạch:
+ PA2 nối với anode của led, còn cathode nối qua điện trở 220R xuống đất
+ PB0 nối với 1 đầu nút nhấn, đầu kia của nút nhấn nối với đất để tương tự như công tắc đóng mạch
+ Nối GND chung
- Nguyên lý:
+ Ban đầu thì PA2=0 -> Led tắt và trạng thái cạnh giả sử là 1
+ Khi nút nhấn nhận từ PB0 thì trạng thái cạnh sẽ từ 1 chuyển về 0 tạo ra sự thay đổi trạng thái -> Led sáng
+ Lặp lại như vậy cho từng trạng thái tiếp theo
 - Kết quả:
--> Led bật/tắt tuỳ vào nút nhấn, ban đầu led tắt, khi nhấn nút led bật, nhấn tiếp thì led tắt và lặp lại như vậy

QUY TRÌNH THỰC HIỆN
1. Cấp xung Clock cho port A và port B và cấu hình cho PA2 và PB0:

<img width="322" height="174" alt="image" src="https://github.com/user-attachments/assets/4e30276d-676c-421b-b087-2b7014a9ab5a" />

- Bật clock cho Port A (bit 2) và Port B (bit 3)
- Cấu hình PA2: MODE2 = 10 → Output tốc độ 2 MHz, CNF2 = 00 -> Output push-pull
- Cấu hình PB0: MODE0 = 00 → Input, CNF0 = 10 -> Input pull-up/pull-down -> PB0=1 mặc định, nhấn nút thì PB0=0

2. Tạo hàm delay đơn giản bằng for:

<img width="400" height="95" alt="image" src="https://github.com/user-attachments/assets/3554a83e-8b25-4fcf-9aa0-53a9fa88147f" />

- Tạo trễ bằng vòng lặp rỗng. Mỗi lần nhấn nút LED đổi trạng thái, trễ này dùng để chống dội phím (debounce).

3. Hàm thực thi main để thực hiện chương trình:

<img width="386" height="359" alt="image" src="https://github.com/user-attachments/assets/78ffc7d8-5a1b-4394-b61b-ba7f6ce9f4ab" />

- Ban đầu led tắt bằng led_state=0 và nút chưa nhấn là prev_button=1
- Đọc giá trị PB0, nếu từ 1 về 0 là nhấn nút thì đảo trạng thái led (led tắt thì bật và ngược lại)
- Thêm trễ chống dội phím
- Lưu lại trạng thái nút hiện tại cho lần kiểm tra tiếp theo

   
