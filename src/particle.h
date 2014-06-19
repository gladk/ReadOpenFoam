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

#include <Eigen/Dense>
#include <memory>
#include <vector>

class particle {
  private:
    Eigen::Vector3d _c, _v;       // Center of mass, velocity
    unsigned long long _id;       // Particle id
    unsigned int _fileId;         // File number of the particle
    unsigned int _snapshot;       // Snapshot
    
  public:
    particle (unsigned long long id, Eigen::Vector3d c, Eigen::Vector3d v);
    
    unsigned long long id() const {return _id;};
    Eigen::Vector3d c() const {return _c;}
    Eigen::Vector3d v() const {return _v;}
};
