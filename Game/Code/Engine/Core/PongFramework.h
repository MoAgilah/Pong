#pragma once

#include <Core/SFFramework.h>

class PongFramework : public SFFramework
{
public:
	PongFramework();

    void Initialise() override;
    void Shutdown() override;
};