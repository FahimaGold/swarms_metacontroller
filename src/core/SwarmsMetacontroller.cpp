#include "SwarmsMetacontroller.h"
namespace argos {
 SwarmsMetacontroller:: SwarmsMetacontroller()
 {
   
   	m_pcRobotState = new ReferenceModel1Dot2();
		m_unTimeStep = 0;
		m_strFsmConfiguration = "";
		m_bMaintainHistory = false;
		m_bPrintReadableFsm = false;
		m_strHistoryFolder = "./";
		m_bFiniteStateMachineGiven = false;
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
	CVector2 sLightVector(0,CRadians::ZERO);
	Real lightIntensity = 5.0;
	// Set the tolerance range for comparison
    const double tolerance = 0.1;

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
			const CCI_EPuckLightSensor::TReadings& readings = m_pcLightSensor->GetReadings();
			m_pcRobotState->SetLightInput(readings);
			/*
			Monitor the light sensor, if the light changes, switch the PFSM controller
			*/
			// Iterate over the sensor readings
         /*for (const auto& reading : readings) {
         // Check if the measured light intensity is within the tolerance range of 5.0
         if (std::abs(reading.Value - 5.0) <= tolerance) {
            // The measured light intensity is approximately 5.0
            SwarmsMetacontroller:: m_strFsmConfiguration = "--nstates 1 --s0 3";
			m_pcFsmBuilder = new AutoMoDeFsmBuilder();
		
			SetFiniteStateMachine(m_pcFsmBuilder->BuildFiniteStateMachine(m_strFsmConfiguration));
         }

		 if (std::abs(reading.Value - 1.0) <= tolerance) {
            // The measured light intensity is approximately 1.0
            SwarmsMetacontroller:: m_strFsmConfiguration= "--nstates 3 --s0 2 --n0 2 --n0x0 1 --c0x0 1 --p0x0 0.93 --n0x1 1 --c0x1 0 --p0x1 0.86 --s1 4 --att1 3.69 --n1 1 --n1x0 1 --c1x0 3 --p1x0 4 --w1x0 18.49 --s2 1 --n2 3 --n2x0 0 --c2x0 2 --p2x0 0.33 --n2x1 0 --c2x1 3 --p2x1 3 --w2x1 4.69 --n2x2 1 --c2x2 0 --p2x2 0.89";
			m_pcFsmBuilder = new AutoMoDeFsmBuilder();
		
			SetFiniteStateMachine(m_pcFsmBuilder->BuildFiniteStateMachine(m_strFsmConfiguration));
         }
		 	}*/

		 /*
		 Since we cannot update light intensity during runtime in Argos3,
		 We will use this condition to change light intensity after some time
		 */ 
		 
		 if (SwarmsMetacontroller::m_unTimeStep == 100){
			lightIntensity = 1.0;
			  SwarmsMetacontroller:: m_strFsmConfiguration= "--nstates 3 --s0 2 --n0 2 --n0x0 1 --c0x0 1 --p0x0 0.93 --n0x1 1 --c0x1 0 --p0x1 0.86 --s1 4 --att1 3.69 --n1 1 --n1x0 1 --c1x0 3 --p1x0 4 --w1x0 18.49 --s2 1 --n2 3 --n2x0 0 --c2x0 2 --p2x0 0.33 --n2x1 0 --c2x1 3 --p2x1 3 --w2x1 4.69 --n2x2 1 --c2x2 0 --p2x2 0.89";
			m_pcFsmBuilder = new AutoMoDeFsmBuilder();
		
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
		m_unTimeStep++;


}
 void SwarmsMetacontroller::Destroy(){
   // m_AutoMoDeController->Destroy();
 }

 void SwarmsMetacontroller::Reset(){
		//m_AutoMoDeController->Reset();
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