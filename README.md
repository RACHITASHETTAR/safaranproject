# safaranproject

# Project step by step procedure:
step1: Upload the code provided on two ESP32 boards and modify the print data to easily identify the sender node and receiver nodes.
step2: With the boards connected to your computer, open a serial connection with each board. You can use the Serial Monitor, or you can use a software like PuTTY and open two windows for both the ESP32 boards. We  should see that both boards receive data.
step3:  To connect the two different  ESP32 borad, sensor monitoring log data with an ESP32 and a DHT module and store all sensor data in a CSV file on the microSD storage
Step5: To collect the sensor data to populate on .csv file to secure table format data from both the sensors on sd card.
Step 5: To syncronize the data to one table using PLX-DAQ  software.

# Design to write a program :
1. calculate sleep duration by comparing the starting time and the current time. We will put the sleep duration value on the sleep_time_ms variable.
2. The painlessMesh library allows us to create a mesh network with the two ESP32 boards 
The MESH_PREFIX refers to the name of the mesh. You can change it to whatever you like. The MESH_PASSWORD, as the name suggests is the mesh password. You can change it to whatever you like. All nodes in the mesh should use the same MESH_PREFIX and MESH_PASSWORD.
The MESH_PORT refers to the the TCP port that you want the mesh server to run on. The default is 5555.
3.To check if our program runs after sleep mode or not, we can use the esp_sleep_get_wakeup_cause() function. We will get ESP_SLEEP_WAKEUP_TIMER if our program wakes up from sleep mode. Then,  perform sensing using the DHT module, and store sensor data on the microSD storage.
4.After the ESP32 is started after a deep sleep, the information about the card needs to be set up again in variable mount_config. The next step is to mount the "/sdcard" mount point and to ensure that the operation is completed without any errors. In case of errors, depending on the error, some helper messages will be printed on the serial terminal.
.Then, perform sensing using the DHT module, and store sensor data on the data1.csv file.
5.Now that the card is mounted without any errors, we can read the values from the DHT sensor.
 Open the file "/sdcard/data1.csv" in the append mode (see the second parameter of the fopen function), taking care not to overwrite the previous values, and write the current humidity and temperature in the file by using the fprintf() function.
6.Now that a new record is added to the file "/sdcard/data1.csv" , we can close the file by calling the function fclose and passing the file descriptor.
7.Now that we have the latest reading stored on our file  and the file is closed, and  prepare it for another deep sleep period. After that, the same process will be done repeatedly.
8.To enable our program to enter sleep mode, we call esp_deep_sleep_start(). Before calling esp_deep_sleep_start(), we set our wake-up timer. In this program, we wake up every 20 seconds.
9. It will generate a data1.csv file. we can use Microsoft Excel to visualize the data1.csv file. we can visualization of the sensor data. similarly data2.csv is generated and next file.
10. To log the data on excel sheet:  Used to Syncronize the data on one table.
PLX-DAQ is Microsoft Excel Plug-in software that helps us to write values from ESP to directly into an Excel file on our Laptop or PC. 
This is my personal favourite because of two reasons:
we can write and monitor the data at the same time and provides us way to plot them as graphs.
To use this software with ESP we have to send the data serially in a specific pattern just like displaying value on serial monitor. 