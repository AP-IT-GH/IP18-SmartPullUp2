#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <MedianFilter.h>
#include <MsTimer2.h>
SoftwareSerial Bluetooth (7,8); // RX and TX Pins
const size_t bufferSize = JSON_OBJECT_SIZE(3);

  




MedianFilter median(5, 600);        //here is the parameters for the median filter. It takes the median of 5 different samples and if the starting value is empty (seed) it will give the value of 600.
String Output;

double sampling = 20; 
double sampleTime = 0;
double Start = 0;
double Downtime = 0;
double Uptime = 0;

float average = 0.0;
float feedback = 0.0;

int oldInput = 0;
int input=0;
int filteredOutput=0;
int output;
int i = 0;
int Count = 0;
int old_func_value = 0;
int oldout = 0;
int Button;
int previous,Shutter; 

long int ref_rest_avg = 0;
long int ref_hang_avg = 0;
const int State = 0;

boolean Flag = true;
boolean First = false;

void filter()   //The purpose of this function is to filter the output from the circuit.
{
for(i = 0; i < 30; i++) 
  {
    input = analogRead(A0);                   //reads value from Analog pin.
    median.in(input);                         //passes the value to the median filter.
    input = median.out();                     //takes the filtered value from the median filter.
    average = average + input;                //adds the values from median filter output.
  }
average = average / 30.0;                     //after adding the values this divides the value with the number of samples.
feedback = feedback * 0.8 + 0.2 * average;    //here we are doin feedback filtering for the average.
output = feedback;                            
}

void measure(){                   // This is the recognizes the pull-up.

filter();                         // calling the filter commented above.

sampleTime += sampling / 1000;    // takes timestamp for the sample and converts it to seconds.
  
if(output < ref_rest_avg - 50)    //compares if the output from filter(); is less than the reference from the bar when no one is hanging on it
  {
    if(Start==0)                  //check if there is no starting time for hanging. if not it copies the current time from MsTimer2
      {
        Start = sampleTime;        
      }    
    if(sampleTime - Start > 1)    //checks if the person has hung for a second. 
      {
        if(ref_hang_avg==0)       //if there's no current referense for hanging it will take 100 samples and calculates reference from their average.
        {
          for(int n = 0; n < 100; n++)
            {
              filter();
              ref_hang_avg += feedback;
            }
          ref_hang_avg /= 100;
        }  
                  
        if(output < ref_hang_avg - ref_hang_avg *0.05)  //compares if the output is less than 5% points under hanging reference. if yes it calls the Counter function.
          {
            Counter(output);
          }
       }
  }
else                                                    //if there's no person hanging from the bar the program will reset the values of hanging reference and start time.
  {
    Start = 0;
    ref_hang_avg = 0;
    Uptime = 0;
    Downtime = 0;
  }
Serial.print(output);
Serial.print("\t");
Serial.print(ref_hang_avg);
Serial.print("\t");
Serial.print(Downtime);
Serial.print("\t");
Serial.print(Uptime);
Serial.print("\t");
Serial.println(Count);


  }

  
int Counter(int value){                                        //This is the Counting function that compares the values and decides if the pull-up is countable or not.
  
  if(value > ref_hang_avg - ref_hang_avg * 0.08)               //checks if the person who's doing the pull-up has came down to starting position. if so it gives true to the boolean and exits from the subroutine
  {
    Downtime = sampleTime;
    Flag=true;
    return;
  }
  
  else                                                         //otherwise it will check if the flag is true.
  {
    if(Flag==true)
    {
      if(value < ref_hang_avg - ref_hang_avg * 0.15)           //if the flag is true and the person whos hanging on the bar has put enough force to the bar.
      {
        Count++;                                              //it will up the count by 1.
        Uptime = sampleTime;
        Flag = false;                                         //it will turn the flag to false so the pull-upper must go down before the next pull-up can be counted.
        return;                                               //exits the subroutine
      }
      else                                                    
      {
        return;
      }
    }
    else                                                    //if non of the above conditions have fulfilled the program will exit back to the filter();
    {
      return;
    }
  }
  return;
}

void FirstPackage(float Weight){

    DynamicJsonBuffer jsonBuffer(bufferSize);
    JsonObject& root = jsonBuffer.createObject();
    root["Type"] = "Initial";
    root["Weight"] = Weight;
    root.printTo(Output);
    Bluetooth.print(Output);
    Serial.println(Output);
    Output="";
}

void Pulls(float Uptime,float Downtime){

    DynamicJsonBuffer jsonBuffer(bufferSize);
    JsonObject& root = jsonBuffer.createObject();
    root["Type"] = "Measurement";
    root["Up"] = Uptime;
    root["Down"] = Downtime;
    root.printTo(Output);  
    Bluetooth.print(Output);
    Serial.println(Output); 
    Output="";                    
}

void Debounce(){
  if (Button == State){  // this function debounces the button so a single button press won't trigger multiple times 
      delay(200);
  }
}

void setup() {
  Bluetooth.begin(9600);
  Serial.begin(9600);
  pinMode(11,OUTPUT);
  digitalWrite(11,HIGH);
  pinMode(4,INPUT_PULLUP);                                  // this enables Arduino's internal pull up resistor so you don't need to get an external resistor to your button circuit
for(int n=0; n<150; n++)                                    //calculates the average reference from the bar when no one is hanging from it.
{
  filter();
  ref_rest_avg += feedback;

}
ref_rest_avg = ref_rest_avg / 150;
MsTimer2::set(sampling,measure);                            //sets the parameters to MsTimer2 and tells where the interupts will occur
MsTimer2::start();                                          //starts the MsTimer2
pinMode(A0,INPUT);
}


void loop() {
  /*
      previous = Button;
      Debounce();
      Button = digitalRead(4);

      
      if(Button == 0) {
         if (First == false){
            FirstPackage(68.5);
            First = true;
         }
         else{
          float up = millis()/1000;
          float down = up + 2;
          Pulls(up,down);
         }
      if (Button == 0 && previous == 0)
      {
        Shutter++;
        if( Shutter ==5){
        digitalWrite(11,LOW);
        delay(8000);
        digitalWrite(11,HIGH);
        Shutter = 0;
        First = false;
        }
      }
      }
      */
   
}
