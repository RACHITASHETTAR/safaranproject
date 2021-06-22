
TaskHandle_t Task1;
TaskHandle_t Task2;

const int redled=21;
const int greenled=25;

void setup()
{
  Serial.begin(115200);
  pinMode(redled,OUTPUT);
   pinMode(greenled,OUTPUT);
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
    for(;;){

      Serial.print("Task1 Running ");
      Serial.println(xPortGetCoreID());
      digitalWrite(redled, HIGH);
      delay(1000);
      digitalWrite(redled, LOW);
      delay(1000);
    }
  }

   void Task2code(void * PvParameters){
    for(;;){

      Serial.print("Task2 Running ");
      Serial.println(xPortGetCoreID());
      digitalWrite(greenled, HIGH);
      delay(1000);
      digitalWrite(greenled, LOW);
      delay(1000);
    }
  }

  void loop()
  {
    
  }
///////////////////////////////////////////////
// C program for the above approach
//#include <conio.h>
#include <stdio.h>
#include <string.h>
  
// Driver Code
int main()
{
    // Substitute the file_path string
    // with full path of CSV file
    FILE* fp = fopen("C:\Users\Esskay development-1\OneDrive\Desktop\", "a+");
  
    char name[50];
    int accountno, amount;
  
    if (!fp) {
        // Error in file opening
        printf("Can't open file\n");
        return 0;
    }
  
    // Asking user input for the
    // new record to be added
    printf("\nEnter Account Holder Name\n");
    scanf("%s", &name);
    printf("\nEnter Account Number\n");
    scanf("%d", &accountno);
    printf("\nEnter Available Amount\n");
    scanf("%d", &amount);
  
    // Saving data in file
    fprintf(fp, "%s, %d, %d\n", name,
            accountno, amount);
  
    printf("\nNew Account added to record");
  
    fclose(fp);
    return 0;
}
