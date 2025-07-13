# STM32F4 Monitor Utility

Утилита для мониторинга STM32F4 через виртуальный сом-порт (USB-UART).

## Функции

* Соединение через **UART (USB-UART)**
* Запрос напряжения, тока, температуры
* Статистика min/max/avg
* Обнаружение превышения порогов
* Логирование с метками времени в файл и на экран

---

## Формат обмена

### Запрос:

```
GET\n
```

### Ответ:

```
VOLT:<значение>;CURR:<значение>;TEMP:<значение>\n
```

#### Пример:

```
VOLT:12.5;CURR:4.3;TEMP:55.1
```

| Поле | Описание         | Тип   |
| ---- | ---------------- | ----- |
| VOLT | Напряжение (В)   | float |
| CURR | Ток (А)          | float |
| TEMP | Температура (°C) | float |

---

## Конфигурация

Файл конфигурации: `config/settings.ini` (JSON):

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

## Сборка

### Linux

```bash
make
```

## Зависимости

### Зависимости:

* `gcc` / `g++`
* `make` или `cmake`
* `socat` — эмуляция COM-портов
* `pyserial` — эмулятор на Python

### Установка в Linux:

```bash
sudo apt update
sudo apt install build-essential socat
pip3 install pyserial
```

---

## Тестирование

### Эмуляция виртуальных COM-портов:

```bash
sudo socat -d -d PTY,raw,echo=0,link=/dev/ttyUSB0 PTY,raw,echo=0,link=/dev/ttyUSB1
```

Запустите `stm32f4_monitor` с `/dev/ttyUSB0` и второй эмулятор/ручной ввод в `/dev/ttyUSB1`.

---

## Лицензия

Проект распространяется по лицензии MIT.
