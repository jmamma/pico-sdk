/* hw_config.c
Copyright 2021 Carl John Kugler III

Licensed under the Apache License, Version 2.0 (the License); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at

   http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
*/
/*

This file should be tailored to match the hardware design.

There should be one element of the spi[] array for each hardware SPI used.

There should be one element of the sd_cards[] array for each SD card slot.
The name is should correspond to the FatFs "logical drive" identifier.
(See http://elm-chan.org/fsw/ff/doc/filename.html#vol)
In general, this should correspond to the (zero origin) array index.
The rest of the constants will depend on the type of
socket, which SPI it is driven by, and how it is wired.

*/

#include <assert.h>
#include <string.h>
//
#include "my_debug.h"
//
#include "hw_config.h"
//
#include "ff.h" /* Obtains integer types */
//
#include "diskio.h" /* Declarations of disk functions */

/* 
This example assumes the following hardware configuration:

|       | SPI0  | GPIO  | Pin   | SPI       | MicroSD   | Description            | 
| ----- | ----  | ----- | ---   | --------  | --------- | ---------------------- |
| MISO  | RX    | 16    | 21    | DO        | DO        | Master In, Slave Out   |
| MOSI  | TX    | 19    | 25    | DI        | DI        | Master Out, Slave In   |
| SCK   | SCK   | 18    | 24    | SCLK      | CLK       | SPI clock              |
| CS0   | CSn   | 17    | 22    | SS or CS  | CS        | Slave (or Chip) Select |
| DET   |       | 22    | 29    |           | CD        | Card Detect            |
| GND   |       |       | 18,23 |           | GND       | Ground                 |
| 3v3   |       |       | 36    |           | 3v3       | 3.3 volt power         |

*/

// Hardware Configuration of SPI "objects"
// Note: multiple SD cards can be driven by one SPI if they use different slave
// selects.
static spi_t spis[] = {  // One for each SPI.
    {
        .hw_inst = spi1,  // SPI1
        .miso_gpio = 8,   // Your defined MISO pin
        .mosi_gpio = 11,  // Your defined MOSI pin
        .sck_gpio = 10,   // Your defined SCK pin
        .baud_rate = 4000 * 1000
    }};

// Hardware Configuration of the SD Card "objects"
static sd_card_t sd_cards[] = {  // One for each SD card
    {
        .pcName = "0:",   // Name used to mount device
        .spi = &spis[0],  // Pointer to the SPI driving this card
        .ss_gpio = 9,     // Your defined SS pin
        .use_card_detect = false,
        .card_detect_gpio = 22,  // Keeping your card detect pin
        .card_detected_true = 1
    }};
/* ********************************************************************** */
size_t sd_get_num() { return count_of(sd_cards); }
sd_card_t *sd_get_by_num(size_t num) {
    assert(num <= sd_get_num());
    if (num <= sd_get_num()) {
        return &sd_cards[num];
    } else {
        return NULL;
    }
}
size_t spi_get_num() { return count_of(spis); }
spi_t *spi_get_by_num(size_t num) {
    assert(num <= spi_get_num());
    if (num <= spi_get_num()) {
        return &spis[num];
    } else {
        return NULL;
    }
}

/* [] END OF FILE */