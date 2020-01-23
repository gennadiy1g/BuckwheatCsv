#include <sstream>

#include "CsvTable/log.h"
#include "GridTable.h"

CsvFileGridTable::CsvFileGridTable(const bfs::path& filePath, OnProgress onProgress)
    : mTokenizedFileLines(filePath, onProgress)
    , mFilePath(filePath)
{
    if (!sLocale) {
        sLocale = std::make_unique<std::locale>(std::locale("C"), new thousand_sep_numpunct());
    }
}

wxString CsvFileGridTable::GetValue(int row, int col)
{
    if (row != mRow) {
        if (mHeadersInFirstRow) {
            mTokenizedFileLine = &mTokenizedFileLines.getTokenizedLine(row + 1);
        } else {
            mTokenizedFileLine = &mTokenizedFileLines.getTokenizedLine(row);
        }
        mRow = row;
    }
    return col < GetNumberCols() ? mTokenizedFileLine->at(col) : wxEmptyString;
}

wxString CsvFileGridTable::GetColLabelValue(int col)
{
    if (mHeadersInFirstRow) {
        return mTokenizedFileLines.getTokenizedLine(0).at(col);
    } else {
        return wxGridTableBase::GetColLabelValue(col);
    }
}

int CsvFileGridTable::GetNumberRows()
{
    if (mHeadersInFirstRow) {
        return mTokenizedFileLines.numLines() - 1;
    } else {
        return mTokenizedFileLines.numLines();
    }
}

wxString CsvFileGridTable::getStatusText()
{
    std::ostringstream stringStream;
    stringStream.imbue(*sLocale);
    stringStream << "Rows: " << GetNumberRows() << "  Columns: " << GetNumberCols();
    return stringStream.str();
}

wxString CsvFileGridTable::getTitle()
{
    std::ostringstream stringStream;
    stringStream << mFilePath.filename().string() << " - " << mFilePath.parent_path().string() << " - ";
    return stringStream.str();
}

void CsvFileGridTable::Clear()
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
    mTokenizedFileLines.clear();
    mRow = -1;
    mTokenizedFileLine = nullptr;
}

void CsvFileGridTable::setTokenizerParams(wchar_t escape, wchar_t separator, wchar_t quote)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
    mTokenizedFileLines.setTokenizerParams(escape, separator, quote);
}
