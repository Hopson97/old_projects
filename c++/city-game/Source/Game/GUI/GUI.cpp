#include "GUI.h"

#include "../Util/Mouse.h"
#include "../Util/Window.h"
#include "../Managers/Resource_Identifier.h"

#include "../Game.h"

#include "Image.h"

GUI :: GUI( const sf::Vector2f& size, const sf::Vector2f& position)
    :   m_background   (  { size.x, size.y + m_topBarHeight + 2 } )
    ,   m_resizeButton ( { 20, m_topBarHeight },
                         { 2, 2 },
                          position,
                          Game::getTexture( Texture_Name::GUI_Resize_Button),
                          std::bind( &GUI::reSize, this ) )
{
    m_background.setPosition( position );
    m_background.setOutlineThickness( 3 );
    m_background.setOutlineColor( sf::Color::Black );

    m_title.setPosition ( position.x + 25, position.y - 4 );
    m_title.setCharacterSize( 15 );
    m_title.setFont( Game::getFont( Font_Name::Rs ) );
}


void GUI :: setTexture(const sf::Texture& texture)
{
    m_background.setTexture( &texture );
}


void GUI :: setBgColour ( const sf::Color& colour )
{
    m_background    .setFillColor( colour );
}

void GUI :: add( std::unique_ptr<GUI_Feature> feature )
{
    m_features.push_back ( std::move ( feature ) );
}

//Add a button
void GUI :: addButton( const sf::Vector2f& size,
                       const sf::Texture&  texture,
                       std::function<void(void)> callback )
{
    if ( !stylersActive() ) {
        return;
    }
    auto position = m_activePositioner->getPosition( size.y );

    m_features.push_back( std::make_unique<Button>(size,
                                                   position,
                                                   getGUIOffset(),
                                                   texture,
                                                   callback  ) );
}


//Add an update label
void GUI :: addSymbolUpdateLabel( const sf::Vector2f& size,
                                  const sf::Texture& symbol,
                                  const int& value,
                                  const std::string& toolTip )
{
    if ( !stylersActive() ) {
        return;
    }
    auto position = m_activePositioner->getPosition( size.y );

    m_features.push_back( std::make_unique<Symbolled_Update_Label>
                        ( size,
                          position,
                          getGUIOffset(),
                          symbol,
                          value,
                          toolTip  ) );
}

void GUI :: addImage   ( const sf::Vector2f& size,
                         const sf::Vector2f& position,
                         const sf::Texture& image )
{
    m_features.push_back( std::make_unique<Image>
                        ( size,
                          position,
                          getGUIOffset(),
                          image ) );
}


void GUI :: setResizeable ( bool canResize )
{
    m_isResizable = canResize;
}

void GUI :: setTitle ( const std::string& title )
{
    m_titleSet = true;
    m_title.setString( title );
}

void GUI :: update()
{
    if ( m_isResizable ) {
        m_resizeButton.update();
    }

    for ( auto& feature : m_features ) {
        feature->update();
    }
}

void GUI :: draw()
{
    //This is basically for if the GUI has been "resized", meaning it is hidden.
    if ( !m_isHidden ) {
        Window::draw( m_background );

        for ( auto& feature : m_features ) {
            feature->draw();
        }
    }

    if ( m_isResizable ) {
        m_resizeButton.draw();
    }

    if ( m_titleSet ) {
        Window::draw( m_title );
    }
}

void GUI::beginColumn ( const sf::Vector2f& beginPosition, int padding )
{
    m_column = Column ( beginPosition, padding );
    m_column.active = true;
    m_row.active    = false;

    m_activePositioner = &m_column;
}

void GUI :: beginRow ( const sf::Vector2f& beginPosition, int padding )
{
    m_row = Row ( beginPosition, padding );
    m_row.active    = true;
    m_column.active = false;

    m_activePositioner = &m_row;
}

bool GUI :: stylersActive  () const
{
    return m_row.active || m_column.active;
}

sf::Vector2f GUI::getGUIOffset() const
{
    return { m_background.getPosition().x,
             m_background.getPosition().y + m_topBarHeight + 5 };
}


void GUI :: reSize()
{
    m_isHidden = !m_isHidden;
}


void GUI :: changeOutlineThickness(int thickness)
{
    m_background.setOutlineThickness(thickness);
}

void GUI :: changeOutlineColour(sf::Color color)
{
    m_background.setOutlineColor(color);
}
