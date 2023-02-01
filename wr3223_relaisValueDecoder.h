
namespace WR3223 
{
    enum ERelais : int
    {
        None = 0,
        Kompressor = 1,
        Zusatzheizung = 2,
        Erdwaermetauscher = 4,
        Bypass = 8,
        Vorheizregister = 16,
        NetzrelaisBypass = 32,
        BedienteilAktiv = 64,
        BedienungViaRSSchnittstelle = 128,
        LuftstufeVorhanden = 256,
        WW_Nachheizregister = 512,
        Magnetventil = 2048,
        VorheizenAktiv = 4096
    };

    class WR3223RelaisValueDecoder
    {
        public:

        /// @brief Veröffentlicht den Status der Relais        
        bool publish_relais(const char *data)
        {
            try
            {      
                int flags = std::atoi(data);

                RelaySensors::bedienteilAktivSensor->publish_state(flags & ERelais::BedienteilAktiv);
                WR3223::bedienteilAktiv = flags & ERelais::BedienteilAktiv;
                RelaySensors::bedienungViaRSSchnittstelleSensor->publish_state(flags & ERelais::BedienungViaRSSchnittstelle);
                RelaySensors::bypassSensor->publish_state(flags & ERelais::Bypass);
                RelaySensors::erdwaermetauscherSensor->publish_state(flags & ERelais::Erdwaermetauscher);
                RelaySensors::kompressorSensor->publish_state(flags & ERelais::Kompressor);
                RelaySensors::luftstufeVorhandenSensor->publish_state(flags & ERelais::LuftstufeVorhanden);
                RelaySensors::magnetventilSensor->publish_state(flags & ERelais::Magnetventil);
                RelaySensors::netzrelaisBypassSensor->publish_state(flags & ERelais::NetzrelaisBypass);
                RelaySensors::vorheizenAktivSensor->publish_state(flags & ERelais::VorheizenAktiv);
                RelaySensors::vorheizregisterSensor->publish_state(flags & ERelais::Vorheizregister);
                RelaySensors::wW_NachheizregisterSensor->publish_state(flags & ERelais::WW_Nachheizregister);
                RelaySensors::zusatzheizungSensor->publish_state(flags & ERelais::Zusatzheizung);
                return true;
            }
            catch(const std::exception& e)
            {
                ESP_LOGE("RELAIS DECODER", "Exception in publish_relais: %s (SetValue: %s)",e.what(), read);
                return false;
            }
        }
    };
}
