// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: Task.hh
// Purpose: Parent class for analysis tasks
 
#ifndef TASK_HH__
#define TASK_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>


class Task
{
protected:
public:
	Task() {};
	~Task() {};
	virtual void Go() {};
	virtual void Go(int filenum) {};
};

#endif // TASK_HH__
