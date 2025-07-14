# STM32F4 Monitor Utility

A cross-platform utility for monitoring STM32F4 power modules via virtual COM port (USB-UART).

## Features

* **UART connection** (virtual COM port)
* Requests **voltage, current, temperature**
* Collects min/max/average statistics
* Detects threshold violations
* Logs data with timestamps to file and console

---

## 📄 Communication Protocol

### Request:

```
GET\n
```

### Response:

```
VOLT:<value>;CURR:<value>;TEMP:<value>\n
```

#### Example:

```
VOLT:12.5;CURR:4.3;TEMP:55.1
```

| Field | Description      | Type  |
| ----- | ---------------- | ----- |
| VOLT  | Voltage (V)      | float |
| CURR  | Current (A)      | float |
| TEMP  | Temperature (°C) | float |

---

## Configuration

Configuration file: `config/settings.ini` (JSON format):

```json
{
  "address": "/dev/ttyUSB0",
  "port": 0,
  "interval": 1,
  "log_path": "logs/telemetry.log",
  "thresholds": {
    "current": 8.0,
    "temperature": 60.0
  }
}
```

---

## Build Instructions

### Linux

```bash
make
```

## Dependencies

### Required:

* `gcc` / `g++`
* `make` or `cmake`
* `socat` — for virtual COM port emulation
* `pyserial` — for Python device emulator (optional)

### Install on Linux:

```bash
sudo apt update
sudo apt install build-essential socat python3 python3-full python3-pip python3-requests python3-venv
# For Debian; if you wish to install a non-Debian-packaged Python package
python3 -m venv .vevn 
source .venv/bin/activate
pip install requests
#
pip3 install pyserial
```

---

## Testing

### Virtual COM port emulation:

Create linked virtual UARTs:

```bash
sudo socat -d -d PTY,raw,echo=0,link=/dev/ttyUSB0 PTY,raw,echo=0,link=/dev/ttyUSB1
```
Connect an emulator `src/test_script.py` on `/dev/ttyUSB1`
Run `stm32f4_monitor` using `/dev/ttyUSB0

---

## 🔗 Licensing

This project is released under the MIT license.
