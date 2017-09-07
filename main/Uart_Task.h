/* Uart Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "soc/uart_struct.h"

/**
 * This example shows how to use the UART driver to handle special UART events.
 *
 * It also reads data from UART0 directly, and echoes it to console.
 *
 * - port: UART0
 * - rx buffer: on
 * - tx buffer: on
 * - flow control: off
 * - event queue: on
 * - pin assignment: txd(default), rxd(default)
 */

#define EX_UART_NUM UART_NUM_2

#define BUF_SIZE        (1024)
#define MAX_MSG_SIZE    (400)
QueueHandle_t uart0_queue;

void uart_event_task(void *pvParameters);

