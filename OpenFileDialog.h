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

    wxString GetPath() const { return mPath; };
    wxChar GetSeparator() const;

private:
    void OnRadioButton(wxCommandEvent& event);

    wxTextCtrl* mTextCtrl { nullptr };

    int mSeparatorId { ID_Comma };
    wxString mSeparator { wxEmptyString };
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

class SeparatorIdValidator : public wxValidator {
public:
    explicit SeparatorIdValidator(int& separatorId);
    SeparatorIdValidator(const SeparatorIdValidator& src) = default;
    virtual bool Validate(wxWindow* parent) override { return true; };
    virtual bool TransferToWindow() override;
    virtual bool TransferFromWindow() override;
    virtual wxObject* Clone() const override { return new SeparatorIdValidator(*this); }

private:
    int& mSeparatorId;
};

class SeparatorValidator : public wxValidator {
public:
    explicit SeparatorValidator(const int& separatorId, wxString& separator);
    SeparatorValidator(const SeparatorValidator& src) = default;
    virtual bool Validate(wxWindow* parent) override;
    virtual bool TransferToWindow() override;
    virtual bool TransferFromWindow() override;
    virtual wxObject* Clone() const override { return new SeparatorValidator(*this); }

private:
    const int& mSeparatorId;
    wxString& mSeparator;
};
