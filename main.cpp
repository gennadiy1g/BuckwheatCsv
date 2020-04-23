#include <functional>

#include <wx/aboutdlg.h>
#include <wx/filedlg.h>

#include "CsvTable/log.h"
#include "CsvTable/utilities.h"
#include "GoToColumnDialog.h"
#include "GridTable.h"
#include "OpenFileDialog.h"
#include "main.h"

using namespace std::literals::string_literals;

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    initLocalization();
    initLogging();

    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    SetAppDisplayName(kAppName);

    MainFrame* frame = new MainFrame();
    frame->Show(true);

    BOOST_LOG_SEV(gLogger, bltrivial::trace) << "argc=" << wxTheApp->argc << FUNCTION_FILE_LINE;
    if (wxTheApp->argc > 1) {
        BOOST_LOG_SEV(gLogger, bltrivial::trace) << "argv[1]=" << wxTheApp->argv[1] << FUNCTION_FILE_LINE;
        frame->showFile(wxTheApp->argv[1]);
    }
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
    return true;
}

bool App::OnExceptionInMainLoop()
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    wxString error {};
    try {
        throw; // Rethrow the current exception.
    } catch (const std::exception& e) {
        error = e.what();
    } catch (...) {
        error = "unknown error.";
    }

    BOOST_LOG_SEV(gLogger, bltrivial::error) << "Unexpected exception has occurred: " << error << FUNCTION_FILE_LINE;
    wxLogError("Unexpected exception has occurred: %s. The program will terminate.", error);

    // Exit the main loop and thus terminate the program.
    return false;
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
    menuFile->Append(wxID_EXIT, "&Quit\tCtrl-Q");

    wxMenu* menuGoTo = new wxMenu;
    menuGoTo->Append(ID_GOTO_ROW, "Go to &Row...\tCtrl-G", "Go to row");
    menuGoTo->Append(ID_GOTO_COLUMN, "Go to &Column...\tCtrl-Shift-G", "Go to column");

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuGoTo, "&Go to");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    mGrid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    mGrid->EnableEditing(false);
    mGridTable = std::make_unique<EmptyGridTable>();
    mGrid->SetTable(mGridTable.get());

    CreateStatusBar();

    SetDropTarget(new FileDropTarget(this));

    Bind(wxEVT_MENU, &MainFrame::OnOpen, this, wxID_OPEN);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnGoToRow, this, ID_GOTO_ROW);
    Bind(wxEVT_MENU, &MainFrame::OnGoToColumn, this, ID_GOTO_COLUMN);
    Bind(wxEVT_THREAD, &MainFrame::OnDropFiles, this, ID_ON_DROP_FILES);

    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
}

void MainFrame::OnExit(wxCommandEvent& event) { Close(true); }

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxAboutDialogInfo aboutInfo;
    aboutInfo.SetName(wxTheApp->GetAppDisplayName());
    aboutInfo.SetVersion("0.9");
    aboutInfo.SetDescription("Viewer for CSV files");
    aboutInfo.SetCopyright("(C) 2019-2020");
    aboutInfo.SetWebSite("https://github.com/gennadiy1g/BuckwheatCsv/releases");

    wxAboutBox(aboutInfo);
}

void MainFrame::OnOpen(wxCommandEvent& event)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    OpenFileDialog openFileDialog(this, mPath, mSeparator, mQuote, mEscape);
    if (openFileDialog.ShowModal() != wxID_CANCEL) {
        showFile(openFileDialog.getPath(), openFileDialog.getSeparator(), openFileDialog.getEscape(), openFileDialog.getQuote());
    }
}

void MainFrame::showFile(wxString path)
{
    std::optional<wchar_t> separator;
    std::optional<wchar_t> quote;
    bool failed { false };

    try {
        detectSeparatorAndQuote(bfs::path(path), separator, quote);
    } catch (const std::exception& e) {
        failed = true;
        wxMessageDialog messageDialog(this, e.what(), "Attention", wxOK | wxICON_ERROR | wxCENTRE);
        messageDialog.ShowModal();
    }

    if (!failed) {
        showFile(path, separator.value_or(L','), L'\0', quote.value_or(L'\"'));
    }
}

void MainFrame::showFile(wxString path, wxChar separator, wxChar escape, wxChar quote)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    {
        wxGridUpdateLocker gridUpdateLocker(mGrid);

        if (path != mPath) {
            BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
            auto oldPath { mPath };
            mPath = path;
            mThreadStatus = ThreadStatus::InProgress;
            mIsCancelled = false;
            mPercent = 0;
            mErrorMessage = "";

            auto threadStatus { mThreadStatus };
            auto isCancelled { false };
            int percent { 0 };

            wxProgressDialog progressDialog("Scanning file", path, 100, this,
                wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_REMAINING_TIME | wxPD_CAN_ABORT);
            progressDialog.Update(0);

            BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
            auto threadError = CreateThread(wxTHREAD_JOINABLE);
            wxASSERT(threadError == wxTHREAD_NO_ERROR);
            threadError = GetThread()->Run();
            wxASSERT(threadError == wxTHREAD_NO_ERROR);
            BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

            while (true) {
                {
                    wxCriticalSectionLocker lock(mPercentCS);
                    percent = mPercent;
                }

                if (!progressDialog.Update(percent)) {
                    // Cancelled by user
                    isCancelled = true;
                    BOOST_LOG_SEV(gLogger, bltrivial::trace) << "isCancelled=" << isCancelled << FUNCTION_FILE_LINE;
                    {
                        wxCriticalSectionLocker lock(mIsCancelledCS);
                        mIsCancelled = true;
                    }
                }

                {
                    wxCriticalSectionLocker lock(mThreadStatusCS);
                    threadStatus = mThreadStatus;
                }

                if (threadStatus == ThreadStatus::Finished || threadStatus == ThreadStatus::Failed || isCancelled) {
                    BOOST_LOG_SEV(gLogger, bltrivial::trace)
                        << "threadStatus=" << int(threadStatus) << ", isCancelled=" << isCancelled << FUNCTION_FILE_LINE;
                    auto exitCode = GetThread()->Wait(wxTHREAD_WAIT_BLOCK);
                    wxASSERT(exitCode == static_cast<wxThread::ExitCode>(0));
                    break;
                } else {
                    wxThread::Sleep(100);
                }
            }

            BOOST_LOG_SEV(gLogger, bltrivial::trace)
                << "threadStatus=" << int(threadStatus) << ", isCancelled=" << isCancelled << FUNCTION_FILE_LINE;
            if (isCancelled) {
                BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
                mPath = oldPath;
                mGridTableNew.reset(nullptr);
                return;
            } else if (threadStatus == ThreadStatus::Finished) {
                BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
                wxASSERT(mGridTableNew);
                mGridTableNew->setTokenizerParams(escape, separator, quote);
                mGrid->SetGridCursor(-1, -1);
                mGrid->SetTable(mGridTableNew.get());
                SetTitle(mGridTableNew->getTitle() + App::kAppName);
                mGridTable = std::move(mGridTableNew);
            } else if (threadStatus == ThreadStatus::Failed) {
                wxMessageDialog messageDialog(this, mErrorMessage, "Attention", wxOK | wxICON_WARNING | wxCENTRE);
                messageDialog.ShowModal();
            }
        } else if (separator != mSeparator || quote != mQuote || escape != mEscape) {
            BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
            mGrid->SetGridCursor(-1, -1);
            mGrid->ClearGrid();
            mGrid->SetTable(nullptr);
            mGridTable->setTokenizerParams(escape, separator, quote);
            mGrid->SetTable(mGridTable.get());
        } else {
            BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
            return;
        }

        mGrid->GoToCell(0, 0);
        SetStatusText(mGridTable->getStatusText());
        BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
    }

    mSeparator = separator;
    mQuote = quote;
    mEscape = escape;
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
}

wxThread::ExitCode MainFrame::Entry()
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
    try {
        mGridTableNew = std::make_unique<CsvFileGridTable>(
            bfs::path(mPath), std::bind(&MainFrame::OnProgress, this, std::placeholders::_1), std::bind(&MainFrame::IsCancelled, this));
        BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
        {
            wxCriticalSectionLocker lock(mThreadStatusCS);
            mThreadStatus = ThreadStatus::Finished;
        }
    } catch (const std::runtime_error& e) {
        BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
        mGridTableNew = std::make_unique<EmptyGridTable>();
        mErrorMessage = e.what();
        {
            wxCriticalSectionLocker lock(mThreadStatusCS);
            mThreadStatus = ThreadStatus::Failed;
        }
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

bool MainFrame::IsCancelled()
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
    auto isCancelled { false };
    {
        wxCriticalSectionLocker lock(mIsCancelledCS);
        isCancelled = mIsCancelled;
    }
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << "isCancelled=" << isCancelled << FUNCTION_FILE_LINE;
    return isCancelled;
}

void MainFrame::OnDropFiles(wxThreadEvent& event)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
    showFile(event.GetString());
}

bool MainFrame::isReadyForDragDrop()
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
    ThreadStatus threadStatus;
    {
        wxCriticalSectionLocker lock(mThreadStatusCS);
        threadStatus = mThreadStatus;
    }
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << "threadStatus=" << int(threadStatus) << FUNCTION_FILE_LINE;
    return threadStatus != ThreadStatus::InProgress;
}

void MainFrame::OnGoToColumn(wxCommandEvent& event)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    GoToColumnDialog goToColumnDialog(this, this->mGridTable.get());
    if (goToColumnDialog.ShowModal() != wxID_CANCEL) {
    }
}
void MainFrame::OnGoToRow(wxCommandEvent& event) {}

FileDropTarget::FileDropTarget(MainFrame* frame)
    : mFrame(frame)
{
}

bool FileDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
    wxASSERT(filenames.GetCount());
    wxThreadEvent event(wxEVT_THREAD, ID_ON_DROP_FILES);
    event.SetString(filenames[0]);
    wxQueueEvent(mFrame, event.Clone());
    return true;
}

wxDragResult FileDropTarget::OnDragOver(wxCoord x, wxCoord y, wxDragResult defResult)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
    if (mFrame->isReadyForDragDrop()) {
        BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
        return wxDragCopy;
    } else {
        BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
        return wxDragNone;
    };
}
