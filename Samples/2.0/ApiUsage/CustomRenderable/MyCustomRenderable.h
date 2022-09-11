
#ifndef _Demo_MyCustomRenderable_H_
#define _Demo_MyCustomRenderable_H_

#include "OgreMovableObject.h"
#include "OgreRenderable.h"

namespace Ogre
{
    class MyCustomRenderable : public MovableObject, public Renderable
    {
        void createBuffers(void);

    public:
        MyCustomRenderable( IdType id, ObjectMemoryManager *objectMemoryManager,
                            SceneManager* manager, uint8 renderQueueId );
        virtual ~MyCustomRenderable();

        //Overrides from MovableObject
        virtual const String& getMovableType(void) const;

        //Overrides from Renderable
        virtual const LightList& getLights(void) const;
        virtual void getRenderOperation( v1::RenderOperation& op, bool casterPass );
        virtual void getWorldTransforms( Matrix4* xform ) const;
        virtual bool getCastsShadows(void) const;
    };

    class MyCustomRenderableFactory : public Ogre::MovableObjectFactory
    {
    public:
        static Ogre::String FACTORY_TYPE_NAME;
        const Ogre::String& getType() const override { return FACTORY_TYPE_NAME; }
        static const Ogre::uint8 RENDER_QUEUE_ID = 3;

        void destroyInstance(Ogre::MovableObject* obj) override
        {
            OGRE_DELETE obj;
        }

    protected:
        virtual Ogre::MovableObject* createInstanceImpl(
            Ogre::IdType id,
            Ogre::ObjectMemoryManager* objectMemoryManager,
            Ogre::SceneManager* manager,
            const Ogre::NameValuePairList* params = 0) override
        {
            return OGRE_NEW MyCustomRenderable(id, objectMemoryManager, manager, RENDER_QUEUE_ID);
        }
    };
}

#endif
