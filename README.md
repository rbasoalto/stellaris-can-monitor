Toyota CAN bus monitor
======================
CAN bus monitor for a Toyota Yaris, using a TI Stellaris Launchpad, MCP2551 CAN line driver, displaying the data on a HD44780 characted LCD (connected via SPI with a 74hc595 shift register).

Presently, the following data is displayed:
- Individual speed of each wheel
- Engine RPM
- Fuel consumption (L/h)

TODOs:
- [x] Show data in reasonable units (speed is in 1e-2 km/h, fuel consumption is in 1e-2 l/h)
- [x] Show fuel consumption per distance (km/l or l/100km)
- [x] Show _one_ consolidated speed value
- [ ] Rewrite/replace example code from TI with crappy licenses

License
=======
The files not marked otherwise are released under the MIT license.
