# esphome

WR3223 controller:

Files not needed:
- energy-meter.yaml 
- hochbeet-conroller.yaml


Features:

- error flags decoding
- 
- store or restore default config from 'EPROM'
- auto detect of control panel -> if not attached, controller will write current state
- status update at least every 20 seconds (default: 10s, can be configured in the UI)

Connection:
![Connection](https://community.home-assistant.io/t/hermes-electronic-wr3223-ventilation-controller-esp32/485307/2)

THX to:
https://github.com/frami/org.openhab.binding.wr3223
