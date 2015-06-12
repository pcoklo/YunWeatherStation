#include <FileIO.h>
#include <Bridge.h>
#include <Process.h>
#include <idDHT11.h>

int idDHT11pin = 3; //Digital pin for comunications
int idDHT11intNumber = 0; //interrupt number (must be the one that use the previus defined pin (see table above)

int sensorValue = 0;        // value read from the Sensor
int outputValue = 0;        // Mapped Value for the serial output
void dht11_wrapper(); // must be declared before the lib initialization

// Lib instantiate
idDHT11 DHT11(idDHT11pin,idDHT11intNumber,dht11_wrapper);

void dht11_wrapper() {
  DHT11.isrCallback();
}

void setup() {
  // Initialize the Bridge and the Serial
  pinMode(13, OUTPUT);
  Bridge.begin();
  FileSystem.begin();
  Serial.begin(9600);

  digitalWrite(13, HIGH);
  // make a string that start with a timestamp for assembling the data to log:
  String dataString;
  dataString += getTimeStamp();
  dataString += "\t";

  //svjetlost
  int sensorValue = analogRead(A3);
  double Vout = sensorValue*0.0048828125;
  int lux = 500 / (10*((5-Vout)/Vout));
  dataString+=lux; dataString += "\t";
  
  //dohvaćanje temperature i vlage
  DHT11.acquire();
  while (DHT11.acquiring());
  int result = DHT11.getStatus();
  
  //temperatura
  int dht = DHT11.getCelsius();
  dataString+=dht; dataString+="\t";
  
  //tlak
  sensorValue = analogRead(A2);
  outputValue = map(sensorValue, 640, 1023, 0, 255);
  dataString+=sensorValue; dataString+="\t";
  
  //vlaga
  dht = DHT11.getHumidity();
  dataString+=dht; dataString+="\t";
  
  //kiša
  sensorValue = analogRead(A0);
  // read three sensors and append to the string:
  if(sensorValue < 700) dataString+="Pada";
  else dataString+="Ne pada";

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  // The FileSystem card is mounted at the following "/mnt/FileSystema1"
  File dataFile = FileSystem.open("/tmp/datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(dataString);
    dataFile.close();
  }
  
  digitalWrite(13, LOW);
}

void loop () {
}

// This function return a string with the time stamp
String getTimeStamp() {
  String result;
  Process time;
  // date is a command line utility to get the date and the time
  // in different formats depending on the additional parameter
  time.begin("date");
  time.addParameter("+%D-%T");  // parameters: D for the complete date mm/dd/yy
  //T for the time hh:mm:ss
  time.run();  // run the command

  // read the output of the command
  bool flag = 0;
  while (time.available() > 0) {
    char c = time.read();
    if(c != '\n' && flag) result += c;
    if(c == '-' && !flag) flag = true;
  }

  return result;
}