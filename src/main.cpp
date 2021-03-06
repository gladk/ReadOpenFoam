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
  
  //=====================================================
  if (not fs::is_directory(outputFolder)) {
    std::cout<<"The directory " << outputFolder<< " does not exists. Creating."<<std::endl;
    if (fs::create_directory(outputFolder)) {
      std::cout<<"The directory " << outputFolder<< " created."<<std::endl;
    }
  }
  
  //=====================================================================
  // Load particle files
  
  unsigned int fileId = 0;
  BOOST_FOREACH(fs::path f, inputFolders) {
    std::vector<Eigen::Vector3d> pos;
    std::vector<Eigen::Vector3d> vel;
    std::vector<double> ids;
    
    {
    // Positions  =======================================================
      fs::path curFilePos = f.string() + "/lagrangian/kinematicCloud/positions.gz";
      std::ifstream file(curFilePos.string(), std::ios_base::in | std::ios_base::binary);
      
      try {
        boost::iostreams::filtering_istream in;
        in.push(boost::iostreams::gzip_decompressor());
        in.push(file);
        unsigned int i = 0;
        unsigned int numP = 0;
        for(std::string str; std::getline(in, str); )
        {   
          if (i == 17) {
            numP = std::stoul (str);
          } else if (i-19 < numP and i > 17) {
            //std::cout << "Processed line " << str << '\n';
            
            std::vector<std::string> tokens;
            std::istringstream iss(str);
            copy(std::istream_iterator<std::string>(iss),
                 std::istream_iterator<std::string>(),
                 std::back_inserter<std::vector<std::string> >(tokens));
            
            const double x = std::stod(tokens[0].erase(0,1));
            const double y = std::stod(tokens[1]);
            const double z = std::stod(tokens[2].erase(tokens[2].size(),1));
            pos.push_back(Eigen::Vector3d(x,y,z));
          }
          i++;
        }
      }
      catch(const boost::iostreams::gzip_error& e) {
           std::cout << e.what() << '\n';
      }
    }
    {
      // Velocities  =====================================================
      fs::path curFileVel = f.string() + "/lagrangian/kinematicCloud/U.gz";
      std::ifstream file(curFileVel.string(), std::ios_base::in | std::ios_base::binary);
      
      try {
        boost::iostreams::filtering_istream in;
        in.push(boost::iostreams::gzip_decompressor());
        in.push(file);
        unsigned int i = 0;
        unsigned int numP = 0;
        for(std::string str; std::getline(in, str); )
        {   
          if (i == 18) {
            numP = std::stoul (str);
          } else if (i-20 < numP and i > 18) {
            std::vector<std::string> tokens;
            std::istringstream iss(str);
            copy(std::istream_iterator<std::string>(iss),
                 std::istream_iterator<std::string>(),
                 std::back_inserter<std::vector<std::string> >(tokens));
            
            const double x = std::stod(tokens[0].erase(0,1));
            const double y = std::stod(tokens[1]);
            const double z = std::stod(tokens[2].erase(tokens[2].size(),1));
            vel.push_back(Eigen::Vector3d(x,y,z));
          }
          i++;
        }
      }
      catch(const boost::iostreams::gzip_error& e) {
           std::cout << e.what() << '\n';
      }
    }
    {
      // Ids  =====================================================
      fs::path curFileIDs = f.string() + "/lagrangian/kinematicCloud/origId.gz";
      std::ifstream file(curFileIDs.string(), std::ios_base::in | std::ios_base::binary);
      
      try {
        boost::iostreams::filtering_istream in;
        in.push(boost::iostreams::gzip_decompressor());
        in.push(file);
        unsigned int i = 0;
        unsigned int numP = 0;
        for(std::string str; std::getline(in, str); )
        {   
          if (i == 18) {
            numP = std::stoul (str);
          } else if (i-20 < numP and i > 18) {
            ids.push_back(std::stod(str));
          }
          i++;
        }
      }
      catch(const boost::iostreams::gzip_error& e) {
           std::cout << e.what() << '\n';
      }
    }
    
    std::cout<<" Files in folder " << f.filename() << " " <<pos.size() << " " << vel.size()  << " " << ids.size()<<"; ";
    std::vector<particle> particles;
    for (unsigned int i=0; i<pos.size(); i++) {
      const particle tmpPart=particle(ids[i], pos[i], vel[i]);
      particles.push_back(tmpPart);
    }
    std::cout<<"Particle size " << particles.size()<<std::endl;
    
    
    // CreatVTK  =====================================================
    std::string _fileNameVTU = outputFolder + "/output_" + std::to_string(fileId) + ".vtu";
    vtkSmartPointer<vtkPoints>    spheresPos    = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> spheresCells  = vtkSmartPointer<vtkCellArray>::New();
    
    vtkSmartPointer<vtkIntArray> spheresId = vtkSmartPointer<vtkIntArray>::New();
    spheresId->SetNumberOfComponents(1);
    spheresId->SetName("p_Id");
    
    vtkSmartPointer<vtkDoubleArray> spheresVelL = vtkSmartPointer<vtkDoubleArray>::New();
    spheresVelL->SetNumberOfComponents(3);
    spheresVelL->SetName("p_Velocity");
    
    BOOST_FOREACH(particle p, particles) {
      vtkIdType pid[1];
      pid[0] = spheresPos->InsertNextPoint(p.c()[0], p.c()[1], p.c()[2]);
      
      spheresId->InsertNextValue(p.id());
      
      double vv[3] = {p.v()[0], p.v()[1], p.v()[2]};
      spheresVelL->InsertNextTupleValue(vv);
      
      spheresCells->InsertNextCell(1,pid);
    }
    
    vtkSmartPointer<vtkUnstructuredGrid> spheresUg = vtkSmartPointer<vtkUnstructuredGrid>::New();
    
    spheresUg->SetPoints(spheresPos);
    spheresUg->SetCells(VTK_VERTEX, spheresCells);
    spheresUg->GetPointData()->AddArray(spheresId);
    spheresUg->GetPointData()->AddArray(spheresVelL);
    
    vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    writer->SetDataModeToAscii();
    
    #ifdef VTK6
      writer->SetInputData(spheresUg);
    #else
      writer->SetInput(spheresUg);
    #endif
    
    writer->SetFileName(_fileNameVTU.c_str());
    writer->Write();
    
    fileId++;
    
  }
  return 0;
}
