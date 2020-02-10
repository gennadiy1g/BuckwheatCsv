#pragma once

#include <memory>

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/dnd.h>
#include <wx/grid.h>
#include <wx/progdlg.h>
#include <wx/thread.h>

enum EventID { ID_ON_DROP_FILES = wxID_HIGHEST + 1 };

class App : public wxApp {
public:
    virtual bool OnInit();
    virtual bool OnExceptionInMainLoop();

    static inline const char* kAppName { "Buckwheat CSV" };
};

class MainFrame : public wxFrame, public wxThreadHelper {
public:
    MainFrame();
    virtual wxThread::ExitCode Entry();
    bool isReadyForDragDrop();

    void showFile(wxString path);
    void showFile(wxString path, wxChar separator, wxChar escape, wxChar quote);

private:
    void OnOpen(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnProgress(int percent);
    void OnDropFiles(wxThreadEvent& event);

    wxString mPath { wxEmptyString };
    wxChar mSeparator { L',' };
    wxChar mEscape { L'\0' };
    wxChar mQuote { L'\"' };

    wxGrid* mGrid { nullptr };
    std::unique_ptr<GridTableBase> mGridTable { nullptr };
    std::unique_ptr<GridTableBase> mGridTableNew { nullptr };

    wxCriticalSection mPercentCS;
    int mPercent { 0 };
    wxCriticalSection mThreadIsDoneCS;
    int mThreadIsDone { true };

    bool mScanFailed { false };
    std::string mErrorMessage {};
};

class FileDropTarget : public wxFileDropTarget {
public:
    FileDropTarget(MainFrame* frame);
    virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) override;
    virtual wxDragResult OnDragOver(wxCoord x, wxCoord y, wxDragResult defResult) override;

private:
    MainFrame* mFrame;
};
