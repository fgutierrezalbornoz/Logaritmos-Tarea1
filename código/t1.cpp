#include <iostream>
#include <vector>
#include <math.h>
#include <cstdlib>

/* 
run
 g++ t1.cpp -o t1 
./t1
*/
// Definir constantes
constexpr size_t MAX_SIZE = 1024;
constexpr size_t ELE_SIZE = sizeof(long long);
constexpr size_t PAGE_SIZE = MAX_SIZE / ELE_SIZE;

class Page {
public:
    long long page[PAGE_SIZE];
    int last_pos = 0;
    Page() {
        std::fill(std::begin(page), std::end(page), 0);
    }
};

class File {
private:
    int p;                  // Número de páginas
    int t=0;
    std::vector<Page> pages; // Vector para almacenar las páginas

public:
    // Constructor que inicializa la clase con el número de páginas
    File(int num_pages) : p(num_pages), pages(num_pages) {
        if (num_pages <= 0) {
            throw std::invalid_argument("El número de páginas debe ser positivo.");
        }
    }

    // Método para acceder a una página específica
    Page& getPage(int index) {
        if (index < 0 || index >= p) {
            throw std::out_of_range("Índice de página fuera de rango.");
        }
        return pages[index];
    }

    // Método para obtener el número de páginas
    int getNumPages() const {
        return p;
    }
    int gett() const {
        return t;
    }

    void insert(long long x){
        long long k = h(x) % (1LL << (t+1));
        if (k<p){
            //insertar en pagina k
            insertInPage(x, pages[k]);

        }
        else{
            //insertar en la pagina k - 2^t
            insertInPage(x, pages[k-(1LL << t)]);
        }
        return;
    }
    void insertInPage(long long x, Page P){
        if (P.last_pos >= PAGE_SIZE) {
            std::cerr << "Error: El array está lleno, no se puede insertar más elementos." << std::endl;
            return;
        }
        P.page[P.last_pos] = x;
        ++P.last_pos;
        return;
    }
};



long long h(int y){
    long long upper = (1LL << 24) - 1; 
    int lower = 0; 
    long long num = rand() % (upper - lower + 1) + lower;
    return num;
}



int main(){
    int num_pages = 2;
    File archivo(num_pages);
     for (int page_index = 0; page_index < num_pages; ++page_index) {
            Page& pagina = archivo.getPage(page_index);
            for (size_t i = 0; i < PAGE_SIZE; ++i) {
                pagina.page[i] = page_index * PAGE_SIZE + i; // Ejemplo de inicialización
            }
        }
    for (int page_index = 0; page_index < num_pages; ++page_index) {
            std::cout << "Valores en la página " << page_index << ":\n";
            Page& pagina = archivo.getPage(page_index);
            for (size_t i = 0; i < PAGE_SIZE; ++i) {
                std::cout << pagina.page[i] << " ";
            }
            std::cout << "\n";
        }
    return 0;

}