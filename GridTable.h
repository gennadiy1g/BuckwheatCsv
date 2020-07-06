#pragma once

#include <locale>
#include <memory>

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

    virtual void setTokenizerParams(wchar_t escape, wchar_t separator, wchar_t quote) {};
    virtual wxString getStatusText() { return ""; };
    virtual wxString getTitle() const { return ""; };
    void SetValue(int, int, const wxString&) override {};

    /* The default implementation of this function makes the applicaiton very slow: it takes a long time to jump between the
      first and the last rows of the grid using Ctrl-Up/Ctrl-Down keyboard shorcuts. The implementatio below makes the applicaiton
      very fast, but it does not allow the applicaiton to search for empty cells. */
    bool IsEmptyCell(int, int) override { return false; };

    virtual bool isNumLinesLimitReached() const { return false; }
    virtual bool isCancelled() const { return false; };

    static inline std::unique_ptr<std::locale> sLocale { nullptr };
};

class EmptyGridTable : public GridTableBase {
public:
    EmptyGridTable() {};
    virtual ~EmptyGridTable() = default; // Defaulted virtual destructor

    int GetNumberRows() override { return 100; }
    int GetNumberCols() override { return 26; }
    wxString GetValue(int row, int col) override { return L""; }
};

class CsvFileGridTable : public GridTableBase {
public:
    explicit CsvFileGridTable(const bfs::path& filePath, OnProgress onProgress = OnProgress(), IsCancelled isCancelled = IsCancelled());
    CsvFileGridTable(const bfs::path& filePath, std::size_t linesToPreview);
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
    virtual wxString GetColLabelValue(int col) override;
    virtual void Clear() override;

    virtual void setTokenizerParams(wchar_t escape, wchar_t separator, wchar_t quote) override;
    virtual wxString getStatusText() override;
    virtual wxString getTitle() const override;

    virtual bool isNumLinesLimitReached() const override { return mTokenizedFileLines.isNumLinesLimitReached(); }
    virtual bool isCancelled() const { return mTokenizedFileLines.isCancelled(); };

private:
    TokenizedFileLines mTokenizedFileLines;

    // The intent of having "the last tokenized line" is to avoid tokenizing the same line multiple times
    int mRow { -1 }; // row number of the last tokenized line
    const std::vector<std::wstring>* mTokenizedFileLine { nullptr }; // pointer to the last tokenized line

    bool mHeadersInFirstRow { true };
    bfs::path mFilePath;
};

struct thousand_sep_numpunct : std::numpunct<char> {
    char do_thousands_sep() const { return ','; } // separate with comma
    std::string do_grouping() const { return "\3"; } // groups of 3 digit
};
