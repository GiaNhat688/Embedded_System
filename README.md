**HỆ THỐNG NHÚNG**

**A. CÁCH TẠO VÀ CẤU HÌNH PROJECT SỬ DỤNG STM32F103C8T6 TRÊN KEILC**
- Mở phần mềm KeilC
- Vào Project -> New uVision Project
- Chọn thư mục và đặt tên cho project
- Chọn STM32F103C8
- Vào Manage Run-Time Enviroment để chọn thư viện
- Chọn các thư viện như CMSIS-CORE; Device-GPIO, Startup, Flash, Framework, etc
- Vào Project -> Options for Target để cấu hình Output, C/C++, Debug, Utilities, etc
- Tạo file mới main.c bằng Add New File trong thư mục Source Group 1
- Viết chương trình nhấp nháy led PC13
- Build Target(F7) và hiển thị 0 error(s), 0 warning(s) là đã build
- Kết nối STM32F103C8T6 bằng St-Link V2
- Nhấn Dowload(F8) để nạp chương trình vào vi điều khiển
- Erase Done, Progamming Done, Verify OK, Application running là nạp thành công

**B. NHẤP NHÁY LED PC13:**
- Led PC13 trên BluePill thuộc nhóm low-drive pin nên ta sẽ thiết kế led PC13 nối về GND, có nghĩa là led sáng khi ghi 0 và led tắt khi ghi 1
***Cách thực hiện:**

Bước 1: Bật Clock cho PortC và cấu hình PC13

<img width="261" height="128" alt="image" src="https://github.com/user-attachments/assets/8c9c8240-189b-4964-b37b-213959c388cb" />

Bước 2: Tạo hàm delay đơn giản để có hiệu ứng nhấp nháy

<img width="274" height="182" alt="image" src="https://github.com/user-attachments/assets/118b62c4-7ba2-44a3-8118-444f376478be" />

Bước 3: Tạo hàm main để thực thi chương trình với độ trễ 500ms

<img width="274" height="182" alt="image" src="https://github.com/user-attachments/assets/893f1c8f-41e2-4147-85b1-ae163b769ee6" />








