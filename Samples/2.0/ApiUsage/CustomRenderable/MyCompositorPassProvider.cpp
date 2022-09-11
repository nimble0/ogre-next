#include "MyCompositorPassProvider.h"
#include "MyCompositorPassDef.h"
#include "MyCompositorPass.h"


using namespace Ogre;

Ogre::CompositorPassDef* MyCompositorPassProvider::addPassDef(
	Ogre::CompositorPassType passType,
	Ogre::IdString customId,
	Ogre::CompositorTargetDef* parentTargetDef,
	Ogre::CompositorNodeDef* parentNodeDef)
{
	if(customId == "mypass")
		return OGRE_NEW MyCompositorPassDef(parentTargetDef);
	else
		return nullptr;
}

Ogre::CompositorPass* MyCompositorPassProvider::addPass(
	const Ogre::CompositorPassDef* definition,
	Ogre::Camera *defaultCamera,
	Ogre::CompositorNode *parentNode,
	const Ogre::RenderTargetViewDef* rtvDef,
	Ogre::SceneManager* sceneManager )
{
	assert(dynamic_cast<const MyCompositorPassDef*>(definition));
	return OGRE_NEW MyCompositorPass(
		static_cast<const MyCompositorPassDef*>(definition),
		defaultCamera,
		sceneManager,
		rtvDef,
		parentNode);
}
