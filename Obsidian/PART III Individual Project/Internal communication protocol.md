## Overview
Commands are sent to all modules at once via daisy chaining and shift registers. To simplify the system, there is not a NOP instruction. This means that appropriate commands have to be sent to every module to prevent unwanted behaviours. Each module contains an 8-bit shift register, and so commands are 8-bits in length.

## Data encoding
#### Quad button
Identifier: 0b00000001

| Input |                     | Output |                     |
| ----- | ------------------- | ------ | ------------------- |
| D7    | _unused_            | D7     | _unused_            |
| D6    | _unused_            | D6     | _unused_            |
| D5    | _unused_            | D5     | _unused_            |
| D4    | _unused_            | D4     | _unused_            |
| D3    | Button 3 (br) light | D3     | Button 3 (br) state |
| D2    | Button 2 (bl) light | D2     | Button 2 (bl) state |
| D1    | Button 1 (tr) light | D1     | Button 1 (tr) state |
| D0    | Button 0 (tl) light | D0     | Button 0 (tl) state |
#### Single knob
Identifier: 0b00000011

| Input |          | Output |            |
| ----- | -------- | ------ | ---------- |
| D7    | _unused_ | D7     | _unused_   |
| D6    | _unused_ | D6     | Encoder B6 |
| D5    | _unused_ | D5     | Encoder B5 |
| D4    | _unused_ | D4     | Encoder B4 |
| D3    | _unused_ | D3     | Encoder B3 |
| D2    | _unused_ | D2     | Encoder B2 |
| D1    | _unused_ | D1     | Encoder B1 |
| D0    | _unused_ | D0     | Encoder B0 |
#### Seven segment
Identifier: 0b00000100

| Input |        | Output |          |
| ----- | ------ | ------ | -------- |
| D7    | Seg DP | D7     | _unused_ |
| D6    | Seg G  | D6     | _unused_ |
| D5    | Seg F  | D5     | _unused_ |
| D4    | Seg E  | D4     | _unused_ |
| D3    | Seg D  | D3     | _unused_ |
| D2    | Seg C  | D2     | _unused_ |
| D1    | Seg B  | D1     | _unused_ |
| D0    | Seg A  | D0     | _unused_ |
#### Quad light
Identifier: 0b00000101

| Input |           | Output |          |
| ----- | --------- | ------ | -------- |
| D7    | _unused_  | D7     | _unused_ |
| D6    | Light 1 R | D6     | _unused_ |
| D5    | Light 1 G | D5     | _unused_ |
| D4    | Light 1 B | D4     | _unused_ |
| D3    | _unused_  | D3     | _unused_ |
| D2    | Light 2 R | D2     | _unused_ |
| D1    | Light 2 G | D1     | _unused_ |
| D0    | Light 2 B | D0     | _unused_ |
