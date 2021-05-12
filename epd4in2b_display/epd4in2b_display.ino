/* Includes ------------------------------------------------------------------*/
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include <stdlib.h>
#include "WiFi.h"
#include "NTP.h"
#define STD_TIMEZONE_OFFSET +8 
#include <Time.h>
#include <Timezone.h>
#include <Wire.h> 
#include <dht11.h>
#include "ArduinoJson.h"
#include <HTTPClient.h>

  /*全局变量/对象*/
  //Create a new image cache named IMAGE_BW and fill it with white
  UBYTE *BlackImage, *RYImage; // Red or Yellow
  UWORD Imagesize = ((EPD_4IN2B_V2_WIDTH % 8 == 0) ? (EPD_4IN2B_V2_WIDTH / 8 ) : (EPD_4IN2B_V2_WIDTH / 8 + 1)) * EPD_4IN2B_V2_HEIGHT;
  //date and time
  String timedate,timetim="";
    /*自动重连*/
  bool AutoConfig()
{
    WiFi.begin();
    //如果觉得时间太长可改
    for (int i = 0; i < 20; i++)
    {
        int wstatus = WiFi.status();
        if (wstatus == WL_CONNECTED)
        {
          Serial.println("WIFI SmartConfig Success");
          Serial.printf("SSID:%s", WiFi.SSID().c_str());
          Serial.printf(", PSW:%s\r\n", WiFi.psk().c_str());
          Serial.print("LocalIP:");
          Serial.print(WiFi.localIP());
          Serial.print(" ,GateIP:");
          Serial.println(WiFi.gatewayIP());
          return true;
        }
        else
        {
          Serial.print("WIFI AutoConfig Waiting......");
          Serial.println(wstatus);
          delay(1000);
        }
    }
    Serial.println("WIFI AutoConfig Faild!" );
    return false;
}
  /*智能配网*/
  bool initwifi(){
    bool f=true;
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();
    //Wait for SmartConfig packet from mobile
    Serial.println("Waiting for SmartConfig.");
    while (!WiFi.smartConfigDone()) {
      delay(500);
      Serial.print(".");
    }
  
    Serial.println("");
    Serial.println("SmartConfig received.");
  
    //Wait for WiFi to connect to AP
    Serial.println("Waiting for WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("WiFi Connected.");
    Serial.println(WiFi.SSID());
    Serial.println(WiFi.psk());
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    return f;
  }
  /*时间日期数字格式化*/
  String pressNum(int num) {
    if (num < 10 )
      return "0" + String(num);
    else
      return String(num);
  }
/*get time*/
void updatetime() {
  int weekdays, days, months,years,minutes,hours; 
  TimeChangeRule *tcr;        // Pointer to the time change rule
  time_t utc = now();
  TimeChangeRule mySTD = {"", First,  Sun, Jan, 0, STD_TIMEZONE_OFFSET * 60};
  Timezone myTZ(mySTD, mySTD);
  time_t localTime = myTZ.toLocal(utc, &tcr);
  weekdays = weekday(localTime);
  if(weekdays!=7){
    weekdays-=1;
  }else{
    weekdays=1;
  }
  days = day(localTime);
  months = month(localTime);
  years = year(localTime);
  timedate+= pressNum(months);
  timedate+= "月 周" ;
  timedate+= weekdays;
  minutes =   minute(localTime);
  hours   =   hour(localTime) ;   //12 hour format use : hourFormat12(localTime)  isPM()/isAM()
  timetim="";
  timetim+=hours;
  timetim+="点";
  if(minutes<10){
    timetim+="0";
   }
  timetim+=minutes;
/*绘制时间*/
  Paint_DrawString_CN(310,22,const_cast<char *>(timetim.c_str()), &Font12CN,WHITE,BLACK);
  
  if(days<10){
    Paint_DrawImage(ling,301,156,48,66);
    if(days==1){
      Paint_DrawImage(yi,349,156,18,66);
    }else if(days==2){
      Paint_DrawImage(er,349,156,48,66);
    }else if(days==3){
      Paint_DrawImage(san,349,156,48,66);
    }else if(days==4){
      Paint_DrawImage(si,349,156,48,66);
    }else if(days==5){
      Paint_DrawImage(wu,349,156,48,66);
    }else if(days==6){
      Paint_DrawImage(liu,349,156,48,66);
    }else if(days==7){
      Paint_DrawImage(qi,349,156,48,66);
    }else if(days==8){
      Paint_DrawImage(ba,349,156,48,66);
    }else if(days==9){
      Paint_DrawImage(jiu,349,156,48,66);
    }
    
  }else if(days>=10&&days<20){
    Paint_DrawImage(yi,301,156,18,66);
    if(days==10){
      Paint_DrawImage(ling,349,156,48,66);
    }else if(days==11){
      Paint_DrawImage(yi,349,156,18,66);
    }else if(days==12){
      Paint_DrawImage(er,349,156,48,66);
    }else if(days==13){
      Paint_DrawImage(san,349,156,48,66);
    }else if(days==14){
      Paint_DrawImage(si,349,156,48,66);
    }else if(days==15){
      Paint_DrawImage(wu,349,156,48,66);
    }else if(days==16){
      Paint_DrawImage(liu,349,156,48,66);
    }else if(days==17){
      Paint_DrawImage(qi,349,156,48,66);
    }else if(days==18){
      Paint_DrawImage(ba,349,156,48,66);
    }else if(days==19){
      Paint_DrawImage(jiu,349,156,48,66);
    }
  }else if(days>=20&&days<30){
    Paint_DrawImage(er,301,156,48,66);
    if(days==20){
      Paint_DrawImage(ling,349,156,48,66);
    }else if(days==21){
      Paint_DrawImage(yi,349,156,18,66);
    }else if(days==22){
      Paint_DrawImage(er,349,156,48,66);
    }else if(days==23){
      Paint_DrawImage(san,349,156,48,66);
    }else if(days==24){
      Paint_DrawImage(si,349,156,48,66);
    }else if(days==25){
      Paint_DrawImage(wu,349,156,48,66);
    }else if(days==26){
      Paint_DrawImage(liu,349,156,48,66);
    }else if(days==27){
      Paint_DrawImage(qi,349,156,48,66);
    }else if(days==28){
      Paint_DrawImage(ba,349,156,48,66);
    }else if(days==29){
      Paint_DrawImage(jiu,349,156,48,66);
    }
  }else{
    Paint_DrawImage(san,301,156,48,66);
    if(days==30){
      Paint_DrawImage(ling,349,156,48,66);
    }else if(days==31){
      Paint_DrawImage(yi,349,156,18,66);
    }
  }
  /*绘制日期*/
  Paint_DrawString_CN(304,216,const_cast<char *>(timedate.c_str()), &Font12CN,BLACK ,WHITE);
}
  /*一言*/
typedef struct {
  char hitokoto[1024];//一言正文。编码方式 unicode。使用 utf-8。
  char from[60];//一言的出处
} HitokotoData_t;
HitokotoData_t Hitokoto;
void get_Hitokoto(void){
  HTTPClient http;
  String hitokostr,t="";
  http.begin("https://v1.hitokoto.cn/");
  int httpCode = http.GET(); 
  if (httpCode > 0) { 

    String payload = http.getString();
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(payload);

    if (!root.success()) {
      Serial.println("JSON parsing failed!");
    } else {
      if (strlen(root["hitokoto"]) > sizeof(Hitokoto.hitokoto)) {
        http.end();
      }
      strcpy(Hitokoto.hitokoto, root["hitokoto"]);
      if(strlen(root["from"])>0){
        strcpy(Hitokoto.from, root["from"]);
      }else{
        strcpy("佚名", root["from"]);
      }
      
    }
  }
  else {
    Serial.println("HTTP request failed！");
  }
  http.end(); 
  hitokostr=Hitokoto.hitokoto;
  Serial.println(hitokostr);
  Paint_DrawString_CN(0, 0, "一言", &Font12CN, WHITE, BLACK);
  Paint_DrawString_CN(33, 0, const_cast<char *>(hitokostr.c_str()), &Font12CN,BLACK, WHITE);
}
/*新闻:微博 观察者*/
const char* news_title;
void get_News(void){
  String temp="";
  int f=random(2);
  HTTPClient http;
  if(f==0){
    http.begin("http://api.rosysun.cn/weibo/");
    Paint_DrawImage(weibo,105,22,32,32);
    Paint_DrawString_CN(136,30,"微博热点", &Font12CN,BLACK,WHITE);
  }else{
    http.begin("http://api.rosysun.cn/guancha/");
    Paint_DrawImage(guancha,105,22,32,32);
    Paint_DrawString_CN(136,30,"观察者网焦点", &Font12CN,BLACK , WHITE);
  }
  int httpCode = http.GET(); 
  if (httpCode > 0) {
    String payload = http.getString();
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(payload);

    if (!root.success()) {
      Serial.println("JSON parsing failed!");
      return;
    } 
    JsonArray& titles = root["data"];
    /*取前11条信息*/
    for(int i=0;i<11;i++){
      JsonObject& title = titles[random(50)];
      news_title = title["title"];
      Paint_DrawPoint(1, 64+i*22, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
      /*取前17个字*/
      if(strlen(news_title)>51){
       for(int j=0;j<51;j++){
         temp+=news_title[j];
       }
       Paint_DrawString_CN(4,56+i*22, temp.c_str(), &Font12CN, BLACK ,WHITE);
       temp="";
     }else{
       Paint_DrawString_CN(4,56+i*22, news_title, &Font12CN, BLACK ,WHITE);
     }
    }
  }else {
    Serial.println("HTTP request failed！");
  }
  http.end(); 
}
/*室内温湿度*/
dht11 DHT11;
void bme(){
  DHT11.read(23);
  String temperature=String(DHT11.temperature)+"℃";
  String humidity = String(DHT11.humidity)+"%";
  Paint_DrawImage(wendu,301,234,32,32);
  Paint_DrawString_CN(334,241,temperature.c_str(), &Font12CN,WHITE,BLACK);
  Paint_DrawImage(shidu,305,268,32,32);
  Paint_DrawString_CN(334,275,humidity.c_str(), &Font12CN,WHITE,BLACK);
}
/*天气信息*/
const char* wt;//天气 温度 地区
const char* tp;
const char* lc;
String weatherstr="";
void get_weather(void){
  
  HTTPClient http;
  http.begin("https://api.seniverse.com/v3/weather/now.json?key=/*这里填写你的心知api*/&location=ip&language=zh-Hans&unit=c");
  int httpCode = http.GET(); 
  if (httpCode > 0) { 
    String payload = http.getString();
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(payload);

    if (!root.success()) {
      Serial.println("JSON parsing failed!");
      return ;
    } 
    JsonObject& results = root["results"][0];
    wt = results["now"]["text"];
    tp = results["now"]["temperature"];
    lc = results["location"]["name"];
    weatherstr = lc;
    weatherstr+=" ";
    weatherstr+= tp;
    weatherstr+="℃";
    String tubiao=wt;
    if(tubiao.equals("多云")||tubiao.equals("晴间多云")||tubiao.equals("大部多云")){
      Paint_DrawImage(duoyun,280,15,128,128);
    }else if(tubiao.equals("晴")){
      Paint_DrawImage(qing,280,15,128,128);
    }else if(tubiao.equals("阴")){
      Paint_DrawImage(yin,280,15,128,128);
    }else if(tubiao.equals("阵雨")){
      Paint_DrawImage(zhenyu,280,15,128,128);
    }else if(tubiao.equals("雷阵雨")){
      Paint_DrawImage(leizhenyu,280,15,128,128);
    }else if(tubiao.equals("雷阵雨伴有冰雹")){
      Paint_DrawImage(leizhenyubanyoubingbao,280,15,128,128);
    }else if(tubiao.equals("小雨")){
      Paint_DrawImage(xiaoyu,280,15,128,128);
    }else if(tubiao.equals("中雨")){
      Paint_DrawImage(zhongyu,280,15,128,128);
    }else if(tubiao.equals("大雨")){
      Paint_DrawImage(dayu,280,15,128,128);
    }else if(tubiao.equals("暴雨")||tubiao.equals("大暴雨")||tubiao.equals("特大暴雨")){
      Paint_DrawImage(baoyu,280,15,128,128);
    }else if(tubiao.equals("冻雨")){
      Paint_DrawImage(dongyu,280,15,128,128);
    }else if(tubiao.equals("雨夹雪")){
      Paint_DrawImage(yujiaxue,280,15,128,128);
    }else if(tubiao.equals("阵雪")){
      Paint_DrawImage(zhenxue,280,15,128,128);
    }else if(tubiao.equals("小雪")){
      Paint_DrawImage(xiaoxue,280,15,128,128);
    }else if(tubiao.equals("中雪")){
      Paint_DrawImage(zhongxue,280,15,128,128);
    }else if(tubiao.equals("大雪")){
      Paint_DrawImage(daxue,280,15,128,128);
    }else if(tubiao.equals("暴雪")){
      Paint_DrawImage(baoxue,280,15,128,128);
    }else if(tubiao.equals("浮尘")){
      Paint_DrawImage(fuchen,280,15,128,128);
    }else if(tubiao.equals("扬沙")||tubiao.equals("沙尘暴")||tubiao.equals("强沙尘暴")){
      Paint_DrawImage(shachenbao,280,15,128,128);
    }else if(tubiao.equals("雾")){
      Paint_DrawImage(dawu,280,15,128,128);
    }else if(tubiao.equals("霾")){
      Paint_DrawImage(mai,280,15,128,128);
    }else if(tubiao.equals("未知")){
      Paint_DrawImage(weizhi_,280,15,128,128);
    }
    /*位置图标*/
    Paint_DrawImage(weizhi,280,128,32,28);
    /*位置+温度*/
    Paint_DrawString_CN(312,140, const_cast<char *>(weatherstr.c_str()), &Font12CN, BLACK ,WHITE);
  }
  else {
    Serial.println("HTTP request failed！");
  }
  http.end(); 
}
// 绘制
  void show(){
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    /*天气*/
      get_weather();
    /*一言*/
    get_Hitokoto();
    /*画线*/
    Paint_DrawPoint(280, 25, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(280, 25, 280, 296, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawPoint(280, 296, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    /*日期*/
    updatetime();
    /*温湿度*/
    bme();
    /*热点新闻*/
    get_News();

    EPD_4IN2B_V2_Display(BlackImage, RYImage);
  }

void setup()
{
  pinMode(23,INPUT);
  // printf("EPD_4IN2B_V2_test Demo\r\n");
  DEV_Module_Init();

  // printf("e-Paper Init and Clear...\r\n");
  EPD_4IN2B_V2_Init();
  EPD_4IN2B_V2_Clear();
  DEV_Delay_ms(500);

  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for black memory...\r\n");
    while(1);
  }
  if ((RYImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for red memory...\r\n");
    while(1);
  }
  printf("NewImage:BlackImage and RYImage\r\n");
  Paint_NewImage(BlackImage, EPD_4IN2B_V2_WIDTH, EPD_4IN2B_V2_HEIGHT, 0, WHITE);
  Paint_NewImage(RYImage, EPD_4IN2B_V2_WIDTH, EPD_4IN2B_V2_HEIGHT, 0, WHITE);
  Paint_SelectImage(BlackImage);
  Paint_Clear(0xff);
  Paint_SelectImage(RYImage);
  Paint_Clear(0);

  
  // 开屏
  EPD_4IN2B_V2_Display(gImage_4in2bc_b, gImage_4in2bc_ry);
  DEV_Delay_ms(5000);
  if(!AutoConfig()){
    if(initwifi()){
      initNTP();
    }
  }else{
    initNTP();
  }
  EPD_4IN2B_V2_Display(BlackImage, RYImage);
  
  /*printf("Clear...\r\n");
  EPD_4IN2B_V2_Clear();

  printf("Goto Sleep...\r\n");
  EPD_4IN2B_V2_Sleep();
  free(BlackImage);
  free(RYImage);
  BlackImage = NULL;
  RYImage = NULL;*/
}

/* The main loop -------------------------------------------------------------*/
void loop()
{
  
  show();
  delay(100000);
}
