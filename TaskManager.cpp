#include <iostream>
#include "TaskManager.h"
using namespace mtm;

bool TaskManager::isEmployeeExist(const string& personName) const{
    for (int i = 0; i < numOfEmployees; i++){
        if (employees[i]->getName() == personName){
            return true;
        }
    }
    return false;
}
