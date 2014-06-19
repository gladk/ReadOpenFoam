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

#include "main.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

bool sortFileTimeCreate(fs::path i, fs::path j) {
  return (fs::last_write_time(i) < fs::last_write_time(j));
}

int main(int ac, char* av[])
{
  std::string outputFolder;
  std::string inputFolder;
  
  
  std::cout<<"\n\
ReadOpenFoam\n\
Copyright (C) 2014 TU Bergakademie Freiberg\nInstitute for Mechanics and Fluid Dynamics\n\
This program comes with ABSOLUTELY NO WARRANTY.\n\
";
  
  try {
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help", "produce help message")
      ("input,i",  po::value<std::string>()->default_value("."), "input folder")
      ("output,o", po::value<std::string>()->default_value("output"), "output folder")
    ;
    
    po::positional_options_description p;
    p.add("input", -1);
    po::variables_map vm;        
    po::store(po::command_line_parser(ac, av).
    options(desc).positional(p).run(), vm);
    po::notify(vm);  
    
    if (vm.count("help")) {
      std::cout << desc ;
      return 0;
    }
    
    if (vm.count("output")) {
      std::cout << "output folder: " << vm["output"].as<std::string>()<<std::endl ;
    }
    outputFolder = vm["output"].as<std::string>();
    
    if (vm.count("input")) {
      std::cout << "input folder: " << vm["input"].as<std::string>()<<std::endl ;
    }
    inputFolder = vm["input"].as<std::string>();
  }
  
  catch(std::exception& e) {
      std::cerr << "error: " << e.what()<<std::endl;
      exit (EXIT_FAILURE);
  }
  catch(...) {
      std::cerr << "Exception of unknown type!"<<std::endl;
  }
  
  //=====================================================
 
  std::vector< fs::path > inputFolders;
  fs::path inpDir (inputFolder);
  fs::directory_iterator end_iter;
  
  if ( fs::exists(inpDir) && fs::is_directory(inpDir))
  {
    for( fs::directory_iterator dir_iter(inpDir) ; dir_iter != end_iter ; ++dir_iter)
    {
      if (fs::is_directory(dir_iter->status()) )
      {
        fs::path curDir (*dir_iter);
        fs::path curFilePos = curDir.string() + "/lagrangian/kinematicCloud/positions.gz";
        fs::path curFileIds = curDir.string() + "/lagrangian/kinematicCloud/origId.gz";
        fs::path curFileVel = curDir.string() + "/lagrangian/kinematicCloud/U.gz";
        
        if(fs::is_regular_file(curFilePos) and fs::is_regular_file(curFileIds) and fs::is_regular_file(curFileVel)) {
          inputFolders.push_back(curDir);
        }
      }
    }
  }
  
  std::sort(inputFolders.begin(), inputFolders.end());
  
  std::cout<<inputFolders.size()<<" file(s) found to analyze"<<std::endl;
  /*
  BOOST_FOREACH(fs::path f, inputFolders) {
    std::cout<<f<<std::endl;
  }
  */
  
  return 0;
}
