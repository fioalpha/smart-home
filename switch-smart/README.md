# DEVICE SWITCH

## Description 
This component develop with the `esp8206 esp01 512 flash memory`, contain two inpout to switch with pull down.
This component connect with `MQTT`, with the topic:
 - `*tele/LWT` - publish  when device connect on MQTT
 - `*/cmnd/POWER` - publish state the device when changing the state
 - `*/stat/RESULT` - each 3000 seconds (5minuts) publish stat current of device
 - `*/stat/CURRENT` - subscribe this topic to check if states change
 - `switch/PLACE/*` - the `PLACE` need change to place where will install  

### Model of data
Send power of devices
```javascript
    {
        "power": "ON"
    }
```