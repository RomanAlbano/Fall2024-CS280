#include "UtilityCustomer.h"    
class ElectricityCustomer:
    public UtilityCustomer{
        private:
        double kWattHourUsed;
        const double priceOfElectricity  = 0.3;
        public:
        double getkWattHourUsed(){
            return kWattHourUsed;
        }
        void setkWattHourUsed(double val){
            kWattHourUsed = val;
        }
        ElectricityCustomer(){
            kWattHourUsed = 0;
            setAccount(-1);
        }
        ElectricityCustomer(double wattUsed, int acc){
            kWattHourUsed = wattUsed;
            setAccount(acc);
        }
        double calculateBill() const override {
            return kWattHourUsed * priceOfElectricity;
        }
        virtual void Display() const {
            std::cout<<"Electric Utility Customer" << std::endl;
            
            UtilityCustomer::Display();
            std::cout<<"Electricity Used (in kWattHour): " << kWattHourUsed << std::endl;
            std::cout<<"total charge: " << calculateBill() << std::endl;
        }
        
    };
