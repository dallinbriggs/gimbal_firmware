/*
 * Copyright (c) 2017, James Jackson and Trey Henrichsen
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/*
 * Driver for the Maxbotix I2CXL-MaxSonar-EZ series sonar.
 * This has been made for and tested with the MB1242 sonar module,
 * but should work for the MB1202, MB1212, MB1222, and MB1232.
 */
#ifndef I2C_SONAR_H
#define I2C_SONAR_H

#include <cstdint>

#include "i2c.h"

//These 3 constants come from the spec sheet for the sonar
#define DEFAULT_ADDRESS 112
#define DEFAULT_REGISTER 0xFF
#define PING_COMMAND 81

//Uncomment this to return raw distances instead of calibrated distances
//#define MB1242_RAW

#define UPDATE_WAIT_MILLIS 50 //minimum time between calls of async_update that actually do something
//50 ms is chosen because it will read only once per two calls to async_update, at max,
//and the spec sheet for the sonar recomends waiting 100 ms in between pings

class I2CSonar
{
private:
    uint32_t last_update_;//The last time that async_update was called
    float value_;//the latest reading from the sensor
    bool new_data_;//Whether or not new data is ready to be returned
    I2C *i2c_;//The i2c object used for communication
    bool ready_to_ping_;//Whether the sensor is ready to make another measurement
    uint8_t buffer_[2];//for recieving data from the sensor
public:
    I2CSonar (I2C *i2c_);
    float async_read();//Returns the most recent reading, converted to meters, or 0 if there is none
    void async_update();//Tries to either start a measurement, or read it from the sensor
    //async_update will do nothing if it has done something in the last UPDATE_WAIT_MILLIS ms
    //Calling it more frequently won't break anything

    //Call backs. For internal use only
    void cb_start_read();//callback after the measure command has been sent to the sensor
    void cb_finished_read();//callback after reading from the sensor has finished
};


#endif
