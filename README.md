# OpenLedger

OpenLedger aims to build an open implementation of Hardware Wallets, made of independent small modules with low decoupled dependencies, running on an RTOS.  
  
The current implementation runs on a ESP32 with an st7789 LCD screen, but looks ahead to decouple this requirements.  

## MVP

ESP32 ST7789 Ethereum Hardware wallet
 - Creating Ethereum Wallets.
 - Importing Ethereum wallets through paper codes or private keys.
 - Exporting Ethereum wallets.
 - managing your PIN.
 - Storing all information encrypted.
 - Accepting/Denying Transactions.
 - Showing Transaction information.
 - Storing and showing the Transactions History.
 - OTA Updates through wifi with a pre-defined Public OTA Server Credentials.
 - Bluetooth connection.
 - BLE Wake up notifications.

This will need defining a Protocol for communication, and forking a known app, adding the implementation to this protocol, and building it.  

## Work Status
- ~~Base code setup~~  
- ~~st7789 Driver~~ 
- **Main menu**
- ~~Serial port navigation~~
- ~~Font rendering library, Roboto, FontAwesome inclusion~~
- **MVP Interface Design**  
- **Layout system** 
- Key Manager System
- Manage PIN: Layout, Implementation.
- Physical navigation
- Creating Ethereum Wallets: Layout, Implementation.
- Forking app and adding specific connection (Metamask?) on Android.
- Accepting/Denying Transactions: **Layout**, Implementation.
- Storing and showing the Transactions History: Layout, Implementation.
- ... TBD
  
~~Done features~~, **Currently working features**, expected features  

## External Resources

[View the current design on Figma
![](https://i.ibb.co/T2v88vD/Screenshot-from-2021-04-29-07-18-56.png)
](https://www.figma.com/file/7RKOkGecLLjVIPMaE8YdY0/OpenLedger?node-id=199%3A326)  
  
[Implementation running @ 199ce52 video
![](https://i.ibb.co/44bS8yQ/Screenshot-from-2021-04-29-07-54-31.png)
](https://streamable.com/g96dd5)  
