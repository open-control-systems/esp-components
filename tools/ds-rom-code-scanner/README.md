# Dallas Semiconductor Rom Code Scanner

This project is designed to monitor the rom codes of all Dallas Semiconductor sensors connected to the configured GPIO.

**Set Up the Hardware:**

- Connect the sensor(s) to the ESP32 board.

**Configure the Software:**

- Ensure you have the ESP-IDF framework installed.
- Configure your `sdkconfig` as per your hardware setup.

**Build and Flash:**

```bash
idf.py build
idf.py flash
```

## Configuration

The firmware can be configured using the following command:

```bash
idf.py menuconfig
```

There are many configuration options, see options prefixed with "OCS_TOOLS_ROM_CODE_SCANNER_".

## Usage

**Power On the Device:**

Ensure the ESP32 board is powered on and connected to your computer.

**Monitor the Serial Output:**

```bash
idf.py monitor
```

This will display the result of the scan process.

## License

This project is licensed under the MPL 2.0 License - see the LICENSE file for details.
