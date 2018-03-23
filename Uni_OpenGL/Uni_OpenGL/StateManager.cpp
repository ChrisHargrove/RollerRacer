////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "StateManager.h"

////////////////////////////////////////////////////////////
// Static Variables
////////////////////////////////////////////////////////////
StateManager StateManager::_Instance;

////////////////////////////////////////////////////////////
StateManager::StateManager(){}

////////////////////////////////////////////////////////////
StateManager::~StateManager(){}

////////////////////////////////////////////////////////////
void StateManager::Input()
{
    if (_ActiveStack.empty()) {
        LogManager::Instance()->LogError("No Active State on Input! StateManager.cpp - Line 21");
        return;
    }
    if (!_ActiveStack.top()->IsPaused()) {
        _ActiveStack.top()->Input();
    }
}

void StateManager::Update(float delta)
{
    if (_ActiveStack.empty()) {
        LogManager::Instance()->LogError("No Active State on Update! StateManager.cpp - Line 32");
        return;
    }
    if (!_ActiveStack.top()->IsPaused()) {
        _ActiveStack.top()->Update(delta);
    }
}

void StateManager::Render()
{
    if (_ActiveStack.empty()) {
        LogManager::Instance()->LogError("No Active State on Render! StateManager.cpp - Line 43");
        return;
    }
    if (!_ActiveStack.top()->IsPaused()) {
        _ActiveStack.top()->Render();
    }
}

bool StateManager::Shutdown()
{
    if(_ActiveStack.empty()) {
        LogManager::Instance()->LogError("No Active States on Shutdown! StateManager.cpp - Line 54");
        return false;
    }
    while(!_ActiveStack.empty()) {
        if (!_ActiveStack.top()->Shutdown()) {
            LogManager::Instance()->LogError("Active Stack Top failed Shutdown! StateManager.cpp - Line 59");
            return false;
        }
        _ActiveStack.pop();
    }
    for (auto s : _StateList) {
        delete s.second;
    }
    _StateList.clear();

    LogManager::Instance()->LogDebug("State Machine Shutdown Successful!");
    return true;
}

bool StateManager::ChangeState(const std::string & StateName)
{
    auto search = _StateList.find(StateName);
    if (search == _StateList.end()) {
        LogManager::Instance()->LogError(StateName + " Does not exist! StateManager.cpp - Line 77");
        return false;
    }
    if (_ActiveStack.empty()) {
        if (!PushState(StateName)) {
            LogManager::Instance()->LogError(StateName + " Push Failed! StateManager.cpp - Line 82");
            return false;
        }
    }
    else {
        while (!_ActiveStack.empty()) {
            if (!_ActiveStack.top()->Shutdown()) {
                LogManager::Instance()->LogError("Couldn't Shutdown top State on State Change! StateManager.cpp - Line 86");
                return false;
            }
            _ActiveStack.pop();
        }
        if (!PushState(StateName)) {
            LogManager::Instance()->LogError(StateName + " Push Failed! StateManager.cpp - Line 95");
            return false;
        }
    }
    LogManager::Instance()->LogDebug(StateName + " Change Successfull!");
    return true;
}

bool StateManager::PushState(const std::string & StateName)
{
    auto search = _StateList.find(StateName);
    if (search == _StateList.end()) {
        LogManager::Instance()->LogError(StateName + " Does not exist! StateManager.cpp - Line 107");
        return false;
    }
    if (!_ActiveStack.empty()) {
        if (!_ActiveStack.top()->IsPaused()) {
            _ActiveStack.top()->Pause();
        }
    }
    _ActiveStack.push(_StateList.at(StateName));
    if (_ActiveStack.top() != _StateList.at(StateName)) {
        LogManager::Instance()->LogError(StateName + "Push Failed! StateManager.cpp - Line 117");
        return false;
    }
    if (!_ActiveStack.top()->Initialize()) {
        LogManager::Instance()->LogError(StateName + "Initialize Failed! StateManager.cpp - Line 121");
        return false;
    }
    LogManager::Instance()->LogDebug(StateName + " Push Successfull!");
    return true;
}

bool StateManager::PopState()
{
    if (_ActiveStack.empty()) {
        LogManager::Instance()->LogError("Active Stack Empty On Pop! StateManager.cpp - Line 131");
        return false;
    }
    if (!_ActiveStack.top()->Shutdown()) {
        LogManager::Instance()->LogError("Shutdown during Pop Failed! StateManager.cpp - Line 135");
        return false;
    }
    if (!_ActiveStack.empty()) {
        if (!_ActiveStack.top()->IsPaused()) {
            LogManager::Instance()->LogWarning("Previous State Wasn't Paused! StateManager.cpp - Line 140");
        }
        else {
            _ActiveStack.top()->Resume();
        }
    }
    return true;
}

bool StateManager::AddState(std::string StateName, State * NewState)
{
    auto search = _StateList.find(StateName);
    if (search != _StateList.end()) {
        LogManager::Instance()->LogError(StateName + " Already Exists! StateManager.cpp - Line 153");
        return false;
    }
    _StateList.emplace(std::pair<std::string, State*>(StateName, NewState));
    return true;
}
