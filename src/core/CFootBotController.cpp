#include "./CFootBotController.h"

CFootBotController::CFootBotController() :
 m_pcWheels(NULL),
   m_pcProximity(NULL),
   m_cAlpha(10.0f),
   m_fDelta(0.5f),
   m_fWheelVelocity(2.5f),
   m_cGoStraightAngleRange(-ToRadians(m_cAlpha),
                           ToRadians(m_cAlpha))  {}

void CFootBotController::Init(TConfigurationNode& t_node) {
   try {
         m_pcWheels    = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
   m_pcProximity = GetSensor  <CCI_FootBotProximitySensor      >("footbot_proximity"    );
   /*
    * Parse the configuration file
    *
    * The user defines this part. Here, the algorithm accepts three
    * parameters and it's nice to put them in the config file so we don't
    * have to recompile if we want to try other settings.
    */
   GetNodeAttributeOrDefault(t_node, "alpha", m_cAlpha, m_cAlpha);
   m_cGoStraightAngleRange.Set(-ToRadians(m_cAlpha), ToRadians(m_cAlpha));
   GetNodeAttributeOrDefault(t_node, "delta", m_fDelta, m_fDelta);
   GetNodeAttributeOrDefault(t_node, "velocity", m_fWheelVelocity, m_fWheelVelocity);
    } catch (CARGoSException& ex) {
        LOGERR << "Error initializing the controller: " << ex.what() << std::endl;
    }
}

void CFootBotController::ControlStep() {
    // Set the wheel velocity of the robot
    m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
}

void CFootBotController::Reset() {}

void CFootBotController::Destroy() {}

/****************************************/
/****************************************/

/* To allow ARGoS to create and manage the controller */
REGISTER_CONTROLLER(CFootBotController, "footbot_controller");
