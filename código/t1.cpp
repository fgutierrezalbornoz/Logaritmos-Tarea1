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
constexpr size_t MAX_COST = 3;

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
    Page* linkedPage;

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
        if (!(linkedPage==nullptr)) {
                std::cout<< "página enlazada" << "\n";
                linkedPage->print();
            }
        }
    Page* getLinkedPage(){
        return linkedPage;
    }

    void setLinkedPage(int p_index){
        linkedPage = new Page();
        linkedPage->initialize(p_index);
    }

    void initialize(int n) {
        page_index = n;
        std::fill(std::begin(page), std::end(page), 0);
    }
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
    void printHT() {
        for(int i=0; i<p;++i){
            std::cout<<"página n°: "<<i<<"\n";
            pages[i].print();
        }
    }
    /* void printHT(){
        for(int i=0; i<p;++i){
            std::cout<<"página n°: "<<i<<"\n";
            pages[i].print();
            std::cout<<"\n";
        }
    } */
    void expand(){
        Page newPage = Page();
        newPage.page_index = p;
        p++;
        pages.push_back(newPage);
        //printHT();
    }
    void redistribute(){
        //HashTable hTCopy = malloc() 
        return;
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
        int sMC = searchMeanCost();
        if (sMC>MAX_COST){
            expand();
            redistribute();
        }
        //std::cout <<"se insertó\n";
        return;
    }
    int searchMeanCost(){
        int cost = 0;
        for(int i=0;i<p;++i){
            cost+=pages[i].cost();
        }
        return (1 + cost/p);
    }
    void insertInPage(long long x, Page& P){
        //std::cout<<"ultimo insertado en pagina actual: "<<P.last_pos<<"\n";
        if (P.last_pos >= PAGE_SIZE) {
            if(P.linkedPage==nullptr){
                //std::cout<<"la página linkeada no ha sido iniciada\n";
                P.setLinkedPage(P.page_index);
            }
            insertInPage(x, *P.linkedPage);
            return;
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
    
    for (int i=0;i<257;++i){
        hT.insert(i);
        if(i==127){
            std::cout<<"el costo promedio es: "<< hT.searchMeanCost()<<"\n";
        }
        if(i==200){
            std::cout<<"el costo promedio es: "<< hT.searchMeanCost()<<"\n";
        }
    }
    hT.printHT();
    std::cout<<"el costo promedio es: "<< hT.searchMeanCost()<<"\n";

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