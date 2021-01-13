#pragma once

//reference https://github.com/assimp/assimp/blob/master/samples/SimpleOpenGL/Sample_SimpleOpenGL.c

/* assimp include files. These three are usually needed. */
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Math/float3.h"


#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

class BoundingBox
{

public:

    BoundingBox(const aiScene* mScene = nullptr){
        scene = mScene;
    }

    ~BoundingBox(){
        scene = nullptr;
    }

    void GetBoundingBoxForNode(const aiNode *nd,
                                   float3 *min,
                                   float3 *max,
                                   aiMatrix4x4 *trafo)
    {
        aiMatrix4x4 prev;
        unsigned int n = 0, t;

        prev = *trafo;
        aiMultiplyMatrix4(trafo, &nd->mTransformation);

        for (; n < nd->mNumMeshes; ++n)
        {
            const aiMesh *mesh = scene->mMeshes[nd->mMeshes[n]];
            for (t = 0; t < mesh->mNumVertices; ++t)
            {

                float3 tmp = float3(mesh->mVertices[t].x,mesh->mVertices[t].y,mesh->mVertices[t].z);
                aiTransformVecByMatrix4(&mesh->mVertices[t], trafo);

                min->x = aisgl_min(min->x, tmp.x);
                min->y = aisgl_min(min->y, tmp.y);
                min->z = aisgl_min(min->z, tmp.z);

                max->x = aisgl_max(max->x, tmp.x);
                max->y = aisgl_max(max->y, tmp.y);
                max->z = aisgl_max(max->z, tmp.z);
            }
        }

        for (n = 0; n < nd->mNumChildren; ++n)
        {
            GetBoundingBoxForNode(nd->mChildren[n], min, max, trafo);
        }
        *trafo = prev;
    }


    void GetBoundingBox(float3 *min, float3 *max)
    {
        aiMatrix4x4 trafo;
        aiIdentityMatrix4(&trafo);

        min->x = min->y = min->z = 1e10f;
        max->x = max->y = max->z = -1e10f;
        GetBoundingBoxForNode(scene->mRootNode, min, max, &trafo);
    }

private:
    const aiScene* scene;

};
