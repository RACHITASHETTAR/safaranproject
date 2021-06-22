# safaranproject

#Project step by step procedure:
step1: Create two task running on two different core
step2:  To connect the two different sensors to the ESP32 borad, sensor monitoring log data with an ESP32 and a DHT module andstore all sensor data in a CSV file on the microSD storage
Step3: To collect the sensor data to populate on .csv file to secure table format data from both the sensors on sd card.
Step4: To syncronize the data to one table using Macros.

Design to write a program :
1. calculate sleep duration by comparing the starting time and the current time. We will put the sleep duration value on the sleep_time_ms variable.
2.To check if our program runs after sleep mode or not, we can use the esp_sleep_get_wakeup_cause() function. We will get ESP_SLEEP_WAKEUP_TIMER if our program wakes up from sleep mode. Then,  perform sensing using the DHT module, and store sensor data on the microSD storage.
3After the ESP32 is started after a deep sleep, the information about the card needs to be set up again in variable mount_config. The next step is to mount the "/sdcard" mount point and to ensure that the operation is completed without any errors. In case of errors, depending on the error, some helper messages will be printed on the serial terminal.
.Then, perform sensing using the DHT module, and store sensor data on the data1.csv file.
4.Now that the card is mounted without any errors, we can read the values from the DHT sensor.
 Open the file "/sdcard/data1.csv" in the append mode (see the second parameter of the fopen function), taking care not to overwrite the previous values, and write the current humidity and temperature in the file by using the fprintf() function.
5.Now that a new record is added to the file "/sdcard/data1.csv" , we can close the file by calling the function fclose and passing the file descriptor.
6.Now that we have the latest reading stored on our file  and the file is closed, and  prepare it for another deep sleep period. After that, the same process will be done repeatedly.
7.To enable our program to enter sleep mode, we call esp_deep_sleep_start(). Before calling esp_deep_sleep_start(), we set our wake-up timer. In this program, we wake up every 20 seconds.
8. It will generate a data1.csv file. we can use Microsoft Excel to visualize the data1.csv file. we can visualization of the sensor data. similarly data2.csv is generated and next file.