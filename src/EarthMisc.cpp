/* -*-c++-*- */
/* osgEarth - Dynamic map generation toolkit for OpenSceneGraph
* Copyright 2016 Pelican Mapping
* http://osgearth.org
*
* osgEarth is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include <osgViewer/Viewer>
#include <osgEarth/Notify>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>
#include <osgEarth/MapNode>
#include <osgEarth/ThreadingUtils>
#include <osgEarth/Metrics>
#include <iostream>
#include <osgEarthUtil/Controls>

#include "ScaleBar.h"
#include "OverviewMap.h"
#include "Compass.h"
#include "StatsHandler.h"

#define LC "[viewer] "

using namespace osgEarth;
using namespace osgEarth::Util;
namespace ui = osgEarth::Util::Controls;

osg::ref_ptr<ScaleBar> g_scaleBar;  // MapScaleBar
osg::ref_ptr<OverviewMapControl> g_overviewMap; // OverviewMap
osg::ref_ptr<ui::ControlCanvas> g_controlCanvas;
osg::ref_ptr<Compass> g_compass;
osg::ref_ptr<StatsHandler> g_statsHandler; // StatsHandler

int
usage(const char* name)
{
    OE_NOTICE 
        << "\nUsage: " << name << " file.earth" << std::endl
        << MapNodeHelper().usage() << std::endl;

    return 0;
}

void createScaleBar(osgEarth::MapNode* mapNode, osgViewer::View* view)
{
    g_scaleBar = new ScaleBar(mapNode, view);
    osgEarth::Util::Controls::HBox* scaleBox
        = new osgEarth::Util::Controls::HBox(osgEarth::Util::Controls::Control::ALIGN_CENTER,
            osgEarth::Util::Controls::Control::ALIGN_BOTTOM,
            osgEarth::Util::Controls::Gutter(2, 2, 2, 2), 2.0f);
    scaleBox->addControl(g_scaleBar->_scaleLabel.get());
    scaleBox->addControl(g_scaleBar->_scaleBar.get());
    scaleBox->setVertFill(true);
    scaleBox->setForeColor(osg::Vec4f(0, 0, 0, 0.8));
    scaleBox->setBackColor(osg::Vec4f(1, 1, 1, 0.5));

    g_controlCanvas->addControl(scaleBox);
    view->addEventHandler(new ScaleBarHandler(g_scaleBar));
}


void createOverviewMap(osgEarth::MapNode* mapNode, osgViewer::View* view)
{
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile("world.jpg");
    if (image.valid()) {
        g_overviewMap = new OverviewMapControl(image);
        g_overviewMap->setWidth(200);
        g_overviewMap->setHeight(100);
        g_controlCanvas->addControl(g_overviewMap.get());
        view->addEventHandler(new OverviewMapHandler(g_overviewMap, dynamic_cast< osgEarth::Util::EarthManipulator*>(view->getCameraManipulator())));
    }
}

void createCopass(osgViewer::View* view)
{
    // create a compass image control, add it to the HUD/Overlay
    g_compass = new Compass("compass.png", g_controlCanvas);
    g_compass->setDrawView(view);

}

void createFrameRate(osgViewer::View* view)
{
    g_statsHandler = new StatsHandler;
    g_statsHandler->setKeyEventTogglesOnScreenStats(osgGA::GUIEventAdapter::KEY_S);
    view->addEventHandler(g_statsHandler);
    // Pick the StatsType based on turnOn flag
    StatsHandler::StatsType type = StatsHandler::FRAME_RATE;
    // Update the stats type in the handler
    g_statsHandler->setStatsType(type, view);
}

int
main(int argc, char** argv)
{
    osg::ArgumentParser arguments(&argc,argv);

    // help?
    if ( arguments.read("--help") )
        return usage(argv[0]);

    float vfov = -1.0f;
    arguments.read("--vfov", vfov);

    

    // create a viewer:
    osgViewer::Viewer viewer(arguments);

    // Tell the database pager to not modify the unref settings
    viewer.getDatabasePager()->setUnrefImageDataAfterApplyPolicy( true, false );

    // thread-safe initialization of the OSG wrapper manager. Calling this here
    // prevents the "unsupported wrapper" messages from OSG
    osgDB::Registry::instance()->getObjectWrapperManager()->findWrapper("osg::Image");

    // install our default manipulator (do this before calling load)
    viewer.setCameraManipulator( new EarthManipulator(arguments) );

    // disable the small-feature culling
    viewer.getCamera()->setSmallFeatureCullingPixelSize(-1.0f);

    // set a near/far ratio that is smaller than the default. This allows us to get
    // closer to the ground without near clipping. If you need more, use --logdepth
    viewer.getCamera()->setNearFarRatio(0.0001);

    if ( vfov > 0.0 )
    {
        double fov, ar, n, f;
        viewer.getCamera()->getProjectionMatrixAsPerspective(fov, ar, n, f);
        viewer.getCamera()->setProjectionMatrixAsPerspective(vfov, ar, n, f);
    }

    // load an earth file, and support all or our example command-line options
    // and earth file <external> tags    
    osg::Node* node = MapNodeHelper().load(arguments, &viewer);
    if ( node )
    {
        viewer.setSceneData( node );


        // install a control canvas for UI elements
        g_controlCanvas = new ui::ControlCanvas();
        node->asGroup()->addChild(g_controlCanvas);

        createScaleBar(MapNode::get(node), &viewer);
        createOverviewMap(MapNode::get(node), &viewer);
        createCopass(&viewer);
        createFrameRate(&viewer);

        Metrics::run(viewer);
    }
    else
    {
        return usage(argv[0]);
    }

    return 0;
}
