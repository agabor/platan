// This file is part of Platan.
// Copyright (C) 2014 Gábor Angyal
//
// Platan is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Platan is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Platan.  If not, see <http://www.gnu.org/licenses/>.

#include "statements.h"

using namespace std;

std::vector<StatementRow> Statements::GetStatements()
{
    return statements;
}

std::vector<StatementExtractRow> Statements::GetStatementsForClass(int class_idx)
{
  std::vector<StatementExtractRow> result;

  for(const StatementRow &row : statements)
  {
      if (row.Class == class_idx)
      {
          StatementExtractRow extract_row;
          extract_row.Ammount = row.Ammount;
          extract_row.Date = row.Date;
          extract_row.Payee = row.Payee;
          result.push_back(extract_row);
      }
  }

  return result;
}


void Statements::SetTimeInterval(QDate start_date, QDate end_date)
{
    data_base.SetTimeInterval(start_date, end_date);
}


void Statements::UnsetTimeInterval()
{
    data_base.UnsetTimeInterval();
}


void Statements::GetCalssification(QMap<int, float> &result)
{
    data_base.GetCalssification(result);
}


void Statements::SetPath(string data_base_path)
{
    data_base.SetPath(data_base_path);
    data_base.ReadData(statements);
}


void Statements::GetClasses(QString lan, QMap<int, QString> &classes)
{
    data_base.GetClasses(lan, classes);
}
