/*
 * disjointPathsPython.cxx
 *
 *  Created on: Jul 7, 2020
 *      Author: fuksova
 */

#include <pybind11/pybind11.h>
#include "disjoint-paths/disjointParams.hxx"
//#include "disjoint-paths/disjointPathsMethods.hxx"
#include "disjoint-paths/ilp/solver-disjoint-ilp.hxx"
//#include "disjoint-paths/completeStructure.hxx"
#include <string>
namespace py = pybind11;



PYBIND11_MODULE(disjointPathsPy, m) {
    m.doc() = "python binding for lifted disjoint paths";

    py::class_<disjointPaths::ParametersParser>(m,"ParametersParser")
            .def(py::init<>())
            .def("get_parsed_params",&disjointPaths::ParametersParser::getParsedStrings,"getting the parsed strings from parser")
            .def("init_from_file", py::overload_cast<std::string&>(&disjointPaths::ParametersParser::initFromFile),"Parses parameters from a file");
           // .def("init_from_stream",&disjointPaths::ParametersParser::initFromStream<std::stringstream>,"Parses parameters from a stream");


     py::class_<disjointPaths::DisjointParams<>>(m, "DisjointParams")
        .def(py::init<std::map<std::string,std::string>&>());
        
     py::class_<disjointPaths::VertexGroups<>>(m, "TimeFramesToVertices")
        .def(py::init<>())
        .def("init_from_vector", &disjointPaths::VertexGroups<>::initFromVector, "Initializes vertices in time frames from a vector of size_t")
        .def("init_from_file", &disjointPaths::VertexGroups<>::initFromFile<disjointPaths::DisjointParams<>>, "Initializes vertices in time frames from a file");

     py::class_<disjointPaths::CompleteStructure<>>(m, "GraphStructure")
        .def(py::init<disjointPaths::VertexGroups<> &>())
        .def("add_edges_from_array", &disjointPaths::CompleteStructure<>::addEdgesFromMatrix, "Initializes edges of the graph between two time frames from a matrix.")
        .def("add_edges_from_vectors", &disjointPaths::CompleteStructure<>::addEdgesFromVectors<disjointPaths::DisjointParams<>>, "Initializes edges of the graph from an Nx2 array of size_t with edge vertices and an Nx1 array of doubles with costs. Restrictions on maximal vertex and maximal time gap from parameters apply.")
        .def("add_edges_from_vectors_all", &disjointPaths::CompleteStructure<>::addEdgesFromVectorsAll, "Initializes edges of the graph from an Nx2 array of size_t with edge vertices and an Nx1 array of doubles with costs. All edges added. No restriction on maximal timegap. ")
        .def("add_edges_from_file", &disjointPaths::CompleteStructure<>::addEdgesFromFile<disjointPaths::DisjointParams<>>, "Initializes all edges of the graph from a file.")
        .def("get_edge_labels",&disjointPaths::CompleteStructure<>::getGraphEdgeLabels,"Returns 0/1 labels of all input edges w.r.t. given set of paths. Label one is given iff detections belong to the same path." )
        .def("get_edge_list",&disjointPaths::CompleteStructure<>::getEdgeList,"Return list of edges present in this graph structure.");

    // m.def("solve_ilp", py::overload_cast<disjointPaths::DisjointParams<>&, disjointPaths::CompleteStructure<>&>(&disjointPaths::solver_ilp_intervals<>), "Solve lifted disjoint paths");

     m.def("solve_ilp", &disjointPaths::solver_ilp<>, "Solve lifted disjoint paths");

     m.def("write_output_to_file", &disjointPaths::writeOutputToFile, "Write output tracks to a specified file");

     m.def("solve_standard_disjoit_paths", &disjointPaths::solver_flow_only<>, "Solve standard disjoint paths problem");

     m.def("get_base_edge_labels",&disjointPaths::getBaseEdgeLabels<std::vector<std::array<size_t,2>>>,"Given a vector of base edge vertices, vector of solution paths and the number of graph vertices, it returns labels to base edges.");

     m.def("get_lifted_edge_labels",&disjointPaths::getLiftedEdgeLabels<std::vector<std::array<size_t,2>>>,"Given a vector of lifted edge vertices, vector of solution paths and the number of graph vertices, it returns labels to base edges.");




}


