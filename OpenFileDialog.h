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
    ID_OtherSeparator,

    ID_Backslash,
    ID_OtherEscape
};

class OpenFileDialog : public wxDialog {
public:
    explicit OpenFileDialog(wxWindow* parent); // Constructor
    virtual ~OpenFileDialog() = default; // Defaulted virtual destructor

    // Disallow assignment and pass-by-value.
    OpenFileDialog(const OpenFileDialog& src) = delete;
    OpenFileDialog& operator=(const OpenFileDialog& rhs) = delete;

    // Explicitly default move constructor and move assignment operator.
    OpenFileDialog(OpenFileDialog&& src) = default;
    OpenFileDialog& operator=(OpenFileDialog&& rhs) = default;

    wxString GetPath() const { return mPath; };
    wxChar GetSeparator() const;

private:
    void OnSeparatorRadioButton(wxCommandEvent& event);
    void OnEscapeRadioButton(wxCommandEvent& event);

    wxTextCtrl* mSeparatorTextCtrl { nullptr };
    wxTextCtrl* mEscapeTextCtrl { nullptr };

    int mSeparatorId { ID_Comma };
    wxChar mSeparator { L'\0' };
    wxString mPath { wxEmptyString };
};

class FilePathValidator : public wxValidator {
public:
    explicit FilePathValidator(wxString& path);
    FilePathValidator(const FilePathValidator& src) = default;
    virtual bool Validate(wxWindow* parent) override;
    virtual bool TransferToWindow() override;
    virtual bool TransferFromWindow() override;
    virtual wxObject* Clone() const override { return new FilePathValidator(*this); }

private:
    wxString& mPath;
};

class RadioButtonValidator : public wxValidator {
public:
    explicit RadioButtonValidator(int& separatorId);
    RadioButtonValidator(const RadioButtonValidator& src) = default;
    virtual bool Validate(wxWindow* parent) override { return true; };
    virtual bool TransferToWindow() override;
    virtual bool TransferFromWindow() override;
    virtual wxObject* Clone() const override { return new RadioButtonValidator(*this); }

private:
    int& mRadioButtonId;
};

class TextCtrlValidator : public wxValidator {
public:
    explicit TextCtrlValidator(const int& separatorId, wxChar& separator);
    TextCtrlValidator(const TextCtrlValidator& src) = default;
    virtual bool Validate(wxWindow* parent) override;
    virtual bool TransferToWindow() override;
    virtual bool TransferFromWindow() override;
    virtual wxObject* Clone() const override { return new TextCtrlValidator(*this); }

private:
    const int& mRadioButtonId;
    wxChar& mChar;
};
