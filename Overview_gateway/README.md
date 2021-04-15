<h1 align="center">Health Overview Gateway</h1>

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)



<p align="center">
<img align="center" width=40% src="../Support/readme_assets/gateway.jpg">
</p>
<br>

# Description
The gateway hardware consists in fact of two components, a nordic development kit and a raspberry pi single board computer.
<br>
The Nordic dev kit is responsible to receive and transmit data packet to the Open Thread mesh network.
<br>
<br>
(MESH RF NETWORK) <--RF--> (Nordic Dev Kit) <--UART--> (R. Pi) <--Wifi/Eth--> (Backend)
<br>
<br>
This folder countain the source code written in Python run on the raspberry pi acting as the internet gateway of the Health Overview system.
<br>
<br>

# Details
The python application running on the raspberry pi enable for the following processes:
- Communicate with he Nordic dev kit through UART
- Ring-buffer the data from the Nordic dev kit
- Relay messages to the backend
- Listen for notification from the backend and relay them to the Nordic dev kit
- Get credentials from the backend to authenticate and authorize the connection
- Transmit data through HTTPS

#