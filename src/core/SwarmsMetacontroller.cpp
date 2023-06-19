#include "SwarmsMetacontroller.h"
namespace argos {
 SwarmsMetacontroller:: SwarmsMetacontroller()
 {
   
   	m_pcRobotState = new ReferenceModel1Dot2();
		m_strFsmConfiguration = "";
		m_bMaintainHistory = false;
		m_bPrintReadableFsm = false;
		m_strHistoryFolder = "./";
		m_bFiniteStateMachineGiven = false;
		strPFSMConfigFile = "/Users/mac/automode-thesis/swarms_metacontroller/src/example/pfsm-config.txt";
 }
   
SwarmsMetacontroller:: ~SwarmsMetacontroller(){
   /* delete m_pcRobotState;
		if (m_strFsmConfiguration.compare("") != 0) {
			delete m_pcFsmBuilder;
		}*/
    
 }

 void SwarmsMetacontroller::Init(TConfigurationNode& t_node){
    /*m_AutoMoDeController->Init(t_node);
    std::cout<< t_node << std::flush;*/
	 try {
			GetNodeAttributeOrDefault(t_node, "fsm-config", m_strFsmConfiguration, m_strFsmConfiguration);
			GetNodeAttributeOrDefault(t_node, "history", m_bMaintainHistory, m_bMaintainHistory);
			GetNodeAttributeOrDefault(t_node, "hist-folder", m_strHistoryFolder, m_strHistoryFolder);
			GetNodeAttributeOrDefault(t_node, "readable", m_bPrintReadableFsm, m_bPrintReadableFsm);

		
		} catch (CARGoSException& ex) {
			THROW_ARGOSEXCEPTION_NESTED("Error parsing <params>", ex);
		}
       
		m_unRobotID = atoi(GetId().substr(5, 6).c_str());
	
		m_pcRobotState->SetRobotIdentifier(m_unRobotID);
       
		/*
		 * If a FSM configuration is given as parameter of the experiment file, create a FSM from it
		 */
		if (m_strFsmConfiguration.compare("") != 0 && !m_bFiniteStateMachineGiven) {
		
			m_pcFsmBuilder = new AutoMoDeFsmBuilder();
		
			SetFiniteStateMachine(m_pcFsmBuilder->BuildFiniteStateMachine(m_strFsmConfiguration));
			if (m_bMaintainHistory) {
				m_pcFiniteStateMachine->SetHistoryFolder(m_strHistoryFolder);
				m_pcFiniteStateMachine->MaintainHistory();
			}
			if (m_bPrintReadableFsm) {
				std::cout << "Finite State Machine description: " << std::endl;
				std::cout << m_pcFiniteStateMachine->GetReadableFormat() << std::endl;
			}
		} else {
			LOGERR << "Warning: No finite state machine configuration found in .argos" << std::endl;
		}



		/*
		 *  Initializing sensors and actuators
		 */
		try{
			m_pcProximitySensor = GetSensor<CCI_EPuckProximitySensor>("epuck_proximity");
			m_pcLightSensor = GetSensor<CCI_EPuckLightSensor>("epuck_light");
			m_pcGroundSensor = GetSensor<CCI_EPuckGroundSensor>("epuck_ground");
			 m_pcRabSensor = GetSensor<CCI_EPuckRangeAndBearingSensor>("epuck_range_and_bearing");
			 m_pcCameraSensor = GetSensor<CCI_EPuckOmnidirectionalCameraSensor>("epuck_omnidirectional_camera");

		} catch (CARGoSException ex) {
			LOGERR<<"Error while initializing a Sensor!\n";
		}

		try{
			m_pcWheelsActuator = GetActuator<CCI_EPuckWheelsActuator>("epuck_wheels");
			m_pcRabActuator = GetActuator<CCI_EPuckRangeAndBearingActuator>("epuck_range_and_bearing");
			m_pcLEDsActuator = GetActuator<CCI_EPuckRGBLEDsActuator>("epuck_rgb_leds");
		} catch (CARGoSException ex) {
			LOGERR<<"Error while initializing an Actuator!\n";
		}

		/*
		 * Starts actuation.
		 */
		 InitializeActuation();
   
}

void SwarmsMetacontroller::ControlStep(){
	
	double highLightIntensity = 5.0;
	
	// sensor range
	const double sensorRange = 1.0;  
    // If the sensorRange is 1.0, and the light intensity in the environment can be 5.0, we need to do a scaling
	double scale = sensorRange / highLightIntensity;

     /*
		 * 1. Update RobotDAO
		 */
		if(m_pcRabSensor != NULL){
			const CCI_EPuckRangeAndBearingSensor::TPackets& packets = m_pcRabSensor->GetPackets();
			//m_pcRobotState->SetNumberNeighbors(packets.size());
			m_pcRobotState->SetRangeAndBearingMessages(packets);
		}
		if (m_pcGroundSensor != NULL) {
			const CCI_EPuckGroundSensor::SReadings& readings = m_pcGroundSensor->GetReadings();
			m_pcRobotState->SetGroundInput(readings);
		}
		if (m_pcLightSensor != NULL) {
			// MAPE-K Monitor
			const CCI_EPuckLightSensor::TReadings& readings = m_pcLightSensor->GetReadings();
			m_pcRobotState->SetLightInput(readings);
			/*
			Monitor the light sensor, if the light changes, switch the PFSM controller
			*/
         // Handling light intensity
		 // MAPE-K Analyze
         if (m_pcRobotState->GetLightReading().Value >= scale) {
			// MAPE-K Plan
           	SwarmsMetacontroller::m_strFsmConfiguration = SwarmsMetacontroller::configFile->SelectConfiguration(SwarmsMetacontroller::strPFSMConfigFile, "light_intensity", 5.0);
            m_pcFsmBuilder = new AutoMoDeFsmBuilder();
		    //MAPE-K Execute
			SetFiniteStateMachine(m_pcFsmBuilder->BuildFiniteStateMachine(m_strFsmConfiguration));
		 }
         // MAPE-K Analyze
		 if (m_pcRobotState->GetLightReading().Value < scale) {
			// MAPE-K Plan
            SwarmsMetacontroller::m_strFsmConfiguration = SwarmsMetacontroller::configFile->SelectConfiguration(SwarmsMetacontroller::strPFSMConfigFile, "light_intensity", 1.0);
			m_pcFsmBuilder = new AutoMoDeFsmBuilder();
		    //MAPE-K Execute
			SetFiniteStateMachine(m_pcFsmBuilder->BuildFiniteStateMachine(m_strFsmConfiguration));
         }
		 	

		 
		
		
		}
		if (m_pcProximitySensor != NULL) {
			const CCI_EPuckProximitySensor::TReadings& readings = m_pcProximitySensor->GetReadings();
			m_pcRobotState->SetProximityInput(readings);
		}

		/*
		 * 2. Execute step of FSM
		 */
		m_pcFiniteStateMachine->ControlStep();

		/*
		 * 3. Update Actuators
		 */
		if (m_pcWheelsActuator != NULL) {
			m_pcWheelsActuator->SetLinearVelocity(m_pcRobotState->GetLeftWheelVelocity(),m_pcRobotState->GetRightWheelVelocity());
		}

		/*
		 * 4. Update variables and sensors
		 */
		if (m_pcRabSensor != NULL) {
			m_pcRabSensor->ClearPackets();
		}


}
 void SwarmsMetacontroller::Destroy(){
   // m_AutoMoDeController->Destroy();
 }

 void SwarmsMetacontroller::Reset(){
		
	}

void SwarmsMetacontroller::InitializeActuation() {
		/*
		 * Constantly send range-and-bearing messages containing the robot integer identifier.
		 */
		if (m_pcRabActuator != NULL) {
			UInt8 data[4];
			data[0] = m_unRobotID;
			data[1] = 0;
			data[2] = 0;
			data[3] = 0;
			m_pcRabActuator->SetData(data);
		}
	}

void SwarmsMetacontroller::SetFiniteStateMachine(AutoMoDeFiniteStateMachine* pc_fine_state_machine){
		m_pcFiniteStateMachine = pc_fine_state_machine;
		m_pcFiniteStateMachine->SetRobotDAO(m_pcRobotState);
		m_pcFiniteStateMachine->Init();
		m_bFiniteStateMachineGiven = true;
}

void SwarmsMetacontroller::SetHistoryFlag(bool b_history_flag) {
		if (b_history_flag) {
			m_pcFiniteStateMachine->MaintainHistory();
		}
	}



  
   REGISTER_CONTROLLER(SwarmsMetacontroller, "swarms_metacontroller");
}