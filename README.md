# esp-idf-RotaryEncoder
RotaryEncoder driver for esp-idf.

I ported from [this](https://haizairenmei.com/2019/12/05/rotary-encoder/).   
Calculating the direction of movement is very simple.   
This is the Japanese homepage.    

# Software requirements   
ESP-IDF V4.4/V5.0.   
ESP-IDF V5.0 is required when using ESP32-C2.   

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-RotaryEncoder
cd esp-idf-RotaryEncoder
idf.py set-target {esp32/esp32s2/esp32s3/esp32c2/esp32c3}
idf.py menuconfig
idf.py flash
```

# Configure for RotaryEncoder

![config-top-encoder1](https://user-images.githubusercontent.com/6020549/223957718-6e0f99b7-2d86-4a99-b29b-b041238d2ecc.jpg)
![config-top-encoder2](https://user-images.githubusercontent.com/6020549/223957750-3547a623-0d32-4a6b-be5e-e619eb68cdd4.jpg)


# Configure for Appication
There are 2 APIs.   
One is an API that uses CallBack and the other is an API that does not use CallBack.   

![config-top-app1](https://user-images.githubusercontent.com/6020549/223958108-88626cf1-d20f-4efa-a908-2d8b726eb27b.jpg)
![config-top-app2](https://user-images.githubusercontent.com/6020549/223958112-1be05f0a-b40f-4d8e-93dc-20b58dd47862.jpg)
![config-top-app3](https://user-images.githubusercontent.com/6020549/223958114-cbe27c66-4247-4ad0-bb46-b7d7e0a06e91.jpg)



# Wireing

|RotaryEncoder||ESP32|ESP32-S2/S3|ESP32-C2/C3|
|:-:|:-:|:-:|:-:|:-:|
|OUT A|--|GPIO15|GPIO1|GPIO1|
|OUT B|--|GPIO16|GPIO2|GPIO2|
|SWITCH|--|GPIO17|GPIO3|GPIO3|
|GND|--|GND|GND|GND|
|VCC|--|3.3V|3.3V|3.3V|

You can change any GPIO using menuconfig.   

![KY-04-Rotary-Encoder-Pinout](https://user-images.githubusercontent.com/6020549/223959379-0fa835f7-8e8f-454b-8147-7b02e5173fb5.jpg)

The RotaryEncoder has two forms, but the functionality is the same.
![rotary-encoder](https://user-images.githubusercontent.com/6020549/223959616-e994e8ad-d612-414a-999e-dfc46d2b193c.jpeg)

# ScreenShot   
The value of Count changes by 4 per click.   
![ScreenShot](https://user-images.githubusercontent.com/6020549/223961866-44fbde58-b12e-498a-84e4-c50929e373f5.jpg)


