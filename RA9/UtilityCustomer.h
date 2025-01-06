#include <iostream>
#include <ostream>
#pragma once

class UtilityCustomer{
    private:
    int accNum;
    public:
    UtilityCustomer(){
        accNum = -1;
    }
    UtilityCustomer(int val){
        accNum=val;
    }
    virtual double calculateBill() const = 0;
    void setAccount(int val){
        accNum = val;
    }
    int getAccount() const{
        return accNum;
    }

    virtual void Display() const{
        std::cout<< "Utility Customer Account Number: " << accNum << std::endl;
    
    }
    bool operator==(UtilityCustomer * UtilityObj) const{
        return getAccount() == UtilityObj->getAccount();
    }
};



