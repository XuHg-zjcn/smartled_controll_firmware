#!/usr/bin/env python3
#######################################################################
# 串口曼彻斯特编码上位机程序
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
import serial
import time
import crc

tab_enc = [
  0xaa, 0xa9, 0xa6, 0xa5,
  0x9a, 0x99, 0x96, 0x95,
  0x6a, 0x69, 0x66, 0x65,
  0x5a, 0x59, 0x56, 0x55
]

dict_dec = {
  0xaa:0x0, 0xa9:0x1, 0xa6:0x2, 0xa5:0x3,
  0x9a:0x4, 0x99:0x5, 0x96:0x6, 0x95:0x7,
  0x6a:0x8, 0x69:0x9, 0x66:0xa, 0x65:0xb,
  0x5a:0xc, 0x59:0xd, 0x56:0xe, 0x55:0xf
}

crc_calc = crc.Calculator(crc.Crc16.MODBUS)

def manchester_enc(data):
    ba = bytearray()
    for byte in data:
        ba.append(tab_enc[byte&0xf])
        ba.append(tab_enc[(byte>>4)&0xf])
    return bytes(ba)

def manchester_dec(data):
    ba = bytearray()
    for l, h in zip(data[::2], data[1::2]):
        if l not in dict_dec or h not in dict_dec:
            byte = 0
        else:
            byte = dict_dec[l]+(dict_dec[h]<<4)
        ba.append(byte)
    return bytes(ba)

devpath = '/dev/ttyUSB0'
baudrate = 500000
s = serial.Serial(devpath, baudrate, timeout=0.1)

i = 0
while True:
    compval = (i%32)*(i%32)
    cmd = b'\x01\x06\x00\x03' + int.to_bytes(compval, 2, 'big')
    checksum = crc_calc.checksum(cmd)
    cmd_withcrc = cmd + int.to_bytes(checksum, 2, 'little')
    s.write(b'\xac' + manchester_enc(cmd_withcrc))
    data = s.read(64)
    if len(data) != 0:
        print(manchester_dec(data).decode())
    i += 1
