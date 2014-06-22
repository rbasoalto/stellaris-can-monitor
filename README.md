Toyota CAN bus monitor
======================
CAN bus monitor for a Toyota Yaris, using a TI Stellaris Launchpad, MCP2551 CAN line driver, displaying the data on a HD44780 characted LCD (connected via SPI with a 74hc595 shift register).

Presently, the following data is displayed:
- Individual speed of each wheel
- Engine RPM
- Average wheel speed
- Fuel consumption (L/h)
- Fuel economy (Km/L)

TODOs:
- [ ] Rewrite/replace example code from TI with crappy licenses
- [ ] Investigate further data to display. Some ideas:
  - [ ] Engine coolant/oil temperature
  - [ ] Engine load
  - [ ] External air temperature (maybe available from intake sensors)
  - [ ] Distance traveled (i.e. not needing to integrate speed data)
  - [ ] Absolute level of fuel in tank
  - [ ] ABS/ESP system status (e.g. light a warning LED when ABS is in action)

License
=======
The files not marked otherwise are released under the MIT license.
