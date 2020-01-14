#include <iostream>
#include <fstream>

using namespace std;

/*
 * ON THE ROAD AGAIN (EXÀMEN GENER 2017, PART 2)
 */


struct Ciutat {
    string nom;
    int n_visites;
};

const int MAX_CIUTATS_VIATGER = 20;
struct Viatger {
    string nom;
    string ruta_ciutats[MAX_CIUTATS_VIATGER];
    int n_ciutats_visitades;
    int kmtotals;
};

const int MAX_CIUTATS = 200;
struct Ciutats {
    Ciutat t[MAX_CIUTATS];
    int n;
};

const int MAX_VIATGERS = 20;
struct Viatgers {
    Viatger t[MAX_VIATGERS];
    int n;
};

const string CIUTAT_ORIGEN = "Girona";

void inicialitzar_ciutats(Ciutats ciutats){
    ciutats.n = 0;
    for(int i = 0; i < MAX_CIUTATS; ++i){
        ciutats.t[i].nom = "";
        ciutats.t[i].n_visites = 0;
    }
}

void inicialitzar_viatgers(Viatgers viatgers){
    viatgers.n = 0;
    for(int i = 0; i < MAX_VIATGERS; ++i){
        viatgers.t[i].nom = "";
        viatgers.t[i].kmtotals = 0;
    }
}

int buscar_viatger(const Viatgers &viatgers, string nom){
    int i = 0;
    bool trobat = false;
    while(!trobat && i < viatgers.n){
        if(nom == viatgers.t[i].nom) trobat = true;
        else i++;
    }
    if(!trobat) i = -1;
    return i;
}

void buscar_ciutat(const Ciutats ciutats, string nom, bool &trobat, int &pos){
    int esquerra = 0, dreta = ciutats.n-1, mig = 0;
    trobat = false;
    while(esquerra <= dreta and not trobat){
        mig = (esquerra+dreta) / 2;
        if(nom == ciutats.t[mig].nom) trobat = true;
        else if(nom < ciutats.t[mig].nom) dreta = mig-1;
        else esquerra = mig+1;
    }
    if(trobat) pos = mig;
    else pos = esquerra;
}

void afegir_viatger(Viatgers &viatgers, string nom){
    Viatger viatger;
    viatger.nom = nom;
    viatger.ruta_ciutats[0] = CIUTAT_ORIGEN;
    viatger.n_ciutats_visitades = 1;
    viatger.kmtotals = 0;
    viatgers.t[viatgers.n] = viatger;
    viatgers.n++;
}

void afegir_ciutat(Ciutats &ciutats, string nom, int pos){
    for(int i = ciutats.n-1; i >= pos; --i){
        ciutats.t[i+1] = ciutats.t[i];
    }
    ciutats.t[pos].nom = nom;
    ciutats.t[pos].n_visites = 0;
    ciutats.n++;
}

void llegir_vacances(ifstream &f_in, Viatgers &viatgers, Ciutats &ciutats){
    while(!f_in.eof()){
        string nom;
        f_in >> nom;
        int pos_viatger = buscar_viatger(viatgers, nom);
        if(pos_viatger == -1){
            afegir_viatger(viatgers, nom);
        }
        Viatger &viatger = viatgers.t[buscar_viatger(viatgers, nom)];

        string ciutat;
        f_in >> ciutat;

        if(ciutat != CIUTAT_ORIGEN){
            bool existeix_ciutat;
            int pos_ciutat;
            buscar_ciutat(ciutats, ciutat, existeix_ciutat, pos_ciutat);
            if(!existeix_ciutat){
                afegir_ciutat(ciutats, ciutat, pos_ciutat);
            }
            ciutats.t[pos_ciutat].n_visites++;
        }
        viatger.ruta_ciutats[viatger.n_ciutats_visitades] = ciutat;
        viatger.n_ciutats_visitades++;

        int kms;
        f_in >> kms;
        viatger.kmtotals += kms;
    }
}

void ordenar_viatgers_per_kms(Viatgers &viatgers){
    for(int i = 0; i < viatgers.n-1; ++i){
        for(int j = 0; j < viatgers.n-1; ++j){
            if(viatgers.t[j].kmtotals < viatgers.t[j+1].kmtotals){
                Viatger vaux = viatgers.t[j];
                viatgers.t[j] = viatgers.t[j+1];
                viatgers.t[j+1] = vaux;
            }
        }
    }
}

void imprimir_info_viatgers(Viatgers &viatgers){
    ordenar_viatgers_per_kms(viatgers);
    for(int i = 0; i < viatgers.n; ++i){
        Viatger viatger = viatgers.t[i];
        cout << viatger.nom << " RECORREGUT TOTAL EN KM= " << viatger.kmtotals << endl;
        cout << "RUTA: ";
        for(int j = 0; j < viatger.n_ciutats_visitades; ++j){
            cout << viatger.ruta_ciutats[j] << " ";
        }
        cout << endl;
    }
}

void imprimir_info_ciutat_mes_visitada(const Ciutats ciutats){
    int pos_mes_visitada = 0;
    int n_visites = 0;
    for(int i = 0; i < ciutats.n; ++i){
        if(ciutats.t[i].n_visites > n_visites){
            pos_mes_visitada = i;
            n_visites = ciutats.t[i].n_visites;
        }
    }
    if (n_visites > 1) cout << "LA CIUTAT MES VISITADA ES: " << ciutats.t[pos_mes_visitada].nom << " AMB " << n_visites << " VISITES";
    else cout << "CAP CIUTAT HA ESTAT VISITADA PER MÉS D'UN AMIC";
}

int main(){

    Ciutats ciutats;
    Viatgers viatgers;

    inicialitzar_ciutats(ciutats);
    inicialitzar_viatgers(viatgers);

    ifstream f_in(
            "C:\\Users\\Bernat\\CLionProjects\\udg-mtp\\udg-mtp-examens1\\mtp1\\examens_anteriors\\2017\\normal\\vacances.txt");

    llegir_vacances(f_in, viatgers, ciutats);

    imprimir_info_viatgers(viatgers);

    cout << endl;
    imprimir_info_ciutat_mes_visitada(ciutats);

    return 0;
}