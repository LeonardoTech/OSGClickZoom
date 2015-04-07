#include "OSGEmpty.h"
#include "PickHandler.h"
#include "CameraController.h"

// zoom to original position
void zoomToOriginal(osgViewer::Viewer* viewer, osg::Vec3d position, osg::Vec3d center, osg::Vec3d up)
{
	viewer->getCameraManipulator()->setHomePosition(position, center, up);
	viewer->home();
}

void zoomTo(osgViewer::Viewer* viewer, osg::Node* node)
{
	osg::BoundingSphere bound = node->getBound();
	osg::Vec3d center = bound.center();
	osg::Vec3d position(center.x(), center.y(), center.z() + 5 * bound.radius());
	do
	{
		auto mt = dynamic_cast<osg::MatrixTransform*>(node);
		if (mt != NULL) {
			center = mt->getMatrix().preMult(center);
			position = mt->getMatrix().preMult(position);
		}
	} while (node->getNumParents() == 1 && (node = node->getParent(0)));
	viewer->getCameraManipulator()->setHomePosition(position, center, osg::Vec3d(0, 1, 0), false);
	viewer->home();
}

void main()
{
	// initial camera position.
	osg::Vec3d position, center, up;
	// when first time zooming the camera, record the initial position.
	bool isFirstTimeZoom = true;

	osgViewer::Viewer viewer;
	auto root = createBoxes();
	//osg::ref_ptr<osg::Multisample> multisample = new osg::Multisample();
	//root->getStateSet()->setAttribute(multisample);
	//osg::DisplaySettings::instance()->setNumMultiSamples(2);
	viewer.setSceneData(root.get());
	viewer.addEventHandler(new PickHandler(&viewer, [&](osg::ShapeDrawable* shape, osg::Node* parent) {
		shape->setColor(shape->getColor() * 3);
		if (isFirstTimeZoom)
		{
			viewer.getCameraManipulator()->getHomePosition(position, center, up);
			isFirstTimeZoom = false;
		}
		osg::DisplaySettings::instance()->setNumMultiSamples(16);
		//viewer.getDisplaySettings()->setNumMultiSamples(2);

		zoomTo(&viewer, parent);
	}, [&]() {
		if (isFirstTimeZoom)
			return;
		zoomToOriginal(&viewer, position, center, up);
	}));
	viewer.realize();
	viewer.run();
}