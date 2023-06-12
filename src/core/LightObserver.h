#include "Observer.h"
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_light_sensor.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/control_interface/ci_controller.h>

#include <argos3/demiurge/epuck-dao/EpuckDAO.h>
#include <argos3/demiurge/epuck-dao/ReferenceModel1Dot2.h>
#include <chrono>
#include <thread>
namespace argos {
class LightObserver: public CCI_Controller {
public:
    LightObserver(Observer &m_observer);
    void StartObserving();
    // void StopObserving();

private:
     Observer& m_observer;
     CCI_EPuckLightSensor* m_lightSensor;
};
}