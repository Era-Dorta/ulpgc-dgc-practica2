#include "testApp.h"

// Indexes for handle left arrow and right arrow key strokes (tested in
// Ubuntu).
const unsigned int KEY_LEFT_ARROW = 356;
const unsigned int KEY_RIGHT_ARROW = 358;


/***
    1. Initializations
***/

testApp::testApp( const unsigned int& w, const unsigned int& h, const unsigned int& guiW )
{
    // Kepp window dimensions.
    appW = w;
    appH = h;
    this->guiW = guiW;
}


void testApp::setup()
{
    //Creates the semaphore with permisions rw,r,r, and 0 tokens
    mutex = sem_open("mutexForServer", O_CREAT, 0644, 0);

    if(mutex == SEM_FAILED) {
      perror("testApp: error creating semaphore");
      return;
    }

    // Initialize last mouse position.
    lastMouseX = guiW+RENDER_WINDOW_BORDER;
    lastMouseY = RENDER_WINDOW_BORDER;

    // Initialize NXT server. The server thread will be waiting for new
    // polygons to send to the NXT.
    server = Server::getInstance();
    server->startThread( true, false ); // blocking, non verbose

    // Initialize currentPolygon iterator to the begin of the polygons container.
    currentPolygon = polygons.begin();

    //Initialise tempPolygon
    tempPolygon = (ofPtr<Polygon>)( new Polygon() );

    // Setup GUI panel.
    setupGUI();

    // Default app mode is polygon creation.
    appMode = MODE_POLYGON_CREATION;


    fractalCurrentRefVertex = 0;
}


void testApp::setupGUI()
{
    unsigned int i;
    const float widthGuiElement = (float)((guiW >> 1)-10);
    char str[10];
    vector<string> vnames;

    // Set the GUI canvas. It will take up a vertical space to the left.
    gui = new ofxUICanvas( 0, 0, guiW, appH );

    /***
    Add to the GUI a radio element for selecting the current app mode.
    ***/
    // Add a label to the radio options.
    gui->addLabel("APP MODE", OFX_UI_FONT_MEDIUM);

    gui->addSpacer();

    // Feed radio options from appModeStr array of strings.
    for( i=0; i<N_APP_MODES; i++ ){
        vnames.push_back( appModeStr[i] );
    }

    // Add the radio selector to the GUI and activate by default the option
    // "POLYGON CREATION".
    appModeSelector = gui->addRadio("App Mode Selector", vnames, OFX_UI_ORIENTATION_VERTICAL);
    appModeSelector->activateToggle( appModeStr[MODE_POLYGON_CREATION] );
    gui->addSpacer();

    /***
    Add to the GUI a radio element for selecting the current number
    of divisions.
    ***/
    // Add a label to the radio options.
    vnames.clear();
    gui->addLabel("FRACTAL DIVISIONS", OFX_UI_FONT_MEDIUM);
    gui->addSpacer();
    // Feed radio integer options.
    for( i=0; i<6; i++ ){
        sprintf( str, "%d", i );
        vnames.push_back( str );
    }

    // Add the radio selector to the GUI and activate by default the option
    // "2".
    fractalDivisionsSelector = gui->addRadio("Fractal Divisions Selector", vnames, OFX_UI_ORIENTATION_HORIZONTAL);
    fractalDivisionsSelector->activateToggle( "2" );
    gui->addSpacer();

    /***
    Add to the GUI a subpanel with buttons for deleting and selecting the
    current polygon.
    ***/
    gui->addLabel("POLYGON ADMINISTRATION", OFX_UI_FONT_MEDIUM);
    gui->addSpacer( OFX_UI_GLOBAL_SPACING_HEIGHT + 250 );
    deletingButton = gui->addLabelButton( "Delete current polygon", false );
    previousPolygonButton = gui->addLabelButton( "Previous poly", false, widthGuiElement );
    // Place the following button next to the last one.
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    nextPolygonButton = gui->addLabelButton( "Next poly", false, widthGuiElement );
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

    gui->addSpacer();

    /***
    Add a text input for typing a file and two buttons for saving to and
    loading from that file.
    ***/
    gui->addLabel( "FILE SAVING/LOADING", OFX_UI_FONT_MEDIUM );
    gui->addSpacer();
    fileInput = gui->addTextInput( "FILE_PATH", "data/foo.txt", OFX_UI_FONT_MEDIUM );

    savingButton = gui->addLabelButton( "Save", false, widthGuiElement );
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    loadingButton = gui->addLabelButton( "Load", false, widthGuiElement );
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

    // Add a invisible "File not found" error message. When user try to load from
    // a non-existing file, this label will turn visible.
    fileNotFoundLabel = gui->addLabel( "FILE NOT FOUND" );
    fileNotFoundLabel->setVisible( false );
    gui->addSpacer();

    /***
    Add to the GUI a panel with a button for sending the drawing to the NXT.
    Also include a label informing the user that red polygons can't be drawn
    by the robot.
    ***/
    gui->addLabel( "SERVER COMMUNICATION", OFX_UI_FONT_MEDIUM );
    gui->addSpacer();
    sendToServerButton = gui->addLabelButton( "Send drawing to NXT", false );
    gui->addLabel( "Note: red polys can't be drawn by robot", OFX_UI_FONT_SMALL );
    gui->addSpacer();

    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
}


/***
    2. Events
***/

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{
    if( !pointOnRenderWindow( x, y ) ){
        return;
    }

    currentMousePos.set( x, y );

    // Save mouse position for next frame.
    lastMouseX = x;
    lastMouseY = y;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    float aux;

    if( !pointOnRenderWindow( x, y ) || !polygons.size() ){
        return;
    }

    // Set one transformation or another according to current app mode
    switch( appMode ){
		case MODE_TRANSLATION:
		    (*currentPolygon)->translate( x-lastMouseX, -y+lastMouseY );
		break;
        case MODE_ROTATION:
            aux = x-lastMouseX;
            (*currentPolygon)->rotate( aux );
        break;
        case MODE_SCALE:
            aux = x-lastMouseX;

            aux = aux ? 1+aux*0.01 : 1;
            (*currentPolygon)->scale( aux, aux );
        break;
        default:
        break;
    }

    // Register last mouse location.
    lastMouseX = x;
    lastMouseY = y;
}


//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    bool exit = false;
    Fractal* tempFractal = NULL;

    if( (appMode != MODE_POLYGON_CREATION) && (appMode != MODE_FRACTAL_CREATION) ){
        return;
    }

    //Do not allow to paint more than 15 cm closer to the
    //edges, since the brick could fall off the board
    //Since canvas is world cm*4, then 15*4 = 60 pixels in canvas
    if( !pointOnRenderWindow( x, y ) ){
        return;
    }


    // Register last mouse location.
    lastMouseX = x;
    lastMouseY = y;

    Vertex position, vector, currentVertex, prevVertex;
    switch(button){
    case L_MOUSE:
        if( appMode == MODE_POLYGON_CREATION ){
            tempPolygon->addVertexFromPixel( x, y );
        }else{
            fractalRefVertexes[fractalCurrentRefVertex] = Polygon::pixelToWorld( x, y );
            if( fractalCurrentRefVertex == 1 ) {
                tempFractal = new Fractal( atoi( ((fractalDivisionsSelector->getActive())->getName()).c_str() ) );

                tempFractal->setVertices( fractalRefVertexes[0], fractalRefVertexes[1] );
                tempFractal->divide();

                addFractal( *tempFractal );

                delete tempFractal;

                fractalCurrentRefVertex = 0;
            }else{
                fractalCurrentRefVertex++;
            }
        }
    break;
    case R_MOUSE:
        if( appMode == MODE_POLYGON_CREATION ){
            addPolygon( tempPolygon );

            //tempPolygon.clear();
        }
        tempPolygon->clear();
    break;
    default:
    break;
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
    // Don't allow the user to resize the window.
    ofSetWindowShape( guiW+appW, appH );
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}


void testApp::guiEvent( ofxUIEventArgs &e )
{
    PolygonsFile polygonsFile;
    int newFractalDivisions;
    Fractal* currentFractal = NULL;

    if( e.widget->getName() == savingButton->getName() ){
        if( savingButton->getValue() ){
            // Button is pressed
            cout << "Saving to file [" << fileInput->getTextString() << "]" << endl;

            if( !polygonsFile.save( fileInput->getTextString(), &polygons ) ){
                fileNotFoundLabel->setVisible( false );
            }else{
                fileNotFoundLabel->setVisible( true );
            }
        }
    }else if( e.widget->getName() == loadingButton->getName() ){
        if( loadingButton->getValue() ){
            // Button is pressed
            cout << "Loading from file [" << fileInput->getTextString() << "]" << endl;
            if( !polygonsFile.load( fileInput->getTextString(), &polygons ) ){
                currentPolygon = polygons.begin();
                //Copy loaded poligons to send to server
                for(unsigned int i = 0; i < polygons.size(); i++){
                    toServerPolygons.push_back( polygons[i] );
                }

                //If current is Fractal show appropiate nº of divisions on
                //the user interface
                if( (polygons.size() > 0) && (*currentPolygon)->getType() == FRACTAL ){
                    newFractalDivisions = atoi( ((fractalDivisionsSelector->getActive())->getName()).c_str() );
                    currentFractal = currentFractal = (dynamic_cast<Fractal *>((*currentPolygon).get()));
                    char str[10];

                    if( newFractalDivisions != currentFractal->getDivisions() ){
                        sprintf( str, "%d", currentFractal->getDivisions() );
                        fractalDivisionsSelector->activateToggle( str );
                    }
                }

                fileNotFoundLabel->setVisible( false );
            }else{
                fileNotFoundLabel->setVisible( true );
            }
        }
    }else if( e.widget->getName() == deletingButton->getName() ){
        if( deletingButton->getValue() ){
            // Button is pressed
            deleteCurrentPolygon();
        }
    }else if( e.widget->getName() == sendToServerButton->getName() ){
        if( sendToServerButton->getValue() ){
            // Button is pressed
            cout << "Sending drawing to server" << endl;
            sendToServer();
        }
    }else if( e.widget->getName() == previousPolygonButton->getName() ){
        if( previousPolygonButton->getValue() ){
            // Button is pressed
            selectPreviousPolygon();
        }
    }else if( e.widget->getName() == nextPolygonButton->getName() ){
        if( nextPolygonButton->getValue() ){
            // Button is pressed
            selectNextPolygon();
        }
    }else if( e.widget->getParent()->getName() == fractalDivisionsSelector->getName() ){
        // User has selected a number of divisions in the "Fractal Divisions"
        // selector. We check if current polygon is a fractal and if the
        // new number of divisions selected by user is different from that of
        // the current polygon.
        if( (polygons.size() > 0) && (*currentPolygon)->getType() == FRACTAL ){
            newFractalDivisions = atoi( ((fractalDivisionsSelector->getActive())->getName()).c_str() );
            currentFractal = (dynamic_cast<Fractal *>((*currentPolygon).get()));

            if( newFractalDivisions != currentFractal->getDivisions() ){
                currentFractal->setDivisions( atoi( ((fractalDivisionsSelector->getActive())->getName()).c_str() ) );
                currentFractal->divide();
            }
        }
    }
}


/***
    3. Updating and drawing
***/

void testApp::update(){
    int w, h;

    ofxUIToggle* appModeSelection = appModeSelector->getActive();
    for( unsigned int i=0; i<N_APP_MODES; i++ ){
        if( !appModeSelection->getName().compare( appModeStr[i] ) && ( appMode != (AppMode)i ) ){
            appMode = (AppMode)i;

            // Whether user has changed from/to fractal creation mode or not,
            // we take the opportunity to reset this for fractal creation.
            fractalCurrentRefVertex = 0;
        }
    }
}


void testApp::draw()
{
    drawEdges();


    std::vector< ofPtr<Polygon> >::iterator it = polygons.begin();

    for( ; it != polygons.end(); ++it ){
        if( it == currentPolygon ){
            if( (*it)->drawableByRobot() ){
                ofSetColor( ofColor::white );
            }else{
                ofSetColor( ofColor::red );
            }
        }else{
            if( (*it)->drawableByRobot() ){
                ofSetColor( 150, 150, 150 );
            }else{
                ofSetColor( 150, 0, 0 );
            }
        }
        (*it)->draw();
        ofSetColor( ofColor::white );
    }

    tempPolygon->draw();
    Vertex origin = Polygon::getOrigin();

    //Vertex currentMouseWorldPos( currentMousePos[X]-origin[X], -currentMousePos[Y]+origin[Y]  );
    Vertex currentMouseWorldPos( lastMouseX-origin[X], -lastMouseY+origin[Y]  );

    if( (appMode == MODE_POLYGON_CREATION) && tempPolygon->getSize() ){
        Polygon::drawLine( tempPolygon->getLastVertex(), currentMouseWorldPos );
    }

    if( ( appMode == MODE_FRACTAL_CREATION ) && (fractalCurrentRefVertex == 1) ){
        Polygon::drawLine( fractalRefVertexes[0],
                           currentMouseWorldPos );
    }

    server->drawBrick();
}


void testApp::drawEdges()
{
    ofSetColor(ofColor::black);
    ofFill();
    //Draw a rectangle that shows the user drawable area
    //Top
    ofRect( guiW, 0, guiW+appW-RENDER_WINDOW_BORDER, RENDER_WINDOW_BORDER );
    //Right
    ofRect( guiW+appW-RENDER_WINDOW_BORDER, RENDER_WINDOW_BORDER, RENDER_WINDOW_BORDER, appH-RENDER_WINDOW_BORDER );
    //Bottom
    //ofRect(0, 410, 602, 60);
    ofRect( guiW, appH-RENDER_WINDOW_BORDER, guiW+appW-RENDER_WINDOW_BORDER, RENDER_WINDOW_BORDER );
    //Left
    ofRect( guiW, RENDER_WINDOW_BORDER, RENDER_WINDOW_BORDER, appH-(RENDER_WINDOW_BORDER<<1) );

    ofSetColor( ofColor::white );
}


/***
    4. Exit
***/

void testApp::exit()
{
    server->exit();
    server->stopThread();
    sem_close(mutex);
    sem_unlink("mutexForServer");

    delete gui;
}

//--------------------------------------------------------------



/***
    5. Auxiliar methods
***/

bool testApp::pointOnRenderWindow( const int& x, const int& y )
{
    return ( x >= guiW+RENDER_WINDOW_BORDER ) &&
            ( x <= guiW+appW-RENDER_WINDOW_BORDER ) &&
            ( y >= RENDER_WINDOW_BORDER ) &&
            ( y <= appH-RENDER_WINDOW_BORDER );
}


/***
    6. Polygons administration
***/


void testApp::addPolygon( Polygon polygon )
{
    ofPtr<Polygon> polygonPtr( new Polygon(polygon) );
    currentPolygon = polygons.insert( polygons.end(), polygonPtr );
    toServerPolygons.push_back(polygons.back());
}


void testApp::addPolygon( ofPtr<Polygon> polygon ){
    Polygon p = (Polygon)(*polygon);
    ofPtr<Polygon> polygonPtr( new Polygon(p) );
    currentPolygon = polygons.insert( polygons.end(), polygonPtr );
    toServerPolygons.push_back(polygons.back());
}

void testApp::addFractal( Fractal fractal )
{
    ofPtr<Polygon> polygonPtr(dynamic_cast<Polygon *>( new Fractal( fractal ) ) );
    currentPolygon = polygons.insert( polygons.end(), polygonPtr );
    toServerPolygons.push_back(polygons.back());
}

void testApp::deleteLastPolygon()
{
    polygons.pop_back();
    // Maybe we should not do this
    toServerPolygons.pop_back();

    currentPolygon = polygons.begin();
}

void testApp::deleteCurrentPolygon()
{
    if( polygons.size() ){

        //If polygon is in toServerPolygons delete it also
        std::vector< ofPtr<Polygon> >::iterator aux;
        for(aux = toServerPolygons.begin(); aux < toServerPolygons.end(); aux++ ){
            if(*aux == *currentPolygon){
                toServerPolygons.erase( aux );
            }
        }
        currentPolygon = polygons.erase( currentPolygon );


        // If deleted polygon was the last of the vector, currentPolygon will
        // point to invalid position "polygons.end()". Change it to
        // polygons.begin().
        if( currentPolygon == polygons.end() ){
            currentPolygon = polygons.begin();
        }
    }
}


void testApp::selectPreviousPolygon()
{
    if( currentPolygon == polygons.begin() ){
        currentPolygon = polygons.end();
    }
    currentPolygon--;

    //If current is Fractal show appropiate nº of divisions on the UI
    if( (polygons.size() > 0) && (*currentPolygon)->getType() == FRACTAL ){
        int newFractalDivisions = atoi( ((fractalDivisionsSelector->getActive())->getName()).c_str() );
        Fractal* currentFractal = currentFractal = (dynamic_cast<Fractal *>((*currentPolygon).get()));
        char str[10];

        if( newFractalDivisions != currentFractal->getDivisions() ){
            sprintf( str, "%d", currentFractal->getDivisions() );
            fractalDivisionsSelector->activateToggle( str );
        }
    }
}


void testApp::selectNextPolygon()
{
    currentPolygon++;
    if( currentPolygon == polygons.end() ){
        currentPolygon = polygons.begin();
    }

    //If current is Fractal show appropiate nº of divisions on the UI
    if( (polygons.size() > 0) && (*currentPolygon)->getType() == FRACTAL ){
        int newFractalDivisions = atoi( ((fractalDivisionsSelector->getActive())->getName()).c_str() );
        Fractal* currentFractal = currentFractal = (dynamic_cast<Fractal *>((*currentPolygon).get()));
        char str[10];

        if( newFractalDivisions != currentFractal->getDivisions() ){
            sprintf( str, "%d", currentFractal->getDivisions() );
            fractalDivisionsSelector->activateToggle( str );
        }
    }
}


/***
    7. Server communication
***/

void testApp::sendToServer()
{
    // If there are polygons to copy to the server, copy them.
    if(toServerPolygons.size() > 0){
        if(server->lock()){
            cout << "Copying polygons to server\n";
            for( unsigned int i = 0; i < toServerPolygons.size(); i++ ){
                server->polygons.push_back(toServerPolygons[i]);
                //Increment semaphore one token for each polygon copied
                release(mutex);
            }
            server->unlock();
            toServerPolygons.clear();
        }
    }
}


void testApp::release( sem_t* mutex_)
{
    if(sem_post(mutex) < 0) {
      perror("main thread: error on post semaphore");
      _Exit(EXIT_FAILURE);
    }
}
