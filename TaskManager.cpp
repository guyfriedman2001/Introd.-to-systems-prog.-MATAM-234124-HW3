#include <iostream>
#include "TaskManager.h"
using namespace mtm;

int TaskManager::idCounter = 0;

Person* TaskManager::isEmployeeExist(const string& personName){
    Person* currentEmployee = nullptr;
    for (int i = 0; i < numOfEmployees; i++){
        if (employees[i]->getName() == personName){
            currentEmployee = employees[i];
        }
    }
    return currentEmployee;
}

void TaskManager::assignTask(const string &personName, const Task &task){
    Person* currentEmployee = isEmployeeExist(personName);
    if(currentEmployee == nullptr){
        if(numOfEmployees == MAX_PERSONS){
            throw std::runtime_error("TaskManager::assignTask: you have reached the maximum number of employees");
        }
        currentEmployee = new Person(personName);
        employees[numOfEmployees++] = currentEmployee;
    }
    Task newTask(task.getPriority(), task.getType(), task.getDescription());
    newTask.setId(++idCounter);
    currentEmployee->assignTask(newTask);
}

void TaskManager::completeTask(const string &personName){
    Person* currentEmployee = isEmployeeExist(personName);
    if(currentEmployee == nullptr){
       return;
    }
    try
    {
        currentEmployee->completeTask();
    }
    catch (const std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void TaskManager::bumpPriorityByType(TaskType type, int priority){
    if(priority <= 0){
        return;
    }
    for(int i = 0; i < numOfEmployees; i++){
        SortedList<Task> oldTasks = this->employees[i]->getTasks();
        SortedList<Task> newTasks = oldTasks.apply([this, type, priority](Task task) { return setPriority(task, type, priority); });
        this->employees[i]->setTasks(newTasks);
    }

}

Task TaskManager::setPriority(Task tasks, TaskType type, int priority){
    if(tasks.getType() == type){
        Task resultTasks(tasks.getPriority() + priority, tasks.getType(), tasks.getDescription());
        resultTasks.setId(tasks.getId());
        return resultTasks;
    }
    return tasks;
}

void TaskManager::printAllEmployees() const{
    for(int i = 0; i < numOfEmployees; i++){
        std::cout << *employees[i] << std::endl;
        std::cout << std::endl;
    }
}

void TaskManager::printAllTasks() const{
    SortedList<Task> allTasks = getAllEmployeesTasks();
    for(auto currentTask : allTasks){
        std::cout << currentTask << std::endl;
    }
    std::cout << std::endl;
}

void TaskManager::printTasksByType(TaskType type) const{
    SortedList<Task> allTasks = getAllEmployeesTasks();
    SortedList<Task> tasksByType = allTasks.filter([type]( Task task) { return (task.getType() == type);});
    for(const Task& currentTask : tasksByType){
        std::cout << currentTask << std::endl;
    }
    std::cout << std::endl;
   
}
SortedList<Task> TaskManager::getAllEmployeesTasks() const{
    SortedList<Task> allTasks;
    for(int i = 0; i < this->numOfEmployees; i++){
        SortedList<Task> tasks(this->employees[i]->getTasks());
        for(auto currentTask : tasks){
            allTasks.insert(currentTask);
        }
    }
    return allTasks;
}

