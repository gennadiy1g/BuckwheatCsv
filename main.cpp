#include <wx/filedlg.h>

#include "CsvTable/utilities.h"
#include "GridTable.h"
#include "OpenFileDialog.h"
#include "main.h"

using namespace std::literals::string_literals;

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    MainFrame* frame = new MainFrame();
#ifdef NDEBUG
    wxLog::EnableLogging(false);
#else
    mLogWindow = std::make_unique<wxLogWindow>(frame, "Log", true, false);
#endif
    wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    frame->Show(true);

    initLocalization();
    initLogging();

    return true;
}

MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, "Buckwheat CSV")
{
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(wxID_OPEN, "&Open...\tCtrl-O", "Open file");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    mGrid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    mGrid->EnableEditing(false);
    mGridTable = new EmptyGridTable();
    mGrid->SetTable(mGridTable, true);

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &MainFrame::OnOpen, this, wxID_OPEN);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}

void MainFrame::OnExit(wxCommandEvent& event) { Close(true); }

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnOpen(wxCommandEvent& event)
{
    OpenFileDialog openFileDialog(this, mPath, mSeparatorId, mSeparator, mEscapeId, mEscape, mQuoteId);
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    mPath = openFileDialog.getPath();
    mSeparatorId = openFileDialog.getSeparatorId();
    mSeparator = openFileDialog.getSeparator();
    mEscapeId = openFileDialog.getEscapeId();
    mEscape = openFileDialog.getEscape();
    mQuoteId = openFileDialog.getQuoteId();
    mQuote = openFileDialog.getQuote();

    mGridTable = new CsvFileGridTable(bfs::path(openFileDialog.getPath()));
    wxGridUpdateLocker gridUpdateLocker(mGrid);
    mGrid->SetTable(mGridTable, true);
}
