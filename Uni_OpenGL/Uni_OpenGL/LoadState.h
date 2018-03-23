#pragma once

#include "State.h"

class LoadState : public State
{
public:
    LoadState();
    virtual ~LoadState();

    // Inherited via State
    virtual bool Initialize() override;
    virtual void Input() override;
    virtual void Update(float delta) override;
    virtual void Render() override;
    virtual bool Shutdown() override;
    virtual void Pause() override;
    virtual void Resume() override;
    virtual bool IsPaused() override;

private:
    bool _HasFinishedLoading;
};

