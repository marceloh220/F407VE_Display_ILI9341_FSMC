# F407VE_Display_ILI9341_FSMC
 Driver to Display ILI9341 on BLACK_F407VE, board based in uController STM32F407VE, using 16-bits FSMC interface

Adafruit_GFX compatibility but with no dependence

Uses:
 - STM HAL
 - MbedOs
 - DigitalOut  (To reset if connected)
 - PwmOut      (To backlight bright control)
 
Removing the DigitalOut, PwmOut and ThisThread::sleep_for can be ported to stmCubeIDE

Default connections of board:
 - FSMC data bus 16-bits (To 8-bits the library must be modified)
 - CS  = NE1   (Can be change in constructor)
 - RS  = A18   (Can be change in constructor)
 - RST = NC    (Not Connected, uses soft reset. Can be change in constructor)
 - BL  = PB_1  (Can be change in constructor to another PwmOut pin)

Display Default:
 - Width  = 240 (Can be change in constructor)
 - Height = 340 (Can be change in constructor)
 - Orientation Portrait (Can be changed in constructor or using method available)

Font size integrate (STM default fonts):
 - Font 8px
 - Font 12px
 - Font 16px
 - Font 20px
 - Font 24px

Times benchmark:

   <img src="https://github.com/marceloh220/F407VE_Display_ILI9341_FSMC/blob/main/example/times.jpg" width="340" height="460" />
