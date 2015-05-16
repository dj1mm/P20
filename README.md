# P20
Southampton Uni, Advance programming 2014 2015 P20 project. This project is about developing an interactive whiteboard program which allows 2 raspberry pis to communicate. When I draw on my program, my partner should see what i drew on my whiteboard, and vice versa ! Using Qt4.

- 2 Raspberry Pi B+
- Qt4, C++, pthread
- WiringPi

Project by: crh2g14 and jwaf1g14

![](https://cloud.githubusercontent.com/assets/12375343/7668025/7d4edb06-fc1a-11e4-966c-1838fc6471c1.jpg)
![](https://cloud.githubusercontent.com/assets/12375343/7668027/7d5b4d00-fc1a-11e4-80ee-6d8d4598c17a.jpg)
![](https://cloud.githubusercontent.com/assets/12375343/7668026/7d509176-fc1a-11e4-92bf-92b5dafca980.jpg)

Test on loopback

![](https://cloud.githubusercontent.com/assets/12375343/7668022/7d4cc35c-fc1a-11e4-9f13-3d5d827bbbe2.jpg)
![](https://cloud.githubusercontent.com/assets/12375343/7668021/7d488dfa-fc1a-11e4-81dc-93700c58221e.jpg)
![](https://cloud.githubusercontent.com/assets/12375343/7668023/7d4e0f8c-fc1a-11e4-8b20-dd88002cec36.jpg)
![](https://cloud.githubusercontent.com/assets/12375343/7668028/7d5f73e4-fc1a-11e4-814f-cb9ce0d0ad78.jpg)

At first we used only 2 wires to send and 2 wires to receive bits. However, it turned out that the communication went out of sync very easily. The delay() functions sometimes "slept" too much (real problem) or if we delayed longer (1s, the communication was successful, but it was slow). So we used a clocked system which would guarantee that both raspberry pies are same stage of communication

- [Sleep functions are not accurate enough](https://www.raspberrypi.org/forums/viewtopic.php?t=10479&p=118168)
- [Accurate delays ?](https://projects.drogon.net/accurate-delays-on-the-raspberry-pi/)

# Improvements ?
- Implement a more robust communication protocol. eg REQ, ACK, NACK ...
- Implement a faster communication. eg UART or SPI
- More commands !
