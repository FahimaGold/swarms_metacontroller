#ifndef MY_HEADER_OBSERVER_H
#define MY_HEADER_OBSERVER_H

 /*
     For a more general purpose to monitor a certain condition.
     Depending on the control software to be used. For AutoMoDe, this Observer is not needed since the observaiton can be handled in the MetaController ControlStep() Method.
     This Class will be implemented by the Metacontroller class 
     */ 
namespace argos {
class Observer{
public:

     virtual void Notify(double monitoredProperty) = 0;

};
}
#endif 