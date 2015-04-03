
#include "OSGEmpty.h"


osg::ref_ptr<osg::Node> createBoxes()
{
	osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
	hints->setDetailRatio(0.5);

	auto box1 = new osg::Box(osg::Vec3(2.0, 0.0, 0.0), 1.0, 1.0, 1.0);
	auto box2 = new osg::Box(osg::Vec3(0.0, 2.0, -2.0), 1.0, 1.0, 1.0);
	auto box3 = new osg::Box(osg::Vec3(-2.0, 0.0, 0.0), 1.0, 1.0, 1.0);

	osg::ref_ptr<osg::ShapeDrawable> shape1 = new osg::ShapeDrawable(box1, hints.get());
	shape1->setColor(osg::Vec4(1, 0.2, 0.2, 1.0));
	shape1->setName("first");

	osg::ref_ptr<osg::ShapeDrawable> shape2 = new osg::ShapeDrawable(box2, hints.get());
	shape2->setColor(osg::Vec4(0.2, 1, 0.2, 1.0));
	shape2->setName("second");

	osg::ref_ptr<osg::ShapeDrawable> shape3 = new osg::ShapeDrawable(box3, hints.get());
	shape3->setColor(osg::Vec4(0.2, 0.2, 1, 1.0));
	shape3->setName("third");

	osg::ref_ptr<osg::Geode> geode1 = new osg::Geode;
	geode1->addDrawable(shape1.get());
	geode1->setName("first");

	osg::ref_ptr<osg::Geode> geode2 = new osg::Geode;
	geode2->addDrawable(shape2.get());
	geode2->setName("second");

	osg::ref_ptr<osg::Geode> geode3 = new osg::Geode;
	geode3->addDrawable(shape3.get());
	geode3->setName("third");


	osg::ref_ptr<osg::MatrixTransform> trans1 = new osg::MatrixTransform;
	trans1->addChild(geode1);
	trans1->setName("first");

	osg::ref_ptr<osg::MatrixTransform> trans2 = new osg::MatrixTransform;
	trans2->addChild(geode2);
	trans2->setName("second");

	osg::ref_ptr<osg::MatrixTransform> trans3 = new osg::MatrixTransform;
	trans3->addChild(geode3);
	trans3->setName("third");

	osg::ref_ptr<osg::Group> group = new osg::Group;
	group->addChild(trans1);
	group->addChild(trans2);
	group->addChild(trans3);
	return group.get();
}