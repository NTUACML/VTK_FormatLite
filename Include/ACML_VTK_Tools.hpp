#pragma once
// STL Lib
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

// Pre Build Class
class VTK_Points;
struct MeshCell;
class VTK_Cells;
class XML_Helper;

//=========== Main Class ============
class ACML_VTK_Tools
{
public:
	ACML_VTK_Tools();
	~ACML_VTK_Tools();

private:

};

ACML_VTK_Tools::ACML_VTK_Tools()
{
}

ACML_VTK_Tools::~ACML_VTK_Tools()
{
}
//=========== Main Class ============

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
	int *NodeId;
	int NodeNum;
};

class VTK_Cells
{
public:
	// Constructor & Distructor
	VTK_Cells() { };
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
	XML_Helper(std::string NodeName_, int Level_ = 0) : NodeName(NodeName_), Level(Level_){};
	~XML_Helper() {};

	// Add Attribute
	void addAttribute(std::string Title, int Value);
	void addAttribute(std::string Title, double Value);
	void addAttribute(std::string Title, std::string Value);

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

	for (std::vector<std::pair<std::string, std::string> >::iterator it = Attr.begin(); it != Attr.end();++it) {
		Ostr += " ";
		Ostr += it->first;
		Ostr += "=\"";
		Ostr += it->second;
		Ostr += "\"";
	}
	Ostr += ">";
	return Ostr;
}