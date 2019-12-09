#pragma once

#include <memory>

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/grid.h>

#include "CsvTable/CsvTable.h"

class EmptyGridTable : public wxGridTableBase {
public:
    EmptyGridTable() {};
    virtual ~EmptyGridTable() = default; // Defaulted virtual destructor

    int GetNumberRows() override
    {
        return 100;
    }

    int GetNumberCols() override
    {
        return 26;
    }

    wxString GetValue(int row, int col) override
    {
        return L"";
    }

    void SetValue(int, int, const wxString&) override
    { /* ignore */
    }

    bool IsEmptyCell(int, int) override
    {
        return false;
    }
};

class CsvFileGridTable : public wxGridTableBase {
public:
    CsvFileGridTable();

    virtual ~CsvFileGridTable() = default; // Defaulted virtual destructor

    // Disallow assignment and pass-by-value.
    CsvFileGridTable(const CsvFileGridTable& src) = delete;
    CsvFileGridTable& operator=(const CsvFileGridTable& rhs) = delete;

    // Explicitly default move constructor and move assignment operator.
    CsvFileGridTable(CsvFileGridTable&& src) = default;
    CsvFileGridTable& operator=(CsvFileGridTable&& rhs) = default;

    int GetNumberRows() override
    {
        return mTokenizedFileLines->numLines();
    }

    int GetNumberCols() override
    {
        return mTokenizedFileLines->numColumns();
    }

    wxString GetValue(int row, int col) override;

    void SetValue(int, int, const wxString&) override
    { /* ignore */
    }

    bool IsEmptyCell(int, int) override
    {
        return false;
    }

private:
    std::unique_ptr<TokenizedFileLines> mTokenizedFileLines;
    int mRow { -1 };
    const std::vector<std::wstring>* mTokenizedFileLine;
};
