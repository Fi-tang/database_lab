#include <stdio.h>
#include <string.h>
#include <math.h>

#include "GraphLite.h"

#define VERTEX_CLASS_NAME(name) KCore##name
int standardKValue = 0;
/**
 @param VertexTypevalue : int (current degree) here use Vertex value to record current_degree
 @param EdgeTypevalue: double(as default) do not care about the Edge size
 @param MessageTypevalue: bool(false means keep as it is , true means is_deleted) 
 Message send bool type as true or false to mark whether this vertex has been deleted or not
*/
class VERTEX_CLASS_NAME(InputFormatter): public InputFormatter {
public:
    int64_t getVertexNum() {
        unsigned long long n;
        sscanf(m_ptotal_vertex_line, "%lld", &n);
        m_total_vertex= n;
        return m_total_vertex;
    }
    int64_t getEdgeNum() {
        unsigned long long n;
        sscanf(m_ptotal_edge_line, "%lld", &n);
        m_total_edge= n;
        return m_total_edge;
    }
    int getVertexValueSize() {
        m_n_value_size = sizeof(int);
        return m_n_value_size;
    }
    int getEdgeValueSize() {
        m_e_value_size = sizeof(double);
        return m_e_value_size;
    }
    int getMessageValueSize() {
        m_m_value_size = sizeof(bool);
        return m_m_value_size;
    }
    void loadGraph() {
        if (m_total_edge <= 0)  return;

        unsigned long long last_vertex;
        unsigned long long from;
        unsigned long long to;
        double weight = 0;
        
        int value = 0;
        int outdegree = 0;
        
        const char *line= getEdgeLine();

        sscanf(line, "%lld %lld", &from, &to);
        addEdge(from, to, &weight);

        last_vertex = from;
        ++outdegree;
        for (int64_t i = 1; i < m_total_edge; ++i) {
            line= getEdgeLine();
            sscanf(line, "%lld %lld", &from, &to);
            if (last_vertex != from) {
                addVertex(last_vertex, &value, outdegree);
                last_vertex = from;
                outdegree = 1;
            } else {
                ++outdegree;
            }
            addEdge(from, to, &weight);
        }
        addVertex(last_vertex, &value, outdegree);
    }
};

class VERTEX_CLASS_NAME(OutputFormatter): public OutputFormatter {
public:
    void writeResult() {
        int64_t vid;
        int value;
        char s[1024];

        for (ResultIterator r_iter; ! r_iter.done(); r_iter.next() ) {
            r_iter.getIdValue(vid, &value);
            if(value >= standardKValue){
                int n = sprintf(s, "%lld\n", (unsigned long long)vid);
                writeNextResLine(s, n);
            }
        }
    }
};


// An aggregator that records a double value tom compute sum, not that important here
class VERTEX_CLASS_NAME(Aggregator): public Aggregator<double> {
public:
    void init() {
        m_global = 0;
        m_local = 0;
    }
    void* getGlobal() {
        return &m_global;
    }
    void setGlobal(const void* p) {
        m_global = * (double *)p;
    }
    void* getLocal() {
        return &m_local;
    }
    void merge(const void* p) {
        m_global += * (double *)p;
    }
    void accumulate(const void* p) {
        m_local += * (double *)p;
    }
};

class VERTEX_CLASS_NAME(): public Vertex<int, double, bool>{
public:
    void compute(MessageIterator* pmsgs){
        if(getSuperstep() == 0){
            // *********************initializing part ***********
            // initialize, set every vertex value to be their initial outdegree
            int initialdegree = m_pme -> m_out_degree;
            *mutableValue() = initialdegree;
            printf("First stage: m_out_degree = %d *value= %d\n", m_pme -> m_out_degree, getValue());
            // if initialdegree < standardKValue, send "true"
            Vertex<int, double, bool>::OutEdgeIterator iter = getOutEdgeIterator();
            for(; !iter.done(); iter.next()){
                if(initialdegree < standardKValue){
                    bool value = true;
                    sendMessageTo(iter.target(), value);
                }
                else{
                    bool value = false;
                    sendMessageTo(iter.target(), value);
                }
            }
        }

        else{
            // **** 1. compute current degree
            int current_degree = getValue();
            for(; !pmsgs -> done(); pmsgs -> next()){
                if(pmsgs -> getValue() == true){
                    // means need to delete a pointer.
                    current_degree--;
                }
            }
            
            //***** 2. decide superstep stop.
            if(getSuperstep() >= 2){
                double global_val = *(double *)getAggrGlobal(0);
                if(global_val == 0 || getValue() == 0){
                    voteToHalt();
                    return;
                }
            }

            if(current_degree < standardKValue){
                double acc = fabs((double)(getValue() - 0));
                accumulateAggr(0, &acc);
                *mutableValue() = 0;
            }
            else{
                double acc = fabs((double)(getValue() - current_degree));
                accumulateAggr(0, &acc);
                *mutableValue() = current_degree;
            }

            Vertex<int, double, bool>::OutEdgeIterator iterr = getOutEdgeIterator();
            for(; !iterr.done(); iterr.next()){
                if(current_degree < standardKValue){
                    bool value = true;
                    sendMessageTo(iterr.target(), value);
                }
                else{
                    bool value = false;
                    sendMessageTo(iterr.target(), value);
                }
            }
        }
    }    
};

class VERTEX_CLASS_NAME(Graph): public Graph {
public:
    VERTEX_CLASS_NAME(Aggregator)* aggregator;

public:
    // argv[0]: PageRankVertex.so
    // argv[1]: <input path>
    // argv[2]: <output path>
    void init(int argc, char* argv[]) {

        setNumHosts(5);
        setHost(0, "localhost", 1411);
        setHost(1, "localhost", 1421);
        setHost(2, "localhost", 1431);
        setHost(3, "localhost", 1441);
        setHost(4, "localhost", 1451);

        if (argc < 4) {
           printf ("Usage: %s <input path> <output path> <K>\n", argv[0]);
           exit(1);
        }

        m_pin_path = argv[1];
        m_pout_path = argv[2];
        standardKValue = atoi(argv[3]);

        aggregator = new VERTEX_CLASS_NAME(Aggregator)[1];
        regNumAggr(1);
        regAggr(0, &aggregator[0]);
    }

    void term() {
        delete[] aggregator;
    }
};

/* STOP: do not change the code below. */
extern "C" Graph* create_graph() {
    Graph* pgraph = new VERTEX_CLASS_NAME(Graph);

    pgraph->m_pin_formatter = new VERTEX_CLASS_NAME(InputFormatter);
    pgraph->m_pout_formatter = new VERTEX_CLASS_NAME(OutputFormatter);
    pgraph->m_pver_base = new VERTEX_CLASS_NAME();

    return pgraph;
}

extern "C" void destroy_graph(Graph* pobject) {
    delete ( VERTEX_CLASS_NAME()* )(pobject->m_pver_base);
    delete ( VERTEX_CLASS_NAME(OutputFormatter)* )(pobject->m_pout_formatter);
    delete ( VERTEX_CLASS_NAME(InputFormatter)* )(pobject->m_pin_formatter);
    delete ( VERTEX_CLASS_NAME(Graph)* )pobject;
}