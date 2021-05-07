#ifndef INC_MAIN_HPP
#define INC_MAIN_HPP

#include "ota_arduinoota.hpp"

#include "drivers/drv-gpio.h"
#include "drivers/drv-relay.hpp"
#include "drivers/drv-led.hpp"
#include "drivers/drv-button.hpp"
#include "drivers/drv-i2c_master.hpp"
#include "drivers/drv-spi_bus.hpp"
#include "drivers/drv-sensors.hpp"

#include "services/svc-ntp.hpp"
#include "services/svc-mdns.hpp"
#include "services/svc-influxdb.hpp"
#include "services/svc-scheduler.hpp"
#include "services/svc-rpnrules.hpp"
#include "services/svc-homeassistant.hpp"
#include "services/svc-api.hpp"

#endif /* INC_MAIN_HPP */
