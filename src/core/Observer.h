#ifndef MY_HEADER_OBSERVER_H
#define MY_HEADER_OBSERVER_H
namespace argos {
class Observer{
public:

     virtual void Notify(double lightIntensity) = 0;

};
}
#endif 