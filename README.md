# esphome

WR3223 controller:

Files not needed:
- energy-meter.yaml 
- hochbeet-conroller.yaml


Features:

- error flags decoding
- store or restore default config from 'EPROM'
- auto detect of control panel -> if not attached, controller writes current state
- status update at least every 20 seconds (default: 10s, can be configured in the UI)
- several config options

Connection:
<br/>
<img src="https://github.com/schmurgel-tg/esphome/blob/main/images/20230101_174032.jpg?raw=true" alt="connection" width="300" height="600" />
Pinout:
<br/>
<img src="https://github.com/schmurgel-tg/esphome/blob/main/pinout.png?raw=true" alt="connection" width="300" height="600" />
<br/>


THX to:
https://github.com/frami/org.openhab.binding.wr3223
