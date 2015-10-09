/* 
 * File:   main.cpp
 * 
 */
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#ifdef __linux
#include <cstdlib>
#elif __APPLE__
#include <stdarg.h>
#include <cstddef>
#include <cstdlib>
#elif _WIN32
#endif

/*
 * Include your main form header here.
 * 
 */
//example
#include "frmMain.h"

using namespace std;

/*
 * Replace 'MyClassName' below to the derived class from
 * wxFormBuilder
 * 
 */
class wxMyApp : public wxApp
{
        public:
                virtual bool OnInit();
};
IMPLEMENT_APP(wxMyApp)
bool wxMyApp::OnInit()
{
        frmMain* frame=new frmMain((wxWindow*)NULL);
        frame->Show(true);
        return true;
}
