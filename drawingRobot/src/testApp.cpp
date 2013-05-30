#include "testApp.h"
using namespace std;

// Indexes for handle left arrow and right arrow key strokes (tested in
// Ubuntu).
const unsigned int KEY_LEFT_ARROW = 356;
const unsigned int KEY_RIGHT_ARROW = 358;


// Do not allow to paint more than 15 cm closer to the edges, since the brick
// could fall off the board.
// Since canvas is world cm*4, then 15*4 = 60 pixels in canvas
const unsigned int RENDER_WINDOW_BORDER = 60;


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

    // Load polygons from file.
    //PolygonsFile polygonsFile;
    //polygonsFile.load( "data/foo.txt", &polygons ); // TODO: Copiar tambien en toServerPolygons.

    // Initialize NXT server. The server thread will be waiting for new
    // polygons to send to the NXT.
    server = Server::getInstance();
    server->startThread( true, false ); // blocking, non verbose

    // Initialize currentPolygon iterator to the begin of the polygons container.
    currentPolygon = polygons.begin();

    // Setup GUI panel.
    setupGUI();

    // Default app mode is polygon creation.
    appMode = MODE_POLYGON_CREATION;
}

void testApp::setupGUI()
{
    unsigned int i;

    // Set the GUI canvas. It will take up a vertical space to the left.
    gui = new ofxUICanvas( 0, 0, guiW, appH );

    // Create a radio element for selecting the current app mode.

    // Add a label to the radio options.
    gui->addLabel("APP MODE", OFX_UI_FONT_MEDIUM);

    gui->addSpacer();
    vector<string> vnames;
    for( i=0; i<N_APP_MODES; i++ ){
        // Feed radio options from appModeStr array of strings.
        vnames.push_back( appModeStr[i] );
    }

    // Add the radio selector to the GUI and activate by default the option
    // "POLYGON CREATION".
    appModeSelector = gui->addRadio("VR", vnames, OFX_UI_ORIENTATION_VERTICAL);
    appModeSelector->activateToggle( appModeStr[MODE_POLYGON_CREATION] );
    gui->addSpacer();

    // Add a text input for typing a file and two buttons for saving to and
    // loading from that file.
    gui->addLabel( "Saving/Loading", OFX_UI_FONT_MEDIUM );
    gui->addSpacer();
    fileInput = gui->addTextInput( "FILE_PATH", "data/foo.txt", OFX_UI_FONT_MEDIUM );
    savingButton = gui->addLabelButton( "Save to file", false );
    loadingButton = gui->addLabelButton( "Load from file", false );

    fileNotFoundLabel = gui->addLabel( "FILE NOT FOUND" );
    fileNotFoundLabel->setVisible( false );
    //gui->addWidgetDown( new ofxUIButton( "Saving button", false, guiW-10, 15 ) );
    //gui->addWidgetDown( new ofxUILabelButton( guiW-10, false, "Load from file", OFX_UI_FONT_MEDIUM ) );
    gui->addSpacer();

    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
}


/***
    2. Events
***/

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch( key ){
        case 'x':
            // TODO: Cambiar, haria que no se pudieran escribir nombres de fichero con x en el textInput.
            deleteCurrentPolygon();
        break;
        /*
        case 'c':
            appMode = MODE_POLYGON_CREATION;
        break;
        case 't':
            appMode = MODE_TRANSLATION;
        break;
        case 'r':
            appMode = MODE_ROTATION;
        break;
        case 's':
            appMode = MODE_SCALE;
        break;
        */
        case KEY_LEFT_ARROW:
            // Select previous polygon in the list.
            if( currentPolygon == polygons.begin() ){
                currentPolygon = polygons.end();
            }
            currentPolygon--;
        break;
        case KEY_RIGHT_ARROW:
            // Select next polygon in the list.
            currentPolygon++;
            if( currentPolygon == polygons.end() ){
                currentPolygon = polygons.begin();
            }
        break;
        default:
            cout << "key: " << key << endl;
        break;
    }
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

    //Convert (x, y) from screen space to world space.
    //Polygon::PixelToWorld( x, y );

	//renderer->PointToSpace( x, y );

    // Set one transformation or another according to pressed key.
    switch( appMode ){
		case MODE_TRANSLATION:
		    currentPolygon->Translate( x-lastMouseX, -y+lastMouseY );
		break;
        case MODE_ROTATION:
            aux = x-lastMouseX;
            currentPolygon->Rotate( aux );
        break;
        case MODE_SCALE:
            aux = x-lastMouseX;

            aux = aux ? 1+aux*0.01 : 1;
            currentPolygon->Scale( aux, aux );
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
    if( appMode != MODE_POLYGON_CREATION ){
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
        tempPolygon.addVertex( x, y );
    break;
    case R_MOUSE:
        addPolygon( tempPolygon );

        tempPolygon.clear();
    break;
    default:
    break;
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}


void testApp::guiEvent( ofxUIEventArgs &e )
{
    PolygonsFile polygonsFile;

    if( e.widget->getName() == savingButton->getName() ){
        if( savingButton->getValue() ){
            cout << "Saving to file [" << fileInput->getTextString() << "]" << endl;

            if( !polygonsFile.save( fileInput->getTextString(), &polygons ) ){
                fileNotFoundLabel->setVisible( false );
            }else{
                fileNotFoundLabel->setVisible( true );
            }
        }
    }else if( e.widget->getName() == loadingButton->getName() ){
        if( loadingButton->getValue() ){
            cout << "Loading from file [" << fileInput->getTextString() << "]" << endl;
            if( !polygonsFile.load( fileInput->getTextString(), &polygons ) ){
                currentPolygon = polygons.begin();
                fileNotFoundLabel->setVisible( false );
            }else{
                fileNotFoundLabel->setVisible( true );
            }
        }
    }
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
    ofRect( guiW, RENDER_WINDOW_BORDER, RENDER_WINDOW_BORDER, appH-RENDER_WINDOW_BORDER<<1 );

    ofSetColor( ofColor::white );
}

/***
    3. Updating and drawing
***/

//--------------------------------------------------------------


//--------------------------------------------------------------


void testApp::update(){
    int w, h;
    //cout << "Soy main thread\n";

    // If there are polygons to copy to the server, copy them.
    if(toServerPolygons.size() > 0){
        cout << "To server poligons > 0\n";
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

    // Prevent the user from resizing the window.
    w = ofGetWidth();
    h = ofGetHeight();

    if( (w != guiW+appW) || (h != appH) ){
        ofSetWindowShape( guiW+appW, appH );
    }

    ofxUIToggle* appModeSelection = appModeSelector->getActive();
    for( unsigned int i=0; i<N_APP_MODES; i++ ){
        if( !appModeSelection->getName().compare( appModeStr[i] ) ){
            appMode = (AppMode)i;
        }
    }
}

//--------------------------------------------------------------

void testApp::draw()
{
    drawEdges();


    std::vector< class Polygon >::iterator it = polygons.begin();

    for( ; it != polygons.end(); ++it ){
        if( it == currentPolygon ){
            ofSetColor( ofColor::white);
        }else{
            ofSetColor( 150, 150, 150 );
        }
        it->draw();
        ofSetColor( ofColor::white);
    }

    tempPolygon.draw();
    Vertex origin = Polygon::getOrigin();

    //Vertex currentMouseWorldPos( currentMousePos[X]-origin[X], -currentMousePos[Y]+origin[Y]  );
    Vertex currentMouseWorldPos( lastMouseX-origin[X], -lastMouseY+origin[Y]  );

    if( (appMode == MODE_POLYGON_CREATION) && tempPolygon.getSize() ){
        Polygon::drawLine( tempPolygon.getLastVertex(), currentMouseWorldPos );
    }

    server->drawBrick();
    //polygon.Draw();
    //drawGUI();
}


/***
    4. Exit
***/

void testApp::exit()
{
    cout << "cerrando 0\n";
    server->exit();
    server->stopThread();
    sem_close(mutex);
    sem_unlink("mutexForServer");
    cout << "cerrando 1\n";

    //gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
}

//--------------------------------------------------------------
void testApp::release( sem_t* mutex_)
{
    if(sem_post(mutex) < 0) {
      perror("main thread: error on post semaphore");
      _Exit(EXIT_FAILURE);
    }
}


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

void testApp::addPolygon( Polygon polygon ){
    currentPolygon = polygons.insert( polygons.end(), polygon );
    toServerPolygons.push_back(tempPolygon);
}

void testApp::deleteLastPolygon()
{
    polygons.pop_back();
}

void testApp::deleteCurrentPolygon()
{
    if( polygons.size() ){
        polygons.erase( currentPolygon );
        if( currentPolygon == polygons.end() ){
            polygons.begin();
        }
    }
}
