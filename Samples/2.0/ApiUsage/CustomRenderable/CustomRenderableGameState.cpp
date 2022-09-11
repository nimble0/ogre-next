
#include "CustomRenderableGameState.h"
#include "CameraController.h"
#include "GraphicsSystem.h"
#include "MyCustomRenderable.h"
#include "MyCompositorPassProvider.h"

#include "OgreSceneManager.h"
#include "OgreRoot.h"
#include "Compositor/OgreCompositorManager2.h"

#include "OgreCamera.h"

namespace Demo
{
    CustomRenderableGameState::CustomRenderableGameState( const Ogre::String &helpDescription ) :
        TutorialGameState( helpDescription ),
        mMyCustomRenderable( 0 )
    {
    }
    void CustomRenderableGameState::initialize0(void)
    {
        Ogre::Root::getSingleton()
            .getCompositorManager2()
            ->setCompositorPassProvider(OGRE_NEW Ogre::MyCompositorPassProvider());
    }
    void CustomRenderableGameState::initialize(void)
    {
        if(!Ogre::Root::getSingleton().hasMovableObjectFactory(Ogre::MyCustomRenderableFactory::FACTORY_TYPE_NAME))
            Ogre::Root::getSingleton().addMovableObjectFactory(OGRE_NEW Ogre::MyCustomRenderableFactory);
    }
    //-----------------------------------------------------------------------------------
    void CustomRenderableGameState::createScene01(void)
    {
        Ogre::SceneManager *sceneManager = mGraphicsSystem->getSceneManager();

        mMyCustomRenderable = OGRE_NEW Ogre::MyCustomRenderable(
                    Ogre::Id::generateNewId<Ogre::MovableObject>(),
                    &sceneManager->_getEntityMemoryManager( Ogre::SCENE_DYNAMIC ),
                    sceneManager, 10u );

        Ogre::SceneNode *sceneNode = sceneManager->getRootSceneNode( Ogre::SCENE_DYNAMIC )->
                createChildSceneNode( Ogre::SCENE_DYNAMIC );
        sceneNode->attachObject( mMyCustomRenderable );

        Ogre::Light *light = sceneManager->createLight();
        Ogre::SceneNode *lightNode = sceneManager->getRootSceneNode()->createChildSceneNode();
        lightNode->attachObject( light );
        light->setPowerScale( Ogre::Math::PI ); //Since we don't do HDR, counter the PBS' division by PI
        light->setType( Ogre::Light::LT_DIRECTIONAL );
        light->setDirection( Ogre::Vector3( -1, -1, -1 ).normalisedCopy() );

        mCameraController = new CameraController( mGraphicsSystem, false );

        TutorialGameState::createScene01();
    }
    //-----------------------------------------------------------------------------------
    void CustomRenderableGameState::destroyScene(void)
    {
        OGRE_DELETE mMyCustomRenderable;
        mMyCustomRenderable = 0;
    }
}
