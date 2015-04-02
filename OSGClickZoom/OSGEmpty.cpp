
#include "OSGEmpty.h"
#ifdef _DEBUG
#pragma comment(lib, "osgViewerd.lib")
#pragma comment(lib, "osgDBd.lib")
#pragma comment(lib, "OpenThreadsd.lib")
#pragma comment(lib, "osgd.lib")
#pragma comment(lib, "osgGAd.lib")
#else
#pragma comment(lib, "osgViewer.lib")
#pragma comment(lib, "osgDB.lib")
#pragma comment(lib, "OpenThreads.lib")
#pragma comment(lib, "osg.lib")
#pragma comment(lib, "osgGA.lib")
#endif
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/ShapeDrawable>

osg::ref_ptr<osg::Node> createQuad()
{
	osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
	hints->setDetailRatio(0.5);

	auto box1 = new osg::Box(osg::Vec3(2.0, 0.0, 0.0), 1.0, 1.0, 1.0);
	auto box2 = new osg::Box(osg::Vec3(0.0, 2.0, -2.0), 1.0, 1.0, 1.0);
	auto box3 = new osg::Box(osg::Vec3(-2.0, 0.0, 0.0), 1.0, 1.0, 1.0);

	osg::ref_ptr<osg::ShapeDrawable> shape1 = new osg::ShapeDrawable(box1, hints.get());
	shape1->setColor(osg::Vec4(1, 0.2, 0.2, 1.0));
	
	osg::ref_ptr<osg::ShapeDrawable> shape2 = new osg::ShapeDrawable(box2, hints.get());
	shape2->setColor(osg::Vec4(0.2, 1, 0.2, 1.0));

	osg::ref_ptr<osg::ShapeDrawable> shape3 = new osg::ShapeDrawable(box3, hints.get());
	shape3->setColor(osg::Vec4(0.2, 0.2, 1, 1.0));


	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(shape1.get());
	geode->addDrawable(shape2.get());
	geode->addDrawable(shape3.get());
	return geode.get();
}

void main()
{
	osgViewer::Viewer viewer;
	osg::Group* root = new osg::Group();
	root->addChild(createQuad().get());
	viewer.setSceneData(root);
	viewer.realize();
	viewer.run();
}