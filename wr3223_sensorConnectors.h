#include "esphome.h"

using namespace esphome::binary_sensor;
using namespace esphome::sensor;
using namespace esphome::text_sensor;

namespace RelaySensors
{
  
    BinarySensor *kompressorSensor = new BinarySensor();
    BinarySensor *zusatzheizungSensor = new BinarySensor();
    BinarySensor *erdwaermetauscherSensor = new BinarySensor();
    BinarySensor *bypassSensor = new BinarySensor();
    BinarySensor *vorheizregisterSensor = new BinarySensor();
    BinarySensor *netzrelaisBypassSensor = new BinarySensor();
    BinarySensor *bedienteilAktivSensor = new BinarySensor();
    BinarySensor *bedienungViaRSSchnittstelleSensor = new BinarySensor();
    BinarySensor *luftstufeVorhandenSensor = new BinarySensor();
    BinarySensor *wW_NachheizregisterSensor = new BinarySensor();
    BinarySensor *magnetventilSensor = new BinarySensor();
    BinarySensor *vorheizenAktivSensor = new BinarySensor();    
}

namespace TempSensors
{
  
    Sensor *verdampfertemperatur_sensor = new Sensor();
    Sensor *kondensatortemperatur_sensor = new Sensor();
    Sensor *aussentemperatur_sensor = new Sensor();    
    Sensor *fortlufttemperatur_sensor = new Sensor();
    Sensor *zulufttemperatur_sensor = new Sensor();
    Sensor *nachvorheizregistertemperatur_sensor = new Sensor();            
}

namespace ErrorSensors
{
    BinarySensor *errorSensor = new BinarySensor();
    TextSensor *errorText = new TextSensor();
}

namespace ModusSensor
{
    BinarySensor *nachheizregisterSensor = new BinarySensor();
    BinarySensor *absenkungSensor = new BinarySensor();
}

namespace LueftungSensor
{
    Sensor *drehzahl_abluft_sensor = new Sensor();
    Sensor *drehzahl_zuluft_sensor = new Sensor();
}

namespace WR3223 
{
    class WR3223TextSensorConnector : public Component 
    {
        public:
        WR3223TextSensorConnector()
        {
            errorTextSensor = ErrorSensors::errorText;
        }

        TextSensor *errorTextSensor;
    };

    class WR3223BinarySensorConnector : public Component 
    {
        public:
        WR3223BinarySensorConnector()
        {
            kompressorRelaySensor = RelaySensors::kompressorSensor;
            zusatzheizungRelaySensor = RelaySensors::zusatzheizungSensor;
            erdwaermetauscherRelaySensor = RelaySensors::erdwaermetauscherSensor;
            bypassRelaySensor = RelaySensors::bypassSensor;
            vorheizregisterRelaySensor = RelaySensors::vorheizregisterSensor;
            netzrelaisBypassRelaySensor = RelaySensors::netzrelaisBypassSensor;
            bedienteilAktivRelaySensor = RelaySensors::bedienteilAktivSensor;
            bedienungViaRSSchnittstelleRelaySensor = RelaySensors::bedienungViaRSSchnittstelleSensor;
            luftstufeVorhandenRelaySensor = RelaySensors::luftstufeVorhandenSensor;
            wW_NachheizregisterRelaySensor = RelaySensors::wW_NachheizregisterSensor;
            magnetventilRelaySensor = RelaySensors::magnetventilSensor;
            vorheizenAktivRelaySensor = RelaySensors::vorheizenAktivSensor;

            errorSensor = ErrorSensors::errorSensor;

            nachheizregisterSensor = ModusSensor::nachheizregisterSensor;
            absenkungSensor = ModusSensor::absenkungSensor;
        }

        BinarySensor *kompressorRelaySensor;
        BinarySensor *zusatzheizungRelaySensor;
        BinarySensor *erdwaermetauscherRelaySensor;
        BinarySensor *bypassRelaySensor;
        BinarySensor *vorheizregisterRelaySensor;
        BinarySensor *netzrelaisBypassRelaySensor;
        BinarySensor *bedienteilAktivRelaySensor;
        BinarySensor *bedienungViaRSSchnittstelleRelaySensor;
        BinarySensor *luftstufeVorhandenRelaySensor;
        BinarySensor *wW_NachheizregisterRelaySensor;
        BinarySensor *magnetventilRelaySensor;
        BinarySensor *vorheizenAktivRelaySensor;

        // Gibt an, ob irgendein fehler aufgetreten ist. Der Genaue Fehler seht im TextSensor.
        BinarySensor *errorSensor;
        
        // Bit 7 aus der Modusabfrage
        BinarySensor *nachheizregisterSensor;
        // Bit 8 aus der Modusabfrage
        BinarySensor *absenkungSensor;
    };

    class WR3223SensorConnector : public Component 
    {
        public:
        WR3223SensorConnector()
        {
            verdampfertemperatur_sensor = TempSensors::verdampfertemperatur_sensor;
            kondensatortemperatur_sensor = TempSensors::kondensatortemperatur_sensor;
            aussentemperatur_sensor = TempSensors::aussentemperatur_sensor;            
            fortlufttemperatur_sensor = TempSensors::fortlufttemperatur_sensor;
            zulufttemperatur_sensor = TempSensors::zulufttemperatur_sensor;
            nachvorheizregistertemperatur_sensor = TempSensors::nachvorheizregistertemperatur_sensor;
            drehzahl_abluft_sensor = LueftungSensor::drehzahl_abluft_sensor;
            drehzahl_zuluft_sensor = LueftungSensor::drehzahl_zuluft_sensor;
        }

        Sensor *verdampfertemperatur_sensor;
        Sensor *kondensatortemperatur_sensor;
        Sensor *aussentemperatur_sensor;        
        Sensor *fortlufttemperatur_sensor;
        Sensor *zulufttemperatur_sensor;
        Sensor *nachvorheizregistertemperatur_sensor;

        Sensor *drehzahl_abluft_sensor;
        Sensor *drehzahl_zuluft_sensor;
    };
}