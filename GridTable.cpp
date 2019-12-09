#include "GridTable.h"

CsvFileGridTable::CsvFileGridTable()
{
    mTokenizedFileLines = std::make_unique<TokenizedFileLines>(LR"^(C:\Users\genna_000\Documents\BuckwheatCsv\test data\Hits.csv)^");
    mTokenizedFileLines->getPositionsOfSampleLines();
}

wxString CsvFileGridTable::GetValue(int row, int col)
{
    if (row != mRow) {
        mTokenizedFileLine = &mTokenizedFileLines->getTokenizedLine(row);
        mRow = row;
    }
    return mTokenizedFileLine->at(col);
}
