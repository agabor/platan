#include "tabletransformer.h"
#include <statementtablemodel.h>
#include <csvtablemodel.h>
#include <vector>

TableTransformer::TableTransformer()
{
}

StatementTableModel *TableTransformer::transform(CSVTableModel *model) const
{
    std::vector<StatementRow> rows;
    for(int r = 0; r < model->rowCount(); ++r)
    {
        StatementRow row;
        row.Ammount = Amount.apply(model, r);
        row.Type = Type.apply(model, r);
        row.Date = Date.apply(model, r);
        row.Payee = Payee.apply(model, r);
        row.PayeeAccount = PayeeAccount.apply(model, r);
        row.Description = Description.apply(model, r);
        rows.push_back(row);
    }
    return new StatementTableModel(rows);
}
