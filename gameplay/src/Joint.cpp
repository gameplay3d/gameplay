#include "Base.h"
#include "Joint.h"
#include "MeshSkin.h"

namespace gameplay
{

Joint::Joint(const char* id)
    : Node(id), _jointMatrixDirty(true)
{
}

Joint::~Joint()
{
}

Joint* Joint::create(const char* id)
{
    return new Joint(id);
}

Node* Joint::cloneSingleNode(NodeCloneContext &context) const
{
    Joint* copy = Joint::create(getId());
    GP_ASSERT(copy);
    context.registerClonedNode(this, copy);
    copy->_bindPose = _bindPose;
    Node::cloneInto(copy, context);
    return copy;
}

Node::Type Joint::getType() const
{
    return Node::JOINT;
}

Scene* Joint::getScene() const
{
    // Overrides Node::getScene() to search the node our skins.
    for (const SkinReference* itr = &_skin; itr && itr->skin; itr = itr->next)
    {
        Model* model = itr->skin ? itr->skin->getModel() : NULL;
        if (model)
        {
            Node* node = model->getNode();
            if (node)
            {
                Scene* scene = node->getScene();
                if (scene)
                    return scene;
            }
        }
    }

    return Node::getScene();
}

void Joint::transformChanged()
{
    Node::transformChanged();
    _jointMatrixDirty = true;
}

void Joint::updateJointMatrix(const Matrix& bindShape, Vector4* matrixPalette)
{
    // Note: If more than one MeshSkin influences this Joint, we need to skip
    // the _jointMatrixDirty optimization since updateJointMatrix() may be
    // called multiple times a frame with different bindShape matrices (and
    // different matrixPallete pointers).
    if (_skin.next || _jointMatrixDirty)
    {
        _jointMatrixDirty = false;

        static Matrix t;
        Matrix::multiply(Node::getWorldMatrix(), getInverseBindPose(), &t);
        Matrix::multiply(t, bindShape, &t);

        GP_ASSERT(matrixPalette);
        matrixPalette[0].set(t.m[0], t.m[4], t.m[8], t.m[12]);
        matrixPalette[1].set(t.m[1], t.m[5], t.m[9], t.m[13]);
        matrixPalette[2].set(t.m[2], t.m[6], t.m[10], t.m[14]);
    }
}

const Matrix& Joint::getInverseBindPose() const
{
    return _bindPose;
}

void Joint::setInverseBindPose(const Matrix& m)
{
    _bindPose = m;
    _jointMatrixDirty = true;
}

void Joint::addSkin(MeshSkin* skin)
{
    if (!_skin.skin)
    {
        // Store skin in root reference
        _skin.skin = skin;
    }
    else
    {
        // Add a new SkinReference to the end of our list
        SkinReference* ref = &_skin;
        while (ref->next)
        {
            ref = ref->next;
        }
        ref->next = new SkinReference();
        ref->next->skin = skin;
    }
}

void Joint::removeSkin(MeshSkin* skin)
{
    if (_skin.skin == skin)
    {
        // Skin is our root referenced skin
        _skin.skin = NULL;

        // Shift the next skin reference down to the root
        if (_skin.next)
        {
            SkinReference* tmp = _skin.next;
            _skin.skin = tmp->skin;
            _skin.next = tmp->next;
            tmp->next = NULL; // prevent deletion
            SAFE_DELETE(tmp);
        }
    }
    else
    {
        // Search for the entry referencing this skin
        SkinReference* ref = &_skin;
        while (SkinReference* tmp = ref->next)
        {
            if (tmp->skin == skin)
            {
                // Link this refernce out
                ref->next = tmp->next;
                tmp->next = NULL; // prevent deletion
                SAFE_DELETE(tmp);
                break;
            }
            ref = tmp;
        }
    }
}

Joint::SkinReference::SkinReference()
    : skin(NULL), next(NULL)
{
}

Joint::SkinReference::~SkinReference()
{
    SAFE_DELETE(next);
}

}
