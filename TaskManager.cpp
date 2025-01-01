#include <iostream>
#include "TaskManager.h"
using namespace mtm;

mtm::Person* mtm::TaskManager::isEmployeeExist(const string& personName) const{
    for (int i = 0; i < numOfEmployees; i++){
        if (employees[i]->getName() == personName){
            return employees[i];
        }
    }
    return nullptr;
}

void mtm::TaskManager::assignTask(const string &personName, const Task &task){
    Person* cuurentEmployee = isEmployeeExist(personName);
    if(cuurentEmployee==nullptr){
        if(numOfEmployees == MAX_PERSONS){
            throw std::runtime_error("TaskManager::assignTask: you have reached the maximum number of employees");
        }
        cuurentEmployee = new Person(personName);
        employees[numOfEmployees++] = cuurentEmployee;
    }
    cuurentEmployee->assignTask(task);
}

void mtm::TaskManager::completeTask(const string &personName){
    Person* cuurentEmployee = isEmployeeExist(personName);
    if(cuurentEmployee == nullptr){
       return;
    }
    try
    {
        cuurentEmployee->completeTask();
    }
    catch (const std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
    }
}