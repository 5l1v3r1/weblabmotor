/*
This example shows how to read the temperature sensor on the Galileo's
onboard ADC, AD7298, using the iio (Industrial I/O) subsystem.

NOTE: This does not provide an accurate reading of the room tenmperature,
because the ADC is placed near the Quark SoC on the PCB, which gets quite warm.
As a result the ADC will always be a few degrees warmer than the actual room
temperature.
*/

char scale[4];
char raw[4];
char offset[4];

int raw_i;
int scale_i;
int offset_i;

int temp;

void setup() {

  Serial.begin(115200);
 
}

void loop() {
 
  temp = getADCTemp();
  Serial.print("Temperature is ");
  Serial.print(temp);
  Serial.println(" degrees celcius.");
  delay(1000);
 
}

int getADCTemp(){

  FILE *fp_raw;
  fp_raw = fopen("/sys/bus/iio/devices/iio:device0/in_temp0_raw", "r");     //read the values from scale, raw and offset files.
  fgets(raw, 4, fp_raw);                                                    //we need all three values, because the formula for
  fclose(fp_raw);                                                           //calulating the actual temperature in milli-degrees Celcius
                                                                            //is: TEMP = (RAW + OFFSET) * SCALE
  FILE *fp_scale;
  fp_scale = fopen("/sys/bus/iio/devices/iio:device0/in_temp0_scale", "r");
  fgets(scale, 4, fp_scale);
  fclose(fp_scale);
 
  FILE *fp_offset;
  fp_offset = fopen("/sys/bus/iio/devices/iio:device0/in_temp0_offset", "r");
  fgets(offset, 4, fp_offset);
  fclose(fp_offset);
  
  raw_i = atoi(raw);         //we have the values now, but they are in ASCII form-                                                      
  scale_i = atoi(scale);     //we need them as integers so we can use them for calculations.
  offset_i = atoi(offset);
 
  int temp = (raw_i + offset_i) * scale_i;  //Calculate temperature in milli-degrees celcius
  temp /= 1000;                         //divide by 1000 to convert to degrees celcius
  return temp; 
 
}

 

 
