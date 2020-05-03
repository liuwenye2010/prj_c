/*
  Data types demo for uint32_t and uint8_t.
 */
 
uint32_t x;
#define N 1000
uint8_t sensor_data[N]={1,2,3,4,5,6,7,8,9,10,11,12};

void setup(){
}

void loop(){
  
  if(x<N)
    sensor_data[x++]=x;
}
