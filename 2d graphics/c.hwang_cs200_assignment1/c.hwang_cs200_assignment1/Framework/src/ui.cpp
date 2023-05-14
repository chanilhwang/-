/******************************************************************************/
/*!
\file   ui.cpp
\par    Purpose: User interface class implementation.
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Prasanna Ghali, Sang Le
\par    email: pghali@digipen.edu, sang.le@digipen.edu
\par    Course: CS200
\date   22/5/2015
*/
/******************************************************************************/

#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "Ui.hpp"
#include "Window.hpp"
#include "Line2D.hpp"
#include "Box2D.hpp"

// Global variables defined in Main.cpp
Vector2D                gDisplacement;
std::vector<Line2D>     gLineList, gSecLineList;
std::vector<LineBox2D>  gLineBoxList, gSecLineBoxList;
Line2D*                 gPPickedLine;
LineBox2D*              gPPickedLineBox;
std::string             gModalMsg;

int                     width, height;
int                     oldWidth, oldHeight;

Color                   gRedClr(255, 0, 0, 255);
Color                   gBlueClr(0, 0, 255, 255);
Color                   gBlackClr(0, 0, 0, 255);
Color                   gWhiteClr(255, 255, 255, 255);

// global objects with file scope
LineAlgoType    gActiveLineType = LINE_ALGO_DDA;
bool            gCreateLine, gMoveLine, gEditLine;
bool            gVertexEditFlag;


// function declarations with file scope
static void             InsertLine( std::vector<Line2D>&, 
                                    std::vector<LineBox2D>&,
                                    Point2D [2]);
static void             DeleteLines(std::vector<Line2D>&,
                                    std::vector<LineBox2D>&);
static void             WriteSceneToFile(   const char*,
                                            std::vector<Line2D>&);
static void             ReadSceneFromFile(  const char*,
                                            std::vector<Line2D>&,
                                            std::vector<LineBox2D>&);
static Color            ReadColorLineFromFile(  std::ifstream&);
static Point2D          ReadPositionLineFromFile(std::ifstream&);

static bool             IsMouseOnLine(const Point2D&);
static bool             IsMouseOnLine(const Point2D&, unsigned int&);
static bool             IsMouseInLineBox(const Point2D&);
static bool             IsMouseInLineBox(const Point2D&, unsigned int&);

// function declarations with global scope
bool                    ActiveLineAlgoType(LineAlgoType);

/******************************************************************************/
/*!
\fn     LRESULT CALLBACK WndProc(   HWND hwnd, UINT message, 
                                    WPARAM wParam, LPARAM lParam)
\brief
        Windows message handler.
\param  hwnd
        Handler to the window.
\param  message
        The message based on user input (mouse/keyboard/menu...)
\param  wParam
        Additional message parameters. See examples below.
\param  lParam
        Additional message parameters. See examples below.
*/
/******************************************************************************/
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LPCTSTR         lpCursorName = IDC_ARROW;
    static Point2D  sPrevMousePos, sLineEndPts[2];
    Point2D         currMousePos(static_cast<float>(LOWORD(lParam)),
                                        static_cast<float>(HIWORD(lParam)));
    unsigned int    lineIdx, lineBoxIdx;
    float           fSign;

    switch (msg)    // Check  message type
    {
        case WM_CREATE:     // Window creation
        {
            ReadSceneFromFile("CS200LineDrawing.scn", gLineList, gLineBoxList);
            
            gPPickedLine    =    0;
            gPPickedLineBox =    0;
            gCreateLine     =    false;
            gMoveLine       =    false;
            gEditLine       =    false;

            sPrevMousePos.SetPosition(0.f, 0.f);

            CheckMenuItem(GetMenu(hwnd), IDM_DDA, MF_CHECKED);
            CheckMenuItem(GetMenu(hwnd), IDM_MIDPOINT, MF_UNCHECKED);
            DrawMenuBar(hwnd);

            lpCursorName    =    IDC_CROSS;

            return 0;
        }

        case WM_SIZE:       // Window resizing
        {
            width = LOWORD(lParam);
            height = HIWORD(lParam);
            return 0;
        }

        case WM_LBUTTONUP:  // left mouse button clicks released
        {
            if (gCreateLine == true)
            {
                // when creating lines, mouse left button click up denotes end point
                sLineEndPts[1]  =    currMousePos;            
                InsertLine(gLineList, gLineBoxList, sLineEndPts);
                gSecLineList.clear(); // discard temporary line
                gCreateLine     =
                gMoveLine       =
                gEditLine       =   false;

                gPPickedLine    =   &gLineList[gLineList.size()-1];
                gPPickedLineBox =   &gLineBoxList[gLineBoxList.size()-1];
            }
            else if (gMoveLine == true)
            {
                gMoveLine       =
                gEditLine       =
                gCreateLine     =   false;
            }
            else if (gEditLine == true)
            {
                gMoveLine       =
                gEditLine       =
                gCreateLine     =   false;

                Vertex2D vtx(gPPickedLine->GetStartVertex());
                Point2D p0 = vtx.GetPosition();
                vtx = gPPickedLine->GetEndVertex();
                Point2D p1 = vtx.GetPosition();
                Point2D inter;

                LineBox2D lb;
                
                if (gVertexEditFlag == true) // start point is moved
                {
                    // end vertex is pivot
                    lb.ComputeIntersection(p1, p0, inter);
                    Point2D diff = inter - p1;
                    p1 += diff;
                    vtx.SetPosition(p1);
                    gPPickedLine->SetEndVertex(vtx);

                    gPPickedLineBox->ComputeStartBox(p0, p1);
                }
                else // end point is moved
                {
                    // start vertex is pivot
                    lb.ComputeIntersection(p0, p1, inter);
                    Point2D diff = inter - p0;
                    p0 += diff;
                    vtx.SetPosition(p0);
                    gPPickedLine->SetStartVertex(vtx);

                    gPPickedLineBox->ComputeEndBox(p0, p1);
                }
            }

            if (IsMouseInLineBox(currMousePos, lineBoxIdx) == true)
            {
                // compute orientation of line
                fSign           =    gLineList[lineBoxIdx].GetDeltaX()*gLineList[lineBoxIdx].GetDeltaY();
                lpCursorName    =    (fSign >= 0.f) ? IDC_SIZENWSE : IDC_SIZENESW;        
            }
            else if (IsMouseOnLine(currMousePos, lineIdx) == true)
                lpCursorName    =    IDC_SIZEALL;
            else
                lpCursorName    =    IDC_CROSS;

            HCURSOR hCursor = LoadCursor(NULL, lpCursorName);
            ::SetCursor(hCursor);

            return 0;
        }

        case WM_LBUTTONDOWN: // left mouse button clicks clicked
        {
            // left mouse button down click determines start point of line segment
            // holding the left button down, move mouse to new location
            // specify end point of line segment by releasing left mouse button
            if (IsMouseInLineBox(currMousePos, lineBoxIdx) == true)
            {
                // if, previously not editing any line box, or if editing a line
                // but now a different line is selected, then displacement vector
                // must be computed using current mouse position as tail of vector
                if (gEditLine == false || gPPickedLineBox != &gLineBoxList[lineBoxIdx])
                {
                    gEditLine       =    true;
                    gMoveLine       =    gCreateLine    =    false;
                    
                    gPPickedLineBox =    &gLineBoxList[lineBoxIdx];
                    gPPickedLine    =    &gLineList[lineBoxIdx];
                    sPrevMousePos   =    currMousePos;

                    Box2D bb        =    gPPickedLineBox->GetStartBox();
                    gVertexEditFlag =    (bb.TestBox2DVsPoint(currMousePos)) ? true : false;
                
                    // update the stationary vertex position to center of its edit box ...
                    if (gVertexEditFlag == true)
                    {
                        // stationary vertex is end point
                        bb    =    gPPickedLineBox->GetEndBox();
                        Vertex2D vtx(gPPickedLine->GetEndVertex());
                        vtx.SetPosition(bb.GetCenter());
                        gPPickedLine->SetEndVertex(vtx);
                    }
                    else
                    {
                        // stationary vertex is start point
                        bb    =    gPPickedLineBox->GetStartBox();
                        Vertex2D vtx(gPPickedLine->GetStartVertex());
                        vtx.SetPosition(bb.GetCenter());
                        gPPickedLine->SetStartVertex(vtx);
                    }
                }

                fSign           =    gPPickedLine->GetDeltaX()*gPPickedLine->GetDeltaY();
                lpCursorName    =    ( fSign >= 0.f) ? IDC_SIZENWSE : IDC_SIZENESW;
            }
            else if (IsMouseOnLine(currMousePos, lineIdx) == true)
            {
                // if, previously not translating any line, or if translating a line
                // but now a different line is selected, then displacement vector
                // must be computed using current mouse position as tail of vector
                if (gMoveLine == false || gPPickedLine != &gLineList[lineIdx])
                {
                    gMoveLine       =    true;
                    gEditLine       =
                    gCreateLine     =    false;
                    gPPickedLine    =    &gLineList[lineIdx];
                    gPPickedLineBox =    &gLineBoxList[lineIdx];
                    sPrevMousePos   =    currMousePos;
                    lpCursorName    =    IDC_SIZEALL;
                }
            }
            else
            {
                // start point of a new line segment is determined by first
                // left mouse button down click ...
                sLineEndPts[0]  =    currMousePos;
                gCreateLine     =    true;
                gMoveLine       =
                gEditLine       =    false;
                gPPickedLine    =    0;
                gPPickedLineBox =    0;
                lpCursorName    =    IDC_CROSS;
            }
                
            HCURSOR hCursor = LoadCursor(NULL, lpCursorName);
            ::SetCursor(hCursor);

            return 0;
        }
        case WM_MOUSEMOVE:      // mouse is dragged
        {
            if (gCreateLine == true)
            {
                // start point is determined by mouse left button click down and the
                // click point is saved in sLineEndPts
                // the end point is determined by mouse left button click up
                // the current mouse location is used to create a temporary line
                
                // current mouse position is recorded as end point of temporary line segment
                //sLineEndPts.push_back(currMousePos);
                sLineEndPts[1]    =    currMousePos;
                
                // temporary list of lines should contain no more than one line segment
                // so, previous temporary line segment is discarded 
                // next, temporary line segment is inserted into temp list and
                // finally, the temporary end point is discarded ...
                gSecLineList.clear();
                InsertLine(gSecLineList, gSecLineBoxList, sLineEndPts);
                lpCursorName = IDC_CROSS;
            }
            else if (gMoveLine == true)
            {
                gDisplacement    =    currMousePos - sPrevMousePos;
                sPrevMousePos    =    currMousePos;
                lpCursorName    =    IDC_SIZEALL;
            }
            else if (gEditLine == true)
            {
                Vector2D vec(currMousePos-sPrevMousePos);
                if (gVertexEditFlag == true)
                {
                    Vertex2D vtx    =    gPPickedLine->GetStartVertex();
                    vtx.SetPosition(vtx.GetPosition() + vec);
                    gPPickedLine->SetStartVertex(vtx);
                    gPPickedLineBox->MoveStart(vec);
                }
                else
                {
                    Vertex2D vtx    =    gPPickedLine->GetEndVertex();
                    vtx.SetPosition(vtx.GetPosition() + vec);
                    gPPickedLine->SetEndVertex(vtx);
                    gPPickedLineBox->MoveEnd(vec);
                }
                
                sPrevMousePos    =    currMousePos;
                lpCursorName    =    IDC_SIZEALL;
            }
            else if (IsMouseInLineBox(currMousePos, lineBoxIdx) == true)
            {
                // arrow icon only for line  lines ...
                if (gPPickedLineBox == &gLineBoxList[lineBoxIdx])
                {
                    fSign            =    gLineList[lineBoxIdx].GetDeltaX()*gLineList[lineBoxIdx].GetDeltaY();
                    lpCursorName    = (fSign >= 0.f) ? IDC_SIZENWSE : IDC_SIZENESW;
                }
                else
                {
                    lpCursorName    =    IDC_SIZEALL;
                }
            }
            else if (IsMouseOnLine(currMousePos) == true)
            {
                lpCursorName    =    IDC_SIZEALL;
            }
            else
            {
                lpCursorName = IDC_CROSS;
            }

            HCURSOR hCursor = LoadCursor(NULL, lpCursorName);
            ::SetCursor(hCursor);

            return 0;
        }

        case WM_CHAR:   // some key is pressed
        {
            if ('s' == wParam || 'S' == wParam)
            {
                WriteSceneToFile("CS200LineDrawing.scn", gLineList);
            }
            if ('C' == wParam) // keycode is specified by wParam
            {
                DeleteLines(gLineList, gLineBoxList);
                DeleteLines(gSecLineList, gSecLineBoxList);
                gPPickedLineBox =   0;
                gPPickedLine    =   0;
                gCreateLine     =    
                gMoveLine       =
                gEditLine       =   false;
                break;
            }
            else if ('c' == wParam)
            {
                unsigned int ui;
                for (ui = 0; ui < gLineList.size(); ++ui)
                {
                    if (gPPickedLine == &gLineList[ui])
                        break;
                }

                if (ui < gLineList.size())
                {
                    gLineList.erase(gLineList.begin()+ui);
                    gLineBoxList.erase(gLineBoxList.begin()+ui);
                    gPPickedLine = 0;
                    gPPickedLineBox = 0;
                }
            }

            return 0;
        }

        case WM_COMMAND:    // Handle messages from the Menu bar
        {
            // Get a handle to the menu
            HMENU hMenu = ::GetMenu(hwnd);
            switch(LOWORD(wParam))
            {
                case IDM_DDA:       // Activate the line DDA menu item
                {
                    if (gActiveLineType == LOWORD(wParam)) break;

                    LineAlgoType tmpAlg =    gActiveLineType;
                    gActiveLineType     =    LINE_ALGO_DDA;
                    CheckMenuItem(hMenu, gActiveLineType, MF_CHECKED);
                    CheckMenuItem(hMenu, tmpAlg, MF_UNCHECKED);
                    DrawMenuBar(hwnd);
                    
                    break;
                }
                case IDM_MIDPOINT:  // Activate the line midpoint menu item
                {
                    if (gActiveLineType == LOWORD(wParam)) break;

                    LineAlgoType tmpAlg =    gActiveLineType;
                    gActiveLineType     =    LINE_ALGO_MIDPOINT;
                    CheckMenuItem(hMenu, gActiveLineType, MF_CHECKED);
                    CheckMenuItem(hMenu, tmpAlg, MF_UNCHECKED);
                    DrawMenuBar(hwnd);
                    
                    break;
                }

                case IDM_ABOUT:     // Display help message
                    MessageBox(hwnd, gModalMsg.c_str(), "CS200 A1 Framework", MB_OK);
                    break;
            }
            
            return 0;
        }

        case WM_SYSCOMMAND:  // look for screensavers and powersave mode
        {
            switch (wParam)
            {
                case SC_SCREENSAVE:     // screensaver trying to start
                case SC_MONITORPOWER:   // monitor going to powersave mode
                    // returning 0 prevents either from happening
                    return 0;
                default:
                    break;
            }
        } break;

        case WM_DESTROY:    // Destroy message
            return 0;

        case WM_CLOSE:      // Close message
            DeleteLines(gLineList, gLineBoxList);
            PostQuitMessage(0);
            return 0;

        default:            // Default handling
            break;
    }

    // Dispatch any unhandled messages
    return DefWindowProc(hwnd, msg, wParam, lParam);
}


/******************************************************************************/
/*!
\fn     static bool IsMouseOnLine(const Point2D& pt)
\brief
        Checks if the mouse is on a line segment.
\param  pt
        Reference to point to be checked, i.e. mouse position.
\return 
        true if mouse is on line segment; false otherwise.
*/
/******************************************************************************/
static bool IsMouseOnLine(const Point2D& pt)
{
    for (unsigned ui = 0; ui < gLineList.size(); ++ui)
    {
        if (gLineList[ui].TestLineVsPoint(pt))
            return true;
    }

    return false;
}


/******************************************************************************/
/*!
\fn     static bool IsMouseOnLine(const Point2D& pt, unsigned int& ui)
\brief
        Checks if the mouse is on a line segment.
\param  pt
        Reference to point to be checked, i.e. mouse position.
\param  ui
        Index to store the line that mouse is on.
\return 
        true if mouse is on line segment; false otherwise.
*/
/******************************************************************************/
static bool IsMouseOnLine(const Point2D& pt, unsigned int& ui)
{
    for (ui = 0; ui < gLineList.size(); ++ui)
    {
        if (gLineList[ui].TestLineVsPoint(pt))    
            return true;
    }

    return false;
}


/******************************************************************************/
/*!
\fn     static bool IsMouseInLineBox(const Point2D& pt)
\brief
        Checks if the mouse is inside the edit square boxes at 
        endpoints of a line segment.
\param  pt
        Reference to point to be checked, i.e. mouse position.
\return 
        true if mouse is inside box; false otherwise.
*/
/******************************************************************************/
static bool IsMouseInLineBox(const Point2D& pt)
{
    for (unsigned int ui = 0; ui < gLineBoxList.size(); ++ui)
    {
        Box2D start =   gLineBoxList[ui].GetStartBox();
        Box2D end   =   gLineBoxList[ui].GetEndBox();

        if (start.TestBox2DVsPoint(pt) || end.TestBox2DVsPoint(pt))
            return true;
    }

    return false;
}


/******************************************************************************/
/*!
\fn     static bool IsMouseInLineBox(const Point2D& pt, unsigned int& ui)
\brief
        Checks if the mouse is inside the edit square boxes at 
        endpoints of a line segment.
\param  pt
        Reference to point to be checked, i.e. mouse position.
\param  ui
        Index to store the line whose boxes the mouse is inside.
\return 
        true if mouse is inside box; false otherwise.
*/
/******************************************************************************/
static bool IsMouseInLineBox(const Point2D& pt, unsigned int& ui)
{
    for (ui = 0; ui < gLineBoxList.size(); ++ui)
    {
        Box2D start =   gLineBoxList[ui].GetStartBox();
        Box2D end   =   gLineBoxList[ui].GetEndBox();

        if (start.TestBox2DVsPoint(pt) || end.TestBox2DVsPoint(pt))
        {
            return true;
        }
    }

    return false;
}


/******************************************************************************/
/*!
\fn     bool ActiveLineAlgoType(LineAlgoType type)
\brief
        Checks if an option is active.
\param  type
        The option to test.
\return 
        true if the option is set to active, false if not.
*/
/******************************************************************************/
bool ActiveLineAlgoType(LineAlgoType type)
{
    return (gActiveLineType == type);
}


/******************************************************************************/
/*!
\fn     void InsertLine(    std::vector<Line2D>& lineList, 
                            std::vector<LineBox2D>& aabbList,
                            Point2D pts[2]) 
\brief
        Adds 2d line primitve to list.
\param  lineList
        The list to which primitive is added.
\param  aabbList
        The list to which endpoint boxes are added.
\param  pts
        2 endpoints of line segment.
\return 
        If true, then completely modeled primitive is added to list.
        If false, temporary primitive is added to list after previous temporary
        primitive is deleted.
*/
/******************************************************************************/
void InsertLine(    std::vector<Line2D>& lineList, 
                    std::vector<LineBox2D>& aabbList,
                    Point2D pts[2]) 
{
    Point2D vec(pts[1] - pts[0]);
    float mag = vec.Magnitude();
    if (mag <= 1.f)
    {
        pts[1].SetX(pts[0].GetX() + 5.f);
        pts[1].SetY(pts[0].GetY() + 5.f);
    }
    else if (mag <= 5.f)
    {
        mag = 5.f/mag;
        pts[1].SetX(pts[0].GetX() + mag*(pts[1].GetX()-pts[0].GetX()));
        pts[1].SetY(pts[0].GetY() + mag*(pts[1].GetY()-pts[0].GetY()));
    }    

    Vertex2D start(pts[0], gRedClr);
    Vertex2D end(pts[1], gRedClr);
    lineList.push_back(Line2D(start, end));

    // compute Box2D's for both end points
    LineBox2D lb;
    lb.ComputeBox(pts[0], pts[1]);
    aabbList.push_back(lb);
}


/******************************************************************************/
/*!
\fn     void DeleteLines(   std::vector<Line2D>& lineList, 
                            std::vector<LineBox2D>& aabbList)
\brief
        Delete memory allocated to primitives by each element in list.
        Next, erases all elements in list.
\param  lineList
        The line list whose contents are to be deleted.
\param  aabbList
        The box list whose contents are to be deleted.
*/
/******************************************************************************/
void DeleteLines(std::vector<Line2D>& lineList, std::vector<LineBox2D>& aabbList)
{
    lineList.clear();
    aabbList.clear();
}


/******************************************************************************/
/*!
\fn     void WriteSceneToFile(const char* ps, std::vector<Line2D>& list)
\brief
        Saves modeled scene to file.
\param  ps
        Name of file to which scene is saved.
\param  list
        The list of 2d line segments to be saved to file.
*/
/******************************************************************************/
void WriteSceneToFile(const char* ps, std::vector<Line2D>& list)
{
    std::ofstream ofs(ps);
    if (ofs.fail() == true)
    {
        throw std::runtime_error("Could not open file!!!");
    }

    ofs << static_cast<unsigned int>(list.size()) << "\n";
    for (unsigned int ui = 0; ui < list.size(); ++ui)
    {
        ofs << list[ui];
    }

    ofs.close();
}


/******************************************************************************/
/*!
\fn     void ReadSceneFromFile( const char* ps, 
                                std::vector<Line2D>& list,
                                std::vector<LineBox2D>& aabbList)
\brief
        Reads saved scene from file and writes the file contents to list 
        of 2d line segments.
\param  ps
        Name of file from which scene is read.
\param  list
        The vector container which will store the retrieved scene.
*/
/******************************************************************************/
void ReadSceneFromFile(const char* ps, 
                       std::vector<Line2D>& list,
                       std::vector<LineBox2D>& aabbList)
{
    Point2D    startVtxPos, endVtxPos;
    Vertex2D startVtx, endVtx;

    std::ifstream ifs(ps, std::ios::in);
    if (ifs.fail() == true)
    {
        //throw std::runtime_error("Could not open file!!!");
        return;
    }
    ifs.seekg(0, std::ios::beg);

    int lineCnt = 0;
    char maxTextLen[100];

    ifs.getline(maxTextLen, 100);
    sscanf_s(maxTextLen, " %d ", &lineCnt);
    while (lineCnt--)
    {
        startVtxPos =    ReadPositionLineFromFile(ifs);
        startVtx.SetPosition(startVtxPos);
        startVtx.SetColor(ReadColorLineFromFile(ifs));
        endVtxPos   =    ReadPositionLineFromFile(ifs);
        endVtx.SetPosition(endVtxPos);
        endVtx.SetColor(ReadColorLineFromFile(ifs));

        list.push_back(Line2D(startVtx, endVtx));

        // compute Box2D's for both end points
        LineBox2D lb;
        lb.ComputeBox(startVtxPos, endVtxPos);
        aabbList.push_back(lb);
    }

    ifs.close();
}


/******************************************************************************/
/*!
\fn     Point2D ReadPositionLineFromFile(std::ifstream& ifs)
\brief
        Reads 2d point coordinates from a text file.
\param  ifs
        Reference to file pointer.
\return 
        Point2D object.
*/
/******************************************************************************/
Point2D ReadPositionLineFromFile(std::ifstream& ifs)
{
    char str[100];
    ifs.getline(str, 100);

    float fx, fy;
    sscanf_s(str, " %f %f", &fx, &fy);
    return (Point2D(fx, fy));
}


/******************************************************************************/
/*!
\fn     Color ReadColorLineFromFile(std::ifstream& ifs)
\brief
        Reads RGBA color coordinates from a text file.
\param  ifs
        Reference to file pointer.
\return 
        Color object.
*/
/******************************************************************************/
Color ReadColorLineFromFile(std::ifstream& ifs)
{
    char str[100];
    ifs.getline(str, 100);

    int r, g, b, a;
    sscanf_s(str, " %d %d %d %d", &r, &g, &b, &a);

    return (Color(  static_cast<unsigned char>(r),
                    static_cast<unsigned char>(g),
                    static_cast<unsigned char>(b),
                    static_cast<unsigned char>(a)));
}
