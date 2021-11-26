
#include "MyGameState.h"
#include "GraphicsSystem.h"

#include "OgreSceneManager.h"
#include "OgreItem.h"

#include "OgreTextAreaOverlayElement.h"

#include "OgreRoot.h"
#include "OgreHlmsUnlit.h"
#include "OgreHlmsUnlitDatablock.h"
#include "OgreTextureGpuManager.h"

using namespace Demo;

namespace Demo
{
    MyGameState::MyGameState( const Ogre::String &helpDescription ) :
        TutorialGameState( helpDescription ),
        mSceneNode( 0 ),
        mDisplacement( 0 )
    {
    }
    //-----------------------------------------------------------------------------------
    void MyGameState::createScene01(void)
    {
        Ogre::SceneManager *sceneManager = mGraphicsSystem->getSceneManager();

        Ogre::Item *item = sceneManager->createItem( "Cube_d.mesh",
                                                     Ogre::ResourceGroupManager::
                                                     AUTODETECT_RESOURCE_GROUP_NAME,
                                                     Ogre::SCENE_DYNAMIC );

        Ogre::TextureGpuManager* textureManager = Ogre::Root::getSingleton()
            .getRenderSystem()
            ->getTextureGpuManager();
        Ogre::TextureGpu* texture = textureManager->createOrRetrieveTexture(
            "doesnt-exist.png",
            Ogre::GpuPageOutStrategy::Discard,
            Ogre::TextureFlags::AutomaticBatching | Ogre::TextureFlags::PrefersLoadingFromFileAsSRGB,
            Ogre::TextureTypes::Type2D,
            Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);

        Ogre::HlmsUnlit* hlms = static_cast<Ogre::HlmsUnlit*>(Ogre::Root::getSingleton().getHlmsManager()->getHlms(Ogre::HLMS_UNLIT));
        auto* datablock = static_cast<Ogre::HlmsUnlitDatablock*>(
            hlms->createDatablock("blah", "blah", Ogre::HlmsMacroblock(), Ogre::HlmsBlendblock(), Ogre::HlmsParamVec()));
        datablock->setTexture(0, texture);

        item->setDatablock(datablock);
        item->setDatablock(Ogre::IdString{});
        hlms->destroyDatablock(datablock->getName());

        // Fixes bug
        // if(texture->isDataReady())
            textureManager->destroyTexture(texture);
        // Hangs here
        textureManager->waitForStreamingCompletion();

        mSceneNode = sceneManager->getRootSceneNode( Ogre::SCENE_DYNAMIC )->
                createChildSceneNode( Ogre::SCENE_DYNAMIC );

        mSceneNode->attachObject( item );

        TutorialGameState::createScene01();
    }
    //-----------------------------------------------------------------------------------
    void MyGameState::update( float timeSinceLast )
    {
        const Ogre::Vector3 origin( -5.0f, 0.0f, 0.0f );

        mDisplacement += timeSinceLast * 4.0f;
        mDisplacement = fmodf( mDisplacement, 10.0f );

        mSceneNode->setPosition( origin + Ogre::Vector3::UNIT_X * mDisplacement );

        TutorialGameState::update( timeSinceLast );
    }
}
