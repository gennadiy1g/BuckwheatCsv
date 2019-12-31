#pragma once

#include <memory>

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/grid.h>
#include <wx/log.h>

class App : public wxApp {
public:
    virtual bool OnInit();

private:
    std::unique_ptr<wxLogWindow> mLogWindow;
};

class MainFrame : public wxFrame {
public:
    MainFrame();

private:
    void OnOpen(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    wxString mPath { wxEmptyString };
    wxChar mSeparator { L',' };
    wxChar mEscape { L'\\' };
    wxChar mQuote { L'"' };

    std::unique_ptr<TokenizedFileLines> mTokenizedFileLines { nullptr };

    wxGrid* mGrid { nullptr };
    wxGridTableBase* mGridTable { nullptr };
};
