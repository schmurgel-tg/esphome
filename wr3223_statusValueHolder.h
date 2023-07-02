#include <EEPROM.h>

namespace WR3223 {

    enum WR3223EnumStatusSW : char
    {
        OFF =                   0b00000000,
        HEAT_PUMP_OFF =         0b00000001,
        VENT_LEVEL_1 =          0b00000110,
        VENT_LEVEL_2 =          0b00000010,
        VENT_LEVEL_3 =          0b00000100,
        ADDITIONAL_HEATING_OFF= 0b00001000,
        VENT_LEVEL_0 =          0b00010000,            
        COOLING_OFF =           0b00100000,
        
        STATUS_MASK_SW =           0b00111111
    };

    enum WR3223EnumStatusTa : char
    {
        MALFUNCTION =               0b00010000,
        HEAT_PUMP_STATUS =          0b00100000,
        ADDITIONAL_HEATER_STATUS =  0b01000000,

        STATUS_MASK_TA =            0b01110000
    };

    class WR3223StatusValueHolder 
    {
        const int swEepromAdress = 0;

        /// @brief Der Wert für das SW Kommando
        int stateValueSW = 0;       
        int stateValueTa = 0;

    public:
        /**
         * @param "Wärmepumpe ein (bei Anlagen mit Wärmepumpe)"
         */
        void setHeatPumpOn(bool heatPumpOn) 
        {
            if(heatPumpOn)
                stateValueSW &= ~WR3223EnumStatusSW::HEAT_PUMP_OFF;
            else
                stateValueSW |= WR3223EnumStatusSW::HEAT_PUMP_OFF;            
        }

        /**
         * @param "Lüftungsstufe 0-3, 0=Aus"
         */
        void setVentilationLevel(int ventilationLevel) 
        {
            // erste einmal den alten Vent_level weg
            stateValueSW &= ~(WR3223EnumStatusSW::VENT_LEVEL_0|
                                WR3223EnumStatusSW::VENT_LEVEL_1|
                                WR3223EnumStatusSW::VENT_LEVEL_2|
                                WR3223EnumStatusSW::VENT_LEVEL_3);
            // dann den neuen            
            switch (ventilationLevel)
            {
                case 1:
                    stateValueSW |= WR3223EnumStatusSW::VENT_LEVEL_1;
                    break;
                case 2:
                    stateValueSW |= WR3223EnumStatusSW::VENT_LEVEL_2;
                    break;
                case 3:
                    stateValueSW |= WR3223EnumStatusSW::VENT_LEVEL_3;
                    break;
                default:
                    stateValueSW |= WR3223EnumStatusSW::VENT_LEVEL_0;
                    break;
            }            
        }

        /**
         * @param "Zusatzheizung ein"
         */
        void setAdditionalHeatingOn(bool additionalHeatingOn)
        {
            if(additionalHeatingOn)
                stateValueSW &= ~WR3223EnumStatusSW::ADDITIONAL_HEATING_OFF;
            else
                stateValueSW |= WR3223EnumStatusSW::ADDITIONAL_HEATING_OFF;            
        }

        /**
         * @param "kühlen (bei Anlagen mit Wärmepumpe)"
         */
        void setCoolingOn(bool coolingOn) 
        {
            if(coolingOn)
            {
                stateValueSW &= ~WR3223EnumStatusSW::COOLING_OFF;
                stateValueSW &= ~WR3223EnumStatusSW::HEAT_PUMP_OFF;
            }
            else
            {
                stateValueSW |= WR3223EnumStatusSW::COOLING_OFF;      
                stateValueSW |= WR3223EnumStatusSW::HEAT_PUMP_OFF;       
            }
        }

        bool getCoolingOnStatus()
        {
            return !(stateValueSW & WR3223EnumStatusSW::COOLING_OFF);
        }

        /**
         * @return Störung
         */
        bool isMalfunction() 
        {
            return (stateValueTa & WR3223EnumStatusTa::MALFUNCTION);
        }

        /**
         * @return Zusatzheizung An/Aus
         */
        bool getAdditionalHeatingOnStatus() 
        {
            return (stateValueTa & WR3223EnumStatusTa::ADDITIONAL_HEATER_STATUS);
        }

        /**
         * @return Wärmepumpe An/Aus
         */
        bool getHeatPumpOnStatus() 
        {
            return (stateValueTa & WR3223EnumStatusTa::HEAT_PUMP_STATUS);
        }

        // /**
        //  * @return "Kühlung An/Aus
        //  */
        // bool getCoolingOnStatus() {
        //     return coolingOn;
        // }

        // stellt den letzten Zustand aus dem Speicher wieder her
        void restore_state_sw()
        {
            EEPROM.begin(4);            
            ESP_LOGD("EEPROM", "current state: %i (sizeof: %i)", stateValueSW, sizeof(stateValueSW));
            EEPROM.get(swEepromAdress, stateValueSW);
            ESP_LOGD("EEPROM", "restore_state: %i (sizeof: %i)", stateValueSW, sizeof(stateValueSW));
            EEPROM.end();
        }                

        void save_state_sw()
        {
            EEPROM.begin(32);            
            ESP_LOGD("EEPROM", "save_state: %i (sizeof: %i)", stateValueSW, sizeof(stateValueSW));
            EEPROM.put(swEepromAdress, stateValueSW);
            EEPROM.commit();
            EEPROM.end();
        }        

        int getSwStatus()
        {
            return stateValueSW;
        }

        /// @brief Setzt den Status aus der Abfrag des Kommandos SW
        /// @param read 
        bool setSWStatus(const char *read)
        {
            try
            {
                int value = WR3223Helper::to_int(read, true);
                ESP_LOGD("VALUEHOLDER", "setSWStaus: %i", value);
                stateValueSW = value & WR3223EnumStatusSW::STATUS_MASK_SW;
                ESP_LOGD("VALUEHOLDER", "setSWStaus (masked): %i", stateValueSW);
                return true;
            }
            catch(const std::exception& e)
            {
                ESP_LOGE("STATUS HOLDER", "Exception in setSWStaus: %s (SetValue: %s)",e.what(), read);
                return false;
            }
        }

        /// @brief Setzt den Status aus der Abfrag des Kommandos Ta
        /// @param read 
        bool setTaStatus(const char *read) 
        {
            
            try
            {    
                int value = WR3223Helper::to_int(read, true);
                ESP_LOGD("VALUEHOLDER", "setTaStatus: %i", value);
                stateValueTa = value & WR3223EnumStatusTa::STATUS_MASK_TA;
                ESP_LOGD("VALUEHOLDER", "setTaStatus (masked): %i", stateValueTa);
                return true;
            }
            catch(const std::exception& e)
            {
                ESP_LOGE("STATUS HOLDER", "Exception in setTaStatus: %s (SetValue: %s)",e.what(), read);
                return false;
            }
        }
    };
}