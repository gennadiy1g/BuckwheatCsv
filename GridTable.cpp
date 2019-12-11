#include "GridTable.h"

CsvFileGridTable::CsvFileGridTable(const bfs::path& filePath)
{
    mTokenizedFileLines = std::make_unique<TokenizedFileLines>(filePath);
}

wxString CsvFileGridTable::GetValue(int row, int col)
{
    if (row != mRow) {
        mTokenizedFileLine = &mTokenizedFileLines->getTokenizedLine(row);
        mRow = row;
    }
    return mTokenizedFileLine->at(col);
}

wxString CsvFileGridTable::GetColLabelValue(int col)
{
    return wxGridTableBase::GetColLabelValue(col);
}
