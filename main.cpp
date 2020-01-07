#include <functional>

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
    mFileStream = std::make_unique<std::ofstream>("wxtrace.log");
    mLogStream = std::make_unique<wxLogStream>(mFileStream.get());
    wxLog::SetActiveTarget(mLogStream.get());
#endif
    wxLogMessage("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
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
    wxLogMessage("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    OpenFileDialog openFileDialog(this, mPath, mSeparator, mQuote, mEscape);
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    auto path = openFileDialog.getPath();
    auto separator = openFileDialog.getSeparator();
    auto quote = openFileDialog.getQuote();
    auto escape = openFileDialog.getEscape();

    if (path != mPath) {
        wxLogMessage("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
        mPath = path;
        mThreadIsDone = false;
        mPercent = 0;

        wxProgressDialog progressDialog("Scanning file", path, 100, this,
            wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_REMAINING_TIME);
        progressDialog.Update(0);

        auto threadError = CreateThread(wxTHREAD_JOINABLE);
        wxASSERT(threadError == wxTHREAD_NO_ERROR);

        threadError = GetThread()->Run();
        wxASSERT(threadError == wxTHREAD_NO_ERROR);

        wxLogMessage("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
        bool threadIsDone { false };
        int prevPercent { -1 }, percent { 0 };
        while (true) {
            {
                wxCriticalSectionLocker lock(mPercentCS);
                percent = mPercent;
            }

            if (percent - prevPercent >= 1) {
                progressDialog.Update(percent);
                prevPercent = percent;
            } else {
                wxTheApp->SafeYieldFor(NULL, wxEVT_CATEGORY_UI);
            }

            {
                wxCriticalSectionLocker lock(mThreadIsDoneCS);
                threadIsDone = mThreadIsDone;
            }

            if (threadIsDone) {
                wxLogMessage("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
                auto exitCode = GetThread()->Wait(wxTHREAD_WAIT_BLOCK);
                wxASSERT(exitCode == (wxThread::ExitCode)0);
                break;
            } else {
                wxThread::Sleep(100);
            }
        }
        wxLogMessage("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);

        mTokenizedFileLines->setTokenizerParams(escape, separator, quote);
        wxLogMessage("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    } else {
        if (separator != mSeparator || quote != mQuote || escape != mEscape) {
            wxLogMessage("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
            mTokenizedFileLines->setTokenizerParams(escape, separator, quote);
        }
    }
    wxLogMessage("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);

    mSeparator = separator;
    mQuote = quote;
    mEscape = escape;

    mGridTable = new CsvFileGridTable(*mTokenizedFileLines);
    wxGridUpdateLocker gridUpdateLocker(mGrid);
    mGrid->SetTable(mGridTable, true);
    wxLogMessage("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
}

wxThread::ExitCode MainFrame::Entry()
{
    wxLogMessage("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    auto tokenizedFileLines = new TokenizedFileLines(bfs::path(mPath), std::bind(&MainFrame::OnProgress, this, std::placeholders::_1));
    wxLogMessage("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    {
        wxCriticalSectionLocker lock(mThreadIsDoneCS);
        mTokenizedFileLines.reset(tokenizedFileLines);
        mThreadIsDone = true;
    }
    return (wxThread::ExitCode)0; // success
}

void MainFrame::OnProgress(int percent)
{
    wxLogMessage("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    {
        wxCriticalSectionLocker lock(mPercentCS);
        mPercent = percent;
    }
    wxLogMessage("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
}
