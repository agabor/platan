#include "statement.h"


QVariant Statement::at(int idx) const
{
    switch (idx)
    {
    case 0:
        return Date;
    case 1:
        return Type;
    case 2:
        return Description;
    case 3:
        return Payee;
    case 4:
        return PayeeAccount;
    case 5:
        return Amount;
    }
    return 0;
}
