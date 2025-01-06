#include "UtilityCustomer.h"
class GasCustomer:
    public UtilityCustomer{
        private:
        double Usage;
        const double priceOfGas = 4.25;
        public:
        double getUsage(){
            return Usage;
        }
        void setUsage(double val){
            Usage = val;
        }
        GasCustomer(){
            Usage = 0;
            setAccount(-1);
        }
        GasCustomer(double use, int acc){
            Usage = use;
            setAccount(acc);
        }
        virtual void Display() const {
            std::cout<<"Gas Utility Customer" << std::endl;
            
            UtilityCustomer::Display();
            std::cout<<"Gas Used (in cubic meters): " << Usage << std::endl;
            std::cout<<"total charge: " << calculateBill() << std::endl;
        }
        double calculateBill() const override {
            return Usage * priceOfGas;
        }

        
    };