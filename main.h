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
    int mSeparatorId { ID_Comma };
    wxChar mSeparator { L'\0' };
    int mEscapeId { ID_Backslash };
    wxChar mEscape { L'\0' };
    int mQuoteId { ID_Double };
    wxChar mQuote { L'\0' };

    wxGrid* mGrid { nullptr };
    wxGridTableBase* mGridTable { nullptr };
};
