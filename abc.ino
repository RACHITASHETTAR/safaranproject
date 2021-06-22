#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include <dht.h>
static const dht_sensor_type_t sensor1_type = DHT_TYPE_DHT22;
static const dht_sensor_type_t sensor2_type = DHT_TYPE_DHT22;

static const gpio_num_t dht_gpio1 = 26;// pin number of sensors to output
static const gpio_num_t dht_gpio2 = 27;

TaskHandle_t Task1;
TaskHandle_t Task2;
SemaphoreHandle_t semaphore;

void setup()
{
  Serial.begin(115200); //enable serial monitor
  pinMode(dht_gpio1,OUTPUT);
  pinMode(dht_gpio2,OUTPUT);
  semaphore = xSemaphoreCreateMutex();
  
//To call the dht-task function:

   xTaskcreatecore1(&dht_task1, "dht_task1", configMINIMAL_STACK_SIZE, NULL, 1, &Task1, 0); //task function//name of the task //stack size of the task  //parameter of the task//priority of task//task handle to keep the track of created task//pin task to core 0
      delay(500);
    xTaskcreatecore2(&dht_task2, "dht_task2", configMINIMAL_STACK_SIZE, NULL, 1, &Task1, 1); // both task run on same priority level 1, sensor 1 is pinned to core 0 and sensor2 to core1
      delay(500);
}

void dht_task1(void *pvParameter1) //function dont return any value
{
    int16_t temperature1 = 0;
    int16_t humidity1 = 0;
    Serial.print("Task1 is running on core");
    Serial.println(xportGetCoreID); //to know the core running
  // to run the code infinite.
  while(1) {
        if (dht_read_data(sensor1_type, dht_gpio1, &humidity1, &temperature1) == ESP_OK)
            Serial.print("Humidity: %d%% Temp: %d^C\n", humidity1 / 10, temperature1 / 10);
        else
            Serial.println("Could not read data from sensor\n");

        vTaskDelay(5000 / portTICK_PERIOD_MS); //will read sensor data every five seconds.
        
    }
    
void dht_task2(void *pvParameter2)
{
    int16_t temperature2 = 0;
    int16_t humidity2 = 0;
     Serial.print("Task2 is running on core");
     Serial.println(xportGetCoreID);
    while(1) {
        if (dht_read_data(sensor2_type, dht_gpio2, &humidity2, &temperature2) == ESP_OK)
            Serial.print("Humidity: %d%% Temp: %d^C\n", humidity2 / 10, temperature2 / 10);
        else
            Serial.println("Could not read data from sensor\n");

        vTaskDelay(10000 / portTICK_PERIOD_MS); //will read sensor data every ten seconds.
        
    }
void loop()
{
static RTC_DATA_ATTR struct timeval sleep_enter_time;
struct timeval now;

    gettimeofday(&now, NULL);
    int sleep_time_ms = (now.tv_sec - sleep_enter_time.tv_sec) * 1000 + (now.tv_usec - sleep_enter_time.tv_usec) / 1000;

     switch (esp_sleep_get_wakeup_cause()) {
      
     
        case ESP_SLEEP_WAKEUP_TIMER: {
            printf("Wake up from timer. Time spent in deep sleep: %dms\n", sleep_time_ms);

            // Options for mounting the filesystem.
            esp_vfs_fat_sdmmc_mount_config_t mount_config = {
                .format_if_mount_failed = false,
                .max_files = 5,
                .allocation_unit_size = 16 * 1024
            };


            sdmmc_card_t* card;
            esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

            if (ret != ESP_OK) {
                if (ret == ESP_FAIL) {
                    printf("Failed to mount filesystem. "
                        "If you want the card to be formatted, set format_if_mount_failed = true.\n");
                } else {
                    printf("Failed to initialize the card (%s). "
                        "Make sure SD card lines have pull-up resistors in place.\n", esp_err_to_name(ret));
                }
                return;
            }

            // Card has been initialized, print its properties
            
            sdmmc_card_print_info(stdout, card);

           printf("Reading sensor data\n");

            if (dht_read_data(sensor_type, dht_gpio, &humidity, &temperature) == ESP_OK)
            {
                printf("Humidity: %d%% Temp: %d^C\n", humidity / 10, temperature / 10);
                FILE* f = fopen("/sdcard/logger.csv", "a");
                if (f == NULL) {
                    printf("Failed to open file for writing\n");
                    return;
                } 
                fprintf(f, "%d,%d\n", humidity / 10, temperature / 10); 
                fclose(f);
            } 
            else
                printf("Could not read data from sensor\n");

        esp_vfs_fat_sdmmc_unmount();
            printf("Card unmounted\n"); 

            break;
        }
        
        case ESP_SLEEP_WAKEUP_UNDEFINED:
        default:
            printf("Not a deep sleep reset\n");
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);

      const int wakeup_time_sec = 20;
    printf("Enabling timer wakeup, %ds\n", wakeup_time_sec);
    esp_sleep_enable_timer_wakeup(wakeup_time_sec * 1000000);

    esp_deep_sleep_start();

}
