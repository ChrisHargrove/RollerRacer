#include "RigidBodyFromOBJ.h"

btConvexHullShape* CreateShape(Model* model) {
	//get all model vertices.
    std::vector<glm::vec3> temp = model->GetVertices();
    std::vector<btVector3> bulletVertices;
    for (auto v : temp) {
		//convert all glm::vec3 to btVector3
        bulletVertices.push_back(btVector3(v.x, v.y, v.z));
    }
	//create a convexhull collision object
    btConvexHullShape* _ConvexHull;
    _ConvexHull = new btConvexHullShape(bulletVertices[0], bulletVertices.size(), sizeof(btVector3));
	//optimise the shape to remove all the unnecessary vertices. while till surrounding the whole shape.
    _ConvexHull->optimizeConvexHull();
    return _ConvexHull;
}