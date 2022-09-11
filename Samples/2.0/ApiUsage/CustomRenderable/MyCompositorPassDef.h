#ifndef _Demo_MyCompositorPassDef_H_
#define _Demo_MyCompositorPassDef_H_

#include "Compositor/Pass/OgreCompositorPassDef.h"

#include "OgreHeaderPrefix.h"

namespace Ogre {

class MyCompositorPassDef : public Ogre::CompositorPassDef
{
public:
	MyCompositorPassDef(
		Ogre::CompositorTargetDef* parentTargetDef
	) :
		Ogre::CompositorPassDef(Ogre::PASS_CUSTOM, parentTargetDef)
	{
		mProfilingId = "MyCompositorPass";
	}
};

}

#include "OgreHeaderSuffix.h"

#endif // _Demo_MyCompositorPassDef_H_
