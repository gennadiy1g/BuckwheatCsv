#pragma once

#include <memory>

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/grid.h>
#include <wx/log.h>
#include <wx/progdlg.h>
#include <wx/thread.h>

class App : public wxApp {
public:
    virtual bool OnInit();

private:
    std::unique_ptr<wxLogWindow> mLogWindow;
};

class MainFrame : public wxFrame, public wxThreadHelper {
public:
    MainFrame();
    virtual wxThread::ExitCode Entry();

private:
    void OnOpen(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnProgress(int percent);

    wxString mPath { wxEmptyString };
    wxChar mSeparator { L',' };
    wxChar mEscape { L'\\' };
    wxChar mQuote { L'"' };
    std::unique_ptr<TokenizedFileLines> mTokenizedFileLines { nullptr };

    wxGrid* mGrid { nullptr };
    wxGridTableBase* mGridTable { nullptr };

    wxCriticalSection mPercentCriticalSection;
    int mPercent { 0 };
};
