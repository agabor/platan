#ifndef XLSDOCUMENT_H
#define XLSDOCUMENT_H

struct XLSCell
{
    int type;
    char *str;
    double d;
    long l;
};

class XLSRow
{
public:
    XLSRow(int cellCount);
    XLSCell *getCell(int idx) const;
    int getCellCount() const;
    void setCell(int idx, int id, char* str, double d, long l);
private:
    XLSCell* cells;
    int cellCount;
};

class XLSSheet
{
public:
    XLSSheet(char *name, int r, int c);
    XLSRow *getRow(int idx) const;
    int getRowCount() const;
    const char *getName() const;
private:
    XLSRow* rows;
    int rowCount;
    char* name;
};

class XLSDocument
{
public:
    XLSDocument(const char* file_name);
    const XLSSheet *getSheet(int idx) const;
    int getSheetCount() const;
private:
    XLSSheet* sheets;
    int sheetCount;
};

#endif // XLSDOCUMENT_H
