#include "../AutoMoDe/src/core/AutoMoDeController.h"
#include "../AutoMoDe/src/core/AutoMoDeFiniteStateMachine.h"
#include "../AutoMoDe/src/core/AutoMoDeFsmBuilder.h"
#include "Observer.h"
namespace argos {
class SwarmsMetacontroller:  public CCI_Controller, public Observer{
    public:
   
	SwarmsMetacontroller();
    virtual ~SwarmsMetacontroller();
    virtual void Init(TConfigurationNode& t_node);
    virtual void ControlStep();
	virtual void Reset();
	virtual void Destroy();
    void SetFiniteStateMachine(AutoMoDeFiniteStateMachine* pc_fine_state_machine);
    void Notify(double lightIntensity) override;

    // Getters
    AutoMoDeController* GetAutoMoDeController();

    // Setters
    void SetAutoMoDeController(AutoMoDeController* m_AutoMoDeController);


    private:
    AutoMoDeFiniteStateMachine* m_pcFiniteStateMachine;
    AutoMoDeController* m_AutoMoDeController;
    AutoMoDeFsmBuilder* m_pcFsmBuilder;
    std::string m_strFsmConfiguration;
    bool m_lightIntensityChanged;
    double m_lightIntensity;
};
}