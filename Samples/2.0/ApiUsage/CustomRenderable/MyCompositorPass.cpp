#include "MyCompositorPass.h"

#include "MyCompositorPassDef.h"
#include "MyCustomRenderable.h"

#include <Compositor/OgreCompositorNode.h>
#include <Compositor/OgreCompositorWorkspace.h>
#include <Compositor/OgreCompositorWorkspaceListener.h>
#include <OgrePixelFormatGpuUtils.h>
#include <OgreRenderSystem.h>
#include <OgreSceneManager.h>
#include <OgreRenderQueue.h>
#include <OgreRoot.h>
#include <OgreItem.h>

using namespace Ogre;

MyCompositorPass::MyCompositorPass(
	const MyCompositorPassDef* definition,
	Ogre::Camera* camera,
	Ogre::SceneManager* sceneManager,
	const Ogre::RenderTargetViewDef* rtv,
	Ogre::CompositorNode* parentNode
) :
	Ogre::CompositorPass(definition, parentNode),

	camera{camera},
	sceneManager{sceneManager},
	mDefinition{definition}
{
	this->initialize(rtv);

	this->renderQueue = std::make_unique<Ogre::RenderQueue>(
		Ogre::Root::getSingleton().getHlmsManager(),
		this->sceneManager,
		Ogre::Root::getSingleton().getRenderSystem()->getVaoManager());
	this->renderQueue->setSortRenderQueue(
		MyCustomRenderableFactory::RENDER_QUEUE_ID,
		Ogre::RenderQueue::RqSortMode::DisableSort);
}
MyCompositorPass::~MyCompositorPass()
{
	this->clearObjects();
}

void MyCompositorPass::addObject(int i)
{
	Ogre::MyCustomRenderable* object = static_cast<MyCustomRenderable*>(Ogre::Root::getSingleton()
		.getMovableObjectFactory(MyCustomRenderableFactory::FACTORY_TYPE_NAME)
		->createInstance(
			Ogre::Id::generateNewId<Ogre::MovableObject>(),
			&this->objectMemoryManager,
			this->sceneManager,
			0));
	object->setRenderQueueGroup(MyCustomRenderableFactory::RENDER_QUEUE_ID);
	this->objects.push_back(object);

	std::unique_ptr<Ogre::SceneNode> node = std::make_unique<Ogre::SceneNode>(
		Ogre::Id::generateNewId<Ogre::Node>(),
		nullptr,
		&this->nodeMemoryManager,
		nullptr);
	node->setPosition(Ogre::Vector3{5.0f * i, 0.0f, 0.0f});
	node->attachObject(object);
	this->nodes.push_back(std::move(node));

	for(auto renderable : object->mRenderables)
		this->renderQueue->addRenderableV2(0, MyCustomRenderableFactory::RENDER_QUEUE_ID, false, renderable, object);
}

void MyCompositorPass::clearObjects()
{
	this->renderQueue->clear();
	auto& objectFactory = *Ogre::Root::getSingleton()
		.getMovableObjectFactory(MyCustomRenderableFactory::FACTORY_TYPE_NAME);
	for(Ogre::MyCustomRenderable* object : this->objects)
		objectFactory.destroyInstance(object);
	this->objects.clear();
	this->nodes.clear();
}

void MyCompositorPass::execute(const Ogre::Camera* lodCamera)
{
	//Execute a limited number of times?
	if( mNumPassesLeft != std::numeric_limits<Ogre::uint32>::max() )
	{
		if( !mNumPassesLeft )
			return;
		--mNumPassesLeft;
	}

	profilingBegin();

	this->clearObjects();
	for(int i = 0; i < 10; ++i)
		this->addObject(i);

	if(!this->nodes.empty())
	{
		Ogre::Transform t;
		std::size_t numNodes = this->nodeMemoryManager.getFirstNode(t, 0);
		Ogre::Node::updateAllTransforms(numNodes, t);
	}

	notifyPassEarlyPreExecuteListeners();

	executeResourceTransitions();
	setRenderPassDescToCurrent();

	this->renderQueue->renderPassPrepare(false, false);

	//Fire the listener in case it wants to change anything
	notifyPassPreExecuteListeners();

	this->sceneManager->_setCamerasInProgress(Ogre::CamerasInProgress(this->camera));
	this->sceneManager->_setCurrentCompositorPass(this);

	Ogre::RenderSystem* renderSystem = this->sceneManager->getDestinationRenderSystem();
	renderSystem->executeRenderPassDescriptorDelayedActions();

	this->renderQueue->render(
		Ogre::Root::getSingleton().getRenderSystem(),
		MyCustomRenderableFactory::RENDER_QUEUE_ID,
		MyCustomRenderableFactory::RENDER_QUEUE_ID + 1,
		false,
		false);
	this->renderQueue->frameEnded();

	this->sceneManager->_setCurrentCompositorPass(nullptr);

	notifyPassPosExecuteListeners();

	profilingEnd();
}

