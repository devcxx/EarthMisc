#include "StatsHandler.h"
#include <limits>


/// Defines the sentinel value used by OSG for no-key mappings
static const int NO_KEY_MAPPING = -1;

StatsHandler::StatsHandler()
  : osgViewer::StatsHandler()
{
  setKeyEventPrintsOutStats(NO_KEY_MAPPING);
  setKeyEventTogglesOnScreenStats(NO_KEY_MAPPING);

  // Ignore events and pass them through
  getCamera()->setAllowEventFocus(false);
}

void StatsHandler::setStatsType(StatsHandler::StatsType statsType, osgViewer::View* onWhichView)
{
  if (onWhichView == NULL)
    return;

  // Due to the way osgViewer::StatsHandler is written, we must iterate through each
  // possible state until we get the one we want, else the display looks bad
  StatsHandler::StatsType validStats = validate_(statsType);
  while (_statsType != validStats)
    cycleStats(onWhichView);
}

StatsHandler::StatsType StatsHandler::statsType() const
{
  return static_cast<StatsHandler::StatsType>(_statsType);
}

StatsHandler::StatsType StatsHandler::validate_(StatsHandler::StatsType type) const
{
  switch (type)
  {
  case NO_STATS:
  case FRAME_RATE:
  case VIEWER_STATS:
  case CAMERA_SCENE_STATS:
  case VIEWER_SCENE_STATS:
    return type;
  case LAST:
    break;
  }
  // Invalid value, so show NO_STATS
  return NO_STATS;
}

void StatsHandler::cycleStats(osgViewer::View* onWhichView)
{
  if (!onWhichView)
    return;

  // Use a definitely-not-used key for simulated presses
  static const int FAKE_KEY = std::numeric_limits<int>::max();

  // Create a fake key event
  osg::ref_ptr<osgGA::GUIEventAdapter> ea = new osgGA::GUIEventAdapter;
  ea->setEventType(osgGA::GUIEventAdapter::KEYDOWN);
  ea->setKey(FAKE_KEY);

  // Fake a key press on an unused key
  int oldKey = getKeyEventTogglesOnScreenStats();
  setKeyEventTogglesOnScreenStats(FAKE_KEY);
  osgViewer::StatsHandler::handle(*ea, *onWhichView);
  // Restore the old key
  setKeyEventTogglesOnScreenStats(oldKey);
}

