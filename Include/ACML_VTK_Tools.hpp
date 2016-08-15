#pragma once
// STL Lib
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
enum BlockType{ Unstructure};

//=========== Point Class ============
class VTK_Points
{
public:
	// Constructor & Distructor
	VTK_Points() = delete;
	~VTK_Points() { Leng = 0; };
	//- Defined Constructor
	VTK_Points(double *_x, double *_y, double *_z, int Length) : x(_x), y(_y), z(_z), Leng(Length) {};

	// Data
	//- Coordinates
	double *x, *y, *z;
	//- Length
	int Leng;
};
//=========== Point Class ============
//=========== Cell Class ============
struct MeshCell
{
	MeshCell(int *NodeId_, int NodeNum_) : NodeId(NodeId_), NodeNum(NodeNum_) {};
	int getVTK_3dMeshType();
	int *NodeId;
	int NodeNum;
};
int MeshCell::getVTK_3dMeshType() {
	if (NodeNum == 4) return 10; // VTK_TETRA
	else if (NodeNum == 8) return 12; // VTK_HEXAHEDRON
	else if (NodeNum == 6) return 13; // VTK_WEDGE
	else if (NodeNum == 5) return 14; // VTK_PYRAMID
	else return 0;
}
class VTK_Cells
{
public:
	// Constructor & Distructor
	VTK_Cells() {};
	~VTK_Cells() {};

	// Add Cell Info
	void addCell(int *Connect, int NodeNum);

	// Data
	std::vector<MeshCell> Cells;
};
void VTK_Cells::addCell(int *Connect, int NodeNum) {
	Cells.emplace_back(Connect, NodeNum);
}
//=========== Cell Class ============
//=========== Data Class ============
class VTK_Data
{
public:
	// Constructor & Distructor
	VTK_Data() = delete;
	~VTK_Data() {};
	VTK_Data(std::string DataName) :Name(DataName) {};

	// Add DataSet
	void addDataSet(double *Input) { DataSet.push_back(Input); };

	std::vector<double*> DataSet;
	std::string Name;
};
//=========== Data Class ============
//=========== XML Class ============
class XML_Helper
{
public:
	// Constructor & Distructor
	XML_Helper(std::string NodeName_, int Level_ = 0) : NodeName(NodeName_), Level(Level_) {};
	~XML_Helper() {};

	// Add Attribute
	void addAttribute(std::string Title, int Value);
	void addAttribute(std::string Title, double Value);
	void addAttribute(std::string Title, std::string Value) {Attr.emplace_back(Title, Value);};

	// Edit Attribute
	void editAttribute(std::string Title, int Value);
	void editAttribute(std::string Title, double Value);
	void editAttribute(std::string Title, std::string Value);

	// Clear Attribute
	void clearAttribute() { Attr.clear(); };

	// Get(Set) Level
	int getLevel() { return Level; };
	void setLevel(int Level_) { Level = Level_; };

	// Print
	//- Footer
	std::string printFooter();
	//- Header
	std::string printHeader();

	// Friend Function for Level Overset
	friend std::ostream &operator<<(std::ostream &s, XML_Helper Helper);
private:
	// Data
	std::string NodeName;
	std::vector<std::pair<std::string, std::string> > Attr;
	int Level;
};
inline std::ostream &operator<<(std::ostream &s, XML_Helper Helper) {
	for (int i = 0; i < Helper.getLevel(); ++i) {s << "\t";} return s;
}
inline std::string XML_Helper::printFooter() {
	std::string Ostr;
	for (int i = 0; i < Level; ++i) {
		Ostr += "\t";
	}
	Ostr += "</"; Ostr += NodeName; Ostr += ">";
	return Ostr;
}
inline void XML_Helper::addAttribute(std::string Title, int Value) {
	std::ostringstream os;
	os << Value;
	std::string Value_s = os.str();
	Attr.emplace_back(Title, Value_s);
}
inline void XML_Helper::addAttribute(std::string Title, double Value) {
	std::ostringstream os;
	os << Value;
	std::string Value_s = os.str();
	Attr.emplace_back(Title, Value_s);
}
inline void XML_Helper::editAttribute(std::string Title, int Value) {
	std::vector<std::pair<std::string, std::string> >::iterator it = 
		std::find_if(Attr.begin(), Attr.end(), [Title](std::pair<std::string, std::string> &T) { return T.first == Title; });
	if (it != Attr.end()) {
		std::ostringstream os;
		os << Value;
		std::string Value_s = os.str();
		it->second = Value_s;
	}
	else { addAttribute(Title, Value); }
}
inline void XML_Helper::editAttribute(std::string Title, double Value) {
	std::vector<std::pair<std::string, std::string> >::iterator it =
		std::find_if(Attr.begin(), Attr.end(), [Title](std::pair<std::string, std::string> &T) { return T.first == Title; });
	if (it != Attr.end()) {
		std::ostringstream os;
		os << Value;
		std::string Value_s = os.str();
		it->second = Value_s;
	}
	else { addAttribute(Title, Value); }
}
inline void XML_Helper::editAttribute(std::string Title, std::string Value) {
	std::vector<std::pair<std::string, std::string> >::iterator it =
		std::find_if(Attr.begin(), Attr.end(), [Title](std::pair<std::string, std::string> &T) { return T.first == Title; });
	if (it != Attr.end()) { it->second = Value; }
	else { addAttribute(Title, Value); }
}
inline std::string XML_Helper::printHeader() {
	std::string Ostr;
	for (int i = 0; i < Level; ++i) { Ostr += "\t"; }
	Ostr += "<"; Ostr += NodeName;

	for (std::vector<std::pair<std::string, std::string> >::iterator it = Attr.begin(); it != Attr.end(); ++it) {
		Ostr += " "; Ostr += it->first; Ostr += "=\""; Ostr += it->second; Ostr += "\"";
	}
	Ostr += ">";
	return Ostr;
}
//=========== XML Class ============
//=========== PVD Struct ============
struct PVD_Data
{
	PVD_Data(double TimeStep_, int Group_, int Part_, std::string FileName_) :
		TimeStep(TimeStep_), Group(Group_), Part(Part_), FileName(FileName_) {};
	double TimeStep;
	int Group, Part;
	std::string FileName;
};
//=========== PVD Struct ============
//=========== Block Class ============
class VTK_Element
{
public:
	// Constructor & Distructor
	VTK_Element() = delete;
	~VTK_Element();
	//- Unstructural
	VTK_Element(VTK_Points *Points_, VTK_Cells *Cells_);

	// Add Points Data
	void addPointsData(VTK_Data* PointsData_) { PointsData.push_back( PointsData_ ); };
	// Add Cells Data
	void addCellsData(VTK_Data* CellsData_) { CellsData.push_back( CellsData_ ); };

	// Block Type
	BlockType MeshType;
	// Points Coord
	VTK_Points *Points;
	// Cells Connectivity
	VTK_Cells *Cells;
	// Points Data, Cells Data
	std::vector<VTK_Data*> PointsData, CellsData;
};
VTK_Element::~VTK_Element()
{
	Points = NULL;
	Cells = NULL;
}
VTK_Element::VTK_Element(VTK_Points *Points_, VTK_Cells *Cells_) {
	// Unstructure Mesh
	MeshType = Unstructure;
	// Get Data
	Points = Points_;
	Cells = Cells_;
}
//=========== Block Class ============
//=========== Main Class ============
class ACML_VTK_Tools
{
public:
	// Constructor & Distructor
	ACML_VTK_Tools() = delete;
	~ACML_VTK_Tools() {};
	ACML_VTK_Tools(std::string FileName_);

	// New Block(Memory Controaled)
	int NewElement(VTK_Element* B);
	// Save
	bool SaveFiles();
	// Set Folder
	void setFolder(std::string Fd) { Folder = Fd; };
	// Set Iter(Iteration Times, Simulation Time)
	void setIter(int IterNum, double T) { Counter = IterNum; Time = T; };

private:
	// Single Element Save file
	bool SingleUnstructureElementSave(std::string Name, VTK_Element* Element);
	// PVD Output
	bool SavePVD();

	// Get Type Extension
	std::string getTypeExtension(BlockType T);

	// File Name
	std::string FileName;
	// Block Set
	std::vector<VTK_Element*> Blocks;
	// PVD Collection
	std::vector<PVD_Data> PVD_Collection;
	// Time
	double Time;
	// Counter
	int Counter;
	// Folder
	std::string Folder;
};
ACML_VTK_Tools::ACML_VTK_Tools(std::string FileName_) :FileName(FileName_)
{
	Time = 0.0;
	Counter = 0;
};
inline int ACML_VTK_Tools::NewElement(VTK_Element* B) {
	Blocks.push_back(B);
	return int(Blocks.size());
}
inline bool ACML_VTK_Tools::SaveFiles() {
	std::ostringstream OutputName, BlockName;
	int B_Num = 0;
	// Block Put
	for (std::vector<VTK_Element*>::iterator it = Blocks.begin(); it != Blocks.end(); ++it) {
		// Clear
		BlockName.str(""); BlockName.clear();
		OutputName.str(""); OutputName.clear();
		// File Name
		BlockName << FileName << "_Itr" << Counter << "_T" << Time << "_B" << B_Num
			<< getTypeExtension((*it)->MeshType);
		OutputName << Folder << BlockName.str();
		// Save Single Block
		if (!SingleUnstructureElementSave(OutputName.str(), *it)) {
			std::cout << "File: " << OutputName.str() << " Open Error" << std::endl;
			return false;
		}
		// Add into PVD Collection
		PVD_Collection.emplace_back(Time, B_Num, 0, BlockName.str());
		// Save PVD File
		if (!SavePVD()) {
			std::cout << "File: " << FileName << ".pvd Open Error" << std::endl;
			return false;
		}
		B_Num++;
	}
	return true;
}
inline bool ACML_VTK_Tools::SingleUnstructureElementSave(std::string Name, VTK_Element* Element) {
	std::ofstream File;
	File.open(Name);
	if(!File.is_open()){
		std::cout << "Opne File: " << Name << " With Error" << std::endl;
		return false;
	}
	// Start Output
	File << "<?xml version=\"1.0\"?>" << std::endl;
	// Node Define
	XML_Helper VTKFile("VTKFile"),
		UnstructuredGrid("UnstructuredGrid", 1),
		Piece("Piece", 2),
		Points("Points", 3), Cells("Cells", 3), PointsData("PointData", 3), CellsData("CellData", 3),
		DataArray("DataArray", 4);

	VTKFile.addAttribute("type", "UnstructuredGrid"); 
	VTKFile.addAttribute("version", 0.1);
	VTKFile.addAttribute("byte_order", "LittleEndian");

	Piece.addAttribute("NumberOfPoints", Element->Points->Leng);
	Piece.addAttribute("NumberOfCells", int(Element->Cells->Cells.size()));

	DataArray.addAttribute("type", "Float32");
	DataArray.addAttribute("Name", "Position");
	DataArray.addAttribute("NumberOfComponents", 3);
	DataArray.addAttribute("format", "ascii");

	// Print
	//- Head
	File << VTKFile.printHeader() << std::endl;
	File << UnstructuredGrid.printHeader() << std::endl;
	File << Piece.printHeader() << std::endl;
	//- Points
	File << Points.printHeader() << std::endl;
	File << DataArray.printHeader() << std::endl;
	for (int i = 0; i < Element->Points->Leng; i++)
	{
		File << DataArray << *(Element->Points->x + i) << " " << *(Element->Points->y + i) << " " << *(Element->Points->z + i) << std::endl;
	}
	File << DataArray.printFooter() << std::endl;
	File << Points.printFooter() << std::endl;
	//- Cells
	File << Cells.printHeader() << std::endl;
	DataArray.editAttribute("type", "Int32");
	DataArray.editAttribute("Name", "connectivity");
	DataArray.editAttribute("NumberOfComponents", 1);
	File << DataArray.printHeader() << std::endl;
	for (int i = 0; i < int(Element->Cells->Cells.size()); i++)
	{
		File << DataArray;
		for (int j = 0; j < Element->Cells->Cells[i].NodeNum; j++) {
			if (j != Element->Cells->Cells[i].NodeNum - 1) {
				File << Element->Cells->Cells[i].NodeId[j] - 1 << " ";
			}
			else {
				File << Element->Cells->Cells[i].NodeId[j] - 1;
			}
		}
		File << std::endl;
	}
	File << DataArray.printFooter() << std::endl;
	DataArray.editAttribute("Name", "offsets");
	File << DataArray.printHeader() << std::endl;
	int Offset = 0;
	File << DataArray;
	for (int i = 0; i < int(Element->Cells->Cells.size()); i++)
	{
		Offset += Element->Cells->Cells[i].NodeNum;
		if (i != int(Element->Cells->Cells.size() - 1)){ File << Offset << " "; }
		else { File << Offset << std::endl; }
	}
	File << DataArray.printFooter() << std::endl;
	DataArray.editAttribute("type", "UInt8");
	DataArray.editAttribute("Name", "types");
	File << DataArray.printHeader() << std::endl;
	File << DataArray;
	for (int i = 0; i < int(Element->Cells->Cells.size()); i++)
	{
		if (i != int(Element->Cells->Cells.size() - 1)) {
			File << Element->Cells->Cells[i].getVTK_3dMeshType() << " ";
		}
		else {
			File << Element->Cells->Cells[i].getVTK_3dMeshType() << std::endl;
		}
	}
	File << DataArray.printFooter() << std::endl;
	File << Cells.printFooter() << std::endl;

	//- Points Data
	if (Element->PointsData.size() > 0) {
		File << PointsData.printHeader() << std::endl;
		for (int i = 0; i < Element->PointsData.size(); i++)
		{
			DataArray.editAttribute("type", "Float32");
			DataArray.editAttribute("Name", Element->PointsData[i]->Name);
			DataArray.editAttribute("NumberOfComponents", int(Element->PointsData[i]->DataSet.size()));
			File << DataArray.printHeader() << std::endl;
			for (int j = 0; j < Element->Points->Leng; j++)
			{
				File << DataArray;
				for (int k = 0; k < Element->PointsData[i]->DataSet.size(); k++)
				{
					if (k < Element->PointsData[i]->DataSet.size() - 1) {
						File << *(Element->PointsData[i]->DataSet[k] + j) << " ";
					}
					else {
						File << *(Element->PointsData[i]->DataSet[k] + j);
					}
				}
				File << std::endl;
			}
			File << DataArray.printFooter() << std::endl;
		}
		File << PointsData.printFooter() << std::endl;
	}

	//- Cells Data
	if (Element->CellsData.size() > 0) {
		File << CellsData.printHeader() << std::endl;
		for (int i = 0; i < Element->CellsData.size(); i++)
		{
			DataArray.editAttribute("type", "Float32");
			DataArray.editAttribute("Name", Element->CellsData[i]->Name);
			DataArray.editAttribute("NumberOfComponents", int(Element->CellsData[i]->DataSet.size()));
			File << DataArray.printHeader() << std::endl;
			for (int j = 0; j < int(Element->Cells->Cells.size()); j++)
			{
				File << DataArray;
				for (int k = 0; k < Element->CellsData[i]->DataSet.size(); k++)
				{
					if (k < Element->CellsData[i]->DataSet.size() - 1) {
						File << *(Element->CellsData[i]->DataSet[k] + j) << " ";
					}
					else {
						File << *(Element->CellsData[i]->DataSet[k] + j);
					}
				}
				File << std::endl;
			}
			File << DataArray.printFooter() << std::endl;
		}
		File << CellsData.printFooter() << std::endl;
	}

	//- Foot
	File << Piece.printFooter() << std::endl;
	File << UnstructuredGrid.printFooter() << std::endl;
	File << VTKFile.printFooter() << std::endl;

	File.close();
	return true;
}
inline bool ACML_VTK_Tools::SavePVD()
{
	std::string PVD_Name;
	std::ostringstream os;
	PVD_Name = Folder + FileName + ".pvd";
	std::ofstream PVDFile;
	PVDFile.open(PVD_Name);
	if (!PVDFile.is_open()) {
		std::cout << "PVD File Open Error" << std::endl;
		return false;
	}
	// Start Output
	PVDFile << "<?xml version = \"1.0\"?>" << std::endl
		<< "<VTKFile type = \"Collection\" version = \"0.1\" byte_order = \"LittleEndian\">" << std::endl;
	PVDFile << "<Collection>" << std::endl;
	//- Collection Part
	for (std::vector<PVD_Data>::iterator it = PVD_Collection.begin(); it != PVD_Collection.end(); it++)
	{
		os.str("");
		os.clear();
		os << "<DataSet timestep=\"" << it->TimeStep << "\" group=\"" << it->Group
			<< "\" part=\"" << it->Part << "\" file=\"" << it->FileName << "\"/>" << std::endl;
		PVDFile << os.str();
	}
	PVDFile << "</Collection>" << std::endl << "</VTKFile>" << std::endl;
	PVDFile.close();
	return true;
}
inline std::string ACML_VTK_Tools::getTypeExtension(BlockType T)
{
	if (T == Unstructure) { return std::string(".vtu");}
	else { return std::string();}
}
//=========== Main Class ============
