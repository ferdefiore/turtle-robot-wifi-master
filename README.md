# Minirobots Turtle Robot WiFi

```
 /'\_/`\  __          __             /\ \            /\ \__
/\      \/\_\    ___ /\_\  _ __   ___\ \ \____    ___\ \ ,_\   ____
\ \ \__\ \/\ \ /' _ `\/\ \/\`'__\/ __`\ \ '__`\  / __`\ \ \/  /',__\
 \ \ \_/\ \ \ \/\ \/\ \ \ \ \ \//\ \L\ \ \ \L\ \/\ \L\ \ \ \_/\__, `\
  \ \_\\ \_\ \_\ \_\ \_\ \_\ \_\\ \____/\ \_,__/\ \____/\ \__\/\____/
   \/_/ \/_/\/_/\/_/\/_/\/_/\/_/ \/___/  \/___/  \/___/  \/__/\/___/

  This is free software:
  you can redistribute it and/or modify it
  under the terms of the GPL version 3
  as published by the Free Software Foundation.
```

## Build instructions

Clone repository

```sh
git clone https://gitlab.com/minirobots/turtle-robot-wifi
```

Install platformio

```
pip install -U platformio
```

Build - You will find the generated binary at `.pioenvs/nodemcuv2/firmware.bin` 

```sh
make
```

Upload and monitor

```sh
make upload-n-monitor
```
