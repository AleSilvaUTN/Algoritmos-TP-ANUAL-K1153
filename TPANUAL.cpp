#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#define Xmax 200
#define Ymax 200

using namespace std;
struct Orden
{
unsigned int espera = 0;
bool soltarGranada1 = false;
bool soltarGranada2 = false;
bool ataqueKamikaze = false;
bool aterrizaje = false;
bool despegue = false;
int siguientex =0;
int siguientey= 0;
};
struct OrdenArchivo
{
int x =0;
int y =0;
unsigned int espera  = 0;
bool soltarGranada1 = false;
bool soltarGranada2 = false;
bool ataqueKamikaze = false;
bool aterrizaje = false;
bool despegue = false;
int siguientex =0;
int siguientey= 0;
};
// por sugerencia de la IA paso las memoria importantes como globales
   Orden memoria[Xmax][Ymax];
   bool coordenada_ocupada[Xmax][Ymax] = {false};
   Orden memoria_temporal[Xmax][Ymax];
   bool coord_ocupada[Xmax][Ymax] = {false};
bool Cargar_Archivo(Orden memoria[Xmax][Ymax],bool coordenada_ocupada[Xmax][Ymax],char r[256])
{
    FILE *archivo;
    OrdenArchivo datos;

    archivo = fopen(r,"rb");
    bool errorG1 = false,errorG2 = false, errorfina = false, errorDes = false, errorRep = false;
    int cantidad_de_despejes=0,veces_que_se_suelta_granada_1=0,veces_que_se_suelta_granada_2=0,cantidad_de_finalizaciones=0,recorrido=0;
    int dx, dy; // uso de aux como gancho para detectar el despegue
    if(!archivo)
    {
        cout<<"//"<<endl;
        return false;
    }
    // hago limpieza en la memoria temporal
for(int i = 0; i<Xmax ; i++)
{
    for(int j = 0; j<Ymax; j++)
        {
            memoria_temporal[i][j].despegue       = false;
            memoria_temporal[i][j].aterrizaje     = false;
            memoria_temporal[i][j].soltarGranada1 = false;
            memoria_temporal[i][j].soltarGranada2 = false;
            memoria_temporal[i][j].ataqueKamikaze = false;
            memoria_temporal[i][j].espera         = 0;
            memoria_temporal[i][j].siguientex     = 0;
            memoria_temporal[i][j].siguientey     = 0;
        }
}
// hago limpieza en la coordenada ocupada
for(int i = 0; i<Xmax ; i++)
{
    for(int j = 0; j<Ymax; j++)
        {
            coord_ocupada[i][j]      = false;
        }
}
    while(fread(&datos,sizeof(OrdenArchivo),1,archivo) == 1)
    {
        if(datos.despegue)
        {
            cantidad_de_despejes++;
            dx = datos.x;
            dy = datos.y;
        }
        if(datos.aterrizaje)
            cantidad_de_finalizaciones++;
        if(datos.soltarGranada1)
            veces_que_se_suelta_granada_1++;
        if(datos.soltarGranada2)
            veces_que_se_suelta_granada_2++;
        if(datos.ataqueKamikaze)
            cantidad_de_finalizaciones++;
    }
    fseek(archivo,0,SEEK_SET);
    while(fread(&datos,sizeof(OrdenArchivo),1,archivo) == 1)
    {
        if((datos.x >= Xmax || datos.x < 0) || (datos.y >= Ymax || datos.y < 0))
        {
            cout<<"coordenada => ("<<datos.x<<","<<datos.y<<") fuera de rango"<<endl;
            fclose(archivo);
            return false;
        }
        if(datos.siguientex >= Xmax || datos.siguientex < 0 || datos.siguientey >= Ymax || datos.siguientey < 0)
        {
            cout<<"coordenada siguiente de ("<<datos.x<<","<<datos.y<<") fuera de rango"<<endl;
            fclose(archivo);
            return false;
        }
        memoria_temporal[datos.x][datos.y].despegue       = datos.despegue;
        memoria_temporal[datos.x][datos.y].aterrizaje     = datos.aterrizaje;
        memoria_temporal[datos.x][datos.y].soltarGranada1 = datos.soltarGranada1;
        memoria_temporal[datos.x][datos.y].soltarGranada2 = datos.soltarGranada2;
        memoria_temporal[datos.x][datos.y].ataqueKamikaze = datos.ataqueKamikaze;
        memoria_temporal[datos.x][datos.y].espera         = datos.espera;
        memoria_temporal[datos.x][datos.y].siguientex     = datos.siguientex;
        memoria_temporal[datos.x][datos.y].siguientey     = datos.siguientey;
        //paso datos
        if(coord_ocupada[datos.x][datos.y])
        {
            cout<<"Error coordenada (x,y) ==> ("<<datos.x<<","<<datos.y<<")"<<endl
            <<"Se repite las coordenadas ingresadas"<<endl;
            errorRep = true;
        }else
        {
            coord_ocupada[datos.x][datos.y] = true;
            recorrido++;
        }
        // la idea con esto es que si x = 0 e y = 0, pueda visualizar si es parte del recorrido
    }
    //marco errores que se detecta rapido
    if(cantidad_de_despejes > 1)
    {
        cout<<"---ERROR CON DESPEGUE---"<<endl<<"Se realizo mas de un despegue"<<endl;
        cout<<"COORDENADAS DE DESPEGUES REALIZADOS "<<endl;
        for(int i = 0 ; i< Xmax ; i++)
          {
              for(int j = 0 ; j < Ymax ; j++)
              {
                  if(memoria_temporal[i][j].despegue)
                  cout<<" ("<<i<<","<<j<<") " ;
              }
          }
        cout<<endl;
        errorDes = true;
    }
    if(cantidad_de_despejes == 0)
    {
        cout<<"---ERROR CON DESPEGUE---"<<endl<<"No se realizo ningun despegue"<<endl;
        cout<<"COORDENADAS LEIADAS nota: No esta en orden "<<endl;
        for(int i = 0 ; i< Xmax ; i++)
          {
              for(int j = 0 ; j < Ymax ; j++)
              {
                  if(coord_ocupada[i][j])
                  cout<<" ("<<i<<","<<j<<")";
              }
          }
        cout<<endl;
        errorDes = true;
    }
    if(cantidad_de_finalizaciones > 1)
    {
        cout<<"---ERROR CON ATAQUE KAMIKAZE O ATERRIZAJE---"<<endl<<"Se intento finalizar mas de una vez "<<endl;
        cout<<"COORDENADAS CON INTENTO DE FINALIZAR "<<endl;
        cout<<"ATQ KAMIKAZE => ";
        for(int i = 0 ; i< Xmax ; i++)
          {
              for(int j = 0 ; j < Ymax ; j++)
              {
                  if(memoria_temporal[i][j].ataqueKamikaze)
                  cout<<"("<<i<<","<<j<<") " ;
              }
          }
        cout<<endl<<"ATERRIZAJE => ";
        for(int i = 0 ; i< Xmax ; i++)
          {
              for(int j = 0 ; j < Ymax ; j++)
              {
                  if(memoria_temporal[i][j].aterrizaje)
                  cout<<"("<<i<<","<<j<<") " ;
              }
          }
        cout<<endl;
        errorfina = true;
    }
    if(cantidad_de_finalizaciones == 0)
    {
        cout<<"---ERROR CON ATAQUE KAMIKAZE O ATERRIZAJE---"<<endl<<"El recorrido no tiene finalizacion"<<endl;
        cout<<"COORDENADAS LEIADAS nota: No esta en orden "<<endl;
        for(int i = 0 ; i< Xmax ; i++)
            {
                for(int j = 0 ; j < Ymax ; j++)
                {
                    if(coord_ocupada[i][j])
                    cout<<" ("<<i<<","<<j<<")";
                }
            }
            cout<<endl;
        errorfina = true;
    }
    if(veces_que_se_suelta_granada_1 >1 )
    {
        cout<<"---ERROR GRANADA 1---"<<endl<<"Se suelta la granada 1 mas de una vez"<<endl;
        cout<<"COORDENADAS DE LAS VECES QUE SE SUELTA LA GRANADA" <<endl;
        for(int i = 0 ; i< Xmax ; i++)
          {
              for(int j = 0 ; j < Ymax ; j++)
              {
                  if(memoria_temporal[i][j].soltarGranada1)
                  cout<<" ("<<i<<","<<j<<") " ;
              }
          }
        cout<<endl;
        errorG1 = true;
    }
    if(veces_que_se_suelta_granada_2 >1 )
    {
        cout<<"---ERROR GRANADA 2---"<<endl<<"Se suelta la granada 2 mas de una vez"<<endl;
        cout<<"COORDENADAS DE LAS VECES QUE SE SUELTA LA GRANADA"<<endl;
        for(int i = 0 ; i< Xmax ; i++)
          {
              for(int j = 0 ; j < Ymax ; j++)
              {
                  if(memoria_temporal[i][j].soltarGranada2)
                  cout<<" ("<<i<<","<<j<<") " ;
              }
          }
        cout<<endl;
        errorG2 = true;
    }
    if(errorDes || errorfina || errorG1 || errorG2 || errorRep)
    {
        fclose(archivo);
        return false;
    }

// fin primera parte
// declaro x y e rx ry que seran aux y movimientos para saber si hubo un recorrido continuo
int x,y,rX, rY;
x = dx;
y = dy;
bool finalizacion = false;
    for(int i = 0; i<recorrido ;i++)
        {
            if(memoria_temporal[x][y].despegue == true)
            {
                if(memoria_temporal[x][y].soltarGranada1 || memoria_temporal[x][y].soltarGranada2 || memoria_temporal[x][y].ataqueKamikaze)
                {
                    cout<<"Error coordenadas (x,y) ==> ("<<x<<","<<y<<")"<<endl
                    << "Hubo ataque en el despegue"<<endl;
                    fclose(archivo);
                    return false;
                }
            }
            if(memoria_temporal[x][y].ataqueKamikaze || memoria_temporal[x][y].aterrizaje)
            {
                    // verifico si se finaliza con ambos meotodos
                    if(memoria_temporal[x][y].ataqueKamikaze && memoria_temporal[x][y].aterrizaje)
                    {
                        cout<<"Error coordenadas (x,y)==> ("<<x<<","<<y<<")"<<endl;
                        cout<<"Intento de doble finalizacion"<<endl;
                        fclose(archivo);
                        return false;
                    }
                    // verifico si hay un siguente movimiento
                    if(memoria_temporal[x][y].siguientex > 0 || memoria_temporal[x][y].siguientey > 0)
                    {
                        cout<<"Error coordenadas(x,y)==> ("<<x<<","<<y<<")"<<endl
                        <<"Al intentar finalizar el dato de la cooordenada marca una coordenada siguente mayor a (0,0)"<<endl;
                    fclose(archivo);
                    return false;
                    }else
                    {
                        i = recorrido+1; // se fuerza el cierre del for
                        finalizacion = true;
                        }
            }else
            {
                // Se comprueba los movimientos diagonales
                rX=x - memoria_temporal[x][y].siguientex;
                rY=y - memoria_temporal[x][y].siguientey;
                if((rX == -1 && rY == -1)||(rX == -1  && rY == 1 )||(rX == 1 && rY == 1 )||(rX == 1 && rY == -1 )||(rX ==0 && rY==0))
                {
                    cout<<"Error coordenada (x,y) ==> ("<<x<<","<<y<<")"<<endl
                    <<"Movimiento diagonal"<<endl
                    <<"En caso de no encontrar las coordenadas pruebe con la coordenada anterior ("<<dx<<","<<dy<<")"<<endl;
                fclose(archivo);
                 return false;
                }
                // Se comprueba los movimientos que no sean continuos
                if((rX > 1 || rX < -1) || (rY > 1 || rY < -1))
                {
                    cout<<"Error coordenada (x,y) ==> ("<<x<<","<<y<<")"<<endl
                    <<"Movimiento no continuo"<<endl
                    <<"En caso de no encontrar las coordenadas pruebe con la coordenada anterior ("<<dx<<","<<dy<<")"<<endl;
                fclose(archivo);
                return false;
                }
                // uso rX y rY como auxiliar para las siguentes coordenadsa
                // uso dx y dy como valores anteriores por si las siguientes coordenadas no existen en el registro
                dx = x;
                dy = y;
                rX = memoria_temporal[x][y].siguientex;
                rY = memoria_temporal[x][y].siguientey;
                x = rX;
                y = rY;

          }

      }

if(finalizacion == false)
{
    cout<<"Se intento salir del FOR mediante un ciclo"<<endl
    <<"Corregir coordenadas ("<<x<<","<<y<<")"<<endl;
    fclose(archivo);
    return false;
}

// hago limpieza en la memoria
for(int i = 0; i<Xmax ; i++)
{
    for(int j = 0; j<Ymax; j++)
        {
            memoria[i][j].despegue       = false;
            memoria[i][j].aterrizaje     = false;
            memoria[i][j].soltarGranada1 = false;
            memoria[i][j].soltarGranada2 = false;
            memoria[i][j].ataqueKamikaze = false;
            memoria[i][j].espera         = 0;
            memoria[i][j].siguientex     = 0;
            memoria[i][j].siguientey     = 0;
        }
}
// limpio la memoria de coordenada ocupada que servira para las otras operaciones
for(int i= 0 ; i< Xmax ; i++)
{
    for(int j = 0 ; j < Ymax ; j++)
    {
        coordenada_ocupada[i][j] = false;
    }
}
      // pasaje de datos a la memoria;
fseek(archivo,0,SEEK_SET);
while(fread(&datos,sizeof(OrdenArchivo),1,archivo))
{
    memoria[datos.x][datos.y].ataqueKamikaze = datos.ataqueKamikaze;
    memoria[datos.x][datos.y].aterrizaje     = datos.aterrizaje;
    memoria[datos.x][datos.y].espera         = datos.espera;
    memoria[datos.x][datos.y].siguientex     = datos.siguientex;
    memoria[datos.x][datos.y].siguientey     = datos.siguientey;
    memoria[datos.x][datos.y].soltarGranada1 = datos.soltarGranada1;
    memoria[datos.x][datos.y].soltarGranada2 = datos.soltarGranada2;
    memoria[datos.x][datos.y].despegue       = datos.despegue;
}

// vuelvo a colocar con las coordenadas de esta operacion
for(int i= 0 ; i< Xmax ; i++)
{
    for(int j = 0 ; j < Ymax ; j++)
    {
        coordenada_ocupada[i][j] = coord_ocupada[i][j];
    }
}
fclose(archivo);
return true;

}

void mostrar_ataque(OrdenArchivo a)
{
        cout<<"===DATOS==="<<endl
        <<endl<<"Depegue              ==> "<<a.despegue
        <<endl<<"Aterrizaje           ==> "<<a.aterrizaje
        <<endl<<"Soltar granada       ==> "<<a.soltarGranada1
        <<endl<<"Soltar granada 2     ==> "<<a.soltarGranada2
        <<endl<<"Ataque Kamikaze      ==> "<<a.ataqueKamikaze
        <<endl<<"Esperar              ==> "<<a.espera
        <<endl<<"Siguente coordenadas ==> ("<<a.siguientex<<","<<a.siguientey<<")"
        <<endl<<"Presiona cualquier tecla para continuar"<<endl;
        getch();
        return;
}
void mostrar_ataquemem(Orden memoria[Xmax][Ymax],int x, int y)
{
 cout<<"===DATOS=="<<endl
        <<endl<<"Depegue              ==> "<<memoria[x][y].despegue
        <<endl<<"Aterrizaje           ==> "<<memoria[x][y].aterrizaje
        <<endl<<"Soltar granada       ==> "<<memoria[x][y].soltarGranada1
        <<endl<<"Soltar granada 2     ==> "<<memoria[x][y].soltarGranada2
        <<endl<<"Ataque Kamikaze      ==> "<<memoria[x][y].ataqueKamikaze
        <<endl<<"Esperar              ==> "<<memoria[x][y].espera
        <<endl<<"Siguente coordenadas ==> ("<<memoria[x][y].siguientex<<","<<memoria[x][y].siguientey<<")"<<endl;
}
void Mostrar_Ataque_Cargado(Orden memoria[Xmax][Ymax])
{
    int x,y,aux1,aux2;
    bool encontrado = false, fin_registro = false;
    while(!encontrado)
        {
            for(int i = 0; i < Xmax ; i++)
            {
                for(int j = 0; j < Ymax ; j++)
                {
                    if(memoria[i][j].despegue == true)
                    {
                        x = i;
                        y = j;
                        encontrado = true;
                        i = Xmax;
                        j = i;
                    }
                }
            }
            if(encontrado == false)
            {
                cout<<"No se encontro el despegue"<<endl;
                return;
            }
        }
        while(!fin_registro)
        {
          cout<<"======================================================================================================="<<endl<<endl;
          cout<<"Coordenada      ("<<x<<","<<y<<")"<<endl;
          mostrar_ataquemem(memoria,x,y);
          aux1 = memoria[x][y].siguientex;
          aux2 = memoria[x][y].siguientey;
          x = aux1;
          y = aux2;
          cout<<endl<<"======================================================================================================="<<endl<<endl;
          if(memoria[x][y].aterrizaje || memoria[x][y].ataqueKamikaze)
          {
          cout<<"======================================================================================================="<<endl;
          cout<<"Coordenada      ("<<x<<","<<y<<")"<<endl;
          mostrar_ataquemem(memoria,x,y);
          cout<<endl<<"======================================================================================================="<<endl;
              fin_registro = true;
          }
          cout<<"Ingrese una tecla para continuar a la siguente coordenada"<<endl;
          getch();
        }

}


void Crear_Un_Archivo_Nuevo(char ruta[256])
{
    FILE *Archivo_Nuevo;
    OrdenArchivo AtaqueNuevo, limpieza = {};
    int Xaux = 0 ,Yaux = 0,asignacion,opcion;
    unsigned int espera;
    bool no_finalizar = true,primer_ingreso = true,no_terminar = true;
    Archivo_Nuevo = fopen(ruta,"wb");
    if(!Archivo_Nuevo)
    {
        cout<<"No se pudo crear el nuevo archivo"<<endl;
        return;
    }

        do{
                if(primer_ingreso)
                {
                    cout<<"Ingrese coordenadas (x,y)"<<endl;
                    cin>>AtaqueNuevo.x>>AtaqueNuevo.y;
                }
                if((AtaqueNuevo.x < Xmax && AtaqueNuevo.x >= 0) && (AtaqueNuevo.y < Ymax && AtaqueNuevo.y >= 0))
                    {
                        primer_ingreso = false;
                do
                    {
                        mostrar_ataque(AtaqueNuevo);
                        cout<<endl<<"COORDENADA ACUTAL      ==> ("<<AtaqueNuevo.x<<","<<AtaqueNuevo.y<<")"<<endl;
                        cout<<"Que desea cambiar"
                        <<endl<<"Depegue              (1)"
                        <<endl<<"Aterrizaje           (2)"
                        <<endl<<"Soltar granada       (3)"
                        <<endl<<"Soltar granada 2     (4)"
                        <<endl<<"Ataque Kamikaze      (5)"
                        <<endl<<"Esperar              (6)"
                        <<endl<<"Siguente coordenadas (7)"
                        <<endl<<"Finalizar            (8)"<<endl;
                       opcion = getch();
                        no_terminar = true;
                switch(opcion)
                    {
                        case 49:
                        do
                            {
                                cout<<"despegue ==> True (T) o False (F) "<<endl;
                                asignacion = getch();
                                if(asignacion == 116 || asignacion == 84)    // t = 116 , T = 84
                                    AtaqueNuevo.despegue = true;
                                if(asignacion == 102 || asignacion == 70)    // f = 102 , F = 70
                                    AtaqueNuevo.despegue = false;
                            }while(asignacion != 116 && asignacion != 84 && asignacion != 102 && asignacion != 70);
                        break;
                        case 50:
                        do
                            {
                                cout<<"Aterrizaje ==> True (T) o False (F) "<<endl;
                                asignacion = getch();
                                if(asignacion == 116 || asignacion == 84)
                                    AtaqueNuevo.aterrizaje = true;
                                if(asignacion == 102 || asignacion == 70)
                                    AtaqueNuevo.aterrizaje = false;
                            }while(asignacion != 116 && asignacion != 84 && asignacion != 102 && asignacion != 70);
                        break;
                        case 51:
                        do
                            {
                                cout<<"Soltar Granada 1 ==> True (T) o False (F) "<<endl;\
                                asignacion = getch();
                                if(asignacion == 116 || asignacion == 84)
                                    AtaqueNuevo.soltarGranada1 = true;
                                if(asignacion == 102 || asignacion == 70)
                                    AtaqueNuevo.soltarGranada1 = false;
                            }while(asignacion != 116 && asignacion != 84 && asignacion != 102 && asignacion != 70);
                        break;
                        case 52:
                        do
                            {
                                cout<<"Soltar Granada 2 ==> True (T) o False (F) "<<endl;\
                                asignacion = getch();
                                if(asignacion == 116 || asignacion == 84)
                                    AtaqueNuevo.soltarGranada2 = true;
                                if(asignacion == 102 || asignacion == 70)
                                    AtaqueNuevo.soltarGranada2 = false;
                            }while(asignacion != 116 && asignacion != 84 && asignacion != 102 && asignacion != 70);
                        break;
                        case 53:
                        do
                            {
                                cout<<"Ataque Kamikaze ==> True (T) o False (F) "<<endl;
                        asignacion = getch();
                        if(asignacion == 116 || asignacion == 84)
                            AtaqueNuevo.ataqueKamikaze = true;
                        if(asignacion == 102 || asignacion == 70)
                            AtaqueNuevo.ataqueKamikaze = false;
                            }while(asignacion != 116 && asignacion != 84 && asignacion != 102 && asignacion != 70);
                        break;
                        case 54:
                            cout<<"Espera     ==> "<<endl;
                            cin>>espera;
                            AtaqueNuevo.espera = espera;
                        break;
                        case 55:
                            cout<<"Coordenadas siguentes (x,y)"<<endl<<"Coordenada actual ("
                            <<AtaqueNuevo.x<<","<<AtaqueNuevo.y<<")"<<endl;
                            cin>>AtaqueNuevo.siguientex>>AtaqueNuevo.siguientey;
                            Xaux = AtaqueNuevo.siguientex;
                            Yaux = AtaqueNuevo.siguientey;
                            break;
                        case 56:
                            no_terminar = false;
                        break;
                    }

                }while(no_terminar);
                do
                {
                    cout<<"Finalizar recorrido? Si (T) No(F)"<<endl;
                    asignacion = getch();
                    if(asignacion == 102 || asignacion == 70)
                    {
                        do
                        {
                            cout<<"Coordenadas nuevas (N) o continuar con las coordenadas siguentes elegidas (si es que eligo) (C)"<<endl;
                            opcion = getch();
                            if(opcion == 78 || opcion == 110)
                            {
                                primer_ingreso = true;
                                fwrite(&AtaqueNuevo,sizeof(OrdenArchivo),1,Archivo_Nuevo);
                                AtaqueNuevo = limpieza;
                            }
                            if(opcion == 67 || opcion == 99)
                            {
                                fwrite(&AtaqueNuevo,sizeof(OrdenArchivo),1,Archivo_Nuevo);
                                AtaqueNuevo = limpieza;
                                AtaqueNuevo.x = Xaux;
                                AtaqueNuevo.y = Yaux;
                                Xaux = 0; // por si no se usa el case 7 otra vez
                                Yaux = 0;
                            }

                        }while(opcion != 78 && opcion != 110 && opcion != 67 && opcion != 99);
                    }
                    if(asignacion == 116 || asignacion == 84 )
                    {
                        no_finalizar = false;
                        fwrite(&AtaqueNuevo,sizeof(OrdenArchivo),1,Archivo_Nuevo);
                        fclose(Archivo_Nuevo);
                    }
                }while(asignacion != 116 && asignacion != 84 && asignacion != 102 && asignacion != 70);

            }else
            {
                cout<<"Supera el limite de coordenadas"<<endl;
                no_finalizar = false;
                primer_ingreso = true;
                fclose(Archivo_Nuevo);
            }
            }while(no_finalizar);
            return;


}

void Corregir_Un_Registro_Del_Archivo(char ruta[256])
{
    FILE *archivo;
    int x,y,i,asignacion,opcion;
    OrdenArchivo registro;
    unsigned int espera;
    bool no_encontrado = true,continuar=true, no_terminar = true;\

    archivo = fopen(ruta,"rb+");
    if(!archivo)
    {
    cout<<"No se encontro la ruta"<<endl;
    return;
    }

    do
    {
        i = 0;
        fseek(archivo,0,SEEK_SET);
        cout<<"Ingrese coordenadas (x,y) ==> ";
        cin>>x>>y;
        while(fread(&registro,sizeof(OrdenArchivo),1,archivo) == 1)
        {
            if(registro.x == x && registro.y == y)
            {
                no_encontrado = false;

                do
                    {
                        mostrar_ataque(registro);
                        cout<<endl<<"COORDENADA ACUTAL      ==> ("<<registro.x<<","<<registro.y<<")"<<endl;
                        cout<<"Que desea cambiar"
                        <<endl<<"Depegue              (1)"
                        <<endl<<"Aterrizaje           (2)"
                        <<endl<<"Soltar granada       (3)"
                        <<endl<<"Soltar granada 2     (4)"
                        <<endl<<"Ataque Kamikaze      (5)"
                        <<endl<<"Esperar              (6)"
                        <<endl<<"Siguente coordenadas (7)"
                        <<endl<<"modificar x e y      (8)"
                        <<endl<<"Finalizar            (9)"<<endl;
                       opcion = getch();
                        no_terminar = true;
                switch(opcion)
                    {
                        case 49:
                        do
                            {
                                cout<<"despegue ==> True (T) o False (F) "<<endl;
                                asignacion = getch();
                                if(asignacion == 116 || asignacion == 84)    // t = 116 , T = 84
                                    registro.despegue = true;
                                if(asignacion == 102 || asignacion == 70)    // f = 102 , F = 70
                                    registro.despegue = false;
                            }while(asignacion != 116 && asignacion != 84 && asignacion != 102 && asignacion != 70);
                        break;
                        case 50:
                        do
                            {
                                cout<<"Aterrizaje ==> True (T) o False (F) "<<endl;
                                asignacion = getch();
                                if(asignacion == 116 || asignacion == 84)
                                    registro.aterrizaje = true;
                                if(asignacion == 102 || asignacion == 70)
                                    registro.aterrizaje = false;
                            }while(asignacion != 116 && asignacion != 84 && asignacion != 102 && asignacion != 70);
                        break;
                        case 51:
                        do
                            {
                                cout<<"Soltar Granada 1 ==> True (T) o False (F) "<<endl;
                                asignacion = getch();
                                if(asignacion == 116 || asignacion == 84)
                                    registro.soltarGranada1 = true;
                                if(asignacion == 102 || asignacion == 70)
                                    registro.soltarGranada1 = false;
                            }while(asignacion != 116 && asignacion != 84 && asignacion != 102 && asignacion != 70);
                        break;
                        case 52:
                        do
                            {
                                cout<<"Soltar Granada 2 ==> True (T) o False (F) "<<endl;
                                asignacion = getch();
                                if(asignacion == 116 || asignacion == 84)
                                    registro.soltarGranada2 = true;
                                if(asignacion == 102 || asignacion == 70)
                                    registro.soltarGranada2 = false;
                            }while(asignacion != 116 && asignacion != 84 && asignacion != 102 && asignacion != 70);
                        break;
                        case 53:
                        do
                            {
                                cout<<"Ataque Kamikaze ==> True (T) o False (F) "<<endl;
                                asignacion = getch();
                                if(asignacion == 116 || asignacion == 84)
                                registro.ataqueKamikaze = true;
                                if(asignacion == 102 || asignacion == 70)
                                registro.ataqueKamikaze = false;
                            }while(asignacion != 116 && asignacion != 84 && asignacion != 102 && asignacion != 70);
                        break;
                        case 54:
                            cout<<"Espera     ==> "<<endl;
                            cin>>espera;
                            registro.espera = espera;
                        break;
                        case 55:
                            cout<<"Coordenadas siguentes (x,y)"<<endl<<"Coordenada actual ("
                            <<x<<","<<y<<")"<<endl;
                            cin>>registro.siguientex>>registro.siguientey;
                            break;
                        case 56:
                            cout<<"A que coordenada quiere cambiar"<<endl;
                            cin>>registro.x;
                            cin>>registro.y;
                            break;
                        case 57:
                            fseek(archivo,i*sizeof(OrdenArchivo),SEEK_SET);
                            fwrite(&registro,sizeof(OrdenArchivo),1,archivo);
                            fflush(archivo);
                            no_terminar = false;
                        break;
                    }

                }while(no_terminar);
            }
        i++;
        }
        if(no_encontrado)
        {
            cout<<"No se encontro las coordenadas"<<endl;
        }
        no_encontrado = true;

        cout<<"Buscar otras coordenadas? "<<endl<<"SI (S) o NO (N)"<<endl;
        asignacion = getch();
        if(asignacion == 78  || asignacion == 110)
        {
            fclose(archivo);
            continuar = false;
        }
        if(asignacion == 83 || asignacion == 115)
        {
                        fseek(archivo,0,SEEK_SET);
                        i=0;
        }
    }while(continuar);


return;
}

void Corregir_Un_Registro_En_Memoria(Orden memoria[Xmax][Ymax], bool coordenada_ocupada[Xmax][Ymax])
{
    int x,y,selector;
    int asignacion;
    unsigned int esperar;
    bool no_finalizar = true, no_fin = true;
    do
    {
        cout<<"Ingrese coordenadas (x,y) ==> "<<endl;
        cin>>x>>y;
        if((x<Xmax && x >= 0)&&( y <Ymax && y >= 0))
        {
        if(coordenada_ocupada[x][y])
        {
        do
        {
            mostrar_ataquemem(memoria,x,y);
            cout<<"Presiona cualquier tecla para continuar"<<endl;
            getch();
            cout<<"Que desea cambiar"
            <<endl<<"Depegue          (1)"
            <<endl<<"Aterrizaje       (2)"
            <<endl<<"Soltar granada   (3)"
            <<endl<<"Soltar granada 2 (4)"
            <<endl<<"Ataque Kamikaze  (5)"
            <<endl<<"Esperar          (6)"
            <<endl<<"Siguiente x e y  (7)"
            <<endl<<"Finalizar        (8)"<<endl;
            selector = getch();

        switch(selector)
        {
        case 49:
            do
            {
                cout<<"Despegue        ==> TRUE (T) o FALSE (F)"<<endl;
                asignacion = getch();
                if(asignacion == 84 || asignacion == 116)
                    memoria[x][y].despegue = true;
                if(asignacion == 70 || asignacion == 102)
                    memoria[x][y].despegue = false;
            }while(asignacion != 84 && asignacion != 116 && asignacion !=70 && asignacion !=102);
            break;
        case 50:
            do
            {
                cout<<"Aterrizaje      ==> TRUE (T) o FALSE (F)"<<endl;
                asignacion = getch();
                if(asignacion == 84 || asignacion == 116)
                    memoria[x][y].aterrizaje = true;
                if(asignacion == 70 || asignacion == 102)
                    memoria[x][y].aterrizaje = false;
            }while(asignacion != 84 && asignacion != 116 && asignacion !=70 && asignacion !=102);
            break;
        case 51:
            do
            {
                cout<<"Soltar Granada 1 ==> TRUE (T) o FALSE (F)"<<endl;
                asignacion = getch();
                if(asignacion == 84 || asignacion == 116)
                    memoria[x][y].soltarGranada1 = true;
                if(asignacion == 70 || asignacion == 102)
                    memoria[x][y].soltarGranada1 = false;
            }while(asignacion != 84 && asignacion != 116 && asignacion !=70 && asignacion !=102);
            break;
        case 52:
            do
            {
                cout<<"Soltar Granada 2 ==> TRUE (T) o FALSE (F)"<<endl;
                asignacion = getch();
                if(asignacion == 84 || asignacion == 116)
                    memoria[x][y].soltarGranada2 = true;
                if(asignacion == 70 || asignacion == 102)
                    memoria[x][y].soltarGranada2 = false;
            }while(asignacion != 84 && asignacion != 116 && asignacion !=70 && asignacion !=102);
            break;
        case 53:
            do
            {
                cout<<"ataque kamikaze ==> TRUE (T) o FALSE (F)"<<endl;
                asignacion = getch();
                if(asignacion == 84 || asignacion == 116)
                    memoria[x][y].ataqueKamikaze = true;
                if(asignacion == 70 || asignacion == 102)
                    memoria[x][y].ataqueKamikaze = false;
            }while(asignacion != 84 && asignacion != 116 && asignacion !=70 && asignacion !=102);
            break;
        case 54:
             cout<<"Esperar ==> "<<endl;
            cin>>esperar;
                memoria[x][y].espera = esperar;
                break;
        case 55:
            cout<<"COORD Sx Sy actual => ("<<memoria[x][y].siguientex<<","<<memoria[x][y].siguientey<<")"<<endl;
            cout<<"Coordenada a reemplazar (x,y) => "<<endl;
            cin>>memoria[x][y].siguientex>>memoria[x][y].siguientey;
            break;
        case 56:
            no_finalizar = false;
            break;
        }
        }while(no_finalizar);
        }else
        {
            cout<<"No hay registros de esas coordenadas"<<endl;
        }
        }else
        {
            cout<<"coordenadas superan el limite"<<endl;
        }

        do
        {
        cout<<"Corregir otras coordenadas? Si (S) No (N)"<<endl;
        asignacion = getch();
        if(asignacion == 78 || asignacion == 110)
            no_fin = false;
        }while(asignacion != 115 && asignacion != 83 && asignacion != 78 && asignacion !=110);
    }while(no_fin);

}

void Guarda_Memoria_En_Un_Archivo_Nuevo(Orden memoria[Xmax][Ymax], char ruta[256])
{
    FILE *archivo;
    int x,y,X,Y,k = 0;
    bool fin= true,econtrado = false;
    OrdenArchivo guardado;
        archivo = fopen(ruta,"wb");
        if(archivo)
        {
            // busco el depegue para guardarlos en x e y
            for(int i = 0;i< Xmax ; i++)
            {
                for(int j =0; j<Ymax; j++)
                {
                    if(memoria[i][j].despegue == true)
                    {
                         x = i;
                         y = j;
                         i = Xmax;
                         j = Ymax;
                         econtrado = true; // con esto accedo al condicional if
                    }
                }
            }
            if(econtrado)
            {
            do
            {
                if( (k >= (Xmax*Ymax)) || x < 0 || x >= Xmax || y < 0 || y >= Ymax)
                {
                    cout<<"La trayectoria es invalida fuera de rango o ciclo sin fin"<<endl;
                    fclose(archivo); // visto en internet
                    return;
                }
                if(memoria[x][y].aterrizaje == true || memoria[x][y].ataqueKamikaze == true) // ultimo guardado al finalizar
                {
                    guardado.x              = x;
                    guardado.y              = y;
                    guardado.ataqueKamikaze = memoria[x][y].ataqueKamikaze;
                    guardado.despegue       = memoria[x][y].despegue;
                    guardado.aterrizaje     = memoria[x][y].aterrizaje;
                    guardado.soltarGranada1 = memoria[x][y].soltarGranada1;
                    guardado.soltarGranada2 = memoria[x][y].soltarGranada2;
                    guardado.espera         = memoria[x][y].espera;
                    guardado.siguientex     = memoria[x][y].siguientex;
                    guardado.siguientey     = memoria[x][y].siguientey;
                    X                       = memoria[x][y].siguientex;
                    Y                       = memoria[x][y].siguientey;
                    fwrite(&guardado,sizeof(OrdenArchivo),1,archivo);
                    fin = false; // pone en fin el do while
                    fclose(archivo);
                }else
                {
                    guardado.x              = x;
                    guardado.y              = y;
                    guardado.ataqueKamikaze = memoria[x][y].ataqueKamikaze;
                    guardado.despegue       = memoria[x][y].despegue;
                    guardado.aterrizaje     = memoria[x][y].aterrizaje;
                    guardado.soltarGranada1 = memoria[x][y].soltarGranada1;
                    guardado.soltarGranada2 = memoria[x][y].soltarGranada2;
                    guardado.espera         = memoria[x][y].espera;
                    guardado.siguientex     = memoria[x][y].siguientex;
                    guardado.siguientey     = memoria[x][y].siguientey;
                    X                       = memoria[x][y].siguientex;
                    Y                       = memoria[x][y].siguientey;
                    fwrite(&guardado,sizeof(OrdenArchivo),1,archivo);
                    // sigo a las siguente coordenadas
                    x                             = X;
                    y                             = Y;

                }
                k++;
            }while(fin);
            cout << "===HECHO==="<<endl;
            return;
        }else
        {
            cout<<"No se encontro el despegue (modificado en la operacion 5)"<<endl;
            fclose(archivo);
            return;
        }
            }else
            {
                cout<<"archivo no encontrado"<<endl;
                return;
            }

}

void Visualizar_Un_Archivo_De_Ataque_En_Html(Orden memoria[Xmax][Ymax],bool coordenada_ocupada[Xmax][Ymax], char nombre[256])
{
    FILE *archivo;
    char html[] = {".html"};
    strcpy(nombre,html);
    archivo = fopen(nombre,"w");
    if(!archivo)
    {
        cout<<"Error al crear el archivo"<<endl;
        return;
    }
    fprintf(archivo, "<html>\n");
    fprintf(archivo, "<head><title>Trayectoria del ataque</title></head>\n");
    fprintf(archivo, "<body>\n");
    fprintf(archivo, "<table border=1>\n");
    fprintf(archivo, "<tr>\n");
    fprintf(archivo, "<td>-</td>");
    for(int j = 0; j < Xmax; j++)
    {
        fprintf(archivo, "<td>%d</td>", j);
    }
    fprintf(archivo, "</tr>\n");

    for(int y = 0; y < Xmax ; y++)
    {
       fprintf(archivo, "<tr>\n");
       fprintf(archivo, "<td>%d</td>\n",y);
       for(int x = 0; x< Ymax; x++)
       {

           if(coordenada_ocupada[x][y])
           {
               fprintf(archivo, "<td>");
               if(memoria[x][y].despegue)
                fprintf(archivo, " D ");
               if(memoria[x][y].aterrizaje)
                fprintf(archivo, " A ");
               if(memoria[x][y].ataqueKamikaze)
                fprintf(archivo, " Ak ");
               if(memoria[x][y].soltarGranada1)
                fprintf(archivo, " Sg1");
               if(memoria[x][y].soltarGranada2)
                fprintf(archivo, " Sg2 ");
               if(memoria[x][y].espera > 0)
                fprintf(archivo, " E %d ",memoria[x][y].espera);
                // para este ultimo solo funciona cuando no hay movimeintos por parte de las anteriores condicionaless
               if(!(memoria[x][y].despegue || memoria[x][y].aterrizaje || memoria[x][y].ataqueKamikaze || memoria[x][y].soltarGranada1 || memoria[x][y].soltarGranada2 || memoria[x][y].espera!=0))
                {
                    if((y - memoria[x][y].siguientey) == -1)
                        fprintf(archivo, " | ");
                    if((y - memoria[x][y].siguientey) ==  1)
                        fprintf(archivo, "| ");
                    if((x - memoria[x][y].siguientex) ==  1)
                        fprintf(archivo, "<=");
                    if((x - memoria[x][y].siguientex) == -1)
                        fprintf(archivo, "=>");

                }
                fprintf(archivo, "</td>\n");
           }else
           {
               fprintf(archivo, "<td></td>");
           }


       }
       fprintf(archivo, "</tr>\n");
    }
    fprintf(archivo, "</table>\n");
    fprintf(archivo, "</body>\n");
    fprintf(archivo, "</html>\n");
    fclose(archivo);
}

int main()
{
    //Orden memoria[Xmax][Ymax];
  //  bool coordenada_ocupada[Xmax][Ymax] = {false};
    short opcion;
    bool CargarArchivo= false,fin = true;
    bool CargarAux = false;
    char ruta[256]="",nombre[256]="";

    do
    {
        cout<<"MENU"<<endl;
        cout
        <<"Cargar Archivo De Ataque En Memoria     (1)"<<endl
        <<"Mostrar Ataque Cargado                  (2)"<<endl
        <<"Crear Un Archivo De Ataque Nuevo        (3)"<<endl
        <<"Corregir Un Registro Del Archivo        (4)"<<endl
        <<"Corregir Un Registro En Memoria         (5)"<<endl
        <<"Guardar Memoria En Un Archivo Nuevo     (6)"<<endl
        <<"Visualizar Un Archivo De Ataque En HTML (7)"<<endl
        <<"Salir                                   (8)"<<endl;
        opcion = getch();
        switch(opcion)
        {
        case 49:
            cout<<"Ingrese la direccion de memoria"<<endl;
            cin>>ruta;
            CargarArchivo = Cargar_Archivo(memoria,coordenada_ocupada,ruta);
            if(CargarArchivo==true)
            {
                cout<<"===CARGADO==="<<endl;
                CargarAux = true;
                strcpy(nombre,ruta);
            }
            break;
        case 50:
            if(CargarArchivo || CargarAux)
            {
                Mostrar_Ataque_Cargado(memoria);
            }else
            cout<<"No hay datos en la memoria"<<endl;
            break;
        case 51:
            cout<<"Ingrese la direccion del archivo a crear"<<endl;
            cin>>ruta;
            Crear_Un_Archivo_Nuevo(ruta);
            break;
        case 52:
            cout<<"Ingrese la direccion del archivo a correir"<<endl;
            cin>>ruta;
            Corregir_Un_Registro_Del_Archivo(ruta);
            break;
        case 53:
            if(CargarArchivo || CargarAux)
            {
                Corregir_Un_Registro_En_Memoria(memoria,coordenada_ocupada);
            }else
            cout<<"No hay memoria en el drone"<<endl;
            break;
        case 54:
            if(CargarArchivo || CargarAux)
            {
                cout<<"Ingrese nombre de ruta para el archivo"<<endl;
                cin>>ruta;
                Guarda_Memoria_En_Un_Archivo_Nuevo(memoria,ruta);
            }else
            cout<<"No hay memoria en el dron"<<endl;
            break;
        case 55:
            if(CargarAux)
            {
                Visualizar_Un_Archivo_De_Ataque_En_Html(memoria,coordenada_ocupada,nombre);
                cout<<"===HECHO==="<<endl;
            }
            else
            {
                cout<<"No hay datos en la memoria"<<endl;
            }
            break;
        case 56:
            fin = false;
            break;
        }
    }while(fin);
    return 0;
}
