#ifndef _Demo_MyCompositorPass_H_
#define _Demo_MyCompositorPass_H_

#include <OgrePrerequisites.h>
#include <Compositor/Pass/OgreCompositorPass.h>
#include <Math/Array/OgreNodeMemoryManager.h>
#include <Math/Array/OgreObjectMemoryManager.h>

#include <memory>
#include <vector>

#include "OgreHeaderPrefix.h"


namespace Ogre {

class MyCompositorPassDef;
class MyCustomRenderable;
class RenderQueue;
class Item;

class MyCompositorPass : public Ogre::CompositorPass
{
	Ogre::Camera* camera;
	Ogre::SceneManager* sceneManager;
	std::unique_ptr<Ogre::RenderQueue> renderQueue;

	Ogre::ObjectMemoryManager objectMemoryManager;
	std::vector<Ogre::MyCustomRenderable*> objects;
	Ogre::NodeMemoryManager nodeMemoryManager;
	std::vector<std::unique_ptr<Ogre::SceneNode>> nodes;

public:
	MyCompositorPass(
		const MyCompositorPassDef* definition,
		Ogre::Camera* camera,
		Ogre::SceneManager* sceneManager,
		const Ogre::RenderTargetViewDef* rtv,
		Ogre::CompositorNode* parentNode);
	~MyCompositorPass();

	void addObject(int i);
	void clearObjects();
	void execute(const Ogre::Camera* lodCamera) override;

private:
	const MyCompositorPassDef* mDefinition;
};

}

#include "OgreHeaderSuffix.h"

#endif // _Demo_MyCompositorPass_H_
