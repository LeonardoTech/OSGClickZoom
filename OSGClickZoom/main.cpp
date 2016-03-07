#include "OSGEmpty.h"
#include "PickHandler.h"
#include "CameraController.h"
#include <osg/ComputeBoundsVisitor>

// zoom to original position
void zoomToOriginal(osgViewer::Viewer* viewer, osg::Vec3d position, osg::Vec3d center, osg::Vec3d up, const double* mat)
{
	viewer->getCameraManipulator()->setHomePosition(position, center, up);
	viewer->home();
	viewer->getCameraManipulator()->setByMatrix(osg::Matrix(mat));
}

void zoomTo(osgViewer::Viewer* viewer, osg::Node* node)
{
	double fov, aspectRatio, zNear, zFar;
	viewer->getCamera()->getProjectionMatrixAsPerspective(fov,
		aspectRatio, zNear, zFar);

	osg::ComputeBoundsVisitor* v = new osg::ComputeBoundsVisitor();
	v->apply(*node);
	auto box = v->getBoundingBox();
	auto xLength = box.xMax() - box.xMin();
	auto yLength = box.yMax() - box.yMin();
	float max;
	if (yLength >= xLength)
	{
		max = yLength;
	}
	else
	{
		max = xLength / aspectRatio;
	}
	auto tx = (max/2.0) / std::tan(fov / 2.0 / 180.0 * 3.14159265358979323846);
	auto center = box.center();
	osg::Vec3d position(center.x(), center.y(), center.z() + /*5 * bound.radius()*/tx);

	//osg::BoundingSphere bound = node->getBound();
	//osg::Vec3d center = bound.center();
	//osg::Vec3d position(center.x(), center.y(), center.z() + 5 * bound.radius());
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
	double mat[16];
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
			auto m = viewer.getCameraManipulator()->getMatrix().ptr();
			for (int i = 0; i < 16; i++)
				mat[i] = m[i];
			isFirstTimeZoom = false;
		}
		osg::DisplaySettings::instance()->setNumMultiSamples(16);
		//viewer.getDisplaySettings()->setNumMultiSamples(2);

		zoomTo(&viewer, parent);
	}, [&]() {
		if (isFirstTimeZoom)
			return;
		zoomToOriginal(&viewer, position, center, up, mat);
		isFirstTimeZoom = true;
	}));
	viewer.realize();
	viewer.run();
}