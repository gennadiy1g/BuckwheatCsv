#pragma once

#include <wx/filepicker.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class GoToColumnDialog : public wxDialog {
public:
    GoToColumnDialog(wxWindow* parent);
    virtual ~GoToColumnDialog() = default;

    // Disallow assignment and pass-by-value.
    GoToColumnDialog(const GoToColumnDialog& src) = delete;
    GoToColumnDialog& operator=(const GoToColumnDialog& rhs) = delete;

    // Explicitly default move constructor and move assignment operator.
    GoToColumnDialog(GoToColumnDialog&& src) = default;
    GoToColumnDialog& operator=(GoToColumnDialog&& rhs) = default;
};
