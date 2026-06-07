#include "raylib.h"
#include <cstring>
#include <stdio.h>
#include <fstream>
#include <ctime>
#include <cstdlib>
#define TAM 25
#define EMAIL 50
#define MAX 100
enum Pantalla{ MENU, AGREGAR, VER_CONTACTO};
enum TipoLlamada{ENTRADA,SALIENTE,PERDIDA};

struct Fecha{
    int dia;
    char mes[TAM];
    int anio;
};
struct Hora{
    int hora;
    int min;
};
struct Duracion{
    int hora;
    int min;
    int seg;
};
struct NodoLlamada{
    Fecha fecha;
    Hora hora;
    Duracion duracion;
    TipoLlamada tipo;
    NodoLlamada* sig;
};
struct Nombre{
    char nombre[TAM];
    char apellido[TAM];
};
struct Contacto{
    int id;
    Nombre nCompleto;
    char telefono[11];
    char email[EMAIL];
    NodoLlamada* historial;

};
struct GuardarContacto{
    int id;
    Nombre nCompleto;
    char telefono[11];
    char email[EMAIL];
};

NodoLlamada *generarHistorialAletorio(int cant);

int buscarContactoPorID(Contacto contactos[],int totalContactos,char busqueda[]);


int main(){
    srand(time(NULL));
    char nombreCompleto[60];
    char textoID[TAM];
    Contacto contactos[MAX];
    int contactoSeleccionado=0;

    FILE *archBin;
    archBin=fopen("contactos.dat", "rb");
    int totalContactos=0;
    if(archBin!=NULL){
        GuardarContacto temp;
        while(fread(&temp, sizeof(GuardarContacto),1,archBin)==1){
            contactos[totalContactos].id=temp.id;
            strcpy(contactos[totalContactos].nCompleto.nombre,temp.nCompleto.nombre);
            strcpy(contactos[totalContactos].nCompleto.apellido,temp.nCompleto.apellido);
            strcpy(contactos[totalContactos].telefono,temp.telefono);
            strcpy(contactos[totalContactos].email,temp.email);
            contactos[totalContactos].historial=generarHistorialAletorio(5);
            totalContactos++;
        }
        fclose(archBin);
    }

    Pantalla pantallaActual=MENU;
    
    InitWindow(1280,720,"Agenda telefonica");
    // ancho, alto, nombre de la ventana
    SetTargetFPS(60);
    // velocidad de actualizacionn de la ventana
    Font fuente=LoadFont("C:/TrabVisual/AgendaTelefonica/fuentes/Roboto-VariableFont_wdth,wght.ttf");
    //COLORES
    Color fondo={240, 240, 240, 255};
    Color rosaPastel1={249, 218, 240, 150};
    Color rosaPastel2={245,203,233,255};
    Color colorElegido=PINK;
    Color colorTarjeta;
    Color colorRegresar; 
    Color colorHistorial;
    
    char inputID[7]="";
    char inputNombre[TAM]="";
    char inputApellido[TAM]="";
    char inputTelefono[11]="";
    char inputEmail[EMAIL]="";
    char inputbusquedaID[7]="";
    int letrasBusqueda=0;
    int letrasID=0;
    int letrasNombre=0;
    int letrasApellido=0;
    int letrasTelefono=0;
    int letrasEmail=0;
    int campoActivo=0; 
    bool existeID=false;
    bool mostrarErrorID=false;
    bool barraBusquedaActiva=false;
    float tempError=0;
    while (!WindowShouldClose()){
        
        Vector2 mouse=GetMousePosition();
        Rectangle botonAgregar= {1120,80,90,110};
        if(CheckCollisionPointRec(mouse, botonAgregar)){
            colorElegido=RED;
        } else {
            colorElegido=PINK;
        }
        if(CheckCollisionPointRec(mouse, botonAgregar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
           pantallaActual=AGREGAR;
       }
        
        BeginDrawing(); //comienza el dibujo de la pantalla
        ClearBackground(fondo); //Limpia la pantalla y coloca el fondo

        if(pantallaActual==MENU){
            // BARRA DE BUSQUEDA
            DrawRectangle(85,75, 350, 60, LIGHTGRAY); //SOMBRA DE LA BARRA
            DrawRectangle(80,70,350,60,WHITE); //RECTANGULO PRINCIPAL DE LA BARRA
            Rectangle recBusqueda={80,70,350,60};
            if (CheckCollisionPointRec(mouse,recBusqueda) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                barraBusquedaActiva= true;
            }
            if(barraBusquedaActiva){
                int tecla=GetCharPressed();
                while(tecla>0){
                    if(letrasBusqueda<6 && tecla>='0' && tecla <='9'){
                        inputbusquedaID[letrasBusqueda]=(char)tecla;
                        letrasBusqueda++;
                        inputbusquedaID[letrasBusqueda]='\0';
                    }
                    tecla=GetCharPressed();
                }
                //eliminiar texto
                if(IsKeyPressed(KEY_BACKSPACE)&&letrasBusqueda>0){
                    letrasBusqueda--;
                    inputbusquedaID[letrasBusqueda]='\0';
                }
            }
            
            DrawCircle(102,100,15,PINK);
            if(strlen(inputbusquedaID)==0){
                DrawTextEx(fuente,"Busqueda por ID",(Vector2) {140, 88}, 30, 2, BLACK);
            } else{
                DrawTextEx(fuente,inputbusquedaID,(Vector2){140,88},30,2,BLACK);
            }
            int resultadoBusqueda=-1;
            if(strlen(inputbusquedaID)>0){
                resultadoBusqueda=buscarContactoPorID(contactos,totalContactos,inputbusquedaID);
            }
            
            
            for(int i=0;i<totalContactos ;i++){
                if(resultadoBusqueda!=-1 && i!=resultadoBusqueda){
                    continue;
                }
                float y=180+(i*85);
                Rectangle tarjeta={80, (float)y,500,60};
                if(i%2==0){
                    colorTarjeta=rosaPastel1;
                } else{
                    colorTarjeta=rosaPastel2;
                }
                if(CheckCollisionPointRec(mouse, tarjeta)){
                    colorTarjeta=PINK;
                    if(CheckCollisionPointRec(mouse, tarjeta)&& IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        contactoSeleccionado=i;
                        pantallaActual=VER_CONTACTO;
                    }
                }
                // TARJETAS
                DrawRectangleRounded(tarjeta, 0.3,10, colorTarjeta); 
                // 0.3 = redondeo de esquinas
                // 10 = suavizado
                //2= GROSOR DE LINEA
                DrawRectangleRoundedLines(tarjeta, 0.3, 10, DARKGRAY);

                snprintf(nombreCompleto, sizeof(nombreCompleto),"%s %s", contactos[i].nCompleto.nombre,contactos[i].nCompleto.apellido);
                DrawTextEx(fuente, nombreCompleto,(Vector2){120, (float)(y+17)}, 30, 0, BLACK);
                
            }
            
            //BTON DE AGREGAR
            DrawRectangleRoundedLines(botonAgregar, 0.4,10, colorElegido);

            //SIMBOLO +
            DrawText("+", 1152,110,50,BLACK);
          
        }else if(pantallaActual==AGREGAR){
            DrawTextEx(fuente, "AGREGAR CONTACTO", (Vector2){450,35},40,2,PINK);
            Rectangle tarjetaAgregar={180,90,900,560};
            DrawRectangleRounded(tarjetaAgregar,0.5,10,fondo);
            DrawRectangleRoundedLinesEx(tarjetaAgregar,0.05,10,3,rosaPastel2);

            Rectangle espacioID={400,170,500,40};
            DrawRectangleRounded(espacioID,0.3,10,WHITE);
            DrawTextEx(fuente,"ID (6 digitos): ", (Vector2){400,130},30,2,BLACK);
            if(CheckCollisionPointRec(mouse,espacioID)&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                campoActivo=1;//activado
            }
            Color bordeID=rosaPastel2;
            if(campoActivo==1){
                bordeID=PINK;
            }
            DrawRectangleRoundedLines(espacioID,0.3,10,bordeID);
            DrawTextEx(fuente, inputID,(Vector2){410,178},25,2,BLACK);

            Rectangle espacioNombre={400,260,500,40};
            DrawTextEx(fuente,"Nombre: ", (Vector2){400,220},30,2,BLACK);
            DrawRectangleRounded(espacioNombre,0.3,10,WHITE);
            if(CheckCollisionPointRec(mouse,espacioNombre)&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                campoActivo=2;//activado
            }
            Color bordeNombre=rosaPastel2;
            if(campoActivo==2){
                bordeNombre=PINK;
            }
            DrawTextEx(fuente, inputNombre,(Vector2){410,268},25,2,BLACK);
            DrawRectangleRoundedLines(espacioNombre,0.3,10,bordeNombre);


            Rectangle espacioApellido={400,350,500,40};
            DrawTextEx(fuente,"Apellido: ", (Vector2){400,310},30,2,BLACK);
            DrawRectangleRounded(espacioApellido,0.3,10,WHITE);
            if(CheckCollisionPointRec(mouse,espacioApellido)&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                campoActivo=3;//activado
            }
            Color bordeApellido=rosaPastel2;
            if(campoActivo==3){
                bordeApellido=PINK;
            }
            DrawTextEx(fuente, inputApellido,(Vector2){410,358},25,2,BLACK);
            DrawRectangleRoundedLines(espacioApellido,0.3,10, bordeApellido);

            Rectangle espacioTelefono={400,440,500,40};
            DrawTextEx(fuente,"Telefono (10 digitos): ", (Vector2){400,400},30,2,BLACK);
            DrawRectangleRounded(espacioTelefono,0.3,10,WHITE);
            if(CheckCollisionPointRec(mouse,espacioTelefono)&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                campoActivo=4;//activado
            }
            Color bordeTelefono=rosaPastel2;
            if(campoActivo==4){
                bordeTelefono=PINK;
            }
            DrawTextEx(fuente, inputTelefono,(Vector2){410,448},25,2,BLACK);
            DrawRectangleRoundedLines(espacioTelefono,0.3,10, bordeTelefono);

            Rectangle espacioEmail={400,530,500,40};
            DrawTextEx(fuente,"Email: ", (Vector2){400,490},30,2,BLACK);
            DrawRectangleRounded(espacioEmail,0.3,10,WHITE);
            if(CheckCollisionPointRec(mouse,espacioEmail)&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                campoActivo=5;//activado
            }
            Color bordeEmail=rosaPastel2;
            if(campoActivo==5){
                bordeEmail=PINK;
            }
            DrawTextEx(fuente, inputEmail,(Vector2){410,538},25,2,BLACK);
            DrawRectangleRoundedLines(espacioEmail,0.3,10, bordeEmail);

            if(IsKeyPressed(KEY_TAB)){
                campoActivo++;
                if(campoActivo>5){
                    campoActivo=1;
                }
            }
            existeID=false;
            for(int i=0; i<totalContactos;i++){
                if(contactos[i].id==atoi(inputID)){
                    existeID=true;
                    break;
                }
            }

            //Boton regresar
            Rectangle btnVolveAgregarr={430,590,160,50};
            Color colorRegresar=rosaPastel2;
            if(CheckCollisionPointRec(mouse, btnVolveAgregarr)){
                colorRegresar=PINK;
                if(CheckCollisionPointRec(mouse, btnVolveAgregarr)&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    pantallaActual=MENU;
                }
            }
            DrawRectangleRounded(btnVolveAgregarr,0.3,10, colorRegresar);
            DrawTextEx(fuente, "Regresar", (Vector2){462,600},20,2,BLACK);


            //boton guardar
            Rectangle btnGuardar={645,592,160,50};
            Color colorGuardar=rosaPastel2;
            if(CheckCollisionPointRec(mouse,btnGuardar)){
                colorGuardar=PINK;
                if(CheckCollisionPointRec(mouse,btnGuardar)&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    FILE *archBin;
                    archBin=fopen("contactos.dat","ab");
                    
                    Contacto nuevo;
                    GuardarContacto temp;
                    if(archBin!=NULL && !existeID){
                        nuevo.id=atoi(inputID);
                        strcpy(nuevo.nCompleto.nombre,inputNombre);
                        strcpy(nuevo.nCompleto.apellido,inputApellido);
                        strcpy(nuevo.telefono,inputTelefono);
                        strcpy(nuevo.email,inputEmail);
                        nuevo.historial=generarHistorialAletorio(5);

                        temp.id=nuevo.id;
                        strcpy(temp.nCompleto.nombre,nuevo.nCompleto.nombre);
                        strcpy(temp.nCompleto.apellido, nuevo.nCompleto.apellido);
                        strcpy(temp.telefono, nuevo.telefono);
                        strcpy(temp.email,nuevo.email);
                        fwrite(&temp, sizeof(GuardarContacto),1,archBin);
                        contactos[totalContactos]=nuevo;
                        totalContactos++;

                        fclose(archBin);
                        strcpy(inputID,"");
                        strcpy(inputNombre,"");
                        strcpy(inputApellido,"");
                        strcpy(inputTelefono,"");
                        strcpy(inputEmail,"");

                        letrasID=0;
                        letrasNombre=0;
                        letrasApellido=0;
                        letrasTelefono=0;
                        letrasEmail=0;
                    } else if(existeID){
                        mostrarErrorID=true;
                        tempError=1.0f;
                    }
                }
            }
            DrawRectangleRounded(btnGuardar,0.3,10,colorGuardar);
            DrawTextEx(fuente,"Guardar",(Vector2){682,602},20,2,BLACK);
            if(mostrarErrorID){
                Rectangle alerta={470,250,340,120};
                // sombra
                DrawRectangleRounded(
                    (Rectangle){475,255,340,120},0.2,10,fondo);
                // ventana principal
                DrawRectangleRounded(alerta,0.2,10,WHITE);

                // borde
                DrawRectangleRoundedLinesEx(alerta,0.2,10,3,PINK);

                // titulo
                DrawTextEx(fuente,"ERROR",(Vector2){590,270},28,2,RED);

                // mensaje
                DrawTextEx(fuente,"El ID ya existe",(Vector2){562,320},24,2,BLACK);

                tempError -= GetFrameTime();

                if(tempError<=0){
                    mostrarErrorID=false;
                }

            }

            int tecla=GetCharPressed();
            while(tecla>0){
                if(tecla==9){
                    tecla=GetCharPressed();
                    continue;
                }
                if(campoActivo==1 && letrasID<6 && tecla>='0' && tecla<='9'){
                    inputID[letrasID]=(char)tecla;
                    letrasID++;
                    inputID[letrasID]='\0';
                } else if(campoActivo==2 && letrasNombre<24){
                    inputNombre[letrasNombre]=(char)tecla;
                    letrasNombre++;
                    inputNombre[letrasNombre]='\0';
                } else if(campoActivo==3 && letrasApellido<24){
                    inputApellido[letrasApellido]=(char)tecla;
                    letrasApellido++;
                    inputApellido[letrasApellido]='\0';
                } else if(campoActivo==4 && letrasTelefono<10 && tecla>='0' && tecla<='9'){
                    inputTelefono[letrasTelefono]=(char)tecla;
                    letrasTelefono++;
                    inputTelefono[letrasTelefono]='\0';
                } else if(campoActivo==5 && letrasEmail<49){
                    inputEmail[letrasEmail]=(char)tecla;
                    letrasEmail++;
                    inputEmail[letrasEmail]='\0';
                }
                tecla=GetCharPressed();
            }

            //BACKSPACE
            if(IsKeyPressed(KEY_BACKSPACE)){
                if(campoActivo==1&&letrasID>0){
                    letrasID--;
                    inputID[letrasID]='\0';
                } else if(campoActivo==2 && letrasNombre>0){
                    letrasNombre--;
                    inputNombre[letrasNombre]='\0';
                } else if(campoActivo==3 && letrasApellido>0){
                    letrasApellido--;
                    inputApellido[letrasApellido]='\0';
                } else if(campoActivo==4 &&letrasTelefono>0){
                    letrasTelefono--;
                    inputTelefono[letrasTelefono]='\0';
                } else if(campoActivo==5 && letrasEmail>0){
                    letrasEmail--;
                    inputEmail[letrasEmail]='\0';
                }
            }
        } else if(pantallaActual==VER_CONTACTO){
            Rectangle tarjetaGrande={150,120,980,450};
            DrawRectangleRoundedLines(tarjetaGrande, 0.2,10,PINK);

            snprintf(nombreCompleto,sizeof(nombreCompleto), "%s %s", contactos[contactoSeleccionado].nCompleto.nombre, contactos[contactoSeleccionado].nCompleto.apellido);
            DrawTextEx(fuente, nombreCompleto,(Vector2){225,200},40,2,BLACK);
            snprintf(textoID, sizeof(textoID), "%d", contactos[contactoSeleccionado].id);            
            DrawTextEx(fuente, textoID,(Vector2){220,280},30,2,BLACK);
            DrawTextEx(fuente, contactos[contactoSeleccionado].telefono, (Vector2){220,340}, 30, 2, BLACK);
            DrawTextEx(fuente, contactos[contactoSeleccionado].email, (Vector2){220,400}, 30, 2, BLACK);
            NodoLlamada* aux=contactos[contactoSeleccionado].historial;
            int y=260;
            while (aux!=NULL)
            {
                char texto[200];
                char tipoTexto[20];
                if (aux->tipo==ENTRADA)
                {
                    strcpy(tipoTexto,"Entrante");
                }else if(aux->tipo==SALIENTE){
                    strcpy(tipoTexto,"Saliente");
                }else{
                    strcpy(tipoTexto,"Perdida");
                }


                if(aux->tipo==SALIENTE){
                    colorHistorial={99,181,248,200};
                } else if(aux->tipo==ENTRADA){
                    colorHistorial={11,139,244,200};
                } else if(aux->tipo==PERDIDA){
                    colorHistorial={9,114,200,200};
                }
                
                sprintf(texto,"%s | %02d:%02d | %02d/%s/%d | %02d:%02d:%02d",tipoTexto,aux->hora.hora,aux->hora.min,aux->fecha.dia,aux->fecha.mes,aux->fecha.anio,aux->duracion.hora,aux->duracion.min,aux->duracion.seg);
                DrawTextEx(fuente,texto,(Vector2){660,(float)y},20,2,colorHistorial);
                y+=30;
                aux=aux->sig;
                
            }
            

            //boton regresar
            Rectangle botonVolver={430,590,160,50};
            Color colorRegresar=rosaPastel2;
            if(CheckCollisionPointRec(mouse, botonVolver)){
                colorRegresar=PINK;
                if(CheckCollisionPointRec(mouse, botonVolver)&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    pantallaActual=MENU;
                }
            }
            DrawRectangleRounded(botonVolver,0.3,10, colorRegresar);
            DrawTextEx(fuente, "Regresar", (Vector2){469,602},20,2,BLACK);
         //Boton eliminar
        Rectangle btnEliminar={650,590,160,50};
        Color colorEleminar=rosaPastel2;
        if (CheckCollisionPointRec(mouse, btnEliminar))
        {
            colorEleminar=PINK;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                NodoLlamada *aux=contactos[contactoSeleccionado].historial;
                while (aux!=NULL)
                {
                    NodoLlamada* borrar=aux;
                    aux=aux->sig;
                    delete borrar;
                }
                for(int i=contactoSeleccionado; i <totalContactos-1; i++)
                {
                    contactos[i]=contactos[i+1];
                }
                totalContactos--;
                FILE*archBin;
                archBin=fopen("contactos.dat","wb");
                if (archBin!=NULL)
                {
                    GuardarContacto temp;
                    for (int i = 0; i < totalContactos; i++)
                    {
                        temp.id=contactos[i].id;
                        strcpy(temp.nCompleto.nombre,contactos[i].nCompleto.nombre);
                        strcpy(temp.nCompleto.apellido,contactos[i].nCompleto.apellido);
                        strcpy(temp.telefono,contactos[i].telefono);
                        strcpy(temp.email,contactos[i].email);
                        fwrite(&temp,sizeof(GuardarContacto),1,archBin);
                    }
                    fclose(archBin);
                }
                pantallaActual=MENU;
                

            }
            
        }
        DrawRectangleRounded(btnEliminar,0.3,10,colorEleminar);
        DrawTextEx(fuente,"Eliminar",(Vector2){688,602},20,2,BLACK);

        }
        EndDrawing();
    }
    UnloadFont(fuente);
   


    //Liberar memoria
    for(int i=0; i<totalContactos;i++){
        NodoLlamada* aux=contactos[i].historial;
        while(aux!=NULL){
            NodoLlamada* borrar=aux;
            aux=aux->sig;
            delete borrar;
        }
    }
    CloseWindow();
    return 0;
}
NodoLlamada *generarHistorialAletorio(int cant){
    NodoLlamada* principal=NULL;
    for (int i=0; i<cant;i++){
        NodoLlamada* nueva=new NodoLlamada;
        const char* meses[]={"Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","Octubre",
                                "Noviembre","Diciembre"};
        int numMes=rand()%12;
        strcpy(nueva->fecha.mes,meses[numMes]);
        int diasMes;
        if (numMes==1){
            diasMes=28;
        }else if(numMes==3||numMes==5||numMes==8||numMes==10){
            diasMes=30;
        }else{
            diasMes=31;
        }
        //STRUCT FECHA
        nueva->fecha.dia=1+rand()%diasMes;
        nueva->fecha.anio=2025+rand()%2;
        nueva->hora.hora=rand()%24;
        nueva->hora.min=rand()%60;
        //STRUCT HORA
        nueva->duracion.hora=rand()%14;
        nueva->duracion.min=rand()%60;
        nueva->duracion.seg=rand()%60;
        //Crea un random para el tipo de llamada
        nueva->tipo=(TipoLlamada)(rand()%3);
        nueva->sig=principal;
        principal=nueva;
       std:: ofstream archivo("historial.txt", std::ios::app);
       char tipoTexto[20];
       if (nueva->tipo==ENTRADA)
       {
        strcpy(tipoTexto,"Entrante");
       }else if(nueva->tipo==SALIENTE){
        strcpy(tipoTexto,"Saliente");
       }else{
        strcpy(tipoTexto,"Perdida");
       }
       archivo<<tipoTexto<<" "<<nueva->fecha.dia<<"/"<<nueva->fecha.mes<<"/"<<nueva->fecha.anio<<" | "<<nueva->hora.hora<<":"<<nueva->hora.min<<" | "<<nueva->duracion.hora<< ":"<< nueva->duracion.min<< ":"<<nueva->duracion.seg<<"\n";
       archivo.close();
    }
    return principal;
}

int buscarContactoPorID(Contacto contactos[],int totalContactos,char busqueda[]){
    for (int i = 0; i < totalContactos; i++)
    {
        char idTexto[10];
        sprintf(idTexto,"%d",contactos[i].id);
        if (strcmp(idTexto,busqueda)==0)
        {
            return i;
        }
    }
    return -1;
}