#include "epuck_obstacleavoidance.h"
#include <argos3/core/utility/configuration/argos_configuration.h>

CEPuckObstacleAvoidance::CEPuckObstacleAvoidance() :
   m_pcWheels(NULL),
   m_pcProximity(NULL),
   m_fWheelVelocity(2.5f) {}

void CEPuckObstacleAvoidance::Init(TConfigurationNode& t_node) {
   m_pcWheels = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
   m_pcProximity = GetSensor<CCI_ProximitySensor>("proximity");

   if (m_pcWheels == NULL || m_pcProximity == NULL) {
      THROW_ARGOSEXCEPTION("Required sensors/actuators not found!");
   }

   GetNodeAttributeOrDefault(t_node, "velocity", m_fWheelVelocity, m_fWheelVelocity);
}

void CEPuckObstacleAvoidance::ControlStep() {
   const std::vector<Real>& proximityReadings = m_pcProximity->GetReadings();

   Real fMaxReadVal = proximityReadings[0];
   UInt32 unMaxReadIdx = 0;

   for (UInt32 i = 1; i < proximityReadings.size(); ++i) {
      if (proximityReadings[i] > fMaxReadVal) {
         fMaxReadVal = proximityReadings[i];
         unMaxReadIdx = i;
      }
   }

   if (fMaxReadVal > 0.0f) {
      if (unMaxReadIdx == 0 || unMaxReadIdx == 1) {
         m_pcWheels->SetLinearVelocity(m_fWheelVelocity, 0.0f);
      } else {
         m_pcWheels->SetLinearVelocity(0.0f, m_fWheelVelocity);
      }
   } else {
      m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
   }
}
REGISTER_CONTROLLER(CEPuckObstacleAvoidance, "epuck_obstacleavoidance_controller")
