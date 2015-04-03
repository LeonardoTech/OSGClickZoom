#pragma once
#include "OSGCommon.h"

class PickHandler :public osgGA::GUIEventHandler
{
public:
	PickHandler(osgViewer::Viewer* viewer, std::function<void(osg::ShapeDrawable* selected, osg::Node* parent)> select_one, std::function<void()> select_empty);
	~PickHandler();

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

protected:
	osgViewer::Viewer* hostViewer;
	void Pick(float x, float y);
	osg::ref_ptr<osg::ShapeDrawable> lastPicked;
	std::function<void(osg::ShapeDrawable*, osg::Node*)> _select_one_call_back;
	std::function<void()> _select_empty_call_back;
};

