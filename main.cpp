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
    wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    OpenFileDialog openFileDialog(this, mPath, mSeparator, mQuote, mEscape);
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    auto path = openFileDialog.getPath();
    auto separator = openFileDialog.getSeparator();
    auto quote = openFileDialog.getQuote();
    auto escape = openFileDialog.getEscape();

    if (path != mPath) {
        wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
        mTokenizedFileLines = std::make_unique<TokenizedFileLines>(bfs::path(path));
        mTokenizedFileLines->setTokenizerParams(escape, separator, quote);
    } else {
        if (separator != mSeparator || escape != mEscape || quote != mQuote) {
            wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
            mTokenizedFileLines->setTokenizerParams(escape, separator, quote);
        }
    }

    mPath = path;
    mSeparator = separator;
    mQuote = quote;
    mEscape = escape;

    mGridTable = new CsvFileGridTable(*mTokenizedFileLines);
    wxGridUpdateLocker gridUpdateLocker(mGrid);
    mGrid->SetTable(mGridTable, true);
}
