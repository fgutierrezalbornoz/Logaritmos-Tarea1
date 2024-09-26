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

long long h(int y){
    long long upper = (1LL << 24) - 1;
    long long num = rand() % upper;
    return num;
}

class Page {
public:
    long long page[PAGE_SIZE];
    int last_pos = 0;
    int page_index;
    long long linked_page[PAGE_SIZE];
    Page() {
        std::fill(std::begin(page), std::end(page), 0);
    }
    void print(){
        for (size_t j = 0; j < PAGE_SIZE; ++j) {
            if(j%8==0){
                std::cout<<"\n";
            }
            std::cout << page[j] << " ";
        }
        std::cout << "\n";
        }
};

class HashTable {
private:
    int p;                  // Número de páginas
    int t=0;
    std::vector<Page> pages; // Vector para almacenar las páginas

public:
    // Constructor que inicializa la clase con el número de páginas
    HashTable(int num_pages) : p(num_pages), pages(num_pages) {
        if (num_pages <= 0) {
            throw std::invalid_argument("El número de páginas debe ser positivo.");
        }
        fill_page_index();
    }

    // Método para acceder a una página específica
    Page& getPage(int index) {
        if (index < 0 || index >= p) {
            throw std::out_of_range("Índice de página fuera de rango.");
        }
        return pages[index];
    }
    void fill_page_index(){
        for(int i=0;i<p;++i){
            Page& page = getPage(i);
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
    void printHT(){
        for(int i=0; i<p;++i){
            std::cout<<"página n°: "<<i<<"\n";
            pages[i].print();
            std::cout<<"\n";
        }
    }
    void expand(){
        Page newPage = Page();
        newPage.page_index = p;
        p++;
        pages.push_back(newPage);
        //printHT();
    }

    void insert(long long x){
        long long k = h(x) % (1LL << t);
        //std::cout<<"página donde debe ocurrir la inserción: "<<k<<" número de páginas actuales "<< p<< "\n";
        if (k<p){
            //insertar en pagina k
            insertInPage(x, pages[k]);

        }
        else{
            //insertar en la pagina k - 2^t
            insertInPage(x, pages[k-(1LL << t)]);
        }
        //std::cout <<"se insertó\n";
        return;
    }
    void insertInPage(long long x, Page& P){
        //std::cout<<"ultimo insertado en pagina actual: "<<P.last_pos<<"\n";
        if (P.last_pos >= PAGE_SIZE) {
            int p_index = P.page_index;
            if(!(p>=(1LL<<(p_index+1)))){
                expand();
            }
            //std::cout<<"hay "<<p<<" páginas \n";
            Page& P_ = getPage(1LL<<(p_index+1)-1);
            insertInPage(x, P_);
            //std::cerr << "Error: El array está lleno, no se puede insertar más elementos." << std::endl;
        }
        P.page[P.last_pos] = x;
        P.last_pos++;
        //P.print();
        return;
    }

};







int main(){
    int num_pages = 1;
    HashTable hT(num_pages);

     /* for (int page_index = 0; page_index < num_pages; ++page_index) {
            Page& pagina = archivo.getPage(page_index);
            for (size_t i = 0; i < PAGE_SIZE; ++i) {
                pagina.page[i] = page_index * PAGE_SIZE + i; // Ejemplo de inicialización
            }
        } */
    
    for (int i=0;i<134;++i){
        hT.insert(i);
    }
    hT.printHT();

    /* for (int page_index = 0; page_index < num_pages; ++page_index) {
            std::cout << "Valores en la página " << page_index << ":\n";
            Page& pagina = archivo.getPage(page_index);
            for (size_t i = 0; i < PAGE_SIZE; ++i) {
                std::cout << pagina.page[i] << " ";
            }
            std::cout << "\n";
        } */
    return 0;

}