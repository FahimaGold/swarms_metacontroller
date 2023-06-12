#include "LightObserver.h"

namespace argos {
   LightObserver:: LightObserver(Observer &Observer)
   : m_observer(Observer){
     // Get a reference to the light sensor
    m_lightSensor = GetSensor<CCI_EPuckLightSensor>("epuck_light");
   }
    void LightObserver::StartObserving(){
         // Start observing the light
    // Example code for monitoring light intensity
    double previousIntensity = 0.0;
    while (true) {
        // Get the current light intensity
        const CCI_EPuckLightSensor::TReadings& readings = m_lightSensor->GetReadings();
        double currentIntensity = readings[0].Value; // Assuming a single light sensor

        // Check if the intensity has changed
        if (currentIntensity != previousIntensity) {
            // Notify the observer (AutoMoDeController) about the change
            m_observer.Notify(currentIntensity);
            previousIntensity = currentIntensity;
        }

        // Sleep for 100 milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    }
    

}
