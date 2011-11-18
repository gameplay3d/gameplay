#include "MeshSkin.h"
#include "Node.h"
#include "StringUtil.h"
#include "Mesh.h"
#include "GPBFile.h"
#include "Animations.h"
#include "Transform.h"
#include "../../gameplay/src/Curve.h"

namespace gameplay
{

MeshSkin::MeshSkin(void) :
    _vertexInfluenceCount(0)
{
    setIdentityMatrix(_bindShape);
}

MeshSkin::~MeshSkin(void)
{
}

unsigned int MeshSkin::getTypeId(void) const
{
    return MESHPART_ID;
}

const char* MeshSkin::getElementName(void) const
{
    return "MeshSkin";
}

void MeshSkin::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    write(_bindShape, 16, file);
    write(_joints.size(), file);
    for (std::list<Node*>::const_iterator i = _joints.begin(); i != _joints.end(); i++)
    {
        (*i)->writeBinaryXref(file);
    }
    write(_bindPoses.size() * 16, file);
    for (std::list<Matrix>::const_iterator i = _bindPoses.begin(); i != _bindPoses.end(); i++)
    {
        write(i->m, 16, file);
    }
}

void MeshSkin::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintf(file, "<bindShape>");
    fprintfMatrix4f(file, _bindShape);
    fprintf(file, "</bindShape>");
    fprintf(file, "<joints>");
    for (std::list<std::string>::const_iterator i = _jointNames.begin(); i != _jointNames.end(); i++)
    {
        fprintf(file, "%s ", i->c_str());
    }
    fprintf(file, "</joints>\n");
    fprintf(file, "<bindPoses count=\"%lu\">", _bindPoses.size() * 16);
    for (std::list<Matrix>::const_iterator i = _bindPoses.begin(); i != _bindPoses.end(); i++)
    {
        for (unsigned int j = 0; j < 16; ++j)
        {
            fprintf(file, "%f ", i->m[j]);
        }
    }
    fprintf(file, "</bindPoses>\n");

    fprintElementEnd(file);
}

void MeshSkin::computeBounds()
{
    // Find the offset of the blend indices and blend weights within the mesh vertices
    int blendIndexOffset = -1;
    int blendWeightOffset = -1;
    for (unsigned int i = 0, count = _mesh->getVertexElementCount(); i < count; ++i)
    {
        const VertexElement& e = _mesh->getVertexElement(i);
        switch (e.usage)
        {
        case BLENDINDICES:
            blendIndexOffset = i;
            break;
        case BLENDWEIGHTS:
            blendWeightOffset = i;
            break;
        }
    }
    if (blendIndexOffset != -1 && blendWeightOffset != -1)
    {
        // Construct a new list of joints which contains all the joints in this mesh skin,
        // as WELL as any nodes that are direct parents of the root joint.
        // We need to do this since animations that affect parent nodes of our joints will
        // ultimately affect the final position of transformed vertices.
        std::vector<Node*> joints;
        for (std::list<Node*>::const_iterator itr = _joints.begin(); itr != _joints.end(); itr++)
        {
            joints.push_back(*itr);
        }

        // Add parent joints that are not yet in the list
        Node* joint = joints[0];
        while (joint->getParent())
        {
            joint = joint->getParent();
            if (std::find(joints.begin(), joints.end(), joint) == joints.end())
                joints.push_back(joint);
        }

        unsigned int jointCount = joints.size();
        unsigned int boneCount = _joints.size();

        std::vector<AnimationChannel*> channels;
        std::vector<Node*> channelTargets;
        std::vector<Curve*> curves;

        // Construct a list of all animation channels that target the joints affecting this mesh skin
        for (unsigned int i = 0; i < jointCount; ++i)
        {
            joint = joints[i];

            // Find all animations that target this joint
            Animations* animations = GPBFile::getInstance()->getAnimations();
            for (unsigned int j = 0, animationCount = animations->getAnimationCount(); j < animationCount; ++j)
            {
                Animation* animation = animations->getAnimation(j);
                for (unsigned int k = 0, channelCount = animation->getAnimationChannelCount(); k < channelCount; ++k)
                {
                    AnimationChannel* channel = animation->getAnimationChannel(k);
                    if (channel->getTargetId() == joint->getId())
                    {
                        if (std::find(channels.begin(), channels.end(), channel) == channels.end())
                        {
                            channels.push_back(channel);
                            channelTargets.push_back(joint);
                        }
                    }
                }
            }

            // TODO: Calculate local (non-transformed/non-animated) bounding volumes for each joint that can be used to 
            // do more precise bounds checking for skinned meshes at runtime.
            // Find all vertices that this joint influences
            /*vertices.clear();
            for (unsigned int j = 0, count = _mesh->getVertexCount(); j < count; ++j)
            {
                const Vertex& v = _mesh->getVertex(j);
                if (v.blendIndices.x == i || v.blendIndices.y == i || v.blendIndices.z == i || v.blendIndices.w == i)
                {
                    vertices.push_back(const_cast<Vertex*>(&v));
                }
            }*/
        }

        // Create a Curve for each animation channel
        float maxDuration = 0.0f;
        for (unsigned int i = 0, channelCount = channels.size(); i < channelCount; ++i)
        {
            AnimationChannel* channel = channels[i];

            const std::vector<float>& keyTimes = channel->getKeyTimes();
            unsigned int keyCount = keyTimes.size();
            if (keyCount == 0)
                continue;

            // Create a curve for this animation channel
            Curve* curve = NULL;
            switch (channel->getTargetAttribute())
            {
            case Transform::ANIMATE_SCALE_ROTATE_TRANSLATE:
                curve = new Curve(keyCount, 10);
                curve->addQuaternionOffset(3);
                break;
            }
            if (curve == NULL)
            {
                // Unsupported/not implemented curve type 
                continue;
            }

            // Copy key values into a temporary array
            unsigned int keyValuesCount = channel->getKeyValues().size();
            float* keyValues = new float[keyValuesCount];
            for (unsigned int j = 0; j < keyValuesCount; ++j)
                keyValues[j] = channel->getKeyValues()[j];

            // Determine animation duration
            float startTime = keyTimes[0];
            float duration = keyTimes[keyCount-1] - startTime;
            if (duration > maxDuration)
                maxDuration = duration;

            // Set curve points
            float* keyValuesPtr = keyValues;
            for (unsigned int j = 0; j < keyCount; ++j)
            {
                // Store time normalized, between 0-1
                float t = (keyTimes[j] - startTime) / duration;

                // Set the curve point
                // TODO: Handle other interpolation types
                curve->setPoint(j, t, keyValuesPtr, gameplay::Curve::LINEAR);

                // Move to the next point on the curve
                keyValuesPtr += curve->getComponentCount();
            }

            delete[] keyValues;
            keyValues = NULL;

            curves.push_back(curve);
        }

        // Compute an all-encompassing bounding volume for the MeshSkin that contains all possible vertex
        // positions for all animations targetting the skin.
        //
        // This is accomplished through the following steps:
        //
        //  - Step over time in small increments (60 fps ~= 17 ms)
        //  - For each time interval:
        //     - For each animation channel:
        //        - Evalulate the curve at the current time
        //        - store the result in a local transform for the target joint (SRT)
        //     - Calculate final matrix pallette of resolved world joint transforms (multplying by parent joints)
        //     - For each vertex in the mesh:
        //        - Calculate final vertex position using skinning w/ blendindices and blendweights and the matrix pallette
        //        - Update the bounding volume of the MeshSkin based on the calculated vertex position
        //
        // First backup existing node transforms so we can restore them when we are finished
        Matrix* oldTransforms = new Matrix[boneCount];
        for (unsigned int i = 0; i < boneCount; ++i)
        {
            memcpy(oldTransforms[i].m, joints[i]->getTransformMatrix().m, 16 * sizeof(float));
        }

        float srt[10];
        Matrix temp;
        Matrix* jointTransforms = new Matrix[boneCount];
        _mesh->bounds.min.set(FLT_MAX, FLT_MAX, FLT_MAX);
        _mesh->bounds.max.set(FLT_MIN, FLT_MIN, FLT_MIN);
        float time = 0.0f;
        while (time < maxDuration)
        {
            // Evaluate joint transforms at this time interval
            for (unsigned int i = 0, curveCount = curves.size(); i < curveCount; ++i)
            {
                Node* joint = channelTargets[i];
                Curve* curve = curves[i];

                // Evalulate the curve at this time to get the new value
                float tn = time / maxDuration;
                if (tn > 1.0f)
                    tn = 1.0f;
                curve->evaluate(tn, srt);

                // Update the joint's local transform
                Matrix::createTranslation(srt[7], srt[8], srt[9], temp.m);
                temp.rotate(*((Quaternion*)&srt[3]));
                temp.scale(srt[0], srt[1], srt[2]);
                joint->setTransformMatrix(temp.m);
            }

            // Store the final matrix pallette of resovled world space joint matrices
            std::list<Matrix>::const_iterator bindPoseItr = _bindPoses.begin();
            for (unsigned int i = 0; i < boneCount; ++i, bindPoseItr++)
            {
                Matrix& m = jointTransforms[i];
                Matrix::multiply(joints[i]->getWorldMatrix().m, bindPoseItr->m, m.m);
                Matrix::multiply(m.m, _bindShape, m.m);
            }

            // Loop through all vertices in the mesh and calculate the final animated position
            // at this time interval using the matrix pallette and blend indices/weights information.
            Vector3 skinnedPos;
            Vector3 tempPos;
            int blendIndices[4];
            float blendWeights[4];
            for (unsigned int i = 0, vertexCount = _mesh->getVertexCount(); i < vertexCount; ++i)
            {
                const Vertex& v = _mesh->getVertex(i);

                // Get blend indices
                blendIndices[0] = (int)v.blendIndices.x;
                blendIndices[1] = (int)v.blendIndices.y;
                blendIndices[2] = (int)v.blendIndices.z;
                blendIndices[3] = (int)v.blendIndices.w;

                // Get blend weights
                blendWeights[0] = v.blendWeights.x;
                blendWeights[1] = v.blendWeights.y;
                blendWeights[2] = v.blendWeights.z;
                blendWeights[3] = v.blendWeights.w;

                // Skin this vertex using the standard vertex skinning algorithm
                skinnedPos.set(0, 0, 0);
                for (unsigned int j = 0; j < 4; ++j)
                {
                    if (blendIndices[j] >= 0 && blendIndices[j] < (int)boneCount)
                    {
                        jointTransforms[blendIndices[j]].transformPoint(v.position, &tempPos);
                        tempPos.scale(blendWeights[j]);
                        skinnedPos.add(tempPos);
                    }
                }

                // Update the bounding box information for this MeshSkin
                if (skinnedPos.x < _mesh->bounds.min.x)
                    _mesh->bounds.min.x = skinnedPos.x;
                if (skinnedPos.y < _mesh->bounds.min.y)
                    _mesh->bounds.min.y = skinnedPos.y;
                if (skinnedPos.z < _mesh->bounds.min.z)
                    _mesh->bounds.min.z = skinnedPos.z;
                if (skinnedPos.x > _mesh->bounds.max.x)
                    _mesh->bounds.max.x = skinnedPos.x;
                if (skinnedPos.y > _mesh->bounds.max.y)
                    _mesh->bounds.max.y = skinnedPos.y;
                if (skinnedPos.z > _mesh->bounds.max.z)
                    _mesh->bounds.max.z = skinnedPos.z;
            }

            // Increment time by 1/60th of second (~ 17 ms)
            time += 170.0f;
        }

        // Compute bounding sphere info for the skin. This computation is not very accurate since it
        // creates the bounding sphere from the bounding box info - so it will not normally provide a
        // tight fit. However, bounding volumes for mesh skins are very approximate anyway and only
        // useful as a very broad/high level first test
        Vector3::add(_mesh->bounds.min, _mesh->bounds.max, &_mesh->bounds.center);
        _mesh->bounds.center.scale(0.5f);
        _mesh->bounds.radius = _mesh->bounds.center.distance(_mesh->bounds.max);

        // Restore original joint transforms
        for (unsigned int i = 0; i < boneCount; ++i)
        {
            joints[i]->setTransformMatrix(oldTransforms[i].m);
        }

        // Cleanup
        for (unsigned int i = 0, curveCount = curves.size(); i < curveCount; ++i)
        {
            delete curves[i];
        }
        delete[] oldTransforms;
        delete[] jointTransforms;
    }
}

void MeshSkin::setBindShape(const float data[])
{
    for (int i = 0; i < 16; i++)
    {
        _bindShape[i] = data[i];
    }
}

void MeshSkin::setVertexInfluenceCount(unsigned int count)
{
    _vertexInfluenceCount = count;
}

void MeshSkin::setJointNames(const std::list<std::string>& list)
{
    _jointNames = list;
}

const std::list<std::string>& MeshSkin::getJointNames()
{
    return _jointNames;
}

void MeshSkin::setJoints(const std::list<Node*>& list)
{
    _joints = list;
}

void MeshSkin::setBindPoses(std::vector<Matrix>& list)
{
    for (std::vector<Matrix>::iterator i = list.begin(); i != list.end(); i++)
    {
        _bindPoses.push_back(*i);
    }
}

bool MeshSkin::hasJoint(const char* id)
{
    for (std::list<std::string>::iterator i = _jointNames.begin(); i != _jointNames.end(); i++)
    {
        if (equals(*i, id))
        {
            return true;
        }
    }
    return false;
}

}
