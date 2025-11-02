**BÀI THỰC HÀNH 14**

---

**BÀI 2: SO SÁNH DÒNG TIÊU THỤ CỦA 2 CHẾ ĐỘ LÀ SLEEP VÀ DELAY(SYSTICK TIMER) KHI THỰC HIỆN CÙNG MỘT CHỨC NĂNG**

---

**A. QUY TRÌNH THỰC HIỆN:**

**(SYSTEMCLOCK = 8MHZ)**

**Cách đo:**

- Nối 1 đầu của đồng hồ với nguồn 3.3V ST-Link
  
- Nối đầu còn lại với 3.3V STM

**1. Delay thông thường:**

<img width="335" height="462" alt="image" src="https://github.com/user-attachments/assets/f960ddc7-fd65-46fb-8985-f8055c7d1299" />

<img width="267" height="222" alt="image" src="https://github.com/user-attachments/assets/846c0cd1-cf7c-4769-a312-d1847e69579d" />

- Dùng SysTick timer làm delay với clock mỗi 1ms
  
- CPU vào vòng while và sẽ ở mãi đó với lượng điện năng tiêu thụ như chạy chương trình thường và thoát ra khi chạy hết while

**2. Sử dụng Sleep:**

<img width="329" height="389" alt="image" src="https://github.com/user-attachments/assets/60c5eab3-e08a-42f3-8a76-30953359ac0f" />

<img width="290" height="367" alt="image" src="https://github.com/user-attachments/assets/ba4baa08-b0cb-4fd0-a5bd-1a26c755c69b" />

- Sử dụng SysTick làm delay cứ mỗi 1ms là ngắt ISR
  
- Bật Sleep mode bằng WFI chờ ISR và CPU sẽ vào trạng thái tạm dừng core clock nhưng các ngoại vi vẫn chạy bình thường

---

**B. KẾT LUẬN:**

- Chương trình chạy ổn định

- Sai số không quá nhiều và giảm được lượng điện năng tiêu thụ tốt

- Khi có ngắt thì CPU thức dậy rồi xử lý ngắt và quay lại lệnh WFI rồi ngủ tiếp

--> Giảm lượng điện năng tiêu thụ

**_Link demo: [Delay_Sleep](https://drive.google.com/file/d/1ZrJDoHXcdKF7ClBgcZsWc4ei_6htYaD5/view?usp=drive_link)_**
