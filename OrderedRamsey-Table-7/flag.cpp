/* 

TODO:

 - The program can be more efficient by precomputing products of things.
   For each H, one can remember F1xF2 for it? Maybe.

 - Make -elcsq calculate partial unlabeles and over many types. Jan has an example
   where this is needed.

 - The program likes to sample vertices uniformly.
   It is hard to tell the program to sample vertices
   sayin color preserving mode. This is useful for crossing number
   of bipartite graphs.
   Maybe use "partition" to describe this instead of vertex color

  - TCE add test with products... / tune it a little

  - OBJE make better writeout in -latex so checking the corectness of the constraint input is easier...

  - fix load_unlabeled_flags_from_file to be efficient

  - make an extended objective function printing in latex

  - allow fractions and other expressions as coefficients (done in extended version)

  - make extended labeled flags!!!
    This should help with rounding as maybe sometimes only some products are actually needed.
    (done)

  - tweak for -elpc that will expand the constraints and use simple constraints instead. Hope is that
    it will be faster this way. Let's see.

  - Make automatic expansions of graphs with 0 in them. That means, when we are loading a flag with coefficient
    and it turns out that it has a 0 in it, one could replace it with the expanded list as it is loaded.

  - Repeated entries in linear constraints are summed up
 
  - make only part of the input constraints - maybe needs to be a separate program.
    Trick is that it should make solution easier. Maybe. 

  - optional name for unlabeled flags - say K_44 and K_35 inputs.... mabe makes life easier?

  - new features -
      - crossing quadruples
      - cyclically ordered vertices
 
    maximize  t  s.t. A/B <= t
    translates to 
    maximize  t  s.t. A <= B.t
    Here we try it...
 
  - rename g_flags  g_flags_cauchy
  - use only lex_min flags when generating them - maybe does not work
    together with the incremental generation
  - Allow the program on n vertices to use flags from programs on n-1 or n-2 vertices
    (if theere are too many, this will make the thing solve faster, but worse result)
  - count_flag_products could be enhanced by first picking the theta and checking that
    it has a proper type
  - proccess output from sdpa
  - do partial unlabaleing of flags for counting the stuff....
  - use product tables (maybe not)
  - polish linear constraints
  - disable unlabeled products for baber 3-graph constraints
  - compute lower-bound as a complement of upper bound
  - Jan's additional constraints for any problem:
    [ x-v \times \left(\sum \alpha_i F_i^2 \right)_unlabel ]_unlabel

  - fix -tt to be able to go through a bunch of different things in one file at once
    and make it more useful in combination with -fpv

  - Better check for CFLAGFILEcontraints labeled the same when doing elcp
 
Some hints:
  export OMP_NUM_THREADS=16

Use:
 Write Makefile that looks like:

CFLAGS= -DG_COLORED_EDGES=2 
SRC=../flag.cpp  
 
 -include ../Makefile.include

And compile. Different targets are available. 

 
Files and file formats:
 
 Problem:
    min/max c_1xG_1 + c_2xG_2 + c_3xG_3
 subject to
    c_4xG_4 + c_5xG_5 + c_6 \geq 0
    c_7xG_7 + c_8xG_8 + c_9 \geq 0
 
 Objective file: (F_edges2__objective.txt)
   c_1 G_1
   c_2 G_2
   c_3 G_3
 0
   c_6
   c_4 G_5
   c_5 G_5
 0
   c_9
   c_7 G_7
   c_8 G_8
 0
 Optional program description (that does not start with a number)

See the the beginning for different defines for compilation.
 
Features: 
  * Currently, the additional constraints are used as
     k( c_4xG_4 + c_5xG_5 + c_6 ) \geq 0  , where k is some variable
     k(c_4xG_4 + c_5xG_5 + c_6) * (G_i) \geq 0, where # of vertices of G_i+G_4 is at most -n 
  * Many parts of the program use OpenMP. You will experience a significant speedup when running
    the program on a multicore, shared memory machine
  * The whole thing is in one file because we hope that compiler might do some interprocedural optimization
    and it is easier to transfer the program between multiple machines.
  * Provides many other small utilities to explore the solutions or perform
    flag algebra calculations.

 Shortening the file:
 unifdef -UG_COLORED_VERTICES  -UG_COLORED_VERTICES_BLIND  -UG_COLORED_EDGES -UG_COLORED_EDGES_BLIND -UG_3EDGES  -UG_ROOTED_3EDGES  -UG_COLORED_3EDGES -UG_COLORED_3EDGES_BLIND -UG_COLORED_3EDGES_PRINT_HUMAN -UG_4EDGES -UG_ROOTED_4EDGES -UG_4EDGES_HACK -UG_ROTATION_SYSTEM -UG_ROTATION_SYSTEM_REVERSE_BLIND -UG_ORDERED_VERTICES -UG_USE_PERMUTATION_GRAPHS -UG_USE_BABER_EQUATIONS  -UG_TOURNAMENT  -UG_USE_FIRST_EDGE_COLOR_FOR_BLOWUP_ONLY -UG_USE_PERMITTED_SUBFLAGS -UWRITE_INTEGER_DENSITIES_FOR_LINEAR_CONSTRAINTS -UG_EARLY_FORBIDDEN_TEST_IN_GENERATING -UG_CROSSINGS -UG_CUTTINGS -UG_LEFTRIGHT -UG_LEFTRIGHT_LOCAL -UG_ORDER_TYPES -UG_ORIENTED_EDGES_BLIND -UG_LEFTRIGHT_BLIND -UG_USE_LEXMIN_FOR_ISOMORPHISM -UG_USE_LEXMIN_FOR_ISOMORPHISM  -UG_ORIENTED_EDGES -UUSE_FOR_CROSSINGS -UUSE_SQUARE_LINEAR_CONSTRAINTS  -UG_ENABLE_FIND_EXTREMAL_VECTORS -UG_ENABLE_HACKS -UG_ENABLE_FLAG_CALCULATOR -UG_USE_INDEXES_FOR_LARGE_UNLABELED_FLAGS -UG_MAYBE_ROOTED_KEDGES flag.cpp

 unifdef -UG_COLORED_VERTICES  -UG_COLORED_VERTICES_BLIND -UG_3EDGES  -UG_ROOTED_3EDGES  -UG_COLORED_3EDGES -UG_COLORED_3EDGES_BLIND -UG_COLORED_3EDGES_PRINT_HUMAN -UG_4EDGES -UG_4EDGES_HACK -UG_ROTATION_SYSTEM -UG_ROTATION_SYSTEM_REVERSE_BLIND -UG_ORDERED_VERTICES -UG_USE_PERMUTATION_GRAPHS -UG_USE_BABER_EQUATIONS  -UG_TOURNAMENT  -UG_USE_FIRST_EDGE_COLOR_FOR_BLOWUP_ONLY -UG_USE_PERMITTED_SUBFLAGS -UWRITE_INTEGER_DENSITIES_FOR_LINEAR_CONSTRAINTS -UG_EARLY_FORBIDDEN_TEST_IN_GENERATING -UG_CROSSINGS -UG_CUTTINGS -UG_LEFTRIGHT -UG_LEFTRIGHT_LOCAL -UG_ORDER_TYPES -UG_ORIENTED_EDGES_BLIND -UG_LEFTRIGHT_BLIND -UG_USE_LEXMIN_FOR_ISOMORPHISM -UG_USE_LEXMIN_FOR_ISOMORPHISM  -UG_ORIENTED_EDGES -UUSE_FOR_CROSSINGS -UUSE_SQUARE_LINEAR_CONSTRAINTS  -UG_ENABLE_FIND_EXTREMAL_VECTORS -UG_ENABLE_HACKS  -UG_USE_INDEXES_FOR_LARGE_UNLABELED_FLAGS -UG_MAYBE_ROOTED_KEDGES flag.cpp


 Change log:
   2023-03  - flag calculator can calculate with matrices to some extent
   2023-02  - flag calculator division fixed
   2022-12  - g_flags where type has only 1 entry is ignored
   2022-10  - added G_ORIENTED_EDGES_UNORIENTED_COLORS
   2022-09  - added G_COLORED_VERTICES_SAMPLED_SEPARATELY_BY_COLORS
   2012-08  - added rooted k-edges
   2022-05  - added rooted 3-edges and 4-edges
   2022-05  - added support for using mosek
   2022-04  - added G_ROOTED_3EDGES 
   2022-04  - tce tests with products but it still needs some work...
   2022-04  - added [ ] and @  to flag calculator
   2022-03  - G_FREE_OBJECTIVE added
   2022-03  - command -ace -helpe, -obje added
   2022-03  - nice flag rotation & labeled vertices described
   2022-03  - First version of -fc implemented, including coefficients calculator using {  }
   2022-02  - generation of labeled flags fixed for speed
   2020-08  - removing duplicates in constraints in parallel
   2020-07  - universal reading from file/cin/filename
   2020-06  - added code for dumping sdp part of constraints  -p
 
Makefile
CFLAGS=-DG_COLORED_EDGES=2
SRC=../flag.cpp

-include ../Makefile.include

 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <iterator>
#include <vector>
#include <set>
#include <utility>
#include <assert.h>
#include <cstring>
#include <algorithm>
#include <cstdarg>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <limits>
#include <limits.h>
#include <unistd.h>
#include <iomanip>  
#include <errno.h>
#include <time.h>
#include <unordered_map>
#include <sys/types.h> 
#include <sys/wait.h> 
//#include <filesystem>
#include <unordered_set>

#ifdef _USING_OMP_
#include <omp.h>
#endif

using namespace std;

/*
// This is just for the editor to let something 
#ifndef G_COLORED_EDGES
#define G_COLORED_EDGES 2
#endif
*/

// This is a Hack for rainbow problems
// Suppose you have k colors and you are coloring edges without a K_4
// that contains all 4 of them. And any edge can have up to all 4 colors.
// They are encoded binary.
//#define HACK_ALL_COLOR_FORBIDDEN
//#define HACK_ALL_COLOR_FORBIDDEN_COLORS 4
//#define HACK_ALL_COLOR_FORBIDDEN_COLORS 5
//#define HACK_ALL_COLOR_FORBIDDEN_COLORS 5

//#define G_DEFAULT_EXTENDED_OBJECTIVE

/*

// Main settings
 
#define G_COLORED_VERTICES        //=x     number of possible colors for vertices. Using colors (1..x)
#define G_COLORED_VERTICES_BLIND  //       vertex color-blind version     
#define G_COLORED_EDGES           //=x     2-edges have x colors (1...x)
#define G_COLORED_EDGES_BLIND     //       partial edge color blind. Reads permutations of colors from a file
#define G_ORIENTED_EDGES          //       1 is not flipped but all other colors have version with +,- for orientations. i.e, 1 can be used in place of non-edge
                                  //       Some people call it skew adjaceny matrix
#define G_ORIENTED_EDGES_BLIND    //       the actuall orientation does not matter anc may be flipped
#define G_3EDGES                  //       enable 3-edges
#define G_ROOTED_3EDGES           //       enable rooted 3-edges
#define G_COLORED_3EDGES          //=x     3-edges have colors (1..x)
#define G_COLORED_3EDGES_BLIND    //       permutations of colorings for 3-edges is loaded from a file
#define G_COLORED_3EDGES_PRINT_HUMAN // human friendly output for 3-edges (costs more hardddrive space)
#define G_4EDGES                  //       enable 4-edges
#define G_ROOTED_4EDGES           //       enable rooted 4-edges
#define G_ROTATION_SYSTEM         //        
#define G_ROTATION_SYSTEM_REVERSE_BLIND

#define G_MAYBE_ROOTED_KEDGES     //=x     enable x-edges that possibly have roots
#define G_MAYBE_ROOTED_KEDGES_DISABLE_ROOTS
#define G_MAYBE_ROOTED_KEDGES_FORCE_ROOTS
#define G_MAYBE_ROOTED_KEDGES_FIRST // =x  first vertex either  0 or 1   
#define G_ORDERED_VERTICES        // not working - usefull for permutations
 
#define G_USE_PERMUTATION_GRAPHS  // Enables generations of graphs af permutation graphs and sets G_COLORED_EDGES=2
 
#define G_USE_BABER_EQUATIONS     //       enables baber's equations.
 
#define G_TOURNAMENT             // define use as a tournament -  in that case color 1 is skipped when generating graphs. Like forbodding color 1 but more effective

#define G_USE_FIRST_EDGE_COLOR_FOR_BLOWUP_ONLY // If the program is working with blow-up, it is more effective to generate the small graphs and then do
                                               // blow-ups. The color used for blowing-up will be color 1. Faster than using forbidden subgraphs.
#define G_USE_PERMITTED_SUBFLAGS // If the number of forbidden subflags of some size is too big, you may enumerate permitted subflags of some size in addition
                                  // to forbidden subflags
#define WRITE_INTEGER_DENSITIES_FOR_LINEAR_CONSTRAINTS  // it will modify the linear constraints by multiplying them by some constant and hope the numbers
                                  // from linear constraints that appear in the sdp will be integers

#define G_FORBIDDEN_NON_INDUCED  // Use non-induced forbidden test. Useful for hypergragps maybe? It is slower
 
#define G_REMOVE_ORIENTATION_WHEN_LOADING // removes orientation - useful when input has orientation but it is no longer desired. 

// Tweaks - maybe not used

#define G_EARLY_FORBIDDEN_TEST_IN_GENERATING  // when coloring edges, test for forbidden graphs after every subgraph 0...x is entirely colored
#define G_FLAG_PRODUCTS_SLOW_LOW_MEMORY       // really slow, dont do this
#define G_USE_LEXMIN_FOR_ISOMORPHISM     // there was an attempt to do this but it does not work fast
 
// Hacks and others - almost surely don't use :-)

#define USE_FOR_CROSSINGS  // Makes some special modifications when run for crossing number..
#define G_CROSSINGS    // [a][b][c][d]  if segment ab crosses segmen cd    - cannot be generated by this program
#define G_CROSSINGS_HUMAN_READABLE
#define G_CUTTINGS     // [a][b][c][d]  if line ab cuts segment cd         - cannot be generated by this program
#define G_LEFTRIGHT    // [a][b][c]     if c on left or right of line ab   - cannot be generated by this program
#define G_LEFTRIGHT_LOCAL // left/right is not a global property - just matters # to one side and another side but does not distinguish the sides
#define G_LEFTRIGHT_BLIND // allows reflection 
#define G_CROSSINGS_SIGNED   //  if segment ab crosses segmen cd and +1 or -1 based on orientation of the crossing

 #define G_ORDER_TYPES             // Enables order types. Actually, order types are done by G_LEFTRIGHT, but this takes the coordinates 
// so it canbe used in drawing.

#define G_COLORED_VERTICES_SAMPLED_SEPARATELY_BY_COLORS  // Colored vertices sampled separately. useful for sampling bipartite graphs in crossing number.
 
#define G_DISABLE_MAIN

#define G_ORIENTED_EDGES_UNORIENTED_COLORS // this is up to which colors there is no orientation. By default, this number is set to 1 as
                                            // it inidcates non-edges

#define G_EXTRNAL_HACK_CPP

 */


#ifndef V
#define V      11  // Maximum number of vertices of flags. If you know you need smaller, use smaller - will be faster and less memory consuming
#endif

#define G_ELCP_BY_DEFAULT // Enable linear constraints products by default

#define G_NOT_ALL_FLAGS_USED // Some extra warnings if there are unexpected flags. This should be enabled

#define G_USING_ZERO_AS_ANY_COLOR // If something has color 0, it is considered to have anycolor or being uncolored. Usefull when trying to find extensions of some graph

#define G_BE_BRAVE_AND_IGNORE_SAFETY_ASSERTS // Will be slower, but if not defined. But if you don't trust the program, comment it :-)

#ifndef G_ORDERED_VERTICES
#define USE_REDUCED_TYPES // 4 4 1:123  and 4 4 1:234 are considered redundant and only one is used. This you want to have on. Not having it is stupid unless you have ordered vertices
#endif

#ifdef G_USE_PERMUTATION_GRAPHS
#ifndef G_COLORED_EDGES
#define G_COLORED_EDGES 2
#endif
#endif

#define MAX_FLAG_TYPES  300000 // Just a constant - raise if neded - means different theta


//#define G_FREE_OBJECTIVE   // This controls how is the objective function built. If defined, allows for negative value of the objective

//#define WRITE_INTEGER_DENSITIES_FOR_LINEAR_CONSTRAINTS

// Imagine you have many linear constraints and these constraints
// are with unlabeled flags of the same size as -n flags
// Then it is faster to identify the index of each graph in the constraint once
// than always test for isomorphism. 
#define G_USE_INDEXES_FOR_LARGE_UNLABELED_FLAGS

// If not specified, we assume that color 1 is for non-edges and hence we these edges are not oriented.
#ifdef G_ORIENTED_EDGES
#ifndef G_ORIENTED_EDGES_UNORIENTED_COLORS
#define G_ORIENTED_EDGES_UNORIENTED_COLORS 1
#endif
#endif

#ifdef G_COLORED_VERTICES_BLIND 
#ifdef G_COLORED_VERTICES_SAMPLED_SEPARATELY_BY_COLORS
#error "G_COLORED_VERTICES_BLIND and G_COLORED_VERTICES_SAMPLED_SEPARATELY_BY_COLORS at the same time are not correctly implemented"
#endif
#endif


bool g_use_simple_linear_constraints = true;
vector<int>  g_use_simple_linear_constraints_list;


#ifdef G_ELCP_BY_DEFAULT
bool g_use_product_linear_constraints = true;
#else
bool g_use_product_linear_constraints = false;
#endif

bool g_use_product_linear_constraints_by_expanding = false;
bool g_use_square_linear_constraints = false;
bool g_use_linear_constraints_self_products = false;


//#define USE_SQUARE_LINEAR_CONSTRAINTS


//#define DISABLE_UNLABELED_PRODUCTS

#ifdef G_MAYBE_ROOTED_KEDGES
#ifndef G_MAYBE_ROOTED_KEDGES_FIRST
#define G_MAYBE_ROOTED_KEDGES_FIRST  0
#endif
#endif

//#define G_CROSSINGS

#ifdef G_ORIENTED_EDGES
#ifndef G_COLORED_EDGES
#error Oriented edges only work if normal edges are set up
#endif
#endif


#ifdef G_CROSSINGS_SIGNED
#ifndef G_CROSSINGS
#define G_CROSSINGS
//#error One needs to enable crossings ad well....
#endif
#endif



#ifdef USE_FOR_CROSSINGS
	//#define G_COLORED_VERTICES = 2 for 2-partite, and 3 for tripartite
	//#define G_CROSSINGS
	//#define G_CUTTINGS
    //#define G_LEFTRIGHT
	//const int     Kn = VERTICES_KN;
#endif




// Copy to +1 counterparts.... to make life easier
#ifdef G_COLORED_VERTICES
#define COLORS_VERTICES (G_COLORED_VERTICES+1)
#endif
#ifdef G_COLORED_EDGES
#define COLORS_EDGES (G_COLORED_EDGES+1)
#endif
#ifdef G_COLORED_3EDGES
#define COLORS_3EDGES (G_COLORED_3EDGES+1)
#endif


#ifndef G_BLOW_UP_COLOR_EDGES
#define G_BLOW_UP_COLOR_EDGES  1
#endif

#ifndef G_BLOW_UP_COLOR_3EDGES
#define G_BLOW_UP_COLOR_3EDGES 1
#endif

#ifndef G_BLOW_UP_COLOR_4EDGES
#define G_BLOW_UP_COLOR_4EDGES 1
#endif


#ifndef G_PRECISION
#define G_PRECISION 16
#endif

#define G_PREFER_MOSEK

int g_blow_up_color_edges[V]; // adds more precision in specifying the blow-up
int g_blow_up_color_3edges[V]; // adds more precision in specifying the blow-up
int g_blow_up_color_4edges[V]; // adds more precision in specifying the blow-up


#ifdef G_EDGE_COLOR_SYMMETRY
#define G_LOAD_COLORED_EDGES_BLIND_PERMUTATIONS
#endif


#ifdef G_COLORED_EDGES_BLIND
#define G_LOAD_COLORED_EDGES_BLIND_PERMUTATIONS
#endif



#ifdef G_LOAD_COLORED_EDGES_BLIND_PERMUTATIONS
vector<vector<int> > g_allowed_edgecolor_permutations;  // permutations of colors of edges
#endif

#ifdef G_COLORED_3EDGES_BLIND
vector<vector<int> > g_allowed_3edgecolor_permutations;  // permutations of colors of 3-edges
#endif


#ifdef G_COLORED_VERTICES_BLIND
vector<vector<int> > g_allowed_vertexcolor_permutations;  // permutations of colors of 3-edges
#endif



#ifdef DONT_USE_C11
double stod(const string&  str)
{
    stringstream ss(str);
    double d;
    ss >> d;
    return d;
}

long stol(const string&  str)
{
    stringstream ss(str);
    long l;
    ss >> l;
    return l;
}

string to_string (long long val)
{
    stringstream ss;
    ss << val;
    return ss.str();
}
#endif


//#ifndef G_MAYBE_ROOTED_KEDGES
//#define G_MAYBE_ROOTED_KEDGES 4
//#endif

#ifdef G_MAYBE_ROOTED_KEDGES
class rooted_kedge
{
public:
    unordered_set<int> vertices;
    int root; // == -1 means no root

    rooted_kedge()
    {
        root = -1;
    }

    rooted_kedge(vector<int> v, int r=-1)
    {
        for (int i : v)
        {
            vertices.insert(i);
        }
        root = r;
    }

    bool max_entry_is(int v) const
    {
        int maxi = -1;
        for(int i : vertices)
        {
            if (i > v)
                return false;
            if (maxi < i)
                maxi = i;
        }
        //cerr << "Fond max = " << maxi << endl;
    
        return maxi == v;
    }    

    int max_entry() const
    {
        int maxi = -1;
  
        for(int i : vertices)
        {
            if (maxi < i)
                maxi = i;
        }

        return maxi;
    }   

    friend ostream &operator<<( ostream &output, const rooted_kedge &re )
    {
        vector<int> v(re.vertices.begin(), re.vertices.end());
        sort(v.begin(), v.end());

        for (int x : v)
        {
//            output << (char)(x+'0');
            output << (char)(x+'0'+G_MAYBE_ROOTED_KEDGES_FIRST);

        }    
        if (re.root != -1)
        {
//            output << ">" <<  (char)(re.root+'0');
            output << ">" <<  (char)(re.root+'0'+G_MAYBE_ROOTED_KEDGES_FIRST);
        }
        return output;
    }
    friend istream &operator>>( istream &input, rooted_kedge &re ) 
    {
        string str_rooted_Kedge;
        input >> str_rooted_Kedge;

        if (str_rooted_Kedge.size() != G_MAYBE_ROOTED_KEDGES && str_rooted_Kedge.size() != G_MAYBE_ROOTED_KEDGES+2)
        {
            cerr << "Error while interpreting '" << str_rooted_Kedge << "' as k-edge. Expecting " << G_MAYBE_ROOTED_KEDGES << " or " << G_MAYBE_ROOTED_KEDGES+2 << " characters" << endl;
            assert(0);
        }

        re.vertices.clear();

        for (int i = 0; i < G_MAYBE_ROOTED_KEDGES; i++)
        {
//            re.vertices.insert(str_rooted_Kedge[i]-'0');
            re.vertices.insert(str_rooted_Kedge[i]-'0'-G_MAYBE_ROOTED_KEDGES_FIRST);
        }

        if (str_rooted_Kedge.size() == G_MAYBE_ROOTED_KEDGES+2)
        {
            assert(str_rooted_Kedge[G_MAYBE_ROOTED_KEDGES]=='>');
//            re.root = (int)(str_rooted_Kedge[G_MAYBE_ROOTED_KEDGES+1]-'0');
            re.root = (int)(str_rooted_Kedge[G_MAYBE_ROOTED_KEDGES+1]-'0'-G_MAYBE_ROOTED_KEDGES_FIRST);
            assert(re.vertices.find(re.root) != re.vertices.end());
        }
        else 
        {
            re.root = -1;
        }
        return input;
   }

};
inline bool operator==(const rooted_kedge& lhs, const rooted_kedge& rhs)
{ 
    if (lhs.root != rhs.root) return false;
    return lhs.vertices == rhs.vertices; 
}
inline bool operator<(const rooted_kedge& lhs, const rooted_kedge& rhs)
{ 
    if (lhs.root < rhs.root) return true;
    if (lhs.root > rhs.root) return false;

    // OK, root is the same
    for (int v = 0; v < V; v++)
    {
        int cvlhs = lhs.vertices.count(v);
        int cvrhs = rhs.vertices.count(v);
        if (cvlhs > cvrhs) return true;
        if (cvlhs < cvrhs) return false;
    }

    // They are equal I guess
    return false;
}
#endif


// This is the main class that holds a flag. 
class flag
{
public:
    flag()
    {
		m_Theta = 0;
        m_Theta_class = 0;
		m_vertices = 0;

#ifdef G_USE_INDEXES_FOR_LARGE_UNLABELED_FLAGS
        m_unlabeled_id = -1;
#endif

#ifdef G_ORDERED_VERTICES
        m_labeled_vertices_cnt = 0;
        for(int v = 0; v < V; v++)
        {
            m_labeled[v] = false;
        }
        update_labeledvector();
#endif
    }

    flag(const string &str) : flag()
    {
        stringstream ss(str);
        load_from_stream(ss, -1, -1);
    }

    virtual ~flag()
    {
        
    }
    
    
    
#ifdef G_ORDERED_VERTICES
    void update_labeledvector()
    {
        int i = 0;
        for (int v = 0; v < m_vertices; v++)
        {
            if (m_labeled[v]) m_labeled_vector[i++] = v;
        }
        assert(i == m_labeled_vertices_cnt);
    }
    
    void set_labeled(const int v)
    {
        assert(v < m_vertices);
        if (is_labeled(v)) return;
        m_labeled[v] = true;
        m_Theta = m_Theta | (1 << v);
        m_labeled_vertices_cnt++;
        update_labeledvector();
    }
    
    void clear_labeled(const int v)
    {
        assert(v < m_vertices);
        if (!is_labeled(v)) return;
        m_labeled[v] = false;
        m_Theta = m_Theta & (~(1 << v));
        m_labeled_vertices_cnt--;
        update_labeledvector();
    }
#endif    
    
    void make_all_vertices_labeled()
    {
#ifdef G_ORDERED_VERTICES
        set_Theta(~(0xFFFFFFFF << m_vertices));
#else
        m_Theta = m_vertices;
#endif
#ifdef G_USE_INDEXES_FOR_LARGE_UNLABELED_FLAGS
        m_unlabeled_id = -1;
#endif
    }
    
    bool is_labeled(const int v) const
    {
        assert(v < m_vertices);
#ifdef G_ORDERED_VERTICES
        return m_labeled[v];
#else
        return  v < m_Theta;
#endif
    }
    
    // returns the number of labeled vertices
    int labeled_vertices_cnt() const
    {
#ifdef G_ORDERED_VERTICES
        return m_labeled_vertices_cnt;
#else
        return m_Theta;
#endif
    }    

    
    // First m_Theta vertices are considered labeled in that order
    void set_Theta(int Theta, int Theta_class = 0)
    {
        // if !G_ORDERED_VERTICES, then the labeled vertices are first. Not true for ordered.
#ifndef G_ORDERED_VERTICES
        //cerr << m_vertices << " " << Theta << endl;
        if (Theta > m_vertices)
        {
            cerr << "FAIL: Attempting to load grap on "<< m_vertices << " vertices with Theta=" << Theta << endl;
        }
        assert(Theta <= m_vertices);
        m_Theta = Theta;
#else
        m_Theta = 0;
        m_labeled_vertices_cnt = 0;
        for (int v = 0; v < m_vertices; v++)
            if (Theta & (1<<v))
            {
                m_Theta = m_Theta | (1 << v);
                m_labeled[v] = true;
                m_labeled_vertices_cnt++;
            }
            else m_labeled[v] = false;
        update_labeledvector();
#endif
        m_Theta_class = Theta_class;
    }
    
	
    int get_Theta()
    {
        return m_Theta;
    }
    
    void set_vertices_and_Theta(int vertices, int Theta, int Theta_class = 0)
    {
        set_vertices(vertices);
        set_Theta(Theta, Theta_class);
    }
	
	
    // set the nymber of vertices and clears the graph
    void set_vertices(int vertices)
    {
        assert(vertices <= V);
        m_vertices = vertices;
#ifdef G_USE_LEXMIN_FOR_ISOMORPHISM                
        m_minlex_signature = "";
#endif
        
#ifdef G_COLORED_VERTICES
		for (int u = 0; u < V; u++)
		{
			m_color_vertex[u] = 0;
		}
		m_colored_vertices[0] = m_vertices;
		for (int c = 1; c < COLORS_VERTICES; c++) m_colored_vertices[c] = 0;
#endif

		
#ifdef G_COLORED_EDGES
        for (int u = 0; u < V; u++)
        {
	        for (int v = 0; v < V; v++)
	        {
	            m_color_edge[u][v] = 0;
	        }
	    }
        
        m_colored_edges[0] = e();
        for (int c = 1; c < COLORS_EDGES; c++)  m_colored_edges[c] = 0;
#endif

#ifdef G_3EDGES
        for (int u1 = 0; u1 < V; u1++)
            for (int u2 = 0; u2 < V; u2++)
                for (int v1 = 0; v1 < V; v1++)
                    m_3edge[u1][u2][v1] = 0;
        m_3edges_cnt = 0;
#endif

#ifdef G_ROOTED_3EDGES
        for (int u1 = 0; u1 < V; u1++)
            for (int u2 = 0; u2 < V; u2++)
                for (int v1 = 0; v1 < V; v1++)
                    m_rooted_3edge[u1][u2][v1] = 0;
        m_rooted_3edges_cnt = 0;
#endif


#ifdef G_COLORED_3EDGES    
        for (int u1 = 0; u1 < V; u1++)
            for (int u2 = 0; u2 < V; u2++)
                for (int v1 = 0; v1 < V; v1++)
                    m_colored_3edge[u1][u2][v1] = 0;
        m_colored_3edges[0] = e3();
        for (int c = 1; c < COLORS_3EDGES; c++)  m_colored_3edges[c] = 0;
#endif       
        
#ifdef G_4EDGES
        for (int u1 = 0; u1 < V; u1++)
            for (int u2 = 0; u2 < V; u2++)
                for (int v1 = 0; v1 < V; v1++)
                    for (int v2 = 0; v2 < V; v2++)
                        m_4edge[u1][u2][v1][v2] = 0;
        m_4edges_cnt = 0;
#endif        

#ifdef G_ROOTED_4EDGES
        for (int u1 = 0; u1 < V; u1++)
            for (int u2 = 0; u2 < V; u2++)
                for (int v1 = 0; v1 < V; v1++)
                    for (int v2 = 0; v2 < V; v2++)
                        m_rooted_4edge[u1][u2][v1][v2] = 0;
        m_rooted_4edges_cnt = 0;
#endif     


#ifdef G_MAYBE_ROOTED_KEDGES
    m_rooted_kedges.clear();
#endif

#ifdef G_ROTATION_SYSTEM
        for (int u1 = 0; u1 < V; u1++)
        {
            for (int u2 = 0; u2 < V; u2++)
            {
                m_rotation_system[u1][u2] = -1;
            }
            m_rotation_system_neighbors[u1] = 0;
        }
        m_rotation_system_noncrosssings = 0;
#endif
        
        
#ifdef G_CROSSINGS
        // now crossing of pairs of edges
        for (int u1 = 0; u1 < V; u1++)
            for (int u2 = 0; u2 < V; u2++)
                for (int v1 = 0; v1 < V; v1++)
                    for (int v2 = 0; v2 < V; v2++)
                        m_cross_edges[u1][u2][v1][v2] = 0;
        m_crossings_cnt = -1;
        m_crossings_in_type = 0;
#endif
        
#ifdef G_CUTTINGS
        // now crossing of pairs of edges
        for (int u1 = 0; u1 < V; u1++)
            for (int u2 = 0; u2 < V; u2++)
                for (int v1 = 0; v1 < V; v1++)
                    for (int v2 = 0; v2 < V; v2++)
                        m_cuttings[u1][u2][v1][v2] = 0;
#endif
        
#ifdef G_LEFTRIGHT
        // now crossing of pairs of edges
        for (int u1 = 0; u1 < V; u1++)
            for (int u2 = 0; u2 < V; u2++)
                for (int v1 = 0; v1 < V; v1++)
                    m_leftright[u1][u2][v1] = 0;
#endif
        
#ifdef G_ORDER_TYPES
        for (int u = 0; u < V; u++)
        {
            m_order_type[u] = make_pair(0,0);
        }
#endif        

#ifdef G_USE_INDEXES_FOR_LARGE_UNLABELED_FLAGS
        m_unlabeled_id = -1;
#endif
    }



#ifdef G_COLORED_VERTICES
    void color_vertex(int u, int color)
    {
        if (color >= COLORS_VERTICES)
        {
            cerr << "Coloring vertex " << u << " with color " << color << " that is more than " << COLORS_VERTICES-1 << endl;
            cerr << "we have " << m_vertices << " vertices and " << m_Theta << " m_Theta" << endl;
        }

        assert(u < m_vertices);
        assert(color >= 0);
        assert(color < COLORS_VERTICES);
        
        m_colored_vertices[m_color_vertex[u]]--;
        
        m_color_vertex[u] = color;
        
        m_colored_vertices[color]++;
        
#ifdef G_USE_LEXMIN_FOR_ISOMORPHISM 
        m_minlex_signature.clear();
#endif      
    }
#endif
    

#ifdef G_COLORED_EDGES
    void color_edge(int u, int v, int color)
    {

#if defined(G_ORIENTED_EDGES) || defined(G_REMOVE_ORIENTATION_WHEN_LOADING)
        if (color < 0)
        {
            color_edge(v, u, -color);
            return;
        }        
#endif
    	assert(u != v);
    	assert(u < m_vertices);
    	assert(v < m_vertices);
    	assert(0 <= u);
    	assert(0 <= v);
    	assert(color >= 0);
        assert(color < COLORS_EDGES);
        
    	int old_color = m_color_edge[u][v];
#ifdef G_ORIENTED_EDGES
        if (old_color < 0) old_color = -old_color;        
#endif        
        
    	m_colored_edges[old_color]--;
    	
#ifdef G_ORIENTED_EDGES
        if (color > G_ORIENTED_EDGES_UNORIENTED_COLORS)   // defaul is 1, it is for not oriented edges, non-edges....
        {
            m_color_edge[u][v] =  color;
            m_color_edge[v][u] = -color;
        }
        else
        {
            m_color_edge[u][v] = m_color_edge[v][u] = color;
        }
#else
        m_color_edge[u][v] = m_color_edge[v][u] = color;
#endif
    	
    	m_colored_edges[color]++;

#ifdef G_USE_LEXMIN_FOR_ISOMORPHISM 
        m_minlex_signature.clear();
#endif
    }
#endif
    
    
#ifdef G_3EDGES
    void set_3edge(int x, int y, int z, int state)
    {
        assert (x < m_vertices);
        assert (y < m_vertices);
        assert (z < m_vertices);
        assert (0 <= x);
        assert (0 <= y);
        assert (0 <= z);
        assert (x != y && x != z && y != z);
        assert (state == 1 || state == 0);


        if (m_3edge[x][y][z] == state) return;
        m_3edge[x][y][z] = state;
        m_3edge[x][z][y] = state;
        m_3edge[y][x][z] = state;
        m_3edge[y][z][x] = state;
        m_3edge[z][x][y] = state;
        m_3edge[z][y][x] = state;
        if (state == 1) m_3edges_cnt++;
        else m_3edges_cnt--;
        
#ifdef G_USE_LEXMIN_FOR_ISOMORPHISM 
        m_minlex_signature.clear();
#endif      
    }
#endif 

#ifdef G_ROOTED_3EDGES
    void set_rooted_3edge(int x, int y, int z, int state)
    {
        assert (x < m_vertices);
        assert (y < m_vertices);
        assert (z < m_vertices);
        assert (0 <= x);
        assert (0 <= y);
        assert (0 <= z);
        assert (x != y && x != z && y != z);
        assert (state == 1 || state == 0);
        
        if (m_rooted_3edge[x][y][z] == state) return;
        m_rooted_3edge[x][y][z] = state;
        m_rooted_3edge[x][z][y] = state;
        m_rooted_3edge[y][x][z] = -state;
        m_rooted_3edge[y][z][x] = -state;
        m_rooted_3edge[z][x][y] = -state;
        m_rooted_3edge[z][y][x] = -state;
        if (state == 1) m_rooted_3edges_cnt++;
        else m_rooted_3edges_cnt--;
        
#ifdef G_USE_LEXMIN_FOR_ISOMORPHISM 
        m_minlex_signature.clear();
#endif      
    }
#endif 

    
#ifdef G_COLORED_3EDGES
    void color_3edge(int x, int y, int z, int color)
    {
        
        assert (x < m_vertices);
        assert (y < m_vertices);
        assert (z < m_vertices);
        assert (0 <= x);
        assert (0 <= y);
        assert (0 <= z);        
        assert(color >= 0);
        assert(color < COLORS_3EDGES);
        assert (x != y);
        assert (x != z);
        assert (z != y);
        
        if (m_colored_3edge[x][y][z] == color) return;
        m_colored_3edges[m_colored_3edge[x][y][z]]--;
        
        m_colored_3edge[x][y][z] = color;
        m_colored_3edge[x][z][y] = color;
        m_colored_3edge[y][x][z] = color;
        m_colored_3edge[y][z][x] = color;
        m_colored_3edge[z][x][y] = color;
        m_colored_3edge[z][y][x] = color;
        m_colored_3edges[color]++;
        
#ifdef G_USE_LEXMIN_FOR_ISOMORPHISM 
        m_minlex_signature.clear();
#endif     
    }
#endif        
 
#ifdef G_4EDGES
    void set_4edge(int x, int y, int z, int w, int state)
    {
        //cerr << x << " " << y << " " << z << " " << w << " " <<m_vertices << endl;
        assert (x < m_vertices);
        assert (y < m_vertices);
        assert (z < m_vertices);
        assert (w < m_vertices);
        assert (0 <= x);
        assert (0 <= y);
        assert (0 <= z);        
        assert (0 <= w);        
        assert (x != y && x != z && y != z && w != x && w != y && w != z) ;
        assert (state == 1 || state == 0);

        if (m_4edge[x][y][z][w] == state) return;        
        m_4edge[x][y][z][w] = state;
        m_4edge[x][z][y][w] = state;
        m_4edge[y][x][z][w] = state;
        m_4edge[y][z][x][w] = state;
        m_4edge[z][x][y][w] = state;
        m_4edge[z][y][x][w] = state;
        m_4edge[x][y][w][z] = state;
        m_4edge[x][z][w][y] = state;
        m_4edge[y][x][w][z] = state;
        m_4edge[y][z][w][x] = state;
        m_4edge[z][x][w][y] = state;
        m_4edge[z][y][w][x] = state;
        m_4edge[x][w][y][z] = state;
        m_4edge[x][w][z][y] = state;
        m_4edge[y][w][x][z] = state;
        m_4edge[y][w][z][x] = state;
        m_4edge[z][w][x][y] = state;
        m_4edge[z][w][y][x] = state;          
        m_4edge[w][x][y][z] = state;
        m_4edge[w][x][z][y] = state;
        m_4edge[w][y][x][z] = state;
        m_4edge[w][y][z][x] = state;
        m_4edge[w][z][x][y] = state;
        m_4edge[w][z][y][x] = state; 
        if (state == 1) m_4edges_cnt++;
        else m_4edges_cnt--;
        
#ifdef G_USE_LEXMIN_FOR_ISOMORPHISM 
        m_minlex_signature.clear();
#endif     
    }
#endif  
    
#ifdef G_ROOTED_4EDGES
    void set_rooted_4edge(int x, int y, int z, int w, int state)
    {
        //cerr << x << " " << y << " " << z << " " << w << " " <<m_vertices << endl;
        assert (x < m_vertices);
        assert (y < m_vertices);
        assert (z < m_vertices);
        assert (w < m_vertices);
        assert (0 <= x);
        assert (0 <= y);
        assert (0 <= z);        
        assert (0 <= w);        
        assert (x != y && x != z && y != z && w != x && w != y && w != z) ;
        assert (state == 1 || state == 0);
        
        if (m_rooted_4edge[x][y][z][w] == state) return;        
        m_rooted_4edge[x][y][z][w] = state;
        m_rooted_4edge[x][z][y][w] = state;
        m_rooted_4edge[x][y][w][z] = state;
        m_rooted_4edge[x][z][w][y] = state;
        m_rooted_4edge[x][w][y][z] = state;
        m_rooted_4edge[x][w][z][y] = state;

        m_rooted_4edge[y][x][z][w] = -state;
        m_rooted_4edge[y][z][x][w] = -state;
        m_rooted_4edge[z][x][y][w] = -state;
        m_rooted_4edge[z][y][x][w] = -state;
        m_rooted_4edge[y][x][w][z] = -state;
        m_rooted_4edge[y][z][w][x] = -state;
        m_rooted_4edge[z][x][w][y] = -state;
        m_rooted_4edge[z][y][w][x] = -state;
        m_rooted_4edge[y][w][x][z] = -state;
        m_rooted_4edge[y][w][z][x] = -state;
        m_rooted_4edge[z][w][x][y] = -state;
        m_rooted_4edge[z][w][y][x] = -state;          
        m_rooted_4edge[w][x][y][z] = -state;
        m_rooted_4edge[w][x][z][y] = -state;
        m_rooted_4edge[w][y][x][z] = -state;
        m_rooted_4edge[w][y][z][x] = -state;
        m_rooted_4edge[w][z][x][y] = -state;
        m_rooted_4edge[w][z][y][x] = -state; 
        if (state == 1) m_rooted_4edges_cnt++;
        else m_rooted_4edges_cnt--;
        
#ifdef G_USE_LEXMIN_FOR_ISOMORPHISM 
        m_minlex_signature.clear();
#endif     
    }
#endif  	
    

#ifdef G_MAYBE_ROOTED_KEDGES
    void add_maybe_rooted_kedge(rooted_kedge e)
    {
        assert(e.vertices.size() == G_MAYBE_ROOTED_KEDGES);
        if (e.root < -1 || e.root >= m_vertices)
        {
                cerr << "Invalid root for edge '" << e  << "'" << endl;
                assert(0);
        }
        for (auto v : e.vertices)
        {
            if (v < 0 || v >= m_vertices)
            {
                cerr << "Invalid edge '" << e  << "' contains vertex " << v << " that is < 0 or >= " << m_vertices << endl;
                assert(0);
            }
        }
        m_rooted_kedges.insert(e);
    } 

    void delete_maybe_rooted_kedge(rooted_kedge e)
    {
        m_rooted_kedges.erase(e);
    } 
#endif

#ifdef G_COLORED_VERTICES
//    void permute_vertex_colors(int color_permutation[])
    void permute_vertex_colors(const vector<int> & color_permutation)
    {
        for (int v = 0; v < m_vertices; v++)
        {
            color_vertex(v, color_permutation[m_color_vertex[v]]);
        }
    }
#endif



#ifdef G_COLORED_EDGES
    // Makes a permutation of edges of this flag
    // Maybe be rewritten for higher effectivity
    void permute_edge_colors(const vector<int> &color_permutation)
    {
        assert (color_permutation.size() >= COLORS_EDGES);
        
        for (int u = 0; u < m_vertices-1; u++)
            for (int v = u+1; v < m_vertices; v++)
        {
#ifdef G_ORIENTED_EDGES
            if (m_color_edge[u][v] < 0)
            {
                color_edge(u,v,-color_permutation[-m_color_edge[u][v]]);                      
            } else {
                color_edge(u,v,color_permutation[m_color_edge[u][v]]);      
            }
#else
            color_edge(u,v,color_permutation[m_color_edge[u][v]]);      
#endif
        }
    }
#endif 
    
    
#ifdef G_COLORED_3EDGES_BLIND
    // Makes a permutation of 3-edges of this flag
    // Maybe be rewritten for higher effectivity
    void permute_3edge_colors(const vector<int> &color_permutation)
    {
        assert (color_permutation.size() >= COLORS_3EDGES);
        
        for (int u = 0; u < m_vertices-2; u++)
            for (int v = u+1; v < m_vertices-1; v++)
                for (int w = v+1; w < m_vertices; w++)
            {
                color_3edge(u,v,w,color_permutation[m_colored_3edge[u][v][w]]);      
            }
    }
#endif


#ifdef G_ORIENTED_EDGES_BLIND
    void swap_orientation()
    {
        for (int u = 0; u < m_vertices; u++)
        {
            for (int v = 0; v < m_vertices; v++)
            {
                // just reversing orientation
                if (abs(m_color_edge[u][v]) > G_ORIENTED_EDGES_UNORIENTED_COLORS) m_color_edge[u][v] = -m_color_edge[u][v];
            }
        }     
    }
#endif

    
#ifdef G_ROTATION_SYSTEM
    void reverse_rotation_system()
    {
        for (int v = 0; v < m_vertices; v++)
        {
            for (int i = 0; i < m_rotation_system_neighbors[v]/2; i++)
            {
                swap(m_rotation_system[v][i], m_rotation_system[v][m_rotation_system_neighbors[v]-i-1]);
            }
        }
        // Reverse does not change m_rotation_system_noncrosssings
    }
#endif    
    
#ifdef G_LEFTRIGHT_BLIND
    void reverse_leftright()
    {
        for (int u = 0; u < m_vertices; u++)
            for (int v = 0; v < m_vertices; v++)
                for (int x = 0; x < m_vertices; x++)
                    m_leftright[u][v][x] = -m_leftright[u][v][x];
    }
#endif
    
// Sequence of functions for checking if two flags have the same type //
private:
    // TODO: This sequence could be rewritten by using early check and no need
    // for creating copies of F - could be more effective.
    bool have_same_type_colorblind_colored_3edges(const flag &h) const
    {
#ifdef G_COLORED_3EDGES_BLIND
        flag F;
        for (int p = 0; p < (int)g_allowed_3edgecolor_permutations.size(); p++)
        {
            F = h; 
            F.permute_3edge_colors(g_allowed_3edgecolor_permutations[p]);
            if (have_same_type_colorblind_colored_edges(F)) return true;            
        }
        return false;
#else
        return have_same_type_colorblind_colored_edges(h);
#endif
    }
    
    
    bool have_same_type_colorblind_colored_edges(const flag &h) const
    {
#ifdef G_COLORED_EDGES_BLIND
        flag F;
        for (int p = 0; p < (int)g_allowed_edgecolor_permutations.size(); p++)
        {
            F = h; 
            F.permute_edge_colors(g_allowed_edgecolor_permutations[p]);
            if (have_same_type_colorblind_oriented_edges(F)) return true;            
        }
        return false;
#else
        return have_same_type_colorblind_oriented_edges(h);
#endif
    }
    
    bool have_same_type_colorblind_oriented_edges(const flag &h) const
    {
#ifdef G_ORIENTED_EDGES_BLIND
        if (have_same_type_colorblind_vertices(h)) return true;
        flag swapped;
        swapped = h;
        swapped.swap_orientation();
        return have_same_type_colorblind_vertices(swapped);
#else
        return have_same_type_colorblind_vertices(h);
#endif
    }
    
    bool have_same_type_colorblind_vertices(const flag &h) const
    {
#ifdef G_COLORED_VERTICES_BLIND
        /* // Old universal version
        int color_permutation[] = {0,1,2,3,4,5,7,8,9,10}; // longer than needed
        
        flag F;
        do {
            F = h; // copy flag
            F.permute_vertex_colors(color_permutation);
            if (have_same_type_not_colorblind(F)) return true;
        } while ( std::next_permutation(color_permutation+1,color_permutation+COLORS_VERTICES));
        return false;
          */
        flag F;
        for (int p = 0; p < (int)g_allowed_vertexcolor_permutations.size(); p++)
        {
            F = h; 
            F.permute_vertex_colors(g_allowed_vertexcolor_permutations[p]);
            if (have_same_type_leftright_blind(F)) return true;            
        }
        return false;        
#else
        return have_same_type_leftright_blind(h);
#endif
    }


    bool have_same_type_leftright_blind(const flag&h) const
    {
#ifdef G_LEFTRIGHT_BLIND
        if (have_same_type_rotation_reverse_blind(h)) return true;
        flag F;
        F = h;
        F.reverse_leftright();
        return have_same_type_rotation_reverse_blind(F);
#else  
        return have_same_type_rotation_reverse_blind(h); 
#endif 
    }

    bool have_same_type_rotation_reverse_blind(const flag &h) const
    {
#ifdef G_ROTATION_SYSTEM_REVERSE_BLIND
        if (have_same_type_not_colorblind(h)) return true;

        flag F=h;
        F.reverse_rotation_system();
        return have_same_type_not_colorblind(F);        
#endif
        return have_same_type_not_colorblind(h);
    }

    // See if the lists are identical up to a rotation
    bool same_lists_up_to_rotation(const int *L1, int L1sz, const int *L2, int L2sz) const
    {
        
        if (L1sz != L2sz)
        {
            return false;
        }

        if (L1sz == 0)
        {
            return true;
        }
        
        // Finds the smallest element in both lists
        int min1 = 0;
        int min2 = 0;
        for (int i = 1; i < L1sz; i++)
        {
            if (L1[i] < L1[min1]) min1 = i;
            if (L2[i] < L2[min2]) min2 = i;
        }
        
        // And now go from the min element and check all entries
        for (int i = 0; i < L1sz; i++)
        {
            if (L1[(i+min1)%L1sz] != L2[(i+min2)%L1sz]) return false;
        }
        
        return true;
    }

    bool have_same_type_not_colorblind(const flag &f) const
    {
        //if (m_Theta != f.m_Theta) return false;
        //if (m_Theta == 0) return true;
        
#ifdef G_COLORED_VERTICES
        // same vertex color
        for (int u = 0; u < m_Theta; u++)
        {
#ifdef G_USING_ZERO_AS_ANY_COLOR
            if (m_color_vertex[u] == 0 || f.m_color_vertex[u] == 0) continue;
#endif            
            if (m_color_vertex[u] != f.m_color_vertex[u]) return false;
        }
#endif
        
        
#ifdef G_COLORED_EDGES
        assert(m_Theta < V);
        for (int u = 0; u < m_Theta; u ++)
            for (int v = u+1; v < m_Theta; v++)
            {
#ifdef G_USING_ZERO_AS_ANY_COLOR
                if (m_color_edge[u][v] == 0 || f.m_color_edge[u][v] == 0) continue;
#endif            
                if (m_color_edge[u][v] != f.m_color_edge[u][v]) return false;
            }
#endif
        
#ifdef G_3EDGES
        for (int x = 0; x < m_Theta-2; x++)
            for (int y = x+1; y < m_Theta-1; y++)
                for (int z = y+1; z < m_Theta; z++)
                {
                    if (m_3edge[x][y][z] != f.m_3edge[x][y][z]) return false;
                }
#endif          

#ifdef G_ROOTED_3EDGES
        for (int x = 0; x < m_Theta-2; x++)
            for (int y = x+1; y < m_Theta-1; y++)
                for (int z = y+1; z < m_Theta; z++)
                {
                    if (m_rooted_3edge[x][y][z] != f.m_rooted_3edge[x][y][z]) return false;
                    if (m_rooted_3edge[y][x][z] != f.m_rooted_3edge[y][x][z]) return false;
                }
#endif     

#ifdef G_COLORED_3EDGES
        for (int x = 0; x < m_Theta-2; x++)
            for (int y = x+1; y < m_Theta-1; y++)
                for (int z = y+1; z < m_Theta; z++)
                {
#ifdef G_USING_ZERO_AS_ANY_COLOR
                    if (m_colored_3edge[x][y][z] == 0 || f.m_colored_3edge[x][y][z] == 0) continue;
#endif              
                    if (m_colored_3edge[x][y][z] != f.m_colored_3edge[x][y][z]) return false;
                }
#endif            
        
#ifdef G_4EDGES
        for (int x = 0; x < m_Theta-3; x++)
            for (int y = x+1; y < m_Theta-2; y++)
                for (int z = y+1; z < m_Theta-1; z++)
                    for (int w = z+1; w < m_Theta; w++)
                    {
                        if (m_4edge[x][y][z][w] != f.m_4edge[x][y][z][w]) return false;
                    }
#endif

#ifdef G_ROOTED_4EDGES
        for (int x = 0; x < m_Theta-3; x++)
            for (int y = x+1; y < m_Theta-2; y++)
                for (int z = y+1; z < m_Theta-1; z++)
                    for (int w = z+1; w < m_Theta; w++)
                    {
                        if (m_rooted_4edge[x][y][z][w] != f.m_rooted_4edge[x][y][z][w]) return false;
                        if (m_rooted_4edge[y][x][z][w] != f.m_rooted_4edge[y][x][z][w]) return false;
                        if (m_rooted_4edge[z][x][y][w] != f.m_rooted_4edge[z][x][y][w]) return false;
                    }
#endif

#ifdef G_MAYBE_ROOTED_KEDGES
    //if (f.m_rooted_kedges == m_rooted_kedges) return true;
    for (auto re : m_rooted_kedges)
    {
        if (re.max_entry() >= m_Theta)
            continue;

        if (f.contains_rooted_kedge(re) == false) return false;
    }

    for (auto re : f.m_rooted_kedges)
    {
        if (re.max_entry() >= m_Theta)
            continue; 

        if (contains_rooted_kedge(re) == false) return false;
    }

#endif
        
#ifdef G_ROTATION_SYSTEM
        for (int x = 0; x < m_Theta; x++)
        {
            int forX[V];
            int forfX[V];
            int forX_cnt = 0;
            int forfX_cnt = 0;
            
            for (int u = 0; u < m_rotation_system_neighbors[x]; u++)
            {
                if (m_rotation_system[x][u] < m_Theta)
                    forX[forX_cnt++] = m_rotation_system[x][u];
            }
            
            for (int u = 0; u < f.m_rotation_system_neighbors[x]; u++)
            {
                if (f.m_rotation_system[x][u] < m_Theta)
                    forfX[forfX_cnt++] = f.m_rotation_system[x][u];
            }
            
            if (forX_cnt != forfX_cnt) return false;
            
            
            if (!same_lists_up_to_rotation(forX, forX_cnt, forfX, forfX_cnt)) return false;
            
        }
 #endif
        
#ifdef G_CROSSINGS
        // Check crossings
        // now crossing of pairs of edges
        for (int u1 = 0; u1 < m_Theta; u1++)
            for (int u2 = u1+1; u2 < m_Theta; u2++)
                for (int v1 = u1+1; v1 < m_Theta; v1++)
                    for (int v2 = v1+1; v2 < m_Theta; v2++)
                        if (m_cross_edges[u1][u2][v1][v2] != f.m_cross_edges[u1][u2][v1][v2]) return false;
#endif
        
#ifdef G_CUTTINGS
        // Check cuttings
        for (int u1 = 0; u1 < m_Theta; u1++)
            for (int u2 = u1+1; u2 < m_Theta; u2++)
                for (int v1 = 0; v1 < m_Theta; v1++)
                    for (int v2 = v1+1; v2 < m_Theta; v2++)
                    {
                        if (m_cuttings[u1][u2][v1][v2] != f.m_cuttings[u1][u2][v1][v2]) return false;
                    }
#endif
        
#ifdef G_LEFTRIGHT
        // Check left-right
        for (int u1 = 0; u1 < m_Theta; u1++)
            for (int u2 = 0; u2 < m_Theta; u2++)
            {
                if (u1 == u2) continue;
#ifdef G_LEFTRIGHT_LOCAL
                bool test_normal = true;
                bool test_flip = true;
                for (int v1 = 0; v1 < m_Theta; v1++)
                {
                    if (m_leftright[u1][u2][v1] != f.m_leftright[u1][u2][v1]) test_normal = false;
                    if (m_leftright[u1][u2][v1] != (-1)*f.m_leftright[u1][u2][v1]) test_flip = false;
                }
                if (test_normal == false && test_flip == false) return false;
#else
                for (int v1 = 0; v1 < m_Theta; v1++)
                {
                    if (m_leftright[u1][u2][v1] != f.m_leftright[u1][u2][v1]) return false;
                }
#endif
            }
#endif
        
#ifdef G_ORDER_TYPES
        // coordinates of order types are not used
        // using G_LEFTRIGHT is sufficient
#endif
        return true;
    }
    

public:    
    // This is the function to call in general program
    bool have_same_type(const flag &f) const
    {
        if (m_Theta_class != 0 && f.m_Theta_class != 0 && m_Theta_class != f.m_Theta_class) return false;
        if (m_Theta == 0 && f.m_Theta == 0) return true;
        if (labeled_vertices_cnt() != f.labeled_vertices_cnt()) return false;
        
#ifdef G_ORDERED_VERTICES
        flag type_this;
        flag type_f;
        get_type_subflag(type_this);
        f.get_type_subflag(type_f);
        return type_this.is_isomorphic_to(type_f);
#else
        return have_same_type_colorblind_colored_3edges(f);
#endif
    }
    

    template <bool verbose_output=false> 
    bool is_isomorphic_to_colorblind_colored_3edges(const flag &h) const
    {
#ifdef G_COLORED_3EDGES_BLIND
        flag F;
        for (int p = 0; p < (int)g_allowed_3edgecolor_permutations.size(); p++)
        {
            F = h; 
            F.permute_3edge_colors(g_allowed_3edgecolor_permutations[p]);
            if (is_isomorphic_to_colorblind_colored_edges<verbose_output>(F)) return true;            
        }
        return false;  
#else
        return is_isomorphic_to_colorblind_colored_edges<verbose_output>(h);
#endif
    }
    
    template <bool verbose_output=false> 
    bool is_isomorphic_to_colorblind_colored_edges(const flag &h) const
    {
#ifdef G_COLORED_EDGES_BLIND
        flag F;
        for (int p = 0; p < (int)g_allowed_edgecolor_permutations.size(); p++)
        {
            // A noticable speed-up for big ones...
            bool good_permuation = true;
            for (int c = 0; c < COLORS_EDGES; c++)
            {
                if (m_colored_edges[g_allowed_edgecolor_permutations[p][c]] != h.m_colored_edges[c])
                {
                    good_permuation = false;
                    break;
                }
            }
            if (!good_permuation) continue;
            F = h; 
            F.permute_edge_colors(g_allowed_edgecolor_permutations[p]);
            if (is_isomorphic_to_colorblind_oriented_edges<verbose_output>(F)) 
            {
                return true;
            }
        }
        assert(g_allowed_edgecolor_permutations.size() != 0);
        return false;  
#else
        return is_isomorphic_to_colorblind_oriented_edges<verbose_output>(h);
#endif
    }
    
    template <bool verbose_output=false> 
    bool is_isomorphic_to_colorblind_oriented_edges(const flag &h) const
    {
#ifdef G_ORIENTED_EDGES_BLIND
        if (is_isomorphic_to_colorblind_vertices<verbose_output>(h)) return true;
        flag swapped;
        swapped = h;
        swapped.swap_orientation();
        return is_isomorphic_to_colorblind_vertices<verbose_output>(swapped);
#else
        return is_isomorphic_to_colorblind_vertices<verbose_output>(h);
#endif
    }

    template <bool verbose_output=false> 
    bool is_isomorphic_to_colorblind_vertices(const flag &h) const
    {
#ifdef G_COLORED_VERTICES_BLIND
        /*
        int color_permutation[] = {0,1,2,3,4,5,7,8,9,10,11}; // longer than needed
        
        flag F;
        do {
            F = h; // copy flag
            F.permute_vertex_colors(color_permutation);
            if (is_isomorphic_to_not_colorblind(F)) return true;
        } while ( std::next_permutation(color_permutation+1,color_permutation+COLORS_VERTICES));
        return false;
        */
        
        flag F;
        for (int p = 0; p < (int)g_allowed_vertexcolor_permutations.size(); p++)
        {
            F = h; 
            F.permute_vertex_colors(g_allowed_vertexcolor_permutations[p]);
            if (is_isomorphic_to_reverseblind_leftright_system<verbose_output>(F)) return true;            
        }
        return false;   
        
#else
        return is_isomorphic_to_reverseblind_leftright_system<verbose_output>(h);
#endif
    }

    template <bool verbose_output=false> 
    bool is_isomorphic_to_reverseblind_leftright_system(const flag &h) const
    {
#ifdef G_LEFTRIGHT_BLIND
        if (is_isomorphic_to_reverseblind_rotation_system(h))
            return true;
        
        flag F=h;
        F.reverse_leftright();
        return is_isomorphic_to_reverseblind_rotation_system(F);
#else
        return is_isomorphic_to_reverseblind_rotation_system(h);
#endif         
    }

    
    bool is_isomorphic_to_reverseblind_rotation_system(const flag &h) const
    {
#ifdef G_ROTATION_SYSTEM_REVERSE_BLIND
        if (is_isomorphic_to_not_colorblind(h))
            return true;
        
        flag F=h;
        F.reverse_rotation_system();
        return is_isomorphic_to_not_colorblind(F);
#else
        return is_isomorphic_to_not_colorblind(h);
#endif         
    }

    template <bool verbose_output=false>    
    bool is_isomorphic_to(const flag &h) const
    {
#ifdef G_USE_LEXMIN_FOR_ISOMORPHISM
        // if we have signatires, we are can just use them and that is it
        if (!m_minlex_signature.empty() && !h.m_minlex_signature.empty())
        {
            if (m_minlex_signature.compare(h.m_minlex_signature) != 0) return false;
            return true;            
        }
#endif
        // Just a quick kill
        if (m_vertices != h.m_vertices) 
        {
            if (verbose_output) cerr << "The number of vertices is different" << endl;
            return false;
        }
        if (labeled_vertices_cnt() != h.labeled_vertices_cnt())
        {
            if (verbose_output) cerr << "The number of labeled vertices is different" << endl;
            return false;
        }
        
        if (m_Theta_class != 0 && h.m_Theta_class != 0  && m_Theta_class != h.m_Theta_class)
        {
            if (verbose_output) cerr << "The m_Theta_class is different" << endl;
            return false;
        } 

#ifdef G_USE_INDEXES_FOR_LARGE_UNLABELED_FLAGS
        // If both flag are unlabeled and they do have unlabeled id, then
        // they are isomorphic iff they have the same id. 
        if (m_Theta == 0 && m_unlabeled_id != -1 && h.m_unlabeled_id != -1)
        {
            return (m_unlabeled_id == h.m_unlabeled_id);
        }
#endif
       
        
#ifdef G_ROTATION_SYSTEM       
        if (m_rotation_system_noncrosssings != h.m_rotation_system_noncrosssings) 
        {
            if (verbose_output) cerr << "m_rotation_system_noncrosssings are different" << endl;
            return false;
        }
        for (int i = 0; i < m_vertices; i++)
        {
            if (m_rotation_system_noncrosssings_v_sorted[i] != h.m_rotation_system_noncrosssings_v_sorted[i]) return false;
        }
#endif
        
        //cerr << "Testing " << h.print() << " and " << print() << endl;
        
        return is_isomorphic_to_colorblind_colored_3edges<verbose_output>(h);
    }
	
	
	// perm is mapping vertices of h to vertices of this. pv is a candidate for
	// mapping of v, all vertices before v are already mapped.
	bool is_map_up_to_v_correct(int v, int pv, const int *perm, const flag &h) const
	{
			
#ifdef G_COLORED_VERTICES
        // 0 as a joker color
        if ((m_color_vertex[pv] != h.m_color_vertex[v]) && (m_color_vertex[pv] != 0) && (0 != h.m_color_vertex[v])) return false;
#endif
						
#ifdef G_COLORED_EDGES
      // check edges, 0 as joker
      for (int u = 0; u < v; u++)
         if (m_color_edge[perm[u]][pv] != h.m_color_edge[u][v] && m_color_edge[perm[u]][pv] != 0 && h.m_color_edge[u][v] != 0) return false;        
#endif

#ifdef G_3EDGES
      for (int x = 0; x < v; x++)
         for (int y = x+1; y < v; y++)
         {
             if (m_3edge[perm[x]][perm[y]][pv] != h.m_3edge[x][y][v]) return false;
         }
#endif   

#ifdef G_ROOTED_3EDGES
      for (int x = 0; x < v; x++)
         for (int y = x+1; y < v; y++)
         {
             // We need to test at least two orderings to make sure they are all correct
             if (m_rooted_3edge[perm[x]][perm[y]][pv] != h.m_rooted_3edge[x][y][v]) return false;
             if (m_rooted_3edge[perm[y]][perm[x]][pv] != h.m_rooted_3edge[y][x][v]) return false;
             //if (m_rooted_3edge[perm[x]][perm[y]][pv] != h.m_rooted_3edge[x][y][v]) return false;
         }
#endif   

#ifdef G_COLORED_3EDGES
        for (int x = 0; x < v; x++)
            for (int y = x+1; y < v; y++)
            {
                 if (m_colored_3edge[perm[x]][perm[y]][pv] != h.m_colored_3edge[x][y][v])
                 {
                     return false;
                 }
            }
#endif          
        
#ifdef G_4EDGES
        for (int x = 0; x < v; x++)
            for (int y = x+1; y < v; y++)
                for (int z = y+1; z < v; z++)
                {
                    if (m_4edge[perm[x]][perm[y]][perm[z]][pv] != h.m_4edge[x][y][z][v]) return false;
                }
#endif

#ifdef G_ROOTED_4EDGES
        for (int x = 0; x < v; x++)
            for (int y = x+1; y < v; y++)
                for (int z = y+1; z < v; z++)
                {
                    if (m_rooted_4edge[perm[x]][perm[y]][perm[z]][pv] != h.m_rooted_4edge[x][y][z][v]) return false;
                    if (m_rooted_4edge[perm[y]][perm[x]][perm[z]][pv] != h.m_rooted_4edge[y][x][z][v]) return false;
                    if (m_rooted_4edge[perm[z]][perm[y]][perm[x]][pv] != h.m_rooted_4edge[z][y][x][v]) return false;
                }
#endif

#ifdef G_MAYBE_ROOTED_KEDGES
        //cerr << "Testing..." << v+1 << endl;
        for(auto &re : h.m_rooted_kedges)
        {
            // Check only edges where v is the largest
            //cerr << "Edge " << re << endl;
            if (re.max_entry_is(v))
            {
                //cerr << "Found edge" << endl;
                rooted_kedge perm_re;
                if (re.root == -1) perm_re.root = -1;
                else if (re.root == v)  perm_re.root = pv;
                else perm_re.root = perm[re.root];

                for(int i : re.vertices)
                {
                    if (i == v) 
                        perm_re.vertices.insert(pv);
                    else
                        perm_re.vertices.insert(perm[i]);
                }

                //cerr << "Testing " << re << " as " << perm_re << endl;

                // Now we have a new edge. See it already exists in the other one.
                if (contains_rooted_kedge(perm_re) == false)
                    return false;
            }
        }
#endif

#ifdef G_ROTATION_SYSTEM
        /*
        // Test type at v
        if (m_rotation_system_neighbors[perm[v]] != h.m_rotation_system_neighbors[v]) return false;
        
        // test types at v
        // Notice that this test is not perfect as it may ignore some mistakes in previous
        // lists. But the test is expensive to run for all I think...
        for (int x = 0; x <= v; x++)
        {
            int permuted[V];
            int original[V];

            for (int i = 0; i < m_rotation_system_neighbors[perm[x]]; i++)
            {
                //cerr << "i="<< i << " v=" << v << " perm[x]="<<perm[x] << endl;
                if (m_rotation_system[perm[x]][i] <= v)
                    permuted[i] = perm[m_rotation_system[perm[x]][i]];
                else
                    permuted[i] = V+1;
                
                if (m_rotation_system[perm[x]][i] <= v)
                    original[i] = h.m_rotation_system[x][i];
                else
                    permuted[i] = V+1;
            }
            
            if (!same_lists_up_to_rotation(permuted, h.m_rotation_system_neighbors[x], original, h.m_rotation_system_neighbors[x]) ) return false;

        }
         */
#endif
        
#ifdef G_CROSSINGS
		// check crossings
		// now crossing of pairs of edge
		for (int u2 = 0; u2 < v; u2++)
		for (int v1 = 0; v1 < v; v1++)
		{
			if (v1 == u2) continue;
			for (int v2 = v1+1; v2 < v; v2++)
			{
				if (v2 == u2) continue;
				if (m_cross_edges[pv][perm[u2]][perm[v1]][perm[v2]] != h.m_cross_edges[v][u2][v1][v2]) return false;
			}
		}
#endif

#ifdef G_CUTTINGS
		// check crossings
		// now crossing of pairs of edges
		for (int u2 = 0; u2 < v; u2++)
		for (int v1 = 0; v1 < v-1; v1++)
		{
			if (v1 == u2) continue;
			for (int v2 = v1+1; v2 < v; v2++)
				if ((m_cuttings[pv][perm[u2]][perm[v1]][perm[v2]] != h.m_cuttings[v][u2][v1][v2]) ||  (m_cuttings[perm[v1]][perm[v2]][pv][perm[u2]] != h.m_cuttings[v1][v2][v][u2])) return false;
		}
#endif	
        
#ifdef G_LEFTRIGHT
		// check crossings
		// now crossing of pairs of edges
        for (int u2 = 0; u2 < v; u2++)
        {
#ifdef G_LEFTRIGHT_LOCAL            
            bool test_normal = true;
            bool test_flip = true;
            for (int v1 = 0; v1 < v; v1++)
            {
                if (v1 == u2) continue;
                if ((m_leftright[pv][perm[u2]][perm[v1]] != h.m_leftright[v][u2][v1])) test_normal = false;
                if ((m_leftright[pv][perm[u2]][perm[v1]] != (-1)*h.m_leftright[v][u2][v1])) test_flip = false;
            }
            if (test_normal == false && test_flip == false) return false;
#else
            for (int v1 = 0; v1 < v; v1++)
            {
                if (v1 == u2) continue;
                if ((m_leftright[pv][perm[u2]][perm[v1]] != h.m_leftright[v][u2][v1])) return false;
            }
#endif
        }
#endif
        
        
#ifdef G_ORDER_TYPES
        // coordinates of order types are not used
        // using G_LEFTRIGHT is sufficient
#endif        
		return true;
	}

    // perm is saying how are vertices of h mapped to *this
    // it means color of x in h is the same as color of perm[x] in *this
    //
    //  v in h is mapped to perm[v] in *this
    //
    bool is_mapping_an_equality(int *perm, const flag &h) const
    {
#ifdef G_COLORED_EDGES
        //	check coloring
        for (int u = 0; u < m_vertices; u++)
        {
            for (int x = u+1; x < m_vertices; x++)
            {
                if (m_color_edge[perm[u]][perm[x]] == 0 || h.m_color_edge[u][x]==0) continue;
                if (m_color_edge[perm[u]][perm[x]] != h.m_color_edge[u][x]) return false;
            }
        }
#endif
        
#ifdef G_3EDGES
        for (int x = 0; x < m_vertices; x++)
            for (int y = x+1; y < m_vertices; y++)
                for (int z = y+1; z < m_vertices; z++)
                {
                    if (m_3edge[perm[x]][perm[y]][perm[z]] != h.m_3edge[x][y][z]) return false;
                }
#endif   

        
#ifdef G_ROOTED_3EDGES
        for (int x = 0; x < m_vertices; x++)
            for (int y = x+1; y < m_vertices; y++)
                for (int z = y+1; z < m_vertices; z++)
                {
                    // At least 2 must match 
                    if (m_rooted_3edge[perm[x]][perm[y]][perm[z]] != h.m_rooted_3edge[x][y][z]) return false;
                    if (m_rooted_3edge[perm[y]][perm[x]][perm[z]] != h.m_rooted_3edge[y][x][z]) return false;
                }
#endif   

#ifdef G_COLORED_3EDGES
        for (int x = 0; x < m_vertices; x++)
            for (int y = x+1; y < m_vertices; y++)
                for (int z = y+1; z < m_vertices; z++)
                {
                    if (m_colored_3edge[perm[x]][perm[y]][perm[z]] != h.m_colored_3edge[x][y][z]) return false;
                }
#endif   
        
        
#ifdef G_4EDGES
        for (int x = 0; x < m_vertices; x++)
            for (int y = x+1; y < m_vertices; y++)
                for (int z = y+1; z < m_vertices; z++)
                    for (int w = z+1; w < m_vertices; w++)
                    {
                        if(m_4edge[perm[x]][perm[y]][perm[z]][perm[w]] != h.m_4edge[x][y][z][w]) return false;
                    }
#endif  

#ifdef G_ROOTED_4EDGES
        for (int x = 0; x < m_vertices; x++)
            for (int y = x+1; y < m_vertices; y++)
                for (int z = y+1; z < m_vertices; z++)
                    for (int w = z+1; w < m_vertices; w++)
                    {
                        if(m_rooted_4edge[perm[x]][perm[y]][perm[z]][perm[w]] != h.m_rooted_4edge[x][y][z][w]) return false;
                        if(m_rooted_4edge[perm[y]][perm[x]][perm[z]][perm[w]] != h.m_rooted_4edge[y][x][z][w]) return false;
                        if(m_rooted_4edge[perm[z]][perm[y]][perm[x]][perm[w]] != h.m_rooted_4edge[z][y][x][w]) return false;
                    }
#endif  

        
#ifdef G_ROTATION_SYSTEM
        // perm is going from h to this
        // quick check
        for (int x = 0; x < m_vertices; x++)
        {
            if (m_rotation_system_noncrosssings_v[perm[x]] != h.m_rotation_system_noncrosssings_v[x]) return false;
        }
        
        for (int x = 0; x < m_vertices; x++)
        {
            if ( m_rotation_system_neighbors[perm[x]] !=  h.m_rotation_system_neighbors[x] ) return false;

            
            int permuted[V];
            for (int i = 0; i < h.m_rotation_system_neighbors[x]; i++)
            {
                permuted[i] = perm[h.m_rotation_system[x][i]];
            }
            
            if (!same_lists_up_to_rotation(permuted, h.m_rotation_system_neighbors[x], m_rotation_system[perm[x]], h.m_rotation_system_neighbors[x]) )
            {
                /*
                cerr << "x= " << x << " perm[x]=" << perm[x] << endl;
                cerr << endl << "Original at " << x << endl;
                for (int i = 0; i < 6; i++) cerr << h.m_rotation_system[x][i]+1 << " ";
                cerr << endl << "Permuted" << endl;
                for (int i = 0; i < 6; i++) cerr << permuted[i]+1 << " ";
                cerr << endl << "Target at " << perm[x] << endl;
                for (int i = 0; i < 6; i++) cerr << m_rotation_system[perm[x]][i]+1 << " ";
                
                cerr << "Failed at x=" << x << endl;
                */
                return false;
            }
            
        }
#endif
        
#ifdef G_COLORED_VERTICES
        for (int u = 0; u < m_vertices; u++)
        {
            if (m_color_vertex[perm[u]] == 0 ||  h.m_color_vertex[u] == 0) continue;
            if (m_color_vertex[perm[u]] != h.m_color_vertex[u]) return false;
        }
#endif
        
#ifdef G_CROSSINGS
        // Check crossings
        // now crossing of pairs of edges
        for (int u1 = 0; u1 < m_vertices; u1++)
            for (int u2 = u1+1; u2 < m_vertices; u2++)
                for (int v1 = u1+1; v1 < m_vertices; v1++)
                    for (int v2 = v1+1; v2 < m_vertices; v2++)
                    {
                        if (m_cross_edges[perm[u1]][perm[u2]][perm[v1]][perm[v2]] != h.m_cross_edges[u1][u2][v1][v2]) return false;
                    }
#endif
        
#ifdef G_CUTTINGS
        // Check crossings
        // now crossing of pairs of edges
        for (int u1 = 0; u1 < m_vertices; u1++)
            for (int u2 = u1+1; u2 < m_vertices; u2++)
                for (int v1 = 0; v1 < m_vertices; v1++)
                    for (int v2 = v1+1; v2 < m_vertices; v2++)
                    {
                        if (m_cuttings[perm[u1]][perm[u2]][perm[v1]][perm[v2]] != h.m_cuttings[u1][u2][v1][v2]) return false;
                    }
#endif
        
#ifdef G_LEFTRIGHT
        // Check crossings
        // now crossing of pairs of edges
        for (int u1 = 0; u1 < m_vertices; u1++)
            for (int u2 = u1+1; u2 < m_vertices; u2++)
            {
#ifdef G_LEFTRIGHT_LOCAL                
                bool test_normal = true;
                bool test_flip = true;
                for (int v1 = 0; v1 < m_vertices; v1++)
                {
                    if (u1 == v1 || u2 == v1) continue;
                    
                    if (m_leftright[perm[u1]][perm[u2]][perm[v1]] != h.m_leftright[u1][u2][v1]) test_normal=false;
                    if (m_leftright[perm[u1]][perm[u2]][perm[v1]] != -1*h.m_leftright[u1][u2][v1]) test_flip=false;
                }
                if (!test_normal && !test_flip) return false;
                //if (!test_normal) return false;
#else
                for (int v1 = 0; v1 < m_vertices; v1++)
                {
                    if (u1 == v1 || u2 == v1) continue;
                    
                    if (m_leftright[perm[u1]][perm[u2]][perm[v1]] != h.m_leftright[u1][u2][v1]) return false;
                }
#endif                
            }
#endif

#ifdef G_ORDER_TYPES
        // coordinates of order types are not used
        // using G_LEFTRIGHT is sufficient
#endif        
        return true;
    }
    
    // Real isomorphism testing
	bool make_perm_isomorphic(int v, int *perm, bool *used, const flag &h) const
	{
		// Test if the permutation is isomorphism
		if (v >= m_vertices)
		{
#ifndef G_BE_BRAVE_AND_IGNORE_SAFETY_ASSERTS
            assert(is_mapping_an_equality(perm,h) == true);
#endif
     
/*            
#ifdef G_LEFTRIGHT
			// Check crossings
			// now crossing of pairs of edges
			for (int u1 = 0; u1 < m_vertices; u1++)
                for (int u2 = u1+1; u2 < m_vertices; u2++)
                {
                    bool test_normal = true;
                    bool test_flip = true;
                    for (int v1 = 0; v1 < m_vertices; v1++)
                    {
                        if (u1 == v1 || u2 == v1) continue;
                        
                        if (m_leftright[perm[u1]][perm[u2]][perm[v1]] != h.m_leftright[u1][u2][v1]) test_normal=false;
                        if (m_leftright[perm[u1]][perm[u2]][perm[v1]] != -1*h.m_leftright[u1][u2][v1]) test_flip=false;
                    }
                    if (test_normal == false && test_flip == false)
                    {
                        //cerr << "Help needed" << endl;
                        return false;
                    }
                }
#endif
 */
#ifdef G_LEFTRIGHT
            return is_mapping_an_equality(perm,h);
#endif
            
            
#ifdef G_ROTATION_SYSTEM
            return is_mapping_an_equality(perm,h);
#endif
			return true;
		}

		for (int pv = m_Theta; pv < m_vertices; pv++)
		{
			// check if used
			if (used[pv]) continue;

            perm[v] = pv;
			if (!is_map_up_to_v_correct(v, pv, perm, h)) continue;

			used[pv]  = true;
			
			if (make_perm_isomorphic(v+1, perm, used, h)) return true;
			
			used[pv]  = false;
		}
		
		return false;
	}
	
	
    template <bool verbose_output=false> 
	bool is_isomorphic_to_not_colorblind(const flag &h) const
	{
        //if (labeled_vertices_cnt() > 0)
        //    cerr << "Starting testing not colorblind " << print() <<" and "<< h.print() << endl;

        // cerr << endl << "Testing " << endl << h.print() << endl << print() << endl;

        
        // quick check
        if (m_vertices != h.m_vertices) 
        {
            //if (labeled_vertices_cnt() > 0)
            //cerr << "Wrong number of vertices " << endl;
            return false;
        }
        if (labeled_vertices_cnt() != h.labeled_vertices_cnt())
        {
            //if (labeled_vertices_cnt() > 0)
            //cerr << "Wrong number of labeled vertices " << labeled_vertices_cnt() << " " << h.labeled_vertices_cnt() << endl;
            return false;
        }
        
#ifdef G_CROSSINGS
        if (m_crossings_cnt != h.m_crossings_cnt) return false;
#endif
		
		
#ifdef G_USE_LEXMIN_FOR_ISOMORPHISM 		
		assert (!m_minlex_signature.empty() && !h.m_minlex_signature.empty());
		if (m_minlex_signature.compare(h.m_minlex_signature) != 0) return false;
		return true;
#endif		
		
#ifdef G_CROSSINGS
		if (m_crossings_cnt != -1 && h.m_crossings_cnt != -1 && m_crossings_cnt != h.m_crossings_cnt) return false;
#endif

#ifdef G_COLORED_VERTICES
        for (int c = 1; c < COLORS_VERTICES; c++)
        {
            //cerr << "c=" << c << "   "  << m_colored_vertices[c] << " "  << m_colored_vertices[0] << " " << h.m_colored_vertices[c] << " "  << h.m_colored_vertices[0] << endl;
            
	        if (m_colored_vertices[c] > h.m_colored_vertices[c]+h.m_colored_vertices[0]) return false;
            if (m_colored_vertices[c]+m_colored_vertices[0] < h.m_colored_vertices[c]) return false;
	    }
#endif
		
#ifdef G_COLORED_EDGES
        for (int c = 0; c < COLORS_EDGES; c++)
        {
            //if (m_vertices==0)
            //    cerr << c << "  " << m_colored_edges[c] << " " << h.m_colored_edges[c] << endl;
	        if (m_colored_edges[c] >  h.m_colored_edges[c]+h.m_colored_edges[0]) return false;
            if (m_colored_edges[c]+h.m_colored_edges[0] <  h.m_colored_edges[c]) return false;
	    }
#endif

#ifdef G_3EDGES
        if (m_3edges_cnt != h.m_3edges_cnt) return false;
#endif   

#ifdef G_ROOTED_3EDGES
        if (m_rooted_3edges_cnt != h.m_rooted_3edges_cnt) return false;
#endif   


#ifdef G_COLORED_3EDGES
        for (int c = 1; c < COLORS_3EDGES; c++)
        {
            if (m_colored_3edges[c] !=  h.m_colored_3edges[c]) return false;
        }
#endif        

#ifdef G_ORDER_TYPES
        // no need for this
#endif        
        
#ifdef G_4EDGES
        if (m_4edges_cnt != h.m_4edges_cnt) return false;
#endif   
        
#ifdef G_ROOTED_4EDGES
        if (m_rooted_4edges_cnt != h.m_rooted_4edges_cnt) return false;
#endif           

#ifdef G_MAYBE_ROOTED_KEDGES
        if (m_rooted_kedges.size() != h.m_rooted_kedges.size()) return false;
#endif

//        if (m_vertices == 0)
//            cerr << "Testing not colorblind " << print() <<" and "<< h.print() << endl;

        //cerr << "Testing not colorblind " << print() <<" and "<< h.print() << endl;


#ifdef G_ORDERED_VERTICES
        // if vertices are ordered, there is no chance to do anything
        if (m_Theta != h.m_Theta)
            return false;
        
        int perm[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
        assert(m_vertices < 20);
        return is_mapping_an_equality(perm, h);
#else
		
#ifdef G_ROTATION_SYSTEM
        //TODO: Maybe this works?
        return use_rotation_system_for_isomorphis(h);
#endif

        // Check for same type
        if (!have_same_type_not_colorblind(h)) return false;

		// try check all permutations --- WASTING HERE!!!!!!!!
		int perm[V];
		bool used[V];

        assert(0 <= m_Theta);
        assert(m_Theta <= V);

		// Theta is preserved by the mapping
        for (int i = 0; i < m_Theta; i++)
        {
            perm[i] = i;
            used[i] = true;
        }
        
        for (int i = m_Theta; i < V; i++)
        {
            perm[i] = 0;
            used[i] = false;
        }
		
/*
#ifdef G_ROTATION_SYSTEM
        bool new_test = use_rotation_system_for_isomorphis(h);
        bool old_test = make_perm_isomorphic(m_Theta, perm, used, h);

        if (new_test != old_test)
        {
            cerr << "h=" << h.print() << endl;
            cerr << "g=" << print() << endl;  
            cerr << "new=" << new_test << endl;
            cerr << "old=" << old_test << endl;
        }
        
        //assert(new_test == old_test);

        return new_test;
#endif
*/
        bool rv = make_perm_isomorphic(m_Theta, perm, used, h);
        if (verbose_output==true && rv==true)
        {
            cerr << "Isomorphic with permutation ";
            for (int i = 0; i < m_vertices; i++)
            { 
                cerr << i << "->" << perm[i] << " ";
            }
            cerr << endl;
            cerr << "This maps the second graph to the first one" << endl;
        }
        return rv;        
#endif
	}

#ifdef G_ROTATION_SYSTEM
    bool use_rotation_system_for_isomorphis(const flag &h) const
    {
        if (m_vertices == 0)
        {
            return true;
        }
        
        // Check for the same type
        if (m_Theta > 0 && !have_same_type_not_colorblind(h)) return false;
        if (m_Theta_class != 0 && h.m_Theta_class != 0 && m_Theta_class != h.m_Theta_class) return false;
    
        // Works only for full length neighbors!
        //cerr << "h=" << h.print() << endl;
        //cerr << "g=" << print() << endl;
        //cerr << m_rotation_system_neighbors[0] << " " << m_vertices-1 << endl;
        assert(m_vertices == 0 || (m_rotation_system_neighbors[0] == m_vertices-1));
        
        //cerr << endl << "Testing " << endl << h.print() << endl << print() << endl;
        
        
        // Theta should be mapped to Theta as identity....
        
        int p0_lb = m_Theta, p0_ub = m_vertices;
        int p1_lb = m_Theta, p1_ub = m_vertices;
        if (m_Theta >= 1)
        {
            p0_lb = 0;
            p0_ub = 1;
        }
        
        if (m_Theta >= 2)
        {
            p1_lb = 1;
            p1_ub = 2;            
        }
        
        // Where is vertex 0 permuted
        for (int p0 = p0_lb; p0 < p0_ub; p0++)
        {
            
            if (m_rotation_system_noncrosssings_v[p0] != h.m_rotation_system_noncrosssings_v[0]) continue;
            
            assert(m_rotation_system_neighbors[p0] == h.m_rotation_system_neighbors[0]);
            int list_length = m_rotation_system_neighbors[p0];
            
            // Where is vertex 1 permuted
            for (int p1 = p1_lb; p1 < p1_ub; p1++)
            {
                if (p0 == p1) continue;
                if (m_rotation_system_noncrosssings_v[p0] != h.m_rotation_system_noncrosssings_v[0]) continue;
                
                //cerr << "p0=" << p0 << " p1=" << p1 << endl;
                
                // Now we find the shift at vertex 0 for 1 and from here we reconstruct
                // the whole permutation (if it is isomorphism)
                int l1inH = -1;  // location of  1 in neighbors of  0 in H
                int lp1inG = -1; // location of p1 in neighbors of p0 in *this
                
                for (int id = 0; id < h.m_rotation_system_neighbors[0]; id++)
                {
                    if (h.m_rotation_system[0][id] == 1)
                    {
                        l1inH = id;
                        break;
                    }
                }
                assert(l1inH != -1);

                
                for (int id = 0; id < m_rotation_system_neighbors[p0]; id++)
                {
                    if (m_rotation_system[p0][id] == p1)
                    {
                        lp1inG = id;
                        break;
                    }
                }
                assert(lp1inG != -1);

                // Now we can construct the whole permutation:
                int perm[V];
                perm[0] = p0;
                for (int id = 0; id < h.m_rotation_system_neighbors[0]; id++)
                {
                    perm[ h.m_rotation_system[0][  (id + l1inH)%list_length ] ]  = 
                            m_rotation_system[p0][ (id + lp1inG)%list_length ];
                }
                
                //cerr << "p0=" << p0 << " p1=" << p1 << " l1inH=" << l1inH << " lp1inG=" << lp1inG << endl;
                //for (int i = 0; i < m_vertices; i++)
                //    cerr << perm[i] << " ";
                //cerr << endl;
                
                // Test if the permutation preserves Theta...
                if (m_Theta > 0)
                {
                    bool theta_OK = true;
                    for (int t = 0; t < m_Theta; t++)
                    {
                        if (perm[t] != t)
                        {
                            theta_OK = false;
                            break;
                        }
                    }
                    if (!theta_OK) continue;
                }
                
                assert(perm[1] == p1);
                assert(perm[0] == p0);
                
                if (is_mapping_an_equality(perm, h))
                {
                    //cerr << endl;
                    //cerr << "h=" << h.print() << endl;
                    //cerr << "g=" << print() << endl;

                    //cerr << "p0=" << p0 << " p1=" << p1 << " l1inH=" << l1inH << " lp1inG=" << lp1inG << endl;
                    //for (int i = 0; i < m_vertices; i++)
                    //    cerr << perm[i] << " ";
                    //cerr << endl;
                    
                    return true;
                }
            }
        }
        
        return false;

        //cerr << "Safety test" << endl;

        /*
        //{1: 5, 2: 1, 3: 2, 4: 3, 5: 4, 6: 7, 7: 6}
        // 0: 4  1: 0  2: 1  3: 2, 4: 3, 5: 6, 6: 5
        int perm[] = {4,0,1,2,3,6,5};
        //int perm[] = {1,2,3,4,0,6,5};
        
        cerr << "test ";
        cerr << is_mapping_an_equality(perm, h) << " ";
        //cerr << "test ";
        //cerr << h.is_mapping_an_equality(perm, *this) << endl;
        
        return false;
        */
        
        /*
        //return false;
        // Test all permutations
        cerr << "Testing all" << endl;
        int perm[] = {0,1,2,3,4,5,6,7,8,9,10};
        
        std::sort (perm,perm+m_vertices);
        
        do {
            if (is_mapping_an_equality(perm, h))
            {
                cerr << "Found one that fits!" << endl;
                return true;
            }
        } while ( std::next_permutation(perm,perm+m_vertices) );
        
        return false;
        */
        
    }
#endif

#if defined(G_3EDGES) ||  defined(G_4EDGES) || defined(G_ROOTED_3EDGES) || defined(G_ROOTED_4EDGES) || defined(G_MAYBE_ROOTED_KEDGES) // || (defined(G_COLORED_EDGES) && (G_COLORED_EDGES == 2))
    // perm is mapping vertices of h to vertices of this. pv is a candidate for
    // mapping of v, all vertices before v are already mapped.
    bool is_map_up_to_v_correct_notinduced(int v, int pv, const int *perm, const flag &h) const
    {
        //cerr << "Testing " << v << " " << pv << endl;
                
#ifdef G_3EDGES
        for (int x = 0; x < v-1; x++)
            for (int y = x+1; y < v; y++)
            {
// Yufey hack
#if defined(G_3EDGES) && defined(G_ROOTED_3EDGES)                    
                    if (m_rooted_3edge[perm[x]][perm[y]][pv] != 0 && h.m_3edge[x][y][v] != 0 &&  h.m_rooted_3edge[x][y][v]==0)
                    {
                        continue;
                    }
#endif

                if (m_3edge[perm[x]][perm[y]][pv] == 0 && h.m_3edge[x][y][v] == 1)
                {
                    return false;
                }
            }
#endif   

#ifdef G_ROOTED_3EDGES
        for (int x = 0; x < v-1; x++)
            for (int y = x+1; y < v; y++)
            {
                if ((m_rooted_3edge[perm[x]][perm[y]][pv] == 0 && h.m_rooted_3edge[x][y][v] != 0) || 
                    (m_rooted_3edge[perm[x]][perm[y]][pv] ==  1 && h.m_rooted_3edge[x][y][v] != 1) || 
                    (m_rooted_3edge[perm[x]][perm[y]][pv] == -1 && h.m_rooted_3edge[x][y][v] != -1) || 
                    (m_rooted_3edge[perm[y]][perm[x]][pv] ==  1 && h.m_rooted_3edge[y][x][v] != 1) || 
                    (m_rooted_3edge[perm[y]][perm[x]][pv] == -1 && h.m_rooted_3edge[y][x][v] != -1) 
                    )
                {
                    return false;
                }
            }
#endif   

#ifdef G_4EDGES
        for (int x = 0; x < v-2; x++)
            for (int y = x+1; y < v-1; y++)
                for (int z = y+1; z < v; z++)
                {
#if defined(G_4EDGES) && defined(G_ROOTED_4EDGES)                    
                    if (m_rooted_4edge[perm[x]][perm[y]][perm[z]][pv] != 0 && h.m_4edge[x][y][z][v] != 0 &&  h.m_rooted_4edge[x][y][z][v]==0)
                    {
                        //cerr << "xx" << endl;
                        continue;
                    }
#endif
                    if (m_4edge[perm[x]][perm[y]][perm[z]][pv] == 0 && h.m_4edge[x][y][z][v] == 1)
                    {
                        return false;
                    }
                 }
#endif 

#ifdef G_ROOTED_4EDGES
        for (int x = 0; x < v-2; x++)
            for (int y = x+1; y < v-1; y++)
                for (int z = y+1; z < v; z++)
                {
                    if ((m_rooted_4edge[perm[x]][perm[y]][perm[z]][pv] == 0 && h.m_rooted_4edge[x][y][z][v] != 0)
                     || (m_rooted_4edge[perm[x]][perm[y]][perm[z]][pv] != 0 && h.m_rooted_4edge[x][y][z][v] != 0 &&
                         ( m_rooted_4edge[perm[x]][perm[y]][perm[z]][pv] != h.m_rooted_4edge[x][y][z][v] ||
                           m_rooted_4edge[perm[y]][perm[x]][perm[z]][pv] != h.m_rooted_4edge[y][x][z][v] || 
                           m_rooted_4edge[perm[z]][perm[y]][perm[x]][pv] != h.m_rooted_4edge[z][y][x][v] ) )
                    )
                    {
                        return false;
                    }
                 }
#endif 

#ifdef G_COLORED_VERTICES
        if ((m_color_vertex[pv] != h.m_color_vertex[v]) && (m_color_vertex[pv] != 0) && (0 != h.m_color_vertex[v])) return false;
#endif     


#ifdef G_MAYBE_ROOTED_KEDGES
        //cerr << "Testing..." << v+1 << endl;
        for(auto &re : h.m_rooted_kedges)
        {
            // Check only edges where v is the largest
            //cerr << "Edge " << re << endl;
            if (re.max_entry_is(v))
            {
                //cerr << "Found edge" << endl;
                rooted_kedge perm_re;
                if (re.root == -1) perm_re.root = -1;
                else perm_re.root = perm[re.root];

                for(int i : re.vertices)
                {
                    perm_re.vertices.insert(perm[i]);
                }

                //cerr << "Testing " << re << " as " << perm_re << endl;

                // Now we have a new edge. See it already exists in the other one.
                if (re.root != -1)
                {
                    if (contains_rooted_kedge(perm_re) == false)
                        return false;
                }
                // allow any orientation to match
                else
                {
                    bool re_found = false;
                    for (auto e : m_rooted_kedges)
                    {
                        if (e.vertices == perm_re.vertices)
                        {
                            re_found = true;
                        }
                    }
                    if (re_found == false)
                        return false;
                }
            }
        } 
#endif
        return true;
    }
    

    // Real isomorphism testing
    bool make_perm_for_noninuced_subflags(int v, int *perm, bool *used, const flag &h) const
    {
        
        // Test if the permutation is isomorphism
        if (v >= h.m_vertices)
        {
#ifdef G_3EDGES
            for (int x = 0; x < m_vertices-2; x++)
                for (int y = x+1; y < m_vertices-1; y++)
                    for (int z = y+1; z < m_vertices; z++)
                    {
                        //assert(!(m_3edge[perm[x]][perm[y]][perm[z]] == 0 && 1 == h.m_3edge[x][y][z]));
                    }
#endif  
#ifdef G_ROOTED_3EDGES
            for (int x = 0; x < m_vertices-2; x++)
                for (int y = x+1; y < m_vertices-1; y++)
                    for (int z = y+1; z < m_vertices; z++)
                    {
                        //assert(!(m_rooted_3edge[perm[x]][perm[y]][perm[z]] == 0 && 0 != h.m_rooted_3edge[x][y][z]));
                    }
#endif   
#ifdef G_4EDGES
            for (int x = 0; x < m_vertices-3; x++)
                for (int y = x+1; y < m_vertices-2; y++)
                    for (int z = y+1; z < m_vertices-1; z++)
                        for (int w = z+1; w < m_vertices; w++)
                    {
                        //assert(!(m_4edge[perm[x]][perm[y]][perm[z]][perm[w]] == 0 && 1 == h.m_4edge[x][y][z][w]));
                    }
#endif   
#ifdef G_ROOTED_4EDGES
            for (int x = 0; x < m_vertices-3; x++)
                for (int y = x+1; y < m_vertices-2; y++)
                    for (int z = y+1; z < m_vertices-1; z++)
                        for (int w = z+1; w < m_vertices; w++)
                    {
                       assert(!(m_rooted_4edge[perm[x]][perm[y]][perm[z]][perm[w]] == 0 && 0 != h.m_rooted_4edge[x][y][z][w]));
                    }
#endif   
            return true;
        }
        
        for (int pv = m_Theta; pv < m_vertices; pv++)
        {

            // check if used
            if (used[pv]) continue;

            //cerr << "Trying v="<< v <<  " to " << pv << endl;
            perm[v] = pv;
            if (!is_map_up_to_v_correct_notinduced(v, pv, perm, h)) continue;
            
            used[pv]  = true;
            
            if (make_perm_for_noninuced_subflags(v+1, perm, used, h)) return true;
            
            used[pv]  = false;
        }
        
        return false;
    }    
        
    bool has_as_notinduced_subflag(const flag &h) const
    {
        //cerr << "Frist check " << h.print() << " in " << print() << endl;



// YUFEI
#if defined(G_3EDGES) && defined(G_ROOTED_3EDGES)
        if (m_3edges_cnt+m_rooted_3edges_cnt < h.m_3edges_cnt) return false;
        if (m_rooted_3edges_cnt < h.m_rooted_3edges_cnt) return false;
#else

#ifdef G_3EDGES
        if (m_3edges_cnt < h.m_3edges_cnt) return false;
#endif   
#ifdef  G_ROOTED_3EDGES
        if (m_rooted_3edges_cnt < h.m_rooted_3edges_cnt) return false;
#endif   
#endif

// YUFEI
#if defined(G_4EDGES) && defined(G_ROOTED_4EDGES)
        if (m_4edges_cnt+m_rooted_4edges_cnt < h.m_4edges_cnt) return false;
        if (m_rooted_4edges_cnt < h.m_rooted_4edges_cnt) return false;
#else
#ifdef G_4EDGES
        if (m_4edges_cnt < h.m_4edges_cnt) return false;
#endif   
#ifdef G_ROOTED_4EDGES
        if (m_rooted_4edges_cnt < h.m_rooted_4edges_cnt) return false;
#endif           
#endif

#ifdef G_MAYBE_ROOTED_KEDGES
        if (m_rooted_kedges.size() < h.m_rooted_kedges.size()) return false;
#endif
        // Check for same type
        if (!have_same_type(h)) return false;
        
        //cerr << "Testing " << h.print() << " in " << print() << endl;

        // try check all permutations --- WASTING HERE!!!!!!!!
        int perm[V];
        bool used[V];
        
        // Theta is preserved by the mapping
        for (int i = 0; i < m_Theta; i++)
        {
            perm[i] = i;
            used[i] = true;
        }
        
        for (int i = m_Theta; i < V; i++)
        {
            perm[i] = 0;
            used[i] = false;
        }
        
        return make_perm_for_noninuced_subflags(m_Theta, perm, used, h);        
    }
#else
    bool has_as_notinduced_subflag(const flag &h) const
    {
        // Not implemented for other
        assert(0);
        return false;
    }
#endif

    bool is_identical_to(const flag &h, bool write_why_not=true) const
    {
        if (m_vertices != h.m_vertices)
        {
            if (write_why_not)
                cerr << "Number of vertices is different: " << m_vertices << " vs " << h.m_vertices << endl;
            return false;
        }

        if (m_Theta != h.m_Theta)
        {
            if (write_why_not)
                cerr << "Thetas are different: " << m_Theta << " vs " << h.m_Theta  << endl;
            return false;
        }
        
        int perm[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
        assert(m_vertices < 20);
        return is_mapping_an_equality(perm, h);
    }
    
    
    
    void create_minlex_signature_blind_3edges(const flag &h, string &lex_min) const
    {
#ifdef G_COLORED_3EDGES_BLIND
        flag F;
        for (int p = 0; p < (int)g_allowed_3edgecolor_permutations.size(); p++)
        {
            F = h; 
            F.permute_3edge_colors(g_allowed_3edgecolor_permutations[p]);
            string Fprint = F.print("");
            if (Fprint.compare(lex_min) < 0)
            {
                lex_min = Fprint;
            }
        }
#else
        string Hprint = h.print("");
        if (Hprint.compare(lex_min) < 0)
        {
            lex_min = Hprint;
        }
#endif
    }
    
    void create_minlex_signature_blind_edges(const flag &h, string &lex_min) const
    {
#ifdef G_COLORED_EDGES_BLIND
        flag F;
        for (int p = 0; p < (int)g_allowed_edgecolor_permutations.size(); p++)
        {
            F = h; 
            F.permute_edge_colors(g_allowed_edgecolor_permutations[p]);
            create_minlex_signature_blind_3edges(F, lex_min);            
        }
#else
        create_minlex_signature_blind_3edges(h, lex_min);
#endif
    }
    
    void create_minlex_signature_oriented_edges(const flag &h, string &lex_min) const
    {
#ifdef G_ORIENTED_EDGES_BLIND
        create_minlex_signature_blind_edges(h,lex_min);
        flag swapped;
        swapped = h;
        swapped.swap_orientation();
        create_minlex_signature_blind_edges(swapped,lex_min);
#else
        create_minlex_signature_blind_edges(h,lex_min);
#endif
    }
    
    
    void create_minlex_signature_blind_vertices(const flag &h, string &lex_min) const
    {
#ifdef G_COLORED_VERTICES_BLIND
        /*
        int color_permutation[] = {0,1,2,3,4,5,7,8,9,10}; // longer than needed
        
        flag F;
        do {
            F = h; // copy flag
            F.permute_vertex_colors(color_permutation);
            create_minlex_signature_oriented_edges(F, lex_min);
        } while ( std::next_permutation(color_permutation+1,color_permutation+COLORS_VERTICES));
        return;
        */
        flag F;
        for (int p = 0; p < (int)g_allowed_vertexcolor_permutations.size(); p++)
        {
            F = h; 
            F.permute_vertex_colors(g_allowed_vertexcolor_permutations[p]);
            create_minlex_signature_oriented_edges(F, lex_min);
        }
#else
        create_minlex_signature_oriented_edges(h, lex_min);
#endif
    }

        
    void create_minlex_signature()
    {
#ifdef G_USE_LEXMIN_FOR_ISOMORPHISM
#ifdef G_ORDERED_VERTICES
        Not implemented
#endif

        
#ifdef G_ROTATION_SYSTEM_REVERSE_BLIND
        Not implemented
#endif        

#ifdef G_LEFTRIGHT_BLIND
        Not implemented
#endif
        
		int perm[V];
		for (int i = 0; i < V; i++) perm[i] = i;
		
		std::sort (perm,perm+V);
					
		//print_for_human();
		string lex_min = print("");
		//cout << "Finding lex min for " << lex_min << endl;
		
		
		flag this_permuted;
		do {
			this_permuted.as_subflag(*this, perm, m_vertices, m_Theta, false);
				
				
            create_minlex_signature_blind_vertices(this_permuted, lex_min);
/*            
            string perm_string = this_permuted.print("");
				if (perm_string.compare(lex_min) < 0)
				{
					lex_min = perm_string;
				}
 */
		} while ( std::next_permutation(perm+m_Theta,perm+m_vertices) );
		
    	m_minlex_signature = lex_min;
#endif    	
    }


    void get_type_subflag(flag &f) const
    {
#ifdef G_ORDERED_VERTICES
        f.as_subflag(*this,m_labeled_vector,m_labeled_vertices_cnt,0);
        f.make_all_vertices_labeled();
#else
        const int mapping[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        f.as_subflag(*this,mapping,m_Theta,m_Theta);        
#endif
        f.m_Theta_class = m_Theta_class;
    }
    
    
    // removes two vertices
    void remove_vertices(int u, int v)
    {
        int mapping[V];
        
        int id = 0;
        for (int i = 0; i < m_vertices;i++)
        {
            if (i == u || i == v) continue;
            mapping[id++] = i;
        }
        
        flag h;
        h.as_subflag(*this, mapping, m_vertices-2, 0);
        
        *this = h;
    }

    void remove_labeled_vertices()
    {
        if (m_Theta == 0)
            return;
        
        int mapping[V];
        
        for (int i = 0; i < m_vertices-m_Theta;i++)
        {
            mapping[i] = i+m_Theta;
        }
        
        flag h;
        h.as_subflag(*this, mapping, m_vertices-m_Theta, 0);
        
        *this = h;
    }


#ifdef G_ROTATION_SYSTEM
    bool is_rotation_even(int a,  int b, int c, int n)
    {
            assert(b < c);
            assert(c < n);
            
            int rotation=0;
            for( int i  = 0; i < m_rotation_system_neighbors[a]; i++)
            {
                if (m_rotation_system[a][i] == b) rotation = 10*rotation+1;
                if (m_rotation_system[a][i] == c) rotation = 10*rotation+2;
                if (m_rotation_system[a][i] == n) rotation = 10*rotation+3;
            }
            
            if (rotation == 123) return true;
            if (rotation == 231) return true;
            if (rotation == 312) return true;
            
            return false;
    }

    void rotation_systems_count_noncrossings()
    {
        // Did not work before but seems to work now....
        // assume u,v,w,x are picked vertices
        // the good rotation is uvw, uwx, uvx, vxw 
        m_rotation_system_noncrosssings = 0;
        for (int i = 0; i < m_vertices; i++)
        {
            m_rotation_system_noncrosssings_v[i] = 0;
        }
        
        for (int u=0; u < m_vertices-3; u++)
            for (int v=u+1; v < m_vertices-2; v++)
                for (int w=v+1; w < m_vertices-1; w++)
                    for (int x=w+1; x < m_vertices; x++)
                    {
                        bool r_u = is_rotation_even(u,  v,w,x); 
                        bool r_v = is_rotation_even(v,  u,w,x); 
                        bool r_w = is_rotation_even(w,  u,v,x); 
                        bool r_x = is_rotation_even(x,  u,v,w); 
                        
                        if ((r_u && !r_v && r_w && !r_x) || (!r_u && r_v && !r_w && r_x))
                        {
                            m_rotation_system_noncrosssings++;
                            m_rotation_system_noncrosssings_v[u]++;
                            m_rotation_system_noncrosssings_v[v]++;
                            m_rotation_system_noncrosssings_v[w]++;
                            m_rotation_system_noncrosssings_v[x]++;
                        }
                    }
        //m_rotation_system_noncrosssings = 0;
        
        for (int i = 0; i < m_vertices; i++) m_rotation_system_noncrosssings_v_sorted[i] = m_rotation_system_noncrosssings_v[i];
        std::sort(m_rotation_system_noncrosssings_v_sorted, m_rotation_system_noncrosssings_v_sorted+m_vertices);

    }
#endif

    void as_subflag(const flag &h,const int *mapping, int vertices, int theta, bool create_signature=true)
    {
        set_vertices_and_Theta(vertices,theta);
		
#ifdef G_COLORED_EDGES
        for (int x = 0; x < m_vertices-1; x++)
            for (int y = x+1; y < m_vertices; y++)
            {
                color_edge(x,y,h.m_color_edge[mapping[x]][mapping[y]]);
            }
#endif
        
#ifdef G_3EDGES
        for (int x = 0; x < m_vertices-2; x++)
            for (int y = x+1; y < m_vertices-1; y++)
                for (int z = y+1; z < m_vertices; z++)
                {
                    set_3edge(x,y,z,h.m_3edge[mapping[x]][mapping[y]][mapping[z]]);
                    
                }
#endif   

#ifdef G_ROOTED_3EDGES
        for (int x = 0; x < m_vertices-2; x++)
            for (int y = x+1; y < m_vertices-1; y++)
                for (int z = y+1; z < m_vertices; z++)
                {
                    if (h.m_rooted_3edge[mapping[x]][mapping[y]][mapping[z]] > 0)
                        set_rooted_3edge(x,y,z,h.m_rooted_3edge[mapping[x]][mapping[y]][mapping[z]]);
                    else if (h.m_rooted_3edge[mapping[y]][mapping[x]][mapping[z]] > 0)
                        set_rooted_3edge(y,x,z,h.m_rooted_3edge[mapping[y]][mapping[x]][mapping[z]]);
                    else if (h.m_rooted_3edge[mapping[z]][mapping[x]][mapping[y]] > 0)
                        set_rooted_3edge(z,x,y,h.m_rooted_3edge[mapping[z]][mapping[x]][mapping[y]]);
                    
                }
#endif 

#ifdef G_COLORED_3EDGES
        for (int x = 0; x < m_vertices-2; x++)
            for (int y = x+1; y < m_vertices-1; y++)
                for (int z = y+1; z < m_vertices; z++)
                {
                    color_3edge(x,y,z,h.m_colored_3edge[mapping[x]][mapping[y]][mapping[z]]);
                    
                }
#endif           
        
#ifdef G_4EDGES
        for (int x = 0; x < m_vertices-3; x++)
            for (int y = x+1; y < m_vertices-2; y++)
                for (int z = y+1; z < m_vertices-1; z++)
                    for (int w = z+1; w < m_vertices; w++)
                {
                    set_4edge(x,y,z,w,h.m_4edge[mapping[x]][mapping[y]][mapping[z]][mapping[w]]);
                    
                }
#endif  

#ifdef G_ROOTED_4EDGES
        for (int x = 0; x < m_vertices-3; x++)
            for (int y = x+1; y < m_vertices-2; y++)
                for (int z = y+1; z < m_vertices-1; z++)
                    for (int w = z+1; w < m_vertices; w++)
                {
                    if (h.m_rooted_4edge[mapping[x]][mapping[y]][mapping[z]][mapping[w]] > 0) set_rooted_4edge(x,y,z,w,1);
                    if (h.m_rooted_4edge[mapping[y]][mapping[x]][mapping[z]][mapping[w]] > 0) set_rooted_4edge(y,x,z,w,1);
                    if (h.m_rooted_4edge[mapping[z]][mapping[y]][mapping[x]][mapping[w]] > 0) set_rooted_4edge(z,y,x,w,1);
                    if (h.m_rooted_4edge[mapping[w]][mapping[y]][mapping[z]][mapping[x]] > 0) set_rooted_4edge(w,y,z,x,1);
                    
                }
#endif  


#ifdef G_MAYBE_ROOTED_KEDGES
        int inverse_mapping_h_to_this[h.m_vertices];
        for (int i  = 0; i < h.m_vertices; i++)
        {
            inverse_mapping_h_to_this[i] = -1; // not mapped
        }

        //cerr << endl << "Mapping is: ";
        for (int i  = 0; i < vertices; i++)
        {
            assert(mapping[i] < h.m_vertices);
            //cerr << i << "->" << mapping[i] << " ";
            inverse_mapping_h_to_this[mapping[i]]= i;
        }
        //cerr << endl;
        //cerr << "vertices="<<m_vertices << " h.vertices="<<h.m_vertices << endl;
        for(auto &re : h.m_rooted_kedges)
        {
            //cerr << "Working on " << re << endl;
            rooted_kedge mapped_re;
            if (re.root == -1) mapped_re.root = -1;
            else 
            {
                // Test if re.root is mapped
                if (inverse_mapping_h_to_this[re.root] == -1)
                    continue;;
                mapped_re.root = inverse_mapping_h_to_this[re.root];
            }

            //cerr << "Inserting " << endl;
            for(int i : re.vertices)
            {
                if (inverse_mapping_h_to_this[i] == -1)
                {
                    break;
                }
                //cerr << i << "from h  as " << inverse_mapping_h_to_this[i] << endl;
                //cerr << "Preinsert " << mapped_re << endl;
                mapped_re.vertices.insert(inverse_mapping_h_to_this[i]);
                //cerr << "Postinsert " << mapped_re << endl;
            }
            // Mapping probably failed, see above
            if (mapped_re.vertices.size() != G_MAYBE_ROOTED_KEDGES)
                continue;

            //cerr << "Mapped " << re << " as " << mapped_re << endl;
            //m_rooted_kedges.insert(mapped_re);
            add_maybe_rooted_kedge(mapped_re);
        }
#endif

#ifdef G_ROTATION_SYSTEM
        // we neeed to construct inverse map and remove neighbors of x that are not
        // mapped - this is then used to map take only subgraphs
        int inverse_map[V];
        for (int i = 0; i < V; i++) inverse_map[i] = -1;
        for (int x = 0; x < m_vertices; x++) inverse_map[mapping[x]] = x;
            
        for (int x = 0; x < m_vertices; x++)
        {
            int mx = mapping[x];
            m_rotation_system_neighbors[x] = 0;
            for (int u = 0; u < h.m_rotation_system_neighbors[mx]; u++)
            {
                if (inverse_map[ h.m_rotation_system[mx][u] ] != -1)
                {
                    m_rotation_system[x][m_rotation_system_neighbors[x]++] = inverse_map[ h.m_rotation_system[mx][u] ];
                }
                
            }
        }
        rotation_systems_count_noncrossings();        
#endif
        
        
#ifdef G_COLORED_VERTICES
		// copy vertices
        for (int x = 0; x < m_vertices; x++)
			color_vertex(x, h.m_color_vertex[mapping[x]]);
#endif

#ifdef G_CROSSINGS
		// copy crosings
		for (int u1 = 0; u1 < m_vertices; u1++)
		for (int u2 = 0; u2 < m_vertices; u2++)
		for (int v1 = 0; v1 < m_vertices; v1++)
		for (int v2 = 0; v2 < m_vertices; v2++)
		{
			m_cross_edges[u1][u2][v1][v2] = h.m_cross_edges[mapping[u1]][mapping[u2]][mapping[v1]][mapping[v2]];
		}
		m_crossings_cnt = count_crossings();
#endif
		
#ifdef G_CUTTINGS
		// copy crosings
		for (int u1 = 0; u1 < m_vertices; u1++)
		for (int u2 = 0; u2 < m_vertices; u2++)
		for (int v1 = 0; v1 < m_vertices; v1++)
		for (int v2 = 0; v2 < m_vertices; v2++)
		{
			m_cuttings[u1][u2][v1][v2] = h.m_cuttings[mapping[u1]][mapping[u2]][mapping[v1]][mapping[v2]];
		}
#endif
        
#ifdef G_LEFTRIGHT
		// copy crosings
		for (int u1 = 0; u1 < m_vertices; u1++)
        for (int u2 = 0; u2 < m_vertices; u2++)
        for (int v1 = 0; v1 < m_vertices; v1++)
        {
            m_leftright[u1][u2][v1] = h.m_leftright[mapping[u1]][mapping[u2]][mapping[v1]];
        }
#endif
        
#ifdef G_ORDER_TYPES
        for (int x = 0; x < m_vertices; x++)
            m_order_type[x] = h.m_order_type[mapping[x]];
#endif
        
#ifdef G_USE_LEXMIN_FOR_ISOMORPHISM
		if (create_signature) create_minlex_signature();
#endif
    }
    

    void as_subflag_in_blowup(const flag &h,const int *mapping, int vertices, int theta, bool create_signature=true)
    {
        set_vertices_and_Theta(vertices,theta);
        
#ifdef G_COLORED_EDGES
        for (int x = 0; x < m_vertices-1; x++)
            for (int y = x+1; y < m_vertices; y++)
            {
                if (mapping[x] != mapping[y])
                {
                    color_edge(x,y,h.m_color_edge[mapping[x]][mapping[y]]);
                }
                else
                {
                    // color_edge(x,y,G_BLOW_UP_COLOR_EDGES);
                    color_edge(x,y,g_blow_up_color_edges[mapping[x]]);
                }
            }
#endif
        
#ifdef G_3EDGES
        for (int x = 0; x < m_vertices-2; x++)
            for (int y = x+1; y < m_vertices-1; y++)
                for (int z = y+1; z < m_vertices; z++)
                {
                    if (mapping[x] != mapping[y] && mapping[x] != mapping[z] && mapping[z] != mapping[y])
                    {
                        set_3edge(x,y,z,h.m_3edge[mapping[x]][mapping[y]][mapping[z]]);
                    }
                    // else no edge
                }
#endif

#ifdef G_ROOTED_3EDGES
        for (int x = 0; x < m_vertices-2; x++)
            for (int y = x+1; y < m_vertices-1; y++)
                for (int z = y+1; z < m_vertices; z++)
                {
                    if (mapping[x] != mapping[y] && mapping[x] != mapping[z] && mapping[z] != mapping[y])
                    {
                        if (h.m_rooted_3edge[mapping[x]][mapping[y]][mapping[z]] > 0) 
                            set_rooted_3edge(x,y,z,h.m_rooted_3edge[mapping[x]][mapping[y]][mapping[z]]);
                        else if (h.m_rooted_3edge[mapping[y]][mapping[x]][mapping[z]] > 0) 
                            set_rooted_3edge(y,x,z,h.m_rooted_3edge[mapping[y]][mapping[x]][mapping[z]]);
                        else if (h.m_rooted_3edge[mapping[z]][mapping[y]][mapping[x]] > 0) 
                            set_rooted_3edge(z,y,x,h.m_rooted_3edge[mapping[z]][mapping[y]][mapping[x]]);

                    }
                    // else no edge
                }
#endif
        
#ifdef G_COLORED_3EDGES
        for (int x = 0; x < m_vertices-2; x++)
            for (int y = x+1; y < m_vertices-1; y++)
                for (int z = y+1; z < m_vertices; z++)
                {
                    if (mapping[x] != mapping[y] && mapping[x] != mapping[z] && mapping[z] != mapping[y])
                    {
                    color_3edge(x,y,z,h.m_colored_3edge[mapping[x]][mapping[y]][mapping[z]]);
                    }
                    else
                    {
                        //color_3edge(x,y,z,G_BLOW_UP_COLOR_3EDGES);
                        color_3edge(x,y,z,g_blow_up_color_3edges[mapping[x]]);
                    }
                }
#endif
        
#ifdef G_4EDGES
        for (int x = 0; x < m_vertices-3; x++)
            for (int y = x+1; y < m_vertices-2; y++)
                for (int z = y+1; z < m_vertices-1; z++)
                    for (int w = z+1; w < m_vertices; w++)
                    {
                        if (mapping[x] != mapping[y] && mapping[x] != mapping[z] && mapping[z] != mapping[y] && mapping[w] != mapping[x] && mapping[w] != mapping[y] && mapping[w] != mapping[z])
                        set_4edge(x,y,z,w,h.m_4edge[mapping[x]][mapping[y]][mapping[z]][mapping[w]]);
                        
                    }
#endif

        
#ifdef G_ROOTED_4EDGES
        for (int x = 0; x < m_vertices-3; x++)
            for (int y = x+1; y < m_vertices-2; y++)
                for (int z = y+1; z < m_vertices-1; z++)
                    for (int w = z+1; w < m_vertices; w++)
                    {
                        if (mapping[x] != mapping[y] && mapping[x] != mapping[z] && mapping[z] != mapping[y] && mapping[w] != mapping[x] && mapping[w] != mapping[y] && mapping[w] != mapping[z])
                        {
                            if (h.m_rooted_4edge[mapping[x]][mapping[y]][mapping[z]][mapping[w]] > 0) set_rooted_4edge(x,y,z,w,1);
                            if (h.m_rooted_4edge[mapping[y]][mapping[x]][mapping[z]][mapping[w]] > 0) set_rooted_4edge(y,x,z,w,1);
                            if (h.m_rooted_4edge[mapping[z]][mapping[y]][mapping[x]][mapping[w]] > 0) set_rooted_4edge(z,y,x,w,1);
                            if (h.m_rooted_4edge[mapping[w]][mapping[y]][mapping[z]][mapping[x]] > 0) set_rooted_4edge(w,y,z,x,1);
                        }
                    }
#endif

#ifdef G_MAYBE_ROOTED_KEDGES
        assert(0);
#endif

#ifdef G_ROTATION_SYSTEM
        assert(0);
#endif
        
#ifdef G_COLORED_VERTICES
        // copy vertices
        for (int x = 0; x < m_vertices; x++)
            color_vertex(x, h.m_color_vertex[mapping[x]]);
#endif
        
#ifdef G_CROSSINGS
        // copy crosings
        for (int u1 = 0; u1 < m_vertices; u1++)
            for (int u2 = 0; u2 < m_vertices; u2++)
                for (int v1 = 0; v1 < m_vertices; v1++)
                    for (int v2 = 0; v2 < m_vertices; v2++)
                    {
                        m_cross_edges[u1][u2][v1][v2] = h.m_cross_edges[mapping[u1]][mapping[u2]][mapping[v1]][mapping[v2]];
                    }
        m_crossings_cnt = count_crossings();
#endif
        
#ifdef G_CUTTINGS
        // copy crosings
        for (int u1 = 0; u1 < m_vertices; u1++)
            for (int u2 = 0; u2 < m_vertices; u2++)
                for (int v1 = 0; v1 < m_vertices; v1++)
                    for (int v2 = 0; v2 < m_vertices; v2++)
                    {
                        m_cuttings[u1][u2][v1][v2] = h.m_cuttings[mapping[u1]][mapping[u2]][mapping[v1]][mapping[v2]];
                    }
#endif
        
#ifdef G_LEFTRIGHT
        assert(0);
        // copy left right - not correct since left/right is not calculated for the new vertices
        for (int u1 = 0; u1 < m_vertices; u1++)
            for (int u2 = 0; u2 < m_vertices; u2++)
                for (int v1 = 0; v1 < m_vertices; v1++)
                {
                    m_leftright[u1][u2][v1] = h.m_leftright[mapping[u1]][mapping[u2]][mapping[v1]];
                }
#endif

        
#ifdef G_ORDER_TYPES
        assert(0);
#endif
        
#ifdef G_USE_LEXMIN_FOR_ISOMORPHISM
        if (create_signature) create_minlex_signature();
#endif
    }
    
    
        
    int e()
    {
        return (m_vertices*(m_vertices-1))/2;
    }
    
    int e3()
    {
        return (m_vertices*(m_vertices-1)*(m_vertices-2))/6;
    }	
	
	
    bool contains_as_subflag_map_rest(const flag &g, int* mapping, int next_to_map, int first_available) const
    {
        if (next_to_map >= g.m_vertices)
        {
            flag h;
            h.as_subflag(*this, mapping,g.m_vertices, g.m_Theta);
            
            //This may happen!!
            // assert(g.m_Theta == 0);
         
            //cerr << "Testing " << h.print() << " and " << g.print() << endl;

            
            if (h.is_isomorphic_to(g))
            {
                return true;
            }
                         
            return false;
        }
        
#ifndef G_ORDERED_VERTICES
        if (first_available < g.m_Theta)
        {
            mapping[next_to_map] = first_available;
            if (contains_as_subflag_map_rest(g,mapping,next_to_map+1,first_available+1)) return true;            
        }
        else
        {
#endif
        for (int i = first_available; i < m_vertices; i++)
        {
            mapping[next_to_map] = i;
            if (contains_as_subflag_map_rest(g,mapping,next_to_map+1,i+1)) return true;
        }
#ifndef G_ORDERED_VERTICES
        }
#endif
        
        return false;
    }
    
    // Count density of g as a subflags
    bool contains_as_subflag(const flag &g) const
    {
        if (g.m_vertices > m_vertices) return false;
        
        int mapping[g.m_vertices];
                
        return contains_as_subflag_map_rest(g,mapping,0,0);        
    }
    
    
	void density_subflag_map_rest(const flag &g, int &total, int &good, int* mapping, int next_to_map, int first_available) const
	{
		if (next_to_map >= g.m_vertices)
		{
			flag h;
			h.as_subflag(*this, mapping,g.m_vertices, g.m_Theta);
			
			assert(g.m_Theta == 0);
			
			if (h.is_isomorphic_to(g)) good++;
			
			total++;
			return;
		}
		
		for (int i = first_available; i < m_vertices; i++)
		{
			mapping[next_to_map] = i;
			density_subflag_map_rest(g,total,good,mapping,next_to_map+1,i+1);
		}
	}
	
	// Count density of g as a subflags
    double density_subflag(const flag &g) const
    {
		if (g.m_vertices > m_vertices) return 0;
		
		int mapping[g.m_vertices];
		
		int total = 0;
		int good = 0;
		
		density_subflag_map_rest(g,total,good,mapping,0,0);
		
        return (double)good/(double)total;
	}

	int count_subflag(const flag &g) const
	{
		if (g.m_vertices > m_vertices) return 0;
		
		int mapping[g.m_vertices];
		
		int total = 0;
		int good = 0;
		
		density_subflag_map_rest(g,total,good,mapping,0,0);
		
      return good;
	}
    

    
    
    
    
    void count_labeled_copies_of_map_rest(const flag &g_labeled, int &good_maps, int* mapping, int *used, int next_to_map) const
    {
        if (next_to_map >= g_labeled.m_vertices)
        {
            flag h;
            h.as_subflag(*this, mapping,g_labeled.m_vertices, g_labeled.m_Theta);
            
            assert(g_labeled.m_Theta == g_labeled.m_vertices);
            
            if (h.have_same_type(g_labeled)) good_maps++;

            return;
        }
        
        for (int i = 0; i < m_vertices; i++)
        {
            if (used[i]) continue;
            mapping[next_to_map] = i;
            used[i] = 1;
            count_labeled_copies_of_map_rest(g_labeled,good_maps,mapping,used,next_to_map+1);
            used[i] = 0;
        }
    }
    

    int count_labeled_copies_of(const flag &g) const
    {
        if (g.m_vertices > m_vertices) return 0;
       
        // This does not work correctly for unlabeled graphs
        assert(g.m_Theta == 0 && m_Theta == 0);
        
        
        // We use have_same_type to check correct mappings...
        flag g_labeled = g;
        g_labeled.m_Theta = g.m_vertices;
        
        int mapping[g.m_vertices];
        int used[m_vertices];
        for (int i = 0; i < m_vertices; i++)
        {
            used[i] = 0;
        }
        
        int good_maps = 0;
        
        count_labeled_copies_of_map_rest(g_labeled,good_maps,mapping,used,0);
        
        return good_maps;
    }
    
    
    
    void generate_subflags_of_size_n_rest(int n, vector<flag> &subflags, int* mapping, int next_to_map, int first_available, const vector<int> &available_to_map) const
    {
        //cerr << "Runnig n=" << n << " mapping=" << mapping[0] << "," << mapping[1] << "," << mapping[2] << " next_to_map=" << next_to_map << endl; 

        if (next_to_map >= n)
        {
            flag h;
            h.as_subflag(*this, mapping, n, 0);
            bool h_is_new = true;
            for (int i = 0; i < (int)subflags.size(); ++i)
            {
                if (h.is_isomorphic_to(subflags[i])) 
                {
                    h_is_new = false;
                    break;
                } 
            }
            if (h_is_new) subflags.push_back(h);            
            return;
        }

        //cerr << "first_available=" << first_available  << " (n-next_to_map)=" << (n-next_to_map) << endl;
        //cerr << "(int)available_to_map.size()-(n-next_to_map)=" << (int)available_to_map.size()-(n-next_to_map) << endl;
        for (int i = first_available; i < (int)available_to_map.size()-(n-next_to_map-1); i++)
        {
            //cerr << "i=" << i << endl;
            mapping[next_to_map] = available_to_map[i];
            generate_subflags_of_size_n_rest(n,subflags,mapping,next_to_map+1,i+1,available_to_map);
        }
    }
   
    
    // Could be written as a template with size of in_subflag - should be faster
    // an/or with arrays
    // if in_subrgaph is specified, then only subflags containing in_subrgaph are generated
    int generate_subflags_of_size_n(int n, vector<flag> &subflags, const vector<int> &in_subflag = vector<int>()) const
    {
        if (n > m_vertices) return 0;
        
        // This does not work correctly for unlabeled graphs
        assert(m_Theta == 0);
                
        int mapping[n];
        
        vector<int> available_to_map;
        for (int i = 0; i < m_vertices; i++)
            if (find(in_subflag.begin(), in_subflag.end(), i) == in_subflag.end()) 
                available_to_map.push_back(i);
        
        for (int i = 0; i < (int)in_subflag.size(); i++)
            mapping[i] = in_subflag[i];
        
        //cerr << "Working on it... " << available_to_map.size() << endl;
        //cerr << "Starting with " << (int)in_subflag.size() << endl;

        generate_subflags_of_size_n_rest(n, subflags, mapping, (int)in_subflag.size(),0,available_to_map);
        
        return (int)subflags.size();
    }
    
    
    
    
    // Copy data from g if the number of vertices of g is <= current number
    void copy_from(const flag &g)
    {
        assert(m_vertices >= g.m_vertices);
     
#ifdef G_COLORED_VERTICES
        // copy vertices
        for (int x = 0; x < g.m_vertices; x++)
            color_vertex(x, g.m_color_vertex[x]);
#endif        
        
#ifdef G_3EDGES
        for (int x = 0; x < g.m_vertices-2; x++)
            for (int y = x+1; y < g.m_vertices-1; y++)
                for (int z = y+1; z < g.m_vertices; z++)
                {
                    set_3edge(x,y,z,g.m_3edge[x][y][z]);                    
                }
#endif

        
#ifdef G_ROOTED_3EDGES
        for (int x = 0; x < g.m_vertices-2; x++)
            for (int y = x+1; y < g.m_vertices-1; y++)
                for (int z = y+1; z < g.m_vertices; z++)
                {
                    if (g.m_rooted_3edge[x][y][z] > 0)
                        set_rooted_3edge(x,y,z,g.m_rooted_3edge[x][y][z]); 
                    else  if (g.m_rooted_3edge[y][x][z] > 0)
                        set_rooted_3edge(y,x,z,g.m_rooted_3edge[y][x][z]);                   
                    else  if (g.m_rooted_3edge[z][x][y] > 0)
                        set_rooted_3edge(z,x,y,g.m_rooted_3edge[z][x][y]);                   
                }
#endif

#ifdef G_COLORED_3EDGES
        for (int x = 0; x < g.m_vertices-2; x++)
            for (int y = x+1; y < g.m_vertices-1; y++)
                for (int z = y+1; z < g.m_vertices; z++)
                {
                    color_3edge(x,y,z,g.m_colored_3edge[x][y][z]);                    
                }
#endif        
        
#ifdef G_4EDGES
        for (int x = 0; x < g.m_vertices-3; x++)
            for (int y = x+1; y < g.m_vertices-2; y++)
                for (int z = y+1; z < g.m_vertices-1; z++)
                    for (int w = z+1; w < g.m_vertices; w++)
                {
                    set_4edge(x,y,z,w,g.m_4edge[x][y][z][w]);                    
                }
#endif        

#ifdef G_ROOTED_4EDGES
        for (int x = 0; x < g.m_vertices-3; x++)
            for (int y = x+1; y < g.m_vertices-2; y++)
                for (int z = y+1; z < g.m_vertices-1; z++)
                    for (int w = z+1; w < g.m_vertices; w++)
                {
                    if (g.m_rooted_4edge[x][y][z][w] > 0) set_rooted_4edge(x,y,z,w,1);                    
                    if (g.m_rooted_4edge[y][x][z][w] > 0) set_rooted_4edge(y,x,z,w,1);                    
                    if (g.m_rooted_4edge[z][y][x][w] > 0) set_rooted_4edge(z,y,x,w,1);                    
                    if (g.m_rooted_4edge[w][y][z][x] > 0) set_rooted_4edge(w,y,z,x,1);                    
                }
#endif  

#ifdef G_MAYBE_ROOTED_KEDGES
    for (auto re : g.m_rooted_kedges)
    {
        add_maybe_rooted_kedge(re);
    }
#endif


#ifdef G_CROSSINGS
		for (int u1 = 0; u1 < g.m_vertices; u1++)
		{
			for (int u2 = u1+1; u2 < g.m_vertices; u2++)
			{
				for (int v1 = u1+1; v1 < g.m_vertices; v1++)
				{
					for (int v2 = v1+1; v2 < g.m_vertices; v2++)
					{
						if (g.m_cross_edges[u1][u2][v1][v2] != 0)
						{   
                            set_edges_crossing(u1,u2,v1,v2, g.m_cross_edges[u1][u2][v1][v2]);
						}
					}
				}
			}
		}
		cout << " ";
#endif


#ifdef G_CUTTINGS
    assert(0);
#endif
        
#ifdef G_LEFTRIGHT
        assert(0);
#endif

#ifdef G_ROTATION_SYSTEM
        assert(0);
#endif
        
        
#ifdef G_COLORED_EDGES
        for (int x = 0; x < g.m_vertices-1; x++)
            for (int y = x+1; y < g.m_vertices; y++)
            {
                color_edge(x,y,g.m_color_edge[x][y]);
            }        
#endif        
    }
    
/***************************************************************************************************************** Printing ****************/
	void print_for_human() const
	{
		// Prints the number of vertices, zero as not type and then part of the diagonal matrix
		cout << m_vertices << " " << m_Theta << " ";
		
#ifdef G_COLORED_VERTICES
		print_vertex_coloring(cout, " ");
		cout << " ";
#endif
		
#ifdef G_CROSSINGS
		for (int u1 = 0; u1 < m_vertices; u1++)
		{
			for (int u2 = u1+1; u2 < m_vertices; u2++)
			{
				for (int v1 = u1+1; v1 < m_vertices; v1++)
				{
					for (int v2 = v1+1; v2 < m_vertices; v2++)
					{
						if (m_cross_edges[u1][u2][v1][v2] != 0)
						{                            
#ifdef G_CROSSINGS_SIGNED                            
							cout << " " << u1 << u2 << "X" << m_cross_edges[u1][u2][v1][v2] << "X" << v1 << v2;
#else
							cout << " " << u1 << u2 << "X" << v1 << v2;
#endif
						}
					}
				}
			}
		}
		cout << " ";
#endif
		cout << endl;
	}

    template<typename T>
    string print_latex(bool use_label, const T &graph_label, bool color_1_nonedge = false)
    {
        if (m_vertices == 0)
        {
            return "";
        }
        
        stringstream ss;
        
        // Prints the number of vertices, zero as not type and then part of the diagonal matrix
#ifndef G_ORDER_TYPES     
        ss << " %\n\\begin{tikzpicture}[flag_pic]";
        ss << "\\outercycle{" << m_vertices << "}{" << m_Theta << "}\n";
#else
        int min_x =  m_order_type[0].first;
        int max_x =  m_order_type[0].first;
        int min_y =  m_order_type[0].second;
        int max_y =  m_order_type[0].second;
        for (int cnt = 1; cnt < m_vertices; cnt++)
        {
            if (m_order_type[cnt].first < min_x) min_x = m_order_type[cnt].first;
            if (m_order_type[cnt].first > max_x) max_x = m_order_type[cnt].first;
            if (m_order_type[cnt].second < min_y) min_y = m_order_type[cnt].second;
            if (m_order_type[cnt].second > max_y) max_y = m_order_type[cnt].second;
        }
        if (min_x == max_x) max_x++;
        if (min_y == max_y) max_y++;
        double scale_x = 5.0/(max_x - min_x);
        double scale_y = 5.0/(max_y - min_y);
        ss << "\n\\begin{tikzpicture}[flag_pic, xscale="<<scale_x<<",yscale="<<scale_y<<"]"<<endl;
        ss << "\\draw ";
        for (int cnt = 0; cnt < m_vertices; cnt++)
        {
            //ss << "(" <<  (m_order_type[cnt].first-min_x)*scale_x << "," << (m_order_type[cnt].second-min_y)*scale_y << ") coordinate (x"<< cnt <<") ";
            ss << "(" <<  m_order_type[cnt].first << "," << m_order_type[cnt].second << ") coordinate (x"<< cnt <<") ";
        }
        ss << ";" << endl;
#endif        

#ifdef G_ORDERED_VERTICES
        const string edgestr = " to[bend left] ";
#else
        const string edgestr = "--";        
#endif        
        
#ifdef G_COLORED_EDGES
        for (int u = 0; u < m_vertices; u++)
        {
            for (int v = u+1; v < m_vertices; v++)
            {
#ifdef G_ORIENTED_EDGES
                if (abs(m_color_edge[u][v]) <= G_ORIENTED_EDGES_UNORIENTED_COLORS)
                    ss << "\\draw[edge_color" << m_color_edge[u][v] << "] (x"<<u<<")" << edgestr << "(x"<<v<<");";
                else
                    if (m_color_edge[u][v] > 0)
                        ss << "\\draw[edge_color" << m_color_edge[u][v] << ", -latex] (x"<<u<<")" << edgestr << "(x"<<v<<");";
                    else
                        ss << "\\draw[edge_color" << -m_color_edge[u][v] << ", latex-] (x"<<u<<")" << edgestr << "(x"<<v<<");";                    
#else
                ss << "\\draw[edge_color" << m_color_edge[u][v] << "] (x"<<u<<")" << edgestr << "(x"<<v<<");"; 
#endif
            }
            ss << "  ";
        }
        ss << endl;
#else  // Edges have no colors
#ifdef G_ORDER_TYPES
        for (int u = 0; u < m_vertices; u++)
        {
            for (int v = u+1; v < m_vertices; v++)
            {

#ifdef G_COLORED_VERTICES
                if (m_color_vertex[u] == m_color_vertex[v]) 
                {
                    ss << "\\draw[edge_hidden] (x"<<u<<")" << edgestr << "(x"<<v<<");"; 
                    continue;
                }
#endif
                ss << "\\draw[edge_thin] (x"<<u<<")" << edgestr << "(x"<<v<<");"; 
            }
        }
#endif
#endif
        
      
#ifdef G_COLORED_VERTICES
        for (int i = 0; i < m_vertices; i++)
        {
            if (i < m_Theta)
                ss << "\\draw (x"<<i<<") node[labeled_vertex_color" << m_color_vertex[i] << "]{};";
            else
                ss << "\\draw (x"<<i<<") node[vertex_color" << m_color_vertex[i] << "]{};";
        }
        ss << endl;
#else
        for (int i = 0; i < m_vertices; i++)
        {
            if (i < m_Theta)
                ss << "\\draw (x"<<i<<") node[labeled_vertex]{};";
            else
                ss << "\\draw (x"<<i<<") node[unlabeled_vertex]{};";
        }
        ss << endl;
#endif
        for (int i = 0; i < m_vertices; i++)
        {
            ss << "\\labelvertex{" << i << "}";
        }
        
#ifdef G_ROTATION_SYSTEM
        //assert(0);
#endif
        
#ifdef G_3EDGES
       int edge_id = 0;
       for (int u = 0; u < m_vertices; u++)
         for (int v = u+1; v < m_vertices; v++)
            for (int w = v+1; w < m_vertices; w++)
                {
                    if (m_3edge[u][v][w])
                    {
                        ss << "\\drawhyperedge{"<<edge_id<<"}{"<<m_vertices<<"}" << endl;
                        ss << "\\drawhypervertex{"<<u<<"}{"<<edge_id<<"}" << endl;
                        ss << "\\drawhypervertex{"<<v<<"}{"<<edge_id<<"}" << endl;
                        ss << "\\drawhypervertex{"<<w<<"}{"<<edge_id<<"}" << endl;
                        edge_id++;                     
                    }
                }
#endif

#ifdef G_ROOTED_3EDGES
       int rooted_edge_id = 0;
#ifdef G_3EDGES
        rooted_edge_id = edge_id;
#endif       
       for (int u = 0; u < m_vertices; u++)
         for (int v = u+1; v < m_vertices; v++)
            for (int w = v+1; w < m_vertices; w++)
                {
                    if (m_rooted_3edge[u][v][w])
                    {
                        ss << "\\drawhyperedge{"<<rooted_edge_id<<"}{"<<m_vertices<<"}" << endl;
                        if (m_rooted_3edge[u][v][w] == -1) ss << "\\drawrootedhypervertex{"<<u<<"}{"<<rooted_edge_id<<"}" << endl;
                        else ss << "\\drawrootedhyperroot{"<<u<<"}{"<<rooted_edge_id<<"}" << endl;
                        if (m_rooted_3edge[v][u][w] == -1) ss << "\\drawrootedhypervertex{"<<v<<"}{"<<rooted_edge_id<<"}" << endl;
                        else ss << "\\drawrootedhyperroot{"<<v<<"}{"<<rooted_edge_id<<"}" << endl;
                        if (m_rooted_3edge[w][v][u] == -1) ss << "\\drawrootedhypervertex{"<<w<<"}{"<<rooted_edge_id<<"}" << endl;
                        else ss << "\\drawrootedhyperroot{"<<w<<"}{"<<rooted_edge_id<<"}" << endl;
                        rooted_edge_id++;                     
                    }
                }
#endif


#ifdef G_COLORED_3EDGES
       int edge_id = 0;
       for (int u = 0; u < m_vertices; u++)
         for (int v = u+1; v < m_vertices; v++)
            for (int w = v+1; w < m_vertices; w++)
                {
                    if (color_1_nonedge == true || m_colored_3edge[u][v][w] != 1)
                    {
                        ss << "\\drawhyperedge{"<<edge_id<<"}{"<<m_vertices<<"}" << endl;
                        ss << "\\drawhypervertexcolor{"<<u<<"}{"<<edge_id<<"}{"<< m_colored_3edge[u][v][w]<<"}"  << endl;
                        ss << "\\drawhypervertexcolor{"<<v<<"}{"<<edge_id<<"}{"<< m_colored_3edge[u][v][w]<<"}"  << endl;
                        ss << "\\drawhypervertexcolor{"<<w<<"}{"<<edge_id<<"}{"<< m_colored_3edge[u][v][w]<<"}"  << endl;
                        edge_id++;                     
                    }
                }
#endif          
        
#ifdef G_4EDGES
       int edge_id = 0;
       for (int u = 0; u < m_vertices; u++)
         for (int v = u+1; v < m_vertices; v++)
            for (int x = v+1; x < m_vertices; x++)
                for (int y = x+1; y < m_vertices; y++)
                {
                    if (m_4edge[u][v][x][y])
                    {
                        ss << "\\drawhyperedge{"<<edge_id<<"}{"<<m_vertices<<"}" << endl;
                        ss << "\\drawhypervertex{"<<u<<"}{"<<edge_id<<"}" << endl;
                        ss << "\\drawhypervertex{"<<v<<"}{"<<edge_id<<"}" << endl;
                        ss << "\\drawhypervertex{"<<x<<"}{"<<edge_id<<"}" << endl;
                        ss << "\\drawhypervertex{"<<y<<"}{"<<edge_id<<"}" << endl;
                        edge_id++;                     
                    }
                }
#endif    

#ifdef G_ROOTED_4EDGES
       int rooted_4edge_id = 0;
#ifdef G_4EDGES
        rooted_4edge_id = edge_id;
#endif         
       for (int u = 0; u < m_vertices; u++)
         for (int v = u+1; v < m_vertices; v++)
            for (int x = v+1; x < m_vertices; x++)
                for (int y = x+1; y < m_vertices; y++)
                {
                    if (m_rooted_4edge[u][v][x][y])
                    {
                        ss << "\\drawhyperedge{"<<rooted_4edge_id<<"}{"<<m_vertices<<"}" << endl;
                        if (m_rooted_4edge[u][v][x][y] == -1) ss << "\\drawrootedhypervertex{"<<u<<"}{"<<rooted_4edge_id<<"}" << endl;
                        else ss << "\\drawrootedhyperroot{"<<u<<"}{"<<rooted_4edge_id<<"}" << endl;
                        
                        if (m_rooted_4edge[v][u][x][y] == -1) ss << "\\drawrootedhypervertex{"<<v<<"}{"<<rooted_4edge_id<<"}" << endl;
                        else ss << "\\drawrootedhyperroot{"<<v<<"}{"<<rooted_4edge_id<<"}" << endl;

                        if (m_rooted_4edge[x][v][u][y] == -1) ss << "\\drawrootedhypervertex{"<<x<<"}{"<<rooted_4edge_id<<"}" << endl;
                        else ss << "\\drawrootedhyperroot{"<<x<<"}{"<<rooted_4edge_id<<"}" << endl;
                        
                        if (m_rooted_4edge[y][v][x][u] == -1) ss << "\\drawrootedhypervertex{"<<y<<"}{"<<rooted_4edge_id<<"}" << endl;
                        else ss << "\\drawrootedhyperroot{"<<y<<"}{"<<rooted_4edge_id<<"}" << endl;

                        rooted_4edge_id++;                     
                    }
                }
#endif    



#ifdef G_MAYBE_ROOTED_KEDGES
       int maybe_rooted_kedge_id = 0;
       for (auto &re : m_rooted_kedges)
       {
            ss << "\\drawhyperedge{"<<maybe_rooted_kedge_id<<"}{"<<m_vertices<<"}" << endl;

            for (int v : re.vertices)
            {
                if (v == re.root)
                    ss << "\\drawrootedhyperroot{"<<v<<"}{"<<maybe_rooted_kedge_id<<"}" << endl;
                else
                    ss << "\\drawrootedhypervertex{"<<v<<"}{"<<maybe_rooted_kedge_id<<"}" << endl;
            }
            maybe_rooted_kedge_id++;                     
        }
#endif



#ifdef G_CROSSINGS
        //assert(0);
#endif
        
#ifdef G_CUTTINGS
        assert(0);
#endif
        
#ifdef G_LEFTRIGHT
        //assert(0);
#endif	
        if (use_label)
        {
            ss << "\\draw (labelpoint) node{"<<graph_label<<"};" << endl;
        }
        ss << "\\end{tikzpicture} " << endl;
        
        return ss.str();
    }
    
    
	
	string print(const string &delimeter=" ") const
	{
		stringstream ss;
		
		// Prints the number of vertices, zero as not type and then part of the diagonal matrix
        ss << m_vertices << delimeter;
        if (m_Theta_class != 0)
            ss << m_Theta << "." <<  m_Theta_class << delimeter << delimeter;
        else 
            ss << m_Theta << delimeter << delimeter;
        
        

        
#ifdef G_COLORED_VERTICES
		print_vertex_coloring(ss, delimeter);
		ss << delimeter;
#endif

#ifdef G_COLORED_EDGES
  		for (int u = 0; u < m_vertices; u++)
  		{
    		for (int v = u+1; v < m_vertices; v++)
				ss << delimeter << m_color_edge[u][v];
            ss << delimeter;
		}
#endif

//#define G_3EDGES
//#define G_COLORED_3EDGES_PRINT_HUMAN

#ifdef G_3EDGES
#ifdef G_3EDGES_PRINT_HUMAN
        ss << delimeter << delimeter << m_3edges_cnt;
        for (int x = 0; x < m_vertices-2; x++)
            for (int y = x+1; y < m_vertices-1; y++)
                for (int z = y+1; z < m_vertices; z++)
                {
                    if (m_3edge[x][y][z] == 1)
                        ss << delimeter << (char)('1'+x) << (char)('1'+y) << (char)('1'+z);
                }
        ss << delimeter << delimeter;        
#else
        ss << delimeter << delimeter << m_3edges_cnt;
        for (int x = 0; x < m_vertices-2; x++)
            for (int y = x+1; y < m_vertices-1; y++)
                for (int z = y+1; z < m_vertices; z++)
                {
                    if (m_3edge[x][y][z]) ss << delimeter << (char)('1'+x) << (char)('1'+y) << (char)('1'+z);
                }
        ss << delimeter << delimeter;
#endif
#endif   

#ifdef G_ROOTED_3EDGES
        ss << delimeter << delimeter << m_rooted_3edges_cnt;
        for (int x = 0; x < m_vertices-2; x++)
            for (int y = x+1; y < m_vertices-1; y++)
                for (int z = y+1; z < m_vertices; z++)
                {
                    if (m_rooted_3edge[x][y][z] > 0)
                        ss << delimeter << (char)('1'+x) << (char)('1'+y) << (char)('1'+z);
                    if (m_rooted_3edge[y][x][z] > 0)
                        ss << delimeter << (char)('1'+y) << (char)('1'+x) << (char)('1'+z);
                    if (m_rooted_3edge[z][x][y] > 0)
                        ss << delimeter << (char)('1'+z) << (char)('1'+x) << (char)('1'+y);
                }
        ss << delimeter << delimeter;        
#endif   





#ifdef G_COLORED_3EDGES
#ifdef G_COLORED_3EDGES_PRINT_HUMAN
        int colored_3edges_count = 0;
        for (int c = 1; c < COLORS_3EDGES; c++)
        {
            colored_3edges_count += m_colored_3edges[c];
        }
        ss << delimeter << delimeter << colored_3edges_count;
        for (int x = 0; x < m_vertices-2; x++)
            for (int y = x+1; y < m_vertices-1; y++)
                for (int z = y+1; z < m_vertices; z++)
                {
                    if (m_colored_3edge[x][y][z] != 0)
                        ss << delimeter << (char)('1'+x) << (char)('1'+y) << (char)('1'+z) << '.' << (char)('0'+m_colored_3edge[x][y][z]);
                }
        ss << delimeter << delimeter;
        
#else        
        ss << delimeter << delimeter;
        ss << "-1" << delimeter;
        for (int x = 0; x < m_vertices-2; x++)
        {
            for (int y = x+1; y < m_vertices-1; y++)
            {
                for (int z = y+1; z < m_vertices; z++)
                {
                    ss << delimeter << m_colored_3edge[x][y][z];
                }
                ss << delimeter;
            }
            ss << delimeter;
        }
#endif        
#endif          
        
#ifdef G_4EDGES
        ss << delimeter << delimeter << m_4edges_cnt;
        for (int x = 0; x < m_vertices-3; x++)
            for (int y = x+1; y < m_vertices-2; y++)
                for (int z = y+1; z < m_vertices-1; z++)
                    for (int w = z+1; w < m_vertices; w++)
                {
                    if (m_4edge[x][y][z][w]) ss << delimeter << (char)('1'+x) << (char)('1'+y) << (char)('1'+z)<< (char)('1'+w);
                }
        ss << delimeter << delimeter;
#endif

#ifdef G_ROOTED_4EDGES
        ss << delimeter << delimeter << m_rooted_4edges_cnt;
        for (int x = 0; x < m_vertices-3; x++)
            for (int y = x+1; y < m_vertices-2; y++)
                for (int z = y+1; z < m_vertices-1; z++)
                    for (int w = z+1; w < m_vertices; w++)
                {
                    if (m_rooted_4edge[x][y][z][w] > 0) ss << delimeter << (char)('1'+x) << (char)('1'+y) << (char)('1'+z)<< (char)('1'+w);
                    if (m_rooted_4edge[y][x][z][w] > 0) ss << delimeter << (char)('1'+y) << (char)('1'+x) << (char)('1'+z)<< (char)('1'+w);
                    if (m_rooted_4edge[z][y][x][w] > 0) ss << delimeter << (char)('1'+z) << (char)('1'+y) << (char)('1'+x)<< (char)('1'+w);
                    if (m_rooted_4edge[w][y][z][x] > 0) ss << delimeter << (char)('1'+w) << (char)('1'+y) << (char)('1'+z)<< (char)('1'+x);
                }
        ss << delimeter << delimeter;
#endif

#ifdef G_MAYBE_ROOTED_KEDGES
        ss << delimeter << delimeter << m_rooted_kedges.size();
        for (auto re : m_rooted_kedges)
        {
            ss << delimeter << re; 
        }
        ss << delimeter << delimeter;
#endif

#ifdef G_CROSSINGS
		print_crossings(ss, delimeter);
		ss << delimeter;
#endif
		
		
#ifdef G_CUTTINGS
		print_cuttings(ss, delimeter);
		ss << delimeter;
#endif
        
#ifdef G_LEFTRIGHT
		print_leftright(ss, delimeter);
		ss << delimeter;
#endif	
        
#ifdef G_ROTATION_SYSTEM
        ss << delimeter << delimeter;
        for (int x = 0; x < m_vertices; x++)
        {
            ss << delimeter << (char)(x+'1') << ":";
            for (int y = 0; y < m_rotation_system_neighbors[x]; y++)
                ss << (char)(m_rotation_system[x][y]+'1');
        }
        ss << delimeter << delimeter;
#endif
        
#ifdef G_ORDER_TYPES
        ss << delimeter << delimeter << delimeter;
        for (int cnt = 0; cnt < m_vertices; cnt++)
        {
            ss << m_order_type[cnt].first << delimeter << m_order_type[cnt].second << delimeter << delimeter;
        }
#endif           
		return ss.str();
	}
    
#ifdef G_COLORED_VERTICES
	void print_vertex_coloring(ostream &ostr, const string &delimiter) const
	{
		for (int i = 0; i < m_vertices; i++)
		{
			ostr << delimiter << m_color_vertex[i];
		}
	}
#endif
	
#ifdef G_CROSSINGS
	void print_crossings(ostream &ostr, const string &delimiter) const
	{
#ifdef G_CROSSINGS_HUMAN_READABLE
        //if (m_vertices > 4)
        {
            ostr << delimiter << m_crossings_cnt << delimiter;
        }
#endif

		for (int u1 = 0; u1 < m_vertices; u1++)
		{
			for (int u2 = u1+1; u2 < m_vertices; u2++)
			{
				for (int v1 = u1+1; v1 < m_vertices; v1++)
				{
					for (int v2 = v1+1; v2 < m_vertices; v2++)
					{
						if (u2 == v1 || u2==v2)  continue;
#ifdef G_CROSSINGS_HUMAN_READABLE
                        if (m_cross_edges[u1][u2][v1][v2] != 0)
                        {
#ifdef G_CROSSINGS_SIGNED                                                        
    						ostr << delimiter << u1 << delimiter << u2 << delimiter  << m_cross_edges[u1][u2][v1][v2] << delimiter << v1 << delimiter << v2 << delimiter;
#else
    						ostr << delimiter << u1 << delimiter << u2 << delimiter << v1 << delimiter << v2 << delimiter;
#endif                            
                        }
#else                        
						ostr << delimiter << m_cross_edges[u1][u2][v1][v2];
#endif                        
					}
				}
			}
		}
	}
#endif
	
#ifdef G_CUTTINGS
	void print_cuttings(ostream &ostr, const string &delimiter) const
	{
		for (int u1 = 0; u1 < m_vertices; u1++)
		{
			for (int u2 = u1+1; u2 < m_vertices; u2++)
			{
				for (int v1 = 0; v1 < m_vertices; v1++)
				{
					for (int v2 = v1+1; v2 < m_vertices; v2++)
					{
					   if (u1 == v1 || u1 == v2 || u2==v1 || u2 == v2)  continue;
						ostr << delimiter << m_cuttings[u1][u2][v1][v2];
					}
				}
			}
		}
	}
#endif
    
#ifdef G_LEFTRIGHT
	void print_leftright(ostream &ostr, const string &delimiter) const
	{
		for (int u1 = 0; u1 < m_vertices; u1++)
		{
			for (int u2 = u1+1; u2 < m_vertices; u2++)
			{
				for (int v1 = 0; v1 < m_vertices; v1++)
                {
                    if (u1 == v1 || u2==v1)  continue;
                    ostr << delimiter << m_leftright[u1][u2][v1];
                }
			}
		}
	}
#endif
/***************************************************************************************************************** Loading ****************/	
	void load_from_string(const char *str)
	{
        if (strcmp(str,"cin") == 0)
        {
            load_from_stream(cin, -1, -1);            
        }
        else
        {
            stringstream s(str);
            load_from_stream(s, -1, -1);
        }
	}
		
	
   bool load_from_stream(istream &stream, int assumed_vertices, int assumed_theta)
   {

        //int poistion = stream.tellg();
        //cerr << "load_from_stream  Rading: " << stream.tellg() <<  "; " << stream.rdbuf() << endl;
        //stream.seekg(poistion);

        int vertices, theta, theta_class=0; //, theta;
        string theta_dot_class; //  (theta loaded as string since it may be  THETA.CLASS)
		stream >> vertices;

		if (stream.fail()) return false;
        
        //stream >> theta_dot_class;
        stream >> theta;

		if (stream.fail()) 
        {
            return false;
        }

        int nextcharacter = stream.peek();
        if (nextcharacter == '.')
        {
#ifndef G_FLAG_PRODUCTS_SLOW_LOW_MEMORY
           cerr << "The program was not compiled with support for repeated flag types.\n";
           cerr << "Use -DG_FLAG_PRODUCTS_SLOW_LOW_MEMORY when compiling the program." << endl;
           assert(0);
#endif
            char ch;
            stream >> ch;
            stream >> theta_class;
        }
        if (nextcharacter == EOF)
        {
            stream.clear();
        }

        if (stream.fail()) 
        {
            cerr << "Fail after . read" << endl;
            return false;
        }

       /*
//       ss << m_Theta << "." <<  m_Theta_class << delimeter << delimeter;
       string only_theta="", only_theta_class="";
       
       std::size_t found_dot = theta_dot_class.find(".");
       if (found_dot != std::string::npos)
       {
           only_theta = theta_dot_class.substr(0,found_dot);
           only_theta_class = theta_dot_class.substr(found_dot+1);
#ifndef G_FLAG_PRODUCTS_SLOW_LOW_MEMORY
           cerr << "The program was not compiled with support for repeated flag types.\n";
           cerr << "Use -DG_FLAG_PRODUCTS_SLOW_LOW_MEMORY when compiling the program." << endl;
           assert(0);
#endif           
       }
       else
       {
           only_theta = theta_dot_class;
           only_theta_class = "";
       }
       theta = stol(only_theta);
       if (only_theta_class.length() > 0)
       {
           theta_class = stol(only_theta_class);
       }
       */

        if (assumed_theta != -1 && theta != assumed_theta)
        {
            cerr << "Err: assumed theta " << assumed_theta << " but loaded " <<  theta << " for " << vertices << " vertices " << endl;
            cerr << "Rest of the stream: " << stream.rdbuf() << endl;
        }      
		assert(assumed_theta == -1 || theta == assumed_theta);
        assert(assumed_vertices == -1 || assumed_vertices == vertices);
		set_vertices_and_Theta(vertices, theta, theta_class);
       
       //cerr << "Set " <<vertices << " " << theta << " " << theta_class << endl;
		
#ifdef G_COLORED_VERTICES		
		for (int i = 0; i < m_vertices; i++)
		{
			int color_v;
			stream >> color_v;
			color_vertex(i,color_v);
		}
#endif		
		
#ifdef G_COLORED_EDGES
		int color;
		for (int u = 0; u < vertices; u++)
		{
			for (int v = u+1; v < vertices; v++)
			{
				stream >> color;
				color_edge(u,v,color);
			}
		}
#endif


#ifdef G_CROSSINGS
//		clear_crossing_table();
		//assert (m_crossings_cnt == 0);

#ifdef G_CROSSINGS_HUMAN_READABLE
    int crossings;
    stream >> crossings;
    for (int i = 0; i< crossings; i++)
    {
        int u1=0, u2=0, v1=0, v2=0, cross_sign=1;
#ifdef G_CROSSINGS_SIGNED        
        stream >> u1 >> u2 >> cross_sign >> v1 >> v2;
#else
        stream >> u1 >> u2 >> v1 >> v2;
#endif        
        set_edges_crossing(u1,u2,v1,v2,cross_sign);
    }
#else
//		int cnt = 0;
		// now crossing of pairs of edges
		for (int u1 = 0; u1 < m_vertices; u1++)
		{
			for (int u2 = u1+1; u2 < m_vertices; u2++)
			{
				for (int v1 = u1+1; v1 < m_vertices; v1++)
				{
					for (int v2 = v1+1; v2 < m_vertices; v2++)
					{
						if (u2 == v1 || u2==v2)  continue;

						//set_edges_crossing(u1,u2,v1,v2,0);
												
						int is_crossing;
						stream >> is_crossing;
						if (is_crossing != 0)
							set_edges_crossing(u1,u2,v1,v2,is_crossing);
					}
				}
			}
		}
#endif		
		//		print_for_human();
		//cerr << m_vertices << " " << count_crossings() << " " << m_crossings_cnt << endl;
//		assert(count_crossings() == m_crossings_cnt);
		m_crossings_cnt = count_crossings();
#endif


#ifdef G_CUTTINGS
	   // now crossing of pairs of edges
	   for (int u1 = 0; u1 < m_vertices; u1++)
	   {
		   for (int u2 = u1+1; u2 < m_vertices; u2++)
		   {
			   for (int v1 = 0; v1 < m_vertices; v1++)
			   {
				   for (int v2 = v1+1; v2 < m_vertices; v2++)
				   {
					   
					   if (u1 == v1 || u1 == v2 || u2==v1 || u2 == v2)  continue;
					   int is_cutting;
					   stream >> is_cutting;
					   set_cutting(u1,u2,v1,v2,is_cutting);
				   }
			   }
		   }
	   }
#endif
       
#ifdef G_LEFTRIGHT
	   // now crossing of pairs of edges
	   for (int u1 = 0; u1 < m_vertices; u1++)
	   {
		   for (int u2 = u1+1; u2 < m_vertices; u2++)
		   {
			   for (int v1 = 0; v1 < m_vertices; v1++)
			   {
					   if (u1 == v1 || u2==v1)  continue;
					   int is_left_or_right;
					   stream >> is_left_or_right;
					   set_leftright(u1,u2,v1,is_left_or_right);
			   }
		   }
	   }
#endif


       
#ifdef G_3EDGES
       int entries_3edge=0;
       stream >> entries_3edge;
       string str_3edge;
       if (entries_3edge >= 0)
       {
           for (int i = 0; i < entries_3edge; i++)
           {
               stream >> str_3edge;
               if (str_3edge.length() != 3)
               {
                   cerr << "unexpected 3-edge '" << str_3edge << "' of length other than 3." << endl;
                   assert(0);
               }
               set_3edge(str_3edge[0]-'1', str_3edge[1]-'1', str_3edge[2]-'1', 1);
           }
       }
       else
       {
           // This allows to specify non-edges instead of edges
           for (int x = 0; x < m_vertices-2; x++)
               for (int y = x+1; y < m_vertices-1; y++)
                   for (int z = y+1; z < m_vertices; z++)
                       {
                           set_3edge(x,y,z,1);
                       }
           for (int i = 0; i < -entries_3edge; i++)
           {
               stream >> str_3edge;
               if (str_3edge.length() != 3)
               {
                   cerr << "unexpected 3-edge '" << str_3edge << "' of length other than 3." << endl;
                   assert(0);
               }
               set_3edge(str_3edge[0]-'1', str_3edge[1]-'1', str_3edge[2]-'1', 0);
           }
       }           
#endif   


   
#ifdef G_ROOTED_3EDGES
       int entries_rooted_3edge=0;
       stream >> entries_rooted_3edge;
       string str_rooted_3edge;
       if (entries_rooted_3edge >= 0)
       {
           for (int i = 0; i < entries_rooted_3edge; i++)
           {
               stream >> str_rooted_3edge;
               if (str_rooted_3edge.length() != 3)
               {
                   cerr << "unexpected 3-edge '" << str_rooted_3edge << "' of length other than 3." << endl;
                   assert(0);
               }
               set_rooted_3edge(str_rooted_3edge[0]-'1', str_rooted_3edge[1]-'1', str_rooted_3edge[2]-'1', 1);
           }
       }
       else
       {
           assert(0);
       }           
#endif   


#ifdef G_COLORED_3EDGES
       // Format for 3 edges: 1) adjacency matrix, 2)  xyz.c
       int entries_colored_3edges;

       stream >> entries_colored_3edges;

       if (entries_colored_3edges == -1)
       {
           int color;
           for (int x = 0; x < m_vertices-2; x++)
               for (int y = x+1; y < m_vertices-1; y++)
                   for (int z = y+1; z < m_vertices; z++)
                   {
                       stream >> color;
                       color_3edge(x,y,z, color);
                   }
       }
       else
       {
           string str_3edge;
           for (int i = 0; i < entries_colored_3edges; i++)
           {
               stream >> str_3edge;
               assert(str_3edge.size() == 5);
               color_3edge(str_3edge[0]-'1', str_3edge[1]-'1', str_3edge[2]-'1', str_3edge[4]-'0');
           }           
       }
#endif       
       
#ifdef G_4EDGES
       // If the number of edges is negative, it means we are loading
       // the complement of the graph
       int entries_4edge;
       stream >> entries_4edge;
       string str_4edge;
       if (entries_4edge >= 0)
       {
           for (int i = 0; i < entries_4edge; i++)
           {
               stream >> str_4edge;
               set_4edge(str_4edge[0]-'1', str_4edge[1]-'1', str_4edge[2]-'1', str_4edge[3]-'1', 1);
           }
       }
       else
       {
           for (int x = 0; x < m_vertices-3; x++)
               for (int y = x+1; y < m_vertices-2; y++)
                   for (int z = y+1; z < m_vertices-1; z++)
                       for (int w = z+1; w < m_vertices; w++)
                       {
                           set_4edge(x,y,z,w,1);
                       }
           for (int i = 0; i < -entries_4edge; i++)
           {
               stream >> str_4edge;
               set_4edge(str_4edge[0]-'1', str_4edge[1]-'1', str_4edge[2]-'1', str_4edge[3]-'1', 0);
           }
       }
#endif

       
#ifdef G_ROOTED_4EDGES
       // If the number of edges is negative, we are in troubles
       int entries_rooted_4edge;
       stream >> entries_rooted_4edge;
       string str_rooted_4edge;
       if (entries_rooted_4edge >= 0)
       {
           for (int i = 0; i < entries_rooted_4edge; i++)
           {
               stream >> str_rooted_4edge;
               set_rooted_4edge(str_rooted_4edge[0]-'1', str_rooted_4edge[1]-'1', str_rooted_4edge[2]-'1', str_rooted_4edge[3]-'1', 1);
           }
       }
       else
       {
           assert(0);
       }
#endif

#ifdef G_MAYBE_ROOTED_KEDGES
       int entries_maybe_rooted_Kedge;
       stream >> entries_maybe_rooted_Kedge;
       if (entries_maybe_rooted_Kedge >= 0)
       {
           for (int i = 0; i < entries_maybe_rooted_Kedge; i++)
           {
                rooted_kedge re;
                stream >> re;

                add_maybe_rooted_kedge(re);
           }
       }
       else
       {
           assert(0);
       }
#endif 

#ifdef G_ROTATION_SYSTEM
       string str_order;
       for (int cnt = 0; cnt < m_vertices; cnt++)
       {
           stream >> str_order;
           int x = str_order[0]-'1';
           m_rotation_system_neighbors[x] = str_order.size()-2;
           if (str_order[1] != ':')
           {
               cerr << "Expected : when reading rotation system out of string '"<<str_order<<"'"
                    << "for vertex "<< cnt <<endl;
               cerr << "Got m_vertices=" << m_vertices << " m_Theta="<<m_Theta << endl;
           }
           assert(str_order[1] == ':');
           assert(m_rotation_system_neighbors[x] >= 0);

           for (int y = 0; y < m_rotation_system_neighbors[x]; y++)
               m_rotation_system[x][y] = str_order[y+2]-'1';
       }
       rotation_systems_count_noncrossings();
#endif

#ifdef G_ORDER_TYPES
           int coordinate_x, coordinate_y;
           for (int cnt = 0; cnt < m_vertices; cnt++)
           {
               stream >> coordinate_x >> coordinate_y;
               m_order_type[cnt].first = coordinate_x;
               m_order_type[cnt].second = coordinate_y;
           }
#endif          

       
       if (!stream) 
       {
           cerr << "Stream failed while loading flag " << print() << endl;
           assert(0);
       }  


    //std::cout << " good()=" << stream.good() << endl;
    //std::cout << " eof()=" << stream.eof() << endl;
    //std::cout << " fail()=" << stream.fail() << endl;
    //std::cout << " bad()=" << stream.bad() << endl;       
       //cerr << print() << endl;
       
       create_minlex_signature();

		return true;
	}
	
    

#ifdef G_CUTTINGS
	void set_cutting(int u1, int u2, int v1, int v2, int cutting)
	{
		assert(u1 != u2 && u1 != v1 && u1 != v2 && u2 != v1 && u2 != v2 && v1 != v2);
		if (m_cuttings[u1][u2][v1][v2] == cutting) return;
		
		m_cuttings[u1][u2][v1][v2] = cutting;
		m_cuttings[u1][u2][v2][v1] = cutting;
		m_cuttings[u2][u1][v1][v2] = cutting;
		m_cuttings[u2][u1][v2][v1] = cutting;
	}
#endif

#ifdef G_LEFTRIGHT
	void set_leftright(int u1, int u2, int v1, int leftright)
	{
		assert(u1 != u2 && u1 != v1 && u2 != v1);
		
		m_leftright[u1][u2][v1] =  leftright;
		m_leftright[u2][u1][v1] =  -leftright;
	}
#endif
	
#ifdef G_CROSSINGS
	void set_edges_crossing(int u1, int u2, int v1, int v2, int crossing)
	{
        assert(u1 <= m_vertices);
        assert(u2 <= m_vertices);
        assert(v1 <= m_vertices);
        assert(v2 <= m_vertices);

		if (m_cross_edges[u1][u2][v1][v2] == crossing) return;
		
		m_cross_edges[u1][u2][v1][v2] = crossing;
#ifdef G_CROSSINGS_SIGNED
		m_cross_edges[u1][u2][v2][v1] = -crossing;
		m_cross_edges[u2][u1][v1][v2] = -crossing;
#else
        m_cross_edges[u1][u2][v2][v1] = crossing;
        m_cross_edges[u2][u1][v1][v2] = crossing;
#endif
		m_cross_edges[u2][u1][v2][v1] = crossing;
		
		m_cross_edges[v1][v2][u1][u2] = crossing;
#ifdef G_CROSSINGS_SIGNED
		m_cross_edges[v1][v2][u2][u1] = -crossing;
		m_cross_edges[v2][v1][u1][u2] = -crossing;
#else
        m_cross_edges[v1][v2][u2][u1] = crossing;
        m_cross_edges[v2][v1][u1][u2] = crossing;
#endif
        m_cross_edges[v2][v1][u2][u1] = crossing;
	
       // if (crossing)
       //                 cerr << "xxxx" << endl;
		//if (m_color_vertex[u1] != m_color_vertex[u2] && m_color_vertex[v1] != m_color_vertex[v2])
		//{
			//if (crossing == 1) m_crossings_cnt++;
			//else m_crossings_cnt--;
		//}
	}
	
	int count_crossings()
	{
		int crossings_cnt = 0;
		m_crossings_in_type = 0;
		
			// now crossing of pairs of edges
			for (int u1 = 0; u1 < m_vertices; u1++)
			{
				for (int u2 = u1+1; u2 < m_vertices; u2++)
				{
					for (int v1 = u1+1; v1 < m_vertices; v1++)
					{
						for (int v2 = v1+1; v2 < m_vertices; v2++)
						{
#ifdef G_COLORED_VERTICES
							// only cross crossings
							if (m_color_vertex[u1] == m_color_vertex[u2] || m_color_vertex[v1] == m_color_vertex[v2]) continue;
#endif
							if (m_cross_edges[u1][u2][v1][v2] != 0)
							{
								crossings_cnt++;
								if (u1 < m_Theta && u2 < m_Theta && v1 < m_Theta && v2 < m_Theta)
								{
									m_crossings_in_type++;
								}
							}
						}
					}
				}
			}
        return crossings_cnt;
	}
#endif

	  

public:
    
    flag( const flag& f)
    {
        *this = f;
    }

    flag& operator=(const flag& f)
    {
        m_vertices = f.m_vertices;
        m_Theta    = f.m_Theta;
        m_Theta_class = f.m_Theta_class;

#ifdef G_USE_INDEXES_FOR_LARGE_UNLABELED_FLAGS
        m_unlabeled_id = f.m_unlabeled_id;
#endif

#ifdef G_ORDERED_VERTICES
        m_labeled_vertices_cnt = f.m_labeled_vertices_cnt;
        for (int i = 0; i < m_vertices; i++)
        {
            m_labeled[i] = f.m_labeled[i];
            m_labeled_vector[i] = f.m_labeled_vector[i]; 
        }
#endif
        
#ifdef G_USE_LEXMIN_FOR_ISOMORPHISM                
        m_minlex_signature = f.m_minlex_signature;
#endif        
        
#ifdef G_COLORED_VERTICES        
        for (int i = 0; i < m_vertices; i++)
        {
            m_color_vertex[i] = f.m_color_vertex[i];
        }
        
        //memcpy(m_color_vertex, f.m_color_vertex, sizeof(int)*m_vertices);
        
        for (int i = 0; i < COLORS_VERTICES; i++)
        {
            m_colored_vertices[i] = f.m_colored_vertices[i];
        }        
#endif
        
#ifdef G_COLORED_EDGES
        for (int i = 0; i < m_vertices; i++)
            for (int j = 0; j < m_vertices; j++)
                {
                    m_color_edge[i][j] = f.m_color_edge[i][j];
                }
        
        for (int i = 0; i < COLORS_EDGES; i++)
        {
            m_colored_edges[i] = f.m_colored_edges[i];
        }         
        
#endif
        
        
#ifdef G_CROSSINGS
        for (int i = 0; i < m_vertices; i++)
            for (int j = 0; j < m_vertices; j++)
                for (int k = 0; k < m_vertices; k++)
                    for (int l = 0; l < m_vertices; l++)
                        m_cross_edges[i][j][k][l] = f.m_cross_edges[i][j][k][l];
        m_crossings_cnt = f.m_crossings_cnt;
        m_crossings_in_type = f.m_crossings_in_type; 
#endif	    
        
#ifdef G_CUTTINGS
        for (int i = 0; i < m_vertices; i++)
            for (int j = 0; j < m_vertices; j++)
                for (int k = 0; k < m_vertices; k++)
                    for (int l = 0; l < m_vertices; l++)
                        m_cuttings[i][j][k][l] = f.m_cuttings[i][j][k][l];
//        TODO!!!
//        int m_cuttings[V][V][V][V]; // m_cuttings[x][y][u][v] == 1 if line x-y intersects segmen uv.
///        assert(0);
#endif
        
#ifdef G_LEFTRIGHT
        for (int i = 0; i < m_vertices; i++)
            for (int j = 0; j < m_vertices; j++)
                for (int k = 0; k < m_vertices; k++)
                    m_leftright[i][j][k] = f. m_leftright[i][j][k];
#endif
        
#ifdef G_3EDGES
        for (int i = 0; i < m_vertices; i++)
            for (int j = 0; j < m_vertices; j++)
                for (int k = 0; k < m_vertices; k++)
                {
                    m_3edge[i][j][k] = f.m_3edge[i][j][k];
                }
        m_3edges_cnt = f.m_3edges_cnt;
#endif
        
#ifdef G_ROOTED_3EDGES
        for (int i = 0; i < m_vertices; i++)
            for (int j = 0; j < m_vertices; j++)
                for (int k = 0; k < m_vertices; k++)
                {
                    m_rooted_3edge[i][j][k] = f.m_rooted_3edge[i][j][k];
                }
        m_rooted_3edges_cnt = f.m_rooted_3edges_cnt;
#endif

#ifdef G_COLORED_3EDGES
        for (int i = 0; i < m_vertices; i++)
            for (int j = 0; j < m_vertices; j++)
                for (int k = 0; k < m_vertices; k++)
        {
            m_colored_3edge[i][j][k] = f.m_colored_3edge[i][j][k];
        }
        
        for (int i = 0; i < COLORS_3EDGES; i++)
        {
            m_colored_3edges[i] = f.m_colored_3edges[i];
        }         
        //memcpy(m_colored_3edges, f.m_colored_3edges, sizeof(int)*COLORS_3EDGES);

#endif        

#ifdef G_4EDGES
        for (int i = 0; i < m_vertices; i++)
            for (int j = 0; j < m_vertices; j++)
                for (int k = 0; k < m_vertices; k++)
                    for (int l = 0; l < m_vertices; l++)
                        m_4edge[i][j][k][l] = f.m_4edge[i][j][k][l];
        m_4edges_cnt = f.m_4edges_cnt;
#endif 

#ifdef G_ROOTED_4EDGES
        for (int i = 0; i < m_vertices; i++)
            for (int j = 0; j < m_vertices; j++)
                for (int k = 0; k < m_vertices; k++)
                    for (int l = 0; l < m_vertices; l++)
                        m_rooted_4edge[i][j][k][l] = f.m_rooted_4edge[i][j][k][l];
        m_rooted_4edges_cnt = f.m_rooted_4edges_cnt;
#endif 


#ifdef G_MAYBE_ROOTED_KEDGES
    m_rooted_kedges = f.m_rooted_kedges;
#endif


#ifdef G_ROTATION_SYSTEM
        for (int i = 0; i < m_vertices; i++)
        {
            m_rotation_system_neighbors[i] = f.m_rotation_system_neighbors[i];
            for (int j = 0; j < m_rotation_system_neighbors[i]; j++)
                m_rotation_system[i][j] = f.m_rotation_system[i][j];
        }
        m_rotation_system_noncrosssings = f.m_rotation_system_noncrosssings;
        for (int i = 0; i < m_vertices; i++)
        {
            m_rotation_system_noncrosssings_v[i] = f.m_rotation_system_noncrosssings_v[i];
            m_rotation_system_noncrosssings_v_sorted[i] = f.m_rotation_system_noncrosssings_v_sorted[i];
        }

#endif
        
#ifdef G_ORDER_TYPES
        for (int i = 0; i < m_vertices; i++)
        {
            m_order_type[i] = f.m_order_type[i];
        }
#endif          
        return *this;
    }
    
		
	int m_vertices;  // number of vertices
    int m_Theta;     // number of labeled vertices in case of not ordered. Binary string which are labeled in ordered
    int m_Theta_class; // Optional thing - same theta can have several classes - this should help with doing fancier rounding.
                       // m_Theta_class == 0 means any class. If not specified, any class is the default

#ifdef G_USE_INDEXES_FOR_LARGE_UNLABELED_FLAGS
    int m_unlabeled_id; // index in g_unlabeled_flags if the graph is unlabeled
#endif

#ifdef G_ORDERED_VERTICES 
    int  m_labeled_vertices_cnt; // number of labeled vertices
    bool m_labeled[V];           // every vertex has true/false if labeled
    int  m_labeled_vector[V];    // ordered list of labeled vertices
#endif
//	int m_id; // id of the original flag

#ifdef G_USE_LEXMIN_FOR_ISOMORPHISM            
// for fast isomorphism testing
	string m_minlex_signature;
#endif
    
	
#ifdef G_COLORED_VERTICES
	int m_color_vertex[V]; // color of a vertex (for bipartite complete flags?)
	int m_colored_vertices[COLORS_VERTICES];
#endif
	
#ifdef G_COLORED_EDGES
	int m_color_edge[V][V];  // adjacency matrix
//	int m_colored_deg[V][COLORS_EDGES]; // number of edges from each vertex of a particular color
	int m_colored_edges[COLORS_EDGES]; // number of edges of each color
//	vector<int> m_deg_sorted;
//	bool m_deg_sorted_valid;
#endif
    
    
#ifdef G_ORIENTED_EDGES
    // Counts indegrees and outdegrees to make the calculation faster
#endif
    
#ifdef G_CROSSINGS
	int m_cross_edges[V][V][V][V];
	int m_crossings_cnt;
	int m_crossings_in_type; // number of crossings that are only part of the type
#endif	    
	
#ifdef G_CUTTINGS
	int m_cuttings[V][V][V][V]; // m_cuttings[x][y][u][v] == 1 if line x-y intersects segmen uv.
#endif

#ifdef G_LEFTRIGHT
	int m_leftright[V][V][V]; // m_cuttings[x][y][u] == 1 or -1 if line x-y if u is on left or right of x-y.
#endif
    
#ifdef G_3EDGES
    int m_3edge[V][V][V]; // m_3edges[x][y][v] == 1 it it is an edge
    int m_3edges_cnt; // number of 3-edges (for faster isomorphism and so on)
#endif

#ifdef G_ROOTED_3EDGES
    int m_rooted_3edge[V][V][V]; // m_3edges[x][y][v] == 1 it it is an edge AND x is the root
    int m_rooted_3edges_cnt; // number of 3-edges (for faster isomorphism and so on)
#endif


#ifdef G_COLORED_3EDGES
    int m_colored_3edge[V][V][V]; // m_3edges[x][y][v] == color
    int m_colored_3edges[COLORS_3EDGES]; // number of 3-edges for each of the colors
#endif    
    
#ifdef G_4EDGES
    int m_4edge[V][V][V][V]; // m_4edges[x][y][v][w] == 1 it it is an edge
    int m_4edges_cnt; // number of 4-edges (for faster isomorphism and so on)
#endif    

#ifdef G_ROOTED_4EDGES
    int m_rooted_4edge[V][V][V][V]; // m_4edges[x][y][v][w] == 1 it it is an edge
    int m_rooted_4edges_cnt; // number of 4-edges (for faster isomorphism and so on)
#endif    

#ifdef G_MAYBE_ROOTED_KEDGES
    set<rooted_kedge> m_rooted_kedges;

    bool contains_rooted_kedge(const rooted_kedge &re) const
    {
        return m_rooted_kedges.find(re) != m_rooted_kedges.end();
    }
#endif

#ifdef G_ROTATION_SYSTEM
    int m_rotation_system[V][V];  // m_rotation_system[x] is a cyclic list of neighbors
    int m_rotation_system_neighbors[V]; // m_rotation_system_neighbors[x] is saying how many neighbors are there for [x]
    int m_rotation_system_noncrosssings;  // counts number of non-crossing K_4s. Should speed-up the isomorphism testing
    int m_rotation_system_noncrosssings_v[V]; // counts number of non-crossing K_4s at every vertex
    int m_rotation_system_noncrosssings_v_sorted[V]; // previous sorted
#endif

#ifdef G_ORDER_TYPES
    pair<int,int> m_order_type[V]; // Coordinates of points
#endif  
};


class flag_and_coefficient
{
public:

    bool operator==(const flag_and_coefficient& fc) const
    {
        if (fc.coefficient != coefficient) return false;
        return g.is_isomorphic_to(fc.g);
    }

	flag   g;
	double coefficient;
};

// Try if the number should be actuallly rounded
double smart_round(double d, double precision)
{
    double dr = round(d);
    if (abs(dr-d) < precision) return dr;
    return d;
}

double g_smart_round_precision = 0.00000001;
double smart_round(double d)
{
    return smart_round(d, g_smart_round_precision);
}



std::ostream& operator<< (std::ostream& stream, const flag_and_coefficient& fc)
{
    if (fc.coefficient == 0) return stream;
    stream.precision(G_PRECISION);
    stream << smart_round(fc.coefficient) << "  " << fc.g.print() << endl;

    return stream;
}




// constraint is of the form...  m_flag + m_constant >= 0
class linear_constraint
{
public:
    linear_constraint()
    {
        m_checked = false; 
        m_labeled_vertices_in_type_cnt = 0;

        m_required_coefficients_sum_for_elcp_constraints = false;
        m_required_coefficients_sum_for_elcp_constraints_value = 0;
    }
    
    
    bool check_constraint()
    {
        if (m_entries.size() == 0) 
        {
            m_checked = false;
            //m_same_types = false;
            cerr << "Error: Constraint has no entries" << endl;
            return false;
        }
        
        m_entries_max_size = m_entries[0].g.m_vertices;
        
        for (int i = 1; i < (int)m_entries.size(); i++)
        {
            if (! m_entries[0].g.have_same_type(m_entries[i].g))
            {
                m_checked = false;
                cerr << "Error: Constraint distinct types for entries" << endl << m_entries[0].g.print() << endl << m_entries[i].g.print() << endl;
                return false;
            }
            if (m_entries_max_size < m_entries[i].g.m_vertices)
            {
                m_entries_max_size = m_entries[i].g.m_vertices;
            }
        }
        
        m_labeled_vertices_in_type_cnt = m_entries[0].g.labeled_vertices_cnt();
        
        m_entries[0].g.get_type_subflag(m_type);
        
        m_checked = true;

//        m_same_types = true;
        
        if (m_constant != 0)
        {
            flag_and_coefficient fc;
            fc.g = m_type;
            fc.coefficient = m_constant;
            m_entries.push_back(fc);
            m_constant = 0;
        }
        
        return true;
    }
    
    string print()
    {
        stringstream ss;
        ss << "0 " << m_constant << endl;
        for (int i = 0; i < (int)m_entries.size(); i++)
        {
            ss.precision(G_PRECISION);
            ss << m_entries[i].coefficient << " " << m_entries[i].g.print() << endl;
        }
        return ss.str();
    }
    
    void add_entry(const flag_and_coefficient& fc)
    {
        for (int i = 0; i < (int)m_entries.size(); i++)
        {
            if (m_entries[i].g.is_isomorphic_to(fc.g))
            {
                m_entries[i].coefficient += fc.coefficient;
                if (m_entries[i].coefficient == 0)
                {
                    m_entries.erase(m_entries.begin()+i);
                }
                return;
            }
        }
        m_entries.push_back(fc);
    }
    
    bool operator==(const linear_constraint &lc) const
    {
        if (lc.m_labeled_vertices_in_type_cnt != m_labeled_vertices_in_type_cnt) return false;
        if (lc.m_entries.size() != m_entries.size()) return false;
        if (lc.m_constant != m_constant) return false;
        if (lc.m_type.is_isomorphic_to(m_type) == false) return false;
        if (lc.m_entries_max_size != m_entries_max_size) return false;

        for (flag_and_coefficient entry : m_entries)
        {
            // Test if it is the same as some other entry in lc
            bool found_match = false;
            for (flag_and_coefficient entry2 : lc.m_entries)
            {
                if (entry == entry2)
                {
                    found_match = true;
                    break;
                }
            }
            if (found_match == false) return false;
        }

        return true;
    }

    bool is_identical_after_type_permutation(const linear_constraint &lc) const
    {
        if (lc == *this) return true;

        /*
        if (lc.m_labeled_vertices_in_type_cnt != m_labeled_vertices_in_type_cnt) return false;
        if (lc.m_entries.size() != m_entries.size()) return false;
        if (lc.m_constant != m_constant) return false;
        if (lc.m_type.is_isomorphic_to(m_type) == false) return false;
        if (lc.m_entries_max_size != m_entries_max_size) return false;
        if (m_entries.size() == 0) return true;
         */
 
        
        int permutation[m_type.m_vertices];
        for (int i = 0; i < m_type.m_vertices; i++) permutation[i] = i;

        // This will be used to permute vertices of flags
        int permutation_fc[V];
        for (int i = 0; i < V; i++) permutation_fc[i] = i;


        flag permuted_type;

        // Skip the identity permutation, already tested at the beginning
        while ( std::next_permutation(permutation,permutation+m_type.m_vertices) )
        {
            permuted_type.as_subflag(m_type, permutation, m_type.m_vertices, m_type.m_Theta);
            if (!m_type.is_isomorphic_to(permuted_type)) continue;

            // Now we got an automorphism of the type
            // so we permute the whole constraint - it is wasting a little but will
            // be easier to read
            linear_constraint lc_tmp;
            
            for (const flag_and_coefficient &fc : m_entries)
            {
                // make permutation of type, the rest is identity
                // from the beginning
                for (int i = 0; i < m_type.m_vertices; i++)
                    permutation_fc[i] = permutation[i];

                flag_and_coefficient new_fc;
                new_fc.coefficient = fc.coefficient;
                new_fc.g.as_subflag(fc.g, permutation_fc, fc.g.m_vertices, fc.g.m_Theta);

                lc_tmp.add_entry(new_fc);
            }

            assert(lc_tmp.check_constraint());

            if (lc == lc_tmp)
                return true;

        }

        return false;
    }

public:
	vector<flag_and_coefficient> m_entries;
	double m_constant;
    
    bool   m_checked;
    //bool   m_same_types;  // true if the entries are of same type and size 
    int    m_entries_max_size;
    int    m_labeled_vertices_in_type_cnt;

    // This is a special bonus thing
    bool    m_required_coefficients_sum_for_elcp_constraints;
    double  m_required_coefficients_sum_for_elcp_constraints_value;

    flag   m_type;
};

std::ostream& operator<< (std::ostream& stream, const linear_constraint& lc)
{
    stream << "0 ";

    // Constant is made into a labeled flag, so this tries to recover it
    // if possible. Maybe there should be a switch for this?
    bool constant_found = false;
    if (lc.m_constant == 0)
    {
        for (const auto & fc :  lc.m_entries)
        {
            if (fc.g.m_vertices == fc.g.m_Theta)
            {
                assert(constant_found == false);
                stream.precision(G_PRECISION);
                stream << smart_round(fc.coefficient) << endl;
                constant_found = true;
            }
        }
    }
    if (!constant_found)
    {
        stream.precision(G_PRECISION);
        stream << smart_round(lc.m_constant) << endl;
    }

    
    for (const auto & fc :  lc.m_entries)
    {
        if (fc.g.m_vertices != fc.g.m_Theta)
        {
            stream << fc;
        }
    }
    return stream;
}




vector<flag> g_unlabeled_flags[V+1];
vector<flag> g_types[V+1]; // types - it is just same as g_unlabeled_flags, but all vertices are labeled :-) Used for reduced types
vector<vector<flag> >  g_flags; // flags to process using multiplication - every type&size is a separate list
vector<vector<flag> >  g_flag_square_linear_constraints; // flags used only for linear constraints



vector<linear_constraint> g_linear_constraints;
vector<flag_and_coefficient> g_objective_combination;
vector<flag_and_coefficient> g_objective_divisor; // may be nothing...
vector<flag> g_forbidden_subflags;
vector<flag> g_forbidden_subflags_by_size[V+1];
vector<flag> g_no_slack_flags; // means no slack in CSDP
string       g_program_description;  // loaded from the objective function
vector<string> g_additional_csdp_blocks;

#ifdef G_COLORED_VERTICES
vector<int> g_vertex_color_pattern;
#endif

#ifdef G_USE_PERMITTED_SUBFLAGS
vector<flag>  g_permitted_subflags[V+1];
#endif


// When using the program for crossing number, it may happen that there is a fixed
// number of vertices in each color (for the -n ? we try to compute). Then it makes
// no sense to use some flags that will 'clearly' never be used because their sqaure
// is never used. This helps avoid such situations.
#ifdef G_COLORED_VERTICES
int g_exact_number_of_colored_vertices[COLORS_VERTICES];
#endif


string time_to_str(time_t time_taken)
{

    int days_t  = time_taken/60/60/24;
    int hours_t = (time_taken%(60*60*24))/60/60;
    int min_t = (time_taken%(60*60))/60;
    int sec_t = time_taken%(60);

    stringstream ss;
    ss << days_t << "d+"
         << hours_t << "h+" 
         << min_t << "m+" 
         << sec_t << "s";

    return ss.str();
}

class mini_timer
{
public:
    mini_timer()
    {
        m_time_start = 0;
    }

    void start()
    {
        m_time_start = time(NULL);
    }

    string report(int processing_id, int total_ids)
    {
        if (m_time_start == 0)
        {
            m_time_start = time(NULL);
            return "";
        }

        time_t time_taken = time(NULL) - m_time_start;

        #ifdef _USING_OMP_
            int adjust = omp_get_num_threads();
        #else
            int adjust = 1;
        #endif

        stringstream ss;
        if (time_taken > 0 && processing_id > adjust)
        {
            time_t total_time  = (time_taken*total_ids)/(processing_id-adjust);
            ss << " took " << time_to_str(time_taken);
            ss << " will take " << time_to_str(total_time-time_taken);
            return ss.str();
        }

        return "";
    } 
    
private:
    time_t m_time_start;
};


int binomial(int n, int k) {
    int b = 1;
    for (int i = 0; i < k; ++i) {
        b *= (n - i);
        b /= (i + 1);
    }
    return b;
}

int factorial(int n) {
    int b = 1;
    for (int i = 2; i <= n; i++) {
        b *= i;
    }
    return b;
}



string filename_prefix()
{
    stringstream filename;
    
    filename << "F"
#ifdef G_COLORED_VERTICES
    << "_vertices" << COLORS_VERTICES-1
#ifdef G_ORDERED_VERTICES
    << "ordered"
#endif
#else
#ifdef G_ORDERED_VERTICES
    << "_ordered"
#endif
#endif
#ifdef G_COLORED_VERTICES_BLIND
    << "blind"
#endif
#ifdef G_COLORED_EDGES
    << "_edges" << COLORS_EDGES-1
#endif
#ifdef G_COLORED_EDGES_BLIND
    << "blind"
#endif
#ifdef G_ORIENTED_EDGES
    << "oriented"
#endif
#ifdef G_ORIENTED_EDGES_BLIND
    << "blind"
#endif
#ifdef G_3EDGES
    << "_3edges"
#endif
#ifdef G_COLORED_3EDGES
    << "_3edges" << COLORS_3EDGES-1
#endif
#ifdef G_COLORED_3EDGES_BLIND
    << "blind"
#endif    
#ifdef G_ROOTED_3EDGES
  << "_rooted3edges"
#endif
#ifdef G_4EDGES
    << "_4edges"
#endif 
#ifdef G_ROOTED_4EDGES
    << "_rooted4edges"
#endif 
#ifdef G_MAYBE_ROOTED_KEDGES
  << "_mayberooted" << G_MAYBE_ROOTED_KEDGES << "edges"
#endif
#ifdef G_CROSSINGS
    << "_crossings"
#endif
#ifdef G_CROSSINGS_SIGNED
    << "signed"
#endif
#ifdef G_CUTTINGS
    << "_cuttings"
#endif
#ifdef G_LEFTRIGHT
    << "_leftright"
#endif	
#ifdef G_LEFTRIGHT_BLIND
    << "blind"
#endif
#ifdef G_LEFTRIGHT_LOCAL
    << "local"
#endif    
#ifdef G_ROTATION_SYSTEM
    << "_rotationsystem"
#endif
#ifdef G_ROTATION_SYSTEM_REVERSE_BLIND
    << "blind"
#endif
#ifdef G_ORDER_TYPES
    << "_ordertypes"
#endif
    ;
    return filename.str();
}



inline int find_flag_in_list_nonparalel(const flag &f, const vector<flag> &fv)
{
    for (int i = 0; i < (int)fv.size(); ++i)
    {
        if (f.is_isomorphic_to(fv[i]))
        {
            return i;
        }
    }
    return -1;
}


inline int find_flag_in_list(const flag &f, const vector<flag> &fv)
{
    // Not worth paralelizing the search.
    if (fv.size() < 500)
    {
        return find_flag_in_list_nonparalel(f,fv);
    }

    // attempt to paralelize the search if it was not parallel yet
#ifdef _USING_OMP_
    if (omp_in_parallel() == false)
    {        
        //cerr << "Trying in parallel" << endl;
        volatile int location = -1;
        
        #pragma omp parallel for shared(location)    
        for (int i = 0; i < (int)fv.size(); ++i)
        {
            if (location != -1) continue;
            if (f.is_isomorphic_to(fv[i]))
            {
                location = i;
            }
        }
        return location;
    }
#endif

    return find_flag_in_list_nonparalel(f,fv);
}

inline int get_flag_type_in_list(const flag& f, vector< vector<flag> > &flags)
{
#ifdef USE_REDUCED_TYPES
    const int identity[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
    flag f_type;
    f_type.as_subflag(f,identity,f.m_Theta,0);
#endif 
    
    // Find correct type
    for (int i = 0; i < (int)flags.size(); i++)
    {
        if (flags[i].size() == 0)
        {
            cerr << "Some bug" << endl;
        }
        if (f.have_same_type(flags[i][0])) return i;
#ifdef USE_REDUCED_TYPES
        if (f.m_Theta == flags[i][0].m_Theta && f.m_Theta_class == flags[i][0].m_Theta_class)
        {
            flag g_flags_type;
            g_flags_type.as_subflag(flags[i][0],identity,flags[i][0].m_Theta,0);
            if (g_flags_type.is_isomorphic_to(f_type))
                return -2;
        }
#endif        
        
    }
    
    return -1;
}

inline void include_flag_in_list(const flag& f, vector< vector<flag> > &flags, int id = -3, bool ignore_reduced_types = true)
{
    if (id == -3)
    {
        id = get_flag_type_in_list(f, flags);
    }
#ifdef  USE_REDUCED_TYPES   
    if (id == -2 && ignore_reduced_types) return;
#endif   
    if (id == -1 || id == -2)
    {
        vector<flag> new_type_list;
        new_type_list.push_back(f);
        flags.push_back(new_type_list);
        return;
    }
    
    flags[id].push_back(f);
}


inline void include_flag_in_list_if_new(const flag& f, vector< vector<flag> > &flags, bool ignore_reduced_types = true)
{
    int id = get_flag_type_in_list(f, flags);
#ifdef USE_REDUCED_TYPES
    if (id == -2 && ignore_reduced_types) 
    {
        return;
    }
#endif
    if (id == -1 || id == -2)
    {
//        cerr << "new type" << endl;
        include_flag_in_list(f, flags, id,ignore_reduced_types);
        return;
    }
    
    if (find_flag_in_list(f,flags[id]) != -1)
    {
//        cerr << "duplicate" << endl;
        return;   
    }
//    cerr << "new for known type" << f.print() << endl;
    flags[id].push_back(f);
}

#ifndef HACK_ALL_COLOR_FORBIDDEN
//#define HACK_ALL_COLOR_FORBIDDEN
#endif

#ifdef HACK_ALL_COLOR_FORBIDDEN
bool forbidden_rainbow_pattern_found(const flag &g, int *colors_picked, int colors_valid, int *edges_used, int*edges_u, int*edges_v, int next_to_find)
{
    if (next_to_find >= colors_valid)
    {
        return true;
    }

    for (int e = 0; e < g.m_vertices*(g.m_vertices-1)/2; e++)
    {
        if (edges_used[e]) continue;
        if ((((g.m_color_edge[edges_u[e]][edges_v[e]]-1)>> colors_picked[next_to_find])& 1) != 1) continue;
        
        edges_used[e] = 1;
        if (forbidden_rainbow_pattern_found(g, colors_picked, colors_valid, edges_used, edges_u, edges_v, next_to_find+1))
            return true;

        edges_used[e] = 0;
    }
    return false;
}
#endif


inline bool is_flag_forbidden_noninduced(const flag &g)
{
    for (int i = 0; i < (int)g_forbidden_subflags.size(); i++)
    {
        if (g.has_as_notinduced_subflag(g_forbidden_subflags[i])) return true;
    }
    return false;
}


inline bool is_flag_forbidden(const flag &g, int verbose_output=0)
{
    //cerr << "Testing forbidden " << g.print() << endl;
    //verbose_output = 1;

#ifdef G_FORBIDDEN_NON_INDUCED
    return is_flag_forbidden_noninduced(g);
#endif

#ifdef HACK_ALL_COLOR_FORBIDDEN
    if (g.m_vertices < 4)
    {
        return false;
    }

    // For each color we count how many times it is used on G
    // Also check that the graph is a complete graph
    int color_counts[HACK_ALL_COLOR_FORBIDDEN_COLORS];
    for (int c = 0; c < HACK_ALL_COLOR_FORBIDDEN_COLORS; c++)
    {
        color_counts[c] = 0;
    }

    for (int u = 0; u < g.m_vertices; u++)
        for (int v = u+1; v < g.m_vertices; v++)
        {
            int label = (g.m_color_edge[u][v]) - 1;
            if (label == 0) {
                return false;
            }
            for (int c = 0; c < HACK_ALL_COLOR_FORBIDDEN_COLORS; c++)
            {
                color_counts[c] += (label >> c)&1;
            }
        }

    int colors_used = 0;
    for (int c = 0; c < HACK_ALL_COLOR_FORBIDDEN_COLORS; c++)
    {
        if (color_counts[c] > 0)  colors_used++;
    }

    if (colors_used  != HACK_ALL_COLOR_FORBIDDEN_COLORS)
    {
        return false;
    }

    // Now we need to check if we can find an actual copy that has all colors. Not just a fake
    // for example by a random edge.
    // We pick HACK_ALL_COLOR_FORBIDDEN_COLORS edges and see if they are in the correct color
    if (g.m_vertices >= 4)
    {
        /*
        int edge_vertices[][2] = { [0,1], [0,2], [0,3], [1,2], [1,3], [2,3] };
        for (e1 = 0; e1 < 6; e1++)
        {
            if (  ((g.m_color_edge[c1_u][c1_v]-1)>>0)& 1 != 1) continue;

        }
        */

       
        int colors_picked[]={0,1,2,3,4,5,6,7,8,9,10,11};
        int colors_valid = HACK_ALL_COLOR_FORBIDDEN_COLORS;
        ///int colors_picked[]={0,0,1,1,2,2};
        //int colors_picked[]={0,0,0,1,1,2};
        //int colors_picked[]={0,0,0,0,1,2};
        //int colors_valid = 6;
        assert(HACK_ALL_COLOR_FORBIDDEN_COLORS < 11);

        int edges_used[] = {0,0,0,0,0,0,0,0,0,0};
        int edges_u[V*V];
        int edges_v[V*V];
        int e = 0;
        for (int u = 0; u < g.m_vertices; u++)
            for (int v = u+1; v < g.m_vertices; v++)
            {
                edges_u[e] = u;
                edges_v[e] = v;
                e++;
            }


        if (forbidden_rainbow_pattern_found(g, colors_picked, colors_valid, edges_used, edges_u, edges_v, 0))
        {
            return true;
        }

        return false;
        /*

        for (int c1_u = 0; c1_u < g.m_vertices; c1_u++)
            for (int c1_v = c1_u+1; c1_v < g.m_vertices; c1_v++)
            {
                if (  (((g.m_color_edge[c1_u][c1_v]-1)>>0)& 1) != 1) continue;

                for (int c2_u = 0; c2_u < g.m_vertices; c2_u++)
                    for (int c2_v = c2_u+1; c2_v < g.m_vertices; c2_v++)
                    {
                        if (c1_u == c2_u && c1_v == c2_v) continue;
                        if (  (((g.m_color_edge[c2_u][c2_v]-1)>>1)& 1) != 1) continue;

                        for (int c3_u = 0; c3_u < g.m_vertices; c3_u++)
                            for (int c3_v = c3_u+1; c3_v < g.m_vertices; c3_v++)
                            {
                                if (c1_u == c3_u && c1_v == c3_v) continue;
                                if (c2_u == c3_u && c2_v == c3_v) continue;
                                if (  (((g.m_color_edge[c3_u][c3_v]-1)>>2)& 1) != 1) continue;
                                for (int c4_u = 0; c4_u < g.m_vertices; c4_u++)
                                    for (int c4_v = c4_u+1; c4_v < g.m_vertices; c4_v++)
                                    {
                                        if (c1_u == c4_u && c1_v == c4_v) continue;
                                        if (c2_u == c4_u && c2_v == c4_v) continue;
                                        if (c3_u == c4_u && c3_v == c4_v) continue;
                                        if (  (((g.m_color_edge[c4_u][c4_v]-1)>>3)& 1) != 1) continue;

                                        if (HACK_ALL_COLOR_FORBIDDEN_COLORS == 4)
                                        {
                                            return true;
                                        }
                                        else
                                        {
                                            for (int c5_u = 0; c5_u < g.m_vertices; c5_u++)
                                                for (int c5_v = c5_u+1; c5_v < g.m_vertices; c5_v++)
                                                {
                                                    if (c1_u == c5_u && c1_v == c5_v) continue;
                                                    if (c2_u == c5_u && c2_v == c5_v) continue;
                                                    if (c3_u == c5_u && c3_v == c5_v) continue;
                                                    if (c4_u == c5_u && c4_v == c5_v) continue;
                                                    if (  (((g.m_color_edge[c5_u][c5_v]-1)>>4)& 1) != 1) continue;

                                                    if (HACK_ALL_COLOR_FORBIDDEN_COLORS == 5)
                                                    {
                                                        return true;
                                                    }
                                                    else
                                                    {
                                                        for (int c6_u = 0; c6_u < g.m_vertices; c6_u++)
                                                            for (int c6_v = c6_u+1; c6_v < g.m_vertices; c6_v++)
                                                            {
                                                                if (c1_u == c6_u && c1_v == c6_v) continue;
                                                                if (c2_u == c6_u && c2_v == c6_v) continue;
                                                                if (c3_u == c6_u && c3_v == c6_v) continue;
                                                                if (c4_u == c6_u && c4_v == c6_v) continue;
                                                                if (c5_u == c6_u && c5_v == c6_v) continue;
                                                                if (  (((g.m_color_edge[c6_u][c6_v]-1)>>5)& 1) != 1) continue;

                                                                return true;
                                                            }
                                                    }
                                                }
                                        }
                                    }
                            }
                    }
            }
        */
    }

    return false;
#endif


    for (int i = 0; i < (int)g_forbidden_subflags.size(); i++)
    {
        //cerr << "Testing " << g.print() << " and " << g_forbidden_subflags[i].print() << endl;
        if (g.contains_as_subflag(g_forbidden_subflags[i])) 
        {
            if (verbose_output > 0)
            {
                cerr << "Forbidding " << g.print() << " because of " << g_forbidden_subflags[i].print() << endl;
            }
            return true;
        }
    }
#ifdef G_USE_PERMITTED_SUBFLAGS
    vector<flag> subflags_list;
    for (int i = 1; i <= g.m_vertices; i++)
    {
        if (g_permitted_subflags[i].size() == 0) continue;
        
        subflags_list.clear();
        g.generate_subflags_of_size_n(i, subflags_list);
        
        //if (subflags_list.size() > 1)
        // cerr << "Number of subflags is " << subflags_list.size() << endl;
        
        // Certainly there is one not permitted...
        if (subflags_list.size() > g_permitted_subflags[i].size()) return true;
        
        for (int j = 0; j < (int)subflags_list.size(); j++)
        {
            if (find_flag_in_list(subflags_list[j], g_permitted_subflags[i]) == -1)
                return true;
        }
    }
#endif    
    return false;
}

//#ifdef G_USE_PERMITTED_SUBFLAGS  not used in early generation - maybe should be... ?
inline bool is_flag_forbidden_avoid_zeros(const flag &g, const vector<int> &in_subrgaph = vector<int>())
{
    vector<flag> subflags_list;
    for (int i = 1; i <= g.m_vertices; i++)
    {
        //if (g_permitted_subflags[i].size() == 0) continue;        
        if (g_forbidden_subflags_by_size[i].size() == 0) continue;
        
        subflags_list.clear();
        g.generate_subflags_of_size_n(i, subflags_list, in_subrgaph);
                
        for (int j = 0; j < (int)subflags_list.size(); j++)
        {
// Do not check subgraphs that are not fully colored
#ifdef G_COLORED_VERTICES
            if (subflags_list[j].m_colored_vertices[0] > 0) continue;
#endif
#ifdef G_COLORED_EDGES            
            if (subflags_list[j].m_colored_edges[0] > 0) continue;
#endif            
#ifdef G_COLORED_3EDGES            
            if (subflags_list[j].m_colored_3edges[0] > 0) continue;
#endif            
            if (find_flag_in_list(subflags_list[j], g_forbidden_subflags_by_size[i]) != -1) return true;
        }
    }
    return false;
}


inline bool g_already_in_known_flags(flag &g, vector<flag> &flag_list)
{
    for (unsigned int i = 0; i < flag_list.size(); i++)
    {
        if (flag_list[i].is_isomorphic_to(g))
        {
            return true;
        }
    }
    return false;
}

inline void add_g_to_known_flags(flag &g, vector<flag> &flag_list)
{
    flag_list.push_back(g);
    //if (g.m_3edges_cnt >= 20)
    {
    //    cerr << g.print() << endl;
    }
}

inline void add_g_to_flags_list_if_new(flag &g, vector<flag> &flag_list)
{
    if (g_already_in_known_flags(g, flag_list)) return;
    add_g_to_known_flags(g, flag_list);
}


#ifdef G_COLORED_EDGES
void try_color_edge(flag &g, int u, int v, vector<flag> &flag_list)
{
    if (v >= g.m_vertices || v == u)
    {
        u++;
        v = u+1;
#ifdef G_EARLY_FORBIDDEN_TEST_IN_GENERATING
        // test if graph on vertices 0...u is not forbidden
        const int identity[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        flag g_to_u;
        g_to_u.as_subflag(g,identity, u+1, 0);
        
        cerr << "Testing " << g_to_u.print() << endl;
        
        if (is_flag_forbidden(g_to_u)) return;
#endif
    }
    if (v >= g.m_vertices)
    {
        if (is_flag_forbidden(g)) return;
        
        g.create_minlex_signature();
        
        if (!g_already_in_known_flags(g,flag_list)) add_g_to_known_flags(g,flag_list);
        //		add_g_to_known_flags(flagh_list);
        return;
    }
    
    
    // Color only uncolored edges.....
    if (g.m_color_edge[u][v] == 0)
    {
// Either color 1 us used for blow-up or for tournaments it is not used at all
// since color 1 does not change orientation
#if defined(G_USE_FIRST_EDGE_COLOR_FOR_BLOWUP_ONLY) || defined(G_TOURNAMENT)         
        for (int color = 2; color < COLORS_EDGES; color++)
#else
        for (int color = 1; color < COLORS_EDGES; color++)
#endif
        {
            g.color_edge(u,v,color);
            try_color_edge(g,u,v+1,flag_list);
#ifdef G_ORIENTED_EDGES
            if (color > G_ORIENTED_EDGES_UNORIENTED_COLORS)
            {
                g.color_edge(v,u,color);
                try_color_edge(g,u,v+1,flag_list);
            }
#endif            
        }
        g.color_edge(u,v,0);
    }
    else
    {
        try_color_edge(g,u,v+1,flag_list);
    }
}
#endif


#ifdef G_3EDGES 
void try_add_3edges(flag &g, int x, int y, int z, vector<flag> &flag_list, bool extending_only_last_vertex)
{
    if (z >= g.m_vertices || z == x)
    {
        y++;
        if (y >= g.m_vertices-1)
        {
            x++;
            if (x >= g.m_vertices-2)
            {
#ifdef G_COLORED_EDGES
                if (extending_only_last_vertex)
                {
                    int v = g.m_vertices;
                    try_color_edge(g,v-1,0,flag_list);
                }
                else 
                {
                    try_color_edge(g,0,1,flag_list);                    
                }
                return;
#endif                 
                if (is_flag_forbidden(g)) return; 

                g.create_minlex_signature();
                
                if (!g_already_in_known_flags(g,flag_list)) 
                {
                    //cerr << g.print() << endl;
                    add_g_to_known_flags(g,flag_list);
                }
                return;
            }
            y = x+1;
        }
        z = y+1;
        try_add_3edges(g,x,y,z,flag_list,extending_only_last_vertex);
        return;
    }
    
    if (g.m_3edge[x][y][z] != 0) try_add_3edges(g,x,y,z+1,flag_list,extending_only_last_vertex);
    else
    {
        g.set_3edge(x,y,z,1);
        try_add_3edges(g,x,y,z+1,flag_list,extending_only_last_vertex);
        g.set_3edge(x,y,z,0);
        try_add_3edges(g,x,y,z+1,flag_list,extending_only_last_vertex);
    }
}
#endif



#ifdef G_ROOTED_3EDGES 
void try_add_rooted_3edges(flag &g, int x, int y, int z, vector<flag> &flag_list, bool extending_only_last_vertex)
{
    if (z >= g.m_vertices || z == x)
    {
        y++;
        if (y >= g.m_vertices-1)
        {
            x++;
            if (x >= g.m_vertices-2)
            {
#ifdef G_3EDGES 
                if (extending_only_last_vertex)
                {
                    try_add_3edges(g,g.m_vertices-1,0,1,flag_list, extending_only_last_vertex);
                }
                else
                {
                    try_add_3edges(g,0,1,2,flag_list, extending_only_last_vertex);                        
                }
                return;

#elif defined(G_COLORED_EDGES)
                if (extending_only_last_vertex)
                {
                    int v = g.m_vertices;
                    try_color_edge(g,v-1,0,flag_list);
                }
                else 
                {
                    try_color_edge(g,0,1,flag_list);                    
                }
                return;
#endif                 
                if (is_flag_forbidden(g)) return; 

                g.create_minlex_signature();
                
                if (!g_already_in_known_flags(g,flag_list)) add_g_to_known_flags(g,flag_list);
                return;
            }
            y = x+1;
        }
        z = y+1;
        try_add_rooted_3edges(g,x,y,z,flag_list,extending_only_last_vertex);
        return;
    }
    
    if (g.m_rooted_3edge[x][y][z] != 0) try_add_rooted_3edges(g,x,y,z+1,flag_list,extending_only_last_vertex);
    else
    {
        // Try all three possible roots
        g.set_rooted_3edge(x,y,z,1);
        try_add_rooted_3edges(g,x,y,z+1,flag_list,extending_only_last_vertex);
        g.set_rooted_3edge(x,y,z,0);
        g.set_rooted_3edge(y,x,z,1);
        try_add_rooted_3edges(g,x,y,z+1,flag_list,extending_only_last_vertex);
        g.set_rooted_3edge(y,x,z,0);
        g.set_rooted_3edge(z,x,y,1);
        try_add_rooted_3edges(g,x,y,z+1,flag_list,extending_only_last_vertex);
        g.set_rooted_3edge(z,x,y,0);
        try_add_rooted_3edges(g,x,y,z+1,flag_list,extending_only_last_vertex);
    }
}
#endif





#ifdef G_COLORED_3EDGES 
void try_color_3edges(flag &g, int x, int y, int z, vector<flag> &flag_list, bool extending_only_last_vertex)
{
    if (z >= g.m_vertices || z == x)
    {
        y++;
        if (y >= g.m_vertices-1)
        {
            x++;
            if (x >= g.m_vertices-2)
            {
#ifdef G_COLORED_EDGES
                if (extending_only_last_vertex)
                {
                    int v = g.m_vertices;
                    try_color_edge(g,v-1,0,flag_list);
                }
                else 
                {
                    try_color_edge(g,0,1,flag_list);                    
                }
                return;
#endif 
                //#ifndef G_EARLY_FORBIDDEN_TEST_IN_GENERATING
                if (is_flag_forbidden(g)) return; 
                //#endif
                if (!g_already_in_known_flags(g, flag_list)) 
                {
                    add_g_to_known_flags(g,flag_list);
                }
                //cerr << "FOUND " << endl << g.print() << endl;
                return;
            }
            y = x+1;
        }
        z = y+1;
        try_color_3edges(g,x,y,z,flag_list, extending_only_last_vertex);
        return;
    }
    
    if (g.m_colored_3edge[x][y][z] != 0) try_color_3edges(g,x,y,z+1,flag_list, extending_only_last_vertex);
    else
    {
#ifdef G_USE_FIRST_EDGE_COLOR_FOR_BLOWUP_ONLY
        for (int color = 2; color < COLORS_3EDGES; color++)
#else
        for (int color = 1; color < COLORS_3EDGES; color++)
#endif            
        {
            g.color_3edge(x,y,z,color);
#ifdef G_EARLY_FORBIDDEN_TEST_IN_GENERATING
            const vector<int> edge_xyz = {x,y,z};
            if (is_flag_forbidden_avoid_zeros(g,edge_xyz)) continue;
#endif
            try_color_3edges(g,x,y,z+1,flag_list, extending_only_last_vertex);
        }
        g.color_3edge(x,y,z,0);
    }
}
#endif

#ifdef G_4EDGES
void try_add_4edges(flag &g, int x, int y, int z, int w, vector<flag> &flag_list, bool extending_only_last_vertex)
{
    if (w >= g.m_vertices || w == x)
    {
        z++;
        if (z >= g.m_vertices-1)
        {
            y++;
            if (y >= g.m_vertices-2)
            {
                x++;
                if (x >= g.m_vertices-3)
                {
#ifdef G_COLORED_3EDGES
                    if (extending_only_last_vertex)
                    {
                        try_color_3edges(g,g.m_vertices-1,0,1,flag_list, extending_only_last_vertex);
                    }
                    else
                    {
                        try_color_3edges(g,0,1,2,flag_list, extending_only_last_vertex);                        
                    }
                    return;
#endif
#ifdef G_3EDGES
                    if (extending_only_last_vertex)
                    {
                        try_add_3edges(g,g.m_vertices-1,0,1,flag_list, extending_only_last_vertex);
                    }
                    else
                    {
                        try_add_3edges(g,0,1,2,flag_list, extending_only_last_vertex);                        
                    }
                    return;
#endif                     
                    if (is_flag_forbidden(g)) return;                
                    g.create_minlex_signature();
                    if (!g_already_in_known_flags(g,flag_list)) add_g_to_known_flags(g,flag_list);
                    return;
                }
                y = x+1;
            }
            z = y+1;
        }
        w = z+1;
        try_add_4edges(g,x,y,z,w,flag_list, extending_only_last_vertex);
        return;
    }
    
    if (g.m_4edge[x][y][z][w] != 0) try_add_4edges(g,x,y,z,w+1,flag_list, extending_only_last_vertex);
    else
    {
        g.set_4edge(x,y,z,w,1);
        try_add_4edges(g,x,y,z,w+1,flag_list, extending_only_last_vertex);
        g.set_4edge(x,y,z,w,0);
        try_add_4edges(g,x,y,z,w+1,flag_list, extending_only_last_vertex);
    }
}
#endif


#ifdef G_ROOTED_4EDGES
void try_add_rooted_4edges(flag &g, int x, int y, int z, int w, vector<flag> &flag_list, bool extending_only_last_vertex)
{
    if (w >= g.m_vertices || w == x)
    {
        z++;
        if (z >= g.m_vertices-1)
        {
            y++;
            if (y >= g.m_vertices-2)
            {
                x++;
                if (x >= g.m_vertices-3)
                {
#ifdef G_4EDGES
                    if (extending_only_last_vertex)
                    {
                        try_add_4edges(g,g.m_vertices-1,0,1,2,flag_list, extending_only_last_vertex);
                    }
                    else
                    {
                        try_add_4edges(g,0,1,2,3,flag_list, extending_only_last_vertex);                        
                    }
                    return;
#endif                         
#ifdef G_COLORED_3EDGES
                    if (extending_only_last_vertex)
                    {
                        try_color_3edges(g,g.m_vertices-1,0,1,flag_list, extending_only_last_vertex);
                    }
                    else
                    {
                        try_color_3edges(g,0,1,2,flag_list, extending_only_last_vertex);                        
                    }
                    return;
#endif
#ifdef G_3EDGES
                    if (extending_only_last_vertex)
                    {
                        try_add_3edges(g,g.m_vertices-1,0,1,flag_list, extending_only_last_vertex);
                    }
                    else
                    {
                        try_add_3edges(g,0,1,2,flag_list, extending_only_last_vertex);                        
                    }
                    return;
#endif                                    
                    if (is_flag_forbidden(g)) return;                
                    g.create_minlex_signature();
                    if (!g_already_in_known_flags(g,flag_list)) add_g_to_known_flags(g,flag_list);
                    return;
                }
                y = x+1;
            }
            z = y+1;
        }
        w = z+1;
        try_add_rooted_4edges(g,x,y,z,w,flag_list, extending_only_last_vertex);
        return;
    }
    
    if (g.m_rooted_4edge[x][y][z][w] != 0) try_add_rooted_4edges(g,x,y,z,w+1,flag_list, extending_only_last_vertex);
    else
    {
        g.set_rooted_4edge(x,y,z,w,1);
        try_add_rooted_4edges(g,x,y,z,w+1,flag_list, extending_only_last_vertex);
        g.set_rooted_4edge(x,y,z,w,0);

        g.set_rooted_4edge(y,x,z,w,1);
        try_add_rooted_4edges(g,x,y,z,w+1,flag_list, extending_only_last_vertex);
        g.set_rooted_4edge(y,x,z,w,0);

        g.set_rooted_4edge(z,y,x,w,1);
        try_add_rooted_4edges(g,x,y,z,w+1,flag_list, extending_only_last_vertex);
        g.set_rooted_4edge(z,y,x,w,0);

        g.set_rooted_4edge(w,y,z,x,1);
        try_add_rooted_4edges(g,x,y,z,w+1,flag_list, extending_only_last_vertex);
        g.set_rooted_4edge(w,y,z,x,0);

        try_add_rooted_4edges(g,x,y,z,w+1,flag_list, extending_only_last_vertex);
    }
}
#endif



#ifdef G_MAYBE_ROOTED_KEDGES
void try_add_maybe_rooted_kedges(flag &g, vector<int> current_edge, vector<flag> &flag_list, bool extending_only_last_vertex)
{
    //cerr << "edge=" << current_edge[0] << current_edge[1] << current_edge[2] << current_edge[3]   << endl;
    //cerr << "flagsize = " << flag_list.size()<< endl;

    assert(current_edge.size() >= 1);

    int k = G_MAYBE_ROOTED_KEDGES;
    int last_in_edge =  current_edge[k-1];

    //  
    //
    //
    //
    if (current_edge[0] >= g.m_vertices || current_edge[0] == last_in_edge)
    {
        //cerr << "Last tested" << endl;
        int test_to_increase;
        for (test_to_increase = 0; test_to_increase < k-1; test_to_increase++)
        {
            if (current_edge[test_to_increase] >=  g.m_vertices-test_to_increase || current_edge[test_to_increase] == last_in_edge)
            {
                //cerr << "increasing " << test_to_increase+1 << endl;
                current_edge[test_to_increase+1]++;
            }
            else
            {
                break;
            }
        }
        assert(test_to_increase <= k-1);
        if (test_to_increase == k-1)
        {
            if (current_edge[k-1] >= g.m_vertices-(k-1))
            {
                //cerr << "Finalizing " << g.print() << endl;
                if (is_flag_forbidden(g)) return;                
                g.create_minlex_signature();
                if (!g_already_in_known_flags(g,flag_list)) 
                {
                    //cerr << g.print() << endl;
                    add_g_to_known_flags(g,flag_list);
                }
                return;                
            }
        }
        while(--test_to_increase >= 0)
        {
            current_edge[test_to_increase] =  current_edge[test_to_increase+1] + 1;
        }

        // It is not possible to increase the last edge
        if (current_edge[0] >= g.m_vertices)
            return;

        try_add_maybe_rooted_kedges(g, current_edge, flag_list, extending_only_last_vertex);
        return;
    }
     
    vector<int> next_edge = current_edge;
    next_edge[0]++;

    //cerr << "next edge=" << next_edge[0] << next_edge[1] << next_edge[2] << next_edge[3]   << endl;
    // cerr << "edge=" << current_edge[0] << current_edge[1] << current_edge[2] << current_edge[3];
    // cerr << " flag_list=" << flag_list.size() << endl;



    // We cannot add any edge
    //if (G_MAYBE_ROOTED_KEDGES > g.m_vertices)
    //    return;

    if (G_MAYBE_ROOTED_KEDGES <= g.m_vertices)
    {

    //return;

    rooted_kedge rke(current_edge, -1);


// Without udate
//  Mon Aug  8 16:05:15 2022 bernard@brumla-5.local /Users/bernard/Nextcloud/flag/ksat ./a.out  -n  7  -ub  time taken 0d+0h+3m+37s Memory=10M
// ./a.out -n 7 -ub  368.30s user 0.64s system 169% cpu 3:37.14 total 

//Mon Aug  8 16:10:54 2022 bernard@brumla-5.local /Users/bernard/Nextcloud/flag/ksat ./a.out  -n  7  -ub  time taken 0d+0h+4m+31s Memory=10M
//./a.out -n 7 -ub  436.86s user 1.20s system 161% cpu 4:31.77 total

//Mon Aug  8 16:15:47 2022 bernard@brumla-5.local /Users/bernard/Nextcloud/flag/ksat ./a.out  -n  7  -ub  time taken 0d+0h+4m+7s Memory=10M
//./a.out -n 7 -ub  393.57s user 0.95s system 159% cpu 4:07.11 total

    // Test if forbidden
#ifdef G_FORBIDDEN_NON_INDUCED
    // If adding non-rooted edge is forbidden, rooted is also
  //  if (is_flag_forbidden(g))
    {
   //     g.delete_maybe_rooted_kedge(rke);
   //     return;
    }
#endif

#ifndef G_MAYBE_ROOTED_KEDGES_FORCE_ROOTS
    g.add_maybe_rooted_kedge(rke);
    try_add_maybe_rooted_kedges(g, next_edge, flag_list, extending_only_last_vertex);
    g.delete_maybe_rooted_kedge(rke);
#endif


#ifndef G_MAYBE_ROOTED_KEDGES_DISABLE_ROOTS
    for (int root=0; root < k; root++)
    {
        rke.root=current_edge[root];
        g.add_maybe_rooted_kedge(rke);
#ifdef G_FORBIDDEN_NON_INDUCED
    //    if (is_flag_forbidden(g) == false)        
#endif        
        try_add_maybe_rooted_kedges(g, next_edge, flag_list, extending_only_last_vertex);
        g.delete_maybe_rooted_kedge(rke);
    }
#endif
    }

    // not adding current edge
    try_add_maybe_rooted_kedges(g, next_edge, flag_list, extending_only_last_vertex);
    // adding with all possible rootings...

}
#endif


#ifdef G_4EDGES_HACK
inline bool test_partly_forbidden(int x, int y, int z, int w, const flag &g)
{
    for (int q = 0; q < g.m_vertices; q++)
    {
        if (q == x || q == y || q == z || q == w) continue;
        int edges = g.m_4edge[x][y][z][w]+g.m_4edge[x][y][z][q]+g.m_4edge[x][y][q][w]+g.m_4edge[x][q][z][w]+g.m_4edge[q][y][z][w];
        if (edges == 5) return true;
        //if (edges == 3 && q < y) return true;
        //if (edges == 1 && q < y) return true;
    }
    return false;
}

inline bool test_forbidden(const flag &g)
{
    for (int x = 0; x < g.m_vertices-4; x++)
        for (int y = x+1; y < g.m_vertices-3; y++)
            for (int z = y+1; z < g.m_vertices-2; z++)
                for (int w = z+1; w < g.m_vertices-1; w++)
                    for (int q = w+1; q < g.m_vertices; q++)
                    {
                        //if (q == x || q == y || q == z || q == w) continue;
                        
                        int edges = g.m_4edge[x][y][z][w]+g.m_4edge[x][y][z][q]+g.m_4edge[x][y][q][w]+g.m_4edge[x][q][z][w]+g.m_4edge[q][y][z][w];
                        if (edges == 5) return true;
                        if (edges == 3) return true;
                        if (edges == 1) return true;
                    }
    return false;
}

int max_edges = 0;


void try_add_4edges_hack(flag &g, int x, int y, int z, int w, vector<flag> &flag_list)
{
    if (w >= g.m_vertices || w == x)
    {
        z++;
        if (z >= g.m_vertices-1)
        {
            y++;
            if (y >= g.m_vertices-2)
            {
                x++;
                if (x >= g.m_vertices-3)
                {
                    if (test_forbidden(g)) return;
                    if (is_flag_forbidden(g)) return;
                    
                    if (!g_already_in_known_flags(g, flag_list)) add_g_to_known_flags(g, flag_list);
                    
                    return;
                }
                y = x+1;
            }
            z = y+1;
        }
        w = z+1;
        try_add_4edges_hack(g, x,y,z,w,flag_list);
        return;
    }

    //cerr << "Testing " << x << " " << y << " " << z << " " << w << endl;

    
    if (g.m_4edge[x][y][z][w] != 0) try_add_4edges_hack(g, x,y,z,w+1,flag_list);
    else
    {
        g.set_4edge(x,y,z,w,1);
        
        if(!test_partly_forbidden(x,y,z,w,g))
        {
            try_add_4edges_hack(g, x,y,z,w+1,flag_list);
        }
        g.set_4edge(x,y,z,w,0);
        
        if(!test_partly_forbidden(x,y,z,w,g))
        {
            try_add_4edges_hack(g, x,y,z,w+1,flag_list);
        }
    }
}

#ifdef G_COLORED_VERTICES
bool try_add_4edges_hack_Y_test_forbidden(flag &g)
{
    //cerr << "Testing " << g.print() << endl;

    int v = g.m_vertices-1;
    //g_unlabeled_flags[v];
    

//#ifdef G_USE_PERMITTED_SUBFLAGS
    vector<flag> subflags_list;

    g.generate_subflags_of_size_n(v, subflags_list);
        

    for (int j = 0; j < (int)subflags_list.size(); j++)
    {
        if (subflags_list[j].m_colored_vertices[1] != 3) continue;
        if (find_flag_in_list(subflags_list[j], g_permitted_subflags[v]) == -1)
        {
            //cerr << "Failed at  " << subflags_list[j].print() << endl;
            return true;
        }
    }
//#endif    
    return false;
}


void try_add_4edges_hack_Y(flag &g, int x, int y, int z, int w, vector<flag> &flag_list)
{
    if (w >= g.m_vertices || w == x)
    {
        z++;
        if (z >= g.m_vertices-1)
        {
            y++;
            if (y >= g.m_vertices-2)
            {
                x++;
                if (x >= g.m_vertices-3)
                {
                    // Here we test if the flag is allowed 
                    if (try_add_4edges_hack_Y_test_forbidden(g)) return;
                    //if (is_flag_forbidden(g)) return;
                    
                    if (!g_already_in_known_flags(g, flag_list)) 
                    {
                        cerr << " Found " << g.print() << endl;
                        add_g_to_known_flags(g, flag_list);
                    }
                    
                    return;
                }
                y = x+1;
            }
            z = y+1;
        }
        w = z+1;
        try_add_4edges_hack_Y(g, x,y,z,w,flag_list);
        return;
    }

    //cerr << "Testing " << x << " " << y << " " << z << " " << w << endl;

    
    if (g.m_4edge[x][y][z][w] != 0) try_add_4edges_hack_Y(g, x,y,z,w+1,flag_list);
    else
    {
        g.set_4edge(x,y,z,w,1);
        
        //if(!test_partly_forbidden(x,y,z,w,g))
        {
            try_add_4edges_hack_Y(g, x,y,z,w+1,flag_list);
        }
        g.set_4edge(x,y,z,w,0);
        
        //if(!test_partly_forbidden(x,y,z,w,g))
        {
            try_add_4edges_hack_Y(g, x,y,z,w+1,flag_list);
        }
    }
}
#endif

#endif



void extensions_of_g_edges(flag &g, vector<flag> &flag_list)
{
    //cerr << "Generating extensions of " << g.print() << endl;
#ifdef G_COLORED_3EDGES    
#ifdef G_COLORED_EDGES
//#error I dont know how to generate 2edges and 3edges at the same time.
    cerr << "Extensions not implemented for 3edges and 2 edges at the same time" << endl;
#endif
#endif

#if defined(G_ROOTED_4EDGES)     
    try_add_rooted_4edges(g,0,1,2,3,flag_list, false);
    return;
#endif      

#if defined(G_4EDGES) && !defined(G_4EDGES_HACK)       
    try_add_4edges(g,0,1,2,3,flag_list, false);
    return;
#endif       
#ifdef G_4EDGES_HACK
    try_add_4edges_hack(g,0,1,2,3,flag_list);
    //try_add_4edges_hack_Y_test_forbidden(g,0,1,2,3,flag_list);
    return;
#endif

#ifdef G_ROOTED_3EDGES       
    try_add_rooted_3edges(g,0,1,2,flag_list, false);
    return;
#endif

#ifdef G_3EDGES       
    try_add_3edges(g,0,1,2,flag_list, false);
    return;
#endif

#ifdef G_COLORED_3EDGES       
    try_color_3edges(g,0,1,2,flag_list, false);
    return;
#endif    
#ifdef G_COLORED_EDGES
    try_color_edge(g,0,1,flag_list);
    return;
#endif

#ifdef G_ROTATION_SYSTEM
    cerr << "This program cannot generate rotation systems. They must be provided externally." << endl;
    exit(0);
#endif

#ifdef G_ORDER_TYPES
    cerr << "This program cannot generate order types. They must be provided externally." << endl;
    exit(0);
#endif     

    cerr << "ERROR: Extensions not implemented for current setup." << endl;
}


#ifdef G_COLORED_VERTICES
void try_color_vertex(flag &g, int u, vector<flag> &flag_list)
{
    if (u >= g.m_vertices)
    {
#if defined(G_COLORED_EDGES) || defined(G_COLORED_3EDGES) || defined(G_3EDGES) || defined(G_4EDGES) || defined(G_ROOTED_3EDGES) || defined(G_ROOTED_4EDGES)
        extensions_of_g_edges(g,flag_list);        
        //        try_color_edge(g, 0,1,flag_list);
#else
        if (is_flag_forbidden(g)) return;
        
        g.create_minlex_signature();
        
        if (!g_already_in_known_flags(g, flag_list)) add_g_to_known_flags(g, flag_list);
#endif
        return;
    }
    
    // Color only uncolored vertices.....
    if (g.m_color_vertex[u] == 0)
    {
        for (int color = 1; color < COLORS_VERTICES; color++)
        {
            g.color_vertex(u,color);
            try_color_vertex(g,u+1,flag_list);
        }
        g.color_vertex(u,0);
    }
    else
    {
        try_color_vertex(g,u+1,flag_list);
    }
}
#endif



void extensions_of_g(flag &g, vector<flag> &flag_list)
{
#ifdef G_COLORED_VERTICES
    try_color_vertex(g,0,flag_list);
#else
    extensions_of_g_edges(g,flag_list);
#endif
}



// u is original, v will be a copy
// edges containing both will have color 1
void duplicate_vertex_u_to_v(flag &g, int u, int v)
{
#if defined(G_4EDGES) || defined(G_ROOTED_4EDGES) || defined(G_3EDGES) || defined(G_ROOTED_3EDGES) || defined(G_ROTATION_SYSTEM) || defined(G_ORDER_TYPES) || defined(G_CROSSINGS)
    assert(0); // this is not implemented
#endif

    
#ifdef G_COLORED_EDGES
    
    for (int x = 0; x < g.m_vertices-1; x++)
    {
        if (x == u) 
            g.color_edge(v,x,1);
        else
            g.color_edge(v,x,g.m_color_edge[u][x]);
    }
#endif

#ifdef G_COLORED_3EDGES
    for (int x = 0; x < g.m_vertices-1; x++)
    {
        for (int y = x+1; y < g.m_vertices-1; y++)
        {
            if (x == u || y == u)
                g.color_3edge(v,x,y,1);
            else
                g.color_3edge(v,x,y,g.m_colored_3edge[u][x][y]);
        }
    }
#endif

#ifdef G_COLORED_VERTICES
    g.color_vertex(v,g.m_color_vertex[u]);    
#endif
    
//    assert(0);
}


#ifdef G_USE_FIRST_EDGE_COLOR_FOR_BLOWUP_ONLY
void generate_blowups_by_one_vertex(const vector<flag> &original_flags, vector<flag> &flag_list)
{
    for (int i = 0; i < (int)original_flags.size(); i++)
    {
        int v = original_flags[i].m_vertices;
        for (int u = 0; u < v; u++)
        {
            flag g;
            g.set_vertices(v+1);
            g.copy_from(original_flags[i]);
            duplicate_vertex_u_to_v(g, u, v);
            
            //if (is_flag_forbidden(g)) return;  // this is unlikely, but we need to try...
            if (!g_already_in_known_flags(g,flag_list)) add_g_to_known_flags(g,flag_list);
        }
    }
    
}
#endif

/*
 void generate_permutations(vector<flag> &flag_list, int vertices)
 {
 g.set_vertices(vertices);
 int perm[g.m_vertices];
 for (int i = 0; i < g.m_vertices; i++) perm[i] = i;
 
 do {
 
 g.set_vertices(g.m_vertices);
 for (int u = 0; u < g.m_vertices; u++)
 for (int v = u+1; v < g.m_vertices; v++)
 {
 if (perm[u]<perm[v]) g.color_edge(u,v,1);
 else g.color_edge(u,v,2);
 }
 
 if (!g_already_in_known_flags(flag_list)) add_g_to_known_flags(flag_list);
 
 
 } while ( std::next_permutation(perm,perm+g.m_vertices) );
 }
*/



///////////////////////////////////////////////////////////////////////////////////////////////////////////////// P(F1,F2,H)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////// P(F1,F2,H)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////// P(F1,F2,H)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////// P(F1,F2,H)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////// P(F1,F2,H)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////// P(F1,F2,H)


// Rest of F2 is also piskced as a subset
void pick_F2_mapping(const flag &F1, const flag &F2, const flag &H, int *mapping_F2, int next_to_map,  bool *used_from_H, int &good_maps)
{
    if (next_to_map >= F2.m_vertices)
    {
        flag H_F2;
        H_F2.as_subflag(H, mapping_F2, F2.m_vertices, F2.m_Theta);
        if (!H_F2.is_isomorphic_to(F2)) return;
		
        good_maps++;
        
        return;
    }
    
    int min_vertex = 0;
    if (next_to_map > F2.m_Theta) min_vertex = mapping_F2[next_to_map-1]+1;
    
    for (int v = min_vertex; v < H.m_vertices; v++)
    {
        if (used_from_H[v]) continue;
        
        used_from_H[v] = true;
        mapping_F2[next_to_map] = v;
        pick_F2_mapping(F1,F2,H,mapping_F2,next_to_map+1,used_from_H,good_maps);
        used_from_H[v] = false;
    }
    
}

// Rest of F1 is picked only as a subset
void pick_F1_mapping(const flag &F1, const flag &F2, const flag &H, int *mapping_F1, int *mapping_F2, int next_to_map,  bool *used_from_H, int &good_maps)
{
    if (next_to_map >= F1.m_vertices)
    {
		//cout << "." << endl;

        // Check if the mapping is corect!
        flag H_F1;
        H_F1.as_subflag(H, mapping_F1, F1.m_vertices,F1.m_Theta);
		
        if (!H_F1.is_isomorphic_to(F1)) return;
        
        pick_F2_mapping(F1,F2,H,mapping_F2,F2.m_Theta,used_from_H,good_maps);
        return;
    }
    
    int min_vertex = 0;
    if (next_to_map > F1.m_Theta) min_vertex = mapping_F1[next_to_map-1]+1;
    
    for (int v = min_vertex; v < H.m_vertices; v++)
    {
        if (used_from_H[v]) continue;
        
        used_from_H[v] = true;
        mapping_F1[next_to_map] = v;
        pick_F1_mapping(F1,F2,H,mapping_F1,mapping_F2,next_to_map+1,used_from_H,good_maps);
        used_from_H[v] = false;
    }
}

// Theta mapping is picked as all permutations...
void pick_theta_mapping(const flag &F1, const flag &F2, const flag &H, int *mapping_theta, int next_to_map, bool *used_from_H, int &good_maps)
{
    if (next_to_map >= F1.m_Theta)
    {
        int mapping_F1[F1.m_vertices];
        int mapping_F2[F2.m_vertices];
        for (int i = 0; i < F1.m_Theta; i++)
        {
            mapping_F2[i] = mapping_F1[i] = mapping_theta[i];
        }
				
        pick_F1_mapping(F1,F2,H,mapping_F1,mapping_F2, F1.m_Theta, used_from_H,good_maps);
        return;
    }
	
    for (int v = 0; v < H.m_vertices; v++)
    {
        if (used_from_H[v]) continue;
        
        if (!H.is_map_up_to_v_correct(next_to_map, v, mapping_theta, F1)) continue;
       
         used_from_H[v] = true;
         mapping_theta[next_to_map] = v;
         pick_theta_mapping(F1,F2,H,mapping_theta,next_to_map+1,used_from_H,good_maps);
		 used_from_H[v] = false;
    }
}

inline double P_F1_F2_IN_H(const flag &F1, const flag &F2, const flag &H, bool return_only_good_maps=false)
{
    assert(F1.m_Theta == F2.m_Theta);

#if defined(G_COLORED_EDGES) && !defined(G_COLORED_EDGES_BLIND)
	for (int c = 1; c < COLORS_EDGES; c++)
	{
        //		cout << "e" << endl;
        if (F1.m_colored_edges[c] > H.m_colored_edges[c]) return 0;
        if (F2.m_colored_edges[c] > H.m_colored_edges[c]) return 0;
	}
#endif

    
#ifdef G_3EDGES    
    if (F1.m_3edges_cnt > H.m_3edges_cnt) return 0;
    if (F2.m_3edges_cnt > H.m_3edges_cnt) return 0;
#endif   

#ifdef G_ROOTED_3EDGES    
    if (F1.m_rooted_3edges_cnt > H.m_rooted_3edges_cnt) return 0;
    if (F2.m_rooted_3edges_cnt > H.m_rooted_3edges_cnt) return 0;
#endif   


#if defined(G_COLORED_3EDGES) && !defined(G_COLORED_3EDGES_BLIND)
    for (int c = 1; c < COLORS_3EDGES; c++)
    {
        //		cout << "e" << endl;
        if (F1.m_colored_3edges[c] > H.m_colored_3edges[c]) return 0;
        if (F2.m_colored_3edges[c] > H.m_colored_3edges[c]) return 0;
    }
#endif
    
#ifdef G_4EDGES    
    if (F1.m_4edges_cnt > H.m_4edges_cnt) return 0;
    if (F2.m_4edges_cnt > H.m_4edges_cnt) return 0;
#endif   
    
#ifdef G_ROOTED_4EDGES    
    if (F1.m_rooted_4edges_cnt > H.m_rooted_4edges_cnt) return 0;
    if (F2.m_rooted_4edges_cnt > H.m_rooted_4edges_cnt) return 0;
#endif   

#ifdef G_MAYBE_ROOTED_KEDGES    
    if (F1.m_rooted_kedges.size() > H.m_rooted_kedges.size()) return 0;
    if (F2.m_rooted_kedges.size() > H.m_rooted_kedges.size()) return 0;
#endif   


#ifdef G_ROTATION_SYSTEM
    // no quick check
#endif
    
#ifdef G_COLORED_VERTICES
#ifdef G_COLORED_VERTICES_BLIND
    assert(0);
#else 
	for (int c = 1; c < COLORS_VERTICES; c++)
	{
		//cout << "v " << F1.m_colored_vertices[c] << " " << H.m_colored_vertices[c] << endl;
		if (F1.m_colored_vertices[c] > H.m_colored_vertices[c]) return 0;
		if (F2.m_colored_vertices[c] > H.m_colored_vertices[c]) return 0;
	}
#endif    
#endif
	
#ifdef G_CROSSINGS	
//	 if (F1.m_crossings_cnt > H.m_crossings_cnt) return 0;
//	 if (F2.m_crossings_cnt > H.m_crossings_cnt) return 0;
	 if (F1.m_crossings_cnt + F2.m_crossings_cnt - F1.m_crossings_in_type > H.m_crossings_cnt) return 0;
#endif
		
    int mapping_theta[F1.m_Theta];
    bool used_from_H[H.m_vertices];
    
    for (int i = 0; i < H.m_vertices; i++)
    {
        used_from_H[i] = false;
    }
	
    int good_maps = 0;
    pick_theta_mapping(F1,F2,H,mapping_theta,0,used_from_H,good_maps);
	
    //cerr << "F1="<<F1.print() << endl;
    //cerr << "F2="<<F2.print() << endl;
    //cerr << "H="<<H.print() << endl;
    //cerr << "good_maps="<<good_maps << endl << endl;

    if (return_only_good_maps) return good_maps;

    int all_maps = 1;
#ifdef G_COLORED_VERTICES_SAMPLED_SEPARATELY_BY_COLORS
    int colors_available[COLORS_VERTICES];
    int colors_needed_F1[COLORS_VERTICES];
    int colors_needed_F2[COLORS_VERTICES];
    colors_available[0] = 0;
    for (int c = 1; c < COLORS_VERTICES; c++)
    {
        colors_available[c] = H.m_colored_vertices[c];
        colors_needed_F1[c] = F1.m_colored_vertices[c];
        colors_needed_F2[c] = F2.m_colored_vertices[c];
    }
    for (int i = 0; i < F1.m_Theta; i++) 
    {
        all_maps *= colors_available[F1.m_color_vertex[i]]--;
        colors_needed_F1[F1.m_color_vertex[i]]--;
        colors_needed_F2[F1.m_color_vertex[i]]--;
    }
    for (int c = 1; c < COLORS_VERTICES; c++)
    {
        all_maps *= binomial(colors_available[c],colors_needed_F1[c]);
        all_maps *= binomial(colors_available[c]-colors_needed_F1[c],colors_needed_F2[c]);
    }    
#else
    for (int i = 0; i < F1.m_Theta; i++) all_maps *= (H.m_vertices-i);
    all_maps *= binomial(H.m_vertices-F1.m_Theta,F1.m_vertices-F1.m_Theta);
    all_maps *= binomial(H.m_vertices-F1.m_vertices,F2.m_vertices-F1.m_Theta);
#endif
	//    if (good_maps > 0)
	//        cout << "good_maps/all_maps: " << good_maps << "/" << all_maps << endl;
    
    
    return (double)good_maps/(double)all_maps;
}



inline double P_F1_F2_F3_IN_H(const flag &F1, const flag &F2, const flag &F3, const flag &H)
{

#ifdef G_COLORED_VERTICES_SAMPLED_SEPARATELY_BY_COLORS
    cerr << "Currently not implemented" << endl;
    assert(0);
#endif

    assert(F1.m_Theta == F2.m_Theta);
    assert(F1.m_Theta == F3.m_Theta);
    
    assert(F1.m_vertices+F2.m_vertices+F3.m_vertices - 2*F1.labeled_vertices_cnt() <= H.m_vertices);
    
    flag type;
    F1.get_type_subflag(type);    

    
    // Some easy checks...
#if defined(G_COLORED_EDGES) && !defined(G_COLORED_EDGES_BLIND)
    for (int c = 1; c < COLORS_EDGES; c++)
    {
        if (F1.m_colored_edges[c] + F2.m_colored_edges[c] + F3.m_colored_edges[c] - 2*type.m_colored_edges[c] > H.m_colored_edges[c]) return 0;
    }
#endif
    
    
#ifdef G_3EDGES
    if (F1.m_3edges_cnt + F2.m_3edges_cnt + F3.m_3edges_cnt - 2*type.m_3edges_cnt  > H.m_3edges_cnt) return 0;
#endif   

#ifdef G_ROOTED_3EDGES
    if (F1.m_rooted_3edges_cnt + F2.m_rooted_3edges_cnt + F3.m_rooted_3edges_cnt - 2*type.m_rooted_3edges_cnt  > H.m_rooted_3edges_cnt) return 0;
#endif   


    
#if defined(G_COLORED_3EDGES) && !defined(G_COLORED_3EDGES_BLIND)
    for (int c = 1; c < COLORS_3EDGES; c++)
    {
        if (F1.m_colored_3edges[c]+F2.m_colored_3edges[c]+F3.m_colored_3edges[c]-2*type.m_colored_3edges[c] > H.m_colored_3edges[c]) return 0;
    }
#endif
    
#ifdef G_4EDGES    
    if (F1.m_4edges_cnt+F2.m_4edges_cnt+F3.m_4edges_cnt-2*type.m_4edges_cnt  > H.m_4edges_cnt) return 0;
#endif   

#ifdef G_ROOTED_4EDGES    
    if (F1.m_rooted_4edges_cnt+F2.m_rooted_4edges_cnt+F3.m_rooted_4edges_cnt-2*type.m_rooted_4edges_cnt  > H.m_rooted_4edges_cnt) return 0;
#endif   

#ifdef G_ROTATION_SYSTEM
    // No quick check
#endif

    
#if defined(G_COLORED_VERTICES) && !defined(G_COLORED_VERTICES_BLIND)
    for (int c = 1; c < COLORS_VERTICES; c++)
    {
        if (F1.m_colored_vertices[c]+F2.m_colored_vertices[c]+F3.m_colored_vertices[c]-2*type.m_colored_vertices[c] > H.m_colored_vertices[c]) return 0;
    }
#endif
    
#ifdef G_CROSSINGS	
    if (F1.m_crossings_cnt + F2.m_crossings_cnt - F1.m_crossings_in_type > H.m_crossings_cnt) return 0;
#endif


    
#ifdef G_ORDERED_VERTICES
    assert(0); // not implemented
#else

    int good_maps = 0;
    int all_maps = 0;
    
    int vertex_split[V];

    int t = 0;
    for (int i = 0; i < type.m_vertices; i++) vertex_split[t++] = i;
    for (int i = type.m_vertices; i < F1.m_vertices; i++) vertex_split[t++] = -1;
    for (int i = type.m_vertices; i < F2.m_vertices; i++) vertex_split[t++] = -2;
    for (int i = type.m_vertices; i < F3.m_vertices; i++) vertex_split[t++] = -3;
    for (;t < H.m_vertices; t++) vertex_split[t] = -4;
    
    std::sort (vertex_split,vertex_split+H.m_vertices);
    
    flag tmp;
    int mapping[V];
    int next_to_map; // temporary
    do {
        all_maps++;
        
        
        //cerr << "split: ";
        //for (int i = 0; i < H.m_vertices; i++)
        //    cerr << vertex_split[i] << " " ;
        //cerr << endl;        
        
        
        // check if Theta correct
        for (int j = 0; j < H.m_vertices; j++)
        {
            if (vertex_split[j] < 0) continue;
            if (vertex_split[j] < type.m_vertices) mapping[vertex_split[j]] = j;
        }
        tmp.as_subflag(H,mapping,type.m_vertices,type.m_Theta);
        //cerr << tmp.print() << " vs " << type.print();
        if (!tmp.is_isomorphic_to(type)) continue;

        
        // check if F1 correct
        next_to_map = type.m_vertices;
        for (int j = 0; j < H.m_vertices; j++)  if (vertex_split[j] == -1) mapping[next_to_map++] = j;
        tmp.as_subflag(H,mapping,F1.m_vertices,type.m_Theta);
        if (!tmp.is_isomorphic_to(F1)) continue;


        // check if F2 correct
        next_to_map = type.m_vertices;
        for (int j = 0; j < H.m_vertices; j++)  if (vertex_split[j] == -2) mapping[next_to_map++] = j;
        tmp.as_subflag(H,mapping,F2.m_vertices,type.m_Theta);
        if (!tmp.is_isomorphic_to(F2)) continue;


        
        // check if F3 correct
        next_to_map = type.m_vertices;
        for (int j = 0; j < H.m_vertices; j++)  if (vertex_split[j] == -3) mapping[next_to_map++] = j;
        tmp.as_subflag(H,mapping,F3.m_vertices,type.m_Theta);
        if (!tmp.is_isomorphic_to(F3)) continue;
        
        good_maps++;
        
    } while ( std::next_permutation(vertex_split,vertex_split+H.m_vertices) ); 

    //cerr << "Good/All: " << good_maps << "/" << all_maps << endl;
    
    return (double)good_maps/(double)all_maps;
#endif        
}


inline double P_F1_F2_IN_labeled_H(const flag &F1, const flag &F2, const flag &H)
{

    //cerr << F1.m_Theta << " " << F2.m_Theta << endl;
    assert(F1.m_Theta == F2.m_Theta);
    if (F1.m_vertices+F2.m_vertices - F1.labeled_vertices_cnt() > H.m_vertices)
    {
        cerr << F1.m_vertices<< "+" << F2.m_vertices << "-" << F1.labeled_vertices_cnt() << " > " << H.m_vertices << endl;
    }
    assert(F1.m_vertices+F2.m_vertices - F1.labeled_vertices_cnt() <= H.m_vertices);
    assert(F1.labeled_vertices_cnt() >= H.labeled_vertices_cnt());
    
    flag type;
    F1.get_type_subflag(type);    

    flag type_F2;
    F2.get_type_subflag(type_F2);   

    if (type.is_isomorphic_to(type_F2) == false)
    {
        return 0;
    }


    if (H.labeled_vertices_cnt() > 0)
    {
        flag typeH;
        H.get_type_subflag(typeH);    
        if (!type.contains_as_subflag(typeH)) return 0;
    }
    
    // Some easy checks...
#if defined(G_COLORED_EDGES) && !defined(G_COLORED_EDGES_BLIND)
    for (int c = 1; c < COLORS_EDGES; c++)
    {
        if (F1.m_colored_edges[c] + F2.m_colored_edges[c] - type.m_colored_edges[c] > H.m_colored_edges[c]) return 0;
    }
#endif
#ifdef G_3EDGES
    if (F1.m_3edges_cnt + F2.m_3edges_cnt - type.m_3edges_cnt  > H.m_3edges_cnt) return 0;
#endif   
#ifdef G_ROOTED_3EDGES
    if (F1.m_rooted_3edges_cnt + F2.m_rooted_3edges_cnt - type.m_rooted_3edges_cnt  > H.m_rooted_3edges_cnt) return 0;
#endif   
#if defined(G_COLORED_3EDGES) && !defined(G_COLORED_3EDGES_BLIND)
    for (int c = 1; c < COLORS_3EDGES; c++)
    {
        if (F1.m_colored_3edges[c]+F2.m_colored_3edges[c]-type.m_colored_3edges[c] > H.m_colored_3edges[c]) return 0;
    }
#endif
#ifdef G_4EDGES    
    if (F1.m_4edges_cnt+F2.m_4edges_cnt-type.m_4edges_cnt  > H.m_4edges_cnt) return 0;
#endif
#ifdef G_ROOTED_4EDGES    
    if (F1.m_rooted_4edges_cnt+F2.m_rooted_4edges_cnt-type.m_rooted_4edges_cnt  > H.m_rooted_4edges_cnt) return 0;
#endif
#ifdef G_ROTATION_SYSTEM
    // No quick check
#endif   
#if defined(G_COLORED_VERTICES) && !defined(G_COLORED_VERTICES_BLIND)
    for (int c = 1; c < COLORS_VERTICES; c++)
    {
        if (F1.m_colored_vertices[c]+F2.m_colored_vertices[c]-type.m_colored_vertices[c] > H.m_colored_vertices[c]) return 0;
    }
#endif
#ifdef G_CROSSINGS	
    if (F1.m_crossings_cnt + F2.m_crossings_cnt - F1.m_crossings_in_type > H.m_crossings_cnt) return 0;
#endif
    
    
#ifdef G_ORDERED_VERTICES

    assert(0); // not implemented
#else

    int good_maps = 0;
    int all_maps = 0;
    
    int vertex_split[V];
    
    int t = 0;
    for (int i = 0; i < type.m_vertices; i++) vertex_split[t++] = i;
    for (int i = type.m_vertices; i < F1.m_vertices; i++) vertex_split[t++] = -1;
    for (int i = type.m_vertices; i < F2.m_vertices; i++) vertex_split[t++] = -2;
    //for (int i = type.m_vertices; i < F3.m_vertices; i++) vertex_split[t++] = -3;
    for (;t < H.m_vertices; t++) vertex_split[t] = -4;
    
    std::sort (vertex_split+H.labeled_vertices_cnt(),vertex_split+H.m_vertices);
    
    flag tmp_type;
    flag tmp_F1;
    flag tmp_F2;
    int mapping[V];
    int next_to_map; // temporary
    do {
                
        // check if Theta correct
        for (int j = 0; j < H.m_vertices; j++)
        {
            if (vertex_split[j] < 0) continue;
            if (vertex_split[j] < type.m_vertices) mapping[vertex_split[j]] = j;
        }
        tmp_type.as_subflag(H,mapping,type.m_vertices,type.m_Theta);

        //cerr << tmp.print() << " vs " << type.print();
        
        // check if F1 correct
        next_to_map = type.m_vertices;
        for (int j = 0; j < H.m_vertices; j++)  if (vertex_split[j] == -1) mapping[next_to_map++] = j;
        tmp_F1.as_subflag(H,mapping,F1.m_vertices,type.m_Theta);
                
        // check if F2 correct
        next_to_map = type.m_vertices;
        for (int j = 0; j < H.m_vertices; j++)  if (vertex_split[j] == -2) mapping[next_to_map++] = j;
        tmp_F2.as_subflag(H,mapping,F2.m_vertices,type.m_Theta);

#ifdef G_COLORED_VERTICES_SAMPLED_SEPARATELY_BY_COLORS
        // check if the labeling is color preserving
        bool color_preserved = true;
        for (int v = 0; v < F1.m_Theta; v++)
        {
            if (F1.m_color_vertex[v] != tmp_type.m_color_vertex[v]) 
            {
                color_preserved = false;
                break;
            }
        }
        if (color_preserved == false)
            continue;

        // check if the map is color preserving
        color_preserved = true;
        for (int c = 1; c < COLORS_VERTICES; c++)
        {
            if (F1.m_colored_vertices[c] != tmp_F1.m_colored_vertices[c] 
              ||F2.m_colored_vertices[c] != tmp_F2.m_colored_vertices[c]) 
            {
                color_preserved = false;
                break;
            }
        }
        if (color_preserved == false)
            continue;
#endif

        all_maps++;


        if (!tmp_type.is_isomorphic_to(type)) continue;
        if (!tmp_F1.is_isomorphic_to(F1)) continue;
        if (!tmp_F2.is_isomorphic_to(F2)) continue;        
        
        good_maps++;
        
    } while ( std::next_permutation(vertex_split+H.labeled_vertices_cnt(),vertex_split+H.m_vertices) ); 
    
    //cerr << "Good/All: " << good_maps << "/" << all_maps << endl;
    
    return (double)good_maps/(double)all_maps;
#endif        
}


#ifdef G_ORDERED_VERTICES
void P_F1_IN_H_ordered_recursion(const flag &F1, const flag &H, int &good_maps, int &all_maps,  bool *used, vector<int> &stack, int next_to_pick)
{
    
    //cerr << "Call " << F1.m_vertices << " " << H.m_vertices << " " << stack.size() << " " << next_to_pick << endl;
    
    if ((int)stack.size() >= F1.m_vertices)
    {
        all_maps++;
        
        flag tmp;
        int mapping[V];
        int next_to_map=0; // temporary
        
        int Theta = 0;
        
        for (int i = 0; i < H.m_vertices; i++)
        {
            if (used[i])
            {
                mapping[next_to_map] = i;
                if (H.m_labeled[i])
                {
                    Theta = Theta | (1 << next_to_map);
                }
                next_to_map++;
            }
        }
        
        
        tmp.as_subflag(H,mapping,F1.m_vertices,Theta);
        if (tmp.is_isomorphic_to(F1)) good_maps++;
        
        return;
    }
    else
    {
        for (int v = next_to_pick; v < H.m_vertices; v++)
        {
            //assert(used[v] == false);
            if (used[v]) continue;
            
            stack.push_back(v);
            used[v] = true;
            
            P_F1_IN_H_ordered_recursion(F1, H, good_maps, all_maps, used, stack, v+1);
            
            used[v] = false;
            stack.pop_back();
        }
    }
}
#endif


inline double P_F1_IN_H(const flag &F1, const flag &H, bool density=true)
{
    
    assert(F1.m_vertices <= H.m_vertices);

#ifdef G_USE_INDEXES_FOR_LARGE_UNLABELED_FLAGS
    if (F1.m_vertices == H.m_vertices && F1.m_unlabeled_id != -1 && H.m_unlabeled_id != -1)
    {
        if (F1.m_unlabeled_id == H.m_unlabeled_id)
            return 1;
        else
            return 0;
    }
#endif

    flag type;
    F1.get_type_subflag(type);    

    if (H.labeled_vertices_cnt() > 0)
    {
        flag typeH;
        H.get_type_subflag(typeH);    
        if (!type.contains_as_subflag(typeH)) return 0;
    }    
    
    // Some easy checks...
#if defined(G_COLORED_EDGES) && !defined(G_COLORED_EDGES_BLIND)
        
    for (int c = 1; c < COLORS_EDGES; c++)
    {
        if (F1.m_colored_edges[c] > H.m_colored_edges[c]) return 0;
    }
#endif
    
    
#ifdef G_3EDGES
    if (F1.m_3edges_cnt > H.m_3edges_cnt) return 0;
#endif   
#ifdef G_ROOTED_3EDGES
    if (F1.m_rooted_3edges_cnt > H.m_rooted_3edges_cnt) return 0;
#endif   

#if defined(G_COLORED_3EDGES) && !defined(G_COLORED_3EDGES_BLIND)
    for (int c = 1; c < COLORS_3EDGES; c++)
    {
        if (F1.m_colored_3edges[c] > H.m_colored_3edges[c]) return 0;
    }
#endif
    
#ifdef G_4EDGES    
    if (F1.m_4edges_cnt  > H.m_4edges_cnt) return 0;
#endif
    
#ifdef G_ROOTED_4EDGES    
    if (F1.m_rooted_4edges_cnt  > H.m_rooted_4edges_cnt) return 0;
#endif

#ifdef G_ROTATION_SYSTEM
//#error TODO  // Could be usefull for some speedup...
#endif
    
#if defined(G_COLORED_VERTICES) && !defined(G_COLORED_VERTICES_BLIND)
    for (int c = 1; c < COLORS_VERTICES; c++)
    {
        if (F1.m_colored_vertices[c] > H.m_colored_vertices[c]) return 0;
    }
#endif
    
#ifdef G_CROSSINGS	
    if (F1.m_crossings_cnt > H.m_crossings_cnt) return 0;
#endif
    

    int good_maps = 0;
    int all_maps = 0;
    
#ifdef G_ORDERED_VERTICES
    //assert(0); // not implemented

    assert(F1.m_labeled_vertices_cnt == 0);
    assert(H.m_labeled_vertices_cnt == 0);

    bool used[V];
    for (int i = 0; i < V; i++) used[i] = false;
    vector<int> stack;
    stack.reserve(F1.m_vertices);
    
    P_F1_IN_H_ordered_recursion(F1, H, good_maps, all_maps,  used, stack, 0);    
#else
    // Map labeled vertices as 0,1,2,theta,-1,-1,-1,-4,-4,-4
    // where -1 indicates number of vertices in F1
    int vertex_split[2*V]; // the 2*V instead of V is here for overflow warning
    
    int t = 0;
    for (int i = 0; i < type.m_vertices; i++) vertex_split[t++] = i;
    for (int i = type.m_vertices; i < F1.m_vertices; i++) vertex_split[t++] = -1;
    for (;t < H.m_vertices; t++) vertex_split[t] = -4;
    
    std::sort (vertex_split+H.labeled_vertices_cnt(),vertex_split+H.m_vertices);
    
    //cerr << "FFF" << endl;

    flag tmp;
    flag tmp_type;
    int mapping[V];
    int next_to_map; // temporary
    do {

        //cerr << "DDDD" << endl;
        //for (int i  = 0; i < F1.m_vertices; i++)
        //{
        //    cerr << i << "->" << mapping[i] << " "; 
        //}
        //cerr << endl;
        
        
        // check if Theta correct
        for (int j = 0; j < H.m_vertices; j++)
        {
            if (vertex_split[j] < 0) continue;
            if (vertex_split[j] < type.m_vertices) mapping[vertex_split[j]] = j;
        }

        // Finish creating the mapping
        next_to_map = type.m_vertices;
        for (int j = 0; j < H.m_vertices; j++)  if (vertex_split[j] == -1) mapping[next_to_map++] = j;


        tmp_type.as_subflag(H,mapping,type.m_vertices,type.m_Theta);
        tmp.as_subflag(H,mapping,F1.m_vertices,type.m_Theta);

#ifdef G_COLORED_VERTICES_SAMPLED_SEPARATELY_BY_COLORS
        // check if the labeling is color preserving
        bool color_preserved = true;
        for (int v = 0; v < F1.m_Theta; v++)
        {
            if (F1.m_color_vertex[v] != tmp_type.m_color_vertex[v]) 
            {
                color_preserved = false;
                //cerr << "Color not preserved for labeled  v=" << v << endl;
                break;
            }
        }
        if (color_preserved == false)
        {
            continue;
        }

        // check if the map is color preserving
        color_preserved = true;
        for (int c = 1; c < COLORS_VERTICES; c++)
        {
            if (F1.m_colored_vertices[c] != tmp.m_colored_vertices[c]) 
            {
                color_preserved = false;
                break;
            }
        }
        if (color_preserved == false)
            continue;
#endif

        all_maps++;

        // check if type is correct
        if (!tmp_type.is_isomorphic_to(type)) continue;
        
        // check if F1 is correct
        tmp.as_subflag(H,mapping,F1.m_vertices,type.m_Theta);
        if (!tmp.is_isomorphic_to(F1)) continue;
        
        good_maps++;
        
    } while ( std::next_permutation(vertex_split+H.labeled_vertices_cnt(),vertex_split+H.m_vertices) ); 
    
    //cerr << "Good/All: " << good_maps << "/" << all_maps << endl;
#endif        
    if (density)
    {
        if (all_maps == 0)
        {
            cerr << "all_maps is zero" << endl;
            cerr << "Calculating P(F1,H), where" << endl;
            cerr << "F1 = " << F1.print() << endl;
            cerr << " H = " << H.print() << endl;
            assert(0);
        }
        return (double)good_maps/(double)all_maps;
    }
    else
        return good_maps;    
}


//  The following is by James Kanze
// http://gabisoft.free.fr/articles/fltrsbf1.html
// https://openclassrooms.com/forum/sujet/fluxtampon-gestion-31091?page=1
// https://lists.boost.org/Archives/boost/att-49459/fltrsbf1.htm
struct FilteringInputStreambuf : public std::streambuf
{
    FilteringInputStreambuf(streambuf * source, bool deleteWhenFinished = false) : mySource(source), myDeleteWhenFinished(deleteWhenFinished)
    {
    }
    
    virtual ~FilteringInputStreambuf()
    {
        resetSource(NULL);
    }
    
    void resetSource(streambuf * newSource, bool deleteWhenFinished = false)
    {
        sync();
        
        if (myDeleteWhenFinished)
        delete mySource;
        
        mySource = newSource;
        myDeleteWhenFinished = deleteWhenFinished;
        setg(NULL, NULL, NULL);
    }
    
    virtual int underflow()
    {
        int result(EOF);
        
        if ( gptr() < egptr() )
        result = static_cast<unsigned char>(*gptr());
        else if ( mySource != NULL )
        {
            result = extract(*mySource);
            
            if (result != EOF)
            {
                assert(result >= 0); // && result <= UCHAR_MAX); // does not work on older machines
                myPushbackBuffer = result;
                setg(&myPushbackBuffer, &myPushbackBuffer, &myPushbackBuffer + 1);
            }
        }
        
        return result;
    }
    virtual int sync()
    {
        int result(EOF);
        
        if (mySource != NULL)
        {
            if (gptr() == egptr() || mySource->sputbackc(*gptr()) != EOF)
            result = mySource->pubsync();
            
            setg( NULL, NULL, NULL ) ;
        }
        
        return result;
    }
    virtual std::streambuf * setbuf(char * buffer, std::streamsize length)
    {
        return mySource == NULL
        ?   static_cast<std::streambuf*>(NULL)
        :   mySource->pubsetbuf(buffer, length);
    }
    
    virtual int extract(std::streambuf & source)
    {
        int ch( source.sbumpc() ) ;
        if ( ch == '#' )
        {
            while ( ch != EOF && ch != '\n' && ch != '\r' )
            {
                ch = source.sbumpc() ;
            }
        }
        if (ch >= 126)
        {
            cerr << "WARNING: Reading from input: " << ch << " which is '" << char(ch) << "'" << endl;
        }
        return ch ;
    }
    
    std::streambuf * mySource;
    char myPushbackBuffer;
    bool myDeleteWhenFinished;    
} ;


class FilteringIstream 
:   private FilteringInputStreambuf
,   public istream
{
    public:

    FilteringIstream( istream& source ):
    FilteringInputStreambuf(source.rdbuf()), istream(this)
    {        
    }
    virtual             ~FilteringIstream(){} ;    
    virtual FilteringInputStreambuf* rdbuf() 
    {
        return this;
    }
     void changeSource(streambuf * newSource)
     {
         resetSource(newSource);
     }
} ;


bool starts_with_flag_expression(string filename)
{
    stringstream ss(filename);

    char ch;
    ss >> ch;

    // m is added for matrix calculator    
    return isdigit(ch) || ch=='-' || ch == '(' || ch == '.' || ch == '{' || ch == '[' || ch == 'm' || ch == 'M';

    // Below is an old version, we need ( for flag calculator
    double number;
    ss >> number;
    return !ss.fail();
}




// This is a macro that creates  istream *istr variable that opened the filename in 
// a smart and nice way. 
//
//     istream *istr = &std::cin; 
#define OPEN_FILE_SMARTLY_BASE(istr, filename, fail_operation) \
    FilteringIstream fscin_XXX(std::cin); \
    istream *istr = &fscin_XXX;\
    stringstream ss_filename_XXX(filename);\
    ifstream infile_XXX; \
    bool file_exists_XXX = false; \
    if (filename != "cin") \
    { \
        infile_XXX.open (filename.c_str(), ifstream::in); \
        if (!infile_XXX.good()) \
        { \
            std::cerr << "Failed opening file " << filename << endl; \
            if (starts_with_flag_expression(filename)) \
            { \
                std::cerr << "Using it as the string itself" << endl; \
                istr = &ss_filename_XXX; \
            } \
            else \
            { \
                std::cerr << "Failed interpreting as flags without comments" << endl; \
                fail_operation; \
            } \
        } \
        else \
        { \
            istr = &infile_XXX; \
            file_exists_XXX = true; \
        } \
    } \
    FilteringIstream infileFilter_XXX(infile_XXX); \
    if (filename != "cin" && file_exists_XXX) \
    { \
        istr = &infileFilter_XXX; \
    } 


#define OPEN_FILE_SMARTLY_RETURN_FALSE_ON_FAIL(istr, filename)  OPEN_FILE_SMARTLY_BASE(istr, filename, return false)
#define OPEN_FILE_SMARTLY(istr, filename)  OPEN_FILE_SMARTLY_BASE(istr, filename, assert(0))

bool load_flags_from_file(string filename, vector<flag> &flag_list,int verbose_output=0)
{
    OPEN_FILE_SMARTLY_RETURN_FALSE_ON_FAIL(istr, filename);
    
 
    if (verbose_output)
        cerr << "Loading labeled flags from file " << filename << endl;
    
    
    flag f;
    while (f.load_from_stream((*istr),-1,-1))
    {
        flag_list.push_back(f);
    }
    
    ///infile.close();
    
    return true;    
}

bool dump_flags_to_file(string filename, vector<flag> &flag_list)
{
    ofstream outfile;
    outfile.open(filename.c_str(), ofstream::out);
    if (!outfile.good())
    {
        cerr << "Failed opening file " << filename << endl;
        return false;
    }
    
    cerr << "Writing flags to file " << filename << endl;

    for (unsigned int x = 0; x < flag_list.size(); x++)
    {
        outfile << flag_list[x].print() << endl;
    }
    
    outfile.close();
    
    return true;
}



bool load_flags_and_coefficients_from_file(string filename, vector<flag_and_coefficient> &flag_list, int verbose_output=0)
{

    OPEN_FILE_SMARTLY_RETURN_FALSE_ON_FAIL(istr, filename);

    if (verbose_output)
        cerr << "Loading labeled flags from file " << filename << endl;
    
    
    flag_and_coefficient fc;
    (*istr) >> fc.coefficient;
    cerr << "Loaded coefficient " << fc.coefficient << endl;
    while ((*istr) && fc.g.load_from_stream((*istr),-1,-1))
    {
        flag_list.push_back(fc);
        (*istr) >> fc.coefficient;
    }
    
    //infile.close();
    
    return true;
}

void dump_flag_and_coefficient(const flag_and_coefficient &fc, bool use_smart_round = false)
{
        if (fc.coefficient == 0) return;
        
        cout.precision(G_PRECISION);
        if (use_smart_round)
        {
            if (smart_round(fc.coefficient) == 0) return;
            cout << smart_round(fc.coefficient);
        }
        else
            cout << fc.coefficient;
        cout << "  " << fc.g.print() << endl;
}

void dump_flags_and_coefficients(vector<flag_and_coefficient> &flag_list, bool use_smart_round = false)
{
    for (unsigned int x = 0; x < flag_list.size(); x++)
    {
        dump_flag_and_coefficient(flag_list[x], use_smart_round);

    }
}



// final_flags means that the flags will not be used any further
// for genertin other flags. Important when combined with G_USE_FIRST_EDGE_COLOR_FOR_BLOWUP_ONLY
bool load_unlabeled_flags_from_file(int sizeKn, bool final_flags = true, bool remove_duplicates_while_loading=false, bool remove_forbidden_wile_loading=false)
{
    stringstream filename;
    
    filename << filename_prefix() << "__n" << sizeKn << "_unlabeled.txt";
    
    ifstream infile;
    infile.open (filename.str().c_str(), ifstream::in);
    if (!infile.good())
    {
        cerr << "Failed opening file with unlabeled flags " << filename.str() << endl;
        return false;
    }

    /*
    for (int i = 0; i < sizeKn; i++)
    {
        if (g_unlabeled_flags[i].size() > 0)
        {
            cerr << "Flags of given size already exists, cannot load again." << endl;
            assert(0);
        }
    }
    */

    cerr << "Loading unlabeled flags from file " << filename.str() << endl;
    
    // For large instances, it really helps to reserve the space in containers
    //  before loading... so we read the file twice
    int sizes_cnt[V], duplicates_cnt[V], forbidden_cnt[V];
    for (int i = 0; i < V; i++)
    {
        sizes_cnt[i] = 0;
        duplicates_cnt[i] = 0;
        forbidden_cnt[i] = 0;
    }
    flag g;
    while (g.load_from_stream(infile,-1,0))
    {
        sizes_cnt[g.m_vertices]++;
    }

    for (int i = 0; i < V; i++) 
    {
        //cerr << "Expecting for size " << i << " flag number: " << sizes_cnt[i] << endl;
        g_unlabeled_flags[i].clear();
        g_unlabeled_flags[i].reserve(sizes_cnt[i]);
    }
    
    //    infile.seek(0);
    infile.close();
    infile.open (filename.str().c_str(), ifstream::in);
    
    
    int tested=0;
    
    while (g.load_from_stream(infile,-1,0))
    {
        
        tested++;
        assert(sizeKn >= g.m_vertices);
        
#ifdef G_USE_FIRST_EDGE_COLOR_FOR_BLOWUP_ONLY
        if (!final_flags) // Drop graphs that contain edges from blow-up
        {
#ifdef G_COLORED_EDGES
            if (g.m_colored_edges[1] > 0) continue;
#endif
#ifdef G_COLORED_3EDGES
            if (g.m_colored_3edges[1] > 0) continue;
#endif
        }
#endif
        if (remove_forbidden_wile_loading && is_flag_forbidden(g))
        {
            forbidden_cnt[g.m_vertices]++;
            continue;
        }
        
        if (remove_duplicates_while_loading)
        {
            volatile bool new_flag = true;
            
            #pragma omp parallel for shared(new_flag)
            for (int i = 0; i < (int)g_unlabeled_flags[g.m_vertices].size();i++)
            {
                
                if (new_flag == false)
                    continue;
                
                if (g.is_isomorphic_to(g_unlabeled_flags[g.m_vertices][i]))
                {
                        #pragma omp atomic write
                    new_flag = false;
                    // This allows to break when a duplicate found - may be usefull to check if the
                    // duplicates are indeed duplicates or if it is a mistake in the program....
                    //if (duplicates_cnt[g.m_vertices] == 2)
                    //{
                    //    cerr << "Found duplicates" << endl;
                    //    cout << g.print() << endl;
                    //    cout << g_unlabeled_flags[g.m_vertices][i].print() << endl;
                    //    exit(0);
                    //}
#ifndef _USING_OMP_                   
                    break;
#endif                    
                }
            }
            if (!new_flag)
            {
                duplicates_cnt[g.m_vertices]++;
                continue;
            }
        }
        g_unlabeled_flags[g.m_vertices].push_back(g);
        //g.reverse_rotation_system();
        //g_unlabeled_flags[g.m_vertices].push_back(g);
        //cerr << g.print() << " " << g_unlabeled_flags[g.m_vertices].size() << " tested " << tested << endl;
    }
    
    infile.close();
    
    for (int i = 0; i <= sizeKn; i++)
    {
        cerr << "Loaded # of unlabeled flags of size " << i << " is " << g_unlabeled_flags[i].size();
        if (remove_forbidden_wile_loading)
        {
            cerr << " and " << forbidden_cnt[i] << " forbiddens";            
        }
        if (remove_duplicates_while_loading)
        {
            cerr << " and " << duplicates_cnt[i] << " duplicates";
        }
        cerr << endl;
    }
    
    
    return true;
}



bool dump_unlabeled_flags(int sizeKn)
{
    stringstream filename;
    filename << filename_prefix() << "__n" << sizeKn << "_unlabeled.txt";
    
    ofstream outfile;
    outfile.open (filename.str().c_str(), ofstream::out);
    if (!outfile.good())
    {
        cerr << "Failed opening file " << filename.str() << endl;
        return false;
    }
    
    cerr << "Writing unlabeled flags to file " << filename.str() << endl;
    
    for (int f = 0; f <= V; f++)
    {
        for (unsigned int x = 0; x < g_unlabeled_flags[f].size(); x++)
        {
            outfile << g_unlabeled_flags[f][x].print() << endl;
        }
    }
    
    outfile.close();
    
    return true;
}


// Creates all subflags of flags of size sizeKn
void generate_all_unlabeled_subflags_from_size(int sizeKn, int verbose_output)
{
    cerr << "Generating smaller unlabeled flags from size " << sizeKn << endl;

    int mapping[V];

    for (int n = sizeKn; n > 0; n--)
    {
        // Delete the ones we are just about to start generating
        g_unlabeled_flags[n-1].clear();
        
        // graph to be processed
        for (int id = 0; id < (int)g_unlabeled_flags[n].size(); id++)
        {
            // vertex to be skipped
            for (int skip=0; skip < n; skip++)
            {
                int mapID = 0;
                for (int u = 0; u < n; u++)
                {
                    if (u == skip) continue;
                    mapping[mapID++] = u;                    
                }
                
                flag F;
                F.as_subflag(g_unlabeled_flags[n][id], mapping, n-1, 0);
                
                volatile bool new_flag = true;
                
                   #pragma omp parallel for shared(new_flag)
                for (int i = 0; i < (int)g_unlabeled_flags[F.m_vertices].size();i++)
                {
                    if (new_flag == false) continue;
                    
                    if (F.is_isomorphic_to(g_unlabeled_flags[F.m_vertices][i]))
                    {
                        #pragma omp atomic write                        
                        new_flag = false;
                        #ifndef _USING_OMP_                   
                        break;
                        #endif 
                    }
                }
                if (!new_flag) continue;
                
                g_unlabeled_flags[F.m_vertices].push_back(F);
            } 
        }
        if (verbose_output)
        {
            cerr << "# of unlabeled flags of size " << n-1 << " is " << g_unlabeled_flags[n-1].size() << endl;
        }
    }
}

#ifdef G_USE_PERMUTATION_GRAPHS
void generate_permutation_graphs(vector<flag> &flag_list, int vertices)
{
    flag g;
    
    g.set_vertices(vertices);
    int perm[g.m_vertices];
    for (int i = 0; i < g.m_vertices; i++) perm[i] = i;
    
    do {
        
        g.set_vertices(g.m_vertices);
        for (int u = 0; u < g.m_vertices; u++)
            for (int v = u+1; v < g.m_vertices; v++)
            {
                if (perm[u]<perm[v]) g.color_edge(u,v,1);
                else g.color_edge(u,v,2);
            }
        
        if (!g_already_in_known_flags(g,flag_list) &&  !is_flag_forbidden(g) ) add_g_to_known_flags(g,flag_list);
        
        
    } while ( std::next_permutation(perm,perm+g.m_vertices) );
}
#endif



void try_extensions_of_g_to_last_vertex_edges(flag &g, vector<flag> &flag_list)
{
#ifdef G_3EDGES
#ifdef G_COLORED_EDGES
//#error I dont know how to generate 2edges and 3edges at the same time.
#endif
#endif
    
    
    int v = g.m_vertices;
    
#if defined(G_MAYBE_ROOTED_KEDGES)
    vector<int> current_edge;
    for (int i = 0; i < G_MAYBE_ROOTED_KEDGES-1; i++)
    {
        current_edge.push_back(G_MAYBE_ROOTED_KEDGES-2-i);
    }
    current_edge.push_back(v-1);
    try_add_maybe_rooted_kedges(g,current_edge,flag_list, true);
    return;
#endif

#if defined(G_ROOTED_4EDGES)     
    try_add_rooted_4edges(g,v-1,0,1,2,flag_list, true);
    return;
#endif

#if defined(G_4EDGES) && !defined(G_4EDGES_HACK)       
    try_add_4edges(g,v-1,0,1,2,flag_list, true);
    return;
#endif
#ifdef G_4EDGES_HACK
    try_add_4edges_hack(g,v-1,0,1,2,flag_list);
    #ifdef G_COLORED_VERTICES
    //if (g.m_color_vertex[v-1] != 1) return; // here we are trying to make K_{4,4}
    //    try_add_4edges_hack_Y(g,v-1,0,1,2,flag_list);
    #endif
    return;
#endif    
#ifdef G_ROOTED_3EDGES
    try_add_rooted_3edges(g,v-1,0,1,flag_list, true);
    return;
#endif
#ifdef G_3EDGES
    try_add_3edges(g,v-1,0,1,flag_list, true);
    return;
#endif

#ifdef G_COLORED_3EDGES
    try_color_3edges(g,v-1,0,1,flag_list, true);
    return;
#endif
#ifdef G_COLORED_EDGES
    try_color_edge(g,v-1,0,flag_list);
    return;
#endif

#ifdef G_ROTATION_SYSTEM
    if (is_flag_forbidden(g)) return; 
    
    g.create_minlex_signature();
        
    if (!g_already_in_known_flags(g,flag_list)) add_g_to_known_flags(g,flag_list);
    return;
#endif    

#ifdef G_ORDER_TYPES
    cerr << "This program cannot generate order types" << endl;
    assert(0);
#endif 

#ifdef G_CROSSINGS
    cerr << "This program simply generate crossings" << endl;
    assert(0);
#endif 

    v++; // this is to avoid warning of unused v
    assert(0);
}


void try_extensions_of_g_to_last_vertex(flag &g, vector<flag> &flag_list)
{
#ifdef G_COLORED_VERTICES
    int v = g.m_vertices;
    
    if ((int)g_vertex_color_pattern.size() > v-1) {
        //cerr << "USing it" << g_vertex_color_pattern[v-1] << endl;
        g.color_vertex(v-1,g_vertex_color_pattern[v-1]);
        try_extensions_of_g_to_last_vertex_edges(g, flag_list);
        return;
    }    
    
    for (int c = 1; c < COLORS_VERTICES; c++)
    {
        //if (g.m_colored_vertices[c]+1 > 5) 
        //{
        //    continue;
        //}        
        g.color_vertex(v-1,c);
        try_extensions_of_g_to_last_vertex_edges(g, flag_list);
    }
#else
    try_extensions_of_g_to_last_vertex_edges(g, flag_list);
#endif
}


#ifdef G_ROTATION_SYSTEM

// for each size, we have a vector of rotations
// Bonus feature: 0 is always first one
// Also, notice that rotations for n do NOT include n itself
vector< vector<int> > g_all_rotations[V];

int rotation_at_a_is_even(int a, int b, int c, int n, flag &f)
{
    assert(b < c);
    assert(c < n);
    
    int rotation=0;
    for( int i  = 0; i < f.m_rotation_system_neighbors[a]; i++)
    {
        if (f.m_rotation_system[a][i] == b) rotation = 10*rotation+1;
        if (f.m_rotation_system[a][i] == c) rotation = 10*rotation+2;
        if (f.m_rotation_system[a][i] == n) rotation = 10*rotation+3;
    }
        
    if (rotation == 123) return 1;
    if (rotation == 231) return 1;
    if (rotation == 312) return 1;
    
    return 0;
}

bool is_rotation_at_ID_of_size_n_even(int ID, int a, int b, int c,  int n)
{
    assert(a < b);
    assert(b < c);
    
    assert((int)g_all_rotations[n].size() > ID);
    
    int rotation=0;
    for( int i  = 0; i < (int)g_all_rotations[n][ID].size(); i++)
    {
        if (g_all_rotations[n][ID][i] == a) rotation = 10*rotation+1;
        if (g_all_rotations[n][ID][i] == b) rotation = 10*rotation+2;
        if (g_all_rotations[n][ID][i] == c) rotation = 10*rotation+3;
    }
    
    if (rotation == 123) return true;
    if (rotation == 231) return true;
    if (rotation == 312) return true;

    return false;
}


// Here we get three vertices a,b,c and a set of available rotations. We try to remove
// those that are NOT compatible with a < b < c, where the new vertex is n
// expected that list at a,b,c in f are valid
void remove_unavailable_rotations(int a, int b, int c, int n, set<int>& available_rotations, flag &f)
{    
    assert(a < b);
    assert(b < c);
    assert(c < n);
    
    // Sum of even rotations must be even
    int rotations = rotation_at_a_is_even(a, b, c, n, f) + rotation_at_a_is_even(b, a, c, n, f) + rotation_at_a_is_even(c, a, b, n, f);    
    
    bool desired_is_even;
    if (rotations == 1 || rotations == 3)
    {
        // The remaining rotations in available_rotations must be even
        desired_is_even = true;
        
    }
    else
    {
        // remaining rotations in available_rotations must be odd
        desired_is_even = false;        
    }
    
    
    //cerr << "Removing rotations for " << f.print() << " rotations=" << rotations << " desired_even=" << desired_is_even << endl;

    
    set<int> new_available_rotations;
    
#ifdef DONT_USE_C11    
    std::set<int>::iterator it;
    for (it = available_rotations.begin(); it != available_rotations.end(); ++it)
    {
        int ID = *it;
#else
    for (auto ID : available_rotations)
    {
#endif        
        if (is_rotation_at_ID_of_size_n_even(ID, a, b, c, f.m_vertices) == desired_is_even)
        {
            new_available_rotations.insert(ID);
        }
#ifdef DONT_USE_C11           
    }
#else
    }
#endif        
    available_rotations = new_available_rotations;
}

void calculate_small_rotations(int n)
{
    if ( g_all_rotations[n].size() > 0)
        return;
    
    if (n == 1)
    {
        vector<int> v;
        g_all_rotations[1].push_back(v);
        return;
    }
    
    // generating all rotations
    int small_ints[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    std::sort (small_ints,small_ints+n-1);
    
    do {
        vector<int> v(small_ints, small_ints+n-1);
        g_all_rotations[n].push_back(v);
    } while ( std::next_permutation(small_ints+1,small_ints+n-1) );    
    
    cerr << "Generated " << g_all_rotations[n].size() << " rotations." << endl;
}

// Tests if f is realizable
bool is_realizable_test_on_5(flag &f)
{
    if (f.m_vertices <= 5) return true;
        
    int mapping[5];
    mapping[4] = f.m_vertices-1;
    
    // Test if the whole thing is realizable by taking all subgraphs on 5
    // vertices and testing if each is isomorphic to something in the list
    // To speed up the test we always include the LAST vertex since the
    // previous ones were already tested....
    for (mapping[0] = 0;            mapping[0] < f.m_vertices-4; mapping[0]++)
    for (mapping[1] = mapping[0]+1; mapping[1] < f.m_vertices-3; mapping[1]++)
    for (mapping[2] = mapping[1]+1; mapping[2] < f.m_vertices-2; mapping[2]++)
    for (mapping[3] = mapping[2]+1; mapping[3] < f.m_vertices-1; mapping[3]++)
    {
        flag tmp;        
        tmp.as_subflag(f, mapping, 5, 0);
        
        //cerr << "Testing " << tmp.print() << endl;
        
        if (!g_already_in_known_flags(tmp, g_unlabeled_flags[5])) return false;        
    }
    
    return true;
}

void try_insert_new_vertex_in_rotations(flag &g, flag &f, vector<flag> &flag_list, set<int>& available_rotations, int v)
{
    //cerr << "size == " << available_rotations.size() << endl;
    
    if (available_rotations.size() == 0)
    {
        //cerr << "No rotations available" << endl;
        return;
    }
    
    if (v >= g.m_vertices)
    {
        //if (available_rotations.size() > 1)
         //   cerr << "Rotations many left " << available_rotations.size() << endl;
        
        // Test all rotations
#ifdef DONT_USE_C11    
        std::set<int>::iterator it;
        for (it = available_rotations.begin(); it != available_rotations.end(); ++it)
        {
            int ID = *it;
#else        
        for (auto ID : available_rotations)
        {
#endif
            
            //cerr << it << endl;
            
            assert((int)g_all_rotations[f.m_vertices].size() > ID);
            
            for (int j = 0; j < (int)g_all_rotations[f.m_vertices][ID].size(); j++ )
            {
                //cerr << "dbg: " << g_all_rotations[f.m_vertices][it][j] << endl;
                f.m_rotation_system[v][j] = g_all_rotations[f.m_vertices][ID][j]; 
            }
            f.m_rotation_system_neighbors[v] = g_all_rotations[f.m_vertices][ID].size();
            
            if (!is_realizable_test_on_5(f)) continue;
            
            //cerr << "Found " <<  f.print()  << endl; 
            f.rotation_systems_count_noncrossings();
            
            try_extensions_of_g_to_last_vertex(f, flag_list);
        }
                
        
        return;
#ifdef DONT_USE_C11              
    }
#else
    }
#endif
    // index of last vertex
    int n = f.m_vertices-1;
    
    // Try all insertions at vertex v
    // and do NOT put it last - it would make the lists rotations the same....
    //
    int insert_up_to = g.m_rotation_system_neighbors[v];
    if (insert_up_to < 1) insert_up_to = 1;
    //cerr << insert_up_to << endl;

    f.m_rotation_system_neighbors[v] = g.m_rotation_system_neighbors[v]+1; 
    for (int i = 0; i < insert_up_to; i++)
    {
        // Inserting n into the list to the right position
        for (int j = 0; j < i; j++)
            f.m_rotation_system[v][j] = g.m_rotation_system[v][j];
        f.m_rotation_system[v][i] = n;
        for (int j = i; j < g.m_rotation_system_neighbors[v]; j++)
            f.m_rotation_system[v][1+j] = g.m_rotation_system[v][j];
        
        // Update available_rotations
        set<int> new_available_rotations = available_rotations;
        for (int a = 0; a < v-1; a++)
            for (int b = a+1; b < v; b++)
            {
                remove_unavailable_rotations(a, b, v, n, new_available_rotations, f);
            }
        
        // Call next 
        try_insert_new_vertex_in_rotations(g, f, flag_list, new_available_rotations, v+1);        
    }
}


void try_rotation_extensions_of_g_by_adding_vertex(flag &g, vector<flag> &flag_list)
{
    
    int n = g.m_vertices+1;

    flag f;
    f.set_vertices_and_Theta(n,0);
    
    // At the beginning, say all rotations are available    
    set<int> available_rotations;
    for (int i = 0; i < (int) g_all_rotations[n].size(); i++)
    {
        available_rotations.insert(i);
    }
 
    if (n == 5)
    {
        cerr << "WARNING! This program is not really able to generate rotations on 5 vertices reliably" << endl;
        cerr << "It does not check if they are actually realizable. The list on 5 is used to check graphs" << endl;
        cerr << "on bigger n for being realizable." << endl;
        cerr << "As a courtesy, here are lists all realizable 5-rotations\n"
        "5 0     1:2345 2:1345 3:1245 4:1235 5:1234\n"   
        "5 0     1:2345 2:1534 3:1254 4:1235 5:1432\n"
        "5 0     1:2345 2:1435 3:1245 4:1325 5:1234\n"   
        "5 0     1:2345 2:1354 3:1245 4:1253 5:1423\n"   
        "5 0     1:2345 2:1354 3:1254 4:1523 5:1234\n" 
#ifndef G_ROTATION_SYSTEM_REVERSE_BLIND
        "5 0     1:5432 2:4531 3:4521 4:3251 5:4321\n"
#endif        
        "As a courtesy, here are lists all convex 5-rotations \n"
        "5 0     1:2345 2:1345 3:1245 4:1235 5:1234\n"  
        "5 0     1:2345 2:1534 3:1254 4:1235 5:1432\n"  
        "5 0     1:2345 2:1435 3:1245 4:1325 5:1234\n" 
        "extra bonus, for hereditary convex rotations, one needs to forbid\n"
        "6 0  1:26345 6:25341 2:16345 3:12645 4:12635 5:12346"
        << endl;
        exit(0);
    }
    
    // Start inserting last vertex into the previous lists...
    try_insert_new_vertex_in_rotations(g, f, flag_list, available_rotations, 0);
}
#endif


void dump_vector_of_flags(vector <flag> *flag_lists, int previous_size, int sizeKn)
{
    stringstream filename;
    filename << filename_prefix() << "__n" << sizeKn << "_unlabeled_dump.txt";
    
    ofstream outfile;
    outfile.open (filename.str().c_str(), ofstream::out);
    if (!outfile.good())
    {
        cerr << "Failed opening file " << filename.str() << endl;
        return;
    }
    
    // dumping of flag lists...
    for (int i = 0; i < previous_size; i++)
    {
        for (int f = 0; f < (int)flag_lists[i].size(); f++)
        {
            outfile << flag_lists[i][f].print() << endl;
        }
        outfile << endl;
    }
    
    outfile.close();
}



// The following function megres dest_flags with src_flags and clears src_flags
void merge_flags_lists_parallel(vector<flag> &dest_flags, vector<flag> &src_flags)
{
    
    //    cout << flag_list.size() << endl;
    
    vector<flag> new_flags;
    new_flags.reserve(src_flags.size());
    
#pragma omp parallel for
    for (int k = 0; k < (int)src_flags.size(); k++)
    {
        if (!g_already_in_known_flags(src_flags[k],dest_flags))
        {
#pragma omp critical (merging_lists)
            {
                new_flags.push_back(src_flags[k]);
            }
        }
    }
    //#ifdef DONT_USE_C11
    dest_flags.insert(dest_flags.end(), new_flags.begin(), new_flags.end() );
    src_flags.clear();
    vector<flag>().swap( src_flags ); // free the memory
    //#else
    //    dest_flags.insert(dest_flags.end(), std::make_move_iterator(new_flags.begin()), std::make_move_iterator(new_flags.end()) );
    //    src_flags.clear();
    //    src_flags.shrink_to_fit(); // free the memory - does not work well on icc
    //#endif  
}


// The following function megres dest_flags with src_flags and clears src_flags
template< class data_type >
void merge_vectors_parallel(vector<data_type> &dest_vector, vector<data_type> &src_vector)
{    
    vector<data_type> new_vector;
    new_vector.reserve(src_vector.size());
    
#pragma omp parallel for
    for (int k = 0; k < (int)src_vector.size(); k++)
    {
        bool already_exists = false;
        for (const auto& test : dest_vector)
        {
            if (test == src_vector[k])
            {
               already_exists = true; 
            }
        }
        if (already_exists == false)
        {
#pragma omp critical (merging_lists)
            {
                new_vector.push_back(src_vector[k]);
            }
        }
    }
    //#ifdef DONT_USE_C11
    dest_vector.insert(dest_vector.end(), new_vector.begin(), new_vector.end() );
    src_vector.clear();
    vector<data_type>().swap( src_vector ); // free the memory
    //#else
    //    dest_flags.insert(dest_flags.end(), std::make_move_iterator(new_flags.begin()), std::make_move_iterator(new_flags.end()) );
    //    src_flags.clear();
    //    src_flags.shrink_to_fit(); // free the memory - does not work well on icc
    //#endif  
}

//#define CROSSING_EXTENSIONS_COLOR 1

// We expect v has highest index of all
void try_crossings_extension_of_g_add_rest(flag &g, vector<flag> &flag_list, int v, int u, int x, int y)
{
    if (u >= v)
    {
        // test, we generated it all
        return;
    } 

    if (x >= v)
    {
        try_crossings_extension_of_g_add_rest(g, flag_list, v, u+1, 1, 0);
        return;
    }

    if (x == u)
    {
        try_crossings_extension_of_g_add_rest(g, flag_list, v, u, x+1, 0);
        return;
    }

    if (x == u)
    {
        try_crossings_extension_of_g_add_rest(g, flag_list, v, u, x+1, 0);
        return;
    }

    //TODO!!!
    assert(0);
}

/*
    void try_crossings_extension_of_g_by_adding_vertex(flag &g, vector<flag> &flag_list)
{    
    int n = g.m_vertices+1;
    int v = n-1;
    flag f;
    f.set_vertices_and_Theta(n,0); 
    f.copy_from(g);
#ifdef CROSSING_EXTENSIONS_COLOR
    f.color_vertex(v, CROSSING_EXTENSIONS_COLOR);
#else
    #ifdef G_COLORED_VERTICES
    #error The color fo extension needs to be present
    #endif
#endif

    // Now we try to add the remaining crossings...
}
*/
 
 
void generate_unlabeled_flags_of_size(int i, int verbose_output, bool dump_unlabeled_while_generating)
{
#ifdef G_USE_PERMUTATION_GRAPHS
    generate_permutation_graphs(g_unlabeled_flags[i],i);
    return;
#endif

#ifdef G_ROTATION_SYSTEM    
    calculate_small_rotations(i);
#endif 
     
    int previous_size = (int)g_unlabeled_flags[i-1].size();
    
    //vector <flag> flag_lists[previous_size]; // does not work on mac (llvm :-( )
    vector <flag> *flag_lists;
    flag_lists = new vector<flag>[previous_size];
    if (flag_lists == NULL)
    {
        cerr << "Memory allocation failed" << endl;
        exit(1);
    }
    
    if ( (int)g_unlabeled_flags[i-1].size() < 1 )
    {
        cerr << "No flags of size " << i-1 << endl;
        cerr << "Generating flags of size " << i << " failed." << endl;
        exit(0);
    }
    
    //            #pragma omp parallel for
#pragma omp parallel for ordered schedule(dynamic)
    for (int j = 0; j < (int)g_unlabeled_flags[i-1].size(); j++)
    {
        flag_lists[j].reserve(previous_size);

        
#ifdef G_ROTATION_SYSTEM
        try_rotation_extensions_of_g_by_adding_vertex(g_unlabeled_flags[i-1][j], flag_lists[j]);
#else        
        flag g;
        g.set_vertices(i);
        g.copy_from(g_unlabeled_flags[i-1][j]);
        //vector <flag> valid_extensions_of_g;
        //cout << "going for " << j << endl;
        try_extensions_of_g_to_last_vertex(g, flag_lists[j]);
#endif
        if (verbose_output)
        {
            cerr << "generated size " << i << ": " << j  << "/" << (int)g_unlabeled_flags[i-1].size() << " found " << flag_lists[j].size() << " flags " << endl;
        }
    }
    if (verbose_output)
    {
        cerr << "Flags generated. Merging part starts." << endl;
    }
    
    if (dump_unlabeled_while_generating)
        dump_vector_of_flags(flag_lists, previous_size, i);

    for (int power = 1; power < previous_size; power *= 2)
    {
        if (verbose_output)
        {
            cerr << "Trying " << power << " in " << previous_size << endl;
        }
        for (int j = 0; j < previous_size; j += 2*power)
        {
            if (j+power < previous_size)
            {
                if (verbose_output)
                {
                    cerr << "Merging  " << j << "<-" << j+power << "  of sizes " <<  flag_lists[j].size() << " " << flag_lists[j+power].size() << endl;
                }
                merge_flags_lists_parallel(flag_lists[j],flag_lists[j+power]);
                //flag_lists[j+power].clear();
            }
        }
        if (dump_unlabeled_while_generating)
            dump_vector_of_flags(flag_lists, previous_size, i);
    }
    
    g_unlabeled_flags[i].swap(flag_lists[0]);
    
    delete[] flag_lists;
}


void get_unlabeled_flags_of_size(int Kn, bool force_generate_flags = false, bool remove_duplicates_while_loading = false, bool remove_forbidden_wile_loading = false, int verbose_output = 0, bool dump_unlabeled_while_generating= false)
{
    // Already loaded
    if (g_unlabeled_flags[Kn].size() > 0)
    {
        return;
    }
    
    // Getting unlabeled flags...
    if (force_generate_flags || !load_unlabeled_flags_from_file(Kn,true,remove_duplicates_while_loading, remove_forbidden_wile_loading))
    {
#ifdef G_CROSSINGS
        cerr << "Cannot generate unlabeled flags. Must be provided." << endl;
        assert(0);
        return;
#endif
        // Try to reuse previous graphs...
        int loaded_size = 0;
        
        // blow-upping and generating would not work correctly...
        if (!force_generate_flags)
        {
            loaded_size = Kn-1;
            cerr << "Trying to load file with smaller flags..." << endl;
            while (loaded_size > 0 && !load_unlabeled_flags_from_file(loaded_size,false)) loaded_size--;
        }
        
        if (loaded_size == 0)
        {
            cerr << "Unable to load any smaller graphs. Starting from empty graph." << endl;
            flag g_zero;
            g_zero.set_vertices_and_Theta(0,0);
            assert(g_unlabeled_flags[0].size() == 0);
            g_unlabeled_flags[0].push_back(g_zero);
        }
        
        cerr << "Generating unlabeled flags of sizes " << loaded_size+1 << " to " << Kn << endl;
        
        for (int i = loaded_size+1; i <= Kn; i++)
        {
            generate_unlabeled_flags_of_size(i, verbose_output, dump_unlabeled_while_generating);
            cerr << "Unlabbeled flags of size "<< i << ": " << g_unlabeled_flags[i].size() << endl;
        }
        
#ifdef G_USE_FIRST_EDGE_COLOR_FOR_BLOWUP_ONLY
        // Create blow-ups
        for (int i = 1; i <= Kn; i++)
        {
            cerr << "Before blow-up unlabbeled flags of size "<< i << ": " << g_unlabeled_flags[i].size() << endl;
            generate_blowups_by_one_vertex(g_unlabeled_flags[i-1], g_unlabeled_flags[i]);
            cerr << "After blow-up unlabbeled flags of size "<< i << ": " << g_unlabeled_flags[i].size() << endl;
        }
#endif
        
        dump_unlabeled_flags(Kn);
    }

    if ( (int)g_unlabeled_flags[Kn].size() < 1 )
    {
        cerr << "No flags of size " << Kn << endl;
        cerr << "Generating flags of size " << Kn << " failed." << endl;
        exit(0);
    }

#ifdef G_USE_INDEXES_FOR_LARGE_UNLABELED_FLAGS
    for (int i = 0; i < (int)g_unlabeled_flags[Kn].size(); i++)
        g_unlabeled_flags[Kn][i].m_unlabeled_id = i;
#endif    

    /*
    // And assert test
    for (int sz = 0; sz < Kn; sz++)
    {
        for (int i = 0; i < (int)g_unlabeled_flags[sz].size(); i++)
            for (int j = i+1; j < (int)g_unlabeled_flags[sz].size(); j++)
            {
                if (g_unlabeled_flags[sz][i].is_isomorphic_to(g_unlabeled_flags[sz][j]))
                {
                    cerr << endl;
                    cerr << "ERROR while generating flags of size " << Kn << " got for size " << sz << " identical flags" << endl;
                    cerr << "i=" << i << ": " << g_unlabeled_flags[sz][i].print() << " j=" << j << ": " << g_unlabeled_flags[sz][j].print() << endl;
                    assert(0); 
                }
 
            }
    }
    */
}


// This should make a cash of loaded flags and have the things a little faster
unordered_map<string, vector<flag> > g_labeled_flags_of_one_type_map;

inline string get_filename_for_labeled_flags(int flag_size, const flag &type)
{
    // Saving of the flags to a separate file and/or loading them
    return filename_prefix() + "__size"+to_string((long long)flag_size)+"_type"+ type.print("")+".txt";
}

bool labeled_flags_of_one_already_exist(int flag_size, const flag &type)
{
    string filename = get_filename_for_labeled_flags(flag_size,type);

    // if it is already in the map, it exists
    if (g_labeled_flags_of_one_type_map.find(filename) != g_labeled_flags_of_one_type_map.end())
        return true;

    vector<flag> flag_list;
    if (load_flags_from_file(filename,flag_list))
    {
        g_labeled_flags_of_one_type_map.insert(make_pair(filename, flag_list));
        return true;
    }
    
    return false;
}


void generate_next_map(int type_size, int flag_size, int id, vector<int> &current_map,  vector<bool> &used_in_map, vector<vector<int> > &mappings_to_try)
{
    // mapping found
    if (id >= flag_size)
    {
        mappings_to_try.push_back(current_map);
        return;
    }

    // finish mapping of unlabeled vertices in any order
    if (id >= type_size)
    {
        for (int i = 0; i < flag_size; i++)
        {
            if (used_in_map[i] == false)
            {
                used_in_map[i] = true;
                current_map[id] = i; 
                generate_next_map(type_size, flag_size, id+1, current_map,  used_in_map, mappings_to_try);
                used_in_map[i] = false;
                return;
            }
        }
        assert(0);
    }

    // Try all possibilities for labeled vertices
    for (int i = 0; i < flag_size; i++)
    {
        if (used_in_map[i] == false)
        {
            used_in_map[i] = true;
            current_map[id] = i; 
            generate_next_map(type_size, flag_size, id+1, current_map,  used_in_map, mappings_to_try);
            used_in_map[i] = false;
        }
    }
}

void generate_maps(int type_size, int flag_size, vector<vector<int> > &mappings_to_try)
{
    vector<int> current_map;
    vector<bool> used_in_map;
    for (int i = 0; i < flag_size; i++)
    {
        current_map.push_back(i);
        used_in_map.push_back(false);        
    }
    generate_next_map(type_size, flag_size, 0, current_map,  used_in_map, mappings_to_try);
}


// Not paralel version - very slow and not completely correct - in particular for ordered version.
void generate_labeled_flags_of_one_type(int flag_size, const flag &type, vector<flag> &flag_list)
{


    cerr << "Generating labeled flags of size " << flag_size << " of type " << type.print() << endl;

    string filename = get_filename_for_labeled_flags(flag_size,type);
                    
    // Make sure we have the unlabeled flags of right size loaded
    get_unlabeled_flags_of_size(flag_size);
    
    int to_label_size = (int)g_unlabeled_flags[flag_size].size();
    //cerr << "Type size " << type_size << " for " << g_unlabeled_flags[flag_size][i].print() << endl;
    int mapping[flag_size];
    flag F;
    
    int type_size = type.labeled_vertices_cnt();
    
    if (type_size != 0)
    {
#ifdef G_ORDERED_VERTICES
//    cerr << "Not implemented for ordered vertices" << endl;
    assert(0);
#endif    
        vector<vector<int> > mappings_to_try;
        generate_maps(type_size, flag_size, mappings_to_try);

        vector<flag> found_already;
        for (int i = 0; i < to_label_size; i++)
        {
            found_already.clear();
            //if (verbose_output)
            //     cerr << "Flag size "<<flag_size << " type size " << type_size << " labeling " << i << "/" << to_label_size << endl;
        
            //for (int j = 0; j < flag_size; j++) mapping[j]=j;
        
            //do {
            for (int mapID = 0; mapID < (int)mappings_to_try.size(); mapID++)
            {
                for (int j = 0; j < flag_size; j++) mapping[j]=mappings_to_try[mapID][j];

                F.as_subflag(g_unlabeled_flags[flag_size][i], mapping, flag_size, type_size); // taking a subflag
                //cerr << "Found " << F.print() << " X " << F.have_same_type(type) << " " << !find_flag_in_list(F, flag_list) << endl;
                //if (F.have_same_type(type) && find_flag_in_list(F, flag_list) == -1)
                //if (F.have_same_type(type) && find_flag_in_list_nonparalel(F, flag_list) == -1)
                if (F.have_same_type(type) && find_flag_in_list_nonparalel(F, found_already) == -1)
                {
                    found_already.push_back(F);
                    flag_list.push_back(F);
                }
                //cerr << "x";
            }
            //} while ( std::next_permutation(mapping,mapping+flag_size) );
            //cerr << endl;
            //cerr << "found_already.size()=" << found_already.size() << " flag_list.size()=" << flag_list.size() << endl;
        }
    }
    else
    {
        //cerr << "Bubu " << flag_size << " "  << g_unlabeled_flags[flag_size].size() << endl;
        //flag_list.erase();
        flag_list = g_unlabeled_flags[flag_size];
    }    

    if (dump_flags_to_file(filename,flag_list))
    {
        cerr << "Generated and written flags to " << filename << endl;
    } 
}

// Not paralel version - very slow and not completely correct - in particular for ordered version.
void get_labeled_flags_of_one_type(int flag_size, const flag &type, vector<flag> &flag_list)
{
#ifdef G_ORDERED_VERTICES
//    cerr << "Not implemented for ordered vertices" << endl;
//    assert(0);
#endif
    
    //cerr << "XXXXX" << endl;

    // Saving of the flags to a separate file and/or loading them
    string filename = get_filename_for_labeled_flags(flag_size,type);

    //umap.insert(make_pair("e", 2.718)); 
 
    if (g_labeled_flags_of_one_type_map.find(filename) == g_labeled_flags_of_one_type_map.end()) 
    {
        #pragma omp critical (generating_special_labeled_flags)
        {   
            if (g_labeled_flags_of_one_type_map.find(filename) == g_labeled_flags_of_one_type_map.end())
            {
                //cerr << "About to start loading " << filename << endl;
                if (load_flags_from_file(filename,flag_list))
                {
                    cerr << "Loaded " <<  flag_list.size() << " flags from " << filename << endl;
                }
                else
                {
                    cerr << "Generating flags for file " << filename << endl;
                    generate_labeled_flags_of_one_type(flag_size, type, flag_list);   
                }
                    
                g_labeled_flags_of_one_type_map.insert(make_pair(filename, flag_list));
            }
        }   
    }
    else
    {
        //cerr << "Type already exists" << endl;
    }


    assert(g_labeled_flags_of_one_type_map.find(filename) != g_labeled_flags_of_one_type_map.end());

    flag_list = g_labeled_flags_of_one_type_map[filename];
}



double unlabel_F1(const flag &F1, int remaining_labeled)
{
#ifdef G_ORDERED_VERTICES
    assert(0);
#endif
    
    flag H = F1;
    H.m_Theta = remaining_labeled;
    
    return P_F1_IN_H(F1, H);
}

vector<flag_and_coefficient> F1_times_F2(const flag &F1, const flag &F2, const flag &type)
{
    
    vector<flag_and_coefficient> combination;

    // Here is a little hack, a flag that has 0 vertices is treated as a constant
    if (F1.m_vertices == 0) 
    {
        flag_and_coefficient fc;
        fc.g = F2;
        fc.coefficient = 1;
        combination.push_back(fc);
        return combination;
    }

    if (F2.m_vertices == 0) 
    {
        flag_and_coefficient fc;
        fc.g = F1;
        fc.coefficient = 1;
        combination.push_back(fc);
        return combination;
    }

    // do something with ID's
    assert(F1.have_same_type(F2));
    
    int labeled_vertices_cnt = F1.labeled_vertices_cnt();
    int result_size = F1.m_vertices + F2.m_vertices - labeled_vertices_cnt;

    //cout << labeled_vertices_cnt << " " << result_size << endl;
     
    vector<flag> big_list;

    get_labeled_flags_of_one_type(result_size, type, big_list);
    
    //cout << big_list.size() << endl;

    
    for (int i = 0; i < (int)big_list.size(); i++)
    {
        double d = P_F1_F2_IN_labeled_H(F1,F2,big_list[i]);
        if (d != 0)
        {
            flag_and_coefficient fc;
            fc.g = big_list[i];
            fc.coefficient = d;
            combination.push_back(fc);
        }
    }

    return combination;
}





void fc_add_FC_to_first(vector<flag_and_coefficient> &FC, vector<flag_and_coefficient> &FC_add, double coeff = 1)
{
    for (int j = 0; j < (int)FC_add.size(); j++)
    {
        int found_ID = -1;
        for (int k = 0; k < (int)FC.size(); k++)
        {
            if (FC_add[j].g.is_isomorphic_to(FC[k].g))
            {
                found_ID = k;
                break;
            }
        }
        if (found_ID == -1)
        {
            flag_and_coefficient fc = FC_add[j];
            fc.coefficient *= coeff;
            FC.push_back(fc);
            
        }
        else
        {
            FC[found_ID].coefficient += coeff*FC_add[j].coefficient;
        }
    }
}

void multiply_FC_by_C(vector<flag_and_coefficient> &FC, double c)
{
    for (int i = 0; i < (int)FC.size(); i++)
    {
        FC[i].coefficient *= c;
    }
}

void fc_F1_times_F2(vector<flag_and_coefficient> &F1, vector<flag_and_coefficient> &F2, vector<flag_and_coefficient> &F1F2sum)
{
    F1F2sum.clear();

    //cerr << "Sizes for product " << F1.size() << " "  << F2.size() << endl;

    for (int i = 0; i < (int)F1.size(); i++)
    {
        for (int j = 0; j < (int)F2.size(); j++)
        {
            //cerr << "Multiplying " << i << " " << j << endl;
            flag type;
            F1[i].g.get_type_subflag(type);
            vector<flag_and_coefficient> F1F2 = F1_times_F2(F1[i].g, F2[j].g, type);
            
            //cerr << "F1F2 size " <<  F1F2.size() << endl;
            
            multiply_FC_by_C(F1F2, F1[i].coefficient*F2[j].coefficient);
            
            fc_add_FC_to_first(F1F2sum,F1F2);
        }
    }
}


void fc_unlabel_in_place(vector<flag_and_coefficient> &F)
{
    
#pragma omp parallel for ordered schedule(dynamic)         
    for (int i = 0; i < (int)F.size(); i++)
    {
        flag_and_coefficient fc;
        
        fc.g = F[i].g;
        fc.g.m_Theta = 0;
        fc.coefficient = P_F1_IN_H(F[i].g, fc.g)*F[i].coefficient;

        F[i].g = fc.g;
        F[i].coefficient = fc.coefficient;
                
    }

    vector<flag_and_coefficient> FU;
    fc_add_FC_to_first(FU,F);

    F=std::move(FU);
}


void help_extended_grammar()
{
    cout << R"( Extended expresions follow grammar below 
   
Expression := Term [ ('+' | '-') Term ]* [ ';' | '@' n   ] \n
Term       := Factor [  '*' Factor ]* \n
Factor     := Power ['^'k] \n
Power      := CFlag | '(' Expression ')' | '[' Expression ']' | [-]CExpression  '(' Expression ')[^k]'  \n 
CFlag      := [-]CExpression  Flag |  'M' MExpression 'X'   (MExpression is 1x1 matrix)  \n
Flag       := Bunch of Digits usually, may contain  . sometimes |  FFile \n
Digit      := '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' | '.' \n
\n
CExpression := CTerm [ ('+' | '-') CTerm ]* \n
CTerm       := CFactor [ ( '*' | '/' ) CFactor ]* \n
CFactor     := CPower ['^' CExpression] \n
CPower      := CNumber | '{' Expression '}' 
CNumber     := [-]Bunch of Digits inclduing .  
FFile       := filename starting with 'F' containing coefficients & flags
\n
MExpression := MTerm [ ('+' | '-') MTerm ]* \n
MTerm       := MFactor [ '*'  MFactor ]* \n
MFactor     := MPower ['^T']   (Only transpose currently) \n 
MPower      := 'mf' [  Expression  [ ','|'&' Expression  ]*    ['\'|'\\']  ]*  'x'
            := 'mc' [  CExpression [ ','|'&' CExpression ]*    ['\'|'\\']  ]*  'x'
Notes:
 ; in Expression is useful to split it from the next constraint if constraint starts with -\n
 [ ]  unlabeling of the expression inside 
 @n  make the expression on n (unlabeled!) vertices. Useful to collect graphs on
     different number of vertices. It should do unlabeling if needed. 
 { } are used in CExpression  (coefficients for flags) instead of ( ) to avoid\n
     confusion. For example  
     2 1 0  + (3+2)/4 1  0 
    would not work. Correct: 2 1 0  + {3+2}/4 1  0 
 M .. X  can calculate with matrices but the result must be 1x1 matrix to be able to use it further
 mc .. x is a matrix of only coefficients. It can be seen as flag  with 0 vertices (they are compatible
    with any type most of the time)   
 
Warning, fun things with + and * may result in unintuitive results like 
  1+3*4/2 (1 2 0 2)^2 
  is actually 
  {1+3*4/2} (1 2 0 2)^2  
Also things like
  4 (1 2 0 2)^2  or 4 [1 2 0 2]  work while 4 * (1 2 0 2)^2  or 4 * [1 2 0 2] do not work.


)";
}
// See for implementation 
// https://codereview.stackexchange.com/questions/54273/simple-c-calculator-which-follows-bomdas-rules



bool fc_cexpression(istream *istr, double &result, ostream *ostr=NULL, stringstream *debug_ss=NULL);
void fc_expression(istream *istr, vector<flag_and_coefficient> &result, ostream *ostr, stringstream *debug_ss=NULL);
void fc_power(istream *istr, vector<flag_and_coefficient> &result, ostream *ostr, stringstream &debug_ss);
void fc_mexpression(istream *istr, vector<vector<vector<flag_and_coefficient> > > &result, ostream *ostr, stringstream *debug_ss);


char fc_token(istream *istr) 
{ 
    //int poistion = istr->tellg();
    //cerr << "fc_token  Rading: " << istr->tellg() <<  "; " << istr->rdbuf() << endl;
    //istr->seekg(poistion);

    char ch;
    (*istr) >> ch;

    //cerr << "fc_token: " << ch << endl;
    return ch;
}

bool fc_cfactor(istream *istr, double &result, ostream *ostr, stringstream &debug_ss) 
{ 
    //int poistion = istr->tellg();
    //cerr << "fc_cfactor  Rading: " << istr->tellg() <<  "; " << istr->rdbuf() << endl;
    //istr->seekg(poistion);

    result = 0;
    char ch = fc_token(istr);
    if (ch == '{') 
    {
        if (ostr != NULL) (*ostr) << " \\Bigg( ";
        fc_cexpression(istr, result, ostr, &debug_ss);
        ch = fc_token(istr);
        if (ch != '}') 
        {
            std::string error = std::string("Expected '}', got: ") + ch;
            throw std::runtime_error(error.c_str());
        }
        if (ostr != NULL) (*ostr) << " \\Bigg) ";
    }
    else if (isdigit(ch) || ch=='-' || ch == '.') {
        istr->unget();

        (*istr) >> result;

        if (ostr != NULL) 
        {
            (*ostr) << result << " ";
        }
    }
    else 
    {
        std::string error = std::string("Unexpected character fc_cfactor got: ") + ch;
        throw std::runtime_error(error);
    }
    return true;
}

bool fc_cpower(istream *istr, double &result, ostream *ostr, stringstream &debug_ss) { 
    //int poistion = istr->tellg();
    //cerr << "fc_cpower  Rading: " << istr->tellg() <<  "; " << istr->rdbuf() << endl;
    //istr->seekg(poistion);

    char ch;
    fc_cfactor(istr, result, ostr, debug_ss);
    ch = fc_token(istr);
    if (ch == '^') 
    {
        double power;
        if (ostr != NULL) (*ostr) << "^{";
        fc_cexpression(istr, power, ostr, &debug_ss);
        if (ostr != NULL) (*ostr) << "}";
        result = pow(result,power);
    }
    else istr->unget();
    return true;
}

bool fc_cterm(istream *istr, double &result, ostream *ostr, stringstream &debug_ss) { 
    //int poistion = istr->tellg();
    //cerr << "fc_cterm  Rading: " << istr->tellg() <<  "; " << istr->rdbuf() << endl;
    //istr->seekg(poistion);

    char ch;
    fc_cpower(istr, result, ostr, debug_ss);

    while(true)
    {
        ch = fc_token(istr);
        if (ch == '*' || ch == '/')
        { 
            if (ch == '*') 
            {
                if (ostr != NULL) (*ostr) << " \\times ";
                double b;
                fc_cpower(istr, b, ostr, debug_ss);

            result *= b;
            }
            if (ch == '/') 
            {
                if (ostr != NULL) (*ostr) << " / ";
                double b;
                fc_cpower(istr, b, ostr, debug_ss);

                if (b == 0)
                {
                    throw std::runtime_error("Division by 0 in flag coefficient");
                }

            result /= b;
            }
        }
        else 
        {
            istr->unget();
            break;
        }
    }
    return true;
}

// One has to be careful with expresion because of -
bool fc_cexpression(istream *istr, double &result, ostream *ostr, stringstream *debug_ss)
{
    //int poistion = istr->tellg();
    //cerr << "fc_cexpression  Rading: " << istr->tellg() <<  "; " << istr->rdbuf() << endl;
    //istr->seekg(poistion);


    stringstream tmp;
    if (debug_ss == NULL) debug_ss = &tmp;

    fc_cterm(istr, result, ostr, *debug_ss);

    while(true)
    {
        char ch = fc_token(istr);
        if (ch == '-' || ch=='+') 
        {
            if (ostr != NULL) (*ostr) << ch;
            (*debug_ss) << ch;

            double b;
            fc_cterm(istr, b, ostr, *debug_ss);
            if (ch == '+')
            {
                result += b;
            }
            else
            {
                result -= b;
            }
        }
        else 
        {
            istr->unget(); 
            break;
        }
    }
    return true;
}


void fc_factor(istream *istr, vector<flag_and_coefficient> &result, ostream *ostr, stringstream &debug_ss) 
{ 
    //int poistion = istr->tellg();
    //cerr << "fc_factor  Rading: " << istr->tellg() <<  "; " << istr->rdbuf() << endl;
    //istr->seekg(poistion);

    result.clear();
    char ch = fc_token(istr);
    if (ch == '(') 
    {
        if (ostr != NULL) (*ostr) << " \\Bigg( ";
        debug_ss << "(";
        fc_expression(istr, result, ostr, &debug_ss);
        ch = fc_token(istr);
        if (ch != ')') 
        {
            string remaining;

            getline(*istr, remaining);
            std::string error = std::string("Expected ')', Read so far: " +debug_ss.str() + "\n Rmeain: "+ remaining + "\n Got: ") + ch;
            throw std::runtime_error(error.c_str());
        }
        if (ostr != NULL) (*ostr) << "\\Bigg) ";
        debug_ss << ")";
    }
    else if (ch == '[')
    {
        if (ostr != NULL) (*ostr) << " \\Bigg\\llbracket ";
        debug_ss << "[";

        vector<flag_and_coefficient> labeled_result;

        fc_expression(istr, labeled_result, ostr, &debug_ss);

        ch = fc_token(istr);
        if (ch != ']') 
        {
            string remaining;

            getline(*istr, remaining);
            std::string error = std::string("Expected ']', Read so far: " +debug_ss.str() + "\n Rmeain: "+ remaining + "\n Got: ") + ch;
            throw std::runtime_error(error.c_str());
        }
        if (ostr != NULL) (*ostr) << "\\Bigg\\rrbracket  ";
        debug_ss << "]";
        
        for (auto & F : labeled_result)
        {
            flag_and_coefficient fc;
            
            fc.g = F.g;
            fc.g.m_Theta = 0;
            // Get the unlabeled coefficient
            fc.coefficient = P_F1_IN_H(F.g, fc.g)*F.coefficient;

            result.push_back(fc);           
            //vector<flag_and_coefficient> tmp;
            //tmp.push_back(fc);            
            //fc_add_FC_to_first(result,tmp);
        }
    }
    else if (isdigit(ch) || ch=='-' || ch=='.' || ch=='{')  
    {
        istr->unget();

        double coefficient;

//        fc_cexpression(istr, fc.coefficient, ostr);
        fc_cexpression(istr, coefficient, NULL, &debug_ss);
//        fc_cexpression(istr, coefficient, NULL, NULL);

        // Now check if we continue with loading a flag or loading 
        ch = fc_token(istr);
        istr->unget();
        if (ch == '(' || ch == '[')
        {
            //throw std::runtime_error("Unexpected ( when loading in fc_factor ");
            cerr << " % Note: Be careful to check the result please!" << endl;
            // The below may lead to confusing behavior
            // -fc ' 1+3*4/2(1 2 0 2)^2' is actually -fc '{1+3*4/2}(1 2 0 2)^2'
            // which does not quite presrve the multiplication and + operation...
            if (ostr != NULL) (*ostr) << " " << coefficient << "\\times ";
            debug_ss <<  " " << coefficient << " ";

            vector<flag_and_coefficient> fc_tmp;
            fc_power(istr, fc_tmp, ostr, debug_ss);
            for (auto &fc : fc_tmp)
            {
                fc.coefficient *= coefficient;
                result.push_back(fc);
            }
        }
        else
        if (ch == 'F')
        {  
            // Load from stream
            string extra_file;
            (*istr) >> extra_file;
            vector<flag_and_coefficient> fc_tmp;
            load_flags_and_coefficients_from_file(extra_file, fc_tmp);
            for (auto &fc : fc_tmp)
            {
                fc.coefficient *= coefficient;
                result.push_back(fc);
            }
        }
        else
        {
            //(*istr) >> fc.coefficient;
            flag_and_coefficient fc;
            fc.coefficient = coefficient;

            //poistion = istr->tellg();
            //cerr << "fc_factor  Before loading flag: " << istr->tellg() <<  "; " << istr->rdbuf() << endl;
            //istr->seekg(poistion);            
            if (!fc.g.load_from_stream((*istr),-1,-1))
            {
                throw std::runtime_error("Expecting to load a flag in expression. Load so far: "+debug_ss.str());
            }
            //poistion = istr->tellg();
            //cerr << "fc_factor  After loading flag: " << istr->tellg() <<  "; " << istr->rdbuf() << endl;
            //istr->seekg(poistion);            


            if (ostr != NULL) 
            {
                //(*ostr) << " [" << fc.coefficient << "]" << " \\vc{" << fc.g.print_latex(false, 0) << "} ";
                (*ostr) << " " << fc.coefficient  << " \\vc{" << fc.g.print_latex(false, 0) << "} ";
            }
            debug_ss << " " << fc.coefficient << " " << fc.g.print() << " ";

            result.push_back(fc);
        }
    }
    else if (ch=='M')  
    {
        vector<vector<vector<flag_and_coefficient> > > matrix_result;
        fc_mexpression(istr, matrix_result, ostr, &debug_ss);

        ch = fc_token(istr);
        if (ch != 'X')
        {
            string remaining;

            getline(*istr, remaining);
            std::string error = std::string("Expected to end matrix  with 'X' Read so far: " +debug_ss.str() + "\n Remeaining: "+ remaining + "\n fc_factor got: " + to_string(int(ch)) + " " + ch);

            throw std::runtime_error(error.c_str());            
        }

        int rows = matrix_result.size();
        if (rows != 1)
        {
            cerr << "When including a matrix operation, the result has to be 1x1 matrix but it has " << rows << " rows" <<endl;
            assert(0);
        }

        int cols = matrix_result[0].size();
        if (cols != 1)
        {
            cerr << "When including a matrix operation, the result has to be 1x1 matrix but it has " << cols << " cols" <<endl;
            assert(0);
        }

        result = matrix_result[0][0];
    }
    else 
    {
        string remaining;

        getline(*istr, remaining);
        std::string error = std::string("Unexpected character in \n Expected (, -, ., {, M, or a digit.\nRead so far: " +debug_ss.str() + "\n Remeaining: "+ remaining + "\n fc_factor got: ") + to_string(int(ch)) + " " + ch;
        throw std::runtime_error(error.c_str());
    }
}

void fc_power(istream *istr, vector<flag_and_coefficient> &result, ostream *ostr, stringstream &debug_ss) { 

    //int poistion = istr->tellg();
    //cerr << "fc_power  Rading: " << istr->tellg() <<  "; " << istr->rdbuf() << endl;
    //istr->seekg(poistion);

    char ch;
    fc_factor(istr, result, ostr, debug_ss);
    ch = fc_token(istr);
    if (istr->eof()) return;
    if (ch == '^') 
    {
        ch = fc_token(istr);
        if (isdigit(ch))
        {
            int power=(int)(ch-'0');
            if (power < 1 || power > 10)
            {
                throw std::runtime_error("Unexpected power " + to_string(power));
            }
            if (ostr != NULL) (*ostr) << " ^{"<<power<<"} ";
            debug_ss <<  " ^{"<<power<<"}";
            vector<flag_and_coefficient> base = result;
            for(;power > 1;power--)
            {
                vector<flag_and_coefficient> tmp = result;
                fc_F1_times_F2(tmp,base,result);
            }
        }
        else
        {
            throw std::runtime_error("^ must be followed by a digit.\n Read so far: " + debug_ss.str());
        }
    }
    else istr->unget();
}

void fc_term(istream *istr, vector<flag_and_coefficient> &result, ostream *ostr, stringstream &debug_ss) { 

    //int poistion = istr->tellg();
    //cerr << "fc_term  Rading: " << istr->tellg() <<  "; " << istr->rdbuf() << endl;
    //istr->seekg(poistion);

    char ch;
    fc_power(istr, result, ostr,debug_ss);

    ch = fc_token(istr);
    if (istr->eof()) return;

    //if (ch == '*' || ch == '/') 
    if (ch == '*') 
    {
        if (ostr != NULL) (*ostr) << " \\times ";
        debug_ss << ch;
        vector<flag_and_coefficient> b;
        fc_term(istr, b, ostr, debug_ss);

        vector<flag_and_coefficient> tmp=result;

        fc_F1_times_F2(tmp,b,result);
    }
    else istr->unget();
}

// One has to be careful with expresion because of -
void fc_expression(istream *istr, vector<flag_and_coefficient> &result, ostream *ostr, stringstream *debug_ss) 
{
    //int poistion = istr->tellg();
    //cerr << "fc_expression  Rading: " << istr->tellg() <<  "; " << istr->rdbuf() << endl;
    //istr->seekg(poistion);

    stringstream tmp;
    if (debug_ss == NULL)
        debug_ss = &tmp;

    fc_term(istr, result, ostr, *debug_ss);

    while(true)
    {
        char ch = fc_token(istr);
        if (istr->eof()) break;

        if (ch == '-' || ch=='+' || ch==';') 
        {
            if (ostr != NULL) (*ostr) << ch;

            if (ch == '+')
            {
                //cerr << "Adding + " << endl;
            }

            (*debug_ss) << ch;

            vector<flag_and_coefficient> b;
            fc_term(istr, b, ostr, *debug_ss);
            if (ch == '+')
            {
                fc_add_FC_to_first(result, b);
            }
            else
            {
                multiply_FC_by_C(b, -1);
                fc_add_FC_to_first(result, b);
            }
        }
        else if (ch == ';' || ch == '@')
        {
            if (ostr != NULL) (*ostr) << ch;
            (*debug_ss) << ch;

            if (ch == ';')
            {
                break;
            }

            int Kn=-1;
            (*istr) >> Kn;

            if (ostr != NULL) (*ostr) << Kn << "  ";
            (*debug_ss) << Kn << " ";

            // Test if all flags in the result have at most K_n vertices
            bool KnBigEnough = true;
            for (auto &fc : result)
            {
                if (fc.g.m_vertices > Kn) 
                {
                    KnBigEnough = false;
                    cerr << "Kn=" << Kn << " but trying to use flag " << fc.g.print() << endl;
                    break;
                }
            }

            if (Kn == -1 || Kn == 0 || Kn > V || KnBigEnough == false)
            {
                string remaining;

                getline(*istr, remaining);
                std::string error = std::string("After @ expected number of vertices (integer) Kn='"+to_string(Kn)+"'.\nRead so far: " +debug_ss->str() + "\n Remeaining: "+ remaining + "\n");
                if (KnBigEnough == false)
                {
                    error = std::string("After @ the number of vertices (integer) must be at least as big as the flags before.\nRead so far: " +debug_ss->str() + "\n Remeaining: "+ remaining + "\n");
                }
                throw std::runtime_error(error.c_str());                
            }


            get_unlabeled_flags_of_size(Kn);
            vector<flag_and_coefficient> projected_result;
            
            for (auto const & F : g_unlabeled_flags[Kn])
            {
                double densitysum = 0;
                for (auto const & R : result)
                {
                    if (R.g.m_vertices <= Kn)
                    {              
                        densitysum += R.coefficient*P_F1_IN_H(R.g, F);                
                    }
                    else
                    {
                        std::string error = std::string("This should not happen");
                        //densitysum += R.coefficient*P_F1_IN_H(F, R.g);
                    }
                }
                flag_and_coefficient fc;
                fc.g = F;
                fc.coefficient = densitysum;
                if (densitysum != 0)
                {
                    projected_result.push_back(fc);
                }
            }
            result = std::move(projected_result);

        }

        else 
        {
            istr->unget(); 
            break;
        }
    }

    //cerr << "Debug: Loaded expression\n" << debug_ss->str() << endl;
}


bool flag_calculator_simple(vector<flag_and_coefficient> &result, string& inputname, ostream *ostr = NULL, int verbose_output=0)
{
   OPEN_FILE_SMARTLY_RETURN_FALSE_ON_FAIL(istr, inputname);

    //ostr = &cout;

    if (ostr != NULL)
    {
        (*ostr) << " $ ";
    }

    fc_expression(istr, result, ostr);

    if (ostr != NULL)
    {
        (*ostr) << " $ ";
    }

    return true;
}



// Matrix utilities


template<class T>bool is_a_rectangle(vector<vector<T> > & M)
{
    if (M.size() == 0) return true;
    unsigned int columns = M[0].size();
    for (auto &row : M)
    {
        if (row.size() != columns)
        {
            cerr << "Matrix is not a rectangular." << endl;
            return false;
        }
    }
    return true;
}

void fcm_A_times_B(vector<vector<vector<flag_and_coefficient> > > &A, vector<vector<vector<flag_and_coefficient> > > &B,  vector<vector<vector<flag_and_coefficient> > > &result)
{
    if ( is_a_rectangle(A) == false || is_a_rectangle(B) == false)
    {
        assert(0);
    }
    int A_rows = A.size();
    int B_rows = B.size();
    if (A_rows == 0 || B_rows == 0)
    {
        cerr << "Matrix has no rows. Cannot be used in multiplication" << endl;
        assert(0);
    }

    int A_cols = A[0].size();
    int B_cols = B[0].size();

    if (A_cols != B_rows)
    {
        cerr << "Cannot multiply matrices of sizes " << A_rows << "x" << A_cols << " with " << B_rows << "x" << B_cols << endl;
        assert(0);
    }

    result.clear(); // Just to make sure
    for (int i = 0; i < A_rows; i++)
    {
        vector<vector<flag_and_coefficient> >  one_row;
        for (int j = 0; j < B_cols; j++)
        {
            vector<flag_and_coefficient> one_entry;
            for (int k = 0; k < A_cols; k++)
            {
                vector<flag_and_coefficient> tmp;
                fc_F1_times_F2(A[i][k], B[k][j],tmp);
                fc_add_FC_to_first(one_entry, tmp);
            }
            one_row.push_back(one_entry);
        }
        result.push_back(one_row);
    }
}


void fcm_A_plus_B(vector<vector<vector<flag_and_coefficient> > > &A, vector<vector<vector<flag_and_coefficient> > > &B,  vector<vector<vector<flag_and_coefficient> > > &result)
{
    result.clear(); // Just to make sure

    if ( is_a_rectangle(A) == false || is_a_rectangle(B) == false)
    {
        assert(0);
    }
    int A_rows = A.size();
    int B_rows = B.size();
    if (A_rows == 0 && B_rows == 0)
        return;

    if (A_rows != B_rows)
    {
        cerr << "Matrices have number of rows " << A_rows << " and " << B_rows << " so they cannot be added." << endl; 
        assert(0);
    }

    int A_cols = A[0].size();
    int B_cols = B[0].size();

    if (A_cols != B_cols)
    {
        cerr << "Cannot add matrices of sizes " << A_rows << "x" << A_cols << " with " << B_rows << "x" << B_cols << endl;
        assert(0);
    }

    for (int i = 0; i < A_rows; i++)
    {
        vector<vector<flag_and_coefficient> >  one_row;
        for (int j = 0; j < A_cols; j++)
        {
            vector<flag_and_coefficient> one_entry = A[i][j];
            fc_add_FC_to_first(one_entry, B[i][j]);
            one_row.push_back(one_entry);
        }
        result.push_back(one_row);
    }
}


void fcm_minus_A(vector<vector<vector<flag_and_coefficient> > > &A)
{
    for (auto &row : A)
    {
        for(auto &entry : row)
        {
            multiply_FC_by_C(entry, -1);
        }
    }
}

void fcm_transpose_A(vector<vector<vector<flag_and_coefficient> > > &A)
{
    if ( is_a_rectangle(A) == false)
    {
        assert(0);
    }
    int A_rows = A.size();
    if (A_rows == 0) return;
    int A_cols = A[0].size();

    vector<vector<vector<flag_and_coefficient> > > result;

    vector< vector<flag_and_coefficient> > one_row;
    one_row.resize(A_rows); 
    for (int i = 0; i < A_cols; i++)
        result.push_back(one_row);


    for (int i = 0; i < A_rows; i++)
        for (int j = 0; j < A_cols; j++)
        {
            result[j][i] = A[i][j];
        }
    A = result;
}

// 
//
//
void fc_mfactor(istream *istr, vector<vector<vector<flag_and_coefficient> > > &result, ostream *ostr, stringstream &debug_ss) 
{
    //int poistion = istr->tellg();
    //cerr << "fc_expression  Rading: " << istr->tellg() <<  "; " << istr->rdbuf() << endl;
    //istr->seekg(poistion);

    char ch = fc_token(istr);
    if (istr->eof()) return;

    if (ch != 'm')
    {
        istr->unget(); 
        return;
    }

    ch = fc_token(istr);
    if (istr->eof()) 
    {
        cerr << "Unexpected end of file while loading a matrix" << endl;
        return;
    }


    if (ostr != NULL) (*ostr) << "\\begin{pmatrix}";

    vector< vector<flag_and_coefficient> > one_row;
    
    // Read matrix of flags
    bool only_coefficients_in_matrix=false;
    if (ch == 'f')
    {
        debug_ss << " mf ";
        only_coefficients_in_matrix = false;
    }
    else if (ch == 'c')
    {
        debug_ss << " mc ";
        only_coefficients_in_matrix = true;
    }
    else
    {
        cerr << "Unexpected token '" << ch << "' only 'f' or 'c' need to follow 'm' to decide if the matric has flags or coefficients." << endl;
        istr->unget();
        return;
    }

    do {
        vector<flag_and_coefficient> one_entry;

        if (only_coefficients_in_matrix)
        {
            double coefficient;
            fc_cexpression(istr, coefficient, NULL);
            if (ostr != NULL) (*ostr) << " " << coefficient << " ";
            debug_ss << " " << coefficient << " ";
            flag_and_coefficient fc;
            fc.coefficient = coefficient;
            fc.g.set_vertices_and_Theta(0,0);
            one_entry.push_back(fc);
        }
        else
        {
            fc_expression(istr, one_entry, ostr, &debug_ss); 
        }

        //if (one_entry.size() == 0)
        //    break;

        one_row.push_back(one_entry);

        ch = fc_token(istr);
        if (ch == ',' || ch == ';' || ch == '&')
        {
            debug_ss << " , ";
            if (ostr != NULL) (*ostr) << " & ";
            continue;
        }


        if (ch == '\\')
        {
            debug_ss << " \\ " << endl;
            if (ostr != NULL) (*ostr) << " \\\\ " << endl;
            result.push_back(one_row);
            one_row.clear();

            // Eat an extra \ in case lines are separated by \\ instead of single one
            ch = fc_token(istr);
            if (ch != '\\')
                istr->unget();

            continue;
        }

        if (ch == 'x')
        {
            result.push_back(one_row);
            break;
        }
        
        // here is some unexpected token
        break;

    } while (!istr->eof());


    debug_ss << " x " << endl;
    if (ostr != NULL) (*ostr) << "\\end{pmatrix}";


    if (ch != 'x')
    {
        cerr << "Matrix needs to end with 'x', unexpected token '" << ch << "'." << endl;
        istr->unget();
        return;      
    }

    //cerr << "Debug: Loaded expression\n" << debug_ss->str() << endl;
}

void fc_mpower(istream *istr, vector<vector<vector<flag_and_coefficient> > > &result, ostream *ostr, stringstream &debug_ss) { 

    //int poistion = istr->tellg();
    //cerr << "fc_mpower  Rading: " << istr->tellg() <<  "; " << istr->rdbuf() << endl;
    //istr->seekg(poistion);

    char ch;
    fc_mfactor(istr, result, ostr, debug_ss);
    ch = fc_token(istr);
    if (istr->eof()) return;
    if (ch == '^') 
    {
        ch = fc_token(istr);
        if (ch == 'T')
        {
            if (ostr != NULL) (*ostr) << " ^T ";
            debug_ss <<  " ^T";
            //TODO
            fcm_transpose_A(result);
        }
        else
        {
            throw std::runtime_error("^ must be followed by T.\n Read so far: " + debug_ss.str());
        }
    }
    else istr->unget();
}

void fc_mterm(istream *istr, vector<vector<vector<flag_and_coefficient> > > &result, ostream *ostr, stringstream &debug_ss) { 

    //int poistion = istr->tellg();
    //cerr << "fc_term  Rading: " << istr->tellg() <<  "; " << istr->rdbuf() << endl;
    //istr->seekg(poistion);

    char ch;
    fc_mpower(istr, result, ostr,debug_ss);

    ch = fc_token(istr);
    if (istr->eof()) return;

    //if (ch == '*' || ch == '/') 
    if (ch == '*') 
    {
        if (ostr != NULL) (*ostr) << " \\times ";
        debug_ss << ch;
        vector<vector<vector<flag_and_coefficient> > > b;
        fc_mterm(istr, b, ostr, debug_ss);

        vector<vector<vector<flag_and_coefficient> > > tmp=result;

        fcm_A_times_B(tmp, b, result);
    }
    else istr->unget();
}


// One has to be careful with expresion because of -
void fc_mexpression(istream *istr, vector<vector<vector<flag_and_coefficient> > > &result, ostream *ostr, stringstream *debug_ss)
{
    //int poistion = istr->tellg();
    //cerr << "fc_cexpression  Rading: " << istr->tellg() <<  "; " << istr->rdbuf() << endl;
    //istr->seekg(poistion);

    stringstream tmp;
    if (debug_ss == NULL) debug_ss = &tmp;

    fc_mterm(istr, result, ostr, *debug_ss);

    while(true)
    {
        char ch = fc_token(istr);
        if (ch == '-' || ch=='+') 
        {
            if (ostr != NULL) (*ostr) << ch;
            (*debug_ss) << ch;

            vector<vector<vector<flag_and_coefficient> > > b;
            fc_mterm(istr, b, ostr, *debug_ss);
            if (ch == '+')
            {
                vector<vector<vector<flag_and_coefficient> > > tmp = result;
                fcm_A_plus_B(tmp, b, result);
            }
            else
            {
                fcm_minus_A(b);
                vector<vector<vector<flag_and_coefficient> > > tmp = result;
                fcm_A_plus_B(tmp, b, result);
            }
        }
        else 
        {
            istr->unget(); 
            break;
        }
    }
}



bool flag_calculator_matrix(vector< vector< vector<flag_and_coefficient> > > &result, string& inputname, ostream *ostr = NULL, int verbose_output=0)
{
   OPEN_FILE_SMARTLY_RETURN_FALSE_ON_FAIL(istr, inputname);

    //ostr = &cout;

    if (ostr != NULL)
    {
        (*ostr) << " $ ";
    }

    stringstream debug_ss;
    fc_mexpression(istr, result, ostr, &debug_ss);

    cerr << endl;
    cerr << debug_ss.str() << endl;

    if (ostr != NULL)
    {
        (*ostr) << " $ ";
    }

    cerr << "Result size=" << result.size() << endl;

    return true;
}



struct one_H_pair
{
public:
    int typeID;
    int idF1;
    int idF2;
    int count;
};

// f is an unlabeled flag
void count_flag_products(stringstream &ss, int matrixID, const flag &f)
{    
    int subset_split[V];
    int mapping_F1[V];
    int mapping_F2[V];
    flag F1, F2; 
 
    // Try to find decomposition of vertices of f into 3 sets.
    // first is of size typesize and the other two are of sizes privatesize
    // Then typesize+privatesize creates one flag.
    
    // We go through all possible type sizes
    for (int typesize = f.m_vertices-2; typesize >= 0; typesize -= 2)
    {
        vector<pair<string,int> > found_pairs;
        vector<one_H_pair> found_id_pairs;

        int privatesize = (f.m_vertices-typesize)/2; 
        int totalsize = typesize+privatesize;  // size of F1 and F2
        int label_f1 = typesize;
        int label_f2 = typesize+1;

#ifdef G_ORDERED_VERTICES
        for (int i = 0; i < typesize; i++) subset_split[i] = -1; // still same order
#else
        for (int i = 0; i < typesize; i++) subset_split[i] = i;
#endif
        assert(typesize+2*privatesize <= V);
        for (int i = 0; i < privatesize; i++)
        {   
            subset_split[typesize+i] = label_f1;
            subset_split[typesize+i+privatesize] = label_f2; // I don't know why the warning is here...
        }
        
        do {
            //std::cout << subset_split[0] << ' ' << subset_split[1] << ' ' << subset_split[2] << ' ' << subset_split[3] << '\n';
        
            // Construction of inverz mapping
#ifdef G_ORDERED_VERTICES
            int inF1 = 0;
            int inF2 = 0;
            int ThetaF1 = 0;
            int ThetaF2 = 0;
            for (int i = 0; i < f.m_vertices; i++)
            {
                if (subset_split[i] == -1)
                {
                    mapping_F1[inF1] = i;
                    mapping_F2[inF2] = i;
                    ThetaF1 |= 1 << inF1++;
                    ThetaF2 |= 1 << inF2++;
                }
                if (subset_split[i] == label_f1) mapping_F1[inF1++] = i; 
                if (subset_split[i] == label_f2) mapping_F2[inF2++] = i; 
            }
            
            F1.as_subflag(f,mapping_F1, totalsize, ThetaF1);
            
            int typeID = get_flag_type_in_list(F1, g_flags);
            if (typeID == -2) continue;
#ifdef G_NOT_ALL_FLAGS_USED
            if (typeID == -1) continue;
#endif            
            assert (typeID != -1);
            
            F2.as_subflag(f,mapping_F2, totalsize, ThetaF2);
            
            if (!F2.have_same_type(F1))
            {
                cerr << typesize << " " << privatesize << endl;
                cerr << f.print() << endl;
                cerr << F1.print() << "   " << F2.print() << endl;
                
                for (int i = 0; i < f.m_vertices;i++)
                    cerr << subset_split[i];
                cerr << endl;
                
                assert(0);
            }
#else
            int inF1 = typesize;
            int inF2 = typesize;            
            for (int i = 0; i < f.m_vertices; i++)
            {
                if (subset_split[i] < typesize) 
                {
                    mapping_F1[subset_split[i]] = i;
                    mapping_F2[subset_split[i]] = i;
                }
                if (subset_split[i] == label_f1) mapping_F1[inF1++] = i; 
                if (subset_split[i] == label_f2) mapping_F2[inF2++] = i; 
            }

            F1.as_subflag(f,mapping_F1, totalsize, typesize);
        
            int typeID = get_flag_type_in_list(F1, g_flags);
            if (typeID == -2) continue;
#ifdef G_NOT_ALL_FLAGS_USED
            if (typeID == -1) continue;
#endif            
            assert (typeID != -1);
            
            F2.as_subflag(f,mapping_F2, totalsize, typesize);
#endif 

            int idF1 = find_flag_in_list(F1,g_flags[typeID]);
            int idF2 = find_flag_in_list(F2,g_flags[typeID]);
            if (idF1 > idF2 ) continue; // count only once when idF1 <= idF2         
            if (idF1 == -1)
            {
#ifdef G_NOT_ALL_FLAGS_USED
                continue;
#endif                
                cerr << "Missing " << F1.print() << endl;
            }
            assert(idF1 >= 0 && idF2 >= 0); 
            
            //stringstream what_found_ss;
            //what_found_ss << typeID+1 << " " << idF1+1 << " " << idF2+1;
            //string what_found = what_found_ss.str();
            
            bool known = false;

            /*
            for(int i = 0; i < (int)found_pairs.size(); i++)
            {
                if (found_pairs[i].first == what_found) 
                {
                    found_pairs[i].second++;
                    known = true;
                    break;
                }
            }
            if (known == false)
            {
                found_pairs.push_back(make_pair(what_found,1));
            }
            */

            for (int i = 0; i < (int)found_id_pairs.size(); i++)
            {
                if (found_id_pairs[i].typeID == typeID && found_id_pairs[i].idF1 == idF1
                && found_id_pairs[i].idF2 == idF2)
                {
                    found_id_pairs[i].count++;               
                    known = true;
                    break;
                }
            }
            if (known == false)
            {
                one_H_pair Hpair;
                Hpair.typeID = typeID;
                Hpair.idF1 = idF1;
                Hpair.idF2 = idF2;
                Hpair.count = 1;
                found_id_pairs.push_back(Hpair);
            }
            
        } while ( std::next_permutation(subset_split,subset_split+f.m_vertices) );
        
        //for(int i = 0; i < (int)found_pairs.size(); i++)
        //{
           // ss << matrixID << " " << found_pairs[i].first << " " <<  found_pairs[i].second << endl; 
        //}
        for(const auto &ids : found_id_pairs)
        {
            ss << matrixID << " " << ids.typeID+1 << " " << ids.idF1+1 << " " << ids.idF2+1 << " " << ids.count << endl; 
        }
    }
}



double compute_linear_combination_in_g(const flag &g, const vector<flag_and_coefficient> &lc)
{
    double total_density = 0;    
    
    for (int i = 0; i < (int)lc.size(); i++)
    {
        total_density += lc[i].coefficient * P_F1_IN_H(lc[i].g, g);
    }
    
    return total_density;
}



double compute_objective_function_for_SDP(const flag &g)
{
#ifdef USE_FOR_CROSSINGS
    if (g.m_crossings_cnt == 0)
    {
        cerr << "Hey " << g.print() << endl;
    }
    ///HACK!!!
    //if (g.m_crossings_cnt > 11) return 11;
	return g.m_crossings_cnt;
#endif

    return compute_linear_combination_in_g(g, g_objective_combination);
    
	double total_density = 0;	

	for (int i = 0; i < (int)g_objective_combination.size(); i++)
	{
        total_density += g_objective_combination[i].coefficient * P_F1_IN_H(g_objective_combination[i].g, g);
        
        // HACK
        //cerr << P_F1_IN_H(g_objective_combination[i].g, g) << " " << g.m_rotation_system_noncrosssings << endl;
    }
    
	return total_density;
}


int print_CSDP_simple_linear_constraints(ostream &ostr, int Kn, int i, int matrixID, int blockID, bool print_blocks, bool print_products, int verbose_output)
{
    
    if (g_use_simple_linear_constraints_list.size() == 0)
    {
        for (int c = 0; c < (int)g_linear_constraints.size(); c++)
        {
            g_use_simple_linear_constraints_list.push_back(c);
        }
    }
    
    int constraint_id = 0;
//    for (int j = 0; j < (int)g_linear_constraints.size(); j++)
    for (int c = 0; c < (int)g_use_simple_linear_constraints_list.size(); c++)
    {
        int j = g_use_simple_linear_constraints_list[c];
        assert(g_linear_constraints[j].m_constant == 0);
        
        constraint_id++;
        
        if (print_products)
        {
            double d = 0;
            for (int k = 0; k < (int)g_linear_constraints[j].m_entries.size(); k++)
            {
                assert (g_linear_constraints[j].m_entries[k].coefficient != 0);
                d += g_linear_constraints[j].m_entries[k].coefficient * P_F1_IN_H(g_linear_constraints[j].m_entries[k].g, g_unlabeled_flags[Kn][i]);
            }
            if (d != 0)
            {
                ostr.precision(G_PRECISION);
                ostr <<  matrixID << " " << blockID << " " << constraint_id << " " << constraint_id << " " << smart_round(d) << endl;
            }
        }
    } 
    
    if (print_blocks)
    {
        ostr << -constraint_id << " ";
        if (verbose_output)
        {
            cerr << "Simple linear are used for " << constraint_id << "/" << g_linear_constraints.size() << " constraints" << endl;
        }
    }
    
    return constraint_id;
}


// We assume print_products may run in parallel
int print_CSDP_product_linear_constraints(ostream &ostr, int Kn, int i, int matrixID, int blockID, bool print_blocks, bool print_products, int verbose_output)
{
    // g_flag_product_linear_constraints[ constraint ] is a vector of flags that can be used to multiply  [constraint]
    static vector<vector<flag> >  g_flag_product_linear_constraints;
    
    if (g_flag_product_linear_constraints.size() != g_linear_constraints.size())
        g_flag_product_linear_constraints.resize(g_linear_constraints.size());
    
    
    int constraints_possible = 0;
    
    int constraint_id = 0;
    for (int j = 0; j < (int)g_linear_constraints.size(); j++)
    {
         // The linear constraints need the same types
        //if (!g_linear_constraints[j].m_same_types) continue;
        
                
        // Size of the other flags in multiplication
        int type_size = g_linear_constraints[j].m_labeled_vertices_in_type_cnt;         
        int flag_size = (Kn - g_linear_constraints[j].m_entries_max_size) + type_size;

#ifdef G_COLORED_VERTICES
        if (flag_size < 1)
#else
        if (flag_size < 2)   // multiplying by just 1 vertex does not do much (unless the vertex has a color)
#endif
        {
            if (print_blocks)
                cerr << "Size of big graphs is too small to use products linear constraint " << j+1 << endl;
            continue;
        }
        
        if (print_products && g_flag_product_linear_constraints[j].size() == 0)
        {
            continue;
        }
                
        // getting labeled flags of the right size
        if (g_flag_product_linear_constraints[j].size() == 0)
        {
            flag type;
            type = g_linear_constraints[j].m_type;
            get_labeled_flags_of_one_type(flag_size, type, g_flag_product_linear_constraints[j]);
            
            if (g_flag_product_linear_constraints[j].size() == 0)
            {
                cerr << "Linear constraint " << j+1 << " cannot be used since generating flags of type " <<  type.print() << " and size " <<  flag_size  << " gave zero flags" << endl;
                continue;
            }
            
            if (verbose_output)
                cerr << "Constraint " << j << "/" << g_linear_constraints.size() << " can be multiplied by " <<  g_flag_product_linear_constraints[j].size()  << " other flags " << endl;
            //for (int i = 0; i < g_flag_product_linear_constraints[j].size(); i++)
            //{
            //    cerr << g_flag_product_linear_constraints[j][i].print() << endl;
            //}
            
        }
        
        constraints_possible++;

        for (int z = 0; z < (int)g_flag_product_linear_constraints[j].size(); z++)
        {
            constraint_id++;
            
            if (print_products)
            {
                // constant
                double d = 0; //g_linear_constraints[j].m_constant * P_F1_IN_H(g_flag_product_linear_constraints[j][z], g_unlabeled_flags[Kn][i]);
                
                // products with other flags in the linear constraint
                for (int k = 0; k < (int)g_linear_constraints[j].m_entries.size(); k++)
                {
                    assert (g_linear_constraints[j].m_entries[k].coefficient != 0);
                    double PF = P_F1_F2_IN_labeled_H(g_linear_constraints[j].m_entries[k].g, g_flag_product_linear_constraints[j][z], g_unlabeled_flags[Kn][i]);
                    
                    d += PF * g_linear_constraints[j].m_entries[k].coefficient;
                }
                if (d != 0)
                {
                    ostr.precision(G_PRECISION);
                    ostr << matrixID << " " << blockID << " " << constraint_id << " " << constraint_id << " " << smart_round(d) << endl;
                    
                }
            }
        }
    } 
    
    
    if (print_blocks && constraint_id != 0)
    {
        ostr << -constraint_id << " ";
        //if (verbose_output)
        {
            cerr << "Product linear constraints are working for " << constraints_possible << "/" << g_linear_constraints.size() << " constraints" << endl;
        }
    }   
    
    return constraint_id;
}

// This is not yet written
int print_CSDP_product_linear_constraints_extra_lines_in_csdp_nontrivial_sum(ostream &ostr, int Kn, int matrixID, int blockID, bool print_constraints, bool  print_constants, int verbose_output)
{
    return 0;
/*
    // g_flag_product_linear_constraints[ constraint ] is a vector of flags that can be used to multiply  [constraint]
    static vector<vector<flag> >  g_flag_product_linear_constraints;
    
    if (g_flag_product_linear_constraints.size() != g_linear_constraints.size())
        g_flag_product_linear_constraints.resize(g_linear_constraints.size());
    
    
    int constraints_possible = 0;
    
    int constraint_id = 0;
    for (int j = 0; j < (int)g_linear_constraints.size(); j++)
    {
         // The linear constraints need the same types
        //if (!g_linear_constraints[j].m_same_types) continue;
        if (g_linear_constraints[j].m_required_coefficients_sum_for_elcp_constraints == false)
            continue;

        // Size of the other flags in multiplication
        int type_size = g_linear_constraints[j].m_labeled_vertices_in_type_cnt;         
        int flag_size = (Kn - g_linear_constraints[j].m_entries_max_size) + type_size;

#ifdef G_COLORED_VERTICES
        if (flag_size < 1)
#else
        if (flag_size < 2)   // multiplying by just 1 vertex does not do much (unless the vertex has a color)
#endif
        {
            if (print_blocks)
                cerr << "Size of big graphs is too small to use products linear constraint " << j+1 << endl;
            continue;
        }
        
        if (print_products && g_flag_product_linear_constraints[j].size() == 0)
        {
            continue;
        }
                
        // getting labeled flags of the right size
        if (g_flag_product_linear_constraints[j].size() == 0)
        {
            flag type;
            type = g_linear_constraints[j].m_type;
            get_labeled_flags_of_one_type(flag_size, type, g_flag_product_linear_constraints[j]);
            
            if (g_flag_product_linear_constraints[j].size() == 0)
            {
                cerr << "Linear constraint " << j+1 << " cannot be used since generating flags of type " <<  type.print() << " and size " <<  flag_size  << " gave zero flags" << endl;
                continue;
            }
            
            if (verbose_output)
                cerr << "Constraints " << j << " can be multiplied by " <<  g_flag_product_linear_constraints[j].size()  << " other flags " << endl;            
        }
        
        constraints_possible++;

        if (print_constants)
        {
            ostr.precision(G_PRECISION);
            ostr << " " << g_linear_constraints[j].m_required_coefficients_sum_for_elcp_constraints_value << " "; 
        }
        
        if (print_constraints)
        {
            for (int z = 0; z < (int)g_flag_product_linear_constraints[j].size(); z++)

            ostr.precision(G_PRECISION);
            ostr << matrixID << " " << blockID << " " << constraint_id << " " << constraint_id << " " << 1 << endl;

            constraint_id++;                    
        }
    } 
    
    
    if (print_blocks && constraint_id != 0)
    {
        ostr << -constraint_id << " ";
        //if (verbose_output)
        {
            cerr << "Product linear constraints are working for " << constraints_possible << "/" << g_linear_constraints.size() << " constraints" << endl;
        }
    }   
    
    return constraint_id;

    m_required_coefficients_sum_for_elcp_constraints
*/    
}


/*
This is pocessing constraints like (H + c) >= 0 
It tries
 (H + c) (F^T M F) 
So we compute F^T M F  and try the downward operator. And then we do the multiplication with (H-c)
 (F^T M F) 
*/
/*
int print_CSDP_square_linear_constraints(ostream &ostr, int Kn, int i, int matrixID, int blockID, bool print_blocks, bool print_products, int verbose_output)
{   
    if (g_flag_square_linear_constraints.size() != g_linear_constraints.size())
        g_flag_square_linear_constraints.resize(g_linear_constraints.size());

    
    int constraint_id = 0;
    
    for (int j = 0; j < (int)g_linear_constraints.size(); j++)
    {
        // The linear constraints need the same types
        // if not, just skip this shit
        //if (!g_linear_constraints[j].m_same_types) continue;
        
        // Size of the other flags in multiplication
        int type_size = g_linear_constraints[j].m_labeled_vertices_in_type_cnt;         
        int flag_size = (Kn - g_linear_constraints[j].m_entries_max_size)/2 + type_size;
        
        if (flag_size == 0)
        {
            if (print_blocks)
                cerr << "Size of big graphs is too small to use products in constraint " << j+1 << endl;
            continue;
        }
        
        if (print_products && g_flag_square_linear_constraints[j].size() == 0)
        {
            continue;
        }
        
        // getting labeled flags of the right size
        if (g_flag_square_linear_constraints[j].size() == 0)
        {
            flag type;
            g_linear_constraints[j].m_entries[0].g.get_type_subflag(type);
            get_labeled_flags_of_one_type(flag_size, type, g_flag_square_linear_constraints[j]);
            
            if (g_flag_square_linear_constraints[j].size() == 0)
            {
                cerr << "Linear constraint " << j+1 << " cannot be used since generating of other flags of type " <<  type.print() << " and size " <<  flag_size  << " failed" << endl;
                continue;
            }
            

            if (verbose_output)
                cerr << "Constraints " << j << " is using " << g_flag_square_linear_constraints[j].size() << " flags " << endl;
            //for (int i = 0; i < g_flag_square_linear_constraints[j].size(); i++)
            //{
            //    cerr << g_flag_square_linear_constraints[j][i].print() << endl;
            //}
            
        }
        
        
        if (print_blocks)
        {
            ostr << g_flag_square_linear_constraints[j].size() << " ";
        }
 
        
            if (print_products)
            {
                //cerr <<  g_unlabeled_flags[Kn][i].print() << endl;
                
                for (unsigned int x = 0; x < g_flag_square_linear_constraints[j].size(); x++)
                    for (unsigned int y = x; y < g_flag_square_linear_constraints[j].size(); y++)
                    {
                        double PF12 = P_F1_F2_IN_H(g_flag_square_linear_constraints[j][x], g_flag_square_linear_constraints[j][y], g_unlabeled_flags[Kn][i]);
                        double d = PF12 * g_linear_constraints[j].m_constant;
                        
                        //cerr << "F1=" << g_flag_square_linear_constraints[j][x].print() << " F2=" << g_flag_square_linear_constraints[j][y].print() << endl;
                        //cerr << "PF12  = " << PF12 << endl; 
                        
                        for (int k = 0; k < (int)g_linear_constraints[j].m_entries.size(); k++)
                        {
                            double PF123 = P_F1_F2_F3_IN_H(g_flag_square_linear_constraints[j][x], g_flag_square_linear_constraints[j][y], g_linear_constraints[j].m_entries[k].g, g_unlabeled_flags[Kn][i]);
                            
                            d += PF123 * g_linear_constraints[j].m_entries[k].coefficient; 
                            
                            //cerr << "PF123 = " << PF123 << endl; 

                            
                        }
                        if (d != 0)
                        {
                            ostr << matrixID <<  " " << blockID+constraint_id << " " << x+1 << " " << y+1 << " " <<  d << endl;
                        }
                    }
            }

        constraint_id++;
    }    
    
    if (print_blocks && constraint_id != 0)
    {
        cerr << "Square linear constraints are working for " << constraint_id << "/" << g_linear_constraints.size() << " constraints" << endl;
    }
    
    return constraint_id;
}
*/


void generate_all_types_containing_one_subtype(int big_type_size, const flag &type, vector<flag> &flag_list)
{
    flag g;
    g.set_vertices_and_Theta(big_type_size,big_type_size);
    g.copy_from(type);
    
    extensions_of_g(g,flag_list);
}




/*
 This is pocessing constraints like  F1+F2 >= 0 
 It tries
 (F1+F2)  [(F^T M F)]
 Let the type of the constraint be t. We take all types t_i that contain t as a subtype.
 The we compute  F_i^T M_i F_i  and do partial unlabeling down to t and finally multiply with (F1+F2).
So we have a set of constraints of form
 (F1+F2) [(F_i^T M_i F_i)]_t
 */
int print_CSDP_square_linear_constraints_v2(ostream &ostr, int Kn, int i, int matrixID, int blockID, bool print_blocks, bool print_products, int verbose_output)
{  
    static vector<vector< vector<vector<flag> > > > g_flag_square_linear_constraints_v2; // flag used only for linear constraints
    
    // For a constraint i, it remembers all 
    static vector< vector<vector<flag> > >  g_flag_square_linear_constraints_v2_typelist; // flag used only for linear constraints

    
    if (g_flag_square_linear_constraints_v2.size() != g_linear_constraints.size())
    {
        g_flag_square_linear_constraints_v2.resize(g_linear_constraints.size());
        g_flag_square_linear_constraints_v2_typelist.resize(g_linear_constraints.size());
    }
    
    
    int constraint_id = 0;
    int constraints_applicable = 0;
    
    for (int j = 0; j < (int)g_linear_constraints.size(); j++)
    {
        // TODO: Shuld be done sooner
        //g_linear_constraints[j].check_if_same_types();
        
        // The linear constraints need the same types
        // if not, just skip this shit
        assert(g_linear_constraints[0].m_checked);
        //if (!g_linear_constraints[j].m_same_types) continue;
         
        // Size of the other flags in multiplication
        int type_size = g_linear_constraints[j].m_labeled_vertices_in_type_cnt;
        
        flag type;
        g_linear_constraints[j].m_entries[0].g.get_type_subflag(type);
        
        int free_vertices = Kn - g_linear_constraints[j].m_entries_max_size; // vertices not used by the constraint
        if (free_vertices < 2)
        {
                if (print_blocks)
                    cerr << "Size of unlabeled flags is too small to use squares in constraint " << j+1 << endl;
                continue;
        }
        
        constraints_applicable++;

        // generating additional flags
        // int current_type_id=0;
        //int max_inner_type_size = type_size+free_vertices-2;
        int max_inner_type_size = type_size;

        
        if (g_flag_square_linear_constraints_v2_typelist[j].size() == 0)
        {
            if (print_products) continue;
            g_flag_square_linear_constraints_v2_typelist[j].resize(max_inner_type_size+1);
            g_flag_square_linear_constraints_v2[j].resize(max_inner_type_size+1);
        }
                
        for (int inner_type_size = type_size; inner_type_size <= max_inner_type_size; inner_type_size++)
        {        
            int flag_size = (free_vertices-(inner_type_size-type_size))/2 + inner_type_size;
            
            //if (print_blocks)
                //cerr << "Inner type size " << inner_type_size << " flag size " << flag_size << endl;
            assert(flag_size > inner_type_size);
            
            if (flag_size == 0)
            {
                if (print_blocks)
                    cerr << "Size of big graphs is too small to use products in constraint " << j+1 << endl;
                continue;
            }
        
           // if (print_products && g_flag_square_linear_constraints_v2[j].size() == 0)
           // {
           //     continue;
           // }
        
            // generate all types of the right size
            if (g_flag_square_linear_constraints_v2_typelist[j][inner_type_size].size() == 0)
            {
                if (print_products) continue;
                generate_all_types_containing_one_subtype(inner_type_size, type, g_flag_square_linear_constraints_v2_typelist[j][inner_type_size]);
                g_flag_square_linear_constraints_v2[j][inner_type_size].resize(g_flag_square_linear_constraints_v2_typelist[j][inner_type_size].size());
                
                //if (print_blocks)
                //    cerr << "Generated " << g_flag_square_linear_constraints_v2_typelist[j][inner_type_size].size() << " inner type(s)" << endl;
                
            }
            
            for (int it = 0; it < (int)g_flag_square_linear_constraints_v2_typelist[j][inner_type_size].size(); it++)
            {
                
                flag inner_type = g_flag_square_linear_constraints_v2_typelist[j][inner_type_size][it];
                
                
                if ( g_flag_square_linear_constraints_v2[j][inner_type_size][it].size() ==  0)
                {
                    if (print_products) break;
                    get_labeled_flags_of_one_type(flag_size, inner_type, g_flag_square_linear_constraints_v2[j][inner_type_size][it]);
                    if (g_flag_square_linear_constraints_v2[j].size() == 0)
                    {
                        cerr << "Linear constraint " << j+1 << " cannot be used since generating of other flags of type " <<  type.print() << " and size " <<  flag_size  << " failed" << endl;
                        continue;
                    }
                    //if (print_blocks)
                    //    cerr << "Generated " << g_flag_square_linear_constraints_v2[j][inner_type_size][it].size() << " flags of type " << inner_type.print() << endl;
                    
                    
                }
                
                //if (verbose_output)
                //    cerr << "Constraints " << j << " is using "  << g_flag_square_linear_constraints_v2[j][inner_type_size][it].size() << " flags " << endl;
                //for (int i = 0; i < g_flag_square_linear_constraints_v2[j].size(); i++)
                //{
                //    cerr << g_flag_square_linear_constraints_v2[j][i].print() << endl;
                //}
                
                
                
                if (print_blocks)
                {
                    ostr << g_flag_square_linear_constraints_v2[j][inner_type_size][it].size() << " ";
                }
                
                if (print_products)
                {
                    //cerr <<  g_unlabeled_flags[Kn][i].print() << endl;
                    
                    for (unsigned int x = 0; x < g_flag_square_linear_constraints_v2[j][inner_type_size][it].size(); x++)
                        for (unsigned int y = x; y < g_flag_square_linear_constraints_v2[j][inner_type_size][it].size(); y++)
                        {
                            flag F1 = g_flag_square_linear_constraints_v2[j][inner_type_size][it][x];
                            flag F2 = g_flag_square_linear_constraints_v2[j][inner_type_size][it][y];
                            
                            // product
                            vector<flag_and_coefficient> F1F2 = F1_times_F2(F1, F2, type);
                            
                            // constant
                            double d = 0;
                            for (int z = 0; z < (int)F1F2.size();z++)
                            {
                                d += g_linear_constraints[j].m_constant * F1F2[z].coefficient * P_F1_IN_H(F1F2[z].g, g_unlabeled_flags[Kn][i]);
                                                       
                                for (int k = 0; k < (int)g_linear_constraints[j].m_entries.size(); k++)
                                {
                                
                                    //cerr << "Computing poduct " << F1.print() << "X" << F2.print() << " =" << F1F2[z].g.print() << " X " << g_linear_constraints[j].m_entries[k].g.print() << " in " << g_unlabeled_flags[Kn][i].print() << endl;
                                    double PF1F2g = P_F1_F2_IN_labeled_H(F1F2[z].g,g_linear_constraints[j].m_entries[k].g, g_unlabeled_flags[Kn][i]);
                                
                                    d += PF1F2g * F1F2[z].coefficient * g_linear_constraints[j].m_entries[k].coefficient; 
                                
                                }
                            }
                            if (d != 0)
                            {
                                ostr.precision(G_PRECISION);
                                 ostr << matrixID <<  " " << blockID+constraint_id << " " << x+1 << " " << y+1 << " " <<  smart_round(d) << endl;
                            }
                        }
                }                
                constraint_id++;
            }
            
        }
    }    
    
    if (print_blocks)
    {
        cerr << "Square linear constraints are working for " << constraints_applicable << "/" << g_linear_constraints.size() << " constraints" << endl;
        cerr << "Number of used blocks is " << constraint_id << endl;
        
    }
    
    //cerr << "Returning " << constraint_id << endl;
    
    return constraint_id;
}



bool has_slack(int Kn, int i)
{
    if (find_flag_in_list(g_unlabeled_flags[Kn][i],g_no_slack_flags) != -1)
    {
        cerr << "Flag " << i << " has no slack" << endl;
    }
    return (find_flag_in_list(g_unlabeled_flags[Kn][i],g_no_slack_flags) == -1);
}

// When calculating parts of the SDP in parallel, we want to have a nice ordered output
// and we do not want to wait for the things to be ordered. So here thing that orders the
// output. By increasing the id which is being written.
//
class parallel_output_linearizer
{
public:

    parallel_output_linearizer(ostream &ostr, int first_ID):
    m_ostr(ostr)
    {
        m_next_ID = first_ID;
    }

    void print_string(int id, const string &str)
    {
        if (id == m_next_ID)
        {
            m_ostr << str;
            m_next_ID++;
            try_writing_next();
        }
        else
        {
            m_to_write.insert(make_pair(id, str));
        }
    }

    void try_writing_next()
    {
        //std::unordered_map<std::string,double>::const_iterator got = mymap.find (input);
        auto next_found = m_to_write.find(m_next_ID);
        while (next_found != m_to_write.end())
        {
            m_ostr << next_found->second;
            m_next_ID++;
            m_to_write.erase(next_found);
            next_found = m_to_write.find(m_next_ID);
        }
    }

    bool is_empty()
    {
        return m_to_write.empty();
    }

    ostream &m_ostr;
    int m_next_ID;
    unordered_map<int, string> m_to_write;
};



int print_CSDP_constraints_header(int Kn, ostream &ostr = cout, int verbose_output = 1)
{   
    int blocks = 0;
    
    // next we have linear constraints blocks

    if (g_linear_constraints.size() != 0)
    {
        if (g_use_simple_linear_constraints)
        {
            // Simple linear constraints
            int simple_constraints = print_CSDP_simple_linear_constraints(ostr, Kn, 0, 0, 0, true, false, verbose_output);
            if (simple_constraints != 0)
            {
                //block_ID_simple_linear_constraints = blocks+1;
                blocks++;
            }
        }
        
        if (g_use_product_linear_constraints)
        {
            int product_constraints = print_CSDP_product_linear_constraints(ostr, Kn, 0, 0, 0, true, false, verbose_output);
            if (product_constraints != 0)
            {
                //block_ID_product_linear_constraints = blocks+1;
                blocks++;
            }
        }
        
#ifdef USE_SQUARE_LINEAR_CONSTRAINTS                
        int square_constraints = print_CSDP_square_linear_constraints(ostr, Kn, 0, 0, 0, true, false, verbose_output);
        if (square_constraints != 0)
        {
            //block_ID_square_linear_constraints = blocks+1;
            blocks += constraint_id;
        }
#endif
        
        if (g_use_square_linear_constraints)
        {
            int square_constraints_v2 = print_CSDP_square_linear_constraints_v2(ostr, Kn, 0, 0, 0, true, false, verbose_output);
            if (square_constraints_v2 != 0)
            {
                //block_ID_square_linear_constraints_v2 = blocks+1;
                blocks += square_constraints_v2;
            }
        }
    }  
    
    return blocks;
}

        
int print_CSDP_constraints_blocks(ostream &ostr, int Kn, int i, int matrixID, int current_csdp_block, int verbose_output = 1)
{        
    if (g_use_simple_linear_constraints)
    {
        if (print_CSDP_simple_linear_constraints(ostr, Kn, i, matrixID, current_csdp_block, false, true, verbose_output) != 0)
        {
            current_csdp_block++;
        }
    }
    
    if (g_use_product_linear_constraints)
    {
        if (print_CSDP_product_linear_constraints(ostr, Kn, i, matrixID, current_csdp_block, false, true, verbose_output) != 0)
        {
            current_csdp_block++;
        }
    }

#ifdef USE_SQUARE_LINEAR_CONSTRAINTS    
    current_csdp_block += print_CSDP_square_linear_constraints(ostr, Kn, i, matrixID, block_ID_square_linear_constraints, false, true, verbose_output);
#endif

    if (g_use_square_linear_constraints)
    {
        current_csdp_block += print_CSDP_square_linear_constraints_v2(ostr, Kn, i, matrixID, current_csdp_block, false, true, verbose_output);
    }  
    
    return current_csdp_block;
}




int print_CSDP_additional_blocks_header(int Kn, ostream &ostr, int verbose_output = 1)
{   
    int blocks = 0;
    
    for (int i = 0; i < (int)g_additional_csdp_blocks.size(); i++)
    {
        ifstream infile;
        infile.open (g_additional_csdp_blocks[i].c_str(), ifstream::in);
        if (!infile.good())
        {
            cerr << "Failed opening file " << g_additional_csdp_blocks[i] << endl;
            assert(0);
            return 0;
        }
        int blockKn = 0;
        infile >> blockKn;

        if (blockKn != Kn)
        {
            cerr << "Loading additional CSDP blocks from " << g_additional_csdp_blocks[i] << " failed." << endl;
            cerr << "Blocks were computed n=" << blockKn << " while this program is running with n="  << Kn << endl;
            assert(0);
            return 0;
        }
        
        int blocksHere = 0;
        infile >> blocksHere;
        assert(blocksHere != 0);
        
        blocks += blocksHere;
        
        for (int j = 0; j < blocksHere; j++)
        {
            int block_size=0;
            infile >> block_size;
            assert(block_size != 0);
            ostr << " " << block_size;
        }
                
        infile.close();
    }
    
    if (verbose_output)
    {
        cerr << "Added " << blocks << " additional blocks." << endl;
    }
    return blocks;
}
        
int print_CSDP_additional_blocks(int Kn, ostream &ostr, int block_offset, int verbose_output = 1)
{   
    int blocks = 0;
    
    for (int i = 0; i < (int)g_additional_csdp_blocks.size(); i++)
    {
        ifstream infile;
        infile.open (g_additional_csdp_blocks[i].c_str(), ifstream::in);
        if (!infile.good())
        {
            cerr << "Failed opening file " << g_additional_csdp_blocks[i] << endl;
            assert(0);
            return 0;
        }
        int blockKn = 0;
        infile >> blockKn;

        if (blockKn != Kn)
        {
            cerr << "Loading additional CSDP blocks from " << g_additional_csdp_blocks[i] << " failed." << endl;
            cerr << "Blocks were computed n=" << blockKn << " while this program is running with n="  << Kn << endl;
            assert(0);
            return 0;
        }
        
        int blocksHere = 0;
        infile >> blocksHere;
        assert(blocksHere != 0);
        
        blocks += blocksHere;
        
        cerr << "Copying " << blocksHere << " dat-s blocks from " << g_additional_csdp_blocks[i] << endl;

        for (int j = 0; j < blocksHere; j++)
        {
            int block_size=0;
            infile >> block_size;
            assert(block_size != 0);
        }

        // HERE we read/write lines as long as possible...
        int matrix_id;
        int block_id;
        int x;
        int y;
        string value;

        int lines_copied = 0;
        
        while(infile.good())
        {    
            infile >> matrix_id >> block_id >> x >> y >> value;
            if (!infile.good())
            {
                break;
            }
            assert(value.size() != 0);
            
            ostr << matrix_id << " " << block_id+block_offset << " " << x << " " << " " << y << " " << value << endl;  
            lines_copied++;
        }
        
        infile.close();
        if (verbose_output)
        {
            cerr << lines_copied << " lines copied from " << g_additional_csdp_blocks[i] << endl;
        } 
        block_offset += blocksHere;
    }
    
    if (verbose_output)
    {
        cerr << "Added " << blocks << " additional blocks." << endl;
    }
    
    return block_offset;
}

        
        


// According to http://plato.asu.edu/ftp/sdpa_format.txt
//
// Solved program
//   max C.X
//   subject to A_1.X = b_1
//              A_2.X = b_2
//              A_mdim.X = b_mdim
//   where X is positive semidefinite matrix
//
// In our UPPER BOUND application
//
//      min    t   
//      s.t.  D_i + [A.X]_i <= t    
//
// We add a slack variable s_i to make the <= to = and move D_i (density - constant to the other side)
// and change to maximization:
//
//      max   -t
//      s.t.  [A.X]_i + s_i - t = -D_i
//
//   C.X is just entry 0 2 1 1 -1.0 which means that in the second block,
//   the first variable is with -1. So we are maximizing -t which corresponds to minimizing t
//
// Now LOWER BOUND application
//
//      max    t
//      s.t.  D_i - [A.X]_i  >= t
//
// We add a slack variable s_i to make the >= to = and move D_i (density - constant to the other side)
// and change to maximization:
//
//      max   t
//      s.t.  [A.X]_i + s_i + t = D_i
//
// Finally, Additional constraints applications:
// Say we want to add  a*G >= c. Then we could add
//  k(a*G(sampled in H_i) -c) >= 0
//
// For lower bound we get (max t)
//     [A.X]_i + k*(a*G_i-c) + s_i + t = D_i
//
// For upper bound we get (min t)
//     [A.X]_i + k*(a*G_i-c) + s_i -t = D_i
//
//
// Notice that by definition, t >= 0
// We can write t = t1 - t2 instead and make t a free variable.
// It is done with the macro G_FREE_OBJECTIVE


void print_CSDP_specific_part(int Kn, bool upper_bound = true, ostream &ostr = cout, int verbose_output = 1)
{
    int mdim = 0;
    mdim = (int)g_unlabeled_flags[Kn].size();
    
    // hack
    int extra_onstraints  = 0;
    //extra_onstraints += print_CSDP_product_linear_constraints_extra_lines_in_csdp(ostr, Kn, 0, false, verbose_output)

    ostr << mdim + extra_onstraints << endl; //<<" =mdim" << endl; // number of constraint matrices
    
    stringstream ssblocks; // String stream for all blocks in the matrix
    int blocks = 0; // number of blocks in ssblocks
 
    // First blocks are flags products:
    for (int f = 0; f < (int)g_flags.size(); f++) ssblocks <<  g_flags[f].size() << " ";
    blocks = (int)g_flags.size();
    
    // next we have one diagonal block of slack variables
#ifdef G_FREE_OBJECTIVE
    // -2 means there are two on the objective function....
    ssblocks << -2-mdim << " ";  // get_number_of_slacks() but we don't go for it here...
#else
    ssblocks << -1-mdim << " ";  // get_number_of_slacks() but we don't go for it here...
#endif
    int objective_index = mdim+1; // Objective variables come after slacks

    blocks++;

    int first_constraint_block = blocks+1;    
 
    blocks += print_CSDP_constraints_header(Kn, ssblocks, verbose_output);
    
    int additional_blocks_offset = blocks+1;
    blocks += print_CSDP_additional_blocks_header(Kn, ssblocks, verbose_output);
    
    ostr << blocks << endl;
    ostr << ssblocks.str() << endl;
 
    for (unsigned int i = 0; i < g_unlabeled_flags[Kn].size(); i++)
    {
        ostr.precision(G_PRECISION);
        if (upper_bound) ostr << -1*smart_round(compute_objective_function_for_SDP(g_unlabeled_flags[Kn][i])) << " "; // outputing b_i
        else             ostr << smart_round(compute_objective_function_for_SDP(g_unlabeled_flags[Kn][i])) << " "; // outputing b_i
    }
    ostr << endl;
    
#ifdef G_FREE_OBJECTIVE
    if (upper_bound) 
    {
        ostr << "0 " << (int)g_flags.size()+1 << " " << objective_index   << " " << objective_index   <<  " -1" << endl; // Objective function
        ostr << "0 " << (int)g_flags.size()+1 << " " << objective_index+1 << " " << objective_index+1 <<  "  1" << endl; // Objective function
    }
    else
    { 
        ostr << "0 " << (int)g_flags.size()+1 << " " << objective_index    << " " << objective_index   <<  "  1" << endl; // Objective function
        ostr << "0 " << (int)g_flags.size()+1 << " " << objective_index+1  << " " << objective_index+1 <<  " -1" << endl; // Objective function
    }
#else
    if (upper_bound) ostr << "0 " << (int)g_flags.size()+1 << " " << objective_index    << " " << objective_index   <<  " -1" << endl; // Objective function
    else             ostr << "0 " << (int)g_flags.size()+1 << " " << objective_index    << " " << objective_index   <<  " 1" << endl; // Objective function
#endif

    mini_timer mt;

    parallel_output_linearizer output_buffer(ostr, 0);

    // printing of subflags
#pragma omp parallel for ordered schedule(dynamic)
    for (int i = 0; i < (int)g_unlabeled_flags[Kn].size(); i++)
    {            
        //cout << "flag: "; c4free_subrgaphs[e][i].print();
        if (verbose_output) 
        {
            #pragma omp critical(cerr)
            {
                std::cerr << "Computing specific part " << i+1 << "/" << g_unlabeled_flags[Kn].size()
                          << mt.report(i, g_unlabeled_flags[Kn].size()) << endl;
            }
        }
        stringstream ss;
        
        int matrixID = i + 1;
     
        // variable for slack
        if (has_slack(Kn,i))
        {
            ss << matrixID << " " << (int)g_flags.size()+1 << " " << matrixID << " " <<  matrixID << " 1" << endl;
        }

        // variable for objective function
#ifdef G_FREE_OBJECTIVE
            if (upper_bound) 
            {
                ss << matrixID << " " << (int)g_flags.size()+1 << " " << objective_index   << " " << objective_index   <<  " -1" << endl;
                ss << matrixID << " " << (int)g_flags.size()+1 << " " << objective_index+1 << " " << objective_index+1 <<  " 1" << endl;
            }
            else
            {
                ss << matrixID << " " << (int)g_flags.size()+1 << " " << objective_index   << " " << objective_index   <<  " 1" << endl;
                ss << matrixID << " " << (int)g_flags.size()+1 << " " << objective_index+1 << " " << objective_index+1 <<  " -1" << endl;
            }
#else
        double objective_divisor = 1;
        if (g_objective_divisor.size() > 0)
        {
            objective_divisor = smart_round( compute_linear_combination_in_g(g_unlabeled_flags[Kn][i], g_objective_divisor));
            if (objective_divisor != 0)
            {
                if (upper_bound) ss << matrixID << " " << (int)g_flags.size()+1 << " " << objective_index    << " " << objective_index  << -objective_divisor << endl;
                else             ss << matrixID << " " << (int)g_flags.size()+1 << " " << objective_index    << " " << objective_index  <<  objective_divisor << endl;
            }
        }
        else
        {
            if (upper_bound) ss << matrixID << " " << (int)g_flags.size()+1 << " " << objective_index   << " " << objective_index   <<  " -1" << endl;
            else             ss << matrixID << " " << (int)g_flags.size()+1 << " " << objective_index   << " " << objective_index   <<  " 1" << endl;
        }
#endif

        
                    
        //int next_block = 
            print_CSDP_constraints_blocks(ss, Kn, i, matrixID, first_constraint_block, verbose_output);                        
// We do not back-up these so whatever order works
// For serious constraints like the cuts heavy programs, this actually may make a differnece.
//#pragma omp ordered
#pragma omp critical
{
            output_buffer.print_string(i, ss.str());
            //ostr << ss.str();
}
    }
    assert(output_buffer.is_empty());

    ostr.flush();
    
    print_CSDP_additional_blocks(Kn, ostr, additional_blocks_offset, verbose_output = 1);
}

void print_CSDP_flag_products_part(int Kn, bool upper_bound = true, ostream &ostr = cout, int verbose_output = 1, bool use_sdp_temp = false, int lowest_not_computed=0)
{
    mini_timer mt;

    parallel_output_linearizer output_buffer(ostr, lowest_not_computed);

#pragma omp parallel for ordered schedule(dynamic)
    for (int i = lowest_not_computed; i < (int)g_unlabeled_flags[Kn].size(); i++)
    {            
        if (verbose_output) 
        {
            #pragma omp critical(cerr)
            {
                std::cerr << "Computing flag products " << i+1 << "/" << g_unlabeled_flags[Kn].size()
                          << mt.report(i, g_unlabeled_flags[Kn].size()) << endl;
            }            
        }

        stringstream ss;
        
        int matrixID = i + 1;
        
#ifdef G_FLAG_PRODUCTS_SLOW_LOW_MEMORY       
         for (int f = 0; f < (int)g_flags.size(); f++)
         {
             for (unsigned int x = 0; x < g_flags[f].size(); x++)
                 for (unsigned int y = x; y < g_flags[f].size(); y++)
                 {
         
                     double PF = P_F1_F2_IN_H(g_flags[f][x], g_flags[f][y], g_unlabeled_flags[Kn][i], true);
                     //					if (x == y) PF = PF*0.5;
                     if (PF != 0)
                     {
                         ss << matrixID <<  " " << f+1 << " " << x+1 << " " << y+1 << " " <<  PF << endl;
                     }
                 }
         }
#else
        count_flag_products(ss, matrixID, g_unlabeled_flags[Kn][i]);
#endif 

    #pragma omp critical (ostr)
    {
        output_buffer.print_string(i, ss.str());
    }

    /*
        // Check if we need to write the output ordered
        if (use_sdp_temp == true)
        {          
    #pragma omp ordered
            {
                ostr << ss.str();
            }
        }
        else
        {
    #pragma omp critical (ostr)
            {
                ostr << ss.str();
            }
        }
    */
    }
    assert(output_buffer.is_empty());
    ostr.flush();
}

void print_CSDP(int Kn, bool upper_bound = true, ostream &ostr = cout, int verbose_output = 1, bool use_sdp_temp=false, bool sdp_temp_up_to_date=true)
{
    cerr << "Computing specific part of the SDP..." << endl;
    print_CSDP_specific_part(Kn, upper_bound, ostr, verbose_output);
    
    if (!use_sdp_temp || sdp_temp_up_to_date == false)
    {
        cerr << "Computing flag products part of the SDP..." << endl;
        print_CSDP_flag_products_part(Kn, upper_bound, ostr, verbose_output, use_sdp_temp);
        return;
    }
    
    cerr << "Getting flag products for SDP..." << endl;    
    
    stringstream filename;
    
    filename << filename_prefix() << "__n" << Kn << "_sdp_products.txt";

    // The sdp_product_file may not contain all products if the computation
    // was accidentally interrupted. We try to go from we stopped...
    // First find were we stopped
    
    int lowest_not_computed = 0;

    ifstream sdp_product_file;
    sdp_product_file.open (filename.str().c_str(),ifstream::in);
    while (sdp_product_file.good())
    {
        int id, type, f1, f2, d;
        sdp_product_file >> id >> type >> f1 >> f2 >> d;
        if (!sdp_product_file.good()) break; // if the read failed, do not use it...
        if (id > lowest_not_computed)
        {
            lowest_not_computed = id; // note that in file the ID is +1
        }
    }
    sdp_product_file.close();
    
    if (lowest_not_computed < (int)g_unlabeled_flags[Kn].size())
    {
        if (lowest_not_computed != 0)
        {
            cerr << "Warning: file " << filename.str() << " contains only products up to " << lowest_not_computed << " out of " << (int)g_unlabeled_flags[Kn].size() << endl;
            cerr << "          We assume it happened by accident so the rows starting with " << lowest_not_computed << " will be deleted and recomputed" << endl;
            stringstream mv_command;
            mv_command << "rm -f "<< filename.str() << "~ ;  mv " << filename.str() << " " << filename.str() << "~";
            cerr << "          Executing " << mv_command.str() << endl;
            if (system(mv_command.str().c_str()) != 0)
            {
                cerr << " Execution failed" << endl;
                exit(1);
            }
            // We remove the last line since it may be incoplete and not catched by the grep
            stringstream grep_command;
            grep_command << "sed \\$d " << filename.str() << "~ |   grep -v '^" << lowest_not_computed << " ' "  << " >" << filename.str();
            cerr << "          Executing " << grep_command.str() << endl;
            if (system(grep_command.str().c_str()) != 0)
            {
                cerr << " Execution failed" << endl;
                exit(1);
            }
            lowest_not_computed--;
        }
     
        ofstream sdp_product_file_out;
        sdp_product_file_out.open (filename.str().c_str(),ofstream::out|ofstream::app);
        if (!sdp_product_file_out.good())
        {
            cerr << "Failed creating file with sdp products " << filename.str() << endl;
            exit(1);
        }
        
        cerr << "Computing flag products of the SDP to file " << filename.str() << endl;
        print_CSDP_flag_products_part(Kn, upper_bound, sdp_product_file_out, verbose_output, use_sdp_temp, lowest_not_computed);
        sdp_product_file_out.close();
    }

   
    sdp_product_file.open (filename.str().c_str(),ifstream::in);
    if (!sdp_product_file.good())
    {
        cerr << "Failed obtaining file with sdp products " << filename.str() << endl;
        exit(1);
    }
    
    cerr << "Copying flag products of the SDP from " << filename.str() << endl;
    ostr << sdp_product_file.rdbuf();
    sdp_product_file.close();
    
    /*
    
    ifstream sdp_product_file;
    sdp_product_file.open (filename.str().c_str(),ifstream::in);
    if (!sdp_product_file.good())
    {
        cerr << "Failed opening file with unlabeled flags " << filename.str() << endl;
        
        ofstream sdp_product_file_out;
        sdp_product_file_out.open (filename.str().c_str(),ofstream::out);
        if (!sdp_product_file_out.good())
        {
            cerr << "Failed creating file with sdp products " << filename.str() << endl;
            exit(1);
        }
        
        cerr << "Computing flag products of the SDP to file " << filename.str() << endl;
        print_CSDP_flag_products_part(Kn, upper_bound, sdp_product_file_out, verbose_output);
        sdp_product_file_out.close();
        
        sdp_product_file.open (filename.str().c_str(), ifstream::in);        
    }

    if (!sdp_product_file.good())
    {
        cerr << "Failed obtaining file with sdp products " << filename.str() << endl;
        exit(1);
    }

    cerr << "Copying flag products of the SDP from " << filename.str() << endl;    
    ostr << sdp_product_file.rdbuf();
    sdp_product_file.close();
     */
}



void process_csdp_solution(istream &ist, int Kn, double lower_bound, double upper_bound, bool print_density)
{

	// Print out flags of the first type:
//   for (unsigned int f0 = 0; f0 < g_flags[0].size(); f0++)
//   {
//   	cout << f0+1 << ": ";
//		g_flags[0][f0].print();
//   }

    // First line is just solution to the dual - Y
	// which mean combination of flags...
	
	double sum = 0;
    double sum_print = 0;
    double count_printed = 0;
	
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size(); i++)
	{
		double density;
		ist >> density;
		
		sum += density;
		
		if (lower_bound <= density && density < upper_bound)
		{
            count_printed++;
            sum_print += density;
            if (print_density)
            {
                std::cout.fill(' ');
                std::cout.width(G_PRECISION+1);
                std::cout.precision(G_PRECISION);
                std::cout << std::left << density << " ";
            }
            
			//cout << density << " ";
			//if (g_unlabeled_flags[Kn][i].count_crossings() == 2)
			{
                std::cout << g_unlabeled_flags[Kn][i].print() << "  # " << i+1 << endl;
			}
		}
	}
	
	cerr << "Printed: " << count_printed << "/" <<  g_unlabeled_flags[Kn].size() << "  Sum of coefficients: " << sum_print << "/" << sum  << endl;
	cerr << "Numering is 1.." << g_unlabeled_flags[Kn].size() << endl;
	return;
	
    /*
    // ist.ignore(numeric_limits<int>::is_signed,'\n');
    char c = ' ';
    while (ist.good() && c != '\n') ist.get(c);
    
    // Second and few other lines are starting with 1
    // That is a matrix that was result of the dual program
    // and we discard it
    
    int matrixID, block, x, y;
    long double value;
    while(ist.good())
    {
        ist >> matrixID >> block >> x >> y >> value;
 
        //cout << "I got: " << matrixID << " " << block << " " << x << " " << y << " " << value << endl;
        
        if (matrixID == 1) continue;
        assert(matrixID == 2);
     
        //ignore almost zeros
        if (abs(value) < 0.0000001) continue;
        
        // ignore those that are just helper variables
        // Every flag type has its own block
        // blocks after (int)g_flags.size() are just slack variables
        int flag_type = block - 1;
        if (flag_type >= (int)g_flags.size()) continue;
            
            // Little bit of debugging
			if (flag_type != 0) continue; 
			              
        cout << block << " " << x << " " << y << " " << value << endl;
    }
     */
}





// This is how mosek sol file usually looks like
// NAME                :
// PROBLEM STATUS      : PRIMAL_AND_DUAL_FEASIBLE
// SOLUTION STATUS     : OPTIMAL
// OBJECTIVE NAME      : OBJ
// PRIMAL OBJECTIVE    : -0.0017266409728998
// DUAL OBJECTIVE      : -0.0017266416878761
// AFFINE CONIC CONSTRAINTS
// INDEX      NAME               I          ACTIVITY                 DUAL
// 0          A1                 0          -3.2742472370617e-06     -2.4760664317725e-05
// 1          A1                 1          2.5607477481104e-07      -0.0021507445817033
// 2          A1                 2          1.5437798352169e-07      -0.0013164480464409
// ...
// 8156       A1                 8156       -2.0705738643966         -1.8795865867501e-11
//
// VARIABLES
// INDEX      NAME               AT ACTIVITY                 LOWER LIMIT        UPPER LIMIT        DUAL LOWER               DUAL UPPER
// 0          X1                 SB -0.0017266409728998      NONE               NONE               0                        0
// 1          X2                 LL 8.8777832330277e-05      0                  NONE               9.3929067286518e-07      0
// ... 
void process_mosek_solution(const string& path, int Kn, double lower_bound, double upper_bound, bool print_density)
{
    OPEN_FILE_SMARTLY(istr, path);

    string s;
    while (s != "CONSTRAINTS")
    {
        (*istr) >> s;
        if ((*istr).good() == false)
        {
            cerr << "Failed finding constraints block" << endl;
            return;
        }
    }

	double sum = 0;
    double sum_print = 0;
    double count_printed = 0;
    int i = 0;

    std::string line;
    while (std::getline((*istr), line))
    {
        std::istringstream iss(line);

        string graph_ID; 
        if (!getline( iss, graph_ID, ' ' )) continue;

        if (graph_ID == "INDEX") continue;
        if (graph_ID == "VARIABLES")
        {
            cerr << "This should not happen. Processing should stop earlier." << endl;
            break;
        }

        // Density is the last entry
        string density_str = "";
        string tmpstr;
        while ( getline( iss, tmpstr, ' ' ) ) 
        {
            if (tmpstr.length() != 0) 
            {
                density_str = tmpstr;
            }
        }

        //cerr << "Read: " << graph_ID << " " << density_str << endl;

        if (i != stol(graph_ID))
        {
            cerr << "Unexpected index " << graph_ID << " while " << i << " was expcted" << endl;
            break;
        }
        // 

        // The dual is negative. I dont know why....
		double density = -stod(density_str);
		
		sum += density;
		
		if (lower_bound <= density && density < upper_bound)
		{
            count_printed++;
            sum_print += density;
            if (print_density)
            {
                std::cout.fill(' ');
                std::cout.width(G_PRECISION+1);
                std::cout.precision(G_PRECISION);
                std::cout << std::left << density << " ";
            }
            
			//cout << density << " ";
			//if (g_unlabeled_flags[Kn][i].count_crossings() == 2)
			{
                std::cout << g_unlabeled_flags[Kn][i].print() << "  # " << i+1 << endl;
			}
		}

        i++;

        // We read them all
        if (i == (int)g_unlabeled_flags[Kn].size()) break;
    }

	
	cerr << "Printed: " << count_printed << "/" <<  g_unlabeled_flags[Kn].size() << "  Sum of coefficients: " << sum_print << "/" << sum  << endl;
	cerr << "Numering is 1.." << g_unlabeled_flags[Kn].size() << endl;
}






/*
void merge_typed_flags_lists_parallel(vector< vector<flag> > &flags_a, vector< vector<flag> > &flags_b)
{
    vector< vector<flag> > new_from_b;
    vector<int> new_from_b;
    new_from_b.reserve(flags_b.size());
    
    for (int i = 0; i < (int)flags_b.size(); i++)
    {
        //cerr << i << "/" << flags_b.size() << endl;
        if (flags_b[i].size() == 0)
        {
            cerr << "Some bugggg" <<endl;
            assert(0);
        }
        int type_id = get_flag_type_in_list(flags_b[i][0], flags_a);
        if (type_id == -1)
        {
            new_from_b.push_back(i);
            //cerr << "push begin" << endl;
            //new_from_b.push_back(flags_b[i]);  // could be done using move
            //cerr << "push end" << endl;
            continue;
        }
#ifdef USE_REDUCED_TYPES    
        if  (type_id == -2) continue; // unsued (reduced types)
#endif        
        
        assert(flags_a[type_id][0].have_same_type(flags_b[i][0]));
        
        cerr << "pm begin " << type_id << endl;
        merge_flags_lists_parallel(flags_a[type_id], flags_b[i]);
        cerr << "pm end" << endl;
    }

    //cerr << "Initial part done" << endl;
    
#ifdef DONT_USE_C11    
 //   flags_a.insert(flags_a.end(), new_from_b.begin(), new_from_b.end() );                                
#else
 //   flags_a.insert(flags_a.end(), std::make_move_iterator(new_from_b.begin()), std::make_move_iterator(new_from_b.end()) );                                
#endif
}
*/



// Not paralel version - very slow and not completely correct - in particular for ordered version.
void generate_labeled_flags_old(int flag_size, int type_size, int verbose_output)
{
#ifdef G_ORDERED_VERTICES
    assert(0);
#endif
    int to_label_size = (int)g_unlabeled_flags[flag_size].size();
    //cerr << "Type size " << type_size << " for " << g_unlabeled_flags[flag_size][i].print() << endl;
    int mapping[flag_size];
    flag F;
    
    for (int i = 0; i < to_label_size; i++)
    {
        if (verbose_output)
            cerr << "Flag size "<<flag_size << " type size " << type_size << " labeling " << i << "/" << to_label_size << endl;

        for (int j = 0; j < flag_size; j++) mapping[j]=j;
        
        do {
            F.as_subflag(g_unlabeled_flags[flag_size][i], mapping, flag_size, type_size); // taking a subflag
            include_flag_in_list_if_new(F,g_flags);
        } while ( std::next_permutation(mapping,mapping+flag_size) );
    }
}


//vector<flag> g_types[V];
void create_types_of_size(int type_size, bool check_sensible_flags=false)
{
    if (g_types[type_size].size() != 0) return;


    // Look if there are already some flags with given types...
    for (int i = 0; i < (int)g_flags.size(); i++)
    {
        if (g_flags[i].size() == 0) continue;
        
        if (g_flags[i][0].labeled_vertices_cnt() == type_size)
        {
            flag type;
            g_flags[i][0].get_type_subflag(type);
            g_types[type_size].push_back(type);
        }
    }
    if (g_types[type_size].size() != 0) return;
    
    
    // if not, just label graphs of apropriate size
    get_unlabeled_flags_of_size(type_size);
    
    if (!check_sensible_flags)
    {
        g_types[type_size] = g_unlabeled_flags[type_size];
    }
    else
    {
        for (int i = 0; i < (int)g_unlabeled_flags[type_size].size(); i++)
        {
#ifdef G_COLORED_VERTICES
            bool type_ok = true;
            for (int c = 1; c < COLORS_VERTICES; c++)
            {
                if (g_exact_number_of_colored_vertices[c] != -1)
                {
                    if ((g_exact_number_of_colored_vertices[c] < g_unlabeled_flags[type_size][i].m_colored_vertices[c]) ||                        
                       ((g_exact_number_of_colored_vertices[c] - g_unlabeled_flags[type_size][i].m_colored_vertices[c])%2 != 0))
                    {
                        type_ok = false;
                        break;
                    }
                }
            }
            if (!type_ok) continue;
#endif
            g_types[type_size].push_back(g_unlabeled_flags[type_size][i]);
        }
    }
    
#pragma omp parallel for
    for (int i = 0; i < (int)g_types[type_size].size(); i++)
    {
        g_types[type_size][i].make_all_vertices_labeled();
        //
        cerr << "New type: " << g_types[type_size][i].print() << endl;
    }
    
    
    cerr << "Generated " << g_types[type_size].size()  << " types of size " << type_size << endl;
}

int get_reduced_type_id(const flag &f)
{
    int type_size = f.labeled_vertices_cnt();
    
    //cerr << "Testing size " << type_size << " " << endl;

    
    for (int i = 0; i < (int)g_types[type_size].size(); i++)
    {
        if (f.have_same_type(g_types[type_size][i]))
        {
            return i;
        }
    }    
    
    /*
    int found_type = -1;
    for (int i = 0; i < (int)g_types[type_size].size(); i++)
    {
        if (f.have_same_type(g_types[type_size][i]))
        {
            if (found_type == -1)
            {
                found_type=i;
            }
            else
            {
                cerr << "Type confusion! " << endl;
                cerr << f.print() << endl;
                cerr << g_types[type_size][i].print() << endl;
                cerr <<  g_types[type_size][found_type].print() << endl;
                exit(1);
            }
        }
    }
    return found_type;
    */
    return -1;
}


void get_types_of_size(int type_size, bool types_from_file, bool check_sensible_flags=false)
{
//    create_types_of_size(type_size);
//    return;
        
    if (!types_from_file)
    {
        create_types_of_size(type_size, check_sensible_flags);
        cerr <<  g_types[type_size].size() <<  " types of size " << type_size << " generated" << endl;
        return;
    }
    
    string filename = filename_prefix() + "__type_"+ to_string(type_size) +".txt";
    
    if (!load_flags_from_file(filename, g_types[type_size]))
    {
        create_types_of_size(type_size, check_sensible_flags);
        dump_flags_to_file(filename, g_types[type_size]);
    }
    else
    {
        cerr <<  g_types[type_size].size() <<  " types of size " << type_size << " loaded from file " << filename << endl;
    }
}


//
void generate_labeled_flags(int flag_size, int type_size, int verbose_output, bool types_from_file=false, bool check_sensible_flags=false)
{        
    // First make types
    get_types_of_size(type_size, types_from_file, check_sensible_flags);
    
    //cerr << "B:EEEE  " << g_types[type_size].size() << endl;
    //cerr <<  "flag_size=" << flag_size << " type_size=" << type_size << endl;
    //cerr << "check_sensible_flags=" << check_sensible_flags << endl;
    
    int to_label_size = (int)g_unlabeled_flags[flag_size].size();
    
    // 
    vector< vector<flag> >  *flags;
    flags = new vector< vector<flag> >[to_label_size];
    
    int types = (int)g_types[type_size].size();
    
    // Avoid copying flags
    for (int i = 0; i < to_label_size; i++)
    {
        flags[i].resize(types);
    }
    
#ifdef G_ORDERED_VERTICES
    int labeled[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    for (int j = 0; j < type_size; j++)
    {
        labeled[flag_size-1-j] = 1;
    }    
    do {
        //for (int j = 0; j < flag_size; j++) cerr << labeled[j] << " ";
        //cerr << endl;
        for (int i = 0; i < to_label_size; i++)
        {
            flag F;
            F = g_unlabeled_flags[flag_size][i];
            int Theta=0;
            for (int j = 0; j < flag_size; j++)
            {
                Theta = (Theta << 1) | labeled[j];
            }
            F.set_Theta(Theta);

            int type_of_F = get_reduced_type_id(F);
            if (type_of_F == -1)
            {
                //cerr << "Testing labeled flag " << F.print() << " type " << type_of_F << endl;
                flag g;
                F.get_type_subflag(g);
                //cerr << "Type: " << g.print() << endl;
            }
            if (type_of_F == -1) continue;
            //if (!g_already_in_known_flags(F,flags[i][type_of_F])) // they are always new
            {
                //cerr << "Adding " << F.print() << endl;
                //flags[i][type_of_F].push_back(F);
                include_flag_in_list_if_new(F, g_flags);
            }
        }
    } while ( std::next_permutation(labeled,labeled+flag_size) );
    
#else
    cerr << "Generating labeled flags for Cauchy-Swartz in parallel " << endl;
#pragma omp parallel for ordered
    for (int i = 0; i < to_label_size; i++)
    {
        if (verbose_output)
            cerr << "Flag size "<<flag_size << " type size " << type_size << " labeling " << i << "/" << to_label_size << endl;

        //cerr << "Type size " << type_size << " for " << g_unlabeled_flags[flag_size][i].print() << endl;
        // Permutations for labeling... Could be improved to distinguish only the first type_size vertices.
        int mapping[flag_size];
        flag F;
        for (int j = 0; j < flag_size; j++) mapping[j]=j;
        
        //int found_good_type = 0;
        do {
            F.as_subflag(g_unlabeled_flags[flag_size][i], mapping, flag_size, type_size); // taking a subflag
            //cerr << "Got one " << F.print() << endl;
            int type_of_F = get_reduced_type_id(F);
            if (type_of_F == -1)
            {
                 //cerr << "Superfluous type" << endl;
                // We have each type only once -  3 3   1 1  2  and  3 3   1 2  1 are different types but enough to use just one
                continue;
            }
            else
            {
                // cerr << "type_of_F = " << type_of_F << endl;
            }
            //found_good_type++;
            if (!g_already_in_known_flags(F,flags[i][type_of_F]))
            {
                if (check_sensible_flags)
                {                    
                    bool flag_OK = true;
#ifdef  G_COLORED_VERTICES                   
                    for (int c = 1; c < COLORS_VERTICES; c++)
                    {
                        if (g_exact_number_of_colored_vertices[c] == -1) continue;
                        int c_labeled = g_types[F.m_Theta][type_of_F].m_colored_vertices[c];
                        if ((g_exact_number_of_colored_vertices[c] < F.m_colored_vertices[c]) ||                        
                            (g_exact_number_of_colored_vertices[c] - c_labeled !=  2*(F.m_colored_vertices[c] - c_labeled) ) )
                        {
                            flag_OK = false;
                            break;
                        }
                    }
#endif                    
                    if (!flag_OK) continue;
                }

                
                    flags[i][type_of_F].push_back(F);
            }
            //include_flag_in_list_if_new(F,flags[i],false);
        } while ( std::next_permutation(mapping,mapping+flag_size) );
        //cout << "Found good type " << found_good_type << endl;
    }
    
    // Todo: this might be also maybe paralelized - there is some paralelism inside...
    for (int i = 0; i < to_label_size; i++)
    {
        for (int j = 0; j < (int)flags[i].size(); j++)
            for (int k = 0; k < (int)flags[i][j].size(); k++)
                include_flag_in_list_if_new(flags[i][j][k], g_flags);
        
        //merge_typed_flags_lists_parallel(g_flags, flags[i]);
    }
#endif
    
    for (int t = 0; t < types; t++)
    {
        int found_t = 0;
        for (int i = 0; i < to_label_size; i++)
        {
            found_t +=  (int)flags[i][t].size();
        }        
        if (found_t == 0)
        {
            cerr << "Type " << t << " has no labeled flags" << endl;
        }
    }
  
    for (int t = 0; t < (int)g_flags.size(); t++)
    {
        if (g_flags[t].size() == 1)
        {
            cerr << "Type " << t << " has only one labeled flag. It is not useful so we pretend it is zero." << endl;
            g_flags.erase(g_flags.begin() + t);
            t--;
        }
    }


/*        
    // Merging together to flags[0]
    cerr << "Merging flags  " << endl;
    for (int power = 1; power < to_label_size; power *= 2)
    {
//        if (verbose_output)
        {
        //    cerr << "Trying " << power << " in " << to_label_size << endl;
        }                
        for (int j = 0; j < to_label_size; j += 2*power)
        {   
            if (j+power < to_label_size)
            {
                //if (verbose_output)
                {
                //    cerr << "Merging  " << j << "<-" << j+power << "  of sizes " <<  flags[j].size() << " " << flags[j+power].size() << endl;
                }
                for (int t = 0; t < types; t++)
                {
                    merge_flags_lists_parallel(flags[j][t],flags[j+power][t]);
                }
            }
        }
    }
    
    // Merging to g_flags
    cerr << "Final merge " << g_flags.size() << " " << flags[0].size() << endl;
    for (int t = 0; t < types; t++)
    {
        g_flags.push_back(flags[0][t]);
    }
*/    
    //merge_typed_flags_lists_parallel(g_flags, flags[0]);

    //cerr << "delete[] " << endl;

    delete[] flags;

    //cerr << "done " << endl;
}


//////////////////////////////////////////////////////// LOAD & DUMP
//////////////////////////////////////////////////////// LOAD & DUMP
//////////////////////////////////////////////////////// LOAD & DUMP
//////////////////////////////////////////////////////// LOAD & DUMP
//////////////////////////////////////////////////////// LOAD & DUMP
//////////////////////////////////////////////////////// LOAD & DUMP


// return true if constraint added
bool add_linear_constraint_if_new(linear_constraint &lc, bool test_with_type_permutations, vector<linear_constraint> &where_to_add = g_linear_constraints, bool use_openmp = false)
{
    if (use_openmp == false)
    {
        for (const linear_constraint &lc_known : where_to_add)
        {
            if (lc_known == lc)
            {
                //cerr << "Duplicate linear constraint..." << endl;
                return false;
            }

            if (test_with_type_permutations && lc.is_identical_after_type_permutation(lc_known))
            {
                //cerr << "Duplicate after relabeling found" << endl;
                // assert(0);
                return false;
            }
        }
    }
    else // parallel version
    {
        bool constraint_is_new = true;

        #pragma omp parallel for ordered schedule(dynamic)
        for (int i = 0; i < (int)where_to_add.size(); i++)
        {
            const linear_constraint &lc_known = where_to_add[i];
            if (constraint_is_new == false) continue;

            if (lc_known == lc)
            {
                #pragma omp critical
                constraint_is_new = false;
                continue;
            }

            if (test_with_type_permutations && lc.is_identical_after_type_permutation(lc_known))
            {
                #pragma omp critical
                constraint_is_new = false;
            }
        }
        if (constraint_is_new == false)
            return false;
    }


    where_to_add.push_back(lc);  
    return true;
}

void remove_constraints_implied_by_others(int verbose_output)
{
    int to_be_removed_counter = 0;


    std::cerr << "Testing useless constraints. To test " << g_linear_constraints.size() << endl;

    vector<linear_constraint> all_linear_constraints(std::move(g_linear_constraints));
    g_linear_constraints.clear();
    g_linear_constraints.reserve(all_linear_constraints.size());


    mini_timer mt;

//    for (const linear_constraint &lc_to_test : all_linear_constraints)
//    const linear_constraint &lc_to_test = g_linear_constraints[0];
    #pragma omp parallel for ordered schedule(dynamic)
    for (int i = 0; i < (int)all_linear_constraints.size(); i++)
    {

        if (verbose_output)
        {
            #pragma omp critical(cerr)
            {
                cerr << "Testing constraint " << i << " out of " << all_linear_constraints.size()
                     << mt.report(i, all_linear_constraints.size()) << endl;
            }
        }

        const linear_constraint &lc_to_test = all_linear_constraints[i];

        bool useful_constraint = true;

        for (const linear_constraint &lc : all_linear_constraints)
        {
            // skip icomparable constriants or is we know that
            // lc cannot be < lc_to_test
            if (lc.m_constant > lc_to_test.m_constant) continue;
            if (lc.m_labeled_vertices_in_type_cnt != lc_to_test.m_labeled_vertices_in_type_cnt) continue;
            if (lc.m_entries_max_size > lc_to_test.m_entries_max_size) continue;
            if (lc.m_entries.size() > lc_to_test.m_entries.size()) continue;
            if (!lc_to_test.m_type.is_isomorphic_to(lc.m_type)) continue;


            // For every flag in lc_to_test, its coefficient in lc is >=
            bool found_one_strict_less = false;
            bool lc_is_not_smaller = false;
            int  matches_not_found = 0;

            for (const flag_and_coefficient &entry_test : lc_to_test.m_entries)
            {
                // Test if it is the same as some other entry in lc
                bool found_match = false;
                for (const flag_and_coefficient &entry2 : lc.m_entries)
                {
                    if (entry_test.g.is_isomorphic_to(entry2.g))
                    {
                        if (entry_test.coefficient < entry2.coefficient)
                        {
                            lc_is_not_smaller = true;
                            break;
                        }

                        if (entry_test.coefficient > entry2.coefficient)
                        {
                            found_one_strict_less = true;
                        }

                        found_match = true;
                        break;
                    }
                }
                if (found_match == false) 
                {
                    found_one_strict_less = true;
                    matches_not_found++;
                }
                if (lc_is_not_smaller) 
                {
                    break;
                }
            }

            if (lc_is_not_smaller) continue;
            if (!found_one_strict_less) continue;
            if (lc.m_entries.size()+matches_not_found != lc_to_test.m_entries.size()) continue;

            if (verbose_output >= 2)
            {
                // We found and extra one
                cerr << "I found a linear constraint that is not needed" << endl;
                cerr << lc_to_test << "is weaker than \n" << lc << endl;
            }
            to_be_removed_counter++;
            useful_constraint = false;
            break;
        }

        #pragma omp ordered
        if (useful_constraint)
        {
            // This is causing a trouble since it can reorder the constraints
            //#pragma omp critical (g_linear_constraints)
            {
                g_linear_constraints.push_back(lc_to_test);
            }
        }
    }
    cerr << "Found " << to_be_removed_counter << " not needed constraints." << endl;
    cerr << "Current number of constraints is " << g_linear_constraints.size() << endl;
}

void simplify_FC(vector<flag_and_coefficient> &FC, double epsilon=0)
{
    vector<flag_and_coefficient> new_FC;
    
    for (int j = 0; j < (int)FC.size(); j++)
    {
        bool processed = false;
        for (int k = 0; k < (int)new_FC.size(); k++)
        {
            if (FC[j].g.is_isomorphic_to(new_FC[k].g))
            {
                new_FC[k].coefficient += FC[j].coefficient;
                processed = true;
                break;
            }
        }
        if (!processed)
        {
            flag_and_coefficient fc = FC[j];
            new_FC.push_back(fc);
        }
    }
    
    FC.clear();
    for (int j = 0; j < (int)new_FC.size(); j++)
    {
        if (abs(new_FC[j].coefficient) > epsilon)
        {
            double rounded_coefficient = smart_round(new_FC[j].coefficient, epsilon);
            if (rounded_coefficient != 0)
            {
                new_FC[j].coefficient = smart_round(new_FC[j].coefficient, epsilon);
                FC.push_back(new_FC[j]);
            }
        }
    }

//    FC = new_FC;
}


bool add_and_polish_constraint_if_useful(linear_constraint &lc, bool test_duplicates_with_type_permutations, int verbose_output)
{

#ifdef G_USE_INDEXES_FOR_LARGE_UNLABELED_FLAGS
        #pragma omp parallel for
        for (int i = 0; i < (int)lc.m_entries.size(); i++)
        {
            auto &fc = lc.m_entries[i];
            if (fc.g.m_Theta == 0 && fc.g.m_unlabeled_id == -1 )
            {
                fc.g.m_unlabeled_id = find_flag_in_list(fc.g, g_unlabeled_flags[fc.g.m_vertices]);
            }
        }
#endif 
        // Sometimes linear constraints may contain same entry several times
        // so we just sum these all up
        simplify_FC(lc.m_entries);
        
        if (lc.m_entries.size() == 0 && lc.m_constant == 0)
        {
            if (verbose_output >= 2)
            {
                cerr << "Constraint empty after simplification" << endl;
            }
            return false;
        }

        lc.check_constraint();
        if (lc.m_checked == false)
        {
            cerr << "check_constraint() failed for constraint:" << endl;
            cerr << lc << endl;
        }
        assert(lc.m_checked);

        return add_linear_constraint_if_new(lc, test_duplicates_with_type_permutations,g_linear_constraints, true);
}

void load_linear_constraints_from_stream(istream &instream, bool test_duplicates_with_type_permutations, bool remove_implied_constraints, int verbose_output)
{
    //
    flag_and_coefficient fc;
    string lc_constat;

    int removed_duplicate_constraints = 0;
    int processed_constraints = 0;

    while (instream)
    {
        linear_constraint lc;
        instream >> lc.m_constant;
        if(!instream.good()) {
            // Not a number means comment continues...
            std::istreambuf_iterator<char> eos;
            std::string s(std::istreambuf_iterator<char>(instream), eos);
            g_program_description = s;
            break;
        }
        processed_constraints++;
        instream >> fc.coefficient;
        
        if (verbose_output >= 3)
            cerr << "Loading linear constraint 0 <= " << lc.m_constant <<  endl;
                
        assert (fc.coefficient != 0);
        
        while (instream && fc.g.load_from_stream(instream,-1,-1))
        {
            if (verbose_output >= 3)
                cerr << "..+..  " << fc.coefficient << " * " << fc.g.print() << endl;
            
            lc.m_entries.push_back(fc);
            instream >> fc.coefficient;

            if (fc.coefficient == 0)
            {
                break;
            }
        }

        //if (!add_linear_constraint_if_new(lc, test_duplicates_with_type_permutations,g_linear_constraints, true))

        if (!add_and_polish_constraint_if_useful(lc, test_duplicates_with_type_permutations, verbose_output))
        {
            removed_duplicate_constraints++;
        }

        if (verbose_output >= 2)
            cerr << "Processed " << processed_constraints << "  constraints, found " << removed_duplicate_constraints << " duplicates " <<  endl;


    }
    if (removed_duplicate_constraints > 0)
    {
        cerr << "Removed " << removed_duplicate_constraints << " duplicate constriants." << endl;
    }

    if (remove_implied_constraints)
    {
        remove_constraints_implied_by_others(verbose_output);
    }
}



bool load_linear_constraints_from_file(const string &filename, bool test_duplicates_with_type_permutations, int verbose_output)
{
    OPEN_FILE_SMARTLY_RETURN_FALSE_ON_FAIL(istr, filename);

    int tmp=3;
    (*istr) >> tmp;
    assert(tmp == 0);
    load_linear_constraints_from_stream(*istr, test_duplicates_with_type_permutations, test_duplicates_with_type_permutations, verbose_output);
    return true;

/*

    ifstream infileF;
    infileF.open (filename.c_str(), ifstream::in);

    if (infileF.good())
    {
        FilteringIstream infile(infileF);
    
        cerr << "Loading additional constraints from file " << filename  <<  endl;
        int tmp;
        infile >> tmp;
        assert(tmp == 0);
        load_linear_constraints_from_stream(infile, test_duplicates_with_type_permutations, test_duplicates_with_type_permutations, verbose_output);
        infileF.close();
        return true;
    }

    cerr << "Failed opening file with objective function " << filename << endl;

    return false;
    */
}


bool load_objective_from_file(const string &filename, bool test_duplicates_in_linear_constriants_with_type_permutations,  int verbose_output)
{
    OPEN_FILE_SMARTLY_RETURN_FALSE_ON_FAIL(istr, filename);
    /*
    ifstream infileF;
    infileF.open (filename.c_str(), ifstream::in);
    if (!infileF.good())
    {
        cerr << "Failed opening file with objective function " << filename << endl;
        return false;
    }
    
    FilteringIstream infile(infileF);
    */

    cerr << "Loading objective function from file " << filename << endl;
        
    flag_and_coefficient fc;
    
    (*istr) >> fc.coefficient;

    // THis is useful if we want to draw the file of constraints.
    if (fc.coefficient == 0)
    {
        load_linear_constraints_from_stream((*istr), test_duplicates_in_linear_constriants_with_type_permutations, test_duplicates_in_linear_constriants_with_type_permutations, verbose_output);
    } 
    else
    { 
    while ((*istr) && fc.g.load_from_stream((*istr),-1,-1))
    {
        // check if the flag is labeled
        if (fc.g.labeled_vertices_cnt() != 0)
        {
            cerr << "WARNING: You are using labeled flags in th objective function" << endl;
        }
        // check if there are no repetitions in the objective function..
        for (int i = 0; i < (int)g_objective_combination.size(); i++)
        {
            if (g_objective_combination[i].g.is_isomorphic_to(fc.g))
            {
                cerr << "WARNING: In the objecive function, flags " << g_objective_combination[i].g.print() << " and " <<
                fc.g.print() << " are the same " << endl;
                //exit(1);
            }
        }
        g_objective_combination.push_back(fc);
        (*istr) >> fc.coefficient;
        
        if (fc.coefficient == 0)
        {
            load_linear_constraints_from_stream((*istr), test_duplicates_in_linear_constriants_with_type_permutations, test_duplicates_in_linear_constriants_with_type_permutations, verbose_output);
            break;
        }
    }
    }
        
    cerr << "# of entries in objective is " << g_objective_combination.size() << endl;
    cerr << "# of linear constraints is " << g_linear_constraints.size() << endl;
    
    return true;
}


void load_extended_linear_constraints_from_stream(istream &instream, bool test_duplicates_with_type_permutations, bool remove_implied_constraints, int verbose_output)
{

    istream *istr = &instream;

    flag_and_coefficient fc;
    string lc_constat;

    int removed_duplicate_constraints = 0;
    int processed_constraints = 0;

    while (instream)
    {
        // read left hand of the for left hand side
        double lefthand;
        try
        {
            fc_cexpression(istr, lefthand);
        }
        catch (...)
        {
            break;
        }
        if(!instream.good()) 
        {
            break;
        }

        char ch;
        ch=fc_token(istr);
        if (ch == '<' || ch == '>')
        {
            char tmp_ch;
            tmp_ch=fc_token(istr);
            if (tmp_ch != '=')
            {
                throw std::runtime_error("< and > must be followed by =, strict inequalities are not possible.");
            }
        }
        if (ch == '<' || ch == '>' || ch == '=')
        {
            linear_constraint lc;
            lc.m_constant = -lefthand;
            fc_expression(istr, lc.m_entries, NULL);

            if (ch == '<' || ch == '=')
            {
                if (!add_and_polish_constraint_if_useful(lc, test_duplicates_with_type_permutations, verbose_output))
                {
                    removed_duplicate_constraints++;
                }
            }

            if (ch == '>' || ch == '=')
            {
                lc.m_constant = -lc.m_constant;

                // Flip all signs to create <= constraint
                for (auto &fc : lc.m_entries)
                {
                    fc.coefficient = -fc.coefficient;
                }
                //
                if (!add_and_polish_constraint_if_useful(lc, test_duplicates_with_type_permutations, verbose_output))
                {
                    removed_duplicate_constraints++;
                }
            }


            processed_constraints++;
            if (verbose_output >= 2)
                cerr << "Processed " << processed_constraints << "  constraints, found " << removed_duplicate_constraints << " duplicates " <<  endl;

        }
        else
        {
            break;
        }
    }

    // Take the rest of the stream as a comment...
    std::istreambuf_iterator<char> eos;
    std::string s(std::istreambuf_iterator<char>(instream), eos);
    g_program_description = s;

    if (removed_duplicate_constraints > 0)
    {
        cerr << "Removed " << removed_duplicate_constraints << " duplicate constriants." << endl;
    }

    if (remove_implied_constraints)
    {
        remove_constraints_implied_by_others(verbose_output);
    }
}


bool load_extended_linear_constraints_from_file(const string &filename, bool test_duplicates_with_type_permutations, int verbose_output)
{
    OPEN_FILE_SMARTLY_RETURN_FALSE_ON_FAIL(istr, filename);

    load_extended_linear_constraints_from_stream((*istr), test_duplicates_with_type_permutations, test_duplicates_with_type_permutations, verbose_output);

    return true;
}


bool load_extended_objective_from_file(const string &filename, bool test_duplicates_in_linear_constriants_with_type_permutations,  int verbose_output)
{
    OPEN_FILE_SMARTLY_RETURN_FALSE_ON_FAIL(istr, filename);

    cerr << "Loading extended objective function from file " << filename << endl;

    // One expression is the objctive function
    fc_expression(istr, g_objective_combination, NULL);

    if ((*istr))
    {
        load_extended_linear_constraints_from_stream((*istr), test_duplicates_in_linear_constriants_with_type_permutations, test_duplicates_in_linear_constriants_with_type_permutations, verbose_output);
    }    
    cerr << "# of entries in objective is " << g_objective_combination.size() << endl;
    cerr << "# of linear constraints is " << g_linear_constraints.size() << endl;
    
    return true;
}




bool load_labeled_flags_from_file(int sizeKn, int verbose_output = 0, string labeled_flags_filename="")
{
    stringstream filename;
    //filename << filename_prefix() << "__n" << sizeKn << "_labeled.txt";

    if (labeled_flags_filename.length() > 0)
        filename << labeled_flags_filename;
    else
        filename << filename_prefix() << "__n" << sizeKn << "_labeled.txt";

    ifstream infileF;
    infileF.open (filename.str().c_str(), ifstream::in);
    if (!infileF.good())
    {
        cerr << "Failed opening file with labeled flags " << filename.str() << endl;
        return false;
    }
    
    FilteringIstream infile(infileF);

/*
    ifstream infile;
    infile.open (filename.str().c_str(), ifstream::in);
    if (!infile.good())
    {
        cerr << "Failed opening file with labeled flags " << filename.str() << endl;
        return false;
    }
*/

    if (verbose_output)
    {
        cerr << "Loading labeled flags from file " << filename.str() << endl;
    }

    
   flag f;
	while (f.load_from_stream(infile,-1,-1))
	{
		include_flag_in_list(f, g_flags);
	}
	
	infileF.close();
    
	return true;
}

bool dump_labeled_flags(int sizeKn, string labeled_flags_filename)
{
    stringstream filename;

    if (labeled_flags_filename.length() > 0)
        filename << labeled_flags_filename;
    else
        filename << filename_prefix() << "__n" << sizeKn << "_labeled.txt";
    
    ofstream outfile;
    outfile.open (filename.str().c_str(), ofstream::out);
    if (!outfile.good())
    {
        cerr << "Failed opening file " << filename.str() << endl;
        return false;
    }
    
    cerr << "Writing labeled flags to file " << filename.str() << endl;
    
    for (int f = 0; f < (int)g_flags.size(); f++)
    {
        for (unsigned int x = 0; x < g_flags[f].size(); x++)
        {
            outfile << g_flags[f][x].print() << endl;
        }
        if (f < (int)g_flags.size()-1) outfile << endl;
    }
    
    outfile.close();
    
    return true;
}



bool compare_flag_sizes(const flag&f1, const flag &f2)
{
    return f1.m_vertices < f2.m_vertices;
}

void sort_flags_by_size(vector<flag> &flag_list)
{
    sort(flag_list.begin(), flag_list.end(), compare_flag_sizes);
}


#ifdef G_USE_PERMITTED_SUBFLAGS
void load_permitted()
{
    stringstream filename;
    filename <<  filename_prefix() << "__permitted.txt";
    
    ifstream infile;
    infile.open (filename.str().c_str(), ifstream::in);
    if (!infile.good())
    {
        cerr << "Failed opening file " << filename.str() << " no permitted structures are used" << endl;
        return;
    }
    
    int flags_in_file = 0;
    
    flag h;
    while (h.load_from_stream(infile,-1,0))
    {
        flags_in_file++;
        // check if h is not already there
        if (!(g_already_in_known_flags(h,g_permitted_subflags[h.m_vertices])))
        {
            g_permitted_subflags[h.m_vertices].push_back(h);
            //cerr << "Loaded forbidden graph " << h.print() << endl;
        }
    }
    
    infile.close();
    
    for (int i = 0; i < V; i++)
    {
        if (g_permitted_subflags[i].size() != 0)
        {
            cerr << "Loaded " << g_permitted_subflags[i].size() << " permitted subflags of size " <<  i  << " graphs from " << filename.str() << endl;
        }
    }
}
#endif

void load_forbidden()
{
    stringstream filename;
    filename <<  filename_prefix() << "__forbidden.txt";
    
    ifstream infile;
    infile.open (filename.str().c_str(), ifstream::in);
    if (!infile.good())
    {
        cerr << "Failed opening file " << filename.str() << " no forbidden structures are used" << endl;
        return;
    }
    
    FilteringIstream filteredinfile(infile);


    int flags_in_file = 0;
    
    flag h;
    while (h.load_from_stream(filteredinfile,-1,0))
    {
        flags_in_file++;
        // check if h is not already there
        if (!(g_already_in_known_flags(h,g_forbidden_subflags)))
        {
            g_forbidden_subflags.push_back(h);
            g_forbidden_subflags_by_size[h.m_vertices].push_back(h);
            //cerr << "Loaded forbidden graph " << h.print() << endl;
        }
    }
    
    infile.close();
    
    // Idea is that it is faster to test smaller subflags than bigger
    // so lets first test smaller ones
    sort_flags_by_size(g_forbidden_subflags);
    
    cerr << "Loaded " << g_forbidden_subflags.size() << " forbidden graphs from " << filename.str() << endl;
    if (flags_in_file != (int)g_forbidden_subflags.size())
    {
        cerr << "File " << filename.str() << " contains " << flags_in_file-g_forbidden_subflags.size() << " duplicate  graphs." << endl;    
    }
    
    //for (int i = 0; i < g_forbidden_subflags.size(); i++)
    //{
    //    cerr << g_forbidden_subflags[i].print() << endl;
    //}
}

bool is_indeed_a_permutation(const vector<int> &maybe_permutation)
{
    int length = (int) maybe_permutation.size();
    int occurences_counter[length];
    for (int i = 0; i < length; i++)
    {
        occurences_counter[i] = 0;
    }

    for (int i = 0; i < length; i++)
    {
        if (maybe_permutation[i] < 0 ||  maybe_permutation[i] >= length)
        {
            cerr << "Permutation 0.."<< length-1 << " contained " << maybe_permutation[i] << endl;
            return false;
        }

        occurences_counter[maybe_permutation[i]]++;
    }

    for (int i = 0; i < length; i++)
    {
        if (occurences_counter[i] == 0)
        {
            cerr << "Permutation 0.."<< length-1 << " is missing entry  " << i << endl;
            return false;
        }
        if (occurences_counter[i] > 1)
        {
            cerr << "Permutation 0.."<< length-1 << " has entry  " << i << " several time (" << occurences_counter[i] << "x)" << endl;
            return false;
        }
    }

    return true;
}

#ifdef G_LOAD_COLORED_EDGES_BLIND_PERMUTATIONS
void load_blind_colorededge_permutations()
{
    stringstream filename;
    filename <<  filename_prefix() << "__edgeblind_permutations.txt";
    
    ifstream infile;
    infile.open (filename.str().c_str(), ifstream::in);
    if (!infile.good())
    {
        cerr << "Failed opening file " << filename.str() << " for allowed edge color permutations (dont forget identity!)" << endl;
        cerr << "Using ALL possible permutations...." << endl;
        int color_permutation[COLORS_EDGES];// = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26}; // longer than needed
        for (int i = 0; i < COLORS_EDGES; i++)
        {
            color_permutation[i] = i;
        }
        
        do {
            vector<int> color_perm;
            for (int i = 0; i < COLORS_EDGES; i++)
            {
                color_perm.push_back(color_permutation[i]);
                cerr << " " << color_permutation[i];
            }                        
            cerr << endl;

            assert(is_indeed_a_permutation(color_perm));

            g_allowed_edgecolor_permutations.push_back(color_perm);
        } while ( std::next_permutation(color_permutation+1,color_permutation+COLORS_EDGES));

        cerr << "# of edge colored permutations: " << g_allowed_edgecolor_permutations.size() << endl;
        return;
    }
    
    while (infile.good())
    {
        vector<int> color_perm;
        for (int i = 0; i < COLORS_EDGES; i++)
        {
            int color;
            infile >> color;
            if (color < 0 || color >= COLORS_EDGES)
            {
                cerr << "Permuation contains color " << color << " which is outside of range [0,"<<COLORS_EDGES<<")"<<endl;
            } 
            assert (color >= 0 && color < COLORS_EDGES);
            color_perm.push_back(color);
        }
        if (!infile.good()) break;
        assert(is_indeed_a_permutation(color_perm));
        g_allowed_edgecolor_permutations.push_back(color_perm);
    }
    
    infile.close();
    
    cerr << "# of edge colored permutations: " << g_allowed_edgecolor_permutations.size() << endl;
}
#endif

//#define G_EDGE_COLOR_SYMMETRY
#ifdef G_EDGE_COLOR_SYMMETRY
void add_edge_color_symmetry_constraints(int Kn)
{
    int old_count  = g_linear_constraints.size();
    for(int n = 2; n < Kn; n++)
    {
        cerr << "Adding symmetry constraints for n = " << n << endl;
        for (auto f : g_unlabeled_flags[n])
        {
            flag_and_coefficient fc1;
            fc1.g=f;
            fc1.coefficient=1;

            flag_and_coefficient fc2;
            fc2.coefficient=-1;

            flag f_perm;
            for (int p = 0; p < (int)g_allowed_edgecolor_permutations.size(); p++)
            {
                fc2.g = f; 
                fc2.g.permute_edge_colors(g_allowed_edgecolor_permutations[p]);
                if (fc2.g.is_isomorphic_to(f)) continue;        

                linear_constraint lc;
                lc.m_constant = 0;
                lc.add_entry(fc1);
                lc.add_entry(fc2);
                lc.check_constraint();
                g_linear_constraints.push_back(lc);
            }
        }
    }
    int new_count  = g_linear_constraints.size();

    cerr << "Added " <<  new_count-old_count << " new constraints" << endl;
}
#endif



#ifdef G_COLORED_VERTICES_BLIND
void load_blind_coloredvertex_permutations()
{
    stringstream filename;
    filename <<  filename_prefix() << "__vertexblind_permutations.txt";
    
    ifstream infile;
    infile.open (filename.str().c_str(), ifstream::in);
    if (!infile.good())
    {
        cerr << "Failed opening file " << filename.str() << " for allowed vertex color permutations (don't forget identity!)" << endl;
        cerr << "Assuming that all permutations are fine" << endl;
        int color_permutation[] = {0,1,2,3,4,5,7,8,9,10}; // longer than needed
        
        do {
            vector<int> color_perm;
            for (int i = 0; i < COLORS_VERTICES; i++)
            {
                color_perm.push_back(color_permutation[i]);
            }
            assert(is_indeed_a_permutation(color_perm));
            g_allowed_vertexcolor_permutations.push_back(color_perm);
        } while ( std::next_permutation(color_permutation+1,color_permutation+COLORS_VERTICES));
        return;
    }
    
    while (infile.good())
    {
        vector<int> color_perm;
        for (int i = 0; i < COLORS_VERTICES; i++)
        {
            int color;
            infile >> color;
            assert (color >= 0 && color < COLORS_VERTICES);
            color_perm.push_back(color);
        }
        if (!infile.good()) break;
        assert(is_indeed_a_permutation(color_perm));
        g_allowed_vertexcolor_permutations.push_back(color_perm);
    }
    
    infile.close();
    
    cerr << "# of vertex colored permutations: " << g_allowed_vertexcolor_permutations.size() << endl;
}
#endif


#ifdef G_COLORED_3EDGES_BLIND
void load_blind_colored3edge_permutations()
{
    stringstream filename;
    filename <<  filename_prefix() << "__3edgeblind_permutations.txt";
    
    ifstream infile;
    infile.open (filename.str().c_str(), ifstream::in);
    if (!infile.good())
    {
        cerr << "Failed opening file " << filename.str() << " for allowed edge color permutations (dont forget identity!)" << endl;
        exit(1);
    }
    
    while (infile.good())
    {
        vector<int> color_perm;
        for (int i = 0; i < COLORS_3EDGES; i++)
        {
            int color;
            infile >> color;
            assert (color >= 0 && color < COLORS_3EDGES);
            assert(is_indeed_a_permutation(color_perm));
            color_perm.push_back(color);
        }
        if (!infile.good()) break;
        g_allowed_3edgecolor_permutations.push_back(color_perm);
    }
    
    infile.close();
    
    cerr << "# of 3edge colored permutations: " << g_allowed_3edgecolor_permutations.size() << endl;
}
#endif


//////////////////////////////////////////////////////// 

/*
#ifdef G_COLORED_EDGES
void try_map_rest(const flag &g, const flag &f, int *mapping, int vertexID, int &all_maps, int &good_maps)
{
    if (vertexID >= f.m_vertices)
    {
        all_maps++;
        
        flag tmp;
        tmp.set_vertices_and_Theta(f.m_vertices, f.m_Theta);
        // check if theta is mapped correctly
        for (int u = 0; u < f.m_vertices; u++)
            for (int v = u+1; v < f.m_vertices; v++)
            {
                tmp.color_edge(u,v, g.m_color_edge[mapping[u]][mapping[v]]);
            }
        if (is_isomorphic_to_not_colorblind(f)) good_maps++;

        return;
    }
    
    for (int pv = 0; pv < g.m_vertices; pv++)
    {
        mapping[vertexID] = pv;
        try_map_rest(g,f,mapping,vertexID+1,all_maps,good_maps);
    }
    
}

bool try_map_root(const flag &g, const flag &f, int *mapping, int vertexID, int &all_maps, int &good_maps)
{
    if (vertexID >= f.m_Theta)
    {
        // check if theta is mapped correctly
        for (int u = 0; u < f.m_Theta; u++)
            for (int v = u+1; v < f.m_Theta; v++)
            {
                if (f.m_color_edge[u][v] != g.m_color_edge[mapping[u]][mapping[v]]) return false;
            }
                
        all_maps = 0;
        good_maps = 0;
        try_map_rest(g,f,mapping,vertexID,all_maps,good_maps);
        
        return true;
    }
    
    for (int pv = 0; pv < g.m_vertices; pv++)
    {
        mapping[vertexID] = pv;
        if (try_map_root(g,f,mapping,vertexID+1,all_maps,good_maps)) return true;
    }
    return false;
}

double compute_flag_in_structure(const flag &g, const flag &f)
{
    int mapping[V];
    
    for (int i = 0; i < f.m_vertices; i++) mapping[i] = -1;
    
    int all_maps = 0, good_maps = 0;
    
    try_map_root(g,f,mapping,0,all_maps,good_maps);
    
    if (all_maps == 0 || good_maps == 0)
    {
        cout << " 0";
        return 0;
    }
    
    
//    cout << " " << (double)(27*3*good_maps/(double)all_maps);
//    cout << " " << (double)(good_maps/(double)all_maps);
	if (good_maps != 0)
	    cout << " 1"; // << good_maps << "/" << all_maps;
	else
		cout << " 0";
    
    return (double)good_maps/(double)all_maps;
}


void test_all(const flag &g)
{
    for (int f = 0; f < (int)g_flags.size(); f++)
    {
        for (unsigned int x = 0; x < g_flags[f].size(); x++)
        {
            compute_flag_in_structure(g,g_flags[f][x]);
        }
        cout << endl;
    }
}
#endif
*/



#ifdef USE_FOR_CROSSINGS
void generate_flags_as_subflags_k6_321(int Kn)
{
    flag F;
    
	
	int mapping[5];
    
    //cerr << "g_flags.size()="<<g_flags.size()<<endl;
    
	// flag (2,1,1) theta (1,0,1)
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
		mapping[0] = 5;
		
		//do {
            
            for (int a = 0; a < 3; a++)
                for (int b = 0; b < 3; b++)
                    for (int c = 3; c < 5; c++)
                {
                    if (a == b) continue;
                    
                    mapping[1] = a;
                    mapping[2] = b;
                    mapping[3] = c;
                    
                    F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 4, 2); // taking a subflag
                    
                    include_flag_in_list_if_new(F,g_flags);
                }
			
		//} while ( std::next_permutation(mapping,mapping+3) );
	}

    //cerr << "g_flags.size()="<<g_flags.size()<<endl;


	// flag (3,1,1) theta (3,0,1)
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
		mapping[0] = 0;
		mapping[1] = 1;
		mapping[2] = 2;
		mapping[3] = 5;
		
		do {
			
			for (int a = 3; a < 5; a++)
				{
					mapping[4] = a;
					
					F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 5, 4); // taking a subflag
					
					include_flag_in_list_if_new(F,g_flags);
				}
			
		} while ( std::next_permutation(mapping,mapping+3) );
    }
    
    //cerr << "g_flags.size()="<<g_flags.size()<<endl;


	// flag (1,2,1) theta (0,2,1)
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
		mapping[0] = 5;
		
		//do {
        for (int a = 0; a < 3; a++)
            for (int b = 3; b < 5; b++)
                for (int c = 3; c < 5; c++)
                {
                    if (b == c) continue;
                    
                    mapping[1] = b;
                    mapping[2] = c;
                    mapping[3] = a;
                    
                    F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 4, 2); // taking a subflag
                    
                    include_flag_in_list_if_new(F,g_flags);
                }
		//} while ( std::next_permutation(mapping,mapping+3) );
	}

    //cerr << "g_flags.size()="<<g_flags.size()<<endl;
    
	// flag (2,2,1) theta (1,2,1)
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
		mapping[0] = 5;
		
		//do {
        for (int a = 0; a < 3; a++)
            for (int b = 3; b < 5; b++)
                for (int c = 3; c < 5; c++)
                    for (int d = 0; d < 3; d++)
                {
                    if (b == c) continue;
                    if (a == d) continue;
                    
                    mapping[1] = a;
                    mapping[2] = b;
                    mapping[3] = c;
                    mapping[4] = d;
                    
                    F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 5, 4); // taking a subflag
                    
                    include_flag_in_list_if_new(F,g_flags);
                }
		//} while ( std::next_permutation(mapping,mapping+3) );
	}
    
    //cerr << "g_flags.size()="<<g_flags.size()<<endl;

    
	// flag (2,1,1) theta (1,0,1)
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
		mapping[0] = 5;
		
		//do {
        
        for (int a = 0; a < 3; a++)
            for (int b = 3; b < 5; b++)
                {
                    mapping[1] = a;
                    mapping[2] = b;
                    
                    F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 3, 1); // taking a subflag
                    
                    include_flag_in_list_if_new(F,g_flags);
                }
        
		//} while ( std::next_permutation(mapping,mapping+3) );
	}

    //cerr << "g_flags.size()="<<g_flags.size()<<endl;


}


void generate_flags_as_subflags_k6_222(int Kn)
{
    flag F;
    
	
	int mapping[5];
    
    
	// flag (2,1,1) theta (2,0,0)
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
        for (int a = 0; a < 2; a++)
            for (int b = 0; b < 2; b++)
                for (int c = 3; c < 5; c++)
                    for (int d = 5; d < 7; d++)
                {
                    if (a == b) continue;
                    
                    mapping[0] = a;
                    mapping[1] = b;
                    mapping[2] = c;
                    mapping[3] = d;
                    
                    F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 4, 2); // taking a subflag
                    
                    include_flag_in_list_if_new(F,g_flags);
                }
	}

 	// flag (1,2,1) theta (0,2,0)
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
        for (int a = 3; a < 5; a++)
            for (int b = 3; b < 5; b++)
                for (int c = 0; c < 3; c++)
                    for (int d = 5; d < 7; d++)
                    {
                        if (a == b) continue;
                        
                        mapping[0] = a;
                        mapping[1] = b;
                        mapping[2] = c;
                        mapping[3] = d;
                        
                        F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 4, 2); // taking a subflag
                        
                        include_flag_in_list_if_new(F,g_flags);
                    }
	}

	// flag (1,1,2) theta (0,0,2)
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
        for (int a = 5; a < 7; a++)
            for (int b = 5; b < 7; b++)
                for (int c = 3; c < 5; c++)
                    for (int d = 0; d < 3; d++)
                    {
                        if (a == b) continue;
                        
                        mapping[0] = a;
                        mapping[1] = b;
                        mapping[2] = c;
                        mapping[3] = d;
                        
                        F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 4, 2); // taking a subflag
                        
                        include_flag_in_list_if_new(F,g_flags);
                    }
	}
}


void generate_flags_as_subflags_k6_33(int Kn)
{
    flag F;
    
	
	int mapping[5];


	// fixed 3 on one side + 1 on the on the other
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
		mapping[0] = 0;
		mapping[1] = 1;
		mapping[2] = 2;
		
		do {
		
		for (int a = 3; a < 6; a++)
			for (int b = 3; b < 6; b++)
			{
				if (a == b) continue;
				
				mapping[3] = a;
				mapping[4] = b;
				
				F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 5, 4); // taking a subflag
				
				include_flag_in_list_if_new(F,g_flags);
			}
			
		} while ( std::next_permutation(mapping,mapping+3) );
	}

	// fixed 3 on other side + 1 on the other
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
		mapping[0] = 3;
		mapping[1] = 4;
		mapping[2] = 5;
		
		do {
			
			for (int a = 0; a < 3; a++)
				for (int b = 0; b < 3; b++)
				{
					if (a == b) continue;
					
					mapping[3] = a;
					mapping[4] = b;
					
					F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 5, 4); // taking a subflag
					
					include_flag_in_list_if_new(F,g_flags);
				}
			
		} while ( std::next_permutation(mapping,mapping+3) );
   }

	
    F.set_vertices_and_Theta(4,2);
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
		for (int a = 0; a < 3; a++)
			for (int b = 3; b < 6; b++)
				for (int c = 0; c < 3; c++)
					for (int d = 3; d < 6; d++)
				{
					if (a == c || b == d) continue;
					
					mapping[0] = a;
					mapping[1] = b;
					mapping[2] = c;
					mapping[3] = d;
					
					F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 4, 2); // taking a subflag
					
					include_flag_in_list_if_new(F,g_flags);
				}
	}
}


void generate_flags_as_subflags_k7_3_4(int Kn)
{
    flag F;
	int mapping[6];


	// side 3 labeled, from side 4 2 labeled, 1 unalbeled
    F.set_vertices_and_Theta(6,5);
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
		mapping[0] = 0;
		mapping[1] = 1;
		mapping[2] = 2;
		
		do {
						
			for (int a = 3; a < 7; a++)
				for (int b = 3; b < 7; b++)
					for (int c = 3; c < 7; c++)
				{
					if (a == b || b == c || a == c) continue;
					
					mapping[3] = a;
					mapping[4] = b;
					mapping[5] = c;
					
					F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 6, 5); // taking a subflag
					
					include_flag_in_list_if_new(F,g_flags);
				}
			
		} while ( std::next_permutation(mapping,mapping+3) );
	}

	// side 4 labeled, from side 3 1 labeled, 1 unalbeled
    F.set_vertices_and_Theta(6,5);
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
		mapping[0] = 3;
		mapping[1] = 4;
		mapping[2] = 5;
		mapping[3] = 6;
		
		do {
			
			for (int a = 0; a < 3; a++)
				for (int b = 0; b < 3; b++)
				{
					if (a == b) continue;
					
					mapping[4] = a;
					mapping[5] = b;
					
					F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 6, 5); // taking a subflag
					
					include_flag_in_list_if_new(F,g_flags);
				}
			
		} while ( std::next_permutation(mapping,mapping+4) );
	}

	// side 3: 1 labeled, 1 unlabled;  side 4: 2 labeled, 1 unlabeled
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
		for (int a = 0; a < 3; a++)
			for (int b = 3; b < 7; b++)
				for (int c = 0; c < 3; c++)
					for (int d = 3; d < 7; d++)
						for (int e = 3; e < 7; e++)
					{
						if (a == c || b == d || b == e || d == e) continue;
						
						mapping[0] = a;
						mapping[1] = b;
						mapping[2] = d;
						mapping[3] = c;
						mapping[4] = e;
						
						F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 5, 3); // taking a subflag
						
						include_flag_in_list_if_new(F,g_flags);
					}
	}
	
	// side 3: 1 labeled, 1 unlabled;  side 4: 2 unlabeled
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
		for (int a = 0; a < 3; a++)
			for (int b = 3; b < 7; b++)
				for (int c = 0; c < 3; c++)
					for (int d = 3; d < 7; d++)
					{
						if (a == c || b == d) continue;
						
						mapping[0] = a;
						mapping[1] = b;
						mapping[2] = c;
						mapping[3] = d;
						
						F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 4, 1); // taking a subflag
						
						include_flag_in_list_if_new(F,g_flags);
					}
	}
	
	// side 3 labeled, from side 2 unalbeled
    F.set_vertices_and_Theta(6,5);
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
		mapping[0] = 0;
		mapping[1] = 1;
		mapping[2] = 2;
		
		do {
						
			for (int a = 3; a < 7; a++)
				for (int b = 3; b < 7; b++)
				{
					if (a == b) continue;
					
					mapping[3] = a;
					mapping[4] = b;
					
					F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 5, 3); // taking a subflag
					
					include_flag_in_list_if_new(F,g_flags);
				}
			
		} while ( std::next_permutation(mapping,mapping+3) );
	}	
}



void generate_flags_as_subflags_k7_3_2_2(int Kn)
{
    flag F;
	int mapping[6];
    
    
	// (3, 1, 1) theta (3,0,0)
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
		mapping[0] = 0;
		mapping[1] = 1;
		mapping[2] = 2;
		
		do {
            
			for (int a = 3; a < 5; a++)
				for (int b = 5; b < 7; b++)
                    {
                        mapping[3] = a;
                        mapping[4] = b;
                        
                        F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 5, 3); // taking a subflag
                        
                        include_flag_in_list_if_new(F,g_flags);
                    }
			
		} while ( std::next_permutation(mapping,mapping+3) );
	}
    
	// (3, 2, 1) theta (3,2,0)
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
		mapping[0] = 0;
		mapping[1] = 1;
		mapping[2] = 2;
		
		do {
            
			for (int a = 3; a < 5; a++)
				for (int b = a+1; b < 5; b++)
                    for (int c = 5; c < 7; c++)
                {
                    mapping[3] = a;
                    mapping[4] = b;
                    mapping[5] = c;
                    
                    F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 6, 5); // taking a subflag
                    
                    include_flag_in_list_if_new(F,g_flags);
                }
			
		} while ( std::next_permutation(mapping,mapping+3) );
	}
    
	// (3, 1, 2) theta (3,0,2)
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
		mapping[0] = 0;
		mapping[1] = 1;
		mapping[2] = 2;
		
		do {
            
			for (int a = 3; a < 5; a++)
				for (int b = 5; b < 7; b++)
                    for (int c = b+1; c < 7; c++)
                    {
                        mapping[3] = c;
                        mapping[4] = b;
                        mapping[5] = a;
                        
                        F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 6, 5); // taking a subflag
                        
                        include_flag_in_list_if_new(F,g_flags);
                    }
			
		} while ( std::next_permutation(mapping,mapping+3) );
	}
    
    
	// (2,1,1) theta (1,0,0)
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
//		do {
			for (int x = 0; x < 3; x++)
				for (int y = 0; y < 3; y++)
            
			for (int a = 3; a < 5; a++)
				for (int b = 5; b < 7; b++)
                {
                    if (x == y) continue;
                    mapping[0] = x;
                    mapping[1] = y;
                    mapping[2] = a;
                    mapping[3] = b;
                    
                    F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 4, 1); // taking a subflag
                    
                    include_flag_in_list_if_new(F,g_flags);
                }
			
//		} while ( std::next_permutation(mapping,mapping+3) );
	}
    
	// (2,2,1) theta (1,2,0)
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
        //		do {
        for (int x = 0; x < 3; x++)
            for (int y = 0; y < 3; y++)
                for (int z = 3; z < 5; z++)
                for (int a = z+1; a < 5; a++)
                    for (int b = 5; b < 7; b++)
                    {
                        if (x == y) continue;
                        mapping[0] = x;
                        mapping[1] = z;
                        mapping[2] = a;
                        mapping[3] = b;
                        mapping[4] = y;
                        
                        F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 5, 3); // taking a subflag
                        
                        include_flag_in_list_if_new(F,g_flags);
                    }
        
        //		} while ( std::next_permutation(mapping,mapping+3) );
	}
    
    // (2,1,2) theta (1,0,2)
    for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
    {
        //		do {
        for (int x = 0; x < 3; x++)
            for (int y = 0; y < 3; y++)
                for (int z = 3; z < 5; z++)
                    for (int a = 5; a < 7; a++)
                        for (int b = a+1; b < 7; b++)
                        {
                            if (x == y) continue;
                            mapping[0] = x;
                            mapping[1] = a;
                            mapping[2] = b;
                            mapping[3] = y;
                            mapping[4] = z;
                            
                            F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 5, 3); // taking a subflag
                            
                            include_flag_in_list_if_new(F,g_flags);
                        }
        
        //		} while ( std::next_permutation(mapping,mapping+3) );
    }

    
    // (2,2,2) theta (1,2,2)
    for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
    {
        //		do {
        for (int x = 0; x < 3; x++)
            for (int y = 0; y < 3; y++)
                        {
                            if (x == y) continue;
                            mapping[0] = x;
                            mapping[1] = 3;
                            mapping[2] = 4;
                            mapping[3] = 5;
                            mapping[4] = 6;
                            mapping[5] = y;

                            F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 6, 5); // taking a subflag
                            
                            include_flag_in_list_if_new(F,g_flags);
                        }
        
        //		} while ( std::next_permutation(mapping,mapping+3) );
    }
}


void generate_flags_as_subflags_k8_3_5(int Kn)
{
    flag F;
	int mapping[8];

	
	for (int i = 0; i < (int)g_unlabeled_flags[Kn].size();i++)
	{
		cerr << "Flags from " << i << endl;
		
		// Flag 3;3+1
		mapping[0] = 0;
		mapping[1] = 1;
		mapping[2] = 2;		
		do {			
			for (int a = 3; a < 8; a++)
			{
				for (int b = 3; b < 8; b++)
				{
					if (b == a) continue;
					for (int c = 3; c < 8; c++)
					{
						if (b == c || a == c) continue;
						for (int d = 3; d < 8; d++)
					{
						if (d == a || d == b || d == c) continue;
						
						mapping[3] = a;
						mapping[4] = b;
						mapping[5] = c;
						mapping[6] = d;
						
						F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 7, 6); // taking a subflag
						
						include_flag_in_list_if_new(F,g_flags);
					}
					}
				}
			}
		} while ( std::next_permutation(mapping,mapping+3) );	

		// Flag 3;1+2
		mapping[0] = 0;
		mapping[1] = 1;
		mapping[2] = 2;		
		do {			
			for (int a = 3; a < 8; a++)
			{
				for (int b = 3; b < 8; b++)
				{
					if (b == a) continue;
					for (int c = 3; c < 8; c++)
					{
						if (b == c || a == c) continue;						
						mapping[3] = a;
						mapping[4] = b;
						mapping[5] = c;						
						F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 6, 4); // taking a subflag						
						include_flag_in_list_if_new(F,g_flags);
					}
				}
			}
		} while ( std::next_permutation(mapping,mapping+3) );	

		// Flag 1+1 ; 1+2
		for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
		{
			if (x == y) continue;
			for (int a = 3; a < 8; a++)
			{
				for (int b = 3; b < 8; b++)
				{
					if (b == a) continue;
					for (int c = 3; c < 8; c++)
					{
						if (b == c || a == c) continue;						
						mapping[0] = x;
						mapping[1] = a;
						mapping[2] = y;
						mapping[4] = b;
						mapping[5] = c;						
						F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 5, 2); // taking a subflag						
						include_flag_in_list_if_new(F,g_flags);
					}
				}
			}
		} 

		// Flag 1+1;3+1
		for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
		{
			if (x == y) continue;
			for (int a = 3; a < 8; a++)
			{
				for (int b = 3; b < 8; b++)
				{
					if (b == a) continue;
					for (int c = 3; c < 8; c++)
					{
						if (b == c || a == c) continue;						
						for (int d = 3; d < 8; d++)
						{
				 			if (d == a || d == b || d == c) continue;
				 			mapping[0] = x;
							mapping[1] = a;
							mapping[2] = b;
							mapping[4] = c;
							mapping[5] = y;						
							mapping[6] = d;						
							F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 6, 4); // taking a subflag						
							include_flag_in_list_if_new(F,g_flags);
						}
					}
				}
			}
		} 
				
		// Flag 1+1;5
		mapping[0] = 3;
		mapping[1] = 3;
		mapping[2] = 5;		
		mapping[3] = 6;		
		mapping[4] = 7;		
		do {			
			for (int a = 0; a < 3; a++)
			{
				for (int b = 0; b < 3; b++)
				{
					if (b == a) continue;
						
						mapping[5] = a;
						mapping[6] = b;
						
						F.as_subflag(g_unlabeled_flags[Kn][i], mapping, 7, 6); // taking a subflag
						
						include_flag_in_list_if_new(F,g_flags);
				}
			}
		} while ( std::next_permutation(mapping+3,mapping+8) );			
		
		}
	
}
#endif

void add_linear_constraints_flags_equal(const flag &f1, const flag &f2)
{
    linear_constraint lc;
    lc.m_constant = 0;
    
    flag_and_coefficient gd;
    gd.coefficient = 1;
    gd.g = f1;
    lc.m_entries.push_back(gd);
    
    gd.coefficient = -1;
    gd.g = f2;
    lc.m_entries.push_back(gd);
    
    lc.check_constraint();
    assert(lc.m_checked);
    g_linear_constraints.push_back(lc);

    // swap the coefficients and also add as a linear constraint
    lc.m_entries[0].coefficient = -1;
    lc.m_entries[1].coefficient = 1;
    lc.check_constraint();
    assert(lc.m_checked);
    g_linear_constraints.push_back(lc);
}


#ifdef USE_FOR_CROSSINGS
int main_crossings(int Kn)
{
    /*
    flag g;
    g.set_vertices(2);
    cout << g.print() << endl;
    
    return 0;
    */
	if (!load_unlabeled_flags_from_file(Kn))
    {
        cerr << "Cannot load unlabeled flags. This program does not generate them." << endl;
        return -1;
    }
	
	cerr << "Loaded "<< endl;

 //   return 0;
/*
    // add objective function
	flag_and_coefficient gd;
	gd.coefficient = 1;
    // Corresponds to one crossing  in bipartite graph
	//gd.g.load_from_string("4  0    0 0 1 1    0 0 1  0 0 1 1 0 0");
    //g_objective_combination.push_back(gd);

    gd.g.load_from_string("4  0    0 0 1 1    0 0 0  0 0 1 0 1 1");
    g_objective_combination.push_back(gd);
    gd.g.load_from_string("4  0    0 0 1 1    0 0 0  1 0 0 1 1 0");
    g_objective_combination.push_back(gd);
*/
    
/*
	linear_constraint lc;
	flag_and_coefficient cut_A, cut_B;
	cut_A.coefficient = 1;
	cut_A.g.load_from_string("4  0    0 0 1 1    0 0 0  0 0 1 0 1 1");
	cut_B.coefficient = -1;
	cut_B.g.load_from_string("4  0    0 0 1 1    0 0 0  1 0 0 1 1 0");
	lc.m_entries.push_back(cut_A);
	lc.m_entries.push_back(cut_B);
	lc.m_constant = 0;
    lc.check_constraint();
	g_linear_constraints.push_back(lc);
*/
 
/*
	linear_constraint lc_cross;
	flag_and_coefficient cross;
	cross.coefficient = -1;
	cross.g.load_from_string("4  0    0 0 1 1    0 0 1  0 0 1 1 0 0");
    lc_cross.m_entries.push_back(cross);
	lc_cross.m_constant = 0.3; // 4.5/15 on 6 vertices
    lc.check_constraint();
	g_linear_constraints.push_back(lc_cross);
*/

/*
    //add constraints for 2 vertices
    flag f12, f13, f23;
    f12.load_from_string("4  0    0 0 1 1    0 0 1");
    f13.load_from_string("4  0    0 0 2 2    0 0 1");
    f23.load_from_string("4  0    1 1 2 2    0 0 1");
    add_linear_constraints_flags_equal(f12, f13);
    add_linear_constraints_flags_equal(f12, f23);
    add_linear_constraints_flags_equal(f13, f23);
    f12.load_from_string("4  0    0 0 1 1    0 0 0");
    f13.load_from_string("4  0    0 0 2 2    0 0 0");
    f23.load_from_string("4  0    1 1 2 2    0 0 0");
    add_linear_constraints_flags_equal(f12, f13);
    add_linear_constraints_flags_equal(f12, f23);
    add_linear_constraints_flags_equal(f13, f23);
  */
    
/*
     // add constraints for 4 vertices
    if (Kn != 4)
    {
        load_unlabeled_flags_from_file(4);    // loads 4
    }
     for (int i = 0; i < (int)g_unlabeled_flags[4].size(); i++)
     {
     flag gswap;
     gswap = g_unlabeled_flags[4][i];
     gswap.swap_colors();
     
     linear_constraint lc;
     
     flag_and_coefficient gd;
     gd.coefficient = 1;
     gd.g = g_unlabeled_flags[4][i];
     lc.m_entries.push_back(gd);
     
     gd.coefficient = -1;
     gd.g = gswap;
     lc.m_entries.push_back(gd);
     
     lc.m_constant = 0;
     lc.check_constraint();
     g_linear_constraints.push_back(lc);
     }
 */
    
/*
	// add constraints for 6 vertices
    if (Kn != 6)
    {
        load_unlabeled_flags_from_file(6);    // loads 6
    }
	for (int i = 0; i < (int)g_unlabeled_flags[6].size(); i++)
	{
		flag gswap;
		gswap = g_unlabeled_flags[6][i];
		gswap.swap_colors();
		
		linear_constraint lc;
		
		flag_and_coefficient gd;
		gd.coefficient = 1;
		gd.g = g_unlabeled_flags[6][i];
		lc.m_entries.push_back(gd);
		
		gd.coefficient = -1;
		gd.g = gswap;
		lc.m_entries.push_back(gd);
		
		lc.m_constant = 0;
        lc.check_constraint();
		g_linear_constraints.push_back(lc);
	}
*/
/*    
	cerr << "Added " << g_linear_constraints.size() << " linear constraints" << endl;
	if (!load_labeled_flags_from_file())
	{
		if (Kn == 6)
		{
			generate_flags_as_subflags_k6_321();
//			generate_flags_as_subflags_k6_33();
//			generate_flags_as_subflags_k6_222();
		}
		if (Kn == 7)
		{
			generate_flags_as_subflags_k7_3_2_2();
		}
		if (Kn == 8)
		{
			generate_flags_as_subflags_k8_3_5();
		}
		
	}
	
	cerr << "got "<< (int)g_flags.size() << " types" << endl;
	
	
    
#ifdef dump_labeled_flags
    dump_labeled_flags();
    return 0;
#endif
    
	print_CSDP();
	*/
	return 0;
}
#endif





void extensions_of_g_with_print(flag &g, bool extension_count_copies)
{
    vector<flag> flag_list;
    
    extensions_of_g(g, flag_list);
    
    for (unsigned int x = 0; x < flag_list.size(); x++)
    {
        if (extension_count_copies)
        {
            cout << flag_list[x].count_labeled_copies_of(g) << "    ";
        }
        cout << flag_list[x].print() << endl;
    }
}


void extensions_of_fc(const string &filename)
{
    cerr << "Extensions of " << filename << endl;
    vector<flag_and_coefficient> F1;
    load_flags_and_coefficients_from_file(filename, F1);

    vector<flag_and_coefficient> FE; 

    for (int i = 0; i < (int)F1.size(); i++)
    {
        vector<flag> flag_list;
        extensions_of_g(F1[i].g, flag_list);
    
        for (unsigned int x = 0; x < flag_list.size(); x++)
        {
            flag_and_coefficient fc;
            fc.g = flag_list[x];
            fc.coefficient = F1[i].coefficient;

            dump_flag_and_coefficient(fc);
            //FE.push_back(fc);
        }
    }
        
    //dump_flags_and_coefficients(FE);
}

void generate_products_of_constraints(int Kn, string objective_file_name, bool force_generating_constriants, int verbose_output)
{
    if (g_linear_constraints.size() == 0) return;
    
    
    stringstream filename;
    filename <<  filename_prefix() << "__n" << Kn << "_generated_constraints_" << objective_file_name << ".txt";
    
    if (!force_generating_constriants)
    {
        if (load_linear_constraints_from_file(filename.str(), false, verbose_output)) return;
    }
    
    
    cerr << "Generating additoinal linear constraints...." << endl;
    // 
    int original_size = (int)g_linear_constraints.size();
    for (int i = 0; i < original_size; i++)
    {
        //if (!g_linear_constraints[i].m_same_types) continue;
        for (int j = i; j < (int)g_linear_constraints.size(); j++)
        {
            
            //if (!g_linear_constraints[j].m_same_types) continue;

            linear_constraint &lci = g_linear_constraints[i];
            linear_constraint &lcj = g_linear_constraints[j];

            
            if (!lci.m_type.have_same_type(lcj.m_type)) continue;
            
            if (lci.m_entries_max_size + lcj.m_entries_max_size - lci.m_labeled_vertices_in_type_cnt > Kn) continue;
            
            cerr << "Adding product of constraints " << i+1 << " and " << j+1 << endl;
            linear_constraint lc;
            
            // Does product of (F1+c1)*(F1+c2)
            
            // c1*c2
            lc.m_constant = lci.m_constant * lcj.m_constant;
            
            flag_and_coefficient fc;
            // F1*c2
            for (int x = 0; x < (int)lci.m_entries.size(); x++)
            {
                fc = lci.m_entries[x];
                fc.coefficient *= lcj.m_constant;
                lc.add_entry(fc);
            }
            // F2*c1
            for (int y = 0; y < (int)lcj.m_entries.size(); y++)
            {
                fc = lcj.m_entries[y];
                fc.coefficient *= lci.m_constant;
                lc.add_entry(fc);
            }
            // F1*F2
            for (int x = 0; x < (int)lci.m_entries.size(); x++)
            {
                for (int y = 0; y < (int)lcj.m_entries.size(); y++)
                {
                    vector<flag_and_coefficient> F1F2 = F1_times_F2(lci.m_entries[x].g, lcj.m_entries[y].g, lci.m_type);
                    
                    double scale = lci.m_entries[x].coefficient * lcj.m_entries[y].coefficient;
                    
                    // polish the list of products and add them to lc
                    for (int z = 0; z < (int)F1F2.size(); z++)
                    {
                        F1F2[z].coefficient *= scale;
                        lc.add_entry(F1F2[z]);
                    }                    
                }              
            }

            lc.check_constraint();
            assert(lc.m_checked);
            g_linear_constraints.push_back(lc);
            if (verbose_output)
                cerr << "Adding new linear constraint " << endl << lc.print();
        }
    }
    
    cerr << "Generated " << g_linear_constraints.size()-original_size << " new linear constraints" << endl;
    cerr << "Writing new linear constraints to " << filename.str() << endl;
    
    ofstream outfile;
    outfile.open (filename.str().c_str(), ofstream::out);
    if (!outfile.good())
    {
        cerr << "Failed opening file " << filename.str() << endl;
        return;
    }
        
    for (int c = original_size; c < (int)g_linear_constraints.size(); c++)
    {
        outfile << g_linear_constraints[c].print() << endl;        
    }
    
    outfile.close();
}


void scale_linear_constraints_to_integers(int Kn)
{
    if (g_linear_constraints.size() == 0) return;
    
    cerr << "Scaling linear constraints...." << endl;
    for (int j = 0; j < (int)g_linear_constraints.size(); j++)
    {
        // Computing the right scale
        double scale=1;
        bool used[V];
        for (int i = 0; i < V; i++) used[i] = false;
        for (int k = 0; k < (int)g_linear_constraints[j].m_entries.size(); k++)
        {
            int v = g_linear_constraints[j].m_entries[k].g.m_vertices;
            if (v < Kn && used[v] == false)
            {
                used[v] = true;
                scale *= binomial(Kn,v);
            }
        }
        
        if (scale != 1)
        {
            cerr << "Scaling linear constraint " << j+1 << " by " << scale << endl;
            // Applying the right scale
            g_linear_constraints[j].m_constant *= scale;
            for (int k = 0; k < (int)g_linear_constraints[j].m_entries.size(); k++)
            {
                g_linear_constraints[j].m_entries[k].coefficient *= scale;
            }
        }
        else
        {
            cerr << "Constraint " << j+1 << " not scaled" << endl;            
        }
    }   
}


// Generates Baber's equalities
void generate_baber_equalities(int Kn, int verbose_output)
{
#ifdef G_USE_BABER_EQUATIONS
#ifdef G_COLORED_VERTICES

#ifdef G_COLORED_3EDGES
#if (G_COLORED_VERTICES != 4) && (G_COLORED_VERTICES != 9)
    cerr << "Baber inequalities need either 4 colors on vertices - if you wnat them for 2-colored 3-edges or 9 colors in case of 3-colored."
    "Current number is  " << G_COLORED_VERTICES << endl;
    return;
#endif
#endif    

#ifdef G_3EDGES
#if (G_COLORED_VERTICES != 4)
    cerr << "Baber inequalities need 4 colors on vertices instead of 2." << endl;
    return;
#endif
#endif

#else
    cerr << "Baber's inequalities need coloring on vertices" << endl;
    return;
#endif

    
    // Loading graphs of size Kn with HALF the number of colors. 
    if (!load_unlabeled_flags_from_file(Kn))
    {
        cerr << "Please first generate the graphs of size " << Kn << " for the baber version of the problem" << endl;
        cerr << "And rename the file to double the number of colors " << endl;
        return;
    }
    
    // Generating graphs of the correct type of size 2 smaller
    for (int i = 1; i <= Kn-2; i++)
    {
        generate_unlabeled_flags_of_size(i, verbose_output, false);
    }
    
    // Generate equalities - the real Baber's Hack
#pragma omp parallel for ordered schedule(dynamic)
    for (int i = 0; i < (int)g_unlabeled_flags[Kn-2].size(); i++)
    {
        stringstream ss;
        
        bool first_nonzero = true;
        
        flag f = g_unlabeled_flags[Kn-2][i];
        cerr << "Generating linear combinations for " << f.print() << endl;
        
        for (int j = 0; j < (int)g_unlabeled_flags[Kn].size(); j++)
        {
            flag h = g_unlabeled_flags[Kn][j];
            
            int count_ab = 0;
            int count_cd = 0;
            
            for (int u = 0; u < Kn-1; u++)
                for (int v = u+1; v < Kn; v++)
                {
                    flag f_ab = h;
                    flag f_cd = h;
                    for (int x = 0; x < Kn; x++)
                    {
                        if (x == u || x == v) continue;
                        
#ifdef G_3EDGES
                        if (h.m_3edge[u][v][x])
                        {
                            f_ab.m_color_vertex[x] = h.m_color_vertex[x] + 2;      // 1-2 -> 3-4
                            f_cd.m_color_vertex[x] = 2*(h.m_color_vertex[x]-1)+2;  // 1-2 -> 2-4
                        }
                        else
                        {
                            f_ab.m_color_vertex[x] = h.m_color_vertex[x];          // 1-2  -> 1-2
                            f_cd.m_color_vertex[x] = 2*(h.m_color_vertex[x]-1)+1;  // 1-2  -> 1-3                          
                        }
#endif
                        // The following could be done more efficiently for all colors at the same time....
#ifdef G_COLORED_3EDGES
#if  (G_COLORED_VERTICES == 4) // 2*2
                        if (h.m_colored_3edge[u][v][x] == 2)
                        {
                            f_ab.m_color_vertex[x] = h.m_color_vertex[x] + 2;      // 1-2 -> 3-4
                            f_cd.m_color_vertex[x] = 2*(h.m_color_vertex[x]-1)+2;  // 1-2 -> 2-4
                        }
                        else if (h.m_colored_3edge[u][v][x] == 1)
                        {
                            f_ab.m_color_vertex[x] = h.m_color_vertex[x];          // 1-2  -> 1-2
                            f_cd.m_color_vertex[x] = 2*(h.m_color_vertex[x]-1)+1;  // 1-2  -> 1-3                          
                        }
                        else
                        {
                            assert(0);
                        }
#endif
#if  (G_COLORED_VERTICES == 9) // 3*3
                        if (h.m_colored_3edge[u][v][x] == 3)
                        {
                            f_ab.m_color_vertex[x] = h.m_color_vertex[x] + 2*3;      // 1,2,3  -> 7,8,9
                            f_cd.m_color_vertex[x] = 3*(h.m_color_vertex[x]-1)+3;    // 1,2,3 ->  3,7,9
                        }
                        else if (h.m_colored_3edge[u][v][x] == 2)
                        {
                            f_ab.m_color_vertex[x] = h.m_color_vertex[x] + 1*3;    // 1,2,3  -> 4,5,6
                            f_cd.m_color_vertex[x] = 3*(h.m_color_vertex[x]-1)+2;  // 1,2,3  -> 2,5,7                          
                        }
                        else if (h.m_colored_3edge[u][v][x] == 1)
                        {
                            f_ab.m_color_vertex[x] = h.m_color_vertex[x];          // 1,2,3  -> 1,2,3
                            f_cd.m_color_vertex[x] = 3*(h.m_color_vertex[x]-1)+1;  // 1,2,3  -> 1,4,7                          
                        }
                        else
                        {
                            assert(0);
                        }                        
#endif 
#endif
                    }
                    
                    //cerr << "removing vertices " << u << " " << v << endl;
                    
                    f_ab.remove_vertices(u,v);
                    if (f_ab.is_isomorphic_to(f)) count_ab++; 
                    
                    f_cd.remove_vertices(u,v);
                    if (f_cd.is_isomorphic_to(f)) count_cd++; 
                    
                    
                    //cerr << "done removing" << endl;
                }

            /*
            if (true)
            {
                if (first_nonzero)
                {
                    first_nonzero = false;
                    ss << endl << "0 0 " << endl;
                }
                ss << count_ab << " " << h.print() << endl;
                ss << -count_cd << " " << h.print() << endl;
            }
            */

            
            if (count_ab != count_cd)
            {
                if (first_nonzero)
                {
                    first_nonzero = false;
                    ss << endl << "0 0 " << endl;
                }
                ss << count_ab-count_cd << " " << h.print() << endl;
            }
            
            
        }
        
        
#pragma omp ordered
        {
            cout << ss.str();
        }
    }
#else
    cerr << "Baber's inequalities were not enabled during compile time" << endl;
#endif
}


void print_latex_header(ostream &outfile, bool color_1_nonedge = false)
{
   
    outfile << ""
    "\\documentclass{article} \n"
    "\\usepackage{tikz}\n"
    "\\usepackage{fullpage} \% Disable if equations are overflowing \n"
    "\\usepackage{multicol}\n"
    "\\usepackage{stmaryrd}\n"
    "\\usepackage{amsmath}\n"
    "\n \% tikz style \n\n"
    "\\newcommand{\\vc}[1]{\\ensuremath{\\vcenter{\\hbox{#1}}}}\n"
    "\\tikzset{flag_pic/.style={scale=1}}  \%  Change the scale to change all figures \n"
    "\\tikzset{unlabeled_vertex/.style={inner sep=1.7pt, outer sep=0pt, circle, fill}} \n"
    "\\tikzset{labeled_vertex/.style={inner sep=2.2pt, outer sep=0pt, rectangle, fill=yellow, draw=black}} \n"
    "\\tikzset{edge_color0/.style={color=black,line width=1.2pt,opacity=0.5}} \n"
#ifdef G_ORIENTED_EDGES
    "\\tikzset{edge_color1/.style={color=red,  line width=1.2pt,opacity=0}} \n"
#else
    ;
    if (color_1_nonedge)
    {
        outfile << "\\tikzset{edge_color1/.style={color=red,  line width=1.2pt,opacity=0}} \n";
    }
    else
    {
        outfile << "\\tikzset{edge_color1/.style={color=red,  line width=1.2pt,opacity=1}} \n";
    }
    outfile <<
#endif
    "\\tikzset{edge_color2/.style={color=blue, line width=1.2pt,opacity=1}} \n"
    "\\tikzset{edge_color3/.style={color=green!80!black,line width=1.2pt}} \n"
    "\\tikzset{edge_color4/.style={color=red,  line width=1.2pt,dotted}} \n"
    "\\tikzset{edge_color5/.style={color=blue, line width=1.2pt,dotted}} \n"
    "\\tikzset{edge_color6/.style={color=green, line width=1.2pt,dotted}} \n"
    "\\tikzset{edge_color7/.style={color=orange, line width=1.2pt}} \n"
    "\\tikzset{edge_color8/.style={color=gray, line width=1.2pt}} \n"
    "\\tikzset{edge_colorroot/.style={color=red, line width=1.7pt}} \n"
    "\\tikzset{edge_thin/.style={color=black}} \n"    
    "\\tikzset{edge_hidden/.style={color=black,dotted,opacity=0}} \n"    
    "\\tikzset{vertex_color1/.style={inner sep=1.7pt, outer sep=0pt, draw, circle, fill=red}} \n"
    "\\tikzset{vertex_color2/.style={inner sep=1.7pt, outer sep=0pt, draw, circle, fill=blue}} \n"
    "\\tikzset{vertex_color3/.style={inner sep=1.7pt, outer sep=0pt, draw, circle, fill=green}} \n"
    "\\tikzset{vertex_color4/.style={inner sep=1.7pt, outer sep=0pt, draw, circle, fill=pink}} \n"
    "\\tikzset{labeled_vertex_color1/.style={inner sep=2.2pt, outer sep=0pt, draw, rectangle, fill=red}} \n"
    "\\tikzset{labeled_vertex_color2/.style={inner sep=2.2pt, outer sep=0pt, draw, rectangle, fill=blue}} \n"
    "\\tikzset{labeled_vertex_color3/.style={inner sep=2.2pt, outer sep=0pt, draw, rectangle, fill=green}} \n"
    "\\tikzset{labeled_vertex_color4/.style={inner sep=2.2pt, outer sep=0pt, draw, rectangle, fill=pink}} \n"
    "\n"
    "\\def\\outercycle#1#2{ \n"
    "\\pgfmathtruncatemacro{\\plusone}{#1+1} \n"
#if defined(G_ORDERED_VERTICES) 
    "\\draw  \\foreach \\x in {0,1,...,\\plusone}{(\\x,0) coordinate(x\\x)};} \n"    
#else
#if defined(G_4EDGES) || defined(G_ROOTED_4EDGES) || defined(G_3EDGES) || defined(G_ROOTED_3EDGES) || defined(G_COLORED_3EDGES) || defined(G_MAYBE_ROOTED_KEDGES)
    "\\pgfmathtruncatemacro{\\minusone}{#1-1} \n"
    "\\draw  \\foreach \\x in {0,1,...,\\minusone}{(0.5*\\x,0) coordinate(x\\x)};} \n"    
#else
// \def\outercycle#1#2{ 
//\pgfmathtruncatemacro{\plusone}{#2+1}
//\pgfmathtruncatemacro{\shift}{270- (360/#2)/2}
//\draw \foreach \x in {0,1,...,\plusone}{(\shift+\x*360/#2:0.9) coordinate(x\x)};
    "\\pgfmathtruncatemacro{\\zeroshift}{270 - (#2-1)*360/#1/2 } \n"
//    "\\draw  \\foreach \\x in {0,1,...,#1}{(270-360/#1/2+\\x*360/#1:1) coordinate(x\\x)};} \n"
    "\\draw  \\foreach \\x in {0,1,...,#1}{(\\zeroshift+\\x*360/#1:1) coordinate(x\\x)};} \n"
//    "\\foreach \\x in {0,1,...,#1}{(270-45+\\x*360/#2:1) coordinate(x\\x)};} \n"
#endif
#endif  
    "\\def\\drawhypervertex#1#2{ \\draw[edge_color2] (x#1)++(0,-0.2-0.2*#2)+(-0.2,0) -- +(0.2,0);} \n"
    "\\def\\drawrootedhypervertex#1#2{ \\draw[edge_color8] (x#1)++(0,-0.2-0.2*#2)+(-0.2,0) -- +(0.2,0);} \n"
    "\\def\\drawrootedhyperroot#1#2{ \\draw[edge_colorroot] (x#1)++(0,-0.2-0.2*#2)+(-0.2,0) -- +(0.2,0);} \n"
    "\\def\\drawhypervertexcolor#1#2#3{ \\draw[edge_color#3] (x#1)++(0,-0.2-0.2*#2)+(-0.2,0) -- +(0.2,0);} \n"
    "\\def\\drawhyperedge#1#2{ \\draw[dotted] (x0)++(0,-0.2-0.2*#1)--++(0.5*#2-0.5,0);} \n"
    "\n"
    "%\\def\\labelvertex#1{\\draw (x#1) node[below]{#1}; }\n"
    "\\def\\labelvertex#1{\\pgfmathtruncatemacro{\\vertexlabel}{#1+1 } \\draw (x#1) node{\\tiny\\vertexlabel}; }\n"
    "%\\def\\labelvertex#1{}"
    "\n\n\\begin{document}\n"
    << endl;    
}

// A human friendly printout that can be process by latex - makes it easier
// to check if the program undersood input. It is not implemented completely.
//
void print_problem_in_latex(string objective_file_name, bool color_1_nonedge = false)
{
    stringstream filename;
    if (objective_file_name != "")
        filename << objective_file_name << "__latex.tex";
    else 
        filename << filename_prefix() << "__latex.tex";
    
    ofstream outfile;
    outfile.open (filename.str().c_str(), ofstream::out);
    if (!outfile.good())
    {
        cerr << "Failed opening file " << filename.str() << endl;
        return;
    }
    
    cerr << "Writing problem formulation in latex to a file " << filename.str() << endl;
    
    print_latex_header(outfile, color_1_nonedge);


    outfile << "Problem: \\verb!" << filename_prefix() << "!\n\n";

    outfile << "Description:\n  \\begin{verbatim}" << g_program_description << "\\end{verbatim}\n";
    
    outfile << "Compile options: \\begin{itemize}\n"
#ifdef G_COLORED_VERTICES      
        "\\item Vertices have " << G_COLORED_VERTICES << " colors \n"
#endif   
#ifdef G_COLORED_VERTICES_BLIND  
        "\\item Vertices are treated colorblind\n"
#endif
#ifdef G_COLORED_VERTICES_SAMPLED_SEPARATELY_BY_COLORS
        "\\item Vertices are treated as if sampled separately by colors\n"
#endif
#ifdef G_COLORED_EDGES
        "\\item Edges have "<< G_COLORED_EDGES << " colors\n"
        "\\item Color coding:  0 black, 1 red, 2 blue, 3 green, 4 red dotted, 5 blue dotted, 6 green dotted\n"
#endif
#ifdef G_COLORED_EDGES_BLIND 
        "\\item Edges are treated colorblind TODO write color permutations\n"
#endif
#ifdef G_ORIENTED_EDGES
    "\\item Edges are oriented \n"
#endif    
#ifdef G_ORIENTED_EDGES_UNORIENTED_COLORS
    "\\item colors up " << G_ORIENTED_EDGES_UNORIENTED_COLORS << " are not oriented\n"
#endif
#ifdef G_ORIENTED_EDGES_BLIND
    "\\item Edges are orientedblind \n"
#endif
#ifdef G_3EDGES                  //       enable 3-edges
        "\\item 3-edges enabled \n"   
#endif
#ifdef G_ROOTED_3EDGES                
        "\\item rooted 3-edges enabled \n"   
#endif
#ifdef G_COLORED_3EDGES
    "\\item 3-edges have "<< G_COLORED_3EDGES << " colors\n"   
#endif    
#ifdef G_COLORED_3EDGES_BLIND   
        "\\item 3-edges are treated colorblind TODO write color permutations\n"    
#endif
#ifdef G_4EDGES
        "\\item 4-edges enabled \n"
#endif
#ifdef G_ROOTED_4EDGES
        "\\item rooted 4-edges enabled \n"
#endif
#ifdef G_MAYBE_ROOTED_KEDGES
        "\\item maybe rooted " << G_MAYBE_ROOTED_KEDGES << "-edges enabled \n"
#endif
#ifdef G_ROTATION_SYSTEM
    "\\item rotation systems enabled \n"
#endif  
#ifdef G_ROTATION_SYSTEM_REVERSE_BLIND
    "\\item Rotation systems are reverse blind\n"
#endif
#ifdef USE_FOR_CROSSINGS
    "\\item Compiled for crossings counting\n"
#endif
#ifdef G_CROSSINGS
    "\\item crossings of pairs enabled\n"
#endif
#ifdef G_CUTTINGS 
    "\\item Cuttings of edges enables\n"
#endif
#ifdef G_LEFTRIGHT
    "\\item left-right enabled\n"
#endif    
#ifdef G_LEFTRIGHT_BLIND
    "\\item left-right is reflection blind\n"
#endif
#ifdef G_LEFTRIGHT_LOCAL
    << "\\item left-right is only local\n"
#endif 
#ifdef G_ORDER_TYPES
    "\\item order types enabled \n"
#endif 
#ifdef G_ORDERED_VERTICES        // nit working
        "\\item Vertices are treated ordered (DOES NOT WORK!!) \n"
#endif
#ifdef G_USE_PERMITTED_SUBFLAGS
        "\\item Using lists of permitted graphs\n"
#endif
#ifdef G_USE_FIRST_EDGE_COLOR_FOR_BLOWUP_ONLY
    "\\item Using first edge color to generate blow-ups\n"
#endif
    
    "\\end{itemize}\n";
    

    if (objective_file_name == "")
        outfile << "\n\n Objective file name: default \n";
    else    
        outfile << "\n\n Objective file name: \\verb!" << objective_file_name << "!\n";

    
    outfile << "\n\n Forbidden flags: " << g_forbidden_subflags.size() << " \\begin{center}\n";
    for (int i = 0; i < (int)g_forbidden_subflags.size(); i++)
    {
        outfile << g_forbidden_subflags[i].print_latex(false,0);
    }
    outfile << "\\end{center}\n";

#ifdef G_USE_PERMITTED_SUBFLAGS
    for (int j = 0; j < V; j++)
    {
        outfile << "\n\n Permitted subflags of size "<< j << ": " << g_permitted_subflags[j].size() << " \\begin{center}\n";
        for (int i = 0; i < (int)g_forbidden_subflags.size(); i++)
        {
            outfile << g_permitted_subflags[j][i].print_latex(false,0);
        }
        outfile << "\\end{center}\n";
    }
#endif    

    outfile << "\n\n Objective is a linear combination of  " << g_objective_combination.size() << " flag(s): \\begin{center}\n";
    for (int i = 0; i < (int)g_objective_combination.size(); i++)
    {
        outfile << std::setprecision(G_PRECISION);
        if (g_objective_combination[i].coefficient >= 0)
            outfile << "+" << g_objective_combination[i].coefficient << " \\vc{" << g_objective_combination[i].g.print_latex(false,0) << "}";
        else
            outfile << g_objective_combination[i].coefficient << " \\vc{" << g_objective_combination[i].g.print_latex(false,0) << "}";            
    }
    outfile << "\\end{center}\n";

    
    outfile << "\\newpage \n\n Linear constraints count : " << g_linear_constraints.size() << "\n";
    for (int i = 0; i < (int)g_linear_constraints.size(); i++)
    {
        outfile << " \\begin{center}\n";
        outfile << " $0 \\leq $";
//        outfile << std::setprecision(G_PRECISION) << g_linear_constraints[i].m_constant;
        outfile << std::setprecision(G_PRECISION) << g_linear_constraints[i].m_constant;
        for (int j = 0; j < (int)g_linear_constraints[i].m_entries.size(); j++)
        {
            if (g_linear_constraints[i].m_entries[j].coefficient >= 0)
                outfile << " +" << g_linear_constraints[i].m_entries[j].coefficient << "~\\vc{" << g_linear_constraints[i].m_entries[j].g.print_latex(false,0) << "}";  
            else
                outfile << " " << g_linear_constraints[i].m_entries[j].coefficient << "~\\vc{" << g_linear_constraints[i].m_entries[j].g.print_latex(false,0) << "}";
        }
        outfile << "\\end{center}\n";
    }
    
//    string print_latex(bool use_label, const T &graph_label) const;

    outfile << "\\end{document}\n";
    outfile.close();
    
    execlp("pdflatex","pdflatex",filename.str().c_str(),(char *)NULL);    
}




void draw_graphs(const string& path, bool file_has_with_densities, bool color_1_nonedge = false,  bool latex_enumerated = false, bool use_cout = false)
{
    OPEN_FILE_SMARTLY(istr, path);

    
    ostream *ostr = &std::cout;

    stringstream filename;
    filename << path << "__latex.tex";
    
    ofstream outfile;

    if (use_cout == false)
    {
        outfile.open (filename.str().c_str(), ofstream::out);
        if (!outfile.good())
        {
            cerr << "Failed opening file " << filename.str() << endl;
            return;
        }
        ostr = &outfile;
    }


    
    //cerr << "Writing problem formulation in latex to a file " << filename.str() << endl;
    print_latex_header((*ostr), color_1_nonedge);    
    //outfile << "Problem: \\verb!" << filename_prefix() << "!\n\n";

    
    
    /*
    vector<flag> drawn;
    
    flag h;
    while (h.load_from_stream(infile,-1,0))
    {
        if (find_flag_in_list(h, drawn) < 0)
        {
            
            outfile << h.print_latex(false,0);
            drawn.push_back(h);
        }
    }
    */
    
    if (latex_enumerated)
    {
        (*ostr) << "%\\begin{multicols}{2}" << endl;
        (*ostr) << "\\begin{enumerate}" << endl;
    }
    
    double coefficient = 0.0;
    flag h;
    if (file_has_with_densities)
    {
        (*istr) >> coefficient;
    }
    while (h.load_from_stream((*istr),-1,-1))
    {
        //if (!is_flag_forbidden(h))
        {

            (*ostr) << "% " << h.print() << endl;

            if (latex_enumerated)
            {
                (*ostr) << "\\item" << endl;
            }

            if (file_has_with_densities)
            {
                //cerr << "Printing " << coefficient << endl;
                //if (abs(coefficient) < 0.0002)
                //    outfile << 0;
                ///else 
                    (*ostr).precision(G_PRECISION);
                    (*ostr) << coefficient << " ";
            }
            
            (*ostr) << "\\vc{"<<h.print_latex(false,0)<<"}\n" ;
            //g_forbidden_subflags.push_back(h);
        }
        if (file_has_with_densities)
        {
            (*istr) >> coefficient;
        }
    }
    

    if (latex_enumerated)
    {
        (*ostr) << "\\end{enumerate}" << endl;
        (*ostr) << "%\\end{multicols}" << endl;
    }

    (*ostr) << "\\end{document}\n";

    if (use_cout == false)
    {
        (*ostr).flush();
        execlp("pdflatex","pdflatex",filename.str().c_str(),(char *)NULL);
    }
}



void compute_densities_in(const string& path, int Kn, int verbose=0)
{
    vector<flag_and_coefficient> big_flags;

    OPEN_FILE_SMARTLY(istr, path);


    flag_and_coefficient fc;
    
    (*istr) >> fc.coefficient;
    while (fc.g.load_from_stream((*istr),-1,0))
    {
        big_flags.push_back(fc);
        //cout << fc.coefficient << " " << fc.g.print() << endl;
        (*istr) >> fc.coefficient;
    }

    cerr << "Loaded " << big_flags.size() << " flags from " << path << endl;
    
    double all_densitysum = 0;
    
    stringstream density_vector;
    density_vector << "density_vector=vector([";

    for (int i = 0; i < (int)g_unlabeled_flags[Kn].size(); i++)
    {
        double densitysum = 0;
        for (int j = 0; j < (int)big_flags.size(); j++)
        {
            if (big_flags[j].g.m_vertices >= Kn)
            {              
                densitysum += big_flags[j].coefficient*P_F1_IN_H(g_unlabeled_flags[Kn][i], big_flags[j].g);
            }
            else
            {
                densitysum += big_flags[j].coefficient*P_F1_IN_H(big_flags[j].g, g_unlabeled_flags[Kn][i]);                
            }
        }
        std::cout.precision(G_PRECISION);
        cout << densitysum << " " <<  g_unlabeled_flags[Kn][i].print() << " # " <<  i+1 << endl;
        density_vector.precision(G_PRECISION);
        density_vector << densitysum << ", ";
        all_densitysum += densitysum;
    }
    density_vector << "])";
     std::cout.precision(G_PRECISION);
    cerr << "Sum of all densities is " << all_densitysum << endl;
    if (verbose)
        cerr << density_vector.str() << endl;
}

// filter_allowed means that the pathfilter contain allowed graphs
// otherwise is contains forbidden graphs
void filter_flags_remove_duplicates(const string& pathall, bool coefficients_in_input)
{
    vector<flag> new_flags;    

    OPEN_FILE_SMARTLY(istr, pathall);

    flag_and_coefficient fc;

    int processed=0;
    int not_duplicates = 0;

    if (coefficients_in_input) (*istr) >> fc.coefficient;
    while (fc.g.load_from_stream((*istr),-1,-1))
    {
        processed++;
        if (find_flag_in_list(fc.g,new_flags) == -1)
        {
            not_duplicates++;
            if (coefficients_in_input)
            {
                std::cout.precision(G_PRECISION);
                cout << fc.coefficient << " ";
            }
            cout << fc.g.print() << endl; // << " # " << not_duplicates++ << endl;
            new_flags.push_back(fc.g);
        }
        if (coefficients_in_input) (*istr) >> fc.coefficient;
    } 
    cerr <<  "Processed: " << processed << " kept: " << not_duplicates << " duplicates: " << processed-not_duplicates << endl;
}

// filter_allowed means that the pathfilter contain allowed graphs
// otherwise is contains forbidden graphs
void filter_flags(const string& pathall, const string& pathfilter, bool filter_allowed, bool coefficients_in_input)
{
    vector<flag> filter_flags;
    load_flags_from_file(pathfilter, filter_flags);
    cerr << "Loaded " << filter_flags.size() << " flags ";
    if (filter_allowed) cerr << "that are allowed" << endl;
    else cerr << "that are forbidden" << endl;
    

    OPEN_FILE_SMARTLY(istr, pathall);


    flag_and_coefficient fc;

    if (coefficients_in_input) (*istr) >> fc.coefficient;
    while (fc.g.load_from_stream((*istr),-1,-1))
    {
        if (find_flag_in_list(fc.g,filter_flags) != -1)
        {
            if (filter_allowed == true)
            {
                if (coefficients_in_input)
                {
                    std::cout.precision(G_PRECISION);
                    cout << fc.coefficient << " ";
                }
                cout << fc.g.print() << endl;
            }
        }
        else
        {
            if (filter_allowed == false)
            {
                if (coefficients_in_input)
                {
                    std::cout.precision(G_PRECISION);
                    cout << fc.coefficient << " ";
                }
                cout << fc.g.print() << endl;
            }
        }
        if (coefficients_in_input) (*istr) >> fc.coefficient;
    }    
}


void filter_flags_using_subflags_fun(const string& pathall, const string& pathsubflagfilter, bool filter_forbidden_subflag, bool coefficients_in_input)
{
    vector<flag> filter_subflags;
    load_flags_from_file(pathsubflagfilter, filter_subflags);
    cerr << "Loaded " << filter_subflags.size() << " flags that" << endl;
    if (filter_forbidden_subflag)
    {
        cerr << " are forbidden as subflags" << endl;
    }
    else
    {
        cerr << " must at least once occur as subflags" << endl;
    }
    
    OPEN_FILE_SMARTLY(istr, pathall);

    flag_and_coefficient fc;
    
    if (coefficients_in_input) (*istr) >> fc.coefficient;
    while (fc.g.load_from_stream((*istr),-1,-1))
    {
        bool contains_subflag = false;
        for (int i = 0; i < (int)filter_subflags.size(); i++)
        {
#ifdef G_FORBIDDEN_NON_INDUCED
            if (filter_forbidden_subflag == true)
            {
                if (fc.g.has_as_notinduced_subflag(filter_subflags[i]))
                {
                    contains_subflag = true;
                    break;
                }
            }
            else
#endif                        
            if (fc.g.contains_as_subflag(filter_subflags[i]))
            {
                contains_subflag = true;
                break;
            }
        }
        
        if (contains_subflag == false && filter_forbidden_subflag == true)
        {
                if (coefficients_in_input)
                {
                    std::cout.precision(G_PRECISION);
                    cout << fc.coefficient << " ";
                }
                cout << fc.g.print() << endl;
        }
        
        if (coefficients_in_input) (*istr) >> fc.coefficient;
    }
    
    //infile.close();
}


inline bool EXISTS_F_IN_blowup_of_H(const flag &F1, const flag &H)
{
    
#ifdef G_ORDERED_VERTICES
    assert(0); // not implemented
#else
    
    // Take mappings of vertices from F1 to H .... Then count how many are isomorphic to the blow-up.
    int mapping[V];
    for (int i = 0; i < V; i++)
    {
        mapping[i] = 0;
    }
    
    while (mapping[F1.m_vertices] == 0)
    {
        // Test current mapping
        flag tmp;
        tmp.as_subflag_in_blowup(H,mapping,F1.m_vertices, F1.m_Theta);
        
        if (tmp.is_isomorphic_to(F1)) 
        {
            return true;
        }
        
        // Increment mapping
        // Keep ordering of the array such that mapping[0] >= mapping[1] >= mapping[2] >=....
        bool add_one = true;
        int  add_to = 0;  // Coordinate in the array that overflew...
        while (add_one)
        {
            if (++mapping[add_to] >= H.m_vertices)
            {
                    add_to++;                    
            }
            else
            {
                while (--add_to >= 0)
                {
                    mapping[add_to] = mapping[add_to+1];
                }
                add_one = false;
            }
        }
    }
    
    return false;
#endif
}


inline double P_F1_IN_blowup_of_H(const flag &F1, const flag &H, bool density=true)
{
    
#ifdef G_ORDERED_VERTICES
    assert(0); // not implemented
#else
    
    // Take mappings of vertices from F1 to H .... Then count how many are isomorphic to the blow-up.
    int mapping[V];
    for (int i = 0; i < V; i++)
    {
        mapping[i] = 0;
    }

#ifdef G_COLORED_VERTICES_SAMPLED_SEPARATELY_BY_COLORS
    cerr << "Currenlty not implemented" << endl;
    assert(0);
#endif

    int64_t good_maps = 0;
    int64_t all_maps = 0;
    
    
    // Compute all maps as H.m_vertices ^ F1.m_vertices
    all_maps = 1;
    for (int p = 0; p < F1.m_vertices; p++) all_maps *= H.m_vertices;
    
    // We are doing this because it can run in parallel
//    for (int64_t map_id=0; map_id < all_maps; map_id++)
/*
    all_maps = 1000000;
#pragma omp parallel
    for (int64_t map_id=0; map_id < all_maps; map_id++)
    {
        int64_t map_id_tmp = ((((int64_t)rand())<<32)+(int64_t)(rand()))%all_maps;
        int mapping[V];
        for (int i = 0; i < V; i++)
        {
            mapping[i] = map_id_tmp%H.m_vertices;
            map_id_tmp /= H.m_vertices;
        }
        // Test current mapping
        flag tmp;
        tmp.as_subflag_in_blowup(H,mapping,F1.m_vertices, F1.m_Theta);
        
        bool is_isomorphic = tmp.is_isomorphic_to(F1);
        {
            if (is_isomorphic) 
            {
#pragma omp critical
                good_maps++;
            }
        }
    }
 */
    //cout << good_maps << " " << all_maps << endl;

    //cout << "All maps=" << all_maps << endl;
    
    all_maps = 0;
    good_maps = 0;
    while (mapping[F1.m_vertices] == 0)
    {
        //if (all_maps %100000000 == 0) cout << all_maps << endl;
        //cerr << all_maps << endl;
        
        // Test current mapping
        flag tmp;
        tmp.as_subflag_in_blowup(H,mapping,F1.m_vertices, F1.m_Theta);

        all_maps++;
        if (tmp.is_isomorphic_to(F1)) 
        {
            good_maps++;
        }
        
                
        // Increment mapping
        bool add_one = true;
        int  add_to = 0;
        while (add_one)
        {
            if (++mapping[add_to] >= H.m_vertices)
            {
                mapping[add_to] = 0;
                add_to++;
            }
            else
            {
                add_one = false;
            }
        }
    }

    //cout << good_maps << " " << all_maps << endl;

//   return good_maps;
//    cout << all_maps << endl;
    
    if (density)
        return (double)good_maps/(double)all_maps;
    else
        return good_maps;
#endif
}


void print_F1_times_F2(double coeff, const flag &F1, const flag &F2)
{
    flag type;
    F1.get_type_subflag(type);
    vector<flag_and_coefficient> F1F2 = F1_times_F2(F1, F2, type);

    for (int i = 0; i < (int)F1F2.size(); i++)
    {
        cout.precision(G_PRECISION);
        cout << coeff*F1F2[i].coefficient << " " << F1F2[i].g.print() << endl;
    }
}

    


    
    
    // Builds extremal vectors for type i and in blowup of graph B
    // Extremal vector is a vector with coordinates being (labeled) flags f of type i
    //  Now the entry in the vector is p(f,B)
    //  We try to compute all possible rootings of the type and for the rooting of the type
    //  we compute the rest of the vertices. Each rooting could give us a different flag.
    //
    // Output is a vector, that contains for each flag of a given type 'coefficient' The coefficient
    // is a sum (another vector) of products (another vector).
    // The products are indexed by the numeber of vertices in B and it is saying how many verties were mapped to each
    // vertex in b. 
    // To make a simpler interpretation of this extremal vector, see the following two functions below 
vector<vector<vector<int> > > find_extremal_vectors_in_blow_up_of_B_for_vector_of_flags_with_fixed_theta(const flag &B, vector<flag> f_for_test, int *mapping_Theta, bool print_output = true)
    {
        // We have one mapping of Theta. Now count how many ways it extends to each flag.
        
        // Entry is 
        vector<vector<vector<int> > > one_extremal_vector;

        // Go over rooting of the type part of the mapping
        if (f_for_test.size() == 0) 
        {   
//            assert(0);
            return one_extremal_vector;
        }
        
        int Theta    = f_for_test[0].m_Theta;
                           
        vector<vector<int> > one_entry;
        one_extremal_vector.resize(f_for_test.size(), one_entry);
        
        
            for (int j = 0; j < (int)f_for_test.size(); j++)
            {
                flag f;
                f = f_for_test[j];
                
                //int good_maps = 0;
                vector<vector<int> > good_maps;
                
                string good_maps_str = "0";
                
                int mapping[V+1];
                for (int i = 0; i < V+1; i++)
                {
                    mapping[i] = mapping_Theta[i];
                }
                
                while (mapping[f.m_vertices] == 0)
                {
                    // Test current mapping
                    flag tmp;
                    tmp.as_subflag_in_blowup(B,mapping,f.m_vertices, f.m_Theta);
                    if (tmp.is_isomorphic_to(f)) 
                    {
                        vector<int> mapping_list;
                        mapping_list.resize(B.m_vertices, 0);
                        

                        for (int i = f.m_Theta; i < f.m_vertices; i++)
                        {
                            mapping_list[mapping[i]]++;
                        }
                    
                        good_maps.push_back(mapping_list);
                    }
                    
                    
                    // Increment mapping
                    bool add_one = true;
                    int  add_to = Theta; // Change mapping only after theta
                    while (add_one)
                    {
                        if (++mapping[add_to] >= B.m_vertices)
                        {
                            mapping[add_to] = 0;
                            add_to++;
                        }
                        else
                        {
                            add_one = false;
                        }
                    }
                }
                one_extremal_vector[j] = good_maps;
                //cout << good_maps << " ";
            }
            //cout << endl;
            
            // Add new extremal vector if found...
        if (print_output)
        {
            cout << "[";
            cout << endl;
            for (int k = 0; k < (int)one_extremal_vector.size(); k++)
            {
                if (k != 0) cout << ",";
                for (int x = 0; x < (int)one_extremal_vector[k].size(); x++)
                {
                    cout << "+";
                    for (int y = 0; y < (int)one_extremal_vector[k][x].size(); y++)
                    {
                        cout << one_extremal_vector[k][x][y];
                    }                    
                }
                
            }
            cout << "]";
        }
        
        return one_extremal_vector;
    }



vector<int> big_extremal_vector_to_int_weighted(const vector<vector<vector<int> > > &big_vector, vector<int> &weights)
{
    vector<int> one_extremal_vector;
    
        for (int j = 0; j < (int)big_vector.size(); j++)
        {
            int weight_sum = 0;
            for (int x = 0; x < (int)big_vector[j].size(); x++)
            {
                
                int one_weight=1;
                for (int y = 0; y < (int)big_vector[j][x].size(); y++)
                {
                    for (int z = 0; z < big_vector[j][x][y]; z++)
                    {
                        one_weight *= weights[y];
                    }
                }
                weight_sum += one_weight;
            }
            
            one_extremal_vector.push_back(weight_sum);
        }
    return  one_extremal_vector;   
}


vector<string> big_extremal_vector_to_str_weighted(const vector<vector<vector<int> > > &big_vector, vector<string> &weights_str)
{
    vector<string> one_extremal_vector;
    
    for (int j = 0; j < (int)big_vector.size(); j++)
    {
        string weight_sum = "";
        for (int x = 0; x < (int)big_vector[j].size(); x++)
        {
            
            string one_weight="";
            for (int y = 0; y < (int)big_vector[j][x].size(); y++)
            {
                for (int z = 0; z < big_vector[j][x][y]; z++)
                {
                    if (one_weight.length() != 0) one_weight.append("*");
                    one_weight.append(weights_str[y]);
                }
            }
            if (one_weight.length() != 0)
            {
                if (weight_sum.length() != 0) weight_sum.append("+");
                weight_sum.append(one_weight);
            }
        }
        
        if (weight_sum.length() == 0) weight_sum = "0";
        one_extremal_vector.push_back(weight_sum);
    }
    return  one_extremal_vector;   
}


    // Builds extremal vectors for type i and in blowup of graph B
    // Extremal vector is a vector with coordinates being (labeled) flags f of type i
    //  Now the entry in the vector is p(f,B)
    //  We try to compute all possible rootings of the type and for the rooting of the type
    //  we compute the rest of the vertices. Each rooting could give us a different flag.
    //
    //
    //
    //
    void find_extremal_vectors_in_blow_up_of_B_for_vector_of_flags(const flag &B, vector<flag> f_for_test, vector<int> &weights, vector<string> &weights_str, bool python_output, int verbose_output, bool use_weights_str)
    {
        // Go over rooting of the type part of the mapping
        if (f_for_test.size() == 0) return;
        int Theta    = f_for_test[0].m_Theta;
        //    int vertices = f_for_test[0].m_vertices;
        
        // First create mapping of Theta
        int mapping_Theta[V+1];
        for (int j = 0; j < V+1; j++)
        {
            mapping_Theta[j] = 0;
        }
        
        vector<vector<vector<vector<int> > > > extremal_vectors;
        //vector< vector<int> > extremal_zero(f_for_test.size(),0);

        vector<vector<int> >  extremal_vectors_int;
        vector<vector<string> >  extremal_vectors_str;
        //vector< vector<int> > extremal_zero(f_for_test.size(),0);
        //vector< vector<int> > extremal_zero_str(f_for_test.size(),"");
        
        while (mapping_Theta[Theta] == 0)
        {
            //cout << "One rooting does: ";
            // We have one mapping of Theta. Now count how many ways it extends to each flag.
//            vector<vector<vector<int> > > one_extremal_vector;
            vector<vector<vector<int> > > one_extremal_vector;
            //one_extremal_vector.resize(f_for_test.size(),0);
            
//            one_extremal_vector = find_extremal_vectors_in_blow_up_of_B_for_vector_of_flags_with_fixed_theta(B, f_for_test, weights, weights_str, mapping_Theta, false);
            one_extremal_vector = find_extremal_vectors_in_blow_up_of_B_for_vector_of_flags_with_fixed_theta(B, f_for_test,  mapping_Theta, false);

            
            // Add new extremal vector if found...
            bool vector_empty = true;
            for (int j = 0; j < (int)one_extremal_vector.size(); j++)
            {
                if (one_extremal_vector[j].size() > 0)
                {
                    vector_empty = false;
                    break;
                }
            }
            
            if (vector_empty == false)
            {                
                // Integer weights
                if (use_weights_str == false)
                {
                    vector<int> one_extremal_vector_int =  big_extremal_vector_to_int_weighted(one_extremal_vector, weights);

                    // Test if one_extremal_vector is already among discovered vectors
                    bool match=false;
                    for (int k = 0; k < (int)extremal_vectors_int.size(); k++)
                    {
                        if (extremal_vectors_int[k] == one_extremal_vector_int)
                        {
                            match = true;
                            break;
                        }
                    }
                    if (!match) 
                    {
                        if (python_output)
                        {
                            if (!extremal_vectors_int.empty()) cout << ",";
                            cout << "[";
                            for (int j = 0; j < (int)one_extremal_vector_int.size(); j++)
                            {
                                    if (j != 0) cout << ",";                            
                                    cout << one_extremal_vector_int[j];
                            }
                            cout << "]";                        
                        }
                        else
                        {
                            assert(0);
                        }
                        extremal_vectors_int.push_back(one_extremal_vector_int);
                    }                   
                }
                else
                {
                    vector<string> one_extremal_vector_str =  big_extremal_vector_to_str_weighted(one_extremal_vector, weights_str);
                    
                    // Test if one_extremal_vector is already among discovered vectors
                    bool match=false;
                    for (int k = 0; k < (int)extremal_vectors_str.size(); k++)
                    {
                        if (extremal_vectors_str[k] == one_extremal_vector_str)
                        {
                            match = true;
                            break;
                        }
                    }
                    if (!match) 
                    {
                        if (python_output)
                        {
                            if (!extremal_vectors_str.empty()) cout << ",";
                            cout << "[";
                            for (int j = 0; j < (int)one_extremal_vector_str.size(); j++)
                            {
                                if (j != 0) cout << ",";                            
                                cout << one_extremal_vector_str[j];
                            }
                            cout << "]";                        
                        }
                        else
                        {
                            assert(0);
                        }
                        extremal_vectors_str.push_back(one_extremal_vector_str);
                    }                                       
                }
                
            }
            
            
            // Increment mapping
            bool add_one = true;
            int  add_to = 0;
            while (add_one)
            {
                if (++mapping_Theta[add_to] >= B.m_vertices)
                {
                    mapping_Theta[add_to] = 0;
                    add_to++;
                }
                else
                {
                    add_one = false;
                }
            }
        }
    }
    
    
// Builds extremal vectors for type i and in blowup of graph B
// Extremal vector is a vector with coordinates being (labeled) flags f of type i
//  Now the entry in the vector is p(f,B)
//  We try to compute all possible rootings of the type and for the rooting of the type
//  we compute the rest of the vertices. Each rooting could give us a different flag.
//
//
//void find_extremal_vectors_in_blow_up_of_B_for_type(const flag &B, int i, vector<int> &weights)
//{
//    find_extremal_vectors_in_blow_up_of_B_for_vector_of_flags(B, g_flags[i], weights);
//    return;
/*    
    
    // Go over rooting of the type part of the mapping
    if (g_flags[i].size() == 0) return;
    int Theta    = g_flags[i][0].m_Theta;
//    int vertices = g_flags[i][0].m_vertices;
    
    // First create mapping of Theta
    int mapping_Theta[V+1];
    for (int j = 0; j < V+1; j++)
    {
        mapping_Theta[j] = 0;
    }
    
    
    vector< vector<int> > extremal_vectors;
    vector<int> extremal_zero(g_flags[i].size(),0);

    
    
    while (mapping_Theta[Theta] == 0)
    {
        //cout << "One rooting does: ";
        // We have one mapping of Theta. Now count how many ways it extends to each flag.
        vector<int> one_extremal_vector;
        one_extremal_vector.resize(g_flags[i].size(),0);
        for (int j = 0; j < (int)g_flags[i].size(); j++)
        {
            flag f;
            f = g_flags[i][j];

            int good_maps = 0;
            int all_maps = 0;

            int mapping[V+1];
            for (int i = 0; i < V+1; i++)
            {
                mapping[i] = mapping_Theta[i];
            }
            
            while (mapping[f.m_vertices] == 0)
            {
                // Test current mapping
                flag tmp;
                tmp.as_subflag_in_blowup(B,mapping,f.m_vertices, f.m_Theta);
                
                int mapping_weight = 1;
                for (int i = f.m_Theta; i < f.m_vertices; i++)
                {
                    mapping_weight *= weights[mapping[i]];
                }
                
                all_maps += mapping_weight;
                
                
                //HACK:
                //
                //for (int u = 0; u < f.m_Theta; u++)
                //    for (int v = 0; v < f.m_Theta; v++)
                //        if (tmp.m_color_edge[u][v] == 1 && f.m_color_edge[u][v] == 2)
                //        {
                //            tmp.color_edge(u,v,2);
                //        }
                
                
                if (tmp.is_isomorphic_to(f)) 
                {
                    good_maps += mapping_weight;
                }
                
                
                // Increment mapping
                bool add_one = true;
                int  add_to = Theta; // Change mapping only after theta
                while (add_one)
                {
                    if (++mapping[add_to] >= B.m_vertices)
                    {
                        mapping[add_to] = 0;
                        add_to++;
                    }
                    else
                    {
                        add_one = false;
                    }
                }
            }
            one_extremal_vector[j] = good_maps;
            //cout << good_maps << " ";
        }
        //cout << endl;
        
        // Add new extremal vector if found...
        if (one_extremal_vector != extremal_zero)
        {
            bool match=false;
            for (int k = 0; k < (int)extremal_vectors.size(); k++)
                if (one_extremal_vector == extremal_vectors[k]) match=true;
            if (!match)
            {
                if (!extremal_vectors.empty()) cout << ",";
                cout << "[";
                extremal_vectors.push_back(one_extremal_vector);
                for (int k = 0; k < (int)one_extremal_vector.size(); k++)
                {
                    if (k != 0) cout << ",";
                    cout << one_extremal_vector[k];
                }
                cout << "]";
            }
        }
        
        
        // Increment mapping
        bool add_one = true;
        int  add_to = 0;
        while (add_one)
        {
            if (++mapping_Theta[add_to] >= B.m_vertices)
            {
                mapping_Theta[add_to] = 0;
                add_to++;
            }
            else
            {
                add_one = false;
            }
        }
    }
 */
//}


vector<int> find_extremal_vectors_read_weights(const flag &B, const string &weights_str)
{
    vector<int> weights;

    if (weights_str == "")
    {
        for (int i = 0; i < B.m_vertices; i++)
        weights.push_back(1);
    }
    else
    {
        stringstream ss(weights_str);
        for (int i = 0; i < B.m_vertices; i++)
        {
            string tmp;
            ss >> tmp;
            int w = stol(tmp);
            if (w == 0)
            {
                cerr << "Unable to convert '" << weights_str << "' in " <<  B.m_vertices << " integer weights." << endl;
                cerr << "Failed at index " << i << endl;
                assert(0);
                return weights;
            }
            weights.push_back(w);
        }
    }
    return weights;
}


vector<string> find_extremal_vectors_read_weights_str(const flag &B, const string &weights_str)
{
    vector<string> weights;
    
    if (weights_str == "")
    {
        for (int i = 0; i < B.m_vertices; i++)
            weights.push_back("");
        //cerr << "Empty" << endl;
    }
    else
    {
        stringstream ss(weights_str);
        for (int i = 0; i < B.m_vertices; i++)
        {
            string tmp;
            ss >> tmp;
            weights.push_back(tmp);
            //cout << tmp << endl;
        }
    }
    return weights;
}


void find_extremal_vectors_process_colors(const flag &B, const string &colors_str)
{
    if (colors_str == "")
    {
        //Just do nothing...
    }
    else
    {
        stringstream ss(colors_str);
        for (int i = 0; i < B.m_vertices; i++)
        {
            string tmp;
            ss >> tmp;
            int c = stol(tmp);
            if (c == 0)
            {
                cerr << "Unable to convert '" << colors_str << "' in " <<  B.m_vertices << " integer colors." << endl;
                cerr << "Failed at index " << i << endl;
                assert(0);
                return;
            }
            g_blow_up_color_edges[i] = c; 
            g_blow_up_color_3edges[i] = c; 
            g_blow_up_color_4edges[i] = c; 
        }
    }
}
    
void find_extremal_vectors_in_blow_up_of(const flag &B, const string &weights_in_str, const string &weights_str_in_str, const string &colors_str, bool python_output, int verbose_output)
{
    vector<int> weights = find_extremal_vectors_read_weights(B, weights_in_str);
    vector<string> weights_str = find_extremal_vectors_read_weights_str(B, weights_str_in_str);
    find_extremal_vectors_process_colors(B, colors_str);

    if (python_output) cout << "extremalVectors=[";
    for (int i = 0; i < (int)g_flags.size(); i++)
    {
        if (verbose_output)
        {
            flag type;
            g_flags[i][0].get_type_subflag(type);
            cerr << endl << "Type " << i << " i.e. " << type.print() << endl;
        }
        if (python_output)
        {
            if (i == 0)
                cout << "[";
            else
                cout << ",[";
        }
        //find_extremal_vectors_in_blow_up_of_B_for_type(B, i, weights, verbose_output);
        find_extremal_vectors_in_blow_up_of_B_for_vector_of_flags(B, g_flags[i], weights, weights_str, python_output, verbose_output, weights_str_in_str != "");
        if (python_output) cout << "]";
    }
    if (python_output) cout << "]" << endl;
}

    

void find_projection_vectors_for_one_file(const string &find_projection_vectors_input_file)
{
    vector<flag_and_coefficient> F;
    load_flags_and_coefficients_from_file(find_projection_vectors_input_file, F);

    if (F.size() == 0)
    {
        cerr << "Input file had no flags!!" << endl;
        //return;
    }

    cerr << "Loaded linear combination(s) of " << F.size() << " flags" << endl;


    // Get all possible types
    vector<flag> F_types; 

    for (const auto &fc : F)
    {
        bool known_type = false;
        for (const auto &t : F_types)
        {
            if (fc.g.have_same_type(t))
            {
                known_type = true;
                break;
            }
        }

        if (known_type == false)
        {
            flag fc_type;
            fc.g.get_type_subflag(fc_type);
            F_types.push_back(std::move(fc_type));
        }
    }

    cerr << "Identified " << F_types.size() << " different linear combinations" << endl;

    // TODO: Projection matrices - better output.
    //cerr << g_types.size() << endl;
    cout << "projection = [[] for i in range(" << g_flags.size() << ")]" << endl;
    for (const auto &F_type : F_types)
    {
        cerr << "# Using type " << F_type.print() << endl;

        //for (const auto & fl : g_flags)
        for (int i = 0; i< (int)g_flags.size(); i++)
        {
            const vector<flag> &fl = g_flags[i];
            if (fl.size() == 0) continue;

            if (fl[0].have_same_type(F_type))
            {
                cout << "projection["<<i<<"].append([";
                for (const auto &f: fl)
                {
                    double coefficient = 0;
                    for (const auto &fc : F)
                    {
                        if (!fc.g.have_same_type(F_type))
                            continue;

                        if (fc.g.is_isomorphic_to(f))
                        {
                            coefficient += fc.coefficient;
                        }
                    }
                    cout.precision(G_PRECISION);
                    cout << coefficient << ",";
                }
                cout << "])" << endl;
                break;
            }
        }
    }
}


         
void test_tight_for_blow_up_B_func(const flag &B, const string &filename, const string &weights_str, const string &colors_str, const string &mapping_Theta_str, int verbose_output)
{
    vector<flag_and_coefficient> flag_list;
    if (!load_flags_and_coefficients_from_file(filename, flag_list))
    {
        return;
    }
    
    vector<int> weights = find_extremal_vectors_read_weights(B, weights_str);
    find_extremal_vectors_process_colors(B, colors_str);    

    vector<flag> f_for_test;
    for (int i = 0; i < (int)flag_list.size(); i++)
        f_for_test.push_back(flag_list[i].g);

    int Theta = f_for_test[0].m_Theta;

    int mapping_Theta[V+1];
    for (int j = 0; j < V+1; j++)
    {
        mapping_Theta[j] = 0;
    }
    
    // Special use, where Theta is provided
    if (mapping_Theta_str != "")
    {
        stringstream ss(mapping_Theta_str);
        for (int i = 0; i < Theta; i++)
        {
            string tmp;
            ss >> tmp;
            int T = 0;
            try {
                T = stol(tmp);
            } catch (...)
            {
                cerr << "Unable to convert '" << mapping_Theta_str << "' in " <<  Theta << " integers." << endl;
                cerr << "Failed at index " << i << endl;
                assert(0);
            }
            if (T < 0 || T >= B.m_vertices)
            {
                cerr << "At index " << i << " got matting of theta to " << T << " which is not in the blowup graph." << endl;
                assert(0);
            }
            mapping_Theta[i] = T;
        }
        
        vector<int> coefs  = big_extremal_vector_to_int_weighted(find_extremal_vectors_in_blow_up_of_B_for_vector_of_flags_with_fixed_theta(B, f_for_test, mapping_Theta, false), weights);
        
        // and now make the dot product...
        double all_sum = 0;
        bool all_zeros = true;
        for (int i = 0; i < (int)flag_list.size(); i++)
        { 
            if (coefs[i] != 0) all_zeros = false;
            //cerr << "Adding " << coefs[i] * flag_list[i].coefficient << endl;
            all_sum += coefs[i] * flag_list[i].coefficient;
        }
        
        if (all_zeros)
            cerr << "Tight since always zero coefficients" << endl;
        else
        {
        if (all_sum == 0)
            cerr << "Tight since dot product zero" << endl;
        else
            cerr << "Not tight, dot product is " << all_sum << endl;
        }
        return;
    }

    
    bool all_zeros_for_all_Thetas = true;
    int only_zeros = 0;
    int zero_sums = 0;
    int nonzero_sums = 0;
    
    // Normal use that tests all Thetas
    while (mapping_Theta[Theta] == 0)
    {
        vector<int> coefs  = big_extremal_vector_to_int_weighted(find_extremal_vectors_in_blow_up_of_B_for_vector_of_flags_with_fixed_theta(B, f_for_test, mapping_Theta, false), weights);

        // and now make the dot product...
        double all_sum = 0;
        bool all_zeros = true;
        for (int i = 0; i < (int)flag_list.size(); i++)
        { 
            if (coefs[i] != 0) all_zeros = false;
            //cerr << "Adding " << coefs[i] * flag_list[i].coefficient << endl;
            all_sum += coefs[i] * flag_list[i].coefficient;
        }
        
        if (all_zeros)
        {
            only_zeros++;
        }
        else
        {
            all_zeros_for_all_Thetas = false;
            if (all_sum == 0)
                zero_sums++;
            else
            {
                nonzero_sums++;
                if (verbose_output)
                {
                    cerr << "Theta mapping ";
                    for (int i = 0; i < Theta; i++) cerr << " " << mapping_Theta[i];
                    cerr << " gave dot product " << all_sum << endl;
                }
            }
        }
        
        
        // Increment mapping
        bool add_one = true;
        int  add_to = 0;
        while (add_one)
        {
            if (++mapping_Theta[add_to] >= B.m_vertices)
            {
                mapping_Theta[add_to] = 0;
                add_to++;
            }
            else
            {
                add_one = false;
            }
        }
    }    

    if (all_zeros_for_all_Thetas)
    {
        cerr << "All mappings had all coefficients always zero" << endl;
    }
    else
    {
        cerr << "Mappings with only zeros          : " << only_zeros << endl;
        cerr << "Mappings with     zero dot product: " << zero_sums << endl;
        cerr << "Mappings with non-zero dot product: " << nonzero_sums << endl;
    }
}
    
    
int labeled_automorphisms(const flag &G)
{
    flag Gl = G;
    Gl.make_all_vertices_labeled();
    
    int count=0;
    
    int vertex_permutation[] = {0,1,2,3,4,5,7,8,9,10,11,12,13,14,15,16,17,18,19,20}; // probably longer than needed
     
    flag F;
    do {
        F.as_subflag(G,vertex_permutation, G.m_vertices, G.m_vertices);
        
        
        if (F.is_isomorphic_to(Gl))
        {
            count++;
        }
         
     } while ( std::next_permutation(vertex_permutation,vertex_permutation+G.m_vertices));
    
    
    return count;
}


class fraction
{
    public:
    fraction()
    {
        m_numerator = 0;
        m_denominator = 1;
    }
    
    fraction(int i)
    {
        m_numerator = i;
        m_denominator = 1;
    }
    
    fraction(int num, int den)
    {
        m_numerator = num;
        m_denominator = den;
        simplify();
    }

    
    
    double get_double()
    {
        return (double)m_numerator/(double)m_denominator;
    }

    void simplify()
    {
        if (m_numerator == 0)
        {
            m_denominator = 1;
            return;
        }
        
        for (int t = 2; t < m_denominator; t++)
        {
            if (m_numerator%t == 0  && m_denominator%t == 0)
            {
                m_numerator /= t;
                m_denominator /= t;
                t--;
            }
        }
    }
    
    fraction operator*(const fraction& a)
    {
        fraction f;
        f.m_numerator = m_numerator * a.m_numerator;
        f.m_denominator = m_denominator * a.m_denominator;
        f.simplify();
        return f;
    }

    fraction operator+(const fraction& a)
    {
        fraction f;
        f.m_numerator = m_numerator * a.m_denominator + m_denominator*a.m_numerator;
        f.m_denominator = m_denominator * a.m_denominator;
        f.simplify();
        return f;
    }

    fraction operator-(const fraction& a)
    {
        fraction f;
        f.m_numerator = m_numerator * a.m_denominator - m_denominator*a.m_numerator;
        f.m_denominator = m_denominator * a.m_denominator;
        f.simplify();
        return f;
    }

    fraction operator/(const long l)
    {
        fraction f;
        f.m_numerator = m_numerator;
        f.m_denominator = m_denominator * l;
        f.simplify();
        return f;
    }

    fraction operator/(const fraction& a)
    {
        fraction f;
        f.m_numerator = m_numerator * a.m_denominator;
        f.m_denominator = m_denominator * a.m_numerator;
        f.simplify();
        return f;
    }
    
    long long m_numerator;
    long long m_denominator;
};

std::ostream& operator<< (std::ostream& stream, const fraction& f)
{
    stream << f.m_numerator << "/" << f.m_denominator;
    return stream;
}


void jan_to_nice_convert(const string&  filename, bool with_coefficients)
{
    cerr << "Converting flags in Jan Volec format in file " << filename << endl;

#if G_COLORED_EDGES == 2
    
    OPEN_FILE_SMARTLY(istr, filename);

    
    /*
    istream *istr = &cin;
    
    ifstream infile;
    if (filename != "cin")
    {
        infile.open (filename.c_str(), ifstream::in);
        if (!infile.good())
        {
            cerr << "Failed opening file " << filename << endl;
            return;
        }
        istr = &infile;
    }
    */
    
    // In case loabeled vertices are out of order....
    //int remapping_jan[]={2,3,0,1,4,5,6,7,8,9,10};
    int remapping_jan[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    
    while (istr->good())
    {
        flag f;
        int vertices;
        int theta;
        int edges;
        double coefficient = 0;
        string coefficient_str;
        //TODO
        (*istr) >> vertices >> theta >> edges;
        
        if (!istr->good()) break;
        //cerr << vertices << " " << theta << " " << edges << " " << coefficient_str << " ";
  
        
        if (with_coefficients)
        {
            (*istr) >> coefficient_str;
            if (coefficient_str[0] != '[') break;
#ifdef DONT_USE_C11
            coefficient_str.erase( coefficient_str.size() - 1,1);
#else            
            coefficient_str.pop_back();
#endif
            coefficient_str.erase(0,1);
            coefficient = stod(coefficient_str);
        }
         
        f.set_vertices_and_Theta(vertices,theta);
        
        for (int u = 0; u < vertices; u++)
            for (int v = u+1; v < vertices; v++)
                f.color_edge(u,v,1);
        
        for (int e = 0; e < edges; e++)
        {
            string e_str;
            (*istr) >> e_str;
            int u = (int)e_str[0] - 'a';
            int v = (int)e_str[1] - 'a';
            
            u = remapping_jan[u];
            v = remapping_jan[v];
            
            
            f.color_edge(u,v,2);
        }
        
        if (with_coefficients) 
        {
            cout.precision(G_PRECISION);
            cout << coefficient << " ";
        }
        cout << f.print() << endl;
    }
    //jan.close();
#else
    cerr << "Not implemented." << endl;
#endif
}

void find_density_vectors_in_iterated_blow_up_of(const flag &B, int Kn)
{
    cerr << "Finding density vectors in iterated blow-up" << endl;
    
#ifdef G_COLORED_VERTICES
    cerr << "Not sure how to do iterated constructions with colored vertices." << endl;
    return;
#endif

#ifdef G_COLORED_EDGES_BLIND
    cerr << "Not sure how to do iterated constructions with blind colored edges." << endl;
    return;
#endif
    
#ifdef G_ORIENTED_EDGES_BLIND
    cerr << "Not sure how to do deal with oriented_blind edges." << endl;
    return;
#endif

#ifdef G_COLORED_EDGES
    
    vector<double> densities[V];
    vector<double> densities_labeled[V];

    vector<fraction> f_densities[V];
    vector<fraction> f_densities_labeled[V];

    
    // We say that vertex always has density one
    // which makes life nice
    densities[1].push_back(1);
    densities_labeled[1].push_back(1);
    
    f_densities[1].push_back(1);
    f_densities_labeled[1].push_back(1);
    
    int VB = B.m_vertices;
    
    for (int VG = 2; VG <= Kn; VG++)
    {
        cerr << "****************** VG=" << VG << endl;
        double sum_density=0;
        
        for (int v = 0; v < (int)g_unlabeled_flags[VG].size(); v++)
        {
            
            flag G = g_unlabeled_flags[VG][v];
            //cerr << "Testing " << G.print() << endl;
            
            // here we generate all mappings of i
            int mapping[VG];
            for (int t = 0; t < VG; t++) mapping[t] = 0;
            
            double label_d = 0; // density counted so far - labeled
            fraction f_label_d = 0;
            
            int recursive_d_cnt = 0;
            
            // This is unrolled recursion for generating the mapping
            while (true)
            {
                //int all_maps = 0;
                
                // test mapping
                // How many mapped to each class
                vector<int> mapped_to[V];
                for (int t = 0; t < VG; t++)
                    mapped_to[mapping[t]].push_back(t); 


                // check that edges between pieces in different parts are correct
                bool invalid_map = false;
                bool recursive_map = false;
                
                for (int a = 0; a < VG-1; a++)
                    for (int b = a+1; b < VG; b++)
                    {
                        if (mapping[a] == mapping[b])
                            continue;
                        if (G.m_color_edge[a][b] != B.m_color_edge[mapping[a]][mapping[b]])
                        {
                            invalid_map = true;
                        }
                        
                    }
                
                if (!invalid_map)
                {
                    double density_map = 1;
                    fraction f_density_map = 1;
                    
                                
                    // count probability inside each part separately
                    for (int n = 0; n < VB; n++)
                    {
                        // all recursively mapped to the same blob
                        if (mapped_to[n].size() == (unsigned int)VG)
                        {
                            recursive_d_cnt++;
                            recursive_map = true;
                            break;
                        }
                    
                        // If not recursive, count what is the probability of the induced subrgaph in n
                        //TODO some scale will be needed
                        if (mapped_to[n].size() >= 2)
                        {
                            int mapping_G_in_n[V];
                            for (int j = 0; j < (int)mapped_to[n].size(); j++) mapping_G_in_n[j] = mapped_to[n][j];
                            flag G_in_n;
                            G_in_n.as_subflag(G, mapping_G_in_n, mapped_to[n].size(), 0);
                            
                            int small_ID = find_flag_in_list(G_in_n, g_unlabeled_flags[mapped_to[n].size()]);
                            assert(small_ID >= 0);
                        
                            density_map *= densities_labeled[mapped_to[n].size()][small_ID];
                            f_density_map = f_density_map * f_densities_labeled[mapped_to[n].size()][small_ID];
                        }
                    }
                    
                    if (!recursive_map)
                    {
                        label_d += density_map;
                        f_label_d = f_label_d + f_density_map;
                    }

                    //cerr << "_label_d="  << label_d << endl;
                    //cerr << "_f_label_d="  << f_label_d << endl;
                    
                    
                }
                            
                
                /*
                cerr << "Map ";
                for (int t = 0; t < i ; t++) 
                    cerr << mapping[t] << " ";
                cerr << "label_d=" << label_d  << " inv_map="  << invalid_map  <<  " rec_map=" <<  recursive_map  <<  " rec_cnt=" <<  recursive_d_cnt << endl;
                */
                
                // increment mapping
                int inc = 0;
                while (inc < VG && mapping[inc] == VB-1) mapping[inc++] = 0;
                if (inc == VG) break; // we are done
                mapping[inc]++;
            }
            
            // count number of non-isomorphic labelings on G....
            // factorial(i)/labeled_automorphisms(G)
            
            //cerr << "label_d="  << label_d << endl;
            //cerr << "f_label_d="  << f_label_d << endl;
            
            //cerr << "labeled_automorphisms(G)=" << labeled_automorphisms(G) << endl;
            double non_iso_cnt = (double)factorial(VG)/labeled_automorphisms(G);
            fraction f_non_iso_cnt = fraction(factorial(VG),labeled_automorphisms(G));
            //cerr << "non_iso_cnt=" << non_iso_cnt << endl;
            //cerr << "f_non_iso_cnt=" << f_non_iso_cnt << endl;
            
            
            // non-recursive part
            double unlabel_d = non_iso_cnt* label_d/pow((double)VB,(double)VG);
            fraction f_unlabel_d = f_non_iso_cnt*f_label_d/( (long)pow(VB,VG));
            //cerr << "non-recursive: " << unlabel_d << endl;
            //cerr << "non-recursive: " << f_unlabel_d << endl;
            
            // now recursive part
            // d \binom{n}{VG}    =  d*VB*\binom{n/VB}{VG} + C
            // d ( \frac{1}{VG!}(1- VB^(VG-1))              =  c
            //
            double recusive_scale = 1 - 1/pow((double)VB,(double)VG-1);
            fraction f_recusive_scale = fraction(1) -  fraction(1)/((long)pow((double)VB,(double)VG-1));
            //cerr << "scale: " << (recusive_scale) << endl;
            //cerr << "scale: " << (f_recusive_scale) << endl;
            double full_d = unlabel_d / recusive_scale;
            fraction f_full_d = f_unlabel_d / f_recusive_scale;
            
            //cout << std::setw(10) << full_d << "   " << f_full_d  << "     " << G.print() << endl;
            cout << std::setw(10) << full_d << "     " << G.print() << endl;
            //cout << f_full_d << " " << G.print() << endl;

            //cerr << "full_d: " << full_d << endl;
            //cerr << "label_d: " << label_d/pow((double)VB,(double)VG)/recusive_scale << endl;
            
            densities[VG].push_back(full_d);
            densities_labeled[VG].push_back(label_d/pow((double)VB,(double)VG)/recusive_scale);
            f_densities[VG].push_back(f_full_d);
            f_densities_labeled[VG].push_back(f_label_d/((long)pow((double)VB,(double)VG))/f_recusive_scale);
            sum_density += full_d;
        }
        
        cerr << "Density sum=" << sum_density << endl;
    }
    
    cerr << "WARNING: This is experimental feature and may not work 100% correctly." << endl;
#else
    cerr << "Feature not implemented for this kind of flags" << endl;
#endif
}





void extend_flag()
{
    
}

    
    
template<typename T>
void make_patterns(vector<T> &ptr,  vector<vector<T> > &patterns, int length, int maxx, T scale)
{
    if ((int)ptr.size() == length)
    {
        patterns.push_back(ptr);
        return;
    }
    for (int x = 0; x < maxx; x++)
    {
        ptr.push_back(x/scale);
        make_patterns(ptr,patterns,length,maxx,scale);
        ptr.pop_back();
    }
}


template<typename T>
void expand_pattern(vector<T> &ptr, vector<T> &template_ptr, vector<vector<T> > &patterns, int length, int maxx, T scale)
{
    if ((int)ptr.size() == length)
    {
        patterns.push_back(ptr);
        return;
    }
    
    if (template_ptr[ptr.size()] == 0 || template_ptr[ptr.size()] == 1)
    {
        ptr.push_back(template_ptr[ptr.size()]);
        expand_pattern(ptr,template_ptr,patterns,length,maxx,scale);
        ptr.pop_back();
        return;
    }
    
    for (int x = 0; x < maxx; x++)
    {
        ptr.push_back(x/scale);
        expand_pattern(ptr,template_ptr,patterns,length,maxx,scale);
        ptr.pop_back();
    }
}


void fix_part(vector<vector<int> > adjacencies, vector<double> &tmpd, int a, int b, int c, int d, int e, double part)
{
    for (int t = 0; t < (int)adjacencies.size(); t++)
    {
        if (adjacencies[t][0] == a && adjacencies[t][1] == b && adjacencies[t][2] == c && adjacencies[t][3] == d && adjacencies[t][4] == e)
        {
            tmpd[t] = part;
            return;
        }
    }
}


void fix_part(vector<vector<int> > adjacencies, vector<double> &tmpd, int a, int b, int c, int d, double part)
{
    for (int t = 0; t < (int)adjacencies.size(); t++)
    {
        if (adjacencies[t][0] == a && adjacencies[t][1] == b && adjacencies[t][2] == c && adjacencies[t][3] == d)
        {
            tmpd[t] = part;
            return;
        }
    }
}


void fix_part(vector<vector<int> > adjacencies, vector<double> &tmpd, int a, int b, int c,  double part)
{
    for (int t = 0; t < (int)adjacencies.size(); t++)
    {
        if (adjacencies[t][0] == a && adjacencies[t][1] == b && adjacencies[t][2] == c)
        {
            tmpd[t] = part;
            return;
        }
    }
}



void fix_part(vector<vector<int> > adjacencies, vector<double> &tmpd, int a, int b, double part)
{
    for (int t = 0; t < (int)adjacencies.size(); t++)
    {
        if (adjacencies[t][0] == a && adjacencies[t][1] == b)
        {
            tmpd[t] = part;
            return;
        }
    }
}


double get_part(vector<vector<int> > adjacencies, vector<double> &part, int a, int b, int c, int d)
{
    for (int t = 0; t < (int)adjacencies.size(); t++)
    {
        if (adjacencies[t][0] == a && adjacencies[t][1] == b && adjacencies[t][2] == c && adjacencies[t][3] == d)
        {
            return part[t];
        }
    }
    assert(0);
    return -1;
}

double get_part(vector<vector<int> > adjacencies, vector<double> &part, int a, int b, int c)
{
    for (int t = 0; t < (int)adjacencies.size(); t++)
    {
        if (adjacencies[t][0] == a && adjacencies[t][1] == b && adjacencies[t][2] == c)
        {
            return part[t];
        }
    }
    assert(0);
    return -1;
}

bool are_constraints_same()
{
    
    return false;
}


 #ifdef G_CROSSINGS 
void hack_crossing_recursive(flag f, 
    vector<flag> &Flist,
    vector< vector<int> > &mappings_1,
    vector< vector<int> > &mappings_2,
    int id_1,
    int id_2)
{
    if (id_2 >= (int)mappings_2.size())
    {
        id_1++;
        id_2 = 0;
    }

    if (id_1 >= (int)mappings_1.size())
    {
        cerr << "We found one!" << endl;
    }

    // Test if some crossings undefined....
    bool undecided_edge=false;
    for (int u1 : mappings_1[id_1])
        for (int a1 : mappings_1[id_1])
        {
            if (u1==a1) continue;
            for (int u2 : mappings_2[id_2])
                for (int a2 : mappings_2[id_2])
                {
                    if (u2 == a2) continue;

                    if (f.m_cross_edges[u1][u2][a1][a2] == 2)
                    {
                        cerr << "Here is an undecided edge " << u1 << u2 << a1 << a2  << endl;
                        undecided_edge = true;
                    }
                }
        }

    // Here we need to add some missing edges....

}
#endif

void generate_subcombinations(int n, int r, int shift, vector< vector<int> > &result)
{
    std::vector<bool> v(n);
    std::fill(v.begin(), v.begin() + r, true);

    do {
        vector<int>  one_result;
        for (int i = 0; i < n; ++i) {
            if (v[i]) {
                //std::cout << (i) << " ";
                one_result.push_back(i+shift);
            }
        }
        result.push_back(one_result);
        //std::cout << "\n";
    } while (std::prev_permutation(v.begin(), v.end()));
}

void hack_crossings()
{
 #ifdef G_CROSSINGS   

    string filename="density34.txt";
    vector<flag_and_coefficient> F;
    load_flags_and_coefficients_from_file(filename, F);



    vector<flag> Flist;
    for (flag_and_coefficient &fc : F)
    {
        if (fc.coefficient > 0.0000001)
        {
            Flist.push_back(fc.g);
        }
    }


    cerr << "Hello " << Flist.size() << endl;


    vector< vector<int> > mappings_1;
    vector< vector<int> > mappings_2;

    generate_subcombinations(5,3,0,mappings_1); // How to map 1
    generate_subcombinations(5,3,5,mappings_2); // How to map 2

// This is trying to make K_5,5 for K_3,4 in all possible ways and find
// some with low crossing number for a potential counterexample
    for (int i = 0; i < (int)Flist.size(); i++)
        for (int j = 0; j < (int)Flist.size(); j++)
        {
            //    vertices 0,1,2  and 3,4,5,6 from gi
            // to vertices 0,1,2  and 5,6,7,8 in f

            //    vertices 0,1,2  and 3,4,5,6 from gj
            // to vertices 2,3,4  and 5,6,7,8 in f

            flag gi = Flist[i];
            flag gj = Flist[j];

            // compatibility test

            flag f;
            f.set_vertices_and_Theta(10,0);
            f.color_vertex(0,1);
            f.color_vertex(1,1);
            f.color_vertex(2,1);
            f.color_vertex(3,1);
            f.color_vertex(4,1);
            f.color_vertex(5,2);
            f.color_vertex(6,2);
            f.color_vertex(7,2);
            f.color_vertex(8,2);
            f.color_vertex(9,2);
            for (int u1 = 0; u1 < 7; u1++)
                for (int a1 = 0; a1 < 7; a1++)
                    for (int u2 = 0; u2 <= 7; u2++)
                        for (int a2 = 0; a2 <= 7; a2++)
                        {
                            // By 2 we mean it is not defined!!
                            // It will be used to check compatibility later.
                            f.m_cross_edges[u1][u2][a1][a2] = 2;           
                        }

            //    vertices 0,1,2  and 3,4,5,6 from gi
            // to vertices 0,1,2  and 5,6,7,8 in f
            for (int u1 = 0; u1 < 3; u1++)
                for (int a1 = u1+1; a1 < 3; a1++)
                    for (int u2 = 3; u2 <= 6; u2++)
                        for (int a2 = 3; a2 <= 6; a2++)
                        {
                            if (u2==a2) continue;
                            f.set_edges_crossing(u1,u2+2,a1,a2+2, gi.m_cross_edges[u1][u2][a1][a2]);

                            f.set_edges_crossing(u1+2,u2+2,a1+2,a2+2, gj.m_cross_edges[u1][u2][a1][a2]);
                        }

            // Now we need to fill the rest.
            hack_crossing_recursive(f, 
    Flist,
    mappings_1,
    mappings_2,
    0,
    0);
                //cerr << f.print() << endl;
        }

    return;

 // TODO....
 // This is calculating the responsibility in some sense....
    //string filename="density34.txt";
    //vector<flag_and_coefficient> F;
    //load_flags_and_coefficients_from_file(filename, F);

    double all_sum = 0;
    for (flag_and_coefficient& f : F)
    {
        cerr << f.coefficient  << endl;
        int to_check[3][3]={ {0,1,2}, {1,2,0}, {2,1,0}};
        for (int id = 0; id < 3; id++)
        {
            int root = to_check[id][0];
            int u = to_check[id][1];
            int v = to_check[id][2];

            //cerr << " root= " << root << endl;
            //cerr << " u= " << u << endl;
            //cerr << " v= " << v << endl;

            // we should just generate permutations..
            for (int n1 = 3; n1 <= 6; n1++)
            {
                for (int nu = 3; nu <= 6; nu++)
                {
                    if (n1==nu) continue;
                    //cerr << "n1= " << n1 << " nu= " << nu << endl; 
                    if (f.g.m_cross_edges[root][n1][u][nu] == 0) continue;
                    for (int n2 = 3; n2 <= 6; n2++)
                    {
                        if (n1==n2 || nu == n2) continue;
                        int nv = 6+5+4+3 -n1-n2-nu;

                        if (f.g.m_cross_edges[root][n2][v][nv] == 0) continue;

                        // now we have a pair of crossings...
                        all_sum += f.coefficient;
                    }
                }
            }
        }
    }

    cerr << "All out - sum is " << all_sum << endl;
    #endif
}

#ifdef G_4EDGES
bool hack_K34_to_K44_metacrossings_is_allowed(vector<flag> &allowed_flags, flag &F1F2)
{
    for (int skip=0; skip < 8; skip++)
    {
        int map[V];
        int to_map = 0;
        for (int i = 0; i < 7; i++)
        {
            if (i == skip) to_map++;
            map[i] = to_map;
            to_map++;
        }
        flag F;
        F.as_subflag(F1F2, map, 7, 0);
        if (find_flag_in_list_nonparalel(F, allowed_flags) == -1)
        {   
            //cerr << skip << " not found " << F.print() << endl;
            //cerr << " " << map[0] << map[1] << map[2] << map[3] << map[4] << map[5] << map[6] << endl;
            return false;
        }
        else
        {
            //cerr << skip << " found " << F.print() << endl;
            //cerr << " " << map[0] << map[1] << map[2] << map[3] << map[4] << map[5] << map[6] << endl;
        }
    }
    //return false;
    return true;
}

void hack_K34_to_K44_metacrossings_add_edges(vector<flag> &allowed_flags, flag &F1F2, int skip03, int skip47, int u03, int u47)
{
    if (u47 == skip47) u47++;

    if (u47 > 7)
    {
        u47 = 4;
        if (u47 == skip47) u47++;

        u03++;
    }
    if (u03 == skip03) u03++;

    if (u03 > 3)
    {
        //cerr << "candidate: " << F1F2.print() << endl;
        if (hack_K34_to_K44_metacrossings_is_allowed(allowed_flags, F1F2))
        {
            #pragma omp critical
            {            
                cout << F1F2.print() << endl;
            }
        }
        return;
    }

    F1F2.set_4edge(u03,skip03,u47,skip47, 1);
    hack_K34_to_K44_metacrossings_add_edges(allowed_flags, F1F2, skip03, skip47, u03, u47+1);

    F1F2.set_4edge(u03,skip03,u47,skip47, 0);
    hack_K34_to_K44_metacrossings_add_edges(allowed_flags, F1F2, skip03, skip47, u03, u47+1);
}

void hack_K34_to_K44_metacrossings()
{
//#if (G_4EDGES && G_COLORED_VERTICES)
    cerr << "HACK" << endl;

    vector<flag> allowed_flags;
    load_flags_from_file("allowed_34.txt", allowed_flags);
    cerr << "Loaded " << allowed_flags.size() << " allowed flags" << endl; 

    get_unlabeled_flags_of_size(7, false, true, true, false, false);

    #pragma omp parallel for ordered schedule(dynamic)
    for (int i = 0; i < (int)g_unlabeled_flags[7].size(); i++)
//    for (int i = 0; i < 5; i++)
    {
        cerr << i << "/" << g_unlabeled_flags[7].size() << endl;

        for (int j = 0; j < (int)g_unlabeled_flags[7].size(); j++)
        {

            flag F1 = g_unlabeled_flags[7][i];
            //cerr << F1.print() << endl;;

            flag F1F2_base_from_F1;
            flag F1F2;
            F1F2.set_vertices_and_Theta(8,0);
            F1F2.color_vertex(0,1);
            F1F2.color_vertex(1,1);
            F1F2.color_vertex(2,1);
            F1F2.color_vertex(3,1);

            F1F2.color_vertex(4,2);
            F1F2.color_vertex(5,2);
            F1F2.color_vertex(6,2);
            F1F2.color_vertex(7,2);

            // Copy F1 flag F1F2; 
            int map_F1_to_F1F2[V];
            map_F1_to_F1F2[0] = 0;
            map_F1_to_F1F2[1] = 1;
            map_F1_to_F1F2[2] = 2;

            map_F1_to_F1F2[3] = 4;
            map_F1_to_F1F2[4] = 5;
            map_F1_to_F1F2[5] = 6;
            map_F1_to_F1F2[6] = 7;

            for (int x = 0; x < F1.m_vertices-3; x++)
                for (int y = x+1; y < F1.m_vertices-2; y++)
                    for (int z = y+1; z < F1.m_vertices-1; z++)
                        for (int w = z+1; w < F1.m_vertices; w++)
                        {
                            F1F2.set_4edge(map_F1_to_F1F2[x],map_F1_to_F1F2[y],map_F1_to_F1F2[z],map_F1_to_F1F2[w],F1.m_4edge[x][y][z][w]);
                        }

            int skipped_in_map_F1 = 3;

            flag F2 = g_unlabeled_flags[7][j];
            
 
            // Copy F1 flag F1F2; 
            //3,4,5,6 goes to 0,1,2,3
            //0,1,2   goes to 4,5,6,7

            F1F2_base_from_F1 = F1F2;

            int map_F2_to_F1F2[V];
            for (map_F2_to_F1F2[3] = 0; map_F2_to_F1F2[3] < 4; map_F2_to_F1F2[3]++)
                for (map_F2_to_F1F2[4] = 0; map_F2_to_F1F2[4] < 4; map_F2_to_F1F2[4]++)
                {
                    if (map_F2_to_F1F2[3]==map_F2_to_F1F2[4]) continue;
                    for (map_F2_to_F1F2[5] = 0; map_F2_to_F1F2[5] < 4; map_F2_to_F1F2[5]++)
                    {
                        if (map_F2_to_F1F2[5]==map_F2_to_F1F2[4]) continue;
                        if (map_F2_to_F1F2[5]==map_F2_to_F1F2[3]) continue;

                        map_F2_to_F1F2[6] = 0+1+2+3 - map_F2_to_F1F2[3]-map_F2_to_F1F2[4]-map_F2_to_F1F2[5];

                        //0,1,2   goes to 4,5,6,7
                        for (map_F2_to_F1F2[0] = 4; map_F2_to_F1F2[0] < 8; map_F2_to_F1F2[0]++)
                            for (map_F2_to_F1F2[1] = 4; map_F2_to_F1F2[1] < 8; map_F2_to_F1F2[1]++)
                            {
                                if (map_F2_to_F1F2[0]==map_F2_to_F1F2[1]) continue;
                                for (map_F2_to_F1F2[2] = 4; map_F2_to_F1F2[2] < 8; map_F2_to_F1F2[2]++)
                                {
                                    if (map_F2_to_F1F2[2]==map_F2_to_F1F2[0]) continue;
                                    if (map_F2_to_F1F2[2]==map_F2_to_F1F2[1]) continue;


                                    // Now we have map_F2_to_F1F2 as well
                                    int skipped_in_map_F2 = 4+5+6+7 - map_F2_to_F1F2[0] - map_F2_to_F1F2[1] - map_F2_to_F1F2[2];

                                    bool consitent_maps = true;
                                    // Testing consistency on inside maps
                                    for (int x = 0; x < F2.m_vertices-3; x++)
                                    {
                                        if (map_F2_to_F1F2[x] == skipped_in_map_F1 || map_F2_to_F1F2[x] == skipped_in_map_F2) continue;
                                        for (int y = x+1; y < F2.m_vertices-2; y++)
                                        {
                                            if (map_F2_to_F1F2[y] == skipped_in_map_F1 || map_F2_to_F1F2[y] == skipped_in_map_F2) continue;
                                            for (int z = y+1; z < F2.m_vertices-1; z++)
                                            {
                                                if (map_F2_to_F1F2[z] == skipped_in_map_F1 || map_F2_to_F1F2[z] == skipped_in_map_F2) continue;
                                                for (int w = z+1; w < F2.m_vertices; w++)
                                                {
                                                    if (map_F2_to_F1F2[w] == skipped_in_map_F1 || map_F2_to_F1F2[w] == skipped_in_map_F2) continue;

                                                    if (F2.m_4edge[x][y][z][w] != F1F2.m_4edge[map_F2_to_F1F2[x]][map_F2_to_F1F2[y]][map_F2_to_F1F2[z]][map_F2_to_F1F2[w]])
                                                    {
                                                        consitent_maps = false;
                                                        break;
                                                    }
                                                }
                                                if (consitent_maps == false) break;
                                            }
                                            if (consitent_maps == false) break;
                                        }
                                        if (consitent_maps == false) break;
                                    }
                                    if (consitent_maps == false) continue;

                                    //cerr << i << " consistent with " << j << endl;
                                    //cerr << F1.print() << endl;
                                    //cerr << F2.print() << endl;
                                    //cerr << F1F2.print() << endl;
                                    //cerr << "skipped_in_map_F1="<<skipped_in_map_F1<<" skipped_in_map_F2="<<skipped_in_map_F2 << endl;

                                    //continue;

                                    // Adding edges from F2
                                    for (int x = 0; x < F2.m_vertices-3; x++)
                                        for (int y = x+1; y < F2.m_vertices-2; y++)
                                            for (int z = y+1; z < F2.m_vertices-1; z++)
                                                for (int w = z+1; w < F2.m_vertices; w++)
                                                {
                                                    F1F2.set_4edge(map_F2_to_F1F2[x],map_F2_to_F1F2[y],map_F2_to_F1F2[z],map_F2_to_F1F2[w],F2.m_4edge[x][y][z][w]);
                                                }                                    
                                    //cerr << F1F2.print() << endl;
                                    //cerr << map_F1_to_F1F2[0] << map_F1_to_F1F2[1] << map_F1_to_F1F2[2] << map_F1_to_F1F2[3] << map_F1_to_F1F2[4] << map_F1_to_F1F2[5] << map_F1_to_F1F2[6] << endl;
                                    //cerr << map_F2_to_F1F2[0] << map_F2_to_F1F2[1] << map_F2_to_F1F2[2] << map_F2_to_F1F2[3] << map_F2_to_F1F2[4] << map_F2_to_F1F2[5] << map_F2_to_F1F2[6] << endl;
                                    //cerr << endl;

                                    // Now we need to try to add all 4-edges including
                                    // skipped_in_map_F1;
                                    // skipped_in_map_F2;
                                    hack_K34_to_K44_metacrossings_add_edges(allowed_flags, F1F2, skipped_in_map_F1, skipped_in_map_F2, 0, 4);

                                    // clean-up
                                    F1F2 = F1F2_base_from_F1;
                                }
                            }
                    }
                }
//#endif
        }
    }


//#endif 
}



bool hack_K34_to_K35_metacrossings_is_allowed(vector<flag> &allowed_flags, flag &F1F2)
{
    for (int skip=3; skip < 8; skip++)
    {
        int map[V];
        int to_map = 0;
        for (int i = 0; i < 7; i++)
        {
            if (i == skip) to_map++;
            map[i] = to_map;
            to_map++;
        }
        flag F;
        F.as_subflag(F1F2, map, 7, 0);
        if (find_flag_in_list_nonparalel(F, allowed_flags) == -1)
        {   
            //cerr << skip << " not found " << F.print() << endl;
            //cerr << " " << map[0] << map[1] << map[2] << map[3] << map[4] << map[5] << map[6] << endl;
            return false;
        }
        else
        {
            //cerr << skip << " found " << F.print() << endl;
            //cerr << " " << map[0] << map[1] << map[2] << map[3] << map[4] << map[5] << map[6] << endl;
        }
    }
    //return false;
    return true;
}

void hack_K34_to_K35_metacrossings_add_edges(vector<flag> &allowed_flags, flag &F1F2, int skip37a, int skip37b, int u02a, int u02b)
{
    if (u02b > 2)
    {
        u02a++;
        u02b = u02a+1;
    }

    if (u02a > 2 || u02b > 2)
    {
        //cerr << "candidate: " << F1F2.print() << endl;
        if (hack_K34_to_K35_metacrossings_is_allowed(allowed_flags, F1F2))
        {
            #pragma omp critical
            {            
                cout << F1F2.print() << endl;
            }
        }
        return;
    }

    //cerr << "TEstting edge " << skip37a << skip37b << u02a << u02b << endl;
    F1F2.set_4edge(skip37a,skip37b,u02a,u02b, 1);
    hack_K34_to_K35_metacrossings_add_edges(allowed_flags, F1F2, skip37a,skip37b,u02a,u02b+1);

    F1F2.set_4edge(skip37a,skip37b,u02a,u02b, 0);
    hack_K34_to_K35_metacrossings_add_edges(allowed_flags, F1F2, skip37a,skip37b,u02a,u02b+1);
}

void hack_K34_to_K35_metacrossings()
{
//#if (G_4EDGES && G_COLORED_VERTICES)
    cerr << "HACK" << endl;

    vector<flag> allowed_flags;
    load_flags_from_file("allowed_34.txt", allowed_flags);
    cerr << "Loaded " << allowed_flags.size() << " allowed flags" << endl; 

    get_unlabeled_flags_of_size(7, false, true, true, false, false);

    #pragma omp parallel for ordered schedule(dynamic)
    for (int i = 0; i < (int)g_unlabeled_flags[7].size(); i++)
//    for (int i = 0; i < 5; i++)
    {
        cerr << i << "/" << g_unlabeled_flags[7].size() << endl;

        for (int j = 0; j < (int)g_unlabeled_flags[7].size(); j++)
        {

            flag F1 = g_unlabeled_flags[7][i];
            //cerr << F1.print() << endl;;

            flag F1F2_base_from_F1;
            flag F1F2;
            F1F2.set_vertices_and_Theta(8,0);
            F1F2.color_vertex(0,1);
            F1F2.color_vertex(1,1);
            F1F2.color_vertex(2,1);

            F1F2.color_vertex(3,2);
            F1F2.color_vertex(4,2);
            F1F2.color_vertex(5,2);
            F1F2.color_vertex(6,2);
            F1F2.color_vertex(7,2);

            // Copy F1 flag F1F2; 
            int map_F1_to_F1F2[V];
            map_F1_to_F1F2[0] = 0;
            map_F1_to_F1F2[1] = 1;
            map_F1_to_F1F2[2] = 2;

            map_F1_to_F1F2[3] = 3;
            map_F1_to_F1F2[4] = 4;
            map_F1_to_F1F2[5] = 5;
            map_F1_to_F1F2[6] = 6;

            for (int x = 0; x < F1.m_vertices-3; x++)
                for (int y = x+1; y < F1.m_vertices-2; y++)
                    for (int z = y+1; z < F1.m_vertices-1; z++)
                        for (int w = z+1; w < F1.m_vertices; w++)
                        {
                            F1F2.set_4edge(map_F1_to_F1F2[x],map_F1_to_F1F2[y],map_F1_to_F1F2[z],map_F1_to_F1F2[w],F1.m_4edge[x][y][z][w]);
                        }

            int skipped_in_map_F1 = 7;

            flag F2 = g_unlabeled_flags[7][j];
            
 
            // Copy F1 flag F1F2; 
            //0,1,2   goes to 0,1,2
            //3,4,5,6 goes to 3,4,5,6,7

            F1F2_base_from_F1 = F1F2;

            int map_F2_to_F1F2[V];
            for (map_F2_to_F1F2[3] = 3; map_F2_to_F1F2[3] < 8; map_F2_to_F1F2[3]++)
                for (map_F2_to_F1F2[4] = 3; map_F2_to_F1F2[4] < 8; map_F2_to_F1F2[4]++)
                {
                    if (map_F2_to_F1F2[3]==map_F2_to_F1F2[4]) continue;
                    for (map_F2_to_F1F2[5] = 3; map_F2_to_F1F2[5] < 8; map_F2_to_F1F2[5]++)
                    {
                        if (map_F2_to_F1F2[5]==map_F2_to_F1F2[4]) continue;
                        if (map_F2_to_F1F2[5]==map_F2_to_F1F2[3]) continue;

                        for (map_F2_to_F1F2[6] = 3; map_F2_to_F1F2[6] < 8; map_F2_to_F1F2[6]++)
                        {
                            if (map_F2_to_F1F2[6]==map_F2_to_F1F2[4]) continue;
                            if (map_F2_to_F1F2[6]==map_F2_to_F1F2[3]) continue;
                            if (map_F2_to_F1F2[6]==map_F2_to_F1F2[5]) continue;

                            // Now we have map_F2_to_F1F2 as well
                            int skipped_in_map_F2 = 3+4+5+6+7 - map_F2_to_F1F2[3]-map_F2_to_F1F2[4]-map_F2_to_F1F2[5] - map_F2_to_F1F2[6];
                        
                            if (skipped_in_map_F2 == skipped_in_map_F1)
                                continue;
                        
                            //0,1,2   goes to 0,1,2
                            for (map_F2_to_F1F2[0] = 0; map_F2_to_F1F2[0] < 3; map_F2_to_F1F2[0]++)
                                for (map_F2_to_F1F2[1] = 0; map_F2_to_F1F2[1] < 3; map_F2_to_F1F2[1]++)
                                {
                                    if (map_F2_to_F1F2[0]==map_F2_to_F1F2[1]) continue;
                                    map_F2_to_F1F2[2] = 0+1+2 - map_F2_to_F1F2[0]-map_F2_to_F1F2[1];

                                    bool consitent_maps = true;
                                    // Testing consistency on inside maps
                                    for (int x = 0; x < F2.m_vertices-3; x++)
                                    {
                                        if (map_F2_to_F1F2[x] == skipped_in_map_F1 || map_F2_to_F1F2[x] == skipped_in_map_F2) continue;
                                        for (int y = x+1; y < F2.m_vertices-2; y++)
                                        {
                                            if (map_F2_to_F1F2[y] == skipped_in_map_F1 || map_F2_to_F1F2[y] == skipped_in_map_F2) continue;
                                            for (int z = y+1; z < F2.m_vertices-1; z++)
                                            {
                                                if (map_F2_to_F1F2[z] == skipped_in_map_F1 || map_F2_to_F1F2[z] == skipped_in_map_F2) continue;
                                                for (int w = z+1; w < F2.m_vertices; w++)
                                                {
                                                    if (map_F2_to_F1F2[w] == skipped_in_map_F1 || map_F2_to_F1F2[w] == skipped_in_map_F2) continue;

                                                    if (F2.m_4edge[x][y][z][w] != F1F2.m_4edge[map_F2_to_F1F2[x]][map_F2_to_F1F2[y]][map_F2_to_F1F2[z]][map_F2_to_F1F2[w]])
                                                    {
                                                        consitent_maps = false;
                                                        break;
                                                    }
                                                }
                                                if (consitent_maps == false) break;
                                            }
                                            if (consitent_maps == false) break;
                                        }
                                        if (consitent_maps == false) break;
                                    }
                                    if (consitent_maps == false) continue;

                                    //cerr << i << " consistent with " << j << endl;
                                    //cerr << F1.print() << endl;
                                    //cerr << F2.print() << endl;
                                    //cerr << F1F2.print() << endl;
                                    //cerr << "skipped_in_map_F1="<<skipped_in_map_F1<<" skipped_in_map_F2="<<skipped_in_map_F2 << endl;

                                    //continue;

                                    // Adding edges from F2
                                    for (int x = 0; x < F2.m_vertices-3; x++)
                                        for (int y = x+1; y < F2.m_vertices-2; y++)
                                            for (int z = y+1; z < F2.m_vertices-1; z++)
                                                for (int w = z+1; w < F2.m_vertices; w++)
                                                {
                                                    F1F2.set_4edge(map_F2_to_F1F2[x],map_F2_to_F1F2[y],map_F2_to_F1F2[z],map_F2_to_F1F2[w],F2.m_4edge[x][y][z][w]);
                                                }                                    
                                    //cerr << F1F2.print() << endl;
                                    //cerr << map_F1_to_F1F2[0] << map_F1_to_F1F2[1] << map_F1_to_F1F2[2] << map_F1_to_F1F2[3] << map_F1_to_F1F2[4] << map_F1_to_F1F2[5] << map_F1_to_F1F2[6] << endl;
                                    //cerr << map_F2_to_F1F2[0] << map_F2_to_F1F2[1] << map_F2_to_F1F2[2] << map_F2_to_F1F2[3] << map_F2_to_F1F2[4] << map_F2_to_F1F2[5] << map_F2_to_F1F2[6] << endl;
                                    //cerr << endl;

                                    // Now we need to try to add all 4-edges including
                                    // skipped_in_map_F1;
                                    // skipped_in_map_F2;
                                    hack_K34_to_K35_metacrossings_add_edges(allowed_flags, F1F2, skipped_in_map_F1, skipped_in_map_F2, 0, 1);

                                    // clean-up
                                    F1F2 = F1F2_base_from_F1;
                                }
                            
                        }
                    }
                }
//#endif
        }
    }


//#endif 
}
#endif

void hack_bipartite()
{
#ifdef G_COLORED_EDGES
    // HACK!!!!
    flag f;
    f.load_from_string("6 4  1 1 2 0 0  1 2 0 0  1 0 0  0 0   1");
    f.load_from_string("5 3  1 1 0 0  1 0 0  0 0  1");
    f.load_from_string("6 4  1 1 2 0 0  1 1 0 0  1 0 0  0 0  1");
    f.load_from_string("5 3  2 2 0 0  1 0 0  0 0  1");
    f.load_from_string("4 2  1 0 0  0 0  1");
    
    
    // K_4
    
    f.load_from_string("5 3  1 1 0 0  1 0 0  0 0  1");
    // end of hack
    
    
    vector<int> tmp;
    vector<vector<int> > adjacencies;
    vector<vector<double> > patterns;
    vector<double> tmpd;
    vector<double> useless;
    
    bool use_induction = false;
    
    /*
    // K_4 for K_8
    f.load_from_string("6 4  1 1 1 0 0  1 1 0 0  1 0 0  0 0   1");
    make_patterns(tmp, adjacencies, f.m_Theta, 2, 1);
    tmpd.resize(adjacencies.size(), 0.5);
    fix_part(adjacencies, tmpd, 1,1,1,1,  0);
    fix_part(adjacencies, tmpd, 1,1,1,0,  0);
    fix_part(adjacencies, tmpd, 1,1,0,1,  1);
    fix_part(adjacencies, tmpd, 1,0,1,1,  1);
    fix_part(adjacencies, tmpd, 0,1,1,1,  1);
*/

    
    
     // K_2
     f.load_from_string("4 2  1 0 0  0 0  1");
     make_patterns(tmp, adjacencies, f.m_Theta, 2, 1);
     tmpd.resize(adjacencies.size(), 0.5);
     fix_part(adjacencies, tmpd, 1,1,  0);
     fix_part(adjacencies, tmpd, 0,1,  1);
     fix_part(adjacencies, tmpd, 1,0,  1);
   //  use_induction = true;
    
    
    
    /*
    // K_4
    f.load_from_string("6 4  1 1 1 0 0  1 1 0 0  1 0 0  0 0   1");
    make_patterns(tmp, adjacencies, f.m_Theta, 2, 1);
    tmpd.resize(adjacencies.size(), 0.5);
    fix_part(adjacencies, tmpd, 1,1,1,0,  0);
    fix_part(adjacencies, tmpd, 1,1,0,1,  0);
    fix_part(adjacencies, tmpd, 1,0,1,1,  1);
    fix_part(adjacencies, tmpd, 0,1,1,1,  1);
     */
    
    
    /*
    // K_3
    f.load_from_string("5 3  1 1 0 0  1 0 0  0 0  1");
    make_patterns(tmp, adjacencies, f.m_Theta, 2, 1);
    tmpd.resize(adjacencies.size(), 0.5);
    fix_part(adjacencies, tmpd, 1,1,1,  0);
    fix_part(adjacencies, tmpd, 1,1,0,  0);
    fix_part(adjacencies, tmpd, 0,1,1,  1);
    fix_part(adjacencies, tmpd, 1,0,1,  1);
    //use_induction = true;
    */
    

    /*
    // K_4 TAISA 2
    f.load_from_string("6 4  1 1 1 0 0  1 1 0 0  1 0 0  0 0   1");
    make_patterns(tmp, adjacencies, f.m_Theta, 2, 1);
    tmpd.resize(adjacencies.size(), 0.5);
    fix_part(adjacencies, tmpd, 1,1,1,0,  0);
    fix_part(adjacencies, tmpd, 1,1,0,1,  0);
    fix_part(adjacencies, tmpd, 1,0,1,1,  1);
    fix_part(adjacencies, tmpd, 0,1,1,1,  1);
    fix_part(adjacencies, tmpd, 1,0,0,1,  0);
    fix_part(adjacencies, tmpd, 1,0,1,0,  0);
    fix_part(adjacencies, tmpd, 0,1,0,1,  1);
    fix_part(adjacencies, tmpd, 0,1,1,0,  1);
    */
    
    
    
    /*
    // K_4 TAISA 1
    f.load_from_string("6 4  1 1 1 0 0  1 1 0 0  1 0 0  0 0   1");
    make_patterns(tmp, adjacencies, f.m_Theta, 2, 1);
    tmpd.resize(adjacencies.size(), 0.5);
    fix_part(adjacencies, tmpd, 1,1,1,0,  0);
    fix_part(adjacencies, tmpd, 1,1,0,1,  0);
    fix_part(adjacencies, tmpd, 1,0,1,1,  1);
    fix_part(adjacencies, tmpd, 0,1,1,1,  1);
    fix_part(adjacencies, tmpd, 1,0,0,1,  0);
    fix_part(adjacencies, tmpd, 1,0,1,0,  1);
    fix_part(adjacencies, tmpd, 0,1,0,1,  1);
    fix_part(adjacencies, tmpd, 0,1,1,0,  0);
    use_induction = true;
    */
    
    /*
     // Co-cherry - hack...
     f.load_from_string("5 3  1 2 0 0  2 0 0  0 0  1");
     make_patterns(tmp, adjacencies, f.m_Theta, 2, 1);
     tmpd.resize(adjacencies.size(), 0.5);
    //fix_part(adjacencies, tmpd, 1,0,0,  0);
    //fix_part(adjacencies, tmpd, 1,1,0,  0);
     //fix_part(adjacencies, tmpd, 1,1,1,  0);
     //fix_part(adjacencies, tmpd, 1,0,1,  0);
     use_induction = true;
    */

    /*
    // Taisa C_5 1
    f.load_from_string("7 5  1 2 2 1 0 0    1 2 2 0 0  1 2 0 0  1 0 0  0 0  1");
    make_patterns(tmp, adjacencies, f.m_Theta, 2, 1);
    tmpd.resize(adjacencies.size(), 0.5);
    fix_part(adjacencies, tmpd, 0,1,0,0,1,   0);
    fix_part(adjacencies, tmpd, 0,1,0,1,0,  0);
    fix_part(adjacencies, tmpd, 1,0,1,0,0,  1);
    fix_part(adjacencies, tmpd, 0,0,1,0,1,  1);
    fix_part(adjacencies, tmpd, 1,0,0,1,0,  1);
    use_induction = false;
     */
    
    /*
    // Taisa C_5 2
    f.load_from_string("7 5  1 2 2 1 0 0    1 2 2 0 0  1 2 0 0  1 0 0  0 0  1");
    make_patterns(tmp, adjacencies, f.m_Theta, 2, 1);
    tmpd.resize(adjacencies.size(), 0.5);
    fix_part(adjacencies, tmpd, 0,1,0,0,1,   0);
    fix_part(adjacencies, tmpd, 0,1,0,1,0,  0);
    fix_part(adjacencies, tmpd, 1,0,1,0,0,  1);
    fix_part(adjacencies, tmpd, 0,0,1,0,1,  1);
    use_induction = false;
     */


    /*
    // Taisa C_5 1 - invert1
    f.load_from_string("7 5  1 2 2 1 0 0    1 2 2 0 0  1 2 0 0  1 0 0  0 0  1");
    make_patterns(tmp, adjacencies, f.m_Theta, 2, 1);
    tmpd.resize(adjacencies.size(), 0.5);
    fix_part(adjacencies, tmpd, 0,0,0,0,0,   0);
    fix_part(adjacencies, tmpd, 0,1,0,1,0,  0);
    fix_part(adjacencies, tmpd, 1,0,1,0,0,  1);
    fix_part(adjacencies, tmpd, 0,0,1,0,1,  1);
    fix_part(adjacencies, tmpd, 1,0,0,1,0,  1);
    use_induction = false;
    */

    /*
     // Taisa C_5 1 - invert1
     f.load_from_string("7 5  1 2 2 1 0 0    1 2 2 0 0  1 2 0 0  1 0 0  0 0  1");
     make_patterns(tmp, adjacencies, f.m_Theta, 2, 1);
     tmpd.resize(adjacencies.size(), 0.5);
    fix_part(adjacencies, tmpd, 0,1,0,0,0,   0);
    fix_part(adjacencies, tmpd, 0,1,0,0,1,   0);
    fix_part(adjacencies, tmpd, 0,0,0,1,0,   0);
    fix_part(adjacencies, tmpd, 0,1,0,1,0,   0);
    fix_part(adjacencies, tmpd, 0,1,0,0,1,   0);

    fix_part(adjacencies, tmpd, 0,0,1,0,0,  1);
    fix_part(adjacencies, tmpd, 0,0,1,0,1,  1);
    fix_part(adjacencies, tmpd, 1,0,1,0,0,  1);
    fix_part(adjacencies, tmpd, 1,0,0,1,0,  1);
    fix_part(adjacencies, tmpd, 1,0,1,0,0,  1);
    fix_part(adjacencies, tmpd, 0,0,1,0,1,  1);
     use_induction = false;
     */

    //return;
    
    //expand_pattern(useless, tmpd, patterns,(int) adjacencies.size(), 10, 9.0);
    
    patterns.push_back(tmpd);
    
    // pattern for putting vertex v to A
    cerr  << "Got " << patterns.size() << " patterns " << endl;
    int cuts = 0;
    for (int p = 0; p < (int)patterns.size(); p++)
        {
     
           
            // check if patter is lexicographic...
            // I want pattern  1,0,0 <= 0,1,0 <= 0,0,1
            //if (get_part(adjacencies, patterns[p], 0,0,1) < get_part(adjacencies, patterns[p], 0,1,0)) continue;
            //if (get_part(adjacencies, patterns[p], 0,1,0) < get_part(adjacencies, patterns[p], 1,0,0)) continue;
             
/*
            if ((get_part(adjacencies, patterns[p], 0,0,1)  == get_part (adjacencies, patterns[p], 0,1,0)) &&
               (get_part(adjacencies, patterns[p], 0,1,0) == get_part(adjacencies, patterns[p], 1,0,0)) &&
                ((get_part(adjacencies, patterns[p], 0,1,1)  < get_part (adjacencies, patterns[p], 1,1,0)) ||
                (get_part(adjacencies, patterns[p], 1,1,0) < get_part(adjacencies, patterns[p], 0,0,1)))
                ) continue;
*/
            
            vector<flag> found;

            stringstream ss;
            bool induction_used = false;
            
            ss.precision(G_PRECISION);
//            cout << "0 " << -18.0/49.0 << endl;  // for K8
//            cout << "0 " << -1.0/4.0 << endl;  // for K5
//            ss << "0 " << -0.08 << endl;  // for K5
            ss << "0 " << -1 << endl;  // for K5
            for (int ai = 0; ai < (int)adjacencies.size(); ai++)
            {
                // pattern for putting vertex v-1 to A
                for (int aj = ai; aj < (int)adjacencies.size(); aj++)
                {
                    // fix F
                    for (int v = 0; v < f.m_Theta; v++)
                    {
                        f.color_edge(v,f.m_vertices-1, 2-adjacencies[ai][v]);
                        f.color_edge(v,f.m_vertices-2, 2-adjacencies[aj][v]);
                    }
                    
                    if (is_flag_forbidden(f)) continue;
                    if (g_already_in_known_flags(f,found)) continue;
                    found.push_back(f);
                    
                    double i_in_A = patterns[p][ai];
                    double j_in_A = patterns[p][aj];
                    
                    double coeff = 0;
                    
                    //Usual
                    coeff = 4*(i_in_A*j_in_A+(1-i_in_A)*(1-j_in_A));
                    
                    // better partitioning fopr 0.5...
                    if (use_induction && i_in_A == 0.5 && j_in_A == 0.5) coeff = 1.0;

                    // K_4-free for cochery hack!
                    //if (i_in_A == 0 && j_in_A == 0) coeff = 8.0/9.0;

                    
                    if (coeff != 0)
                    {
                            //cout << coeff << " ";
                        if (!is_flag_forbidden(f))
                        {
                            ss.precision(G_PRECISION);
                            ss << coeff << " " << f.print() << endl;
                        }
                        if (coeff == 1.0)
                        {
                             induction_used = true;
                             f.color_edge(f.m_vertices-1,f.m_vertices-2,2);
                            if (!is_flag_forbidden(f))
                            {
                                ss.precision(G_PRECISION);                            
                                ss << coeff << " " << f.print() << endl;
                                f.color_edge(f.m_vertices-1,f.m_vertices-2,1);
                            }
                        }
                    /*
                        for (int x = 0; x < f.m_vertices;x++)
                            for (int y = x+1; y < f.m_vertices;y++)
                            {
                                if (f.m_color_edge[x][y]==1) cout << x << y  ;
                            }
                     */
                    //    cout << endl;
                    }
                    
                }
            }
            if (!use_induction || induction_used)
            {
                cuts++;
                cout << ss.str() << endl;
            }
        }
    cerr << "Found " << cuts << " cuts" << endl;
#endif
}



class one_part
{
        public:
        vector<int> m_adjacencies;  // What are the adjacencies to labeled vertices
        int         m_part;         // What is the ID of the partition
        double      m_fraction;     // What fraction of these vertices is in this part
    };
    
void fix_multi_part(vector<one_part> & partition, int a, int b, int part, double fraction)
{
    one_part p;
    p.m_fraction = fraction;
    p.m_part = part;
    p.m_adjacencies.push_back(a);
    p.m_adjacencies.push_back(b);
    
    partition.push_back(p);
}

    
    void fix_multi_part(vector<one_part> & partition, int a, int b, int c, int d, int part, double fraction)
    {
        one_part p;
        p.m_fraction = fraction;
        p.m_part = part;
        p.m_adjacencies.push_back(a);
        p.m_adjacencies.push_back(b);
        p.m_adjacencies.push_back(c);
        p.m_adjacencies.push_back(d);
        
        partition.push_back(p);
    }

    void fix_multi_part(vector<one_part> & partition, int a, int b, int c, int part, double fraction)
    {
        one_part p;
        p.m_fraction = fraction;
        p.m_part = part;
        p.m_adjacencies.push_back(a);
        p.m_adjacencies.push_back(b);
        p.m_adjacencies.push_back(c);
        
        partition.push_back(p);
    }


bool verify_partition_completteness_recoursive(vector<int> &adjacency_test, vector<one_part> &partition, int m_Theta)
{
    if ((int)adjacency_test.size() == m_Theta)
    {
        double sum_of_split = 0;
        for(one_part p : partition)
        {
            // test if the pattern matches
            bool match = true;
            for (int i = 0; i < m_Theta; i++)
            {
                if (adjacency_test[i] != p.m_adjacencies[i])
                {   
                    match = false;
                    break;
                }
            }
            if (match)
            {
               sum_of_split += p.m_fraction; 
            }
        }

        // Some sum_of_split might be zero - for example in a forbidden subgraph
        if (sum_of_split > 0 && sum_of_split != 1)
        {
            cerr << "Partition does not provide correct split for adjacencies ";
            for (int x : adjacency_test)
                cerr << " " << x;
            cerr << "\nThe density sum is " << sum_of_split << " while 1.0 is expected." << endl;
            return false;
        }

        return true;
    }

    for (int part=1; part <= 2; part++)
    {
        adjacency_test.push_back(part);
        if (!verify_partition_completteness_recoursive(adjacency_test, partition, m_Theta)) return false;
        adjacency_test.pop_back();
    }
    return true;
}

bool verify_partition_completteness(vector<one_part> &partition, int m_Theta)
{
    vector<int> adjacency_test;
    return verify_partition_completteness_recoursive(adjacency_test, partition, m_Theta);
}


#ifdef G_COLORED_EDGES
void hack_cccc()
{
    cerr << "Loading from cin a list of flags, then for each changing it to a proper pattern for cuts" << endl;

    vector<flag> flag_list;
    load_flags_from_file("cin", flag_list);
    for (const flag &f : flag_list)
    {
        flag ftmp; // to make sure 0 are outside of assigned
        ftmp.set_vertices_and_Theta(f.m_vertices, f.m_Theta);
        ftmp = f;
        ftmp.m_Theta = f.m_vertices;
        ftmp.m_vertices += 2;
        ftmp.color_edge(ftmp.m_vertices-2, ftmp.m_vertices-1,2);
        cout << ftmp.print() << endl;
    }
}
#endif  

//void hack_cuts_generator()
void hack(string cutinput = "cutinput.txt")
{
#ifdef G_COLORED_EDGES
    
    cerr << "Using hack" << endl;
    
    // HACK!!!!
    flag f;
        
    
    vector<one_part> partition;

    
    // Loading from a file
    istream *istr = &cin;

    ifstream infile;
    if (true)
    {
        infile.open (cutinput, ifstream::in);
        if (!infile.good())
        {
            cerr << "Failed opening file " << cutinput << endl;
            return;
        }
        else
        {
            cerr << "Using file " << cutinput << endl;
        }
        istr = &infile;
    }

    f.load_from_stream(*istr,-1,-1);

    while (istr->good())
    {
        one_part p;

        for (int i = 0; i < f.m_Theta; i++)
        {
            int adjacency;
            *istr >> adjacency;

            p.m_adjacencies.push_back(adjacency);
        }
        if (!istr->good()) break;         

        *istr >> p.m_part;
        assert(p.m_part >= 0);
        //assert(p.m_part <= 1); // allowing multipartition 
        if (!istr->good()) break;         

        *istr >> p.m_fraction;
        assert (p.m_fraction > 0);
        assert (p.m_fraction <= 1);
        
        partition.push_back(p);
    }

    // Here we should test it the partition is valid!
    //cerr << "Warning it is not tested that the cut is complete or correct!" << endl;
    if (!verify_partition_completteness(partition, f.m_Theta))
    {
        return;
    }
// For bipartite case

/*    
     // K_2
     f.load_from_string("4 2  2 0 0  0 0  2");
     fix_multi_part(partition, 2,2,  0,  1);
     fix_multi_part(partition, 1,2,  0,  1);
     fix_multi_part(partition, 2,1,  0,  1);
     fix_multi_part(partition, 1,1,  1,  1);
*/

    /*
     // K_2
     f.load_from_string("4 2  2 0 0  0 0  2");
     fix_multi_part(partition, 2,2,  0,  1);
     fix_multi_part(partition, 1,2,  0,  1);
     fix_multi_part(partition, 2,1,  0,  0.5);
     fix_multi_part(partition, 1,1,  1,  1);
     fix_multi_part(partition, 2,1,  1,  0.5);
    */

    /*
    // P_3
    f.load_from_string("5 3  2 1 0 0  2 0 0  0 0  2");    
    fix_multi_part(partition, 2,1,1,  0, 1);
    fix_multi_part(partition, 2,2,1,  0, 1);
    fix_multi_part(partition, 2,1,2,  0, 1);
    fix_multi_part(partition, 2,2,2,  0, 1);
    fix_multi_part(partition, 1,2,1,  1, 1);
    fix_multi_part(partition, 1,1,2,  1, 1);
    fix_multi_part(partition, 1,2,2,  1, 1);
    fix_multi_part(partition, 1,1,1,  0, 1);
*/
    
 /*
    // P_3
    f.load_from_string("5 3  2 1 0 0  2 0 0  0 0  2");    
    fix_multi_part(partition, 2,1,1,  0, 1);
    fix_multi_part(partition, 2,2,1,  0, 1);
    fix_multi_part(partition, 2,1,2,  0, 1);
    fix_multi_part(partition, 2,2,2,  0, 1);    
    fix_multi_part(partition, 1,2,1,  1, 1);
    fix_multi_part(partition, 1,1,2,  1, 1);
    fix_multi_part(partition, 1,2,2,  1, 1);
    fix_multi_part(partition, 1,1,1,  1, 1);
*/
    
/*    
    // P_3
    f.load_from_string("5 3  2 2 0 0  1 0 0  0 0  2");    
    fix_multi_part(partition, 2,1,1,  0, 1);
    fix_multi_part(partition, 2,2,1,  0, 1);
    fix_multi_part(partition, 2,1,2,  0, 1);
    fix_multi_part(partition, 2,2,2,  0, 1);        
    fix_multi_part(partition, 1,2,1,  1, 1);
    fix_multi_part(partition, 1,1,2,  1, 1);
    fix_multi_part(partition, 1,2,2,  1, 1);
    fix_multi_part(partition, 1,1,1,  1, 1);
*/
    
/*    
    // P_3
    f.load_from_string("5 3  2 2 0 0  1 0 0  0 0  2");    
    fix_multi_part(partition, 2,1,1,  0, 1);
    fix_multi_part(partition, 2,2,1,  0, 1);
    fix_multi_part(partition, 2,1,2,  0, 1);
    fix_multi_part(partition, 2,2,2,  0, 1);        
    fix_multi_part(partition, 1,2,2,  1, 1);
    fix_multi_part(partition, 1,2,1,  1, 0.5);
    fix_multi_part(partition, 1,1,2,  1, 0.5);
    fix_multi_part(partition, 1,1,1,  1, 0.5);
    fix_multi_part(partition, 1,2,1,  0, 0.5);
    fix_multi_part(partition, 1,1,2,  0, 0.5);
    fix_multi_part(partition, 1,1,1,  0, 0.5);
*/
         
/*
    // P_2 and K1
    f.load_from_string("5 3  2 1 0 0  1 0 0  0 0  2");    
    fix_multi_part(partition, 2,1,1,  0, 1);
    fix_multi_part(partition, 2,2,1,  0, 1);
    fix_multi_part(partition, 2,1,2,  0, 1);
    fix_multi_part(partition, 2,2,2,  0, 1);        
    fix_multi_part(partition, 1,2,2,  1, 1);
    fix_multi_part(partition, 1,2,1,  1, 0.5);
    fix_multi_part(partition, 1,1,2,  1, 0.5);
    fix_multi_part(partition, 1,1,1,  1, 0.5);
    fix_multi_part(partition, 1,2,1,  0, 0.5);
    fix_multi_part(partition, 1,1,2,  0, 0.5);
    fix_multi_part(partition, 1,1,1,  0, 0.5);
*/
    
    /*
    //  Cuts for Ping's K_5
    // K_2
    f.load_from_string("4 2  2 0 0  0 0  2");
//    fix_multi_part(partition, 2,2,  0,  1);
//    fix_multi_part(partition, 1,2,  1,  1);
    
    fix_multi_part(partition, 2,1,  2, 1);
    fix_multi_part(partition, 1,1,  2, 0.5);

    fix_multi_part(partition, 1,2,  3, 1);
    fix_multi_part(partition, 1,1,  3, 0.5);
    */
    


/*    
    // Big cut - original Ping cut
    cerr << "Creating cut for K_5" << endl;
    f.load_from_string("6 4  2 2 2 0 0  2 2 0 0  1 0 0  0 0  2");

    fix_multi_part(partition, 2,1,2,2,  0, 1);
    fix_multi_part(partition, 2,1,2,1,  0, 1);
    fix_multi_part(partition, 2,1,1,2,  0, 1);
    fix_multi_part(partition, 1,1,2,2,  0, 0.5);
    fix_multi_part(partition, 2,1,1,1,  0, 0.5);
    fix_multi_part(partition, 1,1,2,1,  0, 0.5);
    fix_multi_part(partition, 1,1,1,2,  0, 0.5);
    fix_multi_part(partition, 1,1,1,1,  0, 1.0/3.0);

    fix_multi_part(partition, 1,2,2,2,  1, 1);
    fix_multi_part(partition, 1,2,2,1,  1, 1);
    fix_multi_part(partition, 1,2,1,2,  1, 1);
    fix_multi_part(partition, 1,1,2,2,  1, 0.5);
    fix_multi_part(partition, 1,2,1,1,  1, 0.5);
    fix_multi_part(partition, 1,1,2,1,  1, 0.5);
    fix_multi_part(partition, 1,1,1,2,  1, 0.5);
    fix_multi_part(partition, 1,1,1,1,  1, 1.0/3.0);


    fix_multi_part(partition, 2,2,1,1,  2, 1);
    fix_multi_part(partition, 2,1,1,1,  2, 0.5);
    fix_multi_part(partition, 1,2,1,1,  2, 0.5);
    fix_multi_part(partition, 1,1,1,1,  2, 1.0/3.0);

    fix_multi_part(partition, 2,2,2,1,  3, 1);
    fix_multi_part(partition, 2,2,1,2,  3, 1);
    fix_multi_part(partition, 2,2,2,2,  3, 1);
*/

/*      
     // Big cut - Jan's modified cut
     cerr << "Creating cut for K_5" << endl;
     f.load_from_string("6 4  2 2 2 0 0  2 2 0 0  1 0 0  0 0  2");
     
     fix_multi_part(partition, 2,1,2,2,  0, 1);
     fix_multi_part(partition, 2,1,2,1,  0, 1);
     fix_multi_part(partition, 2,1,1,2,  0, 1);
     fix_multi_part(partition, 1,1,2,2,  0, 0.5);
     fix_multi_part(partition, 2,1,1,1,  0, 0.5);
     fix_multi_part(partition, 1,1,2,1,  0, 0.5);
     fix_multi_part(partition, 1,1,1,2,  0, 0.5);
     fix_multi_part(partition, 1,1,1,1,  0, 1.0/6.0);
     
     fix_multi_part(partition, 1,2,2,2,  1, 1);
     fix_multi_part(partition, 1,2,2,1,  1, 1);
     fix_multi_part(partition, 1,2,1,2,  1, 1);
     fix_multi_part(partition, 1,1,2,2,  1, 0.5);
     fix_multi_part(partition, 1,2,1,1,  1, 0.5);
     fix_multi_part(partition, 1,1,2,1,  1, 0.5);
     fix_multi_part(partition, 1,1,1,2,  1, 0.5);
     fix_multi_part(partition, 1,1,1,1,  1, 1.0/6.0);
     
     
     fix_multi_part(partition, 2,2,1,1,  2, 1);
     fix_multi_part(partition, 2,1,1,1,  2, 0.5);
     fix_multi_part(partition, 1,2,1,1,  2, 0.5);
     fix_multi_part(partition, 1,1,1,1,  2, 1.0/6.0);
     
     fix_multi_part(partition, 2,2,2,1,  3, 1);
     fix_multi_part(partition, 2,2,1,2,  3, 1);
     fix_multi_part(partition, 2,2,2,2,  3, 1);
     fix_multi_part(partition, 1,1,1,1,  3, 1.0/2.0);
*/     
    
    /*
     // Big cut
     cerr << "Creating cut for K_4" << endl;
     f.load_from_string("5 3  2 1 0 0  2 0 0  0 0  2");    
    fix_multi_part(partition, 2,1,2,  0, 1);
    fix_multi_part(partition, 2,1,1,  0, 1);
    fix_multi_part(partition, 1,1,2,  0, 1);
    fix_multi_part(partition, 1,1,1,  0, 1);
    
    fix_multi_part(partition, 1,2,1,  1, 1);
    
    fix_multi_part(partition, 2,2,1,  2, 1);
    fix_multi_part(partition, 1,2,2,  2, 1);
    fix_multi_part(partition, 2,2,2,  2, 1);
    */

    
/*
    cerr << "Creating cut for K_3" << endl;
    f.load_from_string("4 2  1 0 0  0 0 2");    
    fix_multi_part(partition, 2,2,  0, 1);
    fix_multi_part(partition, 2,1,  0, 1);
    //fix_multi_part(partition, 1,2,  0, 1); 
    fix_multi_part(partition, 1,2,  1, 0.5);
    
    fix_multi_part(partition, 1,2,  1, 0.5);
    fix_multi_part(partition, 1,1,  1, 1);
*/
    
    
    /*
    cerr << "Creating cut for K_3" << endl;
    f.load_from_string("3 1  0 0 2");    
    fix_multi_part(partition, 2,2,  0, 1);
    fix_multi_part(partition, 2,1,  0, 1);
    fix_multi_part(partition, 1,2,  0, 1);
    
    //fix_multi_part(partition, 1,2,  1, 0.5);
    fix_multi_part(partition, 1,1,  1, 1);
      */
    
    
    for (int ai = 0; ai < (int)partition.size(); ai++)
    {
        for (int aj = ai; aj < (int)partition.size(); aj++)
        {            
            if (partition[ai].m_part == partition[aj].m_part)
            {
                // fix F
                for (int v = 0; v < f.m_Theta; v++)
                {
                    f.color_edge(v, f.m_vertices-1, partition[ai].m_adjacencies[v]);
                    f.color_edge(v, f.m_vertices-2, partition[aj].m_adjacencies[v]);
                }
                
                if (is_flag_forbidden(f)) continue;
                
                double i_in_P = partition[ai].m_fraction;
                double j_in_P = partition[aj].m_fraction;
                                
                
                double coeff = i_in_P * j_in_P;
                cout.precision(G_PRECISION);
                cout << coeff << " " << f.print() << endl;
            }            
        }
    }
        
#endif    
} 
    
    
void hack_ping()
{
#ifdef G_COLORED_EDGES
    for (int k = 0; k < (int)g_linear_constraints.size(); k++)
    {
        cout << "k++;" << endl;
        cout << "for (i=0; i<sLI[k]; i++) {" << endl;
        cout << " linearInequalities[k][i] = ";
        for (int j = 0; j < (int)g_linear_constraints[k].m_entries.size(); j++)
        {
            flag f = g_linear_constraints[k].m_entries[j].g;
            if (j != 0) cout << "+";
            cout << "(" << g_linear_constraints[k].m_entries[j].coefficient << ")*inducedDensity(Flag(" << f.m_vertices << "," << f.m_Theta << ",\"";
            for (int u = 0; u < f.m_vertices; u++)
                for (int v = u+1; v < f.m_vertices; v++)
                    if (f.m_color_edge[u][v] == 1) cout << u << v;
            cout << "\"), typeTriangleFlags[i])" << endl;
        }
        cout << ";}" << endl;
    }
    //hack();
#endif
}


// Adding symmetry constraints....
void hack_cuts()
{
#ifdef G_COLORED_VERTICES    
     // This is a hack!!
     vector <int> color_swap;
     color_swap.push_back(0);
     color_swap.push_back(2);
     color_swap.push_back(1);
     for (int i = 0; i < (int)g_unlabeled_flags[6].size(); i++)
     {
         flag gswap;
         gswap = g_unlabeled_flags[6][i];
         gswap.permute_vertex_colors(color_swap);
     
         linear_constraint lc;
     
         flag_and_coefficient gd;
         gd.coefficient = 1;
         gd.g = g_unlabeled_flags[6][i];
         lc.m_entries.push_back(gd);
     
         gd.coefficient = -1;
         gd.g = gswap;
         lc.m_entries.push_back(gd);
     
         lc.m_constant = 0;
         lc.check_constraint();
          assert(lc.m_checked);
         g_linear_constraints.push_back(lc);
     }
#endif    
}

// Translates rotation systems to crossings
void hack_cross()
{
#ifdef G_ROTATION_SYSTEM
    // Crossing of 
/*    
    v1--u2
     |\/|
     |/\|
    u1--v2

 or
 
   v2--u2
    |\/|
    |/\|
   u1--v1
 
This gives
 
    3--2
    |\/|
    |/\|
    1--4
  
odd even even odd 
 
 4--2
 |\/|
 |/\|
 1--3

 even odd odd even
 
*/
//    o      o     e      e
//    "4 0  1:243  2:143 3:124  4:123"

    int mapping[10];
    
    for (int n = 0; n < V; n++)
        for (int id = 0; id < (int)g_unlabeled_flags[n].size(); id++)
        {
            flag h = g_unlabeled_flags[n][id];
            cout << h.m_vertices << " " << h.m_Theta << "   ";
    
    for (int u1 = 0; u1 < h.m_vertices; u1++)
    {
        for (int u2 = u1+1; u2 < h.m_vertices; u2++)
        {
            for (int v1 = u1+1; v1 < h.m_vertices; v1++)
            {
                for (int v2 = v1+1; v2 < h.m_vertices; v2++)
                {
                    if (u2 == v1 || u2==v2)  continue;
                    
                    mapping[0] = u1;
                    mapping[1] = u2;
                    mapping[2] = v1;
                    mapping[3] = v2;
                    
                    flag f;
                    f.as_subflag(h, mapping, 4, 0);

                    bool r_u1 = f.is_rotation_even(0,  1,2,3); 
                    bool r_u2 = f.is_rotation_even(1,  0,2,3); 
                    bool r_v1 = f.is_rotation_even(2,  0,1,3); 
                    bool r_v2 = f.is_rotation_even(3,  0,1,2); 

                     if ((!r_u1 && r_u2 && r_v1 && !r_v2) || (r_u1 && !r_u2 && !r_v1 && r_v2))
                     {
                         cout << " 1";
                     }
                     else
                     {
                         cout << " 0";
                     }
                }
            }
        }
    }
            cout << endl;  
            
        }
#endif
}


void extra_arguments(int extra, int processed, int argc)
{
    if (processed+extra >= argc)
    {
        cerr << "Expected more arguments..." << endl;
        exit(1);
    }
}




// THis prints extensive help
void help()
{
    cout << "Extensive help for flag.cpp program" << endl
    << "Compile flags: "
    
#ifdef G_COLORED_VERTICES    
    << " G_COLORED_VERTICES=" << G_COLORED_VERTICES 
#endif
    
#ifdef G_COLORED_VERTICES_BLIND    
    << " G_COLORED_VERTICES_BLIND" 
#endif
    
#ifdef G_COLORED_VERTICES_SAMPLED_SEPARATELY_BY_COLORS    
    << " G_COLORED_VERTICES_SAMPLED_SEPARATELY_BY_COLORS" 
#endif

#ifdef G_COLORED_EDGES    
    << "G_COLORED_EDGES=" << G_COLORED_EDGES
#endif
    
#ifdef G_COLORED_EDGES_BLIND    
    << "G_COLORED_EDGES_BLIND"
#endif
    
#ifdef G_ORIENTED_EDGES
    << "G_ORIENTED_EDGES"
#endif
    
#ifdef G_ORIENTED_EDGES_BLIND    
    << " G_ORIENTED_EDGES_BLIND"
#endif

#ifdef G_ORIENTED_EDGES_UNORIENTED_COLORS
    << " G_ORIENTED_EDGES_UNORIENTED_COLORS=" << G_ORIENTED_EDGES_UNORIENTED_COLORS
#endif

#ifdef G_3EDGES    
    << " G_3EDGES"
#endif

#ifdef G_ROOTED_3EDGES    
    << " G_ROOTED_3EDGES"
#endif


#ifdef G_COLORED_3EDGES    
    << " G_COLORED_3EDGES="<< G_COLORED_3EDGES
#endif
    
#ifdef G_COLORED_3EDGES_BLIND    
    << " G_COLORED_3EDGES_BLIND"
#endif
    
#ifdef G_COLORED_3EDGES_PRINT_HUMAN    
    << " G_COLORED_3EDGES_PRINT_HUMAN"
#endif

#ifdef G_4EDGES    
    << " G_4EDGES"
#endif

#ifdef G_ROOTED_4EDGES    
    << " G_ROOTED_4EDGES"
#endif

#ifdef G_MAYBE_ROOTED_KEDGES
    << " G_MAYBE_ROOTED_KEDGES="<<G_MAYBE_ROOTED_KEDGES
#endif    
#ifdef G_ROTATION_SYSTEM    
    << " G_ROTATION_SYSTEM"
#endif

#ifdef G_ROTATION_SYSTEM_REVERSE_BLIND    
    << " G_ROTATION_SYSTEM_REVERSE_BLIND"
#endif

#ifdef G_ORDER_TYPES
    << " G_ORDER_TYPES"
#endif   

#ifdef G_ORDERED_VERTICES    
    << " G_ORDERED_VERTICES"
#endif

#ifdef G_USE_PERMUTATION_GRAPHS    
    << " G_USE_PERMUTATION_GRAPHS"
#endif

#ifdef G_USE_BABER_EQUATIONS    
    << " G_USE_BABER_EQUATIONS"
#endif

#ifdef G_TOURNAMENT    
    << " G_TOURNAMENT"
#endif
    
#ifdef G_USE_FIRST_EDGE_COLOR_FOR_BLOWUP_ONLY    
    << " G_USE_FIRST_EDGE_COLOR_FOR_BLOWUP_ONLY"
#endif
    
#ifdef USE_FOR_CROSSINGS    
    << " USE_FOR_CROSSINGS"
#endif

#ifdef G_CROSSINGS    
    << " G_CROSSINGS"
#endif

#ifdef G_CUTTINGS    
    << " G_CUTTINGS"
#endif

#ifdef G_LEFTRIGHT    
    << " G_LEFTRIGHT"
#endif
#ifdef G_LEFTRIGHT_BLIND
    << " G_LEFTRIGHT_BLIND"
#endif
    
    << endl
    << "File format:" << endl
    << "n         Number of vertices, integer >= 0" << endl
    
#ifndef G_ORDERED_VERTICES    
    << "Theta     Number of lableled vertices. First Theta vertices are\n"
    << "          considered labeled\n"
#else
    << "Theta     Bit mask of which vertices are labeled. Since the vertices are\n"
    << "          ordered, we need to 'pick' which are labeled\n"
#endif    

    
#ifdef G_COLORED_VERTICES		
    << "vtx_cols  Colors of vertices. It is a vector of colors for vertices. For three vertices may\n"
    << "          look like   2 1 4    saying the colors are 2,1,4. Colors start from color 1.\n"
#endif		
    
#ifdef G_COLORED_EDGES
    << "edges     Colors of edges. Listing by top right of adjacency matrix that contains color entries.\n"
    << "          For example C_5 in colors 2 and 1 would look like  2 1 1 2   1 1 2   1 2   2.\n"
    << "          Color 0 can be used instead of 'any' color. Good when asking for extensions. Use caution when\n"
    << "          actually using it in some program. It might not work right.\n"    
#ifdef G_ORIENTED_EDGES
    << "          If edges are oriented in the opposite direction, use negative number. So directec C_5\n"
    << "          would look like  2 1 1 -2   1 1 2   1 2   2\n"
    << "          Note that colors up to " << G_ORIENTED_EDGES_UNORIENTED_COLORS << " are not oriented\n"
#endif    
#endif    
    
#ifdef G_CROSSINGS
    << "crossings matrix 0/1 of crossings\n"
#endif
    
    
#ifdef G_CUTTINGS
    << "cutting   TODO description :-( Useful for crossing numer\n"
#endif
    
#ifdef G_LEFTRIGHT
    << "left_right   TODO description :-( Useful for crossing numer\n"
#endif

#ifdef G_ORDER_TYPES
    << "order types    For every vertex remembers [x,y] coordinates. It is NOT used for isomorphism or\n"
    << "               anything else than drawing. The order type information is stored using left_right\n"
#endif  
    
#ifdef G_3EDGES
    << "3-edges_cnt   Number of 3-edges in a graph. (Not colored!)"
    << "3-edges       3-sets that count vertices starting by 1. The 3-edges_cnt may be negative and then\n"
    << "              it is assumed that missing 3-edges are enumerated rather than the present ones.\n"    
#endif   

#ifdef G_ROOTED_3EDGES
    << "rooted_3-edges_cnt   Number of rooted 3-edges in a graph. (Not colored!)"
    << "rooted_3-edges       3-sets that count vertices starting by 1. \n"
    << "                     Rooted vertex is always the first one..\n"

#endif      

#ifdef G_COLORED_3EDGES
    // Format for 3 edges: 1) adjacency matrix, 2)  xyz.c
    << "3-edges_cnt   Number of colored 3-edges. If == -1, what follows is adjacency matrix.\n"
    << "matrix?       If 3-edges_cnt == -1, then adjacency matrix is printed with printing only colors.\n"
    << "              This is usefull for storing the colored 3-graphs generated by computer.\n"
    << "3-edges_col   3-edges with color if 3-edges_cnt >= 0. If edge x,y,z has color c, then the entry\n"
    << "              looks like xyz.c  Good for writing by a human.\n"
#endif       
    
#ifdef G_4EDGES
    << "4-edges_cnt   Number of 4-edges in a graph."
    << "4-edges       4-sets that count vertices starting by 1. The 4-edges_cnt may be negative and then\n"
    << "              it is assumed that missing 4-edges are enumerated rather than the present ones.\n"
#endif

    
#ifdef G_ROOTED_4EDGES
    << "rooted_4-edges_cnt   Number of rooted 4-edges in a graph."
    << "rooted_4-edges       4-sets that count vertices starting by 1.\n"
    << "                     Rooted vertex is always the first one..\n"
#endif

#ifdef G_MAYBE_ROOTED_KEDGES
    << "rooted_k-edges_cnt   Number of rooted k-edges in a graph."
    << "rooted_k-edges       k-sets that count vertices starting by 1.\n"
    << "                     Rooted vertex at the end if at all.\n"
#endif


#ifdef G_ROTATION_SYSTEM
    << "Rotation systems Contains entries  v:abc   which means vertex v has neighbors in clockwise\n"
    << "                 order abc. The number of type v:abc must be equal to the number of vertices n\n"
    << "                 Current implementation loads any v:abc even if abc does not contain all other\n"
    << "                 vertices but isomorphism tesing will fail. Vertices start by 1.\n"
#endif    
    
   
    ;
    
#ifdef USE_FOR_CROSSINGS    
    cerr << " Note that the objective function is using the number of crossings instead of whatever you wrote\n"
         << " in the file for objective function :-) Constraints are used.\n"
         << " There are also some tweaks in generation of labeled flags in case you are trying to run biparteite or so.\n";
#endif    
}



// Converting dat-s file to mosek input
// See http://cblib.zib.de/doc/format3.pdf for format documentation

// In our UPPER BOUND application
//
//      min    t   
//      s.t.  D_i + [A.X]_i <= t    
//
// We add a slack variable s_i to make the <= to = and move D_i (density - constant to the other side)
// and change to maximization problem in dat-s file
//
//      max   -t
//      s.t.  [A.X]_i + s_i - t = -D_i
//
// To MOSEK we rewrite both as
//      min   t
//      s.t.  [A.X]_i  - t + D_i <= 0 


// Now LOWER BOUND application
//
//      max    t
//      s.t.  D_i - [A.X]_i  >= t
//
// We add a slack variable s_i to make the >= to = and move D_i (density - constant to the other side)
// and change to maximization:
//
//      max   t
//      s.t.  [A.X]_i + s_i + t = D_i
//
// To MOSEK we rewrite both as
//      min   t
//      s.t.  [A.X]_i  - t - D_i <= 0 


// The same rewrite works for both upper & lower bound.


bool dats_to_cbf(const string filenamedats, string &outfile_str)
{
    outfile_str = filenamedats+".cbf";

    ifstream infile;
    infile.open (filenamedats.c_str(), ifstream::in);
    if (!infile.good())
    {
        cerr << "Failed opening file for reading " << filenamedats << endl;
        return false;
    }

    ofstream outfile;
    outfile.open (outfile_str.c_str(), ofstream::out);
    if (!outfile.good())
    {
        cerr << "Failed opening file for writing " << outfile_str << endl;
        return false;
    }


    int num_constraints;
    infile >> num_constraints;

    int num_blocks;
    infile >> num_blocks;

    if (num_blocks <= 0 || num_constraints <= 0)
    {
        cerr << "Loading " << filenamedats << endl;
        cerr << "num_constraints = " << num_constraints << endl;
        cerr << "num_blocks = " << num_blocks << endl;
        cerr << "That is suspicious, both should be positive. Maybe it is not dat-s file" << endl;
        return false;
    }


    vector<int> block_sizes;
    for (int i = 0; i < num_blocks; i++)
    {
        int block_size;
        infile >> block_size;
        block_sizes.push_back(block_size);
    }

    outfile << "#Automatic rewrite of " << filenamedats  << endl;
    outfile <<  "VER\n3\n\nOBJSENSE\nMIN\n\n" << endl;

    vector<int> psdvar;
    vector<int>  blocks_to_psdvar; // for each block in dat-s claims which block in psd it is
    vector<vector<int> >  blocks_to_var;
    int linear_vars_cnt=1; // the first one is the objective function
    int psd_blocks_cnt=0;  // first starts with 0

    for (int i = 0; i < num_blocks; i++)
    {
        if (block_sizes[i] > 0)
        {
            psdvar.push_back(i);
            vector<int> emptyvec;
            blocks_to_var.push_back(emptyvec);
            blocks_to_psdvar.push_back(psd_blocks_cnt);
            psd_blocks_cnt++;
        }
        if (block_sizes[i] < 0)
        {
            blocks_to_psdvar.push_back(-1);
            // These are slack variables
            if (block_sizes[i] == -num_constraints-1 || block_sizes[i] == -num_constraints-2)
            {
                vector<int> emptyvec;
                blocks_to_var.push_back(emptyvec);
                continue;
            }
            vector<int> blockvec;
            for (int j = 0; j < -block_sizes[i]; j++)
            {
                blockvec.push_back(linear_vars_cnt++);   
            }
            blocks_to_var.push_back(blockvec);
        }
    }

    // Defining PSD variables
    outfile << "PSDVAR\n" << psdvar.size() << endl;
    for (int i : psdvar)
         outfile << block_sizes[i] << endl;
    outfile << endl;

    if (linear_vars_cnt == 1)
    {
        // Here we have only the objective variable
        outfile << "VAR\n 1 1\nF 1\n\n";
    }
    else
    {
        // Here are some other product variables
        outfile << "VAR\n" << linear_vars_cnt << " 2\n";
        outfile << "F 1\n";
        outfile << "L+ " << (linear_vars_cnt-1) << "\n\n";
    }

    // Number of constraints
    outfile << "CON\n";
    outfile << num_constraints << " 1\n";
    outfile << "L- " << num_constraints << "\n\n";

    // Objective function
    outfile << "OBJACOORD\n1\n0 1\n\n";

    // Now all constant parts
    outfile << "BCOORD\n";
    outfile << num_constraints << "\n";
    for (int i = 0; i < num_constraints; i++)
    {
        double constcoeff;
        infile >> constcoeff;
        outfile.precision(G_PRECISION);
        outfile << i << " " << smart_round(-constcoeff) << endl;
    }
    outfile << endl;


    // Linear variables for the objective function.
    int acoordentires = 0;
    ofstream acoordtmp; 
    string acoordtmp_filename = "";
    if (linear_vars_cnt == 1)
    {
        outfile << "ACOORD\n";
        outfile << num_constraints << "\n";
        for (int i = 0; i < num_constraints; i++)
            outfile  << i << " 0 -1\n";
        outfile << "\n";
    }
    else
    {
        acoordtmp_filename = outfile_str+".ACOORD";
        acoordtmp.open(acoordtmp_filename.c_str(), ofstream::out);
        if (!acoordtmp.good())
        {
            cerr << "Failed opening file for writing " << acoordtmp_filename << endl;
            return false;
        }
        for (int i = 0; i <  num_constraints; i++)
        {
            acoordtmp << i << " 0 -1\n";
            acoordentires += 1;
        }
    }


    int fcoordentires = 0;
    string fcoordtmp_filename = outfile_str+".FCOORD";
    ofstream fcoordtmp; 
    fcoordtmp.open(fcoordtmp_filename.c_str(), ofstream::out);
    if (!fcoordtmp.good())
    {
        cerr << "Failed opening file for writing " << fcoordtmp_filename << endl;
        return false;
    }

    while (infile.good())
    {
        int constraint, block_num, row, column;
        double value;
        infile >> constraint >> block_num >> row >> column >> value;

        if (block_num < 1 || row < 1 || column < 1)
        {
            cerr << "Invalid line read:" << constraint << " " << block_num << " " << row << " " << column << " " << value << endl;
            assert(0);
        }

        // Note block_num, row, column are all >= 1
        if (infile.good() == false || column == 0)
        {
            break;
        }
        // fcoord
        if (block_sizes[block_num - 1] > 0)
        {
            // Notice we switch row & column since the entry should be lower triangular matrix
            fcoordtmp.precision(G_PRECISION);
            fcoordtmp << constraint-1 << " " << blocks_to_psdvar[block_num-1] << " " << column-1 <<  " " << row-1 << " " << smart_round(value) << endl;
            fcoordentires += 1;
        }
        // acoord
        else
        {
            // skip slack variables
            if (block_sizes[block_num - 1] != -num_constraints-1 && block_sizes[block_num - 1] != -num_constraints-2)
            {
                acoordtmp.precision(G_PRECISION);
                acoordtmp <<  constraint-1 << " " <<  blocks_to_var[block_num-1][column-1] << " " <<  smart_round(value) << endl;
                acoordentires += 1;
            }
        }
    }

    fcoordtmp.close();
    if (acoordentires != 0)
        acoordtmp.close();

    if (acoordentires != 0)
    {
        outfile << "ACOORD\n";
        outfile << acoordentires << "\n";

        ifstream tmp_a(acoordtmp_filename.c_str(), ifstream::in);
        if (!tmp_a.good())
        {
            cerr << "Failed opening file for reading " << acoordtmp_filename << endl;
            return false;
        }

        outfile << tmp_a.rdbuf();
        tmp_a.close();
        if (remove(acoordtmp_filename.c_str()) != 0)
        {
            cerr << "Failed deleting temp file " << acoordtmp_filename << endl;
        }
    }


    outfile << "FCOORD\n";
    outfile << fcoordentires << "\n";

    ifstream tmp_f(fcoordtmp_filename.c_str(), ifstream::in);
    if (!tmp_f.good())
    {
        cerr << "Failed opening file for reading " << fcoordtmp_filename << endl;
        return false;
    }

    outfile << tmp_f.rdbuf();
    tmp_f.close();
    if (remove(fcoordtmp_filename.c_str()) != 0)
    {
        cerr << "Failed deleting temp file " << fcoordtmp_filename << endl;
    }   

    outfile.close();
    infile.close();

    return true;
}



void expand_linear_constraints_by_products(int Kn, int verbose_output, string dump_to_tmp_prefix="")
{
    // g_flag_product_linear_constraints[ constraint ] is a vector of flags that can be used to multiply  [constraint]
    //static vector<vector<flag> >  g_flag_product_linear_constraints;
    
    // First try to find all types
    vector<flag> types_used;
    for (int j = 0; j < (int)g_linear_constraints.size(); j++)
    {
        add_g_to_flags_list_if_new(g_linear_constraints[j].m_type, types_used);
    }
    cerr << "Using " << types_used.size() << endl;

    vector<flag> types_not_generated;
    for (const auto &f : types_used)
    {
        // try loading, if fail, add to list for generation
        if (labeled_flags_of_one_already_exist(Kn, f) == false)
        {
            types_not_generated.push_back(f);
            cerr << "Will generate  labeled flags of size " << Kn << " of type " << f.print() << endl;
        }
    }

    #pragma omp parallel for ordered schedule(dynamic)
    for (int j = 0; j < (int)types_not_generated.size(); j++)
    {
        // Generate the files here!!!!
        vector<flag> flag_list;
        generate_labeled_flags_of_one_type(Kn, types_not_generated[j], flag_list);

        string filename = get_filename_for_labeled_flags(Kn,types_not_generated[j]);
        #pragma omp critical
        {
            g_labeled_flags_of_one_type_map.insert(make_pair(filename, flag_list));
        }
    }

    vector<vector<linear_constraint> > linear_constraints_expanded;

    linear_constraints_expanded.resize(g_linear_constraints.size());

    int removed_duplicate_constraints = 0;

    mini_timer mt;

    #pragma omp parallel for ordered schedule(dynamic)
    for (int j = 0; j < (int)g_linear_constraints.size(); j++)
    {
                        
        // Size of the other flags in multiplication
        int type_size = g_linear_constraints[j].m_labeled_vertices_in_type_cnt;         
        int flag_size = (Kn - g_linear_constraints[j].m_entries_max_size) + type_size;

#ifdef G_COLORED_VERTICES
        if (flag_size < 1)
#else
        if (flag_size < 2)   // multiplying by just 1 vertex does not do much (unless the vertex has a color)
#endif
        {
            cerr << "Size of big graphs is too small to use products linear constraint " << j+1 << endl;
            continue;
        }
        
                
        // getting labeled flags of the right size

        vector<flag> constraint_multiplied_by;
        flag type;
        type = g_linear_constraints[j].m_type;
        get_labeled_flags_of_one_type(flag_size, type, constraint_multiplied_by);
        
        if (constraint_multiplied_by.size() == 0)
        {
            cerr << "Linear constraint " << j+1 << " cannot be used since generating flags of type " <<  type.print() << " and size " <<  flag_size  << " gave zero flags" << endl;
            continue;
        }
        
        if (verbose_output)
        {
            #pragma omp critical(cerr)
            {
                cerr << "Constraints " << j << "/" << g_linear_constraints.size() << " can be multiplied by " <<  constraint_multiplied_by.size()  << " other flags "
                     << mt.report(j, g_linear_constraints.size()) << endl;
            }
        }

        //constraints_possible++;

//        #pragma omp parallel for ordered schedule(dynamic)
        for (int z = 0; z < (int)constraint_multiplied_by.size(); z++)
        {
            flag f = constraint_multiplied_by[z];

            //g_linear_constraints[j].m_entries;

            linear_constraint new_lc_f_i;
            //check_constraint()

            vector<flag_and_coefficient> product_labeled;

            for (int i = 0; i < (int)g_linear_constraints[j].m_entries.size(); i++)
            {
                    //cerr << i << " " << j << endl;
                    flag type = g_linear_constraints[j].m_type;
                    vector<flag_and_coefficient> F1F2 = F1_times_F2(g_linear_constraints[j].m_entries[i].g, f, type);
                                    
                    multiply_FC_by_C(F1F2, g_linear_constraints[j].m_entries[i].coefficient);
                    
                    fc_add_FC_to_first(product_labeled,F1F2);
            }

            // Unlabeling fc
            //cerr << "Unlabeling " << endl;

            for (int i = 0; i < (int)product_labeled.size(); i++)
            {
                flag_and_coefficient fc;
                
                fc.g = product_labeled[i].g;
                fc.g.m_Theta = 0;
                fc.coefficient = P_F1_IN_H(product_labeled[i].g, fc.g)*product_labeled[i].coefficient;
                
                vector<flag_and_coefficient> tmp;
                tmp.push_back(fc);
                
                fc_add_FC_to_first(new_lc_f_i.m_entries, tmp);
            }

            // Sometimes linear constraints may contain same entry several times
            // so we just sum these all up
            simplify_FC(new_lc_f_i.m_entries);

            if (!new_lc_f_i.check_constraint())
            {
                cerr << "Something went wrong!" << endl;
                exit(1);
            }

    #ifdef G_USE_INDEXES_FOR_LARGE_UNLABELED_FLAGS
            for (auto& fc: new_lc_f_i.m_entries)
            {
                if (fc.g.m_Theta == 0 && fc.g.m_vertices == Kn && fc.g.m_unlabeled_id == -1)
                    fc.g.m_unlabeled_id = find_flag_in_list(fc.g, g_unlabeled_flags[Kn]);
            }
     #endif       


            // Each has a private one...
            //#pragma omp critical
            {
                //linear_constraints_expanded[j].push_back(new_lc_f_i);
            }
             if (!add_linear_constraint_if_new(new_lc_f_i, false,linear_constraints_expanded[j]))
             {
                #pragma omp atomic
                    removed_duplicate_constraints++;
             }
        }
    }


//
//    
//

    cerr << "Merging generated constraints" << endl;
    //vector<linear_constraint> > linear_constraints_reduced;
    int provided_constraints = g_linear_constraints.size();
    g_linear_constraints.clear();
    int generated_constraints = 0;
    for (int j = 0; j < (int)linear_constraints_expanded.size(); j++)
    {
        generated_constraints += linear_constraints_expanded[j].size();
        merge_vectors_parallel(g_linear_constraints, linear_constraints_expanded[j]);
    }

    removed_duplicate_constraints += generated_constraints - g_linear_constraints.size();

    cerr << "Got " << generated_constraints << " expanded constraints out of " << provided_constraints << " provided ones and after removing duplicates " << g_linear_constraints.size() << endl;

    remove_constraints_implied_by_others(verbose_output);


    /*
    // The following may take awfully long! so there is a parallel version

    // The constraint has no type at this moment, so does not matter
    g_linear_constraints.clear();
    for (auto& lc : linear_constraints_expanded)
    {
        if (!add_linear_constraint_if_new(lc, false))
        {
            removed_duplicate_constraints++;
        }
    }
    */

    if (removed_duplicate_constraints > 0)
    {
        cerr << "Removed " << removed_duplicate_constraints << " duplicate constriants." << endl;
    }

    cerr << "New lc " <<  g_linear_constraints.size() << endl;
}


/*
// This should work on OSX only 
#include <mach/mach_init.h>
#include <mach/task.h>
int getmem (unsigned int *rss, unsigned int *vs)
{
    task_t task = MACH_PORT_NULL;
    struct task_basic_info t_info;
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

    if (KERN_SUCCESS != task_info(mach_task_self(),
       TASK_BASIC_INFO, (task_info_t)&t_info, &t_info_count))
    {
        return -1;
    }
    *rss = t_info.resident_size;
    *vs  = t_info.virtual_size;
    return 0;
}
*/

#include <sys/time.h>
#include <sys/resource.h>
// return memory in Kbytes
long getMemoryUsage() 
{
  struct rusage usage;
  if(0 == getrusage(RUSAGE_SELF, &usage))
#ifdef __APPLE__    
    return usage.ru_maxrss/1024; // bytes on OSX, Kbytes on Linux
#else
    return usage.ru_maxrss; // bytes on OSX, Kbytes on Linux
#endif
  else
    return 0;
}


string getMemoryUsageStr()
{
    long used_K = getMemoryUsage();
    long used_M = used_K / 1024 ;
    long used_G = used_K / 1024 / 1024;

    stringstream ss;

    if (used_G > 0)
    {
        ss << used_G << "G";
    }
    else if (used_M > 0)
    {
        ss << used_M << "M";
    }
    else
    {
        ss << used_K << "K";
    }
    return ss.str();
}

string g_log_string;
bool g_log_disabled = false;
time_t g_log_start_timestamp=0;
string g_log_filename = "flag.log";
int g_log_min_time_taken = 3600; // Minimum number of seconds to run to generate a log


void write_log()
{
    if (g_log_disabled)
    {
        return;
    }

    time_t time_taken = time(NULL) - g_log_start_timestamp;
    if (g_log_start_timestamp == 0)
    {
        time_taken = 0;
    }

    char hostname[512];
    char username[512];
    gethostname(hostname, 512);
    getlogin_r(username, 512);

    // Declaring argument for time() 
    time_t tt; 
  
    // Declaring variable to store return value of 
    // localtime() 
    struct tm * ti; 
  
    // Applying time() 
    time (&tt); 
  
    // Using localtime() 
    ti = localtime(&tt); 

    string current_time_str = asctime(ti);
    current_time_str.pop_back();

    stringstream ss;
    ss << current_time_str << " " << username << "@" << hostname << " " << g_log_string << " time taken " 
          << time_to_str(time_taken) <<  " Memory=" <<  getMemoryUsageStr() <<  endl; 

    cerr << ss.str();

    // More than 1 hour of time
    if (time_taken > g_log_min_time_taken)
    {
        string logfile=getenv("HOME");
        logfile +=  "/" + g_log_filename;

        std::ofstream outfile;
        outfile.open(logfile, std::ios_base::app | std::ios::out);
        if (outfile.is_open())
        {
            outfile << ss.str(); 
            cerr << "Written to a log file " << logfile << endl;
        }
        else
        {
            cerr << "Error opening log file '" << logfile << "':" <<  strerror(errno) << endl;
        }
    }
}

void init_log(int argc, char *argv[])
{
    g_log_start_timestamp = time(NULL);

    stringstream ss;

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        ss << cwd;
    }
    else
    {
        ss << "UNKNOWN_DIRECTORY";
    }

    for (int i  = 0; i < argc; i++)
    {
        string tmp(argv[i]);
        if (tmp.find_first_of("\t\n ") == string::npos)
            ss << " " << argv[i] << " ";
        else
            ss << " \'" << argv[i] << "\' ";
    }
    
    g_log_string = ss.str();
}

#ifdef G_EXTRNAL_HACK_CPP
#include "external_hack.cpp"
//void external_hack();
#endif

#ifndef G_DISABLE_MAIN
int main(int argc, char *argv[])
{
/*
    flag g1("5 0    1 2345>4");
    flag g2("5 0    1 2345");

    cerr << "Iso test:" << g1.is_isomorphic_to_not_colorblind<true>(g2) << endl;

    //return 0;

    vector<flag> flag_list;
    flag g;
    g.set_vertices_and_Theta(5,0);

    vector<int> current_edge;
    for (int i = 0; i < G_MAYBE_ROOTED_KEDGES-1; i++)
    {
        current_edge.push_back(G_MAYBE_ROOTED_KEDGES-2-i);
    }
    current_edge.push_back(g.m_vertices-1);
    try_add_maybe_rooted_kedges(g,current_edge,flag_list, true);

    return 0;
*/
    init_log(argc, argv);
    atexit(write_log);

    // Parse command line
    int Kn = 0;
    string objective_file = "";
    string objective_file_divisor = "";
    string output_file = "";
#ifdef G_DEFAULT_EXTENDED_OBJECTIVE    
    bool extended_objective_format = true;
#else
    bool extended_objective_format = false;
#endif    
    int verbose_output = 0;
    bool use_sdp_temp = false;
    bool extension = false;
    bool extension_count_copies = false;
    string extensions_str = "";
    //flag g_extensions;

    bool forbidden_test = false;
    flag g_forbidden_test;    

    bool use_csdp = true;
    bool use_sdpa = false;
#ifdef G_PREFER_MOSEK
    bool use_mosek = true;
#else
    bool use_mosek = false;
#endif

    string csdp_binary="csdp";
    string sdpa_binary="sdpa";
    string mosek_binary="mosek";

    bool convert_dats_to_cbf = false;
    string convert_dats_to_cbf_file = "";

    bool extensions_fc = false;
    string extensions_fc_file = "";

    bool dump_unlabeled = false;
    bool dump_unlabeled_while_generating = false;
    bool generate_small_unlabeled_from_large = false;
    bool quit_after_generating_unlabeled = false;
    bool quit_after_generating_labeled = false;
    bool force_generate_flags = false;
    bool force_generate_labeled_flags = false;
    bool remove_duplicates_while_loading = false;
    bool remove_forbidden_wile_loading=false;
    bool types_from_file = false;
    string labeled_flags_file = "";

    bool dump_types_in_order = false;

    
    bool upper_bound = false;
    bool lower_bound = false;
    bool force_generating_constriants = false;
    
    bool dont_run_sdp = false;
    bool only_compute_objective = false;
    
    bool find_extremal_vectors = false;
    flag find_extremal_vectors_construction;
    string find_extremal_vectors_weights = "";
    string find_extremal_vectors_weights_str = "";
    string find_extremal_vectors_colors = "";
    bool find_extremal_vectors_python_output = true;


    bool  find_projection_vectors = false;
    string  find_projection_vectors_input_file = "";

    bool test_tight_for_blow_up = false;
    flag test_tight_for_blow_up_B;
    string test_tight_for_blow_up_Cflag_list;
    string test_tight_for_blow_up_B_Theta_mapping;    
    
    bool find_density_vectors_in_iterated = false;
    flag find_density_vectors_in_iterated_construction;
    
    
    bool print_problem_in_latex_and_quit = false;
    bool generate_baber_equalities_and_quit = false;
    
    bool print_constraints_blocks_and_quit = false;
    int  print_constraints_blocks_and_quit_start_from=0;
    
    bool draw_graphs_in_file = false;
    string draw_graphs_in_filename = "";
    bool draw_graphs_with_densities = false;
    bool draw_graphs_color_1_nonedge = true;
    bool draw_graphs_use_enumerate = false;
    bool draw_graphs_use_cout = false;

    bool process_solution = false;
    // By default we print everyhting....
    double process_solution_lower_bound = -2;
    double process_solution_upper_bound = 2; //0.000001;
    bool process_solution_print_density = true;
    bool process_solution_mosek = false;

    bool compute_denisties_in_file = false;
    string compute_denisties_in_filename = "";

    bool no_slack_flags = false;
    string no_slack_flags_filename = "";

    bool jan_to_nice = false;
    bool jan_to_nice_with_coefficiantes = false;
    string jan_to_nice_filename = "";
    
    bool filter_flags_in_file_allowed = false;
    bool filter_flags_in_file_forbidden = false;
    bool filter_flags_coefficients_in_input = false;
    bool filter_flags_in_file_remove_duplicates = false;
    string filter_flags_in_file_input = "";
    string filter_flags_in_file_filter = "";

    bool filter_flags_using_subflags = false;
    bool filter_flags_using_subflags_as_forbidden = true;
    bool filter_flags_using_subflags_coefficients_in_input = false;   
    string filter_flags_using_subflags_in_file_input = "";
    string filter_flags_using_subflags_in_file_filter = "";
    

    int generate_labeled_flags_of_one_type_size = 0;
    bool generate_labeled_flags_of_one_type_command = false;
    flag generate_labeled_flags_of_one_type_type;    


    bool P_F_ISO_H_test = false;
    flag P_F_ISO_H_test_F;
    flag P_F_ISO_H_test_H;
    
    bool P_F_SAME_TYPE_H_test = false;
    flag P_F_SAME_TYPE_H_test_F;
    flag P_F_SAME_TYPE_H_test_H;
    
    bool P_F_IN_H_test = false;
    flag P_F_IN_H_test_F;
    flag P_F_IN_H_test_H;

    bool P_F1_F2_IN_H_test = false;
    flag P_F1_F2_IN_H_test_F1;
    flag P_F1_F2_IN_H_test_F2;
    flag P_F1_F2_IN_H_test_H;
    
    bool  P_F_IDENTICAL_test=false;
    flag P_F_IDENTICAL_test_F;
    flag P_F_IDENTICAL_test_H;
    
    bool P_F_IN_blowup_of_H_test = false;
    flag P_F_IN_blowup_of_H_test_F;
    flag P_F_IN_blowup_of_H_test_H;

    bool EXISTS_F_IN_blowup_of_H_test = false;
    flag EXISTS_F_IN_blowup_of_H_test_F;
    flag EXISTS_F_IN_blowup_of_H_test_H;

    bool P_F1_TIMES_F2_test = false;
    flag P_F1_TIMES_F2_test_F1;
    flag P_F1_TIMES_F2_test_F2;
    double P_F1_TIMES_F2_test_consts = 1;

    bool flag_calculator_general = false;
    bool flag_calculator_general_draw = false;   
    bool flag_calculator_general_matrix = false;
    bool flag_calculator_general_matrix_draw = false;
    bool flag_calculator_CtimesF = false;
    bool flag_calculator_addCtoF = false;
    bool flag_calculator_delCfromF = false;
    bool flag_calculator_F1timesF2 = false;
    bool flag_calculator_F1plusF2 = false;
    bool flag_calculator_unlabelF = false;
    bool flag_calculator_uplabelF = false;
    bool flag_calculator_cleanF  = false;
    bool flag_calculator_F1inF2 = false;
    bool flag_calculator_maxF1inF2 = false;
    string flag_calculator_F1 = "";
    string flag_calculator_F2 = "";
    double flag_calculator_C = 1;
    string flag_calculator_TYPE = "";
    double flag_calculator_epsilon = 0;
    string flag_calculator_general_string = "";
    string flag_calculator_general_matrix_string = "";

    bool generate_subflags_of_size_n_switch = false;
    int generate_subflags_of_size_n_n = 0;
    string generate_subflags_of_size_n_input = "";

    vector<string> additional_constraints;
    vector<string> additional_extended_constraints;
    bool linear_constriants_remove_duplicates_with_types = false;
    bool dump_linear_constriants_and_quite = false;
    

    bool test_constraints = false;
    bool test_constraints_extended = false;
    bool test_constraints_print_negative = false;
    string test_constraints_constraints_file = "";
    string test_constraints_density_vector_file = "";

    string csdp_OMP = "";

    // Enables to call a python script before csdp happens
    string csdp_preprocessing = "";

    for (int i = 0; i < V; i++) g_blow_up_color_edges[i] = G_BLOW_UP_COLOR_EDGES; 
    for (int i = 0; i < V; i++) g_blow_up_color_3edges[i] = G_BLOW_UP_COLOR_3EDGES; 
    for (int i = 0; i < V; i++) g_blow_up_color_4edges[i] = G_BLOW_UP_COLOR_4EDGES; 

    
    bool use_hack = false;
    string use_hack_1 = "";
    
    if (argc <= 1)
    {
        cerr << endl;
        cerr << "Usage: " << argv[0] << " -n SIZE_OF_UNLABELED_FLAGS [-obj OBJECTIVE_FILENAME] [-v] [-o OUTPUT_DAT-S] [-lb | -ub]" << endl;
        cerr << " -help                        Prints extra extensive help" << endl;
        cerr << " -helpe                      Prints extended expressions help" << endl;
        cerr << " -n SIZE_OF_UNLABELED_FLAGS   Size of unlabeled flags" << endl;
        cerr << " -obj  OBJECTIVE_FILENAME     File containing objective function in old format" << endl;
        cerr << " -obje OBJECTIVE_FILENAME     File containing objective function in extended format" << endl;
        cerr << " -objd CFLAGFILE              Linear combination for dividing the objective function. Experimental!!!" << endl;
        cerr << " -v                           Display progress when creating SDP and generating flags and others" << endl;
        cerr << " -o OUTPUT_DAT-S              Write sdp to a file OUTPUT_DAT-S, usable 'stdout', default test.dat-s" << endl;
        cerr << " -ub                          Creates SDP computing UPPER BOUND on the objective" << endl;        
        cerr << " -lb                          Creates SDP computing LOWER BOUND on the objective" << endl;
        cerr << " -fp                          Stores flag products or tries to reuse them - usefull for repeated computations, eats more space" << endl;
        cerr << " -e FLAG                      Prints all extnesions of FLAG and exits" << endl;        
        cerr << " -ec FLAG                     Prints all extnesions of FLAG and # of labeled copies of FLAG in extensions and exits" << endl;        
        cerr << " -e_fc CFLAGFILE              Prints all extnesions with coefficients of flags with coefficients in the file" << endl;        
        cerr << " -ft FLAG                     Forbiden Test - checks if FLAG is forbidden" << endl;        
        cerr << " -ps                          Processess solution created by csdp" << endl;
        cerr << " -psm                         Processess solution created by mosek" << endl;
        cerr << " -pslb DOUBLE                 Lower bound when processing solution created by csdp or mosek" << endl;
        cerr << " -psub DOUBLE                 Upper bound when processing solution created by csdp or mosek" << endl;
        cerr << " -psd                         Print density when processing solutions created by csdp" << endl;
        cerr << " -psdm                        Print density when processing solutions created by mosek" << endl;
        cerr << " -fgf                         Force generating flags - not loading them from files" << endl;
#ifdef G_COLORED_VERTICES    
        cerr << " -vcp  PATTERN                Vertex Color Pattern: When generating flags, use the pattern for filling color of\n"
                "                              vertices rather than trying all possibilities. Good idea to use with -gsul." << endl;
#endif
        cerr << " -fglf                        Force generating labeled flags - not loading them from file" << endl;
        cerr << " -tff                         Types From File for labeled flags (rather than generating) if possible.\n"
                "                              Should be used with -fglf and without -fp."<< endl;
        cerr << " -lff FLAGFILE                Override the name of the file for loading labeled flags" << endl;
        cerr << " -fgc                         Force generating constraints - not loading from file" << endl;
        cerr << " -rd                          Remove duplicates for unlabeled flags that are loaded - usefull if externally generated." << endl;
        cerr << " -rf                          Remove forbidden unlabeled flags that are loaded 0 usefull if externally generated." << endl;
        cerr << " -du                          Dump unlabeled even when loaded from file - usefull for example if the input contain also forbidden flags, then this can work a 1 time filter" << endl;
        cerr << " -dug                         Dump unlabeled flags as they are being generated - usefull for example if you want to enumerate all flags and not really compute the rest" << endl;
        cerr << " -dto                         Dump types in order and stop. Useful to checking what is the order of types when building CSDP program." << endl;
        cerr << " -gsul                        Generate small unlabled from large ones. Usefull if large ones generated externally and smaller are missing." << endl; 
        cerr << " -qagu                        Quit after generating unlabeled flags" << endl;
        cerr << " -qagl                        Quit after generating labeled flags" << endl;
        cerr << " -oco                         Only Compute Objective" << endl;
        cerr << " -nsdp                        Do not run semidefinite solver at the end" << endl;
        cerr << " -nosdp                       Do not run semidefinite solver at the end" << endl;
        cerr << " -csdp CSDPBIN                Command to run when running csdp. Default is csdp." << endl;
        cerr << " -csdp_OMP N                  Restricts the number of threads in csdp by setting\n"
                "                              OMP_NUM_THREADS=N before executing OMP." << endl;
        cerr << " -csdp_pp PYTHONSCRIPT        When dat-s file is ready, a python script is called that may tweak the file. Useful for adding constraints." << endl;
        cerr << " -mosek                       use mosek rather than csdp" << endl;    
        cerr << " -sdpa                        use sdpa rather than csdp" << endl; 
        cerr << " -solver [mosek|sdpa|csdp]    use one of the solvers" << endl;  
        cerr << " -omp N                       Number of threads to use. OMP_NUM_THREADS overwrite." << endl;
        cerr << " -latex                       Print problem formulation in latex and terminate execution" << endl;
        cerr << " -dlcs                        Disable Linear Constraints simple - just multiple with a variable." << endl;
        cerr << " -lcsuse EQNUMBER             List linear equation that is part of simple product. Starting by index 0. Default is all."<< endl;
        cerr << " -dlcp                        Disable Lienar Constraints Products." << endl;
        cerr << " -elcp                        Enable Linear Constraints products - multiplied with other flags." << endl;
        cerr << " -elcp2                       Enable Linear Constraints products by expanding first." << endl;
        cerr << " -elcsq                       Enable Linear Constraints times SQuare of flags." << endl;
        cerr << " -elcsp                       Enable Linear Constraints times Self Products of flags." << endl;
        cerr << " -tc  CFLAGFILE FILENAME      Test if linear constraints in  FILENAME are satiffied on CFLAGFILE." << endl;
        cerr << " -tce CFLAGFILE EFILENAME     Test if linear constraints in  EFILENAME are satiffied on CFLAGFILE." << endl;
        cerr << " -tcpn                        Print negative constraints." << endl;
        cerr << " -ac FILENAME                 Load Additional Constrains from FILENAME. Can be used multiple times." << endl;
        cerr << " -ace FILENAME                Load additional extended constrains from FILENAME. Can be used multiple times." << endl;
        cerr << " -ab FILENAME                 Load Additional Blocks for CSDP from FILENAME. Can be used multiple times." << endl;
        cerr << " -lcdt                        Test linear constraints for duplicate with type permutaitons." << endl;
        cerr << " -dlc                         Dump linear constraints and quite. Usefull with -lcdt or -elcp2" << endl;
        cerr << " -pcbq                        Print constraints blocks as in CSDP output and quit. Can be used as temp." << endl;
        cerr << " -pcbq_sf INT                 Start with processing unlabeled flag INT. Default is start from beginning (1). Used for resuming failed caluclations. INT means in the dat-s file line starting with INT will be first so be careful." << endl;
        cerr << " -nsf FILENAME                No slack Flags. When going for exact result, it might be useful to solve the\n"
                "                              problem with some variables missing slack. This prodices CSDP where slack are not used for the specified flags." << endl;
        cerr << " -df  FILENAME                Draw flags in file FILENAME" << endl;
        cerr << " -dfd FILENAME                Draw flags in file FILENAME where every flag has a density in front" << endl;
        cerr << " -df1                         When drawing flags, make edges colored 1 with non-zero opacity." << endl;
        cerr << " -dfe                         Use \\enumerate and \\itemize when listing flags" << endl;
        cerr << " -dfcout                      Use std::out when writing the drawing" << endl;
        cerr << " -dtc FILENAME                Converting FILENAME that is dat-s file to cbf file. Useful when trying mosek instead of csdp" << endl;
        cerr << " -cdin FILENAME               Compute densities in FILENAME. FILENAME contains flags with coefficients and\n"
                "                              we go trough all flags on -n vertices and evaluate them against the flags in\n"
                "                              the FILENAME. Usefull when playing with statistics from the solution and one\n"
                "                              would like to see what is the corresponding statistics for smaller flags." << endl;        
        cerr << " -jtn  FILENAME               Jan to Nice - read list of flags in Jan Volec's format and write them in our nice one." << endl;
        cerr << " -jtnc FILENAME               Same as -jtn but flags have coefficients in  [    ]." << endl;
        cerr << " -srp DOUBLE                  Smart round precision. When printing for CSDP, sometimes one wants all coefficients\n"
                "                              to be integers but float calculations migh result in errors that need to be rounded." << endl;
        cerr << " -FisoH FLAG_F FLAG_H         Test isomorphism of F and H" << endl;
        cerr << " -FsametypeH FLAG_F FLAG_H    Test is F and H have the same type" << endl;
        cerr << " -FidenticalH FLAG_F FLAG_H   Test if F and H are identical and writes why if they are not"  << endl;
        cerr << " -FinH  FLAG_F FLAG_H         Counts density of F in H including unlabeling" << endl;
        cerr << " -F1F2inH FLAG_F1 FLAG_F2 FLAG_H Counts density of F1 and F2 in H (debug function)" << endl;
        cerr << " -FinbupH FLAG_F FLAG_H       Counts density of F in a blow-up of H including unlabeling. In the blow-up\n"
                "                              vertices in the blow-up are connected by color colored by 1" << endl;
        cerr << " -eFinbupH FLAG_F FLAG_H      Test if F exists in a blow-up of H." << endl;
        cerr << " -cF1timesF2 C FLAG_F1 FLAG_F2 Computes product of flags F1 and F2 and muptiplies by double C. Same type is needed." << endl;
        cerr << " -gbe                         Generate Baber's Equalities and quit" << endl;
        cerr << " -glft N TYPE                 Generate Labeled Flags on N verties of type TYPE" << endl;
        cerr << " -fev FLAG_CONSTRUCTION       Find Extremal Vectors if we assume that blow-up of FLAG_CONSTRUCTION is the right thing.\n"
                "                              Usefull for rounding. Default is G_BLOW_UP_COLOR_EDGES=1"<< endl;
        cerr << " -fevw WEIGHTS                Adds to -fev also weights for every vertex. Default\n"
                "                              weights are everywhere 1. Should be integeres." << endl;
        cerr << " -fevws WEIGHTSSTR            Adds to -fev also weights for every vertex. Default\n"
                "                              is no extra weights. Note these weights are strings! Useful for irrational stuff." << endl;
        cerr << " -fevc COLORS                 Adds to -fev also colors for every vertex. Default\n"
                "                              colors are everywhere 1. Should be integeres." << endl;
        cerr << " -fevfvec                     Print the output as vector of flags rather than as vector for python." << endl;
        cerr << " -fpv CFLAGFILE               Find projection vectors. All should have smae types. TODO" << endl;
        cerr << " -tt CFLAGFILE BLOWUP         Test Tight - test if the combination of flags is tight for extremal example that would be the blowup\n"
                "                              can be combined with -fevw or -fevc" << endl;
        cerr << " -tttm MAPPING                In Test Tight, prescribes mapping of Theta - so only one is tested rather than all possible." << endl;
        cerr << " -fdvi FLAG_CONSTRUCTION      Find density Vectors if we assume that iterated blow-up of FLAG_CONSTRUCTION is the right thing.\n"
                "                              Usefull for checking if the dual vector of CSDP solution is close to the iterated construction." << endl;
        cerr << " -gsn N  FLAG(s)              Generates all subflags on N vertices from the input" << endl;
        cerr << " -ffrd FILEFLAGS_ALL          Filter input list of flags to remove duplicates." << endl;
        cerr << " -ffa FILEFLAGS_ALL FILEFLAGS_ALLOWED  Filter input list of flags to include only flags in the second file." << endl;
        cerr << " -fff FILEFLAGS_ALL FILEFLAGS_FORBIDDEN  Filter input list of flags to include only flags not in the second file." << endl;
        cerr << " -ffac CFLAGFILE FILEFLAGS_ALLOWED  Same as -ffa but input has coefficients." << endl;
        cerr << " -fffc CFLAGFILE FILEFLAGS_FORBIDDEN  Same as -fff but input has coefficients." << endl;
        
        cerr << " -ffsa FILEFLAGS_ALL FILEFLAGS_ALLOWED  Filter input list of flags to include as subflag at least one of the flags in the second file." << endl;
        cerr << " -fffs FILEFLAGS_ALL FILEFLAGS_FORBIDDEN  Filter input list of flags to include only flags without subflags in the second file." << endl;
        cerr << " -ffasc CFLAGFILE FILEFLAGS_ALLOWED    Same as -ffas but input has coefficients." << endl;
        cerr << " -fffsc CFLAGFILE FILEFLAGS_FORBIDDEN  Same as -fffs but input has coefficients." << endl;
        
        cerr << " -fc_addCtoF C  FLAGFILE              Flag Calculator: Loads flags from FLAGFILE and prints them with all with coefficient C" << endl;
        cerr << " -fc_delCfromF CFLAGFILE            Flag Calculator: Loads flags and coefficients from CFLAGFILE and prints flags without coefficients" << endl;
        cerr << " -fc_CtimesF C  CFLAGFILE             Flag Calculator: Loads flags and coefficients from CFLAGFILE and multiplies by C." << endl;
        cerr << " -fc_F1timesF2 CFLAGFILE1 CFLAGFILE2  Flag Calculator: Loads flags and coefficients from CFLAGFILE1 and CFLAGFILE and mulitplies." << endl;
        cerr << " -fc_squareF   CFLAGFILE              Flag Calculator: Loads flags and coefficients from CFLAGFILE and squares." << endl;
        cerr << " -fc_F1plusF2 CFLAGFILE1 CFLAGFILE2   Flag Calculator: Loads flags and coefficients from CFLAGFILE1 and CFLAGFILE and adds them." << endl;
        cerr << " -fc_unlabelF CFLAGFILE               Flag Calculator: Loads flags and coefficients from CFLAGFILE and unlabels them." << endl;
        cerr << " -fc_uplabelF CFLAGFILE TYPE          Flag Calculator: Loads flags and coefficients from CFLAGFILE and uplabels them to TYPE." << endl;
        cerr << " -fc_cleanF   CFLAGFILE               Flag Calculator: Loads flags and coefficients from CFLAGFILE and simplifies." << endl;
        cerr << " -fc_cleanFe  CFLAGFILE EPS           Flag Calculator: Loads flags and coefficients from CFLAGFILE and simplifies,  del entries <= EPS." << endl;
        cerr << " -fc_F1inF2 CFLAGFILE1 FLAGFILE2      Flag Calculator: Calculates CFLAGFILE1 in FLAGFILE2. Like F1 being objective and F2 all on k vertices." << endl;
        cerr << " -fc_maxF1inF2 CFLAGFILE1 FLAGFILE2   Flag Calculator: Calculates max CFLAGFILE1 in FLAGFILE2. Prints also partial results." << endl;
        cerr << " -fcm EXPRESSION                      Flag Calculator: simple calculator with matrices" << endl;
        cerr << " -fcmd EXPRESSION                     Flag Calculator: draw simple calculator with matrices" << endl;
        cerr << " -fc EXPRESSION                       Flag Calculator: simple calculator" << endl;
        cerr << " -fcd EXPRESSION                      Flag Calculator: draw simple calculator" << endl;
        cerr << endl;

   
        cerr << "Filenames:" << endl;
        cerr << "    " << filename_prefix() << "__n?_unlabeled.txt    unlabeled flags" << endl;
        cerr << "    " << filename_prefix() << "__n?_labeled.txt      labeled flags" << endl;
        cerr << "    " << filename_prefix() << "__n?_sdp_products.txt part of sdp" << endl;
        cerr << "    " << filename_prefix() << "__forbidden.txt       forbidden flags" << endl;
        cerr << "    " << filename_prefix() << "__objective.txt       default objective" << endl;
        cerr << "    " << filename_prefix() << "__objectivee.txt      extended objective" << endl;
#ifdef G_USE_PERMITTED_SUBFLAGS
        cerr << "    " << filename_prefix() << "__permitted.txt       permitted subflags" << endl;
#endif        
#ifdef G_COLORED_EDGES_BLIND
        cerr << "    " << filename_prefix() << "__edgeblind_permutations.txt  allowed perms" << endl;
#endif        
#ifdef G_COLORED_3EDGES_BLIND
        cerr << "    " << filename_prefix() << "__3edgeblind_permutations.txt  allowed perms" << endl;
#endif
#ifdef G_COLORED_VERTICES_BLIND
        cerr << "    " << filename_prefix() << "__vertexblind_permutations.txt allowed perms" << endl;
#endif
        cerr << endl;
        return 0;
    }  
    
    
    int i = 1;
    while (i < argc)
    {        
        if (string(argv[i]) == "-help") {
            help();
            return 0;
        } else if (string(argv[i]) == "-helpe") {
            help_extended_grammar();
            return 0;
        } else if (string(argv[i]) == "-n") {
            extra_arguments(1,i,argc);
            Kn = atoi(argv[i+1]);
            i++;
        } else if (string(argv[i]) == "-omp" || string(argv[i]) == "-OMP") {
            extra_arguments(1,i,argc);
            int num_threads = atoi(argv[i+1]);
            if (num_threads > 0)
            {
#ifdef _USING_OMP_                
                omp_set_num_threads(num_threads);
#else
                cerr << "ERROR: Program compiled without OpenMP support. -omp is useless" << endl;
#endif                
            }
            else
            {
                cerr << "ERROR: num_threads in -omp should be an integer > 0";
                cerr << "  maybe coversion error for '" << argv[i+1] << "'" << endl;
                return 1;
            }            
            i++;
        } else if (string(argv[i]) == "-obj") {
            extra_arguments(1,i,argc);
            extended_objective_format = false;
            objective_file = argv[i + 1];
            i++;   
        } else if (string(argv[i]) == "-obje") {
            extra_arguments(1,i,argc);
            objective_file = argv[i + 1];
            extended_objective_format = true;
            i++;   
        } else if (string(argv[i]) == "-objd") {
            extra_arguments(1,i,argc);
            objective_file_divisor = argv[i + 1];
            i++;   
        } else if (string(argv[i]) == "-o") {
            extra_arguments(1,i,argc);
            output_file = argv[i + 1];
            i++;
        } else if (string(argv[i]) == "-csdp") {
            extra_arguments(1,i,argc);
            csdp_binary = argv[i + 1];
            use_mosek = false;
            use_csdp = true;
            i++;
        } else if (string(argv[i]) == "-solver") {
            extra_arguments(1,i,argc);
            string solver_str  = argv[i + 1];
            use_mosek = false;
            use_csdp = false;
            use_sdpa = false;
            if (solver_str == "mosek") use_mosek = true;
            if (solver_str == "csdp") use_csdp = true;
            if (solver_str == "sdpa") use_sdpa = true;
            if ((use_mosek||use_csdp||use_sdpa) == false)
            {
                cerr << "Error: -solver must be followed by 'sdpa', 'csdp', or 'mosek'. It was followe by '" << solver_str << "'" << endl;
                return 1; 
            }
            i++;
                      
        } else if (string(argv[i]) == "-dtc") {
            convert_dats_to_cbf = true;
            extra_arguments(1,i,argc);
            convert_dats_to_cbf_file = argv[i + 1];
            i++;
        } else if (string(argv[i]) == "-v") {
            verbose_output = 1;
        } else if (string(argv[i]) == "-vv") {
            verbose_output = 2;
        } else if (string(argv[i]) == "-vvv") {
            verbose_output = 3;
        } else if (string(argv[i]) == "-vvvv") {
            verbose_output = 4;
        } else if (string(argv[i]) == "-latex") {
            print_problem_in_latex_and_quit = true;
        } else if (string(argv[i]) == "-dto") {
            dump_types_in_order = true;            
        } else if (string(argv[i]) == "-df") {
            extra_arguments(1,i,argc);
            draw_graphs_in_filename = argv[i + 1];
            draw_graphs_in_file = true;
            draw_graphs_with_densities = false;
            i++;            
        } else if (string(argv[i]) == "-dfd") {
            extra_arguments(1,i,argc);
            draw_graphs_in_filename = argv[i + 1];
            draw_graphs_in_file = true;
            draw_graphs_with_densities = true;
            i++;
        } else if (string(argv[i]) == "-df1") {
            draw_graphs_color_1_nonedge = false;
        } else if (string(argv[i]) == "-dfe") {
            draw_graphs_use_enumerate = true;   
        } else if (string(argv[i]) == "-dfcout") {
            draw_graphs_use_cout = true;   
        } else if (string(argv[i]) == "-jtn") {
            extra_arguments(1,i,argc);
            jan_to_nice_filename = argv[i + 1];
            jan_to_nice = true;
            i++;
        } else if (string(argv[i]) == "-tc") {
            extra_arguments(2,i,argc);
            test_constraints_density_vector_file = argv[i + 1];
            test_constraints_constraints_file = argv[i + 2];
            test_constraints = true;
            i+=2;
        } else if (string(argv[i]) == "-tce") {
            extra_arguments(2,i,argc);
            test_constraints_extended= true;
            test_constraints_density_vector_file = argv[i + 1];
            test_constraints_constraints_file = argv[i + 2];
            test_constraints = true;
            i+=2;
        } else if (string(argv[i]) == "-tcpn") {
            test_constraints_print_negative = true;
        } else if (string(argv[i]) == "-fpv") {
            extra_arguments(1,i,argc);
            find_projection_vectors = true;
            find_projection_vectors_input_file = argv[i + 1];
            i++;            
        } else if (string(argv[i]) == "-jtnc") {
            extra_arguments(1,i,argc);
            jan_to_nice_filename = argv[i + 1];
            jan_to_nice = true;
            jan_to_nice_with_coefficiantes = true;
            i++;
        } else if (string(argv[i]) == "-gsn") {
            extra_arguments(2,i,argc);
            generate_subflags_of_size_n_n = stol(argv[i + 1]);
            generate_subflags_of_size_n_input = argv[i + 2];
            generate_subflags_of_size_n_switch = true;
            i+=2;
        } else if (string(argv[i]) == "-ffrd") {
            extra_arguments(1,i,argc);
            filter_flags_in_file_input = argv[i + 1];
            filter_flags_coefficients_in_input = false;
            filter_flags_in_file_remove_duplicates = true;
            i+=1;                         
        } else if (string(argv[i]) == "-ffa") {
            extra_arguments(2,i,argc);
            filter_flags_in_file_input = argv[i + 1];
            filter_flags_in_file_filter = argv[i+2];
            filter_flags_in_file_allowed = true;
            filter_flags_coefficients_in_input = false;
            i+=2;              
        } else if (string(argv[i]) == "-fff") {
            extra_arguments(2,i,argc);
            filter_flags_in_file_input = argv[i + 1];
            filter_flags_in_file_filter = argv[i+2];
            filter_flags_in_file_forbidden = true;
            filter_flags_coefficients_in_input = false;
            i+=2;
        } else if (string(argv[i]) == "-ffac") {
            extra_arguments(2,i,argc);
            filter_flags_in_file_input = argv[i + 1];
            filter_flags_in_file_filter = argv[i+2];
            filter_flags_in_file_allowed = true;
            filter_flags_coefficients_in_input = true;
            i+=2;
        } else if (string(argv[i]) == "-fffc") {
            extra_arguments(2,i,argc);
            filter_flags_in_file_input = argv[i + 1];
            filter_flags_in_file_filter = argv[i+2];
            filter_flags_in_file_forbidden = true;
            filter_flags_coefficients_in_input = true;
            i+=2;      
        } else if (string(argv[i]) == "-ffas") {
            extra_arguments(2,i,argc);
            filter_flags_using_subflags = true;
            filter_flags_using_subflags_in_file_input = argv[i + 1];
            filter_flags_using_subflags_in_file_filter = argv[i+2];
            filter_flags_using_subflags_as_forbidden = false;
            filter_flags_using_subflags_coefficients_in_input = false;
            i+=2;  
        } else if (string(argv[i]) == "-fffs") {
            extra_arguments(2,i,argc);
            filter_flags_using_subflags = true;
            filter_flags_using_subflags_in_file_input = argv[i + 1];
            filter_flags_using_subflags_in_file_filter = argv[i+2];
            filter_flags_using_subflags_as_forbidden = true;
            filter_flags_using_subflags_coefficients_in_input = false;
            i+=2;  
        } else if (string(argv[i]) == "-ffasv") {
            extra_arguments(2,i,argc);
            filter_flags_using_subflags = true;
            filter_flags_using_subflags_in_file_input = argv[i + 1];
            filter_flags_using_subflags_in_file_filter = argv[i+2];
            filter_flags_using_subflags_as_forbidden = false;
            filter_flags_using_subflags_coefficients_in_input = true;
            i+=2;  
        } else if (string(argv[i]) == "-fffsc") {
            extra_arguments(2,i,argc);
            filter_flags_using_subflags = true;
            filter_flags_using_subflags_in_file_input = argv[i + 1];
            filter_flags_using_subflags_in_file_filter = argv[i+2];
            filter_flags_using_subflags_as_forbidden = true;
            filter_flags_using_subflags_coefficients_in_input = true;
            i+=2;  
        } else if (string(argv[i]) == "-lb") {
            lower_bound = true;
        } else if (string(argv[i]) == "-ub") {
            upper_bound = true;
        } else if (string(argv[i]) == "-rd") {
            remove_duplicates_while_loading = true;
        } else if (string(argv[i]) == "-rf") {
            remove_forbidden_wile_loading = true;
        } else if (string(argv[i]) == "-du") {
            dump_unlabeled = true;
        } else if (string(argv[i]) == "-dug") {
            dump_unlabeled_while_generating = true;
        } else if (string(argv[i]) == "-gsul") {
            generate_small_unlabeled_from_large = true;
        } else if (string(argv[i]) == "-qagu") {
            quit_after_generating_unlabeled = true;
        } else if (string(argv[i]) == "-qagl") {
            quit_after_generating_labeled = true;
        } else if (string(argv[i]) == "-oco") {
            only_compute_objective = true;
        } else if (string(argv[i]) == "-srp") {
            extra_arguments(1,i,argc);
            g_smart_round_precision = strtod(argv[i+1], NULL);
            i++;     
        } else if (string(argv[i]) == "-cdin") {
            extra_arguments(1,i,argc);
            compute_denisties_in_file = true;
            compute_denisties_in_filename = argv[i + 1];
            i++;
        } else if (string(argv[i]) == "-ps") {
            process_solution = true;
        } else if (string(argv[i]) == "-psd") {
            process_solution = true;
            process_solution_print_density = true;
        } else if (string(argv[i]) == "-psm") {
            process_solution = true;
            process_solution_mosek = true;
        } else if (string(argv[i]) == "-psmd" || string(argv[i]) == "-psdm") {
            process_solution = true;
            process_solution_print_density = true;
            process_solution_mosek = true;
        } else if (string(argv[i]) == "-pslb") {
            extra_arguments(1,i,argc);
            process_solution = true;
            process_solution_lower_bound = strtod(argv[i+1], NULL);
            if (process_solution_lower_bound == 0.0)
            {
                cerr << "Setting lower bound to 0 is not allowed since it starts as 0" << endl;
                return 1;
            }
            //if (process_solution_lower_bound  > process_solution_upper_bound) 
            {
                process_solution_upper_bound = 1;
            }
            i++;            
        } else if (string(argv[i]) == "-psub") {
            extra_arguments(1,i,argc);
            process_solution = true;
            process_solution_upper_bound = strtod(argv[i+1], NULL);
            if (process_solution_upper_bound == 0.0)
            {
                cerr << "Setting upper bound to 0 is not allowed since it the program makes no sense after." << endl;
                return 1;
            }
            //if (process_solution_lower_bound  > process_solution_upper_bound) 
            {
                process_solution_lower_bound = 0;
            }
            i++;            
        } else if (string(argv[i]) == "-gbe") {
            generate_baber_equalities_and_quit = true;
        } else if (string(argv[i]) == "-glft") {
            extra_arguments(2,i,argc);
            generate_labeled_flags_of_one_type_command = true;
            generate_labeled_flags_of_one_type_size = atoi(argv[i + 1]);
            generate_labeled_flags_of_one_type_type.load_from_string(argv[i + 2]);
            i += 2;
        } else if (string(argv[i]) == "-nsdp") {
            dont_run_sdp = true; 
        } else if (string(argv[i]) == "-nosdp") {
            dont_run_sdp = true; 
        } else if (string(argv[i]) == "-nocsdp") {
            use_csdp = false;
        } else if (string(argv[i]) == "-csdp_OMP") {
            extra_arguments(1,i,argc);
            csdp_OMP = argv[i + 1];
            i++;
        } else if (string(argv[i]) == "-csdp_pp") {
            extra_arguments(1,i,argc);
            csdp_preprocessing = argv[i + 1];
            i++;
        } else if (string(argv[i]) == "-sdpa") {
            use_sdpa = !use_sdpa;
        } else if (string(argv[i]) == "-mosek") {
            use_mosek = !use_mosek;                        
        } else if (string(argv[i]) == "-fgf") {
            force_generate_flags = true;
            force_generate_labeled_flags = true;

#ifdef G_COLORED_VERTICES    
        } else if (string(argv[i]) == "-vcp") {
            extra_arguments(1,i,argc);
            istringstream ss(argv[i + 1]);
            int color;
            while(ss >> color || !ss.eof()) {
                if(ss.fail()) {
                    cerr << "Unable to parse'" <<  argv[i + 1] << "' as a list of strings" << endl;
                    return 1;
                }
                //cerr << "color=" << color << " " << COLORS_VERTICES << endl;
                assert(color >= 0 && color < COLORS_VERTICES);
                g_vertex_color_pattern.push_back(color);      
            }
            i++;
#endif            
        } else if (string(argv[i]) == "-tff") {
            types_from_file = true;
        } else if (string(argv[i]) == "-fglf") {
            force_generate_labeled_flags = true;
        } else if (string(argv[i]) == "-lff") {
            extra_arguments(1,i,argc);
            labeled_flags_file =  argv[i + 1];
            i += 1;
        } else if (string(argv[i]) == "-fev") {
            extra_arguments(1,i,argc);
            find_extremal_vectors = true;
            find_extremal_vectors_construction.load_from_string(argv[i + 1]);
            i++;
        } else if (string(argv[i]) == "-fevw") {
            extra_arguments(1,i,argc);
            find_extremal_vectors_weights =  argv[i + 1];
            i += 1;     
        } else if (string(argv[i]) == "-fevws") {
            extra_arguments(1,i,argc);
            find_extremal_vectors_weights_str =  argv[i + 1];
            i += 1;
        } else if (string(argv[i]) == "-fevc") {
            extra_arguments(1,i,argc);
            find_extremal_vectors_colors =  argv[i + 1];
            i += 1;
        } else if (string(argv[i]) == "-fevfvec") {
            find_extremal_vectors_python_output = false;            
        } else if (string(argv[i]) == "-tt") {
            extra_arguments(2,i,argc);
            test_tight_for_blow_up = true;
            test_tight_for_blow_up_Cflag_list = argv[i + 1];              
            test_tight_for_blow_up_B.load_from_string(argv[i + 2]);
            i += 2;
        } else if (string(argv[i]) == "-tttm") {
            extra_arguments(1,i,argc);
            test_tight_for_blow_up_B_Theta_mapping = argv[i + 1];              
            i += 1;            
        } else if (string(argv[i]) == "-fdvi") {
            extra_arguments(1,i,argc);
            find_density_vectors_in_iterated = true;
            find_density_vectors_in_iterated_construction.load_from_string(argv[i + 1]);
            i++;
        } else if (string(argv[i]) == "-fgc") {
            force_generating_constriants = true;
        } else if (string(argv[i]) == "-fp") {
            use_sdp_temp = true;
            
        } else if (string(argv[i]) == "-dlcs") {
            g_use_simple_linear_constraints = false;
        } else if (string(argv[i]) == "-lcsuse") {
            extra_arguments(1,i,argc);
            int uselc = (int)strtol(argv[i+1],NULL,0);
            g_use_simple_linear_constraints_list.push_back(uselc);
            i++;
        } else if (string(argv[i]) == "-dlcp") {
            g_use_product_linear_constraints = false;
        } else if (string(argv[i]) == "-elcp") {
            g_use_product_linear_constraints = true;
        } else if (string(argv[i]) == "-elcp2") {
            g_use_product_linear_constraints_by_expanding = true;            
        } else if (string(argv[i]) == "-elcsq") {
            g_use_square_linear_constraints = true;
        } else if (string(argv[i]) == "-elcsp") {
            g_use_linear_constraints_self_products = true;
        } else if (string(argv[i]) == "-ac") {
            extra_arguments(1,i,argc);
            additional_constraints.push_back(argv[i+1]);
            i++;
        } else if (string(argv[i]) == "-ace") {
            extra_arguments(1,i,argc);
            additional_extended_constraints.push_back(argv[i+1]);
            i++;            
        } else if (string(argv[i]) == "-ab") {
            extra_arguments(1,i,argc);
            g_additional_csdp_blocks.push_back(argv[i+1]);
            i++;            
        } else if (string(argv[i]) == "-lcdt") {
            linear_constriants_remove_duplicates_with_types = true;
        } else if (string(argv[i]) == "-dlc") {
            dump_linear_constriants_and_quite = true;
        } else if (string(argv[i]) == "-pcbq") {
            print_constraints_blocks_and_quit = true;
        } else if (string(argv[i]) == "-pcbq_sf") {
            extra_arguments(1,i,argc);
            print_constraints_blocks_and_quit_start_from = atoi(argv[i+1])-1;
            if (print_constraints_blocks_and_quit_start_from < 0)
            {
                cerr << "Error converting " << argv[i+1] << " into a numebr." << endl;
                return 0;
            }
            i++;                     
        } else if (string(argv[i]) == "-nsf") {
            extra_arguments(1,i,argc);
            no_slack_flags=true;
            no_slack_flags_filename = argv[i+1];
            i++;
        } else if (string(argv[i]) == "-extensions" || string(argv[i]) == "-e" || string(argv[i]) == "-ec") {
            extra_arguments(1,i,argc);
            extension_count_copies = string(argv[i]) == "-ec";
            extensions_str = argv[i+1];
            //g_extensions.load_from_string(argv[i + 1]);
            extension = true;
            i++;
        } else if (string(argv[i]) == "-e_fc") {
            extra_arguments(1,i,argc);
            extensions_fc_file = argv[i+1];
            extensions_fc = true;
            i++;
        } else if (string(argv[i]) == "-forbidden_test" || string(argv[i]) == "-ft" ) {
            extra_arguments(1,i,argc);
            g_forbidden_test.load_from_string(argv[i + 1]);
            forbidden_test = true;
            i++;
        } else if (string(argv[i]) == "-FisoH" ) {
            extra_arguments(2,i,argc);
            P_F_ISO_H_test_F.load_from_string(argv[i + 1]);
            P_F_ISO_H_test_H.load_from_string(argv[i + 2]);
            P_F_ISO_H_test = true;
            i += 2;
        } else if (string(argv[i]) == "-FsametypeH" ) {
            extra_arguments(2,i,argc);
            P_F_SAME_TYPE_H_test_F.load_from_string(argv[i + 1]);
            P_F_SAME_TYPE_H_test_H.load_from_string(argv[i + 2]);
            P_F_SAME_TYPE_H_test = true;
            i += 2;
        } else if (string(argv[i]) == "-FidenticalH" ) {
            extra_arguments(2,i,argc);
            P_F_IDENTICAL_test_F.load_from_string(argv[i + 1]);
            P_F_IDENTICAL_test_H.load_from_string(argv[i + 2]);
            P_F_IDENTICAL_test = true;
            i += 2;            
        } else if (string(argv[i]) == "-FinH" ) {
            extra_arguments(2,i,argc);
            P_F_IN_H_test_F.load_from_string(argv[i + 1]);
            P_F_IN_H_test_H.load_from_string(argv[i + 2]);
            P_F_IN_H_test = true;
            i += 2;
        } else if (string(argv[i]) == "-F1F2inH" ) {
            extra_arguments(3,i,argc);
            P_F1_F2_IN_H_test_F1.load_from_string(argv[i + 1]);
            P_F1_F2_IN_H_test_F2.load_from_string(argv[i + 2]);
            P_F1_F2_IN_H_test_H.load_from_string(argv[i + 3]);
            P_F1_F2_IN_H_test = true;
            i += 3;            
        } else if (string(argv[i]) == "-FinbupH" ) {
            extra_arguments(2,i,argc);
            P_F_IN_blowup_of_H_test_F.load_from_string(argv[i + 1]);
            P_F_IN_blowup_of_H_test_H.load_from_string(argv[i + 2]);
            P_F_IN_blowup_of_H_test = true;
            i += 2;
        } else if (string(argv[i]) == "-eFinbupH" ) {
            extra_arguments(2,i,argc);
            EXISTS_F_IN_blowup_of_H_test_F.load_from_string(argv[i + 1]);
            EXISTS_F_IN_blowup_of_H_test_H.load_from_string(argv[i + 2]);
            EXISTS_F_IN_blowup_of_H_test = true;
            i += 2;
        } else if (string(argv[i]) == "-cF1timesF2" ) {
            extra_arguments(3,i,argc);
            P_F1_TIMES_F2_test_consts=stod(argv[i + 1]);
            P_F1_TIMES_F2_test_F1.load_from_string(argv[i + 2]);
            P_F1_TIMES_F2_test_F2.load_from_string(argv[i + 3]);
            P_F1_TIMES_F2_test = true;
            i += 3;
        } else if (string(argv[i]) == "-fc_addCtoF") {
            extra_arguments(2,i,argc);
            flag_calculator_addCtoF = true;
            flag_calculator_C  = stod(argv[i + 1]);
            flag_calculator_F1 = argv[i + 2];
            i += 2;
        } else if (string(argv[i]) == "-fc_delCfromF") {
            extra_arguments(1,i,argc);
            flag_calculator_delCfromF = true;
            flag_calculator_F1 = argv[i + 1];
            i += 1;
        } else if (string(argv[i]) == "-fc_CtimesF") {
            extra_arguments(2,i,argc);
            flag_calculator_CtimesF = true;
            flag_calculator_C  = stod(argv[i + 1]);
            flag_calculator_F1 = argv[i + 2];
            i += 2;
        } else if (string(argv[i]) == "-fc_F1timesF2") {
            extra_arguments(2,i,argc);
            flag_calculator_F1timesF2 = true;
            flag_calculator_F1 = argv[i + 1];
            flag_calculator_F2 = argv[i + 2];
            i += 2;
        } else if (string(argv[i]) == "-fc_squareF") {
            extra_arguments(1,i,argc);
            flag_calculator_F1timesF2 = true;
            flag_calculator_F1 = argv[i + 1];
            flag_calculator_F2 = argv[i + 1];
            i += 1;            
        } else if (string(argv[i]) == "-fc_F1plusF2") {
            extra_arguments(2,i,argc);
            flag_calculator_F1plusF2 = true;
            flag_calculator_F1 = argv[i + 1];
            flag_calculator_F2 = argv[i + 2];
            i += 2;
        } else if (string(argv[i]) == "-fc_unlabelF") {
            extra_arguments(1,i,argc);
            flag_calculator_unlabelF = true;
            flag_calculator_F1 = argv[i + 1];
            i += 1;
        } else if (string(argv[i]) == "-fc_uplabelF") {
            extra_arguments(2,i,argc);
            flag_calculator_uplabelF = true;
            flag_calculator_F1 = argv[i + 1];
            flag_calculator_TYPE = argv[i + 2];
            i += 2;
        } else if (string(argv[i]) == "-fc_cleanF") {
            extra_arguments(1,i,argc);
            flag_calculator_cleanF = true;
            flag_calculator_F1 = argv[i + 1];
            i += 1;
        } else if (string(argv[i]) == "-fc_cleanFe") {
            extra_arguments(2,i,argc);
            flag_calculator_cleanF = true;
            flag_calculator_F1 = argv[i + 1];
            flag_calculator_epsilon = stod(argv[i + 2]);
            i += 2;            
        } else if (string(argv[i]) == "-fc_F1inF2") {
            extra_arguments(2,i,argc);
            flag_calculator_F1inF2 = true;
            flag_calculator_F1 = argv[i + 1];
            flag_calculator_F2 = argv[i + 2];
            i += 2;
        } else if (string(argv[i]) == "-fc_maxF1inF2") {
            extra_arguments(2,i,argc);
            flag_calculator_maxF1inF2 = true;
            flag_calculator_F1 = argv[i + 1];
            flag_calculator_F2 = argv[i + 2];
            i += 2;
        } else if (string(argv[i]) == "-fc") {
            extra_arguments(1,i,argc);
            flag_calculator_general = true;
            flag_calculator_general_string = argv[i + 1];
            i += 1;  
        } else if (string(argv[i]) == "-fcd") {
            extra_arguments(1,i,argc);
            flag_calculator_general = true;
            flag_calculator_general_string = argv[i + 1];
            i += 1;  
            flag_calculator_general_draw = true;  
        } else if (string(argv[i]) == "-fcm") {
            extra_arguments(1,i,argc);
            flag_calculator_general_matrix = true;
            flag_calculator_general_matrix_string = argv[i + 1];
            i += 1;  
        } else if (string(argv[i]) == "-fcmd") {
            extra_arguments(1,i,argc);
            flag_calculator_general_matrix = true;
            flag_calculator_general_matrix_string = argv[i + 1];
            i += 1;  
            flag_calculator_general_matrix_draw = true;     
        } else if (string(argv[i]) == "-hack" ) {
            use_hack = true;
        } else if (string(argv[i]) == "-hack1" ) {
            use_hack = true;
            use_hack_1 = argv[i + 1];
            i += 1;
        } else {
            cerr << "Unsupported argument " << argv[i] << " " << endl;
            return 0;
        }
        i++;
    }
    
    if (i > argc)
    {
        cerr << "Probably some missing argument." << endl;
        return 1;
    }

    

    if (convert_dats_to_cbf)
    {
        string tmp;
        if (dats_to_cbf(convert_dats_to_cbf_file, tmp))
        {
            cerr << "dat-s file " << convert_dats_to_cbf_file << " converted to cbf file " << convert_dats_to_cbf_file << endl;
        }
        else
        {
            cerr << "Failed converting dat-s file " << convert_dats_to_cbf_file << " to cbf file " << convert_dats_to_cbf_file << endl;
        }
        return 0;
    }

    
#ifdef G_LOAD_COLORED_EDGES_BLIND_PERMUTATIONS    
    load_blind_colorededge_permutations();
#endif
#ifdef G_COLORED_3EDGES_BLIND    
    load_blind_colored3edge_permutations();
#endif
#ifdef G_COLORED_VERTICES_BLIND    
    load_blind_coloredvertex_permutations();
#endif

    
    
    if (draw_graphs_in_file) {
        draw_graphs(draw_graphs_in_filename, draw_graphs_with_densities, draw_graphs_color_1_nonedge, draw_graphs_use_enumerate, draw_graphs_use_cout);
        return 0;
    }
    
    if (jan_to_nice)
    {
        jan_to_nice_convert(jan_to_nice_filename, jan_to_nice_with_coefficiantes);
        return 0;
    }
    


    if (generate_subflags_of_size_n_switch)
    {
        vector<flag> flag_list;
        load_flags_from_file(generate_subflags_of_size_n_input, flag_list);
        for (flag& f : flag_list)
        {
            cerr << "processing " << f.print() << endl;
            cerr << "subflags of size " << generate_subflags_of_size_n_n << endl;
            vector<flag> subflags;
            f.generate_subflags_of_size_n(generate_subflags_of_size_n_n, subflags);

            // dump the generated subflags
            // but only if has three vertices colored [1]
            for (flag& sf : subflags)
            {

                cout << sf.print() << endl;
                // This is a hack to only draw the subgraphs that we have
                // anyway..
                /*
                if (sf.m_color_vertex[2]==1 && sf.m_color_vertex[3]==2)
                {
                    cout << sf.print() << endl;
                }

                // And a symmetric one
                if (sf.m_color_vertex[3]==1 && sf.m_color_vertex[4]==2)
                {
                    sf.m_color_vertex[0]=sf.m_color_vertex[1]=sf.m_color_vertex[2]=sf.m_color_vertex[3]=2;
                    sf.m_color_vertex[4]=sf.m_color_vertex[5]=sf.m_color_vertex[6]=1;
                    //cout << sf.print() << endl;
                }
                */
            }
        }

        return 0;        
    }


    if (generate_labeled_flags_of_one_type_command)
    {
        vector<flag> all_flags;
        generate_labeled_flags_of_one_type(generate_labeled_flags_of_one_type_size, generate_labeled_flags_of_one_type_type, all_flags);
        for (int i = 0; i < (int)all_flags.size(); i++)
        {
            cout << all_flags[i].print() << endl;
        }
        return 0;
    }
    
    if (filter_flags_in_file_remove_duplicates)
    {
        filter_flags_remove_duplicates(filter_flags_in_file_input, filter_flags_coefficients_in_input);
        return 0;
    }

    if (filter_flags_in_file_allowed)
    {
        filter_flags(filter_flags_in_file_input, filter_flags_in_file_filter, true, filter_flags_coefficients_in_input);
        return 0;
    }
    if (filter_flags_in_file_forbidden)
    {
        filter_flags(filter_flags_in_file_input, filter_flags_in_file_filter, false, filter_flags_coefficients_in_input);
        return 0;
    }

    if (filter_flags_using_subflags)
    {
        filter_flags_using_subflags_fun(filter_flags_using_subflags_in_file_input, filter_flags_using_subflags_in_file_filter, filter_flags_using_subflags_as_forbidden, filter_flags_using_subflags_coefficients_in_input);
        return 0;

    }
    
    if (P_F_ISO_H_test)
    {   
        if (verbose_output > 0)
            cout << "Isomorphic: " << P_F_ISO_H_test_F.is_isomorphic_to<true>(P_F_ISO_H_test_H) << endl;
        else
        {
            cout << "Isomorphic: " << P_F_ISO_H_test_F.is_isomorphic_to<false>(P_F_ISO_H_test_H) << endl;
        }
            
        //cout << "Isomorphic: " << P_F_ISO_H_test_H.is_isomorphic_to(P_F_ISO_H_test_F) << endl;
        return 0;
    }    
    
    if (P_F_SAME_TYPE_H_test)
    {
        cout << "Having same type:" << P_F_SAME_TYPE_H_test_F.have_same_type(P_F_SAME_TYPE_H_test_H) << endl;
        return 0;
    }
    

    
    if (P_F_IDENTICAL_test)
    {
        cout << "Identity test: ";
        if (P_F_IDENTICAL_test_F.is_identical_to(P_F_IDENTICAL_test_H))
        {
            cout << "passed - both identical" << endl;
        }
        else
        {
            cout << "Not identical" << endl;
        }
        return 0;
    }
    
    
    load_forbidden();

#ifdef G_USE_PERMITTED_SUBFLAGS
    load_permitted();    
#endif  
    
    
    if (extension)
    {
        vector<flag> flag_list;
        load_flags_from_file(extensions_str, flag_list);
        for (flag& f : flag_list)
        {
            if (verbose_output)
            {
                cerr << "# extending " << f.print() << endl;
            }
            //extensions_of_g_with_print(g_extensions,extension_count_copies);
            extensions_of_g_with_print(f,extension_count_copies);
        }
        return 0;
    }


    if (extensions_fc)
    {
        extensions_of_fc(extensions_fc_file);
        return 0;
    }

    if (forbidden_test)
    {
        if (is_flag_forbidden(g_forbidden_test, verbose_output))
        {
            cerr << "-ft flag is forbidden " << g_forbidden_test.print() << endl;
        } else {
            cerr << "-ft flag is not forbidden " << g_forbidden_test.print() << endl;    
        }
        return 0;
    }
    
    if (P_F_IN_H_test)
    {   
        cout << "Density: " << P_F1_IN_H(P_F_IN_H_test_F, P_F_IN_H_test_H) << " Count: " << P_F1_IN_H(P_F_IN_H_test_F, P_F_IN_H_test_H, false) << endl;
        return 0;
    }

    if (P_F1_F2_IN_H_test)
    {   
        cout << "P_F1_F2_IN_H=: " << P_F1_F2_IN_H(P_F1_F2_IN_H_test_F1, P_F1_F2_IN_H_test_F2, P_F1_F2_IN_H_test_H, true) << endl;
        return 0;
    }

    if (P_F_IN_blowup_of_H_test)
    {
        cerr << "Density: ";
        cout << std::setw(10) << P_F1_IN_blowup_of_H(P_F_IN_blowup_of_H_test_F, P_F_IN_blowup_of_H_test_H) << endl;
        // << " Count: " << P_F1_IN_blowup_of_H(P_F_IN_blowup_of_H_test_F, P_F_IN_blowup_of_H_test_H,false)<< "/" << P_F_IN_blowup_of_H_test_H.m_vertices << "^" << P_F_IN_blowup_of_H_test_F.m_vertices << endl;
        return 0;
    }

    
    if (EXISTS_F_IN_blowup_of_H_test)
    {
        cout << "F in blowup of H: " << EXISTS_F_IN_blowup_of_H(EXISTS_F_IN_blowup_of_H_test_F, EXISTS_F_IN_blowup_of_H_test_H) << endl;
        return 0;
    }
    
    if (generate_baber_equalities_and_quit)
    {
        generate_baber_equalities(Kn, verbose_output);
        return 0;
    }
    
    
    
    
    if (P_F1_TIMES_F2_test)
    {
        cout << "Product of " << P_F1_TIMES_F2_test_F1.print() << " and " << P_F1_TIMES_F2_test_F2.print() << " is " << endl;
        print_F1_times_F2(P_F1_TIMES_F2_test_consts,P_F1_TIMES_F2_test_F1,P_F1_TIMES_F2_test_F2);
        return 0;
    }
    
    if (flag_calculator_addCtoF)
    {
        cerr << "Including coefficent " << flag_calculator_C << " to flags " << flag_calculator_F1  << endl;
        vector<flag> all_flags;
        load_flags_from_file(flag_calculator_F1, all_flags);
        for (int i = 0; i < (int)all_flags.size(); i++)
        {
            cout << flag_calculator_C << "  " << all_flags[i].print() << endl;
        }
        return 0;
    }

    if (flag_calculator_delCfromF)
    {
        cerr << "Removing coefficent coefficients from  " << flag_calculator_F1  << endl;
        vector<flag_and_coefficient> F1;
        load_flags_and_coefficients_from_file(flag_calculator_F1, F1);
        for (flag_and_coefficient &f : F1)
        {
            cout << f.g.print() << endl;
        }
        return 0;
    }

    if (flag_calculator_CtimesF)
    {
        cerr << "Mutiplying " << flag_calculator_F1 << " by " << flag_calculator_C << endl;
        vector<flag_and_coefficient> F1;
        load_flags_and_coefficients_from_file(flag_calculator_F1, F1);
        
        multiply_FC_by_C(F1, flag_calculator_C);
        
        dump_flags_and_coefficients(F1);
        return 0;
    }
    
    
    
    if (flag_calculator_F1plusF2)
    {
        cerr << "Summing " << flag_calculator_F1 << " and " << flag_calculator_F2 << endl;
        vector<flag_and_coefficient> F1, F2;
        load_flags_and_coefficients_from_file(flag_calculator_F1, F1);
        load_flags_and_coefficients_from_file(flag_calculator_F2, F2);
        
        fc_add_FC_to_first(F1,F2);
        
        simplify_FC(F1);
        
        dump_flags_and_coefficients(F1);
        return 0;
    }
    
    
    if (flag_calculator_F1timesF2)
    {
        cerr << "Multiplying " << flag_calculator_F1 << " and " << flag_calculator_F2 << endl;
        vector<flag_and_coefficient> F1, F2;
        load_flags_and_coefficients_from_file(flag_calculator_F1, F1);
        load_flags_and_coefficients_from_file(flag_calculator_F2, F2);
        vector<flag_and_coefficient> sumF;
        
        fc_F1_times_F2(F1, F2, sumF);

        dump_flags_and_coefficients(sumF);
        return 0;
    }
    
    
    if (flag_calculator_unlabelF)
    {
        cerr << "Unlabeling " << flag_calculator_F1 << endl;
        vector<flag_and_coefficient> F1;
        load_flags_and_coefficients_from_file(flag_calculator_F1, F1);
        
                
        fc_unlabel_in_place(F1);
        dump_flags_and_coefficients(F1);

        /*
        vector<flag_and_coefficient> FU;
        
        for (int i = 0; i < (int)F1.size(); i++)
        {
            flag_and_coefficient fc;
            
            fc.g = F1[i].g;
            fc.g.m_Theta = 0;
            fc.coefficient = P_F1_IN_H(F1[i].g, fc.g)*F1[i].coefficient;
            
            vector<flag_and_coefficient> tmp;
            tmp.push_back(fc);
            
            fc_add_FC_to_first(FU,tmp);
        }
        
        dump_flags_and_coefficients(FU);
        */
        return 0;
    }
    
    if (flag_calculator_uplabelF)
    {
        cerr << "Uplabeling " << flag_calculator_F1 << " to type " << flag_calculator_TYPE << endl;
        vector<flag_and_coefficient> F1;
        load_flags_and_coefficients_from_file(flag_calculator_F1, F1);
        
        flag type;
        type.load_from_string(flag_calculator_TYPE.c_str());
        if (type.m_Theta != type.m_vertices)
        {
            cerr << "Expected type as second argument - so number of labeled vertices is the same as unlabeled." << endl;
            cerr << "But provided something with " <<type.m_Theta << " labeled and " << type.m_vertices << " total vertices." << endl;
            return 0;
        }
        
        vector<flag_and_coefficient> FU;
        
        for (int i = 0; i < (int)F1.size(); i++)
        {
            if (F1[i].g.m_Theta != 0) {
                cerr << "Cannot uplabel labeled types!" << endl;
                return 0;
            }
            int flag_size = F1[i].g.m_vertices + type.m_vertices;
            vector<flag> flag_list;
            get_labeled_flags_of_one_type(flag_size, type, flag_list);
            
            vector<flag_and_coefficient> tmp;
            for (int j = 0; j < (int) flag_list.size(); j++)
            {
                // remove labeled vertices....
                flag test = flag_list[j];
                test.remove_labeled_vertices();
                if (test.is_isomorphic_to(F1[i].g))
                {
                    flag_and_coefficient fc;
                    fc.g = flag_list[j];
                    fc.g.m_Theta = type.m_vertices;
                    fc.coefficient = F1[i].coefficient;
                    tmp.push_back(fc);
                }
            }
            
            fc_add_FC_to_first(FU,tmp);
        }
        
        dump_flags_and_coefficients(FU);
        return 0;
    }
    

    if (flag_calculator_cleanF)
    {
        cerr << "Simplyfying " << flag_calculator_F1 << " with epsilon " << flag_calculator_epsilon << endl;
        vector<flag_and_coefficient> F1;
        load_flags_and_coefficients_from_file(flag_calculator_F1, F1);

        simplify_FC(F1, flag_calculator_epsilon);
        dump_flags_and_coefficients(F1, true);
        return 0;
        
    }
    
    if (flag_calculator_F1inF2 || flag_calculator_maxF1inF2)
    {
        cerr << "Calculating ";
        if (flag_calculator_maxF1inF2) cerr << "max ";
        cerr << flag_calculator_F1 << " in " << flag_calculator_F2 << endl;
        vector<flag_and_coefficient> F1;
        load_flags_and_coefficients_from_file(flag_calculator_F1, F1);
        stringstream filename;
        double maxF1inF2 = -1000000000000;
        
        OPEN_FILE_SMARTLY(istr, flag_calculator_F2);

        flag g;
        while (g.load_from_stream(*istr,-1,-1))
        {
            double total_density = 0;    
            
            for (int i = 0; i < (int)F1.size(); i++)
            {
                total_density += F1[i].coefficient * P_F1_IN_H(F1[i].g, g);                
            }
            
            if (flag_calculator_F1inF2 || total_density >= maxF1inF2)
            {
                cout.precision(G_PRECISION);
                cout << smart_round(total_density) << " " << g.print() << endl;
                maxF1inF2 = total_density;
            }
        }
        
        //infile.close();
        return 0;
    }

    if (flag_calculator_general)
    {
        vector<flag_and_coefficient> F;


        ostream *ostr = NULL;

        if (flag_calculator_general_draw)
        {
            ostr = &cout;
        }

        if (ostr != NULL)
        {
            print_latex_header((*ostr), draw_graphs_color_1_nonedge);
        }

        flag_calculator_simple(F, flag_calculator_general_string, ostr);
        
        if (ostr != NULL)
        {
            (*ostr) << "\\end{document}" << endl;
        }

        //simplify_FC(F);  
        dump_flags_and_coefficients(F);
        return 0;
    }

    if (flag_calculator_general_matrix)
    {
        vector< vector < vector<flag_and_coefficient> > > FM ;

        ostream *ostr = NULL;

        if (flag_calculator_general_matrix_draw)
        {
            ostr = &cout;
        }

        if (ostr != NULL)
        {
            print_latex_header((*ostr), draw_graphs_color_1_nonedge);
        }

        flag_calculator_matrix(FM, flag_calculator_general_matrix_string, ostr);
        
        if (ostr != NULL)
        {
            (*ostr) << "\\end{document}" << endl;
        }


        cout << "mf " << endl; 
        bool first_row=true;
        for (auto FV : FM)
        {   
            if (first_row == false)
            {
                cout << " \\ " << endl;
            }
            first_row = false;

            bool first_column=true;
            for (auto F : FV)
            {
                if (first_column == false)
                {
                    cout << " , " << endl;
                }
                first_column = false;

                //simplify_FC(F);  
                dump_flags_and_coefficients(F);
            }
        }
        cout << "x "  << endl; 
        return 0;
    }


    //  0 <=  Sum c_iF_i
    //  0 <=  H * (Sum c_iF_i)  for many H 
    if (test_constraints)
    {
        vector<flag_and_coefficient> density_vector;

        load_flags_and_coefficients_from_file(test_constraints_density_vector_file, density_vector);

        // Check that all graphs in the density vector have the same order
        if (density_vector.size() == 0)
        {
            cerr << "No entries in the density vecotr of flags loaded from file " << test_constraints_density_vector_file << endl;
            return 0;
        }
        if (verbose_output)
        {
            cerr << "Loaded density vector with " << density_vector.size() << " entries." << endl;
        }

        // Reusing variable Kn, this is where we work now
        Kn = density_vector[0].g.m_vertices;

        for (auto &Fj : density_vector)
        {
            if (Fj.g.m_vertices != Kn)
            {
                cerr << "Found a flag in " << test_constraints_density_vector_file << " of order " << Fj.g.m_vertices << " while " << Kn << " is expected " << endl;
                cerr << "The flag is: " << Fj.g.print() << endl;
                return 0;
            }
        }

        // Constraints loaded in g_linear_constraints
        if (test_constraints_extended)
            load_extended_linear_constraints_from_file(test_constraints_constraints_file, true, verbose_output);
        else
            load_linear_constraints_from_file(test_constraints_constraints_file, true, verbose_output);

        int useful_constraints = 0;
        int useless_constraints = 0;
        int constraint_ID = 1;
        for (auto &lc  : g_linear_constraints)
        {

            if (verbose_output)
            {
                cerr << "Testing usefulness of constraint " << constraint_ID++ << " out of " << g_linear_constraints.size()  << "\n";
                cerr << "Constraint has " << lc.m_entries.size() << " entries" << endl;
            }


            

            // Evaluation of the constraint 
            // 0 <=  \sum alpha_i F_i  =  \sum alpha_i p(F_i,G)
            // p(F_i,G)  =  \sum_{Fj on n vertices}  p(F_i,F_j)p(F_j,G) 
            // 0 <=  \sum alpha_i p(F_i,G) = \sum_i alpha_i  \sum_{Fj} p(F_i,F_j)p(F_j,G)
            //    = \sum_{Fj} \sum_i alpha_i p(F_i,F_j)p(F_j)
            double bigsum = 0;
            
            // Should be uncommented.....
            linear_constraint lc_tmp = lc;

            // A huge speed boost by firts unlabeling the constraint
            fc_unlabel_in_place(lc_tmp.m_entries);

            #pragma omp parallel for schedule(dynamic) reduction(+:bigsum)
            for (int FjID = 0; FjID < (int)density_vector.size(); FjID++)
            //for (auto &Fj : density_vector)
            {
                const flag_and_coefficient &Fj = density_vector[FjID];

                for (auto &Fi : lc_tmp.m_entries)
                {
                    //   \alpha_i    *     p(F_i,F_j)    *     p(F_j)
                    bigsum += Fi.coefficient*P_F1_IN_H(Fi.g, Fj.g)*Fj.coefficient;
                }
            }
            

            if (verbose_output)
                cerr << "Big sum = " << bigsum << endl << endl;
            double bigsum_min = bigsum;

            // If -elcp option was set, we try products as well
            // The base constraint 
            // 0 <=  \sum alpha_i F_i  
            // implies also constraints
            // 0 <=  F_k * (\sum alpha_i F_i)
            // 0 <=  \sum alpha_i (F_k*F_i)  =  \sum alpha_i  P(F_k,F_i; G)
            // 0 <=  \sum alpha_i  P(F_k,F_i; G)
            // p(F_k,F_i; G)  =  \sum_{Fj on n vertices}  p(F_k,F_i;F_j)p(F_j,G) 
            //
            // 0 <=   \sum_{Fj} \sum alpha_i  P(F_k,F_i; F_j) p(F_j,G)
            // The line above is what we evaluate
            if (g_use_product_linear_constraints)
            {
                int type_size = lc.m_labeled_vertices_in_type_cnt;         
                int flag_size = (Kn - lc.m_entries_max_size) + type_size;

                //if (flag_size == type_size) continue;
         // multiplying by just 1 vertex does not do much (unless the vertex has a color)
#ifdef G_COLORED_VERTICES
                if (flag_size < 1) continue;
#else
                if (flag_size < 2)  continue;
#endif
                vector<flag> flags_to_multiply_with; 
                get_labeled_flags_of_one_type(flag_size, lc.m_type, flags_to_multiply_with);

/*
                int Fk_counter = 1;
                for (auto &Fk : flags_to_multiply_with)
                {
                    bigsum = 0;
                    #pragma omp parallel for schedule(dynamic) reduction(+:bigsum)
                    for (int FjID = 0; FjID < (int)density_vector.size(); FjID++)
                    //for (auto &Fj : density_vector)
                    {
                        const flag_and_coefficient &Fj = density_vector[FjID];

                        for (auto &Fi : lc.m_entries)
                        {
                            //   \alpha_i    *     p(F_i,F_j)    *     p(F_j)
                            bigsum += Fi.coefficient*P_F1_F2_IN_labeled_H(Fk, Fi.g, Fj.g)*Fj.coefficient;
                            //bigsum += Fi.coefficient*P_F1_IN_H(Fi.g, Fj.g)*Fj.coefficient;
                        }
                    }

                    if (bigsum == 0)
                    {
                        cerr << "This should not happen" << endl;
                        cerr << "Fk = " << Fk.print() << endl;
                        cerr << "Constraint " << endl <<  lc.print();
                        cerr << "Output:" << endl;                     
                        return 0;                        
                    }

                    if (verbose_output >= 2)
                        cerr << "Big sum = " << bigsum  << " for  " << Fk_counter++ << " out of " << flags_to_multiply_with.size() << endl;

                    if (test_constraints_print_negative && bigsum < 0)
                    {
                    //    cout << "# Constraint value " <<  bigsum  << " \n" <<  lc.print() << " Times " << Fk.print() << endl;
                    }

                    // Keep the smallest one
                    if (bigsum < bigsum_min)
                    {
                        bigsum_min = bigsum;
                    }
                } 
            }
*/
                int Fk_counter = 1;
                for (auto &Fk : flags_to_multiply_with)
                {
                    vector<flag_and_coefficient> Fkc;
                    flag_and_coefficient fc;
                    fc.g = Fk;
                    fc.coefficient = 1;
                    Fkc.push_back(fc);

                    vector<flag_and_coefficient> FkFi;

                    fc_F1_times_F2(lc.m_entries, Fkc, FkFi);
                    fc_unlabel_in_place(FkFi);

                    bigsum = 0;

                    #pragma omp parallel for schedule(dynamic) reduction(+:bigsum)
                    for (int FjID = 0; FjID < (int)density_vector.size(); FjID++)
                    //for (auto &Fj : density_vector)
                    {
                        const flag_and_coefficient &Fj = density_vector[FjID];

                        for (auto &Fi : FkFi)
                        {
                            //   \alpha_i    *     p(F_i,F_j)    *     p(F_j)
                            bigsum += Fi.coefficient*P_F1_IN_H(Fi.g, Fj.g)*Fj.coefficient;
                        }
                    }

                    if (bigsum == 0)
                    {
                        cerr << "This should not happen" << endl;
                        cerr << "Fk = " << Fk.print() << endl;
                        cerr << "Constraint " << endl <<  lc.print();
                        cerr << "Output:" << endl;                     
                        return 0;                        
                    }

                    if (verbose_output >= 2)
                        cerr << "Big sum = " << bigsum  << " for  " << Fk_counter++ << " out of " << flags_to_multiply_with.size() << endl;

                    if (test_constraints_print_negative && bigsum < 0)
                    {
                    //    cout << "# Constraint value " <<  bigsum  << " \n" <<  lc.print() << " Times " << Fk.print() << endl;
                    }

                    // Keep the smallest one
                    if (bigsum < bigsum_min)
                    {
                        bigsum_min = bigsum;
                    }
                } 
            }

            if (bigsum_min < 0.00001) useful_constraints++;
            else useless_constraints++;
            if (test_constraints_print_negative && bigsum_min < 0)
            {
                 cout << "# Constraint smalles value " <<  bigsum_min  << " \n" <<  lc.print() << endl;
            }
        }
        cerr << "# of useful  constraints: " << useful_constraints << endl;
        cerr << "# of useless constraints: " << useless_constraints << endl;

        return 0;
    }
 



    if (test_tight_for_blow_up)
    {
        assert(find_extremal_vectors_weights_str == "");
        test_tight_for_blow_up_B_func(test_tight_for_blow_up_B, test_tight_for_blow_up_Cflag_list, find_extremal_vectors_weights, find_extremal_vectors_colors, test_tight_for_blow_up_B_Theta_mapping, verbose_output);
        return 0;
    }



    if (use_hack)
    {
    #ifdef G_EXTRNAL_HACK_CPP
        cerr << "Using external hack" << endl;
        external_hack();
        return 0;
    #endif

        //hack_K34_to_K35_metacrossings();

        if  (use_hack_1 == "")
        {
            //hack();
            //hack_crossings();
        }
        else
        {
        // HACK!!
            //hack(use_hack_1);
        //hack_ping();
        }
        return 0;
    }    

    
    //////////////////////////////// Normal use....
    
#ifndef USE_FOR_CROSSINGS    
    if (objective_file == "")
    {
        stringstream filename;
        filename <<  filename_prefix() << "__objective.txt";
    
        
        std::ifstream filetmp(filename.str());
        if(filetmp.is_open())
        {
            objective_file = filename.str();
            cerr << "Objective file not provided, trying default filename " << objective_file << endl;
        }
        else
        {
            stringstream filenamee;
            filenamee <<  filename_prefix() << "__objectivee.txt";

            std::ifstream filetmpe(filenamee.str());
            if(filetmpe.is_open())
            {
                objective_file = filenamee.str();
                extended_objective_format = true;
                cerr << "Objective file not provided, using default EXTENDED filename " << objective_file << endl;
            }
            else
            {
                cerr << "Objective file needed. Default " << filename.str() << " or " << filenamee.str() << " were not found." << endl;
                return 1;
            }
        }
    }


    if (extended_objective_format)
    {
        if (!load_extended_objective_from_file(objective_file, linear_constriants_remove_duplicates_with_types, verbose_output))
        {
            return 1;
        }
    }
    else
    {
        if (!load_objective_from_file(objective_file, linear_constriants_remove_duplicates_with_types, verbose_output))
        {
            return 1;
        }    
    }
#else //  USE_FOR_CROSSINGS
    if (objective_file != "")
    {
        cerr << "Objective function is ignored and it is always used as the number of crossings!" << endl;
        return 1;
    }
    else
    {
        stringstream filename;
        filename <<  filename_prefix();
        
        objective_file = filename.str(); 
    }
    
    cerr << "NOTE: Program is compiled for using crossings count as objective function." << endl;
    cerr << "      Usual objective function is being ignored." << endl;    
#endif    
//#ifdef USE_FOR_CROSSINGS
//    cerr << "Warning: using special function for crossings - not everything works." << endl;
//    return main_crossings(Kn);
//#endif

    
    if (objective_file_divisor != "")
    {
        load_flags_and_coefficients_from_file(objective_file_divisor, g_objective_divisor);
        cerr << "Loaded a linear combination of " << g_objective_divisor.size() << " flags as a global divisor" << endl;
    }


    
    if (additional_constraints.size() > 0)
    {
        for (int i = 0; i < (int)additional_constraints.size(); i++)
        {
            if (!load_linear_constraints_from_file(additional_constraints[i], linear_constriants_remove_duplicates_with_types, verbose_output))
            {
                cerr << "Loading additional constraints from " << additional_constraints[i] << " failed" << endl;
                return 0;
            }
        }
    }
    
    if (additional_extended_constraints.size() > 0)
    {
        cerr << "Loadig " << additional_extended_constraints.size()  << " extended constraint(s) " << endl;
        for (int i = 0; i < (int)additional_extended_constraints.size(); i++)
        {
            //if (verbose_output)
            {
                cerr << "Loadig constraint from  '" << additional_extended_constraints[i] << "'" << endl;
            }

            load_extended_linear_constraints_from_file(additional_extended_constraints[i], linear_constriants_remove_duplicates_with_types, verbose_output); 
        }
    }
    


    if (g_additional_csdp_blocks.size() > 0)
    {
        for (int i = 0; i < (int)g_additional_csdp_blocks.size(); i++)
        {
            
            ifstream infile;
            infile.open (g_additional_csdp_blocks[i].c_str(), ifstream::in);
            if (!infile.good())
            {
                cerr << "Failed opening file " << g_additional_csdp_blocks[i] << endl;
                return 0;
            }
            int blockKn;
            infile >> blockKn;
            
            if (blockKn != Kn)
            {
                cerr << "Loading additional CSDP blocks from " << g_additional_csdp_blocks[i] << " failed." << endl;
                cerr << "Blocks were computed n=" << blockKn << " while this program is running with n="  << Kn << endl;
                return 0;
            }
            infile.close();
            cerr << "Using additional csdp blocks from " << g_additional_csdp_blocks[i] << endl;
        }
    }

#ifdef G_EDGE_COLOR_SYMMETRY
    add_edge_color_symmetry_constraints(Kn);
#endif


    if (g_use_linear_constraints_self_products)
    {
        generate_products_of_constraints(Kn,objective_file,force_generating_constriants,verbose_output);
    }    


#ifdef WRITE_INTEGER_DENSITIES_FOR_LINEAR_CONSTRAINTS
    scale_linear_constraints_to_integers(Kn);
#endif

    if (g_use_product_linear_constraints_by_expanding)
    {    
        expand_linear_constraints_by_products(Kn, verbose_output);
    }
    

    if (dump_linear_constriants_and_quite)
    {
        for (const auto&  lc : g_linear_constraints)
        {
            cout << lc << endl;
        }
        return 0;
    }




    assert(Kn <= V);
    
    if (print_problem_in_latex_and_quit)
    {
        print_problem_in_latex(objective_file, draw_graphs_color_1_nonedge);
        return 0;
    }
    

    get_unlabeled_flags_of_size(Kn, force_generate_flags, remove_duplicates_while_loading, remove_forbidden_wile_loading, verbose_output, dump_unlabeled_while_generating);
    if (generate_small_unlabeled_from_large) generate_all_unlabeled_subflags_from_size(Kn, verbose_output);
    if (dump_unlabeled) dump_unlabeled_flags(Kn);
    if (quit_after_generating_unlabeled)
    {
        cerr << "Quitting after generating unlabeled flags." << endl;
        return 0;
    }

    // 
    if (compute_denisties_in_file)
    {
        compute_densities_in(compute_denisties_in_filename, Kn, verbose_output);
        return 0;
    }
    
    
    if (find_density_vectors_in_iterated)
    {
        find_density_vectors_in_iterated_blow_up_of(find_density_vectors_in_iterated_construction, Kn);
        return 0;
    }    

    if (only_compute_objective)
    {
        cout.precision(G_PRECISION);
        for (int i = 0; i < (int)g_unlabeled_flags[Kn].size(); i++)
            cout << smart_round(compute_objective_function_for_SDP(g_unlabeled_flags[Kn][i])) << "  " << g_unlabeled_flags[Kn][i].print() << " # " << i+1 << endl;
        return 0;
    }
    

/*
#ifdef G_USE_INDEXES_FOR_LARGE_UNLABELED_FLAGS
    if (verbose_output)
        cerr  << "Setting up indexes for large unlabeled graphs in constraints" << endl;
    for (auto&  lc : g_linear_constraints)
    {
        if (lc.m_entries_max_size != Kn || lc.m_type.m_Theta != 0)
            continue;
        //#pragma omp parallel for ordered schedule(dynamic)
        #pragma omp parallel for
        for (int i = 0; i < (int)lc.m_entries.size(); i++)
        {
            auto &fc = lc.m_entries[i];
            if (fc.g.m_Theta == 0 && fc.g.m_vertices == Kn && fc.g.m_unlabeled_id == -1 )
            {
                //cerr << "XXXX" << endl;
                fc.g.m_unlabeled_id = find_flag_in_list(fc.g, g_unlabeled_flags[Kn]);
            }
        }
    }
#endif   
*/    
    ///////// Now the standard thing......
    
    // labeled flags
    g_flags.reserve(MAX_FLAG_TYPES); // Should make live much faster
    if (force_generate_labeled_flags || !load_labeled_flags_from_file(Kn, 1, labeled_flags_file))
    {

// This is old code used for debuging 
#ifdef USE_FOR_CROSSINGS____
        if (Kn == 6)
        {
            // generate_flags_as_subflags_k6_321(Kn);
            			generate_flags_as_subflags_k6_33(Kn);
            //			generate_flags_as_subflags_k6_222(Kn);
        }
        if (Kn == 7)
        {
            generate_flags_as_subflags_k7_3_2_2(Kn);
            //generate_flags_as_subflags_k7_3_4(Kn);
        }
        if (Kn == 8)
        {
            generate_flags_as_subflags_k8_3_5(Kn);
        }        
#else


        // Initialize g_exact_number_of_colored_vertices - currently important just
        // when computing with crossings of bipartite graphs...
#ifdef G_COLORED_VERTICES           
        bool check_sensible_flags = true;
        if (g_unlabeled_flags[Kn].size() == 0)
        {
            cerr << "No unlabeled flags of size " << Kn << endl;
            return 0;
        }
        
        g_exact_number_of_colored_vertices[0]=-1;
        for (int c = 1; c < COLORS_VERTICES; c++)
        {
            g_exact_number_of_colored_vertices[c] = g_unlabeled_flags[Kn][0].m_colored_vertices[c];
            for (int i = 1; i < (int)g_unlabeled_flags[Kn].size(); i++)
            {
                if (g_exact_number_of_colored_vertices[c] != g_unlabeled_flags[Kn][i].m_colored_vertices[c])
                {
                    g_exact_number_of_colored_vertices[c]  = -1;
                    break;
                }
            }
        }
#else
        bool check_sensible_flags = false;
#endif
        
        cerr << "Generating labeled flags..." << endl;
        if (check_sensible_flags)
            cerr << " ... using only sensible flags" << endl;

        // Generating all labeled flags:
        int last_types = (int)g_flags.size();
//// TODO: Make this parallel        
        for (int i = 1; i <= Kn/2; i++)
        {
#ifdef DISABLE_UNLABELED_PRODUCTS
            if (Kn-2*i == 0) continue;
#endif
            cerr << "Getting labeled flags of size " << Kn-i << ":" << Kn-2*i << endl;
            generate_labeled_flags(Kn-i,Kn-2*i, verbose_output, types_from_file, check_sensible_flags);
            int gain = (int)g_flags.size() - last_types;
            cerr << "Got "  << gain << " types ";
            for (int j = last_types; j < (int)g_flags.size(); j++)
                cerr << g_flags[j].size() << " ";
            cerr << endl;
            last_types = (int)g_flags.size();
        }
#endif // For crossings
        dump_labeled_flags(Kn, labeled_flags_file);
    }
    cerr << "Labeled flag have " << (int)g_flags.size() << " types. Counts: ";
    for (int i = 0; i < (int)g_flags.size(); i++) cerr << " " << g_flags[i].size();
    cerr << endl;
    if (g_flags.size() == 0)
    {
        cerr << "WARNING: No labeled flags. Computations might not work." << endl;
    }
    if (quit_after_generating_labeled)
    {
        return 0;
    }
    


    if (find_projection_vectors)
    {
#ifndef G_FLAG_PRODUCTS_SLOW_LOW_MEMORY        
        cerr << "It might be useful to use -DG_FLAG_PRODUCTS_SLOW_LOW_MEMORY in the Makefile" << endl;
        cerr << "This will allow types to have .X notation and include more equations at once to the file" << endl;
#endif        
        find_projection_vectors_for_one_file(find_projection_vectors_input_file);
        return 0;
    }

    if (find_extremal_vectors)
    {
        find_extremal_vectors_in_blow_up_of(find_extremal_vectors_construction, find_extremal_vectors_weights, find_extremal_vectors_weights_str, find_extremal_vectors_colors, find_extremal_vectors_python_output, verbose_output);
        return 0;
    }
    
    
   if (dump_types_in_order)
   {
	 for (int f = 0; f < (int)g_flags.size(); f++)
	 {
	 	assert(g_flags[f].size() != 0);
		flag type_f;
		g_flags[f][0].get_type_subflag(type_f);
		cout << type_f.print() << endl;
	 }
   	return 0;
   }
    


    if (print_constraints_blocks_and_quit)
    {
        // A double check that we have the correct Kn
        cout << Kn << endl;
        stringstream ss;
        cout << print_CSDP_constraints_header(Kn, ss, verbose_output) << endl;
        cout << ss.str();
        cout << endl;
        
        mini_timer mt;
        parallel_output_linearizer output_buffer(cout, print_constraints_blocks_and_quit_start_from);

        if (print_constraints_blocks_and_quit_start_from != 0)
        {
            cerr << "We are not starting from zero! We hope you know what are you doing." << endl;
        }

        // printing of subflags
         #pragma omp parallel for ordered schedule(dynamic)
        for (int i = print_constraints_blocks_and_quit_start_from; i < (int)g_unlabeled_flags[Kn].size(); i++)
        {            
            if (verbose_output) 
            {
                #pragma omp critical(cerr)
                {
                    std::cerr << "Computing product part for part " << i+1 << "/" << g_unlabeled_flags[Kn].size()
                              << mt.report(i-print_constraints_blocks_and_quit_start_from, g_unlabeled_flags[Kn].size()-print_constraints_blocks_and_quit_start_from) << endl;
                }
            }
            stringstream ss;
            
            int matrixID = i + 1;
         
            print_CSDP_constraints_blocks(ss, Kn, i, matrixID, 0, verbose_output);            
            

            // We do not back-up these so whatever order works
            // For serious constraints like the cuts heavy programs, this actually may make a differnece.
            #pragma omp critical
            {
                output_buffer.print_string(i, ss.str());
            }
        }
        assert(output_buffer.is_empty());
        return 0;
    }
    

    if (!upper_bound && !lower_bound)
    {
        cerr << "I dont know if I should try upper bound our lower bound - you need to tell me using -lb or -ub" << endl;
        return 1;
    }
    
    if (upper_bound && lower_bound)
    {
        cerr << "I'm sorry, but I cannot do both upper and lower bound at the same time." << endl;
        return 1;
    }    

    
    // 
    if (no_slack_flags)
    {
        load_flags_from_file(no_slack_flags_filename, g_no_slack_flags);
        cerr << "Loaded list of " << g_no_slack_flags.size() << " flags with no slack" << endl;
    }
    
    // create result_file
    if (output_file == "" )
    {
        stringstream filename;
        filename << "SDP_n" << Kn << "_";
        if (lower_bound) filename << "LB_";
        else  filename << "UB_";
        filename <<  objective_file << ".dat-s"; 
        output_file = filename.str();
    }
    stringstream filename;
    filename << output_file << ".result";
    string result_file = filename.str();   
    
    
    
    if (process_solution && process_solution_mosek == false)
    {
        if (output_file == "stdout")
        {
            process_csdp_solution(cin, Kn, process_solution_lower_bound, process_solution_upper_bound, process_solution_print_density);
            return 0;
        } 
        else 
        {
            ifstream results;
            results.open (result_file.c_str(),ifstream::in);
            if (!results.good())
            {
                cerr << "Failed opening file with CSDP result " << result_file << endl;
                cerr << "Will try MOSEK solution file" << endl;
                process_solution_mosek = true;
            }
            else
            {
                cerr << "Processing CSDP solution file " << result_file << endl;
                process_csdp_solution(results, Kn, process_solution_lower_bound, process_solution_upper_bound, process_solution_print_density);
                return 0;
            }
        }
    }

    if (process_solution && process_solution_mosek == true)
    {
        stringstream filename;
        filename << output_file << ".sol";
        string mosek_result_file = filename.str();   

        cerr << "Processing MOSEK solution file " << mosek_result_file << endl;
        process_mosek_solution(mosek_result_file, Kn, process_solution_lower_bound, process_solution_upper_bound, process_solution_print_density);            
        return 0;
    }


    bool sdp_temp_up_to_date = true;
    if (force_generate_flags || force_generate_labeled_flags)
    {
        sdp_temp_up_to_date = false;
        //cerr << "Todo";
        //assert(0);
    }
        
    if (output_file == "stdout")
    {
        cerr << "Generating SDP program to stdout..." << endl;
        print_CSDP(Kn,upper_bound,cout,verbose_output,use_sdp_temp, sdp_temp_up_to_date);
        
    } else {
        if (output_file == "" )
        {
            stringstream filename;
            filename << "SDP_n" << Kn << "_";
            if (lower_bound) filename << "LB_";
            else  filename << "UB_";
            filename <<  objective_file << ".dat-s"; 
            output_file = filename.str();
        }
        stringstream filename;
        filename << output_file << ".result";
        string result_file = filename.str();
               
        ofstream outfile;
        outfile.open (output_file.c_str(), ofstream::out);
        if (!outfile.good())
        {
            cerr << "Failed opening file " << output_file << endl;
            return -1;
        }
        cerr << "Generating SDP program to " << output_file << endl;
        print_CSDP(Kn,upper_bound,outfile,verbose_output,use_sdp_temp, sdp_temp_up_to_date);
        outfile.close();

        if (csdp_preprocessing != "")
        {
            stringstream ss;
            ss << "python " << csdp_preprocessing << " " << output_file;
            cerr << "Executing preprocessor " << csdp_preprocessing << endl;
            int rv = system(ss.str().c_str());
            if (rv == -1)
            {
                cerr << "Execition failed!" << endl;
                assert(0);
            }
        }

        if (!dont_run_sdp)
        {
            if (!csdp_OMP.empty())
            {
                setenv("OMP_NUM_THREADS", csdp_OMP.c_str(), 1);
            }

            //
            cerr << "Trying hack by setting MKL_DEBUG_CPU_TYPE=5" << endl;
            setenv("MKL_DEBUG_CPU_TYPE", "5", 1);            

           int csdp_return_value = 1;
           if (system(NULL)) 
           {
                // cout << "Command processor exists";
                //assert(use_sdpa == false); 
                string command_test;

                cerr << "Solvers found: ";


                bool mosek_available = false;
                command_test = "which " + mosek_binary + " > /dev/null 2>&1";
                if (system(command_test.c_str())) {
                    // Command doesn't exist...
                } else {
                    // Command does exist, do something with it...
                    mosek_available = true;
                    cerr << " mosek ";
                }

                bool sdpa_available = false;
                command_test = "which  " + sdpa_binary + " > /dev/null 2>&1";
                if (system(command_test.c_str())) {
                    // Command doesn't exist...
                } else {
                    // Command does exist, do something with it...
                    sdpa_available = true;
                    cerr << " sdpa ";
                }

                bool csdp_available = false;
                command_test = "which "  + csdp_binary + "  > /dev/null 2>&1";
                if (system(command_test.c_str())) {
                    // Command doesn't exist...
                } else {
                    // Command does exist, do something with it...
                    csdp_available = true;
                    cerr << " csdp ";
                }
                cerr << endl;



                stringstream system_command;
                string log_file = output_file;

                if (use_mosek && mosek_available)
                {
                    log_file += ".mosek.log";

                    string cbf_filename;
                    cerr << "using mosek, rewriting .dat-s to .cbf" << endl;
                    dats_to_cbf(output_file, cbf_filename);
                    system_command <<  "'" << mosek_binary <<  "' '" << cbf_filename << "'  2>&1  | tee  '"  <<  log_file << "'";
                    //system_command <<  "'./dats-to-mosek.py' '" << output_file << "'  '" <<  result_file << "'  2>&1  | tee  '"  <<  log_file << "'";
                }
                else if (use_sdpa && sdpa_available)
                {
                    log_file += ".sdpa.log";
                    system_command <<  "'" << sdpa_binary << "' -ds '" << output_file << "' -o '" <<  result_file << "'  2>&1  | tee  '"  <<  log_file << "'";
                }
                else if (use_csdp && csdp_available)
                { 
                    log_file += ".csdp.log";
                    system_command <<  "'" << csdp_binary << "'  '" << output_file << "'  '" <<  result_file << "'  2>&1  | tee  '"  <<  log_file << "'";
                }
                else 
                {
                    cerr << "ERROR: No semidefinie programming solver found." << endl;
                    return 1;
                }
                cerr << "Executing: " << system_command.str() << endl;
                csdp_return_value = system(system_command.str().c_str());

                 //execlp(csdp_binary.c_str(),"csdp",output_file.c_str(),result_file.c_str(),(char *)NULL);
           }

           if (csdp_return_value != 0)
           {
               cout << "Command processor doesn't exists or the system call failed."; 
  
                pid_t csdp_PID = 0;
                csdp_PID = fork();
                
                if (csdp_PID == 0)
                {
                    // clean memory! TODO

                    if (use_sdpa)
                    {
                        execlp("sdpa","sdpa","-ds",output_file.c_str(),"-o",result_file.c_str(),(char *)NULL);                
                    }
                    else
                    {
                        execlp(csdp_binary.c_str(),"csdp",output_file.c_str(),result_file.c_str(),(char *)NULL);
                    }

                    cerr << "Strugglig with executing: " << csdp_binary.c_str() << endl;
                    cerr << "Something went wrong: " <<  strerror(errno) << endl;
                }
                else
                {
                    int csdo_status;
                    pid_t tpid = wait(&csdo_status);
                    if (tpid != csdp_PID)
                    {
                        cerr << "Something went wrong!" << endl;
                    }

                }
           }

        }
        //execlp("sdpa","sdpa","-ds",output_file.c_str(),"-o",result_file.c_str(),(char *)NULL);
        //system("csdp",output_file.c_str());
    }
    
    cerr << "Done." << endl;
        
    return 0;    
}
#endif


