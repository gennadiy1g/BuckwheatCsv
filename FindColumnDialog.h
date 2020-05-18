#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class FindColumnDialog : public wxDialog {
public:
    explicit FindColumnDialog(wxWindow* parent);
    virtual ~FindColumnDialog() = default;

    // Disallow assignment and pass-by-value.
    FindColumnDialog(const FindColumnDialog& src) = delete;
    FindColumnDialog& operator=(const FindColumnDialog& rhs) = delete;

    // Explicitly default move constructor and move assignment operator.
    FindColumnDialog(FindColumnDialog&& src) = default;
    FindColumnDialog& operator=(FindColumnDialog&& rhs) = default;
};
