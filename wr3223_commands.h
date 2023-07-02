#include "esphome.h"
#include <bitset>
#include <string>
#include <iomanip>
#include <sstream>


// allgm. startpunkt der eigentlichen daten
static const int data_start = 3;

namespace WR3223 {
  namespace WR3223Helper
  { 
    // const std::string WHITESPACE = " \n\r\t\f\v";
  
    // std::string ltrim(const std::string &s)
    // {
    //   size_t start = s.find_first_not_of(WHITESPACE);
    //   return (start == std::string::npos) ? "" : s.substr(start);
    // }

    // std::string rtrim(const std::string &s)
    // {
    //   size_t end = s.find_last_not_of(WHITESPACE);
    //   return (end == std::string::npos) ? "" : s.substr(0, end + 1);
    // }

    // std::string trim(const std::string &s) 
    // {
    //   return rtrim(ltrim(s));
    // }

    // int getNumericValue(std::string &s)
    // {
    //   s = trim(s);
    //   int decPoint = s.find(".");
    //   if (decPoint > 0) {
    //       s = s.substr(0, decPoint);
    //   }
    //   return std::stoi(trim(s));
    // }

    float to_float(const char *data)
    {      
      std::string fs(data);
      float f = std::stof(fs);
      return f;      
    }

    float to_float(std::string data)
    {      
      float f = std::stof(data);
      return f;      
    }

    /// @brief Liefert den Inhalt als Ganze Zahl. Schneidet Evtl. Nachkommastellen ab.
    /// @param data Die Daten, welche die Zahl darstellen
    /// @param positiv Gibt an, ob die zahl immer als positive Zahl geliefert werden soll
    /// @return Liefert eine Ganze Zahl
    int to_int(const char *data, bool positiv = false)
    {                  
      int result = (int)to_float(data);
      if(positiv && result < 0)
        result *= -1;
      return result;
    }

    /// @brief Liefert den Inhalt als Ganze Zahl. Schneidet Evtl. Nachkommastellen ab.
    /// @param data Die Daten, welche die Zahl darstellen
    /// @param positiv Gibt an, ob die zahl immer als positive Zahl geliefert werden soll
    /// @return Liefert eine Ganze Zahl
    int to_int(std::string data, bool positiv = false)
    {                  
      int result = (int)to_float(data);
      if(positiv && result < 0)
        result *= -1;
      return result;
    }


    /// @brief Liefert die Nummer mit der angegebene anzahl an Nachkommastellen
    /// @param number Die umzuwandelde Nummer
    /// @param precision Die Anzahl der Nachkommastellen
    /// @return Die Nummer als String
    std::string to_string(float number, uint precision = 1)
    {      
      std::stringstream stream;
      stream << std::fixed << std::setprecision(precision) << number;
      return stream.str();
    }

    std::string to_bitwise(int number)    
    {
      return std::bitset<8>(number).to_string();
    }

    std::string to_hex_string(uint8_t *data, int length)
    {
      std::stringstream ss;
      ss << std::hex << std::setfill('0');
      for (int i = 0; i < length; i++)            
          ss << ":" <<std::setw(2) << static_cast<unsigned>(data[i]);
      return ss.str();
    }

    std::string to_hex_string(char *data, int length)
    {
      std::stringstream ss;
      ss << std::hex << std::setfill('0');
      for (int i = 0; i < length; i++)            
          ss << ":" <<std::setw(2) << static_cast<unsigned>(data[i]);
      return ss.str();
    }
  } 

  /// @brief Die Art des Ergebnisses, welches beim lesen dieses Commandos zu erwarten ist
  enum ECommandResultType
  {
    Number,
    Text,
    Bool,
    Relais,
    Error,
    Status,
    Modus
  };

  class WR3223Commands {
    public:
    //Abtau ein, beginn Abtauung ab Verdampfertemperatur. Temperatur lesen/schreiben
    static constexpr const char *AE = "AE";

    // Abtau aus, ende Abtauung ab Verdampfertemperatur. Temperatur lesen/schreiben
    static constexpr const char *AA = "AA";

    // Abtau Zuluft lesen/schreiben 
    static constexpr const char *Az = "Az";

    // Abtauluft lesen/schreiben 
    static constexpr const char *Aa = "Aa";

    // Frost aus lesen/schreiben 
    static constexpr const char *AR = "AR";

    // Frost an Schnittstelle lesen/schreiben 
    static constexpr const char *AZ = "AZ";

    // Abtaupause lesen/schreiben 
    static constexpr const char *AP = "AP";

    // Abtaunachlauf lesen/schreiben 
    static constexpr const char *AN = "AN";

    // Frost Verzögerung lesen 
    static constexpr const char *AV = "AV";

    // Verdampfertemperatur istwert auslesen 
    static constexpr const char *T1 = "T1";

    // Kondensatortemperatur auslesen 
    static constexpr const char *T2 = "T2";

    // Aussentemperatur auslesen 
    static constexpr const char *T3 = "T3";

    // Ablufttemperatur (Raumtemperatur) 
    static constexpr const char *T4 = "T4";

    // Temperatur nach Wärmetauscher (Fortluft) lesen 
    static constexpr const char *T5 = "T5";

    // Zulufttemperatur auslesen 
    static constexpr const char *T6 = "T6";

    // Temperatur nach Solevorerwärmung lesen 
    static constexpr const char *T7 = "T7";

    // Temperatur nach Vorheizregister lesen 
    static constexpr const char *T8 = "T8";

    // aktuelle Luftstufe lesen/schreiben 
    static constexpr const char *LS = "LS";

    // Luftstufe 1 lesen/schreiben 
    static constexpr const char *L1 = "L1";

    // Luftstufe 2 lesen/schreiben 
    static constexpr const char *L2 = "L2";

    // Luftstufe 3 lesen/schreiben 
    static constexpr const char *L3 = "L3";

    // Zuluft +/- lesen/schreiben 
    static constexpr const char *LD = "LD";

    // Abluft +/- lesen/schreiben 
    static constexpr const char *Ld = "Ld";

    // Erdwärmetauscher vorhanden lesen/schreiben 
    static constexpr const char *EC = "EC";

    // Schaltpunkt Sommer stopp lesen/schreiben 
    static constexpr const char *Es = "Es";

    // Schaltpunkt Erdwärmetauscher Sommer lesen/schreiben 
    static constexpr const char *ES = "ES";

    // Schaltpunkt Erdwärmetauscher Winter lesen/schreiben 
    static constexpr const char *EW = "EW";

    // Schaltpunkt Solepumpe Ein lesen/schreiben 
    static constexpr const char *EE = "EE";

    // Schaltpunkt Solepumpe aus lesen/schreiben 
    static constexpr const char *EA = "EA";

    // Fehler lesen 
    static constexpr const char *ER = "ER";

    // Status auslesen 
    static constexpr const char *ST = "ST";

    // Status schreib byte auslesen/schreiben (nur bei PC Steuerung) 
    static constexpr const char *SW = "SW";

    // Zuluftsoll Temperatur auslesen/schreiben (nur bei PC Steuerung) 
    static constexpr const char *SP = "SP";

    // Zulufttemperatur Sollwert
    static constexpr const char *Re = "Re";

    // Raumsollwert
    static constexpr const char *Rd = "Rd";

    // Mode auslesen/schreiben (nur bei PC Steuerung) 
    static constexpr const char *MD = "MD";

    // Relais lesen 
    static constexpr const char *RL = "RL";

    // Steuerspannnung Zuluft lesen 
    static constexpr const char *UZ = "UZ";

    // Steuerspannnung Abluft lesen 
    static constexpr const char *UA = "UA";

    // Drehzahl Zuluftmotor lesen 
    static constexpr const char *NZ = "NZ";

    // Drehzahl Abluftmotor lesen 
    static constexpr const char *NA = "NA";

    // Grenzdrehzahl lesen/schreiben (*50) 
    static constexpr const char *NM = "NM";

    // Configuration auslesen 
    static constexpr const char *CN = "CN";

    // Maximale Kondensationstemperatur lesen/schreiben 
    static constexpr const char *KM = "KM";

    // Zusatzheizung frei (ausgeschaltet (0) oder freigegeben (1)) lesen/schreiben 
    static constexpr const char *ZH = "ZH";

    // Zusatzheizung Ein lesen/schreiben 
    static constexpr const char *ZE = "ZE";

    // Wärmepumpe frei (freigegeben (1) oder aus (0)) lesen/schreiben 
    static constexpr const char *WP = "WP";

    // Pausezeit für Druckabbau bei automatischer Umschaltung lesen/schreiben 
    static constexpr const char *PA = "PA";

    // Identifikation lesen 
    static constexpr const char *II = "II";

    // Messwert, RWZ aktl., Aktuelle Rückwärmzahl in % 
    static constexpr const char *RA = "RA";

    // Parameter, Delta n 1 max, Max. Drehzahlabweichung Zu-/Abluft in Stufe 1 
    static constexpr const char *D1 = "D1";

    // Parameter, Delta n 2 max, Max. Drehzahlabweichung Zu-/Abluft in Stufe 2 
    static constexpr const char *D2 = "D2";

    // Parameter, Delta n 3 max, Max. Drehzahlabweichung Zu-/Abluft in Stufe 3 
    static constexpr const char *D3 = "D3";

    // Parameter, ERDluft +S1, Drehzahlerhöhung Zuluftventilator Stufe 1, wenn Erdwärmetauscher ein (0% bis 40%) 
    static constexpr const char *E1 = "E1";

    // Parameter, ERDluft +S2, Drehzahlerhöhung Zuluftventilator Stufe 2, wenn Erdwärmetauscher ein (0% bis 40%) 
    static constexpr const char *E2 = "E2";

    // Parameter, ERDluft +S3, Drehzahlerhöhung Zuluftventilator Stufe 3, wenn Erdwärmetauscher ein (0% bis 40%) 
    static constexpr const char *E3 = "E3";

    // Parameter, LuflREDUK, Luftwechsel um 3% reduziert ab Außentemp. ...°C (-20°C bis +10°C) 
    static constexpr const char *LR = "LR";

    // Parameter Solar max 
    static constexpr const char *SM = "SM";

    // Messwert Solar Nutzen (Stunden) 
    static constexpr const char *SN = "SN";

    // Parameter Delta T Aus Temperaturdifferenz zwischen Speicher u. Kollektor 
    static constexpr const char *DA = "DA";

    // Parameter Delta T Ein Temperaturdifferenz zwischen Speicher u. Kollektor 
    static constexpr const char *DE = "DE";

    // Parameter Unterstuetzungsgeblaese bei Luftstufe 1 bei EWT 
    static constexpr const char *S1 = "S1";

    // Parameter Unterstuetzungsgeblaese bei Luftstufe 2 bei EWT 
    static constexpr const char *S2 = "S2";

    // Parameter Unterstuetzungsgeblaese bei Luftstufe 3 bei EWT 
    static constexpr const char *S3 = "S3";

    // Parameter Warmwasser Sollwert 
    static constexpr const char *WS = "WS";

    // Parameter EVU Sperre 
    static constexpr const char *Tf = "Tf";

    // Status auslesen 
    static constexpr const char *Ta = "Ta";
  };

  class CommandPair {
    Sensor *sensor = nullptr;
    BinarySensor *binarySensor = nullptr;
    TextSensor *textSensor = nullptr;    
    bool(*publisherFunc)(const char*) = nullptr;

    public:    
    CommandPair(const char *pCmd, Sensor *sensor)
    {
      this->sensor = sensor;
      cmd = pCmd;
      resultType = ECommandResultType::Number;

    }

    CommandPair(const char *pCmd, BinarySensor *binarySensor)
    {
      this->binarySensor = binarySensor;
      cmd = pCmd;
      resultType = ECommandResultType::Bool;
    }

    CommandPair(const char *pCmd, TextSensor *textSensor)
    {
      this->textSensor = textSensor;
      cmd = pCmd;
      resultType = ECommandResultType::Text;
    }

    CommandPair(const char *pCmd, ECommandResultType pResultType)
    {
      cmd = pCmd;
      resultType = pResultType;      
    }

    CommandPair(const char *pCmd, bool(*function)(const char*))
    {
      cmd = pCmd;
      publisherFunc = function;
    }

    const char* cmd;

    char GetCommandBit() {return cmd[0];}
    char GetCommand() {return cmd[1];}    

    // Der Typ des Kommandos
    ECommandResultType resultType;        
    
    /// @brief veröffentlicht die Daten, falls eine Sensorkomponente gesetzt
    /// @param data 
    /// @return Liefert ob die Daten veröffentlicht wurden
    bool publish_data(std::string data)
    {
      try
      {
        if(resultType == ECommandResultType::Number && sensor != nullptr)
        {
          sensor->publish_state(WR3223Helper::to_float(data));
          return true;
        }
        if(resultType == ECommandResultType::Bool && binarySensor != nullptr)
        {
          binarySensor->publish_state(data != "0");
           return true;
        }
        if(resultType == ECommandResultType::Text && binarySensor != nullptr)
        {
          textSensor->publish_state(data);
           return true;
        }
        if(publisherFunc != nullptr)
        {
          return publisherFunc(data.c_str());
        }
        return false;
      }
      catch(const std::exception& e)
      {        
        ESP_LOGD("COMMAND", "Cmd %s Tried to publish_data: %s", cmd, data.c_str());  
        ESP_LOGD("COMMAND", "But Exception occured: %s", e.what());  
        return false;
      }
    }
  };
}
