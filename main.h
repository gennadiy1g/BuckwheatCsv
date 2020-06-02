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

enum EventID {
    ID_ON_DROP_FILES = wxID_HIGHEST + 1,
    ID_GOTO_ROW,
    ID_GOTO_COLUMN,
    ID_FIND_COLUMN,
    ID_AUTOSIZE_COLUMNS_FIT,
    ID_AUTOSIZE_COLUMNS_DEFAULT
};

enum class ThreadStatus { NotStarted, InProgress, Failed, Finished };

class App : public wxApp {
public:
    virtual bool OnInit();
    virtual bool OnExceptionInMainLoop();

    static inline const char* kAppName { "Buckwheat CSV" };
};

class MainFrame : public wxFrame, public wxThreadHelper {
public:
    MainFrame();
    virtual ~MainFrame();
    virtual wxThread::ExitCode Entry();
    bool isReadyForDragDrop();

    void showFile(wxString path);
    void showFile(wxString path, wxChar separator, wxChar escape, wxChar quote);

private:
    void OnOpen(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnProgress(int percent);
    bool IsCancelled();
    void OnDropFiles(wxThreadEvent& event);
    void OnGoToColumn(wxCommandEvent& event);
    void OnFindColumn(wxCommandEvent& event);
    void OnGoToRow(wxCommandEvent& event);

    wxString mPath { wxEmptyString };
    wxChar mSeparator { L',' };
    wxChar mEscape { L'\0' };
    wxChar mQuote { L'\"' };

    wxGrid* mGrid { nullptr };
    std::unique_ptr<GridTableBase> mGridTable { nullptr };
    std::unique_ptr<GridTableBase> mGridTableNew { nullptr };

    wxCriticalSection mPercentCS;
    int mPercent { 0 };

    wxCriticalSection mIsCancelledCS;
    bool mIsCancelled { false };

    wxCriticalSection mThreadStatusCS;
    ThreadStatus mThreadStatus { ThreadStatus::NotStarted };

    std::string mErrorMessage {};
};

class FileDropTarget : public wxFileDropTarget {
public:
    explicit FileDropTarget(MainFrame* frame);
    virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) override;
    virtual wxDragResult OnDragOver(wxCoord x, wxCoord y, wxDragResult defResult) override;

private:
    MainFrame* mFrame;
};
