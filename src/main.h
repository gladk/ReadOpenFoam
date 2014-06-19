/*
    This file is part of ReadOpenFoam.
    ReadOpenFoam is the programm to read OpenFoam files and create VTK.
    Copyright (C) 2013, 2014 TU Bergakademie Freiberg, Institute for Mechanics and Fluid Dynamics

    Author: 2014 Anton Gladky <gladky.anton@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    RheometerAnalyze is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ReadOpenFoam.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <boost/filesystem.hpp> 
#include <boost/program_options.hpp>
#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/version.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>
#include <Eigen/Dense>
#include "particle.h"

