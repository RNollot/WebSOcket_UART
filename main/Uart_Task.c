/* Uart Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

#include "Uart_Task.h"
#include "WebSocket_Task.h"
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

extern QueueHandle_t Uart_tx_queue;

static const char *TAG = "uart_events";

void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    size_t buffered_size;
    uint8_t* dtmp = (uint8_t*) malloc(BUF_SIZE);

    WebSocket_frame_t data_to_send = { 0 };

    data_to_send.payload = (char*) malloc(BUF_SIZE);
    
    
    for(;;) {
        //Waiting for UART event.
        if(xQueueReceive(uart0_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
            ESP_LOGI(TAG, "uart[%d] event:", EX_UART_NUM);
            switch(event.type) {
                //Event of UART receving data
                /*We'd better handler data event fast, there would be much more data events than
                other types of events. If we take too much time on data event, the queue might
                be full.
                in this example, we don't process data in event, but read data outside.*/
                case UART_DATA:

                    uart_get_buffered_data_len(EX_UART_NUM, &buffered_size);

                    int len = uart_read_bytes(EX_UART_NUM, (uint8_t*) &data_to_send.payload[data_to_send.payload_length], BUF_SIZE, 100 / portTICK_RATE_MS);

                    printf("payload_length :%u, len :%d\n", data_to_send.payload_length, len);

                    data_to_send.payload_length += len;

                    if(strchr(data_to_send.payload, '\n') != NULL)
                    {
                        WebSocket_frame_t __tx_frame;
                        __tx_frame.payload_length=data_to_send.payload_length;
                        __tx_frame.payload = (char*) malloc(data_to_send.payload_length);

                        memcpy(__tx_frame.payload, data_to_send.payload, data_to_send.payload_length);
                        xQueueSend(Uart_tx_queue, &__tx_frame,0);
                        memset(data_to_send.payload, 0, data_to_send.payload_length);
                        data_to_send.payload_length = 0;
                    }


                    break;
                //Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    ESP_LOGI(TAG, "hw fifo overflow\n");
                    //If fifo overflow happened, you should consider adding flow control for your application.
                    //We can read data out out the buffer, or directly flush the rx buffer.
                    uart_flush(EX_UART_NUM);
                    break;
                //Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    ESP_LOGI(TAG, "ring buffer full\n");
                    //If buffer full happened, you should consider encreasing your buffer size
                    //We can read data out out the buffer, or directly flush the rx buffer.
                    uart_flush(EX_UART_NUM);
                    break;
                //Event of UART RX break detected
                case UART_BREAK:
                    ESP_LOGI(TAG, "uart rx break\n");
                    break;
                //Event of UART parity check error
                case UART_PARITY_ERR:
                    ESP_LOGI(TAG, "uart parity error\n");
                    break;
                //Event of UART frame error
                case UART_FRAME_ERR:
                    ESP_LOGI(TAG, "uart frame error\n");
                    break;
                //UART_PATTERN_DET
                case UART_PATTERN_DET:
                    ESP_LOGI(TAG, "uart pattern detected\n");
                    break;
                //Others
                default:
                    ESP_LOGI(TAG, "uart event type: %d\n", event.type);
                    break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}