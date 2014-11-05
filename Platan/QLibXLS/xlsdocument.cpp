#include <cstring>
#include <new>

extern "C"{
#include <libxls/xls.h>
}

#include "xlsdocument.h"


xlsWorkBook * xls_open(const char *file_name);

XLSDocument::XLSDocument(const char *file_name)
{
    xlsWorkBook* pWB = xls_open(file_name);
    sheetCount = pWB->sheets.count;
    sheets = static_cast<XLSSheet*>(::operator new (sizeof(XLSSheet)*sheetCount));
    for (int i = 0; i < sheetCount; ++i)
    {
        auto pWS=xls_getWorkSheet(pWB,i);
        xls_parseWorkSheet(pWS);
        auto rows = pWS->rows;
        char * name = pWB->sheets.sheet[i].name;
        auto sheet = new (&sheets[i])XLSSheet(name, rows.lastrow+1, rows.lastcol);
        for (int r = 0; r <= rows.lastrow; ++r)
        {
            auto row = sheet->getRow(r);
            for (int c = 0; c < rows.lastcol; ++c)
            {
                auto cell = rows.row[r].cells.cell[c];
                row->setCell(c, cell.id, cell.str, cell.d, cell.l);
            }
        }
    }

    xls_close(pWB);
}

xlsWorkBook * xls_open(const char *file_name)
{
    auto size = strlen(file_name)+1;//+1 is the null terminator
    char *mut_fn = new char[size];
    strcpy_s(mut_fn, size, file_name);

    const char* encod = "iso-8859-15//TRANSLIT";
    size = strlen(encod)+1;//+1 is the null terminator
    char* mut_encod = new char[size];
    strcpy_s(mut_encod, size, encod);

    xlsWorkBook* pWB = xls_open(mut_fn, mut_encod);

    delete [] mut_fn;
    delete [] mut_encod;

    return pWB;
}

const XLSSheet *XLSDocument::getSheet(int idx) const
{
    if (idx < 0 || idx >= sheetCount)
        return nullptr;
    return &sheets[idx];
}

int XLSDocument::getSheetCount() const
{
    return sheetCount;
}


XLSSheet::XLSSheet(char *name, int r, int c)
{
    auto size = strlen(name)+1;//+1 is the null terminator
    this->name = new char[size];
    strcpy_s(this->name, size, name);
    rowCount = r;
    rows = static_cast<XLSRow*>(::operator new (sizeof(XLSRow) * r));
    for (int i = 0; i < r; ++i)
        new(&rows[i]) XLSRow(c);
}

XLSRow *XLSSheet::getRow(int idx) const
{
    if (idx < 0 || idx >= rowCount)
        return nullptr;
    return &rows[idx];
}

int XLSSheet::getRowCount() const
{
    return rowCount;
}

const char *XLSSheet::getName() const
{
    return name;
}


XLSRow::XLSRow(int cellCount)
{
    this->cellCount = cellCount;
    cells = new XLSCell[cellCount];
}

XLSCell *XLSRow::getCell(int idx) const
{
    if (idx < 0 || idx >= cellCount)
        return nullptr;
    return &cells[idx];
}

int XLSRow::getCellCount() const
{
    return cellCount;
}

void XLSRow::setCell(int idx, int id, char *str, double d, long l)
{
    XLSCell &c = cells[idx];
    c.type = id;
    c.str = str;
    c.d = d;
    c.l = l;
}
