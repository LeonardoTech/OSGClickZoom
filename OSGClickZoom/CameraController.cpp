#include "CameraController.h"


CameraController::CameraController()
{
}

CameraController::~CameraController()
{
}

void CameraController::UpdateCamera(osgViewer::Viewer* viewer)
{
	if (!viewer)
		return;
	//hostViewer = viewer;

	osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
	if (!wsi)
	{
		return;
	}
	unsigned int width, height;
	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);

	// traits
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->x = 0;
	traits->y = 0;
	traits->width = width;
	traits->height = height;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	// graphics context
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits);
	if (!gc.valid())
	{
		return;
	}

	_camera = viewer->getCamera();//new osg::Camera;//
	//_camera->setGraphicsContext(gc);
	//_camera->setViewport(0, 0, width, height);
	auto eye = osg::Vec3(-10, 0, -10);
	auto center = osg::Vec3();
	auto up = osg::Vec3(0, 1, 0);
	auto manipulator = viewer->getCameraManipulator();
	_camera->setViewMatrix(osg::Matrix::lookAt(eye, center, up));
	//viewer->addSlave(_camera.get());
	//viewer->setCamera(_camera.get());
}
