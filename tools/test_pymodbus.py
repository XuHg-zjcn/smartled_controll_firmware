#!/usr/bin/env python3
#######################################################################
# 使用pymodbus库测试与设备通信
# Copyright (C) 2025  Xu Ruijun
#
# This program is free software: you can redistribute it and/or modify
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#######################################################################
import time
import numpy as np
from pymodbus.client import ModbusSerialClient
from pymodbus import FramerType

ADDR_LED = 0x0000

client = ModbusSerialClient('/dev/ttyUSB0', framer=FramerType.RTU, baudrate=250000, bytesize=8, parity='N', timeout=0.1)

print('读取当前亮度')
print(client.read_holding_registers(address=ADDR_LED, device_id=1))
time.sleep(1)

print('设置亮度')
print(client.write_registers(address=ADDR_LED, values=[100,], device_id=1))
time.sleep(1)

print('读取电压电流')
print(client.read_input_registers(address=0x0010, count=2, device_id=1))
time.sleep(1)

print('关灯')
print(client.write_coils(address=ADDR_LED, values=[0], device_id=1))
time.sleep(1)

print('开灯')
print(client.write_coil(address=ADDR_LED, value=1, device_id=1))
time.sleep(1)

print('持续设置亮度，呼吸灯效果')
seq = np.sin(np.linspace(0, np.pi, 200))**2
loop = 0
while True:
    loop += 1
    print('loop', loop)
    for i in seq:
        value = int(i*1024)
        client.write_register(address=ADDR_LED, value=value, device_id=1)
        time.sleep(0.01)
