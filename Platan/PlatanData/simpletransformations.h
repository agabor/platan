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

#ifndef SIMPLETRANSFORMATIONS_H
#define SIMPLETRANSFORMATIONS_H

#include "transformation.h"
class QString;

class IdentityTransFormation : public Transformation<QString>
{
public:
    IdentityTransFormation();
    QString convert(QString Data) const override;
	QString getErrorMessage() const override;
};

class IntTransformation : public Transformation<int>
{
public:
    IntTransformation();
	int convert(QString data) const override;
	QString getErrorMessage() const override;
};

class FloatTransformation : public Transformation<float>
{
public:
    FloatTransformation();

	float convert(QString data) const override;
	QString getErrorMessage() const override;
};


#endif // SIMPLETRANSFORMATIONS_H

