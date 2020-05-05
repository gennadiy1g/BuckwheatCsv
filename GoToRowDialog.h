#pragma once

#include <string>

#include <wx/filepicker.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/textctrl.h>

#include "GridTable.h"

class GoToRowDialog : public wxDialog {
public:
    GoToRowDialog(wxWindow* parent, GridTableBase* gridTable);
    virtual ~GoToRowDialog() = default;

    // Disallow assignment and pass-by-value.
    GoToRowDialog(const GoToRowDialog& src) = delete;
    GoToRowDialog& operator=(const GoToRowDialog& rhs) = delete;

    // Explicitly default move constructor and move assignment operator.
    GoToRowDialog(GoToRowDialog&& src) = default;
    GoToRowDialog& operator=(GoToRowDialog&& rhs) = default;

    int getRow() const { return mRow; }

private:
    wxTextCtrl* mTextCtrl { nullptr };
    unsigned int mRow { 0 };
};
