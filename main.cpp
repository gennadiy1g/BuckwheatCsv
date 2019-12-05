#include <boost/locale/localization_backend.hpp>
#include <numeric>

#include "App.h"
#include "CsvTable/log.h"
#include "CsvTable/utilities.h"
#include "GridTable.h"

namespace blocale = boost::locale;

using namespace std::literals::string_literals;

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    initLocalization();
    initLogging();

    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << "->" << FUNCTION_FILE_LINE;

    auto backends = blocale::localization_backend_manager::global().get_all_backends();
    std::string backendsList = std::accumulate(backends.cbegin(), backends.cend(), ""s,
        [](const std::string& a, const std::string& b) { return a + (a == "" ? "" : ", ") + b; });
    BOOST_LOG_SEV(gLogger, bltrivial::debug) << "Localization backends: " << backendsList << '.';

    MainFrame* frame = new MainFrame();
    frame->Show(true);
    return true;
}

MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
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
    mGridTable = new EmptyGridTable();
    mGrid->SetTable(mGridTable, true);

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &MainFrame::OnOpen, this, wxID_OPEN);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnOpen(wxCommandEvent& event)
{
    mGridTable = new CsvFileGridTable();
    mGrid->SetTable(mGridTable, true);
}
