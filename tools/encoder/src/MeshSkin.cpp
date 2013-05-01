#include "Base.h"
#include "MeshSkin.h"
#include "Node.h"
#include "StringUtil.h"
#include "Mesh.h"
#include "GPBFile.h"
#include "Animations.h"
#include "Transform.h"
#include "Curve.h"
#include "Matrix.h"

namespace gameplay
{

MeshSkin::MeshSkin(void) :
    _vertexInfluenceCount(0)
{
    Matrix::setIdentity(_bindShape);
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
    write((unsigned int)_joints.size(), file);
    for (std::vector<Node*>::const_iterator i = _joints.begin(); i != _joints.end(); ++i)
    {
        (*i)->writeBinaryXref(file);
    }
    write((unsigned int)_bindPoses.size() * 16, file);
    for (std::vector<Matrix>::const_iterator i = _bindPoses.begin(); i != _bindPoses.end(); ++i)
    {
        write(i->m, 16, file);
    }

    /*
    // Write joint bounding spheres
    write((unsigned int)_jointBounds.size(), file);
    for (unsigned int i = 0; i < _jointBounds.size(); ++i)
    {
        BoundingVolume& v = _jointBounds[i];
        write(v.center.x, file);
        write(v.center.y, file);
        write(v.center.z, file);
        write(v.radius, file);
    }
    */
}

void MeshSkin::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintf(file, "<bindShape>");
    fprintfMatrix4f(file, _bindShape);
    fprintf(file, "</bindShape>");
    fprintf(file, "<joints>");
    for (std::vector<std::string>::const_iterator i = _jointNames.begin(); i != _jointNames.end(); ++i)
    {
        fprintf(file, "%s ", i->c_str());
    }
    fprintf(file, "</joints>\n");
    fprintf(file, "<bindPoses count=\"%lu\">", _bindPoses.size() * 16);
    for (std::vector<Matrix>::const_iterator i = _bindPoses.begin(); i != _bindPoses.end(); ++i)
    {
        for (unsigned int j = 0; j < 16; ++j)
        {
            fprintf(file, "%f ", i->m[j]);
        }
    }
    fprintf(file, "</bindPoses>\n");

    fprintElementEnd(file);
}

unsigned int MeshSkin::getJointCount() const
{
    return _joints.size();
}

void MeshSkin::setBindShape(const float data[])
{
    for (int i = 0; i < 16; ++i)
    {
        _bindShape[i] = data[i];
    }
}

void MeshSkin::setVertexInfluenceCount(unsigned int count)
{
    _vertexInfluenceCount = count;
}

void MeshSkin::setJointNames(const std::vector<std::string>& list)
{
    _jointNames = list;
}

const std::vector<std::string>& MeshSkin::getJointNames()
{
    return _jointNames;
}

const std::vector<Node*>& MeshSkin::getJoints() const
{
    return _joints;
}

void MeshSkin::setJoints(const std::vector<Node*>& list)
{
    _joints = list;
}

void MeshSkin::setBindPoses(std::vector<Matrix>& list)
{
    for (std::vector<Matrix>::iterator i = list.begin(); i != list.end(); ++i)
    {
        _bindPoses.push_back(*i);
    }
}

bool MeshSkin::hasJoint(const char* id)
{
    for (std::vector<std::string>::iterator i = _jointNames.begin(); i != _jointNames.end(); ++i)
    {
        if (equals(*i, id))
        {
            return true;
        }
    }
    return false;
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
    if (blendIndexOffset == -1 || blendWeightOffset == -1)
    {
        // Need blend indices and blend weights to calculate skinned bounding volume
        return;
    }

    LOG(2, "Computing bounds for skin of mesh: %s\n", _mesh->getId().c_str());

    // Get the root joint
    Node* rootJoint = _joints[0];
    Node* parent = rootJoint->getParent();
    while (parent)
    {
        // Is this parent in the list of joints that form the skeleton?
        // If not, then it's simply a parent node to the root joint
        if (find(_joints.begin(), _joints.end(), parent) != _joints.end())
        {
            rootJoint = parent;
        }
        parent = parent->getParent();
    }

    // If the root joint has a parent node, temporarily detach it so that its transform is
    // not included in the bounding volume calculation below
    Node* rootJointParent = rootJoint->getParent();
    if (rootJointParent)
    {
        rootJointParent->removeChild(rootJoint);
    }

    unsigned int jointCount = _joints.size();
    unsigned int vertexCount = _mesh->getVertexCount();

    LOG(3, "  %u joints found.\n", jointCount);

    std::vector<AnimationChannel*> channels;
    std::vector<Node*> channelTargets;
    std::vector<Curve*> curves;
    std::vector<Vector3> vertices;
    _jointBounds.resize(jointCount);

    // Construct a list of all animation channels that target the joints affecting this mesh skin
    LOG(3, "  Collecting animations...\n");
    LOG(3, "  0%%\r");
    for (unsigned int i = 0; i < jointCount; ++i)
    {
        Node* joint = _joints[i];

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
                    if (find(channels.begin(), channels.end(), channel) == channels.end())
                    {
                        channels.push_back(channel);
                        channelTargets.push_back(joint);
                    }
                }
            }
        }

        // Calculate the local bounding volume for this joint
        vertices.clear();
        BoundingVolume jointBounds;
        jointBounds.min.set(FLT_MAX, FLT_MAX, FLT_MAX);
        jointBounds.max.set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        for (unsigned int j = 0; j < vertexCount; ++j)
        {
            const Vertex& v = _mesh->getVertex(j);

            if ((v.blendIndices.x == i && !ISZERO(v.blendWeights.x)) ||
                (v.blendIndices.y == i && !ISZERO(v.blendWeights.y)) ||
                (v.blendIndices.z == i && !ISZERO(v.blendWeights.z)) ||
                (v.blendIndices.w == i && !ISZERO(v.blendWeights.w)))
            {
                vertices.push_back(v.position);
                // Update box min/max
                if (v.position.x < jointBounds.min.x)
                    jointBounds.min.x = v.position.x;
                if (v.position.y < jointBounds.min.y)
                    jointBounds.min.y = v.position.y;
                if (v.position.z < jointBounds.min.z)
                    jointBounds.min.z = v.position.z;
                if (v.position.x > jointBounds.max.x)
                    jointBounds.max.x = v.position.x;
                if (v.position.y > jointBounds.max.y)
                    jointBounds.max.y = v.position.y;
                if (v.position.z > jointBounds.max.z)
                    jointBounds.max.z = v.position.z;
            }
        }
        if (vertices.size() > 0)
        {
            // Compute center point
            Vector3::add(jointBounds.min, jointBounds.max, &jointBounds.center);
            jointBounds.center.scale(0.5f);
            // Compute radius
            for (unsigned int j = 0, jointVertexCount = vertices.size(); j < jointVertexCount; ++j)
            {
                float d = jointBounds.center.distanceSquared(vertices[j]);
                if (d > jointBounds.radius)
                    jointBounds.radius = d;
            }
            jointBounds.radius = sqrt(jointBounds.radius);
        }
        _jointBounds[i] = jointBounds;

        LOG(3, "  %d%%\r", (int)((float)(i+1) / (float)jointCount * 100.0f));
    }
    LOG(3, "\n");

    unsigned int channelCount = channels.size();

    // Create a Curve for each animation channel
    float maxDuration = 0.0f;
    LOG(3, "  Building animation curves...\n");
    LOG(3, "  0%%\r");
    for (unsigned int i = 0; i < channelCount; ++i)
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
            curve->setQuaternionOffset(3);
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

        if (duration > 0.0f)
        {
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
            curves.push_back(curve);
        }

        delete[] keyValues;
        keyValues = NULL;

        LOG(3, "  %d%%\r", (int)((float)(i+1) / (float)channelCount * 100.0f));
    }
    LOG(3, "\n");

    // Compute a total combined bounding volume for the MeshSkin that contains all possible
    // vertex positions for all animations targeting the skin. This rough approximation allows
    // us to store a volume that can be used for rough intersection tests (such as for visibility
    // determination) efficiently at runtime.

    // Backup existing node transforms so we can restore them when we are finished
    Matrix* oldTransforms = new Matrix[jointCount];
    for (unsigned int i = 0; i < jointCount; ++i)
    {
        memcpy(oldTransforms[i].m, _joints[i]->getTransformMatrix().m, 16 * sizeof(float));
    }

    float time = 0.0f;
    float srt[10];
    Matrix temp;
    Matrix* jointTransforms = new Matrix[jointCount];
    _mesh->bounds.min.set(FLT_MAX, FLT_MAX, FLT_MAX);
    _mesh->bounds.max.set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    _mesh->bounds.center.set(0, 0, 0);
    _mesh->bounds.radius = 0;
    Vector3 skinnedPos;
    Vector3 tempPos;
    LOG(3, "  Evaluating joints...\n");
    LOG(3, "  0%%\r");
    BoundingVolume finalBounds;
    while (time <= maxDuration)
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
        std::vector<Matrix>::const_iterator bindPoseItr = _bindPoses.begin();
        for (unsigned int i = 0; i < jointCount; ++i, bindPoseItr++)
        {
            BoundingVolume bounds = _jointBounds[i];
            if (ISZERO(bounds.radius))
                continue;

            Matrix& m = jointTransforms[i];
            Matrix::multiply(_joints[i]->getWorldMatrix().m, bindPoseItr->m, m.m);
            Matrix::multiply(m.m, _bindShape, m.m);

            // Get a world-space bounding volume for this joint
            bounds.transform(m);
            if (ISZERO(finalBounds.radius))
                finalBounds = bounds;
            else
                finalBounds.merge(bounds);
        }

        // Increment time by 1/30th of second (~ 33 ms)
        if (time < maxDuration && (time + 33.0f) > maxDuration)
            time = maxDuration;
        else
            time += 33.0f;

        LOG(3, "  %d%%\r", (int)(time / maxDuration * 100.0f));
    }
    LOG(3, "\n");

    // Update the bounding sphere for the mesh
    _mesh->bounds = finalBounds;

    // Restore original joint transforms
    for (unsigned int i = 0; i < jointCount; ++i)
    {
        _joints[i]->setTransformMatrix(oldTransforms[i].m);
    }

    // Cleanup
    for (unsigned int i = 0, curveCount = curves.size(); i < curveCount; ++i)
    {
        delete curves[i];
    }
    delete[] oldTransforms;
    delete[] jointTransforms;

    // Restore removed joints
    if (rootJointParent)
    {
        rootJointParent->addChild(rootJoint);
    }
}

}
