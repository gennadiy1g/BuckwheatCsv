#include "app.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    AppFrame* frame = new AppFrame();
    frame->Show(true);
    return true;
}

AppFrame::AppFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    mGrid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    mTable = new EmptyGridTable();
    mGrid->SetTable(mTable, true);

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &AppFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &AppFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &AppFrame::OnExit, this, wxID_EXIT);
}

void AppFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void AppFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
}

void AppFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}
