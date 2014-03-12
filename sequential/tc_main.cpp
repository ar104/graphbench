#include <cstring>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include <set>
#include <map>
#include "graph.h"
#include "graph_io.h"
#include "tc.h"
#include "graph_util.h"

using std::cout;
using std::endl;

static void doTriangleCount (const Graph &graph) {
    timespec before, after;
    std::set<Triangle> triangles;
    //std::set<Triangle>::iterator it;

    cout << "Running undirected triangle count" << endl;

    // run triangle count and time it
    clock_gettime(CLOCK_MONOTONIC, &before);
    undirectedTriangleCount(graph, triangles);
    clock_gettime(CLOCK_MONOTONIC, &after);

    // print triangles
    /*
    for(it = triangles.begin(); it != triangles.end(); ++it) {
        cout << it->a.getLabel()
              << " "
              << it->b.getLabel()
              << " "
              << it->c.getLabel()
              << std::endl;
    }*/

    // construct the runtime
    time_t sec = after.tv_sec - before.tv_sec;
    long milli = (after.tv_nsec - before.tv_nsec) / 1000000;
    if (milli < 0) milli += 1000; // if after's nsec < before's nsec

    cout << "Runtime: "
         << sec
         << "."
         << milli
         << "s"
         << std::endl;

    cout << "Triangle count: "
         << triangles.size()
         << endl;
}

int main(int argc, const char **argv) {
    Graph graph;
    const char* graph_file = NULL;
    const char* format = NULL;

    if (argc < 7) {
        cout << "Must specify which graph file to use "
             << "(e.g. '-g graph.txt' or '--graph graph.txt') "
             << endl
             << "as well as the file's format "
             << "(e.g. '-f tsv' or '--format csv')"
             << endl;
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc-1; ++i) {
        if (strcmp(argv[i],"-g") == 0 || strcmp(argv[i],"--graph") == 0) {
            graph_file = argv[i + 1];
        } else if (strcmp(argv[i],"-f") == 0 || strcmp(argv[i],"--format") == 0) {
            format = argv[i + 1];
        }
    }

    if (format == NULL) {
        cout << "Must specify the format of the graph input file. "
             << "e.g. '-f tsv' or '--format csv'" << endl;
        return EXIT_FAILURE;
    }

    if (graph_file == NULL) {
        cout << "Must specify which graph to use."
             << "e.g. '-g graph.txt' or '--graph graph.txt'" << endl;
        return EXIT_FAILURE;
    }

    // import the graph in the specified format
    if (strcmp(format, "tsv") == 0) {
        importTSVGraph(graph_file, graph, false);
    } else if (strcmp(format, "csv") == 0) {
        importCSVGraph(graph_file, graph, false);
    } else {
        // the specified format does not match any supported format
        cout << "Unknown graph file format: " << format << endl;
        return EXIT_FAILURE;
    }

    doTriangleCount(graph);

    return EXIT_SUCCESS;
}

