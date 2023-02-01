
namespace WR3223 
{
    enum EError : int
    {
        /// @brief Temperatur Sensor Kurzschluss
        ERROR_TEMP_SENSOR_SHORT = 1,
        /// @brief Offset error
        ERROR_OFFSET = 2,
        /// @brief Temperatur Sensor unterbrochen
        ERROR_TEMP_SENSOR_INTERUPT = 3,
        /// @brief Hochdruckfehler
        ERROR_HIGH_PRESSURE = 4,
        /// @brief Error sys ram
        ERROR_SYS_RAM = 61,
        /// @brief Error sys rom
        ERROR_SYS_ROM = 62,
        /// @brief Error sys ee
        ERROR_SYS_EE = 65,
        /// @brief Error sys io
        ERROR_SYS_IO = 66,
        /// @brief Error sys 67 ad
        ERROR_SYS_67_AD = 67,
        /// @brief Zuluft fehlt
        ERROR_SUPPLY_AIR = 128,
        /// @brief Abluft fehlt
        ERROR_OUTGOING_AIR = 132,
        /// @brief Kondensationsfehler
        ERROR_CONDENSER = 130,
        /// @brief Vorheizfehler
        ERROR_PREHEATING = 133
    };

    class WR3223ErrorValueDecoder 
    {    
        std::string combineNewLine(std::string str, std::string toAppend)
        {
            if(str.length() > 0)
                str.append("\n");
            str.append(toAppend);
            return str;
        }

        public:   
        int lastReadValue = 0;        
        
        bool error_temp_sensor_short() { return (lastReadValue & ERROR_TEMP_SENSOR_SHORT) == ERROR_TEMP_SENSOR_SHORT;}
        bool error_offset() { return (lastReadValue & ERROR_OFFSET) == ERROR_OFFSET;}
        bool error_temp_sensor_interupt() { return (lastReadValue & ERROR_TEMP_SENSOR_INTERUPT) == ERROR_TEMP_SENSOR_INTERUPT;}
        bool error_high_pressure() { return (lastReadValue & ERROR_HIGH_PRESSURE) == ERROR_HIGH_PRESSURE;}
        bool error_sys_ram() { return (lastReadValue & ERROR_SYS_RAM) == ERROR_SYS_RAM;}
        bool error_sys_rom() { return (lastReadValue & ERROR_SYS_ROM) == ERROR_SYS_ROM;}
        bool error_sys_ee() { return (lastReadValue & ERROR_SYS_EE) == ERROR_SYS_EE;}
        bool error_sys_io() { return (lastReadValue & ERROR_SYS_IO) == ERROR_SYS_IO;}
        bool error_sys_67_ad() { return (lastReadValue & ERROR_SYS_67_AD) == ERROR_SYS_67_AD;}
        bool error_supply_air() { return (lastReadValue & ERROR_SUPPLY_AIR) == ERROR_SUPPLY_AIR;}
        bool error_outgoing_air() { return (lastReadValue & ERROR_OUTGOING_AIR) == ERROR_OUTGOING_AIR;}
        bool error_condenser() { return (lastReadValue & ERROR_CONDENSER) == ERROR_CONDENSER;}
        bool error_preheating() { return (lastReadValue & ERROR_PREHEATING) == ERROR_PREHEATING;}


        /// @brief Liefert ob ein Fehler Vorliegt
        /// @return True wenn ein Fehler vorliegt
        bool isError()
        {
            return error_temp_sensor_short() ||
                    error_offset() ||
                    error_temp_sensor_interupt() ||
                    error_high_pressure() ||
                    error_sys_ram() ||
                    error_sys_rom() ||
                    error_sys_ee() ||
                    error_sys_io() ||
                    error_sys_67_ad() ||
                    error_supply_air() ||
                    error_outgoing_air() ||
                    error_condenser() ||
                    error_preheating();
        }

        bool publishValue(const char *read) 
        {   
            try
            {
                lastReadValue = WR3223Helper::to_int(read);
                ErrorSensors::errorSensor->publish_state(isError());
                ErrorSensors::errorText->publish_state(getErrorText());
                return true;
            }
            catch(const std::exception& e)
            {
                ESP_LOGE("ERROR DECODER", "Exception in publishValue: %s (SetValue: %s)",e.what(), read);
                return false;
            }
        }

        std::string getErrorText()
        {
            if(isError() == false)
                return "kein Fehler";

            std::string result;

            if(error_temp_sensor_short())
                result.append("ERROR_TEMP_SENSOR_SHORT");
            if(error_offset())
                result = combineNewLine(result,"ERROR_OFFSET");
            if(error_temp_sensor_interupt())
                result = combineNewLine(result,"ERROR_TEMP_SENSOR_INTERUPT");
            if(error_high_pressure())
                result = combineNewLine(result,"ERROR_HIGH_PRESSURE");
            if(error_sys_ram())
                 result = combineNewLine(result,"ERROR_SYS_RAM");
            if(error_sys_rom())
                result = combineNewLine(result,"ERROR_SYS_ROM");
            if(error_sys_ee())
                result = combineNewLine(result,"ERROR_SYS_EE");
            if(error_sys_io())
                result = combineNewLine(result,"ERROR_SYS_IO");
            if(error_sys_67_ad())
                result = combineNewLine(result,"ERROR_SYS_67_AD");
            if(error_supply_air())
                result = combineNewLine(result,"ERROR_SUPPLY_AIR");
            if(error_outgoing_air())
                result = combineNewLine(result,"ERROR_OUTGOING_AIR");
            if(error_condenser())
                result = combineNewLine(result,"ERROR_CONDENSER");
            if(error_preheating())
                result = combineNewLine(result,"ERROR_PREHEATING");

            return result;
        }
    };
}