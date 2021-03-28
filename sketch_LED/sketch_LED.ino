#define BLINKER_PRINT Serial
#define BLINKER_WIFI
#include <WS2812FX.h>
#include <Blinker.h>
#include <Adafruit_NeoPixel.h>

char auth[] = "";
char ssid[] = "kzkl";
char pswd[] = "123456780";


#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN            1
#define NUMPIXELS      60


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


#define RGB_1 "col-blink"   //RGB拾色器组件
#define NUM_R "num-R"       //R数值返回组件
#define NUM_G "num-G"       //G数值返回组件
#define NUM_B "num-B"       //B数值返回组件
#define NUM_Time "num-time" //运行时间返回组件

#define BUTTONS "btn-switch"  //LED开关
#define BUTTON0 "btn-mode0"   //自定义模式
#define BUTTON1 "btn-mode1"   //彩虹模式
#define BUTTON2 "btn-mode2"   //呼吸模式
#define BUTTON3 "btn-mode3"   //双色模式
#define BUTTON4 "btn-mode4"   //动感模式
#define BUTTON5 "btn-mode5"   //呼吸模式
BlinkerButton btnS(BUTTONS);
BlinkerButton btn0(BUTTON0);
BlinkerButton btn1(BUTTON1);
BlinkerButton btn2(BUTTON2);
BlinkerButton btn3(BUTTON3);
BlinkerButton btn4(BUTTON4);
BlinkerButton btn5(BUTTON5);

BlinkerNumber numR(NUM_R);
BlinkerNumber numG(NUM_G);
BlinkerNumber numB(NUM_B);
BlinkerNumber numT(NUM_Time);
BlinkerRGB RGB1(RGB_1);

uint8_t wait=100;               //闪烁频率
uint8_t count=0;
uint8_t fadeAmount=1;
uint8_t myColors[8][3]={
        {3,168,158}, // Blue
        {227,23,13},//red
        {255,128,0}, // Orange
        {171,171,0}, // Yellow
        {64,224,205}, // Green
        {0,171,85}, // Aqua
        {138,43,226}, // Purple
        {171,0,85}, // Pink
  
  };
uint8_t cR=255,cG=255,cB=255,cW=100;       //记录RGB
uint8_t tR=255,tG=255,tB=255,tW=100;      //临时记录RGB
uint8_t crMode= 0;                   //控制当前模式
bool switchState=true;              //开关状态 默认打开


void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Blinker.attachHeartbeat(heartbeat);
    RGB1.attach(RGB1_callback);
    btnS.attach(btnS_callback);
    btn0.attach(btn0_callback);
    btn1.attach(btn1_callback);
    btn2.attach(btn2_callback);
    btn3.attach(btn3_callback);
//    btn4.attach(btn4_callback);
//    btn5.attach(btn5_callback);
//    btn6.attach(btn6_callback);
//    btn7.attach(btn7_callback);
//    btn8.attach(btn8_callback);
    update_ing();
    pixels.begin();
    pixels.show();
}

void loop()
{
    Blinker.run();
    switch(crMode){
      case 1:
        pixels.setBrightness(cW);
        mode1();
        break;
      case 2:
        mode2();
        break;
      case 3:
        pixels.setBrightness(cW);
        mode3();
        break;
      case 4:
        break;
      case 5:
        break;
      case 6:
        break;
      case 7:
        break;
      case 8:
        break;
      default:
        mode0();
        break;
      
    }
    
}
void btnS_callback(const String & state){
   Blinker.delay(13); 
   BLINKER_LOG("get button state: ", state);
   Blinker.delay(13); 
   if(state=="on"){
    switchState=true;
    btnS.text("已开启");
    btnS.color("#00ff00");
    btnS.print("on");
    cW=tW;
   }else if(state=="off"){
    switchState=false;
    btnS.text("已关闭");
    btnS.color("#292421");
    btnS.print("off");
    tW=cW;
    cW=0;
    pixels.clear();
    pixels.show();
   }
   
}
//自定义模式按钮回调
void btn0_callback(const String & state){
   Blinker.delay(13); 
   BLINKER_LOG("get button state: ", state);
   Blinker.delay(13); 
   crMode=0;
   update_ing();
   
}
//彩虹模式按钮回调
void btn1_callback(const String & state){
   Blinker.delay(13); 
   BLINKER_LOG("get button state: ", state);
   Blinker.delay(13); 
   crMode=1;
   update_ing();
   
}
//呼吸模式按钮回调
void btn2_callback(const String & state){
   Blinker.delay(13); 
   BLINKER_LOG("get button state: ", state);
   Blinker.delay(13); 
   crMode=2;
   update_ing();
   
}
//双色模式按钮回调
void btn3_callback(const String & state){
   Blinker.delay(13); 
   BLINKER_LOG("get button state: ", state);
   Blinker.delay(13); 
   //getNextColors();
   crMode=3;
   update_ing();
   
}
//void btn4_callback(const String & state){
//   Blinker.delay(13); 
//   BLINKER_LOG("get button state: ", state);
//   Blinker.delay(13); 
//   crMode=0;
//   update_ing();
//   
//}
void RGB1_callback(uint8_t r_value, uint8_t g_value, uint8_t b_value, uint8_t bright_value)
{
  if (crMode!=1&&crMode!=3)
  {
    Blinker.delay(13); 
    BLINKER_LOG("R: ", r_value);
    BLINKER_LOG("G: ", g_value);
    BLINKER_LOG("B: ", b_value);
    BLINKER_LOG("Bright: ", bright_value);
    Blinker.delay(13);
    cR = r_value;
    cG = g_value;
    cB = b_value;
    cW = bright_value;

    update_ing();
  }
  else if (crMode)
  {
    Blinker.delay(13);
    BLINKER_LOG("Bright: ", bright_value);
    Blinker.delay(13);
    cW = bright_value;
    pixels.setBrightness(cW);
    pixels.show();
  }
      
}

//自定义模式
void mode0() {
  pixels.fill(pixels.Color(cR, cG, cB), 0, NUMPIXELS);
  pixels.setBrightness(cW);
  pixels.show();
  
}
//彩虹模式
void mode1() {
  uint16_t i, j;
  for(j=0; j<256&&crMode==1&&switchState; j++) { 
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    Blinker.delay(20);
  }
}
//呼吸灯效果
void mode2(){
  

  pixels.fill(pixels.Color(cR, cG, cB), 0, NUMPIXELS);
  pixels.setBrightness(tW);
  pixels.show();
  tW = tW + fadeAmount;
  if (tW <= 30 || tW >= 200) {
    fadeAmount = -fadeAmount ; 
  }
  Blinker.delay(20);
   
}
//双色扩散模式
void mode3(){
  for(int i=0;i<8&&crMode==3&&switchState;i++){
    for(int j=0;j<8&&crMode==3&&switchState;j++){
      cR=myColors[i][0];
      cG=myColors[i][1];
      cB=myColors[i][2];
      tR=myColors[j][0];
      tG=myColors[j][1];
      tB=myColors[j][2];
      midColorWipe(pixels.Color(cR, cG, cB));
      midColorWipe(pixels.Color(tR, tG, tB));
    }

   }
  
}
//中间向两边扩散
void midColorWipe(uint32_t c){
  uint16_t i=(pixels.numPixels()/2)-1,j=(pixels.numPixels())/2;
  for(; i>=0&&j<pixels.numPixels()&&crMode==3&&switchState; i--,j++) {
    pixels.setPixelColor(i, c);
    pixels.setPixelColor(j, c);
    pixels.show();
    Blinker.delay(100);
  }
}
//
void getNextColors(){
  count=(count+1)%8;
  switch(count){
  case 0:
    cR=myColors[0][0];
    cG=myColors[0][1];
    cB=myColors[0][2];
    tR=myColors[7][0];
    tG=myColors[7][1];
    tB=myColors[7][2];
    break;
  case 1:
    cR=myColors[0][0];
    cG=myColors[0][1];
    cB=myColors[0][2];
    tR=myColors[2][0];
    tG=myColors[2][1];
    tB=myColors[2][2];
    break;
  case 2:
    cR=myColors[0][0];
    cG=myColors[0][1];
    cB=myColors[0][2];
    tR=myColors[1][0];
    tG=myColors[1][1];
    tB=myColors[1][2];
    break;
  case 3:
    cR=myColors[0][0];
    cG=myColors[0][1];
    cB=myColors[0][2];
    tR=myColors[6][0];
    tG=myColors[6][1];
    tB=myColors[6][2];
    break;
  case 4:
    cR=myColors[5][0];
    cG=myColors[5][1];
    cB=myColors[5][2];
    tR=myColors[3][0];
    tG=myColors[3][1];
    tB=myColors[3][2];
    break;
  }
}
// Input a value 0 to 255 to get a color value.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
void update_ing() //更新APP显示
{
  time_t run_time = Blinker.runTime();//同步运行时间
  numT.print(run_time/60);
  
  RGB1.print(cR, cG, cB, cW); //取色板及亮度组件更新
  numR.print(cR);
  numG.print(cG);
  numB.print(cB);
  if(switchState){
    btnS.color("#00ff00");
    btnS.text("已开启");
    btnS.print("on");
  }else{
    btnS.color("#292421");
    btnS.text("已关闭");
    btnS.print("off");
  }
  if (crMode == 0) //模式0
  {
    btn0.color("#00ff00");
    btn1.color("#292421"); 
    btn2.color("#292421"); 
    btn3.color("#292421"); 
//    btn4.color("#292421"); 
//    btn5.color("#292421"); 
//    btn6.color("#292421"); 
//    btn7.color("#292421"); 
//    btn8.color("#292421");
    btn0.print();
    btn1.print();
    btn2.print();
    btn3.print();
//    btn4.print();
//    btn5.print();
//    btn6.print();
//    btn7.print();
//    btn8.print();
  }
  else if (crMode == 1) //模式1
  {
    btn0.color("#292421");
    btn1.color("#00ff00"); 
    btn2.color("#292421"); 
    btn3.color("#292421"); 
//    btn4.color("#292421"); 
//    btn5.color("#292421"); 
//    btn6.color("#292421"); 
//    btn7.color("#292421"); 
//    btn8.color("#292421");
    btn0.print();
    btn1.print();
    btn2.print();
    btn3.print();
//    btn4.print();
//    btn5.print();
//    btn6.print();
//    btn7.print();
//    btn8.print();
  }
  else if (crMode == 2) //模式2
  {
    btn0.color("#292421");
    btn1.color("#292421"); 
    btn2.color("#1E90FF"); 
    btn3.color("#292421"); 
//    btn4.color("#292421"); 
//    btn5.color("#292421"); 
//    btn6.color("#292421"); 
//    btn7.color("#292421"); 
//    btn8.color("#292421");
    btn0.print();
    btn1.print();
    btn2.print();
    btn3.print();
//    btn4.print();
//    btn5.print();
//    btn6.print();
//    btn7.print();
//    btn8.print();
  }
  else if (crMode == 3) //模式3
  {
    btn0.color("#292421");
    btn1.color("#292421"); 
    btn2.color("#292421"); 
    btn3.color("#1E90FF"); 
//    btn4.color("#292421"); 
//    btn5.color("#292421"); 
//    btn6.color("#292421"); 
//    btn7.color("#292421"); 
//    btn8.color("#292421");
    btn0.print();
    btn1.print();
    btn2.print();
    btn3.print();
//    btn4.print();
//    btn5.print();
//    btn6.print();
//    btn7.print();
//    btn8.print();
  }
  /*else if (crMode == 4) //模式4
  {
    btn0.color("#292421");
    btn1.color("#292421"); 
    btn2.color("#292421"); 
    btn3.color("#292421"); 
    btn4.color("#1E90FF"); 
    btn5.color("#292421"); 
    btn6.color("#292421"); 
    btn7.color("#292421"); 
    btn8.color("#292421");
    btn0.print();
    btn1.print();
    btn2.print();
    btn3.print();
    btn4.print();
    btn5.print();
    btn6.print();
    btn7.print();
    btn8.print();
  }else if (crMode == 5) //模式5
  {
    btn0.color("#292421");
    btn1.color("#292421"); 
    btn2.color("#292421"); 
    btn3.color("#292421"); 
    btn4.color("#292421"); 
    btn5.color("#1E90FF"); 
    btn6.color("#292421"); 
    btn7.color("#292421"); 
    btn8.color("#292421");
    btn0.print();
    btn1.print();
    btn2.print();
    btn3.print();
    btn4.print();
    btn5.print();
    btn6.print();
    btn7.print();
    btn8.print();
  }
  else if (crMode == 6) //模式6
  {
    btn0.color("#292421");
    btn1.color("#292421"); 
    btn2.color("#292421"); 
    btn3.color("#292421"); 
    btn4.color("#292421"); 
    btn5.color("#292421"); 
    btn6.color("#1E90FF"); 
    btn7.color("#292421"); 
    btn8.color("#292421");
    btn0.print();
    btn1.print();
    btn2.print();
    btn3.print();
    btn4.print();
    btn5.print();
    btn6.print();
    btn7.print();
    btn8.print();
  }
  else if (crMode == 7) //模式7
  {
    btn0.color("#292421");
    btn1.color("#292421"); 
    btn2.color("#292421"); 
    btn3.color("#292421"); 
    btn4.color("#292421"); 
    btn5.color("#292421"); 
    btn6.color("#292421"); 
    btn7.color("#1E90FF"); 
    btn8.color("#292421");
    btn0.print();
    btn1.print();
    btn2.print();
    btn3.print();
    btn4.print();
    btn5.print();
    btn6.print();
    btn7.print();
    btn8.print();
  }
 else if (crMode == 8) //模式8
  {
    btn0.color("#292421");
    btn1.color("#292421"); 
    btn2.color("#292421"); 
    btn3.color("#292421"); 
    btn4.color("#292421"); 
    btn5.color("#292421"); 
    btn6.color("#292421"); 
    btn7.color("#292421"); 
    btn8.color("#1E90FF");
    btn0.print();
    btn1.print();
    btn2.print();
    btn3.print();
    btn4.print();
    btn5.print();
    btn6.print();
    btn7.print();
    btn8.print();
  }*/
}
void heartbeat() //心跳包
{
  Blinker.delay(13);
  BLINKER_LOG("同步!");
  Blinker.delay(13);
  update_ing();
}
void dataRead(const String &data) //未定义组件触发时的回调函数
{
  Blinker.delay(13);
  BLINKER_LOG("未定义组件触发: ", data);
  Blinker.delay(13);
  Blinker.vibrate();
  uint32_t BlinkerTime = millis();
  Blinker.delay(13);
  Blinker.print("millis", BlinkerTime);
  Blinker.delay(13);
}
