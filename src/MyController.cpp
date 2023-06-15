#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/configuration/argos_configuration.h>
using namespace argos;

class MyController : public CCI_Controller {
public:
   void Init(TConfigurationNode& t_node) override {
      
   }

   void ControlStep() override {
     
   }

   void Reset() override {
      
   }
};

REGISTER_CONTROLLER(MyController, "my_controller");

extern "C" {
   int Init(argos::TConfigurationNode& t_tree) {
     
      return 0;
   }

   void Destroy() {
    
   }
}
