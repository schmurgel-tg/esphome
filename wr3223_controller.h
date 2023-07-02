
namespace WR3223
{
  WR3223Connector *connector;
  WR3223StatusValueHolder *statusHolder;
  WR3223ErrorValueDecoder *errorHolder;
  WR3223ModusValueDecoder *modeHolder;
  WR3223RelaisValueDecoder *relaisDecoder;

  bool publish_to_statusHolder_ta(const char* data)  {
    return statusHolder->setTaStatus(data);
  }

  bool publish_to_statusHolder_sw(const char* data)  {
    return statusHolder->setSWStatus(data);
  }

  bool publish_to_errorHolder(const char* data)  {
    return errorHolder->publishValue(data);
  }

  bool publish_to_modeHolder(const char* data)  {
    return modeHolder->publishValue(data);
  }

  bool publish_to_relaisDecoder(const char* data)  {    
    return relaisDecoder->publish_relais(data);
  }

  class WR3223Controller : public PollingComponent, public CustomAPIDevice {
  public:
    WR3223Controller(int update_intervall, UARTComponent *parent) : PollingComponent(update_intervall)
    { 
      connector = new WR3223Connector(parent);
      statusHolder = new WR3223StatusValueHolder();
      errorHolder = new WR3223ErrorValueDecoder();
      modeHolder = new WR3223ModusValueDecoder();
      relaisDecoder = new WR3223RelaisValueDecoder();
    }     

    // diese kommandos muss bei fehlendem Bedienteil mindesten alle 20 sekunden den Status schreiben
    const CommandPair StatusWriteCmd = CommandPair(WR3223Commands::SW, publish_to_statusHolder_sw);
    const CommandPair RelaisCmd =  CommandPair(WR3223Commands::RL, publish_to_relaisDecoder);
    const CommandPair StatusUpdateCmd = CommandPair(WR3223Commands::Ta, publish_to_statusHolder_ta);
    const CommandPair ModusCmd = CommandPair(WR3223Commands::MD, publish_to_modeHolder);
    const CommandPair ErrorCmd = CommandPair(WR3223Commands::ER, publish_to_errorHolder);

    const CommandPair LufstufeCmd = CommandPair(WR3223Commands::LS, ECommandResultType::Number);    
    const CommandPair ZusatzheizungFreiCmd = CommandPair(WR3223Commands::ZH, ECommandResultType::Bool);
    const CommandPair ZusatzheizungEinCmd = CommandPair(WR3223Commands::ZE, ECommandResultType::Bool);

    const CommandPair VentLv1Cmd = CommandPair(WR3223Commands::L1, ECommandResultType::Number);
    const CommandPair VentLv2Cmd = CommandPair(WR3223Commands::L2, ECommandResultType::Number);
    const CommandPair VentLv3Cmd = CommandPair(WR3223Commands::L3, ECommandResultType::Number);

    const CommandPair ZuluftSoll = CommandPair(WR3223Commands::SP, ECommandResultType::Number);
    const CommandPair ZuluftSollRe = CommandPair(WR3223Commands::Re, ECommandResultType::Number);
    const CommandPair RaumsollRd = CommandPair(WR3223Commands::Rd, ECommandResultType::Number);

    const CommandPair SommerstopEs = CommandPair(WR3223Commands::Es, ECommandResultType::Number);
    // const CommandPair AbtauStartCmd = CommandPair(WR3223Commands::AE, ECommandResultType::Number);
    // const CommandPair AbtauEndeCmd = CommandPair(WR3223Commands::AA, ECommandResultType::Number);

    // const CommandPair AbtauLuftstufeCmd = CommandPair(WR3223Commands::Az, ECommandResultType::Number);
    // const CommandPair AbtauPauseCmd = CommandPair(WR3223Commands::AP, ECommandResultType::Number);

    // const CommandPair AbtauNachlaufzeitCmd = CommandPair(WR3223Commands::AN, ECommandResultType::Number);
    // const CommandPair AbtauRueckwaermezahlCmd = CommandPair(WR3223Commands::AR, ECommandResultType::Number);

    // const CommandPair ZuluftCmd = CommandPair(WR3223Commands::LD, ECommandResultType::Number);

    const static int cmdPairCount = 16;
    const CommandPair Commands[cmdPairCount] = 
    {  

      ModusCmd,

      CommandPair(WR3223Commands::T1, TempSensors::verdampfertemperatur_sensor),
      CommandPair(WR3223Commands::T2, TempSensors::kondensatortemperatur_sensor),
      CommandPair(WR3223Commands::T3, TempSensors::aussentemperatur_sensor),      
      CommandPair(WR3223Commands::T5, TempSensors::fortlufttemperatur_sensor),
      CommandPair(WR3223Commands::T6, TempSensors::zulufttemperatur_sensor),      
      CommandPair(WR3223Commands::T8, TempSensors::nachvorheizregistertemperatur_sensor),
      
      StatusUpdateCmd,
      
      RelaisCmd,

      ErrorCmd,      

      CommandPair(WR3223Commands::NA, LueftungSensor::drehzahl_abluft_sensor),
      CommandPair(WR3223Commands::NZ, LueftungSensor::drehzahl_zuluft_sensor),
      
      ZuluftSoll,
      ZuluftSollRe,
      RaumsollRd,

      SommerstopEs
      //StatusWriteCmd,
      //LufstufeCmd 
    };

    /// @brief Liefert den Ventilatorstellwert der Übergebenen Luftstufe (40-100%)
    int get_vent_level_speed(int pLevel)
    {
      switch (pLevel)
      {
        case 1:
          return std::stoi(readLine(VentLv1Cmd));
        case 2:
          return std::stoi(readLine(VentLv2Cmd));
        case 3:
          return std::stoi(readLine(VentLv3Cmd));
        default:
          return 0;        
      }
    }

    /// @brief Setzt den Ventilatorstellwert der Übergebenen Luftstufe (40-100%)
    bool set_vent_level_speed(int pLevel, int pSpeed)
    {
      if(pSpeed < 40 || pSpeed > 100)
      {
        ESP_LOGE("SET VENT LEVEL", "Ventilatorstellwert für Luftstufe %i konnte nicht gesetzt werden.", pLevel);
        ESP_LOGE("SET VENT LEVEL", "Wert %i liegt außerhalb des zulässigen Bereichs: [40-100]", pSpeed);
        return false;
      }

      char const* data = std::to_string(pSpeed).c_str();
      bool result = false;
      switch (pLevel)
      {
        case 1:
          result = connector->write(VentLv1Cmd, data);          
          break;
        case 2:
          result = connector->write(VentLv2Cmd, data);          
          break;
        case 3:
          result = connector->write(VentLv3Cmd, data);          
          break;
        default:
          break;
      }
      vent_level_1_speed->update();
      vent_level_2_speed->update();
      vent_level_3_speed->update();
      return result;
    }

    void set_update_interval(uint32_t update_interval) override
    {
      if(update_interval > 20000)
        update_interval = 20000;
      else if (update_interval < 1000)
        update_interval = 1000;
      
      this->update_interval_ = update_interval;
    }

    void setup() override 
    { 
      // Declare a service "hello_world"
      //  - Service will be called "esphome.<NODE_NAME>_hello_world" in Home Assistant.
      //  - The service has no arguments
      //  - The function on_hello_world declared below will attached to the service.
      register_service(&WR3223Controller::Refresh_Relais, "Refresh_Relais");
      register_service(&WR3223Controller::Config_Restore, "Config_Restore");
      register_service(&WR3223Controller::Config_Save, "Config_Save");
    }

    void Refresh_Relais() 
    {
      relaisDecoder->publish_relais(readLine(RelaisCmd).c_str());
    }

    void Config_Restore()
    {

      ESP_LOGI("Test_Read", "Status SW bit content: %s", readLine(StatusWriteCmd).c_str());
      ESP_LOGI("Test_Read", "Status Ta bit content: %s", readLine(StatusUpdateCmd).c_str());
      statusHolder->restore_state_sw();
      //ESP_LOGD("MODUS", "L1: %s", readLine(TestCmd1));
      //ESP_LOGD("MODUS", "L2: %s", readLine(TestCmd2));
      //ESP_LOGD("MODUS", "L3: %s", readLine(TestCmd3));
    }

    void Config_Save()
    {
      statusHolder->save_state_sw();
      //char const* data = std::to_string(60).c_str();
      //connector->write(TestCmd1, data);
    }

    void Show(std::string pShow)
    {
      ESP_LOGD("MODUS", "SHOW: %s", pShow.c_str());
    }

    /// @brief Liefert den Betriebsmodus
    /// @return 
    int Get_Modus()
    {
      int result = modeHolder->get_active_mode();
      ESP_LOGD("MODUS", "Get_Modus: %i", result);
      return result;
    }

    /// @brief Setzt den Betriebsmodus
    /// @param pMode 
    bool Set_Modus(esphome::optional<unsigned int> pMode)
    { 
      if(pMode.has_value() == false)
      {
        ESP_LOGD("MODUS", "Modus_Set: NO VALUE");
        return false;
      }
      else
      {

        ESP_LOGD("MODUS", "Modus_Set: %i", (int)pMode.value());
        ESP_LOGD("MODUS", "Modus_Set: %s", modeHolder->getModeDisplayValue((int)pMode.value()));

        char const* data = std::to_string((int)pMode.value()).c_str();
        return connector->write(ModusCmd, data); // return write ACK ergebnis compare
      }
    }

    int Get_Luftstufe()
    {
      return std::stoi(readLine(LufstufeCmd));
    }    

#pragma region SW (StatusWrite Commands)

    bool Set_Luftstufe(esphome::optional<unsigned int> pLevel)
    {
      if(pLevel.has_value() == false)
      {
        ESP_LOGD("VENT_LEVEL", "Set_Luftstufe: NO VALUE");
        return false;
      }
      if (pLevel.value() < 0 || pLevel.value() > 3) 
      {
        ESP_LOGD("VENT_LEVEL", "Set_Luftstufe: Level %i außerhalb des gültigen Bereichs [0-3]", pLevel.value());
        return false;
      }      

      if((int)pLevel.value() == Get_Luftstufe())
      {
        ESP_LOGD("VENT_LEVEL", "Set_Luftstufe: Die Aktuelle Luftstufe ist bereits Stufe %i", pLevel.value());
        return true;
      }

      statusHolder->setVentilationLevel((int)pLevel.value());
      return write_current_status_sw();
    }

    bool Get_Zusatzheizung_On()
    {
      return statusHolder->getAdditionalHeatingOnStatus();
    }
    
    bool Set_Zusatzheizung_On(bool pOn)
    {
      // wenn der Status schon stimmt, dann brauchen wir nicht noch einmal zu setzen
      if(statusHolder->getAdditionalHeatingOnStatus() == pOn)
        return true;

      statusHolder->setAdditionalHeatingOn(pOn);
      char const* data = std::to_string((int)statusHolder->getSwStatus()).c_str();
      if(connector->write(StatusWriteCmd, data))
        return true;
      
      statusHolder->setAdditionalHeatingOn(!pOn);
      return write_current_status_sw();
    }

    bool Get_Waermepunpe_On()
    {
      return statusHolder->getHeatPumpOnStatus();
    }
    
    bool Set_Waermepunpe_On(bool pOn)
    {
      // wenn der Status schon stimmt, dann brauchen wir nicht noch einmal zu setzen
      if(statusHolder->getHeatPumpOnStatus() == pOn)
        return true;

      statusHolder->setHeatPumpOn(pOn);
      char const* data = std::to_string((int)statusHolder->getSwStatus()).c_str();
      if(connector->write(StatusWriteCmd, data))
        return true;
      
      statusHolder->setHeatPumpOn(!pOn);
      return write_current_status_sw();
    }

// wo wir den 'echten' status der Kühlung herbekommen ist noch unklar?!?! 
// evtl. aus der Wärmepumpe, wenn sommermodus aktiv ist?!
    bool Get_Kuehlung_On()
    {
      return statusHolder->getCoolingOnStatus();
    }
    
    bool Set_Kuehlung_On(bool pOn)
    {
      // den Status können wir derzeit nicht ermitteln
      // // wenn der Status schon stimmt, dann brauchen wir nicht noch einmal zu setzen
      // if(statusHolder->getCoolingOnStatus() == pOn)
      //   return true;


      statusHolder->setCoolingOn(pOn);
      char const* data = std::to_string((int)statusHolder->getSwStatus()).c_str();
      if(connector->write(StatusWriteCmd, data))
        return true;
      
      statusHolder->setCoolingOn(!pOn);
      return false;
    }
    
#pragma endregion
  
    bool write_current_status_sw()
    {
      // try 
      // {
      //   float test = 18;
      //   char const* sollTemp = std::to_string(test).substr(0,4).c_str();
      //   connector->write(ZuluftSoll, sollTemp);
      //   connector->write(ZuluftSollRe, sollTemp);
      //   connector->write(RaumsollRd, sollTemp);
      //   connector->write(SommerstopEs, sollTemp);
      // }
      // catch(const std::exception& e)
      // {
      //   ESP_LOGE("SOLLTEMP", "Fehler beim schreiben der Daten: %s", e.what()); 
      // }    
      
      char const* data = std::to_string((int)statusHolder->getSwStatus()).c_str();
      return connector->write(StatusWriteCmd, data);
    }

    void update() override 
    {

      if(freshStart == true)
      {
        // das machen wir nur einmal zu beginn
        freshStart = false;
        Refresh_Relais();
        statusHolder->restore_state_sw();
        write_current_status_sw();

        vent_level_1_speed->update();
        vent_level_2_speed->update();
        vent_level_3_speed->update();
      }
      else
        write_current_status_sw();      

      // ESP_LOGI("INFO", "AbtauStartCmd: %s", readLine(AbtauStartCmd).c_str());
      // ESP_LOGI("INFO", "AbtauEndeCmd: %s", readLine(AbtauEndeCmd).c_str());
      // ESP_LOGI("INFO", "AbtauLuftstufeCmd: %s", readLine(AbtauLuftstufeCmd).c_str());
      // ESP_LOGI("INFO", "AbtauPauseCmd: %s", readLine(AbtauPauseCmd).c_str());
      // ESP_LOGI("INFO", "AbtauNachlauzeitCmd: %s", readLine(AbtauNachlauzeitCmd).c_str());
      // ESP_LOGI("INFO", "AbtauRueckwaermezahlCmd: %s", readLine(AbtauRueckwaermezahlCmd).c_str());

      for (int i = 0; i < cmdPairCount; i++)
      {
        CommandPair cmd = Commands[i];
        std::string resdata = readLine(cmd);
        if(cmd.publish_data(resdata) == false)
          ESP_LOGE("FINAL", "FAILED Command: %s Data: %s", cmd.cmd, resdata.c_str());  
      }
      // currentCommand++;
    }

    std::string readLine(const CommandPair cmd)
    {
      const int max_line_length = 15;
      static char answer[max_line_length];
      
      ESP_LOGI("START", "Command: %s", cmd.cmd);

      int dataLength = connector->readLine(answer, max_line_length, cmd);

      if(dataLength < 1)
        return "";

      char contentData[dataLength];
      std::copy(answer + data_start, answer + data_start + dataLength, contentData);
      contentData[dataLength] = 0;

      return std::string(contentData);
    }
  };
}

