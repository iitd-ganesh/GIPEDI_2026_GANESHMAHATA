<!-- ## Alarm based motor on/off system

1. We have conneced a push botton at D1 (when water comes switch will sort and continuously send High signal) and when water supply is off it will send low signal or disconnected function.
2. if water supply is there then Turn ON (pump) led Connected to D3 pin. set an alarm of 1 hour later to the time pump is on. this 1h feature is working only if water supply is continue for > 1h
3. if water supply(high continuous signal) is off with in < 1h then  . turn off the (pump) within 2 sec also led D3 pin.
4. DS1307-RTC connected and oled conned to the A4 AND A5 PIN OF UNO BOARD


Display used SSD1306 OLED- take standard size and I2C address
---Display Real time
---Diaplay Water supply 
---Diaplay Pump Staus(ON/OFF) -->

