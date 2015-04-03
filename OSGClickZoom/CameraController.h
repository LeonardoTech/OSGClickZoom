#pragma once
#include "OSGCommon.h"
class CameraController
{
public:
	CameraController();
	~CameraController();

	void UpdateCamera(osgViewer::Viewer* viewer);

protected:
	osg::ref_ptr<osg::Camera> _camera;
	//osgViewer::Viewer* hostViewer;

	// initial camera pos.
};

