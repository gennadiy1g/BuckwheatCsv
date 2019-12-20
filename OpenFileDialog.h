#pragma once

#include <wx/filepicker.h>
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

enum {
    ID_Comma = wxID_HIGHEST + 1,
    ID_Tab,
    ID_Semicolon,
    ID_VerticalBar,
    ID_Space,
    ID_Other
};

class OpenFileDialog : public wxDialog {
public:
    OpenFileDialog(wxWindow* parent); // Constructor
    virtual ~OpenFileDialog() = default; // Defaulted virtual destructor

    // Disallow assignment and pass-by-value.
    OpenFileDialog(const OpenFileDialog& src) = delete;
    OpenFileDialog& operator=(const OpenFileDialog& rhs) = delete;

    // Explicitly default move constructor and move assignment operator.
    OpenFileDialog(OpenFileDialog&& src) = default;
    OpenFileDialog& operator=(OpenFileDialog&& rhs) = default;

    wxString GetPath() const { return mFilePickerCtrl->GetPath(); };

private:
    void OnRadioButton(wxCommandEvent& event);

    wxFilePickerCtrl* mFilePickerCtrl { nullptr };
    int mSeparatorID { ID_Comma };
    wxTextCtrl* mTextCtrl { nullptr };
};

class FilePathValidator : public wxValidator {
public:
    virtual bool Validate(wxWindow* parent) override;
    virtual bool TransferToWindow() override { return true; };
    virtual bool TransferFromWindow() override { return true; };
    virtual wxObject* Clone() const override { return new FilePathValidator(*this); }
};
