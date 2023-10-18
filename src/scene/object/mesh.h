#ifndef MESH_H
#define MESH_H

#include "object.h"
#include "string.h"

namespace scene
{

    class Mesh: public HittableGroup
    {
        public:
        virtual ~Mesh() = default;

    };

    class ObjMesh: public Mesh {
        public:
        ObjMesh(char* dir) {}
    };

}

#endif