#include "GridTable.h"

CsvFileGridTable::CsvFileGridTable()
{
    mTokenizedFileLines = std::make_unique<TokenizedFileLines>(LR"^(C:\Users\genna_000\Documents\BuckwheatCsv\test data\Hits.csv)^");
    mTokenizedFileLines->getPositionsOfSampleLines();
}

wxString CsvFileGridTable::GetValue(int row, int col)
{
    if (row != mPrevRow) {
        mPrevTokenizedFileLine = &mTokenizedFileLines->getTokenizedLine(row);
        mPrevRow = row;
    }
    return mPrevTokenizedFileLine->at(col);
}
