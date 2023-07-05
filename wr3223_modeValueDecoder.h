namespace WR3223 
{
    enum WR3223EnumModus : char
    {
        AUS =                       0b00000000, 
        SOMMER_BETRIEB =            0b00000001,
        SOMMER_ABLUFT =             0b00000010,
        WINTER_BETRIEB =            0b00000011,
        HANBETRIEB_WINTERBETRIEB =  0b00000100,

        ABSENKUNG_ON =              0b01000000,
        NACHHEIZREGISTER_ON =       0b10000000
    };

    class WR3223ModusValueDecoder 
    {        
        static constexpr int STATUS_MASK = 0b11000111; // 1 + 2 + 4 + 64 + 128;
        
        // für die Auswahl des Modus interessieren uns nur die erste 3 bit
        static constexpr int MODE_MASK = 0b00000111;

        public:        
        // Der aktuelle gesamtwert, welcher zuletzt gelesen wurde
        int activeMode;        

        bool get_absenkung_on()
        {
            return static_cast<bool>(activeMode & WR3223EnumModus::ABSENKUNG_ON);
        }        

        bool get_nachheizregister_on()
        {
            return static_cast<bool>(activeMode & WR3223EnumModus::NACHHEIZREGISTER_ON);
        }            


        /// @brief Setzt den Status aus der Abfrag des Kommandos MD
        /// @param read 
        bool publishValue(const char *read) 
        {
            try
            {
                activeMode = WR3223Helper::to_int(read, false);
                activeMode &= STATUS_MASK;
                ModusSensor::absenkungSensor->publish_state(get_absenkung_on());
                ModusSensor::nachheizregisterSensor->publish_state(get_nachheizregister_on());
                return true;
            }
            catch(const std::exception& e)
            {
                ESP_LOGE("MODE DECODER", "Exception in publishValue: %s (SetValue: %s)",e.what(), read);
                return false;
            }                        
        }           

        /// @brief Liefert nur die relevanten Bits für den Modus (also ohne nachheizregister und absenkung)
        /// @return 
        int get_active_mode()
        {            
            return activeMode & MODE_MASK;
        }  
        
        std::string getModeDisplayValue(int pMode)
        {
            if(static_cast<bool>(pMode & WR3223EnumModus::WINTER_BETRIEB))
                return "Winterbetrieb";
            else if(static_cast<bool>(pMode & WR3223EnumModus::HANBETRIEB_WINTERBETRIEB))
                return "Handbetrieb";
            else if(static_cast<bool>(pMode & WR3223EnumModus::SOMMER_ABLUFT))
                return "Sommer-Abluftbetrieb";
            else if(static_cast<bool>(pMode & WR3223EnumModus::SOMMER_BETRIEB))
                return "Sommerbetrieb";
            else
                return "AUS";
        }  
    };
}