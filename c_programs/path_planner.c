
/*
* EcoMender Bot (EB): Task 2B Path Planner
*
* This program computes the valid path from the start point to the end point.
* Make sure you don't change anything outside the "Add your code here" section.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#define V 32

#ifdef __linux__ // for host pc

    #include <stdio.h>

    void _put_byte(char c) { putchar(c); }

    void _put_str(char *str) {
        while (*str) {
            _put_byte(*str++);
        }
    }

    void print_output(uint8_t num) {
        if (num == 0) {
            putchar('0'); // if the number is 0, directly print '0'
            _put_byte('\n');
            return;
        }

        if (num < 0) {
            putchar('-'); // print the negative sign for negative numbers
            num = -num;   // make the number positive for easier processing
        }

        // convert the integer to a string
        char buffer[20]; // assuming a 32-bit integer, the maximum number of digits is 10 (plus sign and null terminator)
        uint8_t index = 0;

        while (num > 0) {
            buffer[index++] = '0' + num % 10; // convert the last digit to its character representation
            num /= 10;                        // move to the next digit
        }
        // print the characters in reverse order (from right to left)
        while (index > 0) { putchar(buffer[--index]); }
        _put_byte('\n');
    }

    void _put_value(uint8_t val) { print_output(val); }

#else  // for the test device

    void _put_value(uint8_t val) { }
    void _put_str(char *str) { }

#endif

// main function
int main(int argc, char const *argv[]) {

    #ifdef __linux__

        const uint8_t START_POINT   = atoi(argv[1]);
        const uint8_t END_POINT     = atoi(argv[2]);
        uint8_t NODE_POINT          = 0;
        uint8_t CPU_DONE            = 0;

    #else
        // Address value of variables for RISC-V Implementation
        #define START_POINT         (* (volatile uint8_t * ) 0x02000000)
        #define END_POINT           (* (volatile uint8_t * ) 0x02000004)
        #define NODE_POINT          (* (volatile uint8_t * ) 0x02000008)
        #define CPU_DONE            (* (volatile uint8_t * ) 0x0200000c)

    #endif

    // array to store the planned path
    uint8_t path_planned[32];
    // index to keep track of the path_planned array
    uint8_t idx = 0;

    /* Functions Usage

    instead of using printf() function for debugging,
    use the below function calls to print a number, string or a newline

    for newline: _put_byte('\n');
    for string:  _put_str("your string here");
    for number:  _put_value(your_number_here);

    Examples:
            _put_value(START_POINT);
            _put_value(END_POINT);
            _put_str("Hello World!");
            _put_byte('\n');
    */

    // ############# Add your code here #############
    #ifdef __linux__
        uint32_t graph[32];
    #else
        uint32_t *graph = (uint32_t *) 0x02000010;
    #endif

    // Define adjacency list using bit representation
    // Each uint32_t represents connections for a node
    
    graph[ 0]  =  (1 << 1) | (1 << 10) | (1 << 6);                                    // 0 : connects to  1, 10, 6
    graph[ 1]  =  (1 << 0) | (1 << 11) | (1 << 2);                                    // 1 : connects to  0, 11, 2
    graph[ 2]  =  (1 << 3) | (1 << 5)  | (1 << 4) | (1 << 1);                         // 2 : connects to  3,  5, 4, 1
    graph[ 3]  =  (1 << 2);                                                           // 3 : connects to  2
    graph[ 4]  =  (1 << 2);                                                           // 4 : connects to  2
    graph[ 5]  =  (1 << 2);                                                           // 5 : connects to  2
    graph[ 6]  =  (1 << 7) | (1 << 8) | (1 << 9) | (1 << 0);                          // 6 : connects to  7,  8, 9, 0
    graph[ 7]  =  (1 << 6);                                                           // 7 : connects to  6
    graph[ 8]  =  (1 << 6);                                                           // 8 : connects to  6
    graph[ 9]  =  (1 << 6);                                                           // 9 : connects to  6
    graph[10]  =  (1 << 0)  | (1 << 11) | (1 << 24) | (1 << 26);                      // 10: connects to  1, 11, 24, 26
    graph[11]  =  (1 << 10) | (1 << 12) | (1 << 1) | (1 << 19);                       // 11: connects to 10, 12,  1, 19
    graph[12]  =  (1 << 11) | (1 << 13) | (1 << 14);                                  // 12: connects to 11, 13, 14
    graph[13]  =  (1 << 12);                                                          // 13: connects to 12
    graph[14]  =  (1 << 12) | (1 << 15) | (1 << 16);                                  // 14: connects to 12, 15, 16
    graph[15]  =  (1 << 14);                                                          // 15: connects to 14
    graph[16]  =  (1 << 14) | (1 << 17) | (1 << 18);                                  // 16: connects to 14, 17, 18
    graph[17]  =  (1 << 16);                                                          // 17: connects to 16
    graph[18]  =  (1 << 16) | (1 << 19) | (1 << 21);                                  // 18: connects to 16, 19, 21
    graph[19]  =  (1 << 18) | (1 << 20) | (1 << 11);                                  // 19: connects to 18, 20, 11
    graph[20]  =  (1 << 19);                                                          // 20: connects to 19
    graph[21]  =  (1 << 18) | (1 << 22) | (1 << 23);                                  // 21: connects to 18, 22, 23
    graph[22]  =  (1 << 21);                                                          // 22: connects to 21
    graph[23]  =  (1 << 21) | (1 << 24) | (1 << 30);                                  // 23: connects to 21, 24, 30
    graph[24]  =  (1 << 23) | (1 << 25) | (1 << 10);                                  // 24: connects to 23, 25, 10
    graph[25]  =  (1 << 24);                                                          // 25: connects to 24
    graph[26]  =  (1 << 10) | (1 << 27) | (1 << 28);                                  // 26: connects to 10, 27, 28
    graph[27]  =  (1 << 26);                                                          // 27: connects to 26
    graph[28]  =  (1 << 26) | (1 << 29) | (1 << 30);                                  // 28: connects to 26, 29, 30
    graph[29]  =  (1 << 28);                                                          // 29: connects to 28
    graph[30]  =  (1 << 28) | (1 << 31) | (1 << 23);                                  // 30: connects to 28, 31, 23
    graph[31]  =  (1 << 30);                                                          // 31: connects to 30


    // Arrays for Dijkstra's algorithm
    #ifdef __linux__
        //uint32_t graph[32];
        uint8_t dist[32];
        uint8_t prev[32];
        uint8_t visited[32];
    #else
        //uint32_t *graph = (uint32_t *) 0x02000010;
        uint8_t *dist = (uint8_t *) 0x02000090;
        uint8_t *prev = (uint8_t *) 0x020000B0;
        uint8_t *visited = (uint8_t *) 0x020000D0;
    #endif



    // Initialize arrays
    for (uint8_t i = 0; i < 32; i++) {
        dist[i] = UINT8_MAX;
        prev[i] = UINT8_MAX;
        visited[i] = 0;
    }

    // Set start point distance to 0
    dist[START_POINT] = 0;

    // Dijkstra's algorithm
    for (uint8_t count = 0; count < 32; count++) {
        // Find minimum distance vertex
        uint8_t min_dist = UINT8_MAX;
        uint8_t u = 0;
        
        for (uint8_t v = 0; v < 32; v++) {
            if (!visited[v] && dist[v] < min_dist) {
                min_dist = dist[v];
                u = v;
            }
        }
        
        // If we can't reach any more nodes, break
        if (min_dist == UINT8_MAX) {
            break;
        }
        
        visited[u] = 1;
        
        // Check all possible connections
        for (uint8_t v = 0; v < 32; v++) {
            if ((graph[u] & (1U << v)) && !visited[v]) {
                uint8_t alt = dist[u] + 1; // Using weight 1 for all edges
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                }
            }
        }
    }

    // Reconstruct path
    uint8_t current = END_POINT;
    uint8_t temp_path[32];
    uint8_t temp_idx = 0;

    // Build path from end to start
    while (current != UINT8_MAX && temp_idx < 32) {
        temp_path[temp_idx++] = current;
        current = prev[current];
    }

    // Reverse path into final array
    idx = 0;
    for (int i = temp_idx - 1; i >= 0; i--) {
        path_planned[idx++] = temp_path[i];
    }
    // ##############################################

    // the node values are written into data memory sequentially.
    for (int i = 0; i < idx; ++i) {
        NODE_POINT = path_planned[i];
    }
    // Path Planning Computation Done Flag
    CPU_DONE = 1;

    #ifdef __linux__    // for host pc

        _put_str("######### Planned Path #########\n");
        for (int i = 0; i < idx; ++i) {
            _put_value(path_planned[i]);
        }
        
        // _put_byte('\n');
        // _put_str("######### START_POINT ##########\n");
        // _put_value(START_POINT);

        // _put_byte('\n');
        // _put_str("######### END_POINT ############\n");
        // _put_value(END_POINT);

        // _put_byte('\n');
        // _put_str("######### Node Points ##########\n");
        // for (int i = 0; i < idx; ++i) {
        //     NODE_POINT = path_planned[i];
        //     _put_value(NODE_POINT);
        // }

        // _put_byte('\n');
        // _put_str("######### CPU_DONE Flag #########\n");
        // _put_value(CPU_DONE);

        _put_str("################################\n");

    #endif

    return 0;
}

