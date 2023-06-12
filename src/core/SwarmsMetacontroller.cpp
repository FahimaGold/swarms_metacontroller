#include "./SwarmsMetacontroller.h"

namespace argos {
 SwarmsMetacontroller:: SwarmsMetacontroller(){
    m_AutoMoDeController = new AutoMoDeController();
 }
   
SwarmsMetacontroller:: ~SwarmsMetacontroller(){
    
 }

 void SwarmsMetacontroller::Init(TConfigurationNode& t_node){
    m_AutoMoDeController->Init(t_node);
}

void SwarmsMetacontroller::Notify(double lightIntensity) {
    // Handle the notification from the observer
    // Use the light intensity information as needed
    // Light intenstiy was changed
    if (lightIntensity <=2){
        SwarmsMetacontroller:: m_strFsmConfiguration = "--nstates 3 --s0 2 --n0 2 --n0x0 1 --c0x0 1 --p0x0 0.93 --n0x1 1 --c0x1 0 --p0x1 0.86 --s1 4 --att1 3.69 --n1 1 --n1x0 1 --c1x0 3 --p1x0 4 --w1x0 18.49 --s2 1 --n2 3 --n2x0 0 --c2x0 2 --p2x0 0.33 --n2x1 0 --c2x1 3 --p2x1 3 --w2x1 4.69 --n2x2 1 --c2x2 0 --p2x2 0.89";
    }

    else if(lightIntensity >= 5){
        SwarmsMetacontroller:: m_strFsmConfiguration = "--nstates 1 --s0 3";
    }
    SwarmsMetacontroller::m_lightIntensityChanged = true;
    // Example: Update the behavior based on the light intensity
    SwarmsMetacontroller::m_lightIntensity = lightIntensity;
}
void SwarmsMetacontroller::ControlStep(){
    m_AutoMoDeController->ControlStep();
    // Call AutoMoDeController ControlStep();
    if(m_lightIntensityChanged){
        // handling the switch here
     
        m_pcFsmBuilder = new AutoMoDeFsmBuilder();
			m_AutoMoDeController->SetFiniteStateMachine(m_pcFsmBuilder->BuildFiniteStateMachine(m_strFsmConfiguration));
        m_lightIntensityChanged = false;
    }

}
 void SwarmsMetacontroller::Destroy() {
    m_AutoMoDeController->Destroy();
 }

 void SwarmsMetacontroller::Reset() {
		m_AutoMoDeController->Reset();
	}

    // Getters

    AutoMoDeController* SwarmsMetacontroller:: GetAutoMoDeController(){
        return SwarmsMetacontroller::m_AutoMoDeController;
    }
    
    // Setters
    void SwarmsMetacontroller::SetAutoMoDeController(AutoMoDeController *autoMoDeController){
        m_AutoMoDeController = autoMoDeController;
    }

}