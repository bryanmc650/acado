/*
 *    This file is part of ACADO Toolkit.
 *
 *    ACADO Toolkit -- A Toolkit for Automatic Control and Dynamic Optimization.
 *    Copyright (C) 2008-2013 by Boris Houska, Hans Joachim Ferreau,
 *    Milan Vukov, Rien Quirynen, KU Leuven.
 *    Developed within the Optimization in Engineering Center (OPTEC)
 *    under supervision of Moritz Diehl. All rights reserved.
 *
 *    ACADO Toolkit is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 3 of the License, or (at your option) any later version.
 *
 *    ACADO Toolkit is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with ACADO Toolkit; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

/**
 *    \file src/code_generation/export_simulink_interface.cpp
 *    \author Milan Vukov
 *    \date 2013
 */

#include <acado/code_generation/export_simulink_interface.hpp>
#include <acado/code_generation/templates/templates.hpp>

#include <sstream>

using namespace std;

BEGIN_NAMESPACE_ACADO


ExportSimulinkInterface::ExportSimulinkInterface(	const std::string& _makefileName,
													const std::string& _wrapperHeaderFileName,
													const std::string& _wrapperSourceFileName,
													const std::string& _moduleName,
													const std::string& _commonHeaderName,
													const std::string& _realString,
													const std::string& _intString,
													int _precision,
													const std::string& _commentString
													)
	: makefile(MAKEFILE_SFUN_QPOASES, _makefileName, "", _realString, _intString, _precision, "%"),
	  wrapperSource(SOLVER_SFUN_SOURCE, _wrapperSourceFileName, _commonHeaderName, _realString, _intString, _precision, _commentString),
	  wrapperHeader(SOLVER_SFUN_HEADER, _wrapperHeaderFileName, _commonHeaderName, _realString, _intString, _precision, _commentString),
	  moduleName( _moduleName )
{}


returnValue ExportSimulinkInterface::configure(	unsigned N,
												unsigned NX,
												unsigned NDX,
												unsigned NXA,
												unsigned NU,
												unsigned NP,
												unsigned NY,
												unsigned NYN,
												BooleanType _initialStateFixed,
												unsigned _wMatrixType,
												BooleanType _hardcodedConstraints,
												BooleanType _useArrivalCost,
												BooleanType _compCovMatrix
												)
{
	stringstream s;


	//
	// Source file configuration
	//

	wrapperSource.dictionary[ "@MODULE_NAME@" ] = moduleName;

	wrapperSource.fillTemplate();

	//
	// Header file configuration
	//
	wrapperHeader.dictionary[ "@MODULE_NAME@" ] = moduleName;

	wrapperHeader.fillTemplate();

	//
	// Makefile configuration
	//

	makefile.dictionary[ "@MODULE_NAME@" ] = moduleName;
	makefile.dictionary[ "@REAL_TYPE@" ] = makefile.realString;

	s.str(std::string()); s << N;
	makefile.dictionary[ "@N@" ] = s.str();
	s.str(std::string()); s << NX;
	makefile.dictionary[ "@NX@" ] = s.str();
	s.str(std::string()); s << NXA;
	makefile.dictionary[ "@NXA@" ] = s.str();
	s.str(std::string()); s << NDX;
	makefile.dictionary[ "@NDX@" ] = s.str();
	s.str(std::string()); s << NU;
	makefile.dictionary[ "@NU@" ] = s.str();
	s.str(std::string()); s << NP;
	makefile.dictionary[ "@NP@" ] = s.str();
	s.str(std::string()); s << NY;
	makefile.dictionary[ "@NY@" ] = s.str();
	s.str(std::string()); s << NYN;
	makefile.dictionary[ "@NYN@" ] = s.str();

	makefile.dictionary[ "@INIT_STATE_FIXED@" ] = _initialStateFixed == BT_TRUE ? "1" : "0";

	s.str(std::string()); s << _wMatrixType;
	makefile.dictionary[ "@WEIGHT_MATRIX_TYPE@" ] = s.str();

	makefile.dictionary[ "@HARCODED_CONSTRAINTS@" ] = _hardcodedConstraints == BT_TRUE ? "1" : "0";

	makefile.dictionary[ "@ARRIVAL_COST@" ] = _useArrivalCost == BT_TRUE ? "1" : "0";

	makefile.dictionary[ "@COV_MATRIX@" ] = _compCovMatrix == BT_TRUE? "1" : "0";

	makefile.fillTemplate();

	return SUCCESSFUL_RETURN;
}

returnValue ExportSimulinkInterface::exportCode()
{
	makefile.exportCode();
	wrapperSource.exportCode();
	wrapperHeader.exportCode();

	return SUCCESSFUL_RETURN;
}


CLOSE_NAMESPACE_ACADO
