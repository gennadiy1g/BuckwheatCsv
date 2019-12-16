#include "GridTable.h"

CsvFileGridTable::CsvFileGridTable(const bfs::path& filePath)
    : mTokenizedFileLines(filePath)
{
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
    return mTokenizedFileLine->at(col);
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
