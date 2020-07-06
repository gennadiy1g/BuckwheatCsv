#include <sstream>

#include "CsvTable/log.h"
#include "GridTable.h"

CsvFileGridTable::CsvFileGridTable(const bfs::path& filePath, OnProgress onProgress, IsCancelled isCancelled)
    : mTokenizedFileLines(filePath, onProgress, isCancelled)
    , mFilePath(filePath)
{
    if (!sLocale) {
        sLocale = std::make_unique<std::locale>(std::locale("C"), new thousand_sep_numpunct());
    }
}

CsvFileGridTable::CsvFileGridTable(const bfs::path& filePath, std::size_t linesToPreview)
    : mTokenizedFileLines(filePath, linesToPreview)
    , mFilePath(filePath)
{
}

wxString CsvFileGridTable::GetValue(int row, int col)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << "row=" << row << ", col=" << col << FUNCTION_FILE_LINE;

    if (row != mRow) {
        BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
        if (mHeadersInFirstRow) {
            mTokenizedFileLine = &mTokenizedFileLines.getTokenizedLine(row + 1);
        } else {
            mTokenizedFileLine = &mTokenizedFileLines.getTokenizedLine(row);
        }
        mRow = row;
    }

    // A malformed line may contain less values than the number of column headers
    return col < static_cast<int>(mTokenizedFileLine->size()) ? mTokenizedFileLine->at(col) : wxEmptyString;
}

wxString CsvFileGridTable::GetColLabelValue(int col)
{
    wxASSERT(col < static_cast<int>(mTokenizedFileLines.getTokenizedLine(0).size()));
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
    if (!mTokenizedFileLines.isCancelled()) {
        stringStream << "Rows: " << (isNumLinesLimitReached() ? "more than " : "") << GetNumberRows();
    } else {
        stringStream << "Rows: top " << GetNumberRows() << " out of approximately " << mTokenizedFileLines.approxNumLines();
    };
    stringStream << "  Columns: " << GetNumberCols();
    return stringStream.str();
}

wxString CsvFileGridTable::getTitle() const
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
