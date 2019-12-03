#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/grid.h>

class App : public wxApp
{
public:
    virtual bool OnInit();
};

class AppFrame : public wxFrame
{
public:
    AppFrame();

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    wxGrid* mGrid;
};

enum { ID_Hello = 1 };
