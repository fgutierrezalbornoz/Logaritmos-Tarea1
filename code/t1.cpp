// Código base Francisco
#include <iostream>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <fstream>

constexpr size_t MAX_SIZE = 1024;  // Tamaño máximo de una página en bytes.
constexpr size_t ELE_SIZE = sizeof(long long);  // Tamaño de un elemento long long (8 bytes).
constexpr size_t PAGE_SIZE = MAX_SIZE / ELE_SIZE; // Número máximo de elementos que caben en una página (128 elementos).
constexpr size_t MAX_COST = 3; // Costo máximo promedio permitido

// Función de Hash h(y)
// Devuelve un valor random entre 0 y 2^64 − 1 para cualquier elemento
long long h(long long y){
    long long upper = (1LL << 24) - 1;
    long long num = rand() % upper;
    return num;
}



class Page {
public:
    long long page[PAGE_SIZE];  // Array que almacena los elementos de la página.
    int last_pos = 0;  // Índice del próximo lugar disponible en la página.
    int page_index;  // Índice de la página en la tabla hash.
    Page* linkedPage;  // Puntero a otra página en caso de rebalse (colisiones).

    // Constructor inicializa la página con ceros.
    Page() : linkedPage(nullptr) {
    std::fill(std::begin(page), std::end(page), 0);
    }

    // Imprime los elementos de la página y de cualquier página enlazada.
    void print() {
        for (size_t j = 0; j < PAGE_SIZE; ++j) {
            if(j%8==0){
                std::cout<<"\n";
            }
            std::cout << page[j] << " ";
        }
        std::cout << "\n";
        if (!(linkedPage==nullptr)) {
            std::cout<< "página enlazada" << "\n";
            linkedPage->print();
        }
    }

    // Retorna el puntero linkedPage
    Page* getLinkedPage(){
        return linkedPage;
    }

    // Crea y setea una nueva página enlazada
    void setLinkedPage(int p_index){
        linkedPage = new Page();
        linkedPage->initialize(p_index);
    }

    void initialize(int n) {
        page_index = n;
        std::fill(std::begin(page), std::end(page), 0);
    }

    // Calcula el costo en la búsqueda
    int cost(){
        if(linkedPage==nullptr){
            return 0;
        }
        return (1 + linkedPage->cost());
    }
};

class HashTable {
private:
    int p;                  // Número de páginas
    int t=0;
    std::vector<Page> pages; // Vector para almacenar las páginas
    long long total_io_cost = 0;   // Acumula el costo Total Real

public:
    // Constructor que inicializa la clase con el número de páginas
    HashTable(int num_pages) : p(num_pages), pages(num_pages) {
        if (num_pages <= 0) {
            throw std::invalid_argument("El número de páginas debe ser positivo.");
        }
        fill_page_index();
    }

    // Método para acceder a una página especifica
    Page& getPage(int index) {
        if (index < 0 || index >= p) {
            throw std::out_of_range("Índice de página fuera de rango.");
        }
        total_io_cost++;  // Costo de leer la página
        return pages[index];
    }

    // Asigna índices a cada página en la tabla.
    void fill_page_index(){
        for(int i=0;i<p;++i){
            Page& page = pages[i];
            page.page_index = i;
        }
    }

    // Método para obtener el número de páginas
    int getNumPages() const {
        return p;
    }

    int gett() const {
        return t;
    }

    // Método para obtener el costo Total real
    long long getTotalIOCost() const {
        return total_io_cost;
    }

    //Imprime todas las páginas
    void printHT() {
        for(int i=0; i<p;++i){
            std::cout<<"página n°: "<<i<<"\n";
            pages[i].print();
        }
    }

    // Expande la tabla agregando una nueva página.
    void expand(){
        Page newPage = Page();
        newPage.page_index = p;
        pages.push_back(newPage);
        p++;
        if (p == (1 << (t + 1))) {
            t++;
        }
        //printHT();
    }

    // Inserta un elemento x en la tabla, y expande si se supera el costo promedio MAX_COST.
    void insert(long long x){
        long long k = h(x) % (1LL << (t + 1));
        //std::cout<<"página donde debe ocurrir la inserción: "<<k<<" número de páginas actuales "<< p<< "\n";
        if (k < p){
            insertInPage(x, pages[k]);
        }
        else{
            insertInPage(x, pages[k-(1LL << t)]);
        }
        int sMC = searchMeanCost();
        if (sMC > MAX_COST){
            expand();
            redistribute();
        }
        //std::cout <<"se insertó\n";
        return;
    }

    // Calcula el costo promedio de búsqueda.
    int searchMeanCost(){
        int cost = 0;
        for(int i = 0; i < p; ++i){
            cost += pages[i].cost();
        }
        return (1 + cost / p);
    }

    void redistribute(){
        int index_to_expand = p - (1 << t); // Índice página no distribuida

        Page &oldPage = pages[index_to_expand];
        Page &newPage = pages.back(); // Nueva página creada en `expand`

        for (int i = 0; i < oldPage.last_pos; ++i) {
            long long value = oldPage.page[i];
            long long k = h(value) % (1LL << (t + 1));
            if (k != index_to_expand) {
                newPage.page[newPage.last_pos] = value;
                newPage.last_pos++;
                oldPage.page[i] = 0; // Marcar como movido
                total_io_cost++;
            }
            total_io_cost++;
        }
        // Compactar la página original
        oldPage.last_pos = 0;
        for (int i = 0; i < PAGE_SIZE; ++i) {
            if (oldPage.page[i] != 0) {
                oldPage.page[oldPage.last_pos++] = oldPage.page[i];
            }
        }
        return;
    }

    // Inserta un elemento en la página P. Si la página está llena, inserta en la página enlazada.
    void insertInPage(long long x, Page& P){
        for (int i = 0; i < P.last_pos; ++i){
            if (P.page[i] == x){
                return;
            }
        }
        //std::cout<<"ultimo insertado en pagina actual: "<<P.last_pos<<"\n";
        if (P.last_pos >= PAGE_SIZE) {
            if(P.linkedPage == nullptr){
                //std::cout<<"la página linkeada no ha sido iniciada\n";
                P.setLinkedPage(P.page_index);
            }
            insertInPage(x, *P.linkedPage);
            return;
        }
        P.page[P.last_pos] = x;
        P.last_pos++;
        total_io_cost++;
        return;
    }

    // Limpia la tabla hash
    void clean(){
        for(int i = 0; i < p; ++i){
            Page &page = pages[i];
            page.last_pos = 0;
            if(page.linkedPage != nullptr){
                delete page.linkedPage;
                page.linkedPage = nullptr;
            }
        }
        pages.clear();
        p = 1;
        t = 0;
        total_io_cost = 0;
        pages.resize(p);
        pages[0] = Page();
        fill_page_index();
    }
};

int main(){
    int num_pages = 1;
    HashTable hT(num_pages);
    std::ofstream file;
    file.open("costo_promedio.txt");
    long long maxnum = 4000LL;
    long long start = 10;
    for (long long i = start; i <= maxnum; ++i){
        std::cout << i << "\n";
        hT.insert(i);
        if (i%10LL == 0LL){
            //std::cout << "el costo real promedio es:
            file << i << " " << hT.getTotalIOCost() << "\n";
        }
    }
    file.close();
    //hT.printHT();
    std::cout << "el costo real promedio es: " << hT.getTotalIOCost()/(maxnum - start + 1) << "\n";

    return 0;
}