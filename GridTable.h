#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/grid.h>

#include "CsvTable/CsvTable.h"

class GridTableBase : public wxGridTableBase {
public:
    GridTableBase() {};
    virtual ~GridTableBase() = default; // Defaulted virtual destructor

    void setTokenizerParams(wchar_t escape, wchar_t fieldSeparator, wchar_t quote) {};
};

class EmptyGridTable : public GridTableBase {
public:
    EmptyGridTable() {};
    virtual ~EmptyGridTable() = default; // Defaulted virtual destructor

    int GetNumberRows() override { return 100; }
    int GetNumberCols() override { return 26; }
    wxString GetValue(int row, int col) override { return L""; }
    void SetValue(int, int, const wxString&) override {}
    bool IsEmptyCell(int, int) override { return false; }
    virtual void Clear() override {};
};

class CsvFileGridTable : public GridTableBase {
public:
    explicit CsvFileGridTable(const bfs::path& filePath, OnProgress onProgress = OnProgress());
    virtual ~CsvFileGridTable() = default; // Defaulted virtual destructor

    // Disallow assignment and pass-by-value.
    CsvFileGridTable(const CsvFileGridTable& src) = delete;
    CsvFileGridTable& operator=(const CsvFileGridTable& rhs) = delete;

    // Explicitly default move constructor and move assignment operator.
    CsvFileGridTable(CsvFileGridTable&& src) = default;
    CsvFileGridTable& operator=(CsvFileGridTable&& rhs) = default;

    int GetNumberRows() override;
    int GetNumberCols() override { return mTokenizedFileLines.numColumns(); }
    wxString GetValue(int row, int col) override;
    void SetValue(int, int, const wxString&) override {}
    bool IsEmptyCell(int, int) override { return false; }
    virtual wxString GetColLabelValue(int col) override;
    virtual void Clear() override { mTokenizedFileLines.Clear(); };

    void setTokenizerParams(wchar_t escape = L'\\', wchar_t fieldSeparator = L',', wchar_t quote = L'\"');

private:
    TokenizedFileLines mTokenizedFileLines;
    int mRow { -1 };
    bool mHeadersInFirstRow { true };
    const std::vector<std::wstring>* mTokenizedFileLine { nullptr };
};
