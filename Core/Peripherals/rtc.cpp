//
// Created by Natalia Bidzinska on 23.09.2020.
//

#include "rtc.h"
#include "obc.h"

namespace hw
{

Rtc::Rtc(const I2CBus* i2c, uint8_t address)
        : i2c_(i2c),
          slaveAddress_(address)
{}

void Rtc::init() const
{
#if HW_RTC_SET_TIME
    TimeAndDate data;
    data.second  = HW_RTC_SECOND;
    data.minute  = HW_RTC_MINUTE;
    data.hour    = HW_RTC_HOUR;
    data.weekday = HW_RTC_WEEKDAY;
    data.day     = HW_RTC_DAY;
    data.month   = HW_RTC_MONTH;
    data.year    = HW_RTC_YEAR;

    setTimeAndDate(data);
#endif
    readTimeAndDateTask_.addToScheduler();
}

uint8_t Rtc::convertBcdToDec(const uint8_t bcdData)
{
    return ((bcdData >> 4) * 10) + (bcdData & 0x0F);
}

uint8_t Rtc::convertDecToBcd(const uint8_t decData)
{
    return ((decData / 10) << 4) | (decData % 10);
}

void Rtc::setTimeAndDate(const TimeAndDate& timeAndDate) const
{
    TimeAndDate converted;
    for(size_t i = 0; i < converted.array.size(); ++i)
    {
        converted.array[i] = convertDecToBcd(timeAndDate.array[i]);
    }
    i2c_->memoryWrite<0x00>(slaveAddress_, converted.array.data(), converted.array.size());
}

void Rtc::readTimeAndDate()
{
    if(i2c_->memoryRead<0x00>(slaveAddress_, buffer_.array.data(), buffer_.array.size()) == BusResult::Ok)
    {
        for(auto& elem : buffer_.array)
        {
            elem = convertBcdToDec(elem);
        }
    }
}

void Rtc::readTimeAndDateTaskFunction(void *args)
{
    (void)args;
    Rtc &rtc = obc().hardware.rtc;
    while(true)
    {
        rtc.readTimeAndDate();
        os::Task::delay(500);
    }
}

const TimeAndDate &Rtc::getTimeAndDateBuffer() const
{
    return buffer_;
}

}