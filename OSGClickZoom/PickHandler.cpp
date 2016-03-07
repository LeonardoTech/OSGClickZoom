#include "PickHandler.h"



PickHandler::PickHandler(osgViewer::Viewer* viewer, std::function<void(osg::ShapeDrawable*, osg::Node*)> select_one, std::function<void()> select_empty) :
hostViewer(viewer),
lastPicked(nullptr),
_select_one_call_back(select_one),
_select_empty_call_back(select_empty)
{
}


PickHandler::~PickHandler()
{
}

void createBox(float x, float y, osgViewer::Viewer* viewer)
{
	auto camera = viewer->getCamera();
	osg::Matrix MVPW(camera->getViewMatrix() *
		camera->getProjectionMatrix() *
		camera->getViewport()->computeWindowMatrix());

	osg::Matrixd inverseMVPW = osg::Matrixd::inverse(MVPW);
	auto center = osg::Vec3(x, y, 0) * inverseMVPW;
	auto box = new osg::ShapeDrawable(new osg::Box(center, 1));
	auto group = new osg::Geode();
	group->addDrawable(box);
	viewer->getSceneData()->asGroup()->addChild(group);
}

bool PickHandler::handle(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa)
{
	switch (ea.getEventType())
	{
	case osgGA::GUIEventAdapter::PUSH:
		if (ea.getButton() == 1)
		{
			Pick(ea.getX(), ea.getY());
			//createBox(ea.getX(), ea.getY(), static_cast<osgViewer::Viewer*>(&aa));
		}
		return true;
	default:
		break;
	}
	return false;
}

void PickHandler::Pick(float x, float y)
{
	if (lastPicked)
	{
		lastPicked->setColor(lastPicked->getColor() / 3);
	}
	osgUtil::LineSegmentIntersector::Intersections intersections;
	if (hostViewer->computeIntersections(x, y, intersections))
	{
		for (auto hitr = intersections.begin(); hitr != intersections.end(); ++hitr)
		{
			if (!hitr->nodePath.empty() && !(hitr->nodePath.back()->getName().empty()))
			{
				const osg::NodePath& np = hitr->nodePath;
				for (int i = np.size() - 1; i >= 0; i--)
				{
					if (hitr->nodePath.back()->getName() != np[i]->getName())
						continue;
					auto transform = dynamic_cast<osg::MatrixTransform*>(np[i]);
					if (!transform)
						continue;
					auto geode = dynamic_cast<osg::Geode*>(transform->getChild(0));
					if (!geode)
						continue;
					auto list = geode->getDrawableList();
					auto len = list.size();
					for (size_t i = 0; i < len; i++)
					{
						auto shape = dynamic_cast<osg::ShapeDrawable*>(list[i].get());
						if (!shape)
							continue;
						// high light selected
						_select_one_call_back(shape, transform);
						lastPicked = shape;
						return;
						//osg::StateSet* state = node->getOrCreateStateSet();
						//state->setMode(GL_BLEND, osg::StateAttribute::ON);
						//osg::Material* mtrl = dynamic_cast<osg::Material*>(state->getAttribute(osg::StateAttribute::MATERIAL));
						//if (!mtrl)
						//{
						//	mtrl = new osg::Material;
						//}
						//mtrl->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 0.0, 0.0, 0.8));
						//mtrl->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 0.0, 0.0, 0.8));
						//mtrl->setTransparency(osg::Material::FRONT_AND_BACK, 0.2);
						//state->setAttributeAndModes(mtrl, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
						//state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
					}
				}
			}
		}
	}
	lastPicked = nullptr;
	_select_empty_call_back();
}
