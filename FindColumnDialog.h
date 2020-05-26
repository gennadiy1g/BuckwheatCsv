#pragma once

#include <wx/dataview.h>
#include <wx/srchctrl.h>
#include <wx/timer.h>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "GridTable.h"

class FindColumnDialog : public wxDialog {
public:
    explicit FindColumnDialog(wxWindow* parent, GridTableBase* gridTable);
    virtual ~FindColumnDialog() = default;

    // Disallow assignment and pass-by-value.
    FindColumnDialog(const FindColumnDialog& src) = delete;
    FindColumnDialog& operator=(const FindColumnDialog& rhs) = delete;

    // Explicitly default move constructor and move assignment operator.
    FindColumnDialog(FindColumnDialog&& src) = default;
    FindColumnDialog& operator=(FindColumnDialog&& rhs) = default;

private:
    void populateColumnsListCtrl(const wxString& partOfName);
    void OnSearch(wxCommandEvent& event);
    void OnTextChange(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnDataViewItemActivated(wxDataViewEvent& event);

    wxSearchCtrl* mSearchCtrl { nullptr };
    wxDataViewListCtrl* mDataViewList { nullptr };
    GridTableBase* mGridTable { nullptr };
    wxTimer mTimer;
};
