/*
 * Copyright 2011 by Matt Mets <matt.mets@makerbot.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef COOLING_FAN_HH_
#define COOLING_FAN_HH_

#include "Heater.hh"
#include "Configuration.hh"

/// The cooling fan module represents a simple state machine that
/// implements a bang/bang control mechanism to turn a fan on when
/// the measured temperature goes above a given setpoint, and
/// off when it goes below the setpoint.
///
/// Porting notes:
/// The current implementation is limited in that it expects the fan
/// to be connected to the extruder motor (and controlled by
/// #setExtruderMotor(). In addition, it expects that a separate entity
/// is updating the heater, so that itfs current temperature reading is
/// always valid.
/// \ingroup SoftwareLibraries

#define DEFAULT_COOLING_FAN_SETPOINT_C  50
#define DEFAULT_COOLING_FAN_ENABLE      FAN_ENABLED


class CoolingFan {
private:
        /// Enable the cooling fan, setting it to run at full speed.
        void enableFan(bool enable);

        Heater& heater;  ///<  Heater module to read the current temperature from.
        uint16_t eeprom_base;   ///< Base address to read EEPROM configuration from
        Pin Fan_Pin;

        uint8_t enabled;   ///< If true, the control circuit actively controls the fan.
        uint8_t setPoint;   ///< Setpoint temperature, in degrees Celcius.

public:
        /// Create a new cooling fan controller instance.
        /// \param[in] heater Heater to use as an input to the controller
        /// \param[in] eeprom_base_in EEPROM address where the fan settings are stored.
        CoolingFan(Heater& heater,
                   const uint16_t eeprom_base_in, const Pin &fan);

        /// Enable or disable the cooling fan module, The fan state will not be modified
        /// until the next call to #manageCoolingFan().  When disabling, the fan will
        /// be disabled immediatel.
	void enable(uint8_t enable);

        /// Reset the cooling fan module, reloading it's default state (tempertaure
        /// and enabled status) from the EEPROM.
	void reset();

        /// Update the cooling fan status. This should be called periodically,
        /// possibly every time the temperature reading is updated.
        // TODO: rename this to something more generic (update()?)
	void manageCoolingFan();
};

#endif
