#pragma once

#include <memory>

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/grid.h>
#include <wx/progdlg.h>
#include <wx/thread.h>

class App : public wxApp {
public:
    virtual bool OnInit();
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

    wxGrid* mGrid { nullptr };
    std::unique_ptr<wxGridTableBase> mGridTable { nullptr };
    std::unique_ptr<wxGridTableBase> mGridTable2 { nullptr };

    wxCriticalSection mPercentCS;
    int mPercent { 0 };
    wxCriticalSection mThreadIsDoneCS;
    int mThreadIsDone { false };
};
