#include "CTaskManager.h"
#include "CGame.h"

extern CGame GAP;

void CTaskManager::AddTask(task_shared_ptr task){
    taskCount++;
    task->SetId(taskCount);
    Tasks.push_back(task);
    return;
}

void CTaskManager::DestroyTask(int id){
    std::vector<task_shared_ptr>::iterator iter = Tasks.begin();
    while (iter != Tasks.end()){
        if((*iter)->GetId() == id){
            Tasks.erase(iter);
            return;
        }
        iter++;
    }
}

void CTaskManager::Update(){
    std::vector<task_shared_ptr>::iterator iter = Tasks.begin();
//    int counter = 0;
    while (iter != Tasks.end()){
//        task_shared_ptr task = task_shared_ptr(*iter);
//        unit_weak_ptr porter = task->GetPorter();
//        if(auto ps = porter.lock()){
//            std::string str = *(ps->GetName());
//            int res = ps->GetResource();
//            counter++;
//        }
//        build_weak_ptr pickup = task->GetPickUp();
//        if(auto ps = pickup.lock()){
//            std::string str = *(ps->GetName());
//            int res = ps->GetResource();
//            counter++;
//        }
//        build_weak_ptr dropoff = task->GetDropOff();
//        if(auto ps = dropoff.lock()){
//            std::string str = *(ps->GetName());
//            int res = ps->GetResource();
//            counter++;
//        }
        if((*iter)->GetCompleted()){
            if(auto s = (*iter)->GetPickUp().lock()){
                s->RemoveOutgoing((*iter)->GetId());
            }
            if(auto s = (*iter)->GetDropOff().lock()){
                s->RemoveOutgoing((*iter)->GetId());
            }
            Tasks.erase(iter);
        }else{
            iter++;
        }
    }
}
