#include "jsapi.h"

JSAPI::JSAPI(ImportPlugin *parent) : JSObject(parent)
{

}

void JSAPI::addStatement(float amount, QString type, QJSValue date, QString payee, QString payeeAccount, QString description)
{
  Statement s;
  s.setAmount(amount);

  if (s.isAmountNaN())
    return;

  s.setType(type);
  int y = date.property("y").toInt();
  int m = date.property("m").toInt();
  int d = date.property("d").toInt();
  s.setDate(SQLiteDate(y, m, d));
  s.setPayee(payee);
  s.setPayeeAccount(payeeAccount);
  s.setDescription(description);
  s.setCategory(0);
  ((ImportPlugin*)m_plugin)->addStatement(s);
}

