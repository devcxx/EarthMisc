
#ifndef STATSHANDLER_H
#define STATSHANDLER_H

#include <osg/observer_ptr>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/View>

/**
 * Specialization of the osgViewer::StatsHandler that allows for easy programmatic
 * changes to the currently displayed statistics.  Note that the default hotkeys
 * for the osgViewer::StatsHandler ('s' and 'S') are not respected unless explicitly
 * set by the user.
 */
class StatsHandler : public osgViewer::StatsHandler
{
public:
    /** Typedef the base class StatsType for ease of use */
    typedef osgViewer::StatsHandler::StatsType StatsType;

    /**
   * Instantiate a new StatsHandler.  This instance should be associated with any view
   * or viewer using the addEventHandler() call, otherwise window resize events will
   * not be observed.
   */
    StatsHandler();

    /**
   * Programmatically alter the stats type shown.  This is equivalent to pressing the
   * toggling hotkey specified in setKeyeventTogglesOnScreenStats().
   * @param statsType Statistics to show on the main view
   * @param onWhichView View with which to associate the stats
   */
    void setStatsType(StatsType statsType, osgViewer::View* onWhichView);

    /** Cycles to the next stats type for the given view. */
    void cycleStats(osgViewer::View* onWhichView);

    /** Retrieves the currently displayed statistics. */
    StatsType statsType() const;


private:
    /** Safely bounds the enum to [0,LAST) */
    StatsType validate_(StatsType type) const;
};


#endif /* STATSHANDLER_H */
