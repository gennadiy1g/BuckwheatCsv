#include <functional>

#include <wx/filedlg.h>

#include "CsvTable/log.h"
#include "CsvTable/utilities.h"
#include "GridTable.h"
#include "OpenFileDialog.h"
#include "main.h"

using namespace std::literals::string_literals;

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    MainFrame* frame = new MainFrame();
    frame->Show(true);

    initLocalization();
    initLogging();

    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
    return true;
}

MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, App::kAppName)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    // set the frame icon
    SetIcon(wxICON(table));

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
    mGridTable = std::make_unique<EmptyGridTable>();
    mGrid->SetTable(mGridTable.get());

    CreateStatusBar();

    Bind(wxEVT_MENU, &MainFrame::OnOpen, this, wxID_OPEN);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);

    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
}

void MainFrame::OnExit(wxCommandEvent& event) { Close(true); }

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnOpen(wxCommandEvent& event)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    OpenFileDialog openFileDialog(this, mPath, mSeparator, mQuote, mEscape);
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    showFile(openFileDialog.getPath(), openFileDialog.getSeparator(), openFileDialog.getEscape(), openFileDialog.getQuote());
}

void MainFrame::showFile(wxString path, wxChar separator, wxChar escape, wxChar quote)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    if (path != mPath) {
        BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
        mPath = path;
        mThreadIsDone = false;
        mPercent = 0;
        mScanFailed = false;
        mErrorMessage = "";

        wxProgressDialog progressDialog("Scanning file", path, 100, this,
            wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_REMAINING_TIME);
        progressDialog.Update(0);

        wxGridUpdateLocker gridUpdateLocker(mGrid);
        mGrid->ClearGrid();

        auto threadError = CreateThread(wxTHREAD_JOINABLE);
        wxASSERT(threadError == wxTHREAD_NO_ERROR);

        threadError = GetThread()->Run();
        wxASSERT(threadError == wxTHREAD_NO_ERROR);

        BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
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
                BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
                auto exitCode = GetThread()->Wait(wxTHREAD_WAIT_BLOCK);
                BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
                wxASSERT(exitCode == static_cast<wxThread::ExitCode>(0));
                break;
            } else {
                wxThread::Sleep(100);
            }
        }
        BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

        mGrid->SetTable(mGridTable2.get());
        mGridTable = std::move(mGridTable2);
        BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
        mGridTable->setTokenizerParams(escape, separator, quote);
    } else if (separator != mSeparator || quote != mQuote || escape != mEscape) {
        BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
        wxGridUpdateLocker gridUpdateLocker(mGrid);
        mGridTable->setTokenizerParams(escape, separator, quote);
    }
    SetStatusText(mGridTable->getStatusText());
    SetTitle(mGridTable->getTitle() + App::kAppName);

    mSeparator = separator;
    mQuote = quote;
    mEscape = escape;
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    if (mScanFailed) {
        wxMessageDialog messageDialog(this, mErrorMessage, "Attention", wxOK | wxICON_WARNING | wxCENTRE);
        messageDialog.ShowModal();
    }
}

wxThread::ExitCode MainFrame::Entry()
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
    try {
        mGridTable2 = std::make_unique<CsvFileGridTable>(bfs::path(mPath), std::bind(&MainFrame::OnProgress, this, std::placeholders::_1));
    } catch (const std::runtime_error& e) {
        BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
        mGridTable2 = std::make_unique<EmptyGridTable>();
        mScanFailed = true;
        mErrorMessage = e.what();
    }
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
    {
        wxCriticalSectionLocker lock(mThreadIsDoneCS);
        mThreadIsDone = true;
    }
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
    return static_cast<wxThread::ExitCode>(0); // success
}

void MainFrame::OnProgress(int percent)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
    {
        wxCriticalSectionLocker lock(mPercentCS);
        mPercent = percent;
    }
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
}
