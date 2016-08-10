#pragma once
// STL Lib
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

// Pre Build Class

enum BlockType{ Unstructure};

class VTK_Points
{
public:
	// Constructor & Distructor
	VTK_Points() = delete;
	~VTK_Points() { Leng = 0; };

	// Defined Constructor
	VTK_Points(double *_x, double *_y, double *_z, int Length) : x(_x), y(_y), z(_z), Leng(Length) {};

	// Data
	//- Coordinates
	double *x, *y, *z;
	//- Length
	int Leng;
};

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

class XML_Helper
{
public:
	// Constructor & Distructor
	XML_Helper(std::string NodeName_, int Level_ = 0) : NodeName(NodeName_), Level(Level_) {};
	~XML_Helper() {};

	// Add Attribute
	void addAttribute(std::string Title, int Value);
	void addAttribute(std::string Title, double Value);
	void addAttribute(std::string Title, std::string Value);

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

std::ostream &operator<<(std::ostream &s, XML_Helper Helper) {
	for (int i = 0; i < Helper.getLevel(); ++i) {
		s << "\t";
	}
	return s;
}

std::string XML_Helper::printFooter() {
	std::string Ostr;
	for (int i = 0; i < Level; ++i) {
		Ostr += "\t";
	}
	Ostr += "</";
	Ostr += NodeName;
	Ostr += ">";
	return Ostr;
}

void XML_Helper::addAttribute(std::string Title, int Value) {
	std::ostringstream os;
	os << Value;
	std::string Value_s = os.str();

	Attr.emplace_back(Title, Value_s);
}
void XML_Helper::addAttribute(std::string Title, double Value) {
	std::ostringstream os;
	os << Value;
	std::string Value_s = os.str();

	Attr.emplace_back(Title, Value_s);
}
void XML_Helper::addAttribute(std::string Title, std::string Value) {
	Attr.emplace_back(Title, Value);
}

std::string XML_Helper::printHeader() {
	std::string Ostr;
	for (int i = 0; i < Level; ++i) {
		Ostr += "\t";
	}
	Ostr += "<";
	Ostr += NodeName;

	for (std::vector<std::pair<std::string, std::string> >::iterator it = Attr.begin(); it != Attr.end(); ++it) {
		Ostr += " ";
		Ostr += it->first;
		Ostr += "=\"";
		Ostr += it->second;
		Ostr += "\"";
	}
	Ostr += ">";
	return Ostr;
}

//=========== Block Class ============
class VTK_Element
{
public:
	// Constructor & Distructor
	VTK_Element() = delete;
	~VTK_Element();
	//- Unstructural
	VTK_Element(VTK_Points *Points_, VTK_Cells *Cells_);

	// Block Type
	BlockType MeshType;
	// Points Coord
	VTK_Points *Points;
	// Cells Connectivity
	VTK_Cells *Cells;
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
	~ACML_VTK_Tools();
	ACML_VTK_Tools(std::string FileName_);

	// New Block(Memory Controaled)
	int NewBlock(VTK_Element* B);

	// Save
	bool SaveFiles();

private:
	// Single Element Save file
	bool SingleUnstructureElementSave(std::string Name, VTK_Element* Element);



	// File Name
	std::string FileName;
	// Block Set
	std::vector<VTK_Element*> Blocks;
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

ACML_VTK_Tools::~ACML_VTK_Tools()
{
	
}

int ACML_VTK_Tools::NewBlock(VTK_Element* B) {
	Blocks.push_back(B);
	return int(Blocks.size());
}

bool ACML_VTK_Tools::SaveFiles() {
	std::string OutputName;
	OutputName += FileName;
	OutputName += ".vtu";
	SingleUnstructureElementSave(OutputName, *(Blocks.begin()));
}

bool ACML_VTK_Tools::SingleUnstructureElementSave(std::string Name, VTK_Element* Element) {
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
		Points("Points", 3), Cells("Cells", 3), 
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
	DataArray.clearAttribute();
	DataArray.addAttribute("type", "Int32");
	DataArray.addAttribute("Name", "connectivity");
	DataArray.addAttribute("NumberOfComponents", 1);
	DataArray.addAttribute("format", "ascii");
	File << DataArray.printHeader() << std::endl;
	for (int i = 0; i < int(Element->Cells->Cells.size()); i++)
	{
		File << DataArray;
		for (int j = 0; j < Element->Cells->Cells[i].NodeNum; j++) {
			if (j != Element->Cells->Cells[i].NodeNum - 1) {
				File << Element->Cells->Cells[i].NodeId[j] << " ";
			}
			else {
				File << Element->Cells->Cells[i].NodeId[j];
			}
		}
		File << std::endl;
	}
	File << DataArray.printFooter() << std::endl;
	DataArray.clearAttribute();
	DataArray.addAttribute("type", "Int32");
	DataArray.addAttribute("Name", "offsets");
	DataArray.addAttribute("NumberOfComponents", 1);
	DataArray.addAttribute("format", "ascii");
	File << DataArray.printHeader() << std::endl;
	int Offset = 0;
	File << DataArray;
	for (int i = 0; i < int(Element->Cells->Cells.size()); i++)
	{
		Offset += Element->Cells->Cells[i].NodeNum;
		File << Offset << " ";
	}
	File << std::endl;
	File << DataArray.printFooter() << std::endl;
	DataArray.clearAttribute();
	DataArray.addAttribute("type", "UInt8");
	DataArray.addAttribute("Name", "types");
	DataArray.addAttribute("NumberOfComponents", 1);
	DataArray.addAttribute("format", "ascii");
	File << DataArray.printHeader() << std::endl;
	File << DataArray;
	for (int i = 0; i < int(Element->Cells->Cells.size()); i++)
	{
		File << Element->Cells->Cells[i].getVTK_3dMeshType() << " ";
	}
	File << std::endl;
	File << DataArray.printFooter() << std::endl;
	File << Cells.printFooter() << std::endl;

	//- Foot
	File << Piece.printFooter() << std::endl;
	File << UnstructuredGrid.printFooter() << std::endl;
	File << VTKFile.printFooter() << std::endl;

	File.close();
	return true;
}
//=========== Main Class ============
