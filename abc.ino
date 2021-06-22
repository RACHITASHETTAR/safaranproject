#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include <dht.h>
static const dht_sensor_type_t sensor_type = DHT_TYPE_DHT22;
static const gpio_num_t dht_gpio = 26;
TaskHandle_t Task1;
TaskHandle_t Task2;

void setup()
{
  Serial.begin(115200);
}
//create a task that will be executed in task1 code executed on ESP321
xTaskcreatecore1()
{
  Task1code, //task function
  "Task1", //name of the task
  10000, //stack size of the task 
  NULL, //parameter of the task
  1, //priority of task
  &Task1, //task handle to keep the track of created task
  0); //pin task to core 0
  delay(500);
  }
  
 xTaskcreatecore2()
{
  Task2code,
  "Task2",
  10000,
  NULL,
  1,
  &Task2,
  1); //pin task to core 1
  delay(500);
  }
  
  void Task1code(void * PvParameters){
    int16_t temperature = 0;
    int16_t humidity = 0; 
    
    for(;;){

      Serial.print("Task1 Running ");
      Serial.println(xPortGetCoreID());

    while(1) {
        if (dht_read_data(sensor_type, dht_gpio, &humidity, &temperature) == ESP_OK)
        FILE* f = fopen("/desktop/data1.csv", "a");
                if (f == NULL) {
                    printf("Failed to open file for writing\n");
                    return;
                } 
                fprintf(f, "%d,%d\n", humidity / 10, temperature / 10); 
                fclose(f);
            } 
            else
                printf("Could not read data from sensor\n");
                 vTaskDelay(5000 / portTICK_PERIOD_MS);
     }  
  }
  
   void Task2code(void * PvParameters){
     int16_t temperature = 0;
    int16_t humidity = 0; 
   for(;;){

      Serial.print("Task1 Running ");
      Serial.println(xPortGetCoreID());
  
    while(1) {
        if (dht_read_data(sensor_type, dht_gpio, &humidity, &temperature) == ESP_OK)
        FILE* f = fopen("/desktop/data2.csv", "a");
                if (f == NULL) {
                    printf("Failed to open file for writing\n");
                    return;
                } 
                fprintf(f, "%d,%d\n", humidity / 10, temperature / 10); 
                fclose(f);
            } 
            else
                printf("Could not read data from sensor\n");
                 vTaskDelay(5000 / portTICK_PERIOD_MS);
      }  
  }
