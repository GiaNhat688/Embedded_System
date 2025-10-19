BÁO CÁO THỰC HÀNH

QUẢN LÝ TRẠNG THÁI TASK DÙNG QUEUE TRONG FREERTOS

---

CHƯƠNG TRÌNH SỬ DỤNG STM32F103C8T6 VÀ FREERTOS DÙNG QUEUE ĐỂ NHẤP NHÁY LED TẠI CÁC TẦN SỐ KHÁC NHAU

---

A. QUY TRÌNH THỰC HIỆN:

1. Khai báo & include:

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

→ Gọi thư viện thanh ghi STM32 và các file cần cho FreeRTOS (Task, Queue).

2. Cấu trúc dữ liệu:

typedef struct {
    uint16_t freq;
    uint8_t duty;
} PWM_Params_t;

→ Gói tần số (freq) và độ rộng xung (duty) để gửi qua hàng đợi (Queue).

3. Hàm cấu hình GPIO:

void GPIO_Config(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL &= ~(0xF << (5 * 4));
    GPIOA->CRL |= (0x1 << (5 * 4));
}

→ Bật clock cho PORTA và cấu hình PA5 làm output (LED).

4. Task 1 – vTask_LED:

void vTask_LED(void *pvParameters) {
    PWM_Params_t params;
    for (;;) {
        if (xQueueReceive(xQueue, &params, portMAX_DELAY) == pdTRUE) {
            ...
        }
    }
}

→ Nhận dữ liệu freq & duty từ Queue,
→ Tính chu kỳ và thời gian bật/tắt LED,
→ Chớp LED theo tần số và duty đó.

5. Task 2 – vTask_Generator:

void vTask_Generator(void *pvParameters) {
    PWM_Params_t params;
    uint16_t freq_list[] = {1, 2, 5, 8, 10};
    uint8_t duty_list[] = {10, 30, 50, 70, 90};
    ...
}

→ Mỗi 1 giây thay đổi cặp (tần số, duty)
→ Gửi dữ liệu đó sang Queue cho Task LED dùng.

6. main():

int main(void) {
    GPIO_Config();
    xQueue = xQueueCreate(5, sizeof(PWM_Params_t));
    xTaskCreate(vTask_LED, "LED", 128, NULL, 1, NULL);
    xTaskCreate(vTask_Generator, "Gen", 128, NULL, 1, NULL);
    vTaskStartScheduler();
}

→ Cấu hình GPIO
→ Tạo Queue (lưu tối đa 5 phần tử)
→ Tạo 2 Task
→ Bắt đầu FreeRTOS Scheduler để cả 2 Task chạy song song.

---

B. TỔNG KẾT:

- Task 1: đọc dữ liệu → blink LED.
- Task 2: tạo dữ liệu → gửi sang Queue.
→ LED sẽ nhấp nháy với tần số và duty thay đổi liên tục theo thời gian.

---

C. HƯỚNG PHÁT TRIỂN:

1. Nâng cấp điều khiển LED:
- Dùng PWM thật (TIMx) thay vì vTaskDelay → LED sáng mượt, chính xác tần số & duty.
- Task 1 chỉ cần cập nhật TIMx->ARR (chu kỳ) và TIMx->CCR1 (duty).
- Có thể mở rộng điều khiển động cơ DC, servo, LED RGB.

2. Truyền thông giữa các Task
- Thêm UART Task để gửi thông tin freq & duty ra terminal (debug hoặc hiển thị).
- Thêm Task nhận lệnh UART → thay đổi tần số/duty theo người dùng nhập.
→ Ví dụ: gõ “F5 D50” → tần số 5Hz, duty 50%.

---

Link mô phỏng của nhóm: https://drive.google.com/drive/folders/1fJC_7aZNZXzkxWVUYC7e5eSyV4-Zhvk2
