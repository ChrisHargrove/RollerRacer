#pragma once

#include <BULLET\btBulletCollisionCommon.h>
#include <BULLET\btBulletDynamicsCommon.h>

#include "Model.h"

btConvexHullShape* CreateShape(Model* model);