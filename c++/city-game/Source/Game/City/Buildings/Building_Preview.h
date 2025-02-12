#ifndef BUILDING_PREVIEW_H
#define BUILDING_PREVIEW_H

#include <SFML/Graphics.hpp>
#include <string>

#include "../../GUI/GUI.h"
#include "../../Util/Window.h"

class Building_Data;

class Builder;

// Tells the GUI how to act depending on its state
enum class PR_STATE
    {
        SHOW,           // Used for transition effects
        HIDE            // Only two, might add more though
    };                  // TODO: Changing effects?

class Building_Preview
{
    public:
        Building_Preview( const Building_Data* data ); // ctor


        void update (); // Update all the GUIs :D
        void draw   ();

        void changeState        ( PR_STATE state); // Change state
        void setBuildingName    ( const std::string& str );

    private:
        void resetImagePreview();

        GUI     m_mainFrame;        // All the GUIs in the preview
        GUI     m_nameLabel;

        PR_STATE state;

        float   relative_xpos;

        const Building_Data* m_buildingData;
};

#endif // BUILDING_PREVIEW_H
