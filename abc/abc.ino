#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include <dht.h>
#include "painlessMesh.h"
#define   MESH_PREFIX     "Safran"
#define   MESH_PASSWORD   "Safran123"
#define   MESH_PORT       5555

painlessMesh  mesh;
static const dht_sensor_type_t sensor1_type = DHT_TYPE_DHT22;
static const gpio_num_t dht_gpio1 = 26;// pin number of sensors to output
TaskHandle_t Task1;
static RTC_DATA_ATTR struct timeval sleep_enter_time;
struct timeval now;
gettimeofday(&now, NULL);
int sleep_time_ms = (now.tv_sec - sleep_enter_time.tv_sec) * 1000 + (now.tv_usec - sleep_enter_time.tv_usec) / 1000;
void dht_task1() ; // Prototype so PlatformIO doesn't complain
String getReadings(); // Prototype for sending sensor readings
Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
/////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(115200); //enable serial monitor
  pinMode(dht_gpio1,OUTPUT);
  //////////////////////////////////////////////
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  userScheduler.addTask(dht_task1);
  taskSendMessage.enable();
  
//To call the dht-task function:
//Create tasks: to send messages and get readings;
   xTaskcreatecore1(&dht_task1, "dht_task1", configMINIMAL_STACK_SIZE, NULL, 1, &Task1, 0); //task function//name of the task //stack size of the task  //parameter of the task//priority of task//task handle to keep the track of created task//pin task to core 0
      delay(500);
Initialize_PlxDaq();
}

/////////////////////////////////////////////////////
void dht_task1(void *pvParameter1) //function dont return any value
{
String msg = getReadings();
  mesh.sendBroadcast(msg);
    int16_t temperature1 = 0;
    int16_t humidity1 = 0;
    Serial.print("Task1 is running on core");
     print_esp_sleep_get_wakeup_cause();
     
  // to run the code infinite.
  while(1) {
        if (dht_read_data(sensor1_type, dht_gpio1, &humidity1, &temperature1) == ESP_OK)
            Serial.print("Humidity: %d%% Temp: %d^C\n", humidity1 / 10, temperature1 / 10);
        else
            Serial.println("Could not read data from sensor\n");

        vTaskDelay(5000 / portTICK_PERIOD_MS); //will read sensor data every five seconds.       
    }
////////////////////////////////////////////////////
String getReadings () {
  JSONVar jsonReadings;
  jsonReadings["node"] = nodeNumber;
  jsonReadings["temp"] = temperature1();
  jsonReadings["hum"] = humidity1();
  readings = JSON.stringify(jsonReadings);
  return readings;
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());
  JSONVar myObject = JSON.parse(msg.c_str());
  int node = myObject["node"];
  double temp = myObject["temp"];
  double hum = myObject["hum"];
  double pres = myObject["pres"];
  Serial.print("Node: ");
  Serial.println(node);
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

////////////////////////

void print_esp_sleep_get_wakeup_cause()
{
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
////////////////////////////////////////

void Initialize_PlxDaq()
{
Serial.println("CLEARDATA"); //clears up any data left from previous projects
Serial.println("LABEL,Date,Time,Temperature,Humidity"); //always write LABEL, to indicate it as first line
}
void Write_PlxDaq()
  {
    Serial.print("DATA"); // write "DATA" to Inidicate the following as Data
    Serial.print(","); //Move to next column using a ","

    Serial.print(temperature); //Store date on Excel
    Serial.print(","); //Move to next column using a ","

    Serial.print(humidity); //Store date on Excel
    Serial.print(","); //Move to next column using a ","

    Serial.println(); //End of Row move to next row

  }
//////////////////////////////////////////
void loop()
{
   Write_PlxDaq();
   mesh.update();
   delay(5000); //Wait for 5 seconds before writing the next data 
}
