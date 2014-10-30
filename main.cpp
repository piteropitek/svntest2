#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#include <sstream>

boost::property_tree::ptree xml2tree;

using boost::property_tree::ptree;
void clean_up_element (boost::property_tree::ptree& ptree){
	for (ptree::iterator node = ptree.begin(); node != ptree.end(); ++node){
		if (node->first=="Person") {
			clean_up_element(node->second);
		}
		else if (node->first == "<xmlattr>"){
			for (ptree::iterator sub_node = node->second.begin(); sub_node != node->second.end(); ++sub_node){
				if (sub_node->first == "name" || sub_node->first == "surname" || sub_node->first == "age");
				else node->second.erase(sub_node);
			}
		}
		else if (node->first == "Address" && node->second.find("<xmlattr>")!=node->second.not_found()){
			for (ptree::iterator sub_node=node->second.find("<xmlattr>")->second.begin();
			 sub_node != node->second.find("<xmlattr>")->second.end(); ++sub_node){
				if (sub_node->first == "city" || sub_node->first == "street" || sub_node->first == "house_no");
				else node->second.find("<xmlattr>")->second.erase(sub_node);
			}
		}
		else {
			ptree.erase(node);
		}
	}
}

boost::property_tree::ptree move_element(boost::property_tree::ptree::iterator& node){
	boost::property_tree::ptree temptree;
	temptree = node->second;

	clean_up_element (temptree);

    xml2tree.add_child(node->first, temptree);
    node->second.clear();

	return temptree;
}

using boost::property_tree::ptree;
void search_tree(ptree ptree, const std::string element_name){
	for (ptree::iterator node = ptree.begin(); node != ptree.end(); ++node){
		if (node->first==element_name) {
			move_element(node);
		}
		search_tree(node->second, element_name);
	}
}

int main(){
    boost::property_tree::ptree xmltree;
    read_xml("xml.xml", xmltree);
    search_tree(xmltree, "Person");
    read_xml("xml2.xml", xmltree);
	search_tree(xmltree, "Person");
    write_xml("file", xmltree);
	write_xml("file2", xml2tree);

    return 0;
}
