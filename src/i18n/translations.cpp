#include "translations.h"
#include "../app/app_settings_manager.h"


static const char* SPANISH[(int)StringId::StringId_Count] = {
    "Abrir: ", "Esto reinicia el controlador.", "Cargando archivo...", "Reiniciando Dispositivo...", 
    "Si", "No", "Consola G-code", "Escribir Comando",

    "Inicio", "Archivos", "Manual", "Utilidad", "Ajustes", // Max 8 for sidebar

    "X+", "X-", "Y+", "Y-", "Z+", "Z-",
    "Paso (mm)", "HOME", "SET X0", "SET Y0", "SET Z0", "PROBAR Z", "$X",

    "CONTROLADORA CNC", "En Curso:", "Sin Archivo", "CORRIENDO", "PAUSADO", "TERMINADO", "ERROR", "LISTO",
    "Velocidad", "Potencia",
    "X", "Y", "Z",
    "Previsualizacion Desactivada.",
    "Trabajo completado","Tiempo: ", "Lineas: ",

    "Archivos", "No se detecta la tarjeta SD",
    "Archivo Invalido",
    "================================== \n ¡ADVERTENCIA! Trabajo Interrumpido \n ================================== \n", 
    "\nEn la linea ", " de ", 
    ". \n ================================== \n COMO FUNCIONA: \n 1- Se realizara el protocolo HOME. \n 2- Se posicionara la herramienta en la ultima ubicación registrada. \n 3- Iniciara nuevamente el trabajo desde la ultima linea de codigo leida. \n ================================== \n¿Desea reanudar?  ", 
    "Reanudando trabajo...",

    "Tipo de herramienta","Husillo", "Laser", "Potencia Referencia", "ON", "OFF", "CW", "CCW",
    "+ Nueva herramienta", "Eliminar herramienta", "Usar herramienta", "Diametro", "Uso", "Uso (Horas)", "On/Off", "Herramienta ", "¿Desea eliminar esta herramienta?",

    "Acerca del dispositivo", "Wi-Fi", "Maquina", "Ajustes del Controlador",
    "Firmware", "Proyecto", "G-code Offline Controller",
    "Buscar redes", "Olvidar red", "No conectado", " (Protegida)",
    "Conectando...", "Conectando a", "Conectado", "Contraseña para: ", "Modo punto de acceso", "Buscando redes...",
    "Velocidad de Jog", "Velocidad de Framing", "Previsualizacion G-code", "Recuperar Trabajos", "Idioma",
    "Velocidad de Jog (mm/min)", "Velocidad de Framing (mm/min)",
    "Altura segura Z", "Calcular Framing", "Suspender pantalla", "Minutos de Suspension", "Sonido al Presionar",
    "Almacenamiento SD", "Controlador", "Version del Controlador", "Version CNC", "Temperatura Controlador",
    "Tiempo encendido", "RAM Usada / Total", "Frecuencia CPU",
    "Puerto serial", "Senal Wi-Fi", "Direccion IP", "Direccion MAC",

    "Posicion maquina (MPos)", "Posicion de trabajo (WPos)",
    "Duracion pulso step", "Retardo desactivar steppers", "Invertir puerto step", "Invertir puerto direccion",
    "Invertir habilitar steppers", "Invertir limit switches", "Invertir probe", "Opciones reporte estado",
    "Desviacion de union", "Tolerancia de arco", "Reporte en pulgadas", "Limites de software",
    "Limites de hardware", "Ciclo de homing", "Direccion de homing", "Feed de homing",
    "Feed de busqueda homing", "Debounce homing", "Retraccion homing", "Velocidad max spindle",
    "Velocidad min spindle", "Modo laser", "Pasos/mm eje X", "Pasos/mm eje Y",
    "Pasos/mm eje Z", "Velocidad max X", "Velocidad max Y", "Velocidad max Z",
    "Aceleracion X", "Aceleracion Y", "Aceleracion Z", "Recorrido max X",
    "Recorrido max Y", "Recorrido max Z", 
    "us", "ms", "mascara", "mm", "mm/min", "RPM", "pasos/mm", "mm/s2",

    "Cancelar", "Confirmar", "Cargar archivo?", "Cargando...", "Framing no Disponible."
};

static const char* ENGLISH[(int)StringId::StringId_Count] = {
    "Open: ", "This resets the controller.", "Loading file...", "Restarting Device...",
    "Yes", "No", "G-code Console", "Write Command",

    "Home", "Files", "Jog", "Tools", "Settings",

    "X+", "X-", "Y+", "Y-", "Z+", "Z-",
    "Step (mm)", "HOME", "SET X0", "SET Y0", "SET Z0", "PROBE Z", "$X",

    "CNC CONTROLLER", "In Progress:", "No File.", "RUNNING", "PAUSED", "COMPLETED", "ERROR", "READY",
    "Feed", "Power",
    "X", "Y", "Z",
    "Preview Disabled",
    "Job completed", "Time: ", "Lines: ",

    "Files", "SD card not detected",
    "Invalid File",
    "================================== \n WARNING! Work Interrupted \n ================================== \n ", 
    "\n On line ", " of ", 
    ". \n ================================== \n HOW IT WORKS: \n 1- The HOME protocol will be executed. \n2- The tool will be positioned at the last registered location. \n3- The job will restart from the last line of code read. \n ================================== \n Do you want to resume?", 
    "Resuming work...",

    "Tool type","Spindle", "Laser", "Power Reference", "ON", "OFF", "CW", "CCW",
    "+ New Tool", "Delete Tool", "Use Tool", "Diameter", "Usage", "Usage (Hours)", "On/Off", "Tool ", "Do you want to delete this tool?",
    
    "About device", "Wi-Fi", "Machine", "Controller Settings",
    "Firmware", "Project", "G-code Offline Controller",
    "Scan networks", "Forget network", "Not connected", " (Protected)",
    "Connecting...", "Connecting to", "Connected", "Password for: ", "Access point mode", "Scanning networks...",
    "Jog speed", "Framing speed", "G-code preview", "CNC Job Recovery", "Language",
    "Jogging Speed (mm/min)", "Framing Speed (mm/min)",
    "Safe Z height", "Calculate Framing", "Screen sleep", "Sleep timeout", "Sound when Pressed",
    "SD Storage", "Controller", "Controller Version", "CNC Version", "Controller Temperature",
    "Power-On Time", "RAM Used / Total", "CPU Frequency",
    "Serial Port", "Wi-Fi Signal", "IP Address", "MAC Address",

    "Machine position (MPos)", "Work position (WPos)",
    "Step pulse time", "Step idle delay", "Step port invert", "Direction port invert",
    "Step enable invert", "Limit pins invert", "Probe pin invert", "Status report options",
    "Junction deviation", "Arc tolerance", "Report inches", "Soft limits",
    "Hard limits", "Homing cycle", "Homing direction invert", "Homing locate feed",
    "Homing search feed", "Homing debounce", "Homing pull-off", "Max spindle speed",
    "Min spindle speed", "Laser mode", "X-axis steps/mm", "Y-axis steps/mm",
    "Z-axis steps/mm", "X-axis max rate", "Y-axis max rate", "Z-axis max rate",
    "X-axis acceleration", "Y-axis acceleration", "Z-axis acceleration", "X-axis max travel",
    "Y-axis max travel", "Z-axis max travel", 
    "us", "ms", "mask", "mm", "mm/min", "RPM", "steps/mm", "mm/s2",

    "Cancel", "Confirm", "Load file?", "Loading...", "Framing Not Available"
};

static const char* ITALIAN[(int)StringId::StringId_Count] = {
    "Apri: ", "Questo ripristina il controller.", "Caricamento file...", "Riavvio dispositivo...",
    "Si", "No", "Console G-code", "Scrivi Comando",

    "Home", "File", "Move", "Strumenti", "Impostazioni",

    "X+", "X-", "Y+", "Y-", "Z+", "Z-",
    "Passo (mm)", "HOME", "AZZERA X", "AZZERA Y", "AZZERA Z", "SONDA Z", "$X",

    "CONTROLLER CNC", "In corso:", "Nessun File.", "IN ESECUZIONE", "IN PAUSA", "COMPLETATO", "ERRORE", "PRONTO",
    "Avanzamento", "Potenza",
    "X", "Y", "Z",
    "Anteprima disabilitata",
    "Lavoro completato", "Tempo: ", "Linee: ",

    "File", "Scheda SD non rilevata",
    "File non valido",
    "================================== \n ATTENZIONE! Lavoro Interrotto \n ================================== \n ", 
    "\n Alla linea ", " di ", 
    ". \n ================================== \n COME FUNZIONA: \n 1- Verra eseguito il protocollo HOME. \n2- L'utensile si posizionera all'ultima posizione registrata. \n3- Il lavoro ripartira dall'ultima linea di codice letta. \n ================================== \n Vuoi riprendere?", 
    "Ripresa del lavoro...",

    "Tipo utensile", "Mandrino", "Laser", "Riferimento Potenza", "ON", "OFF", "CW", "CCW",
    "+ Nuovo Utensile", "Elimina Utensile", "Usa Utensile", "Diametro", "Uso", "Uso (Ore)", "On/Off", "Utensile ", "Vuoi eliminare questo utensile?",
    
    "Info dispositivo", "Wi-Fi", "Macchina", "Impostazioni Controller",
    "Firmware", "Progetto", "Controller Offline G-code", "Scansione reti", "Dimentica rete", "Non connesso", " (Protetta)",
    "Connessione...", "Connessione a", "Connesso", "Password per: ", "Modalita Punto d'Accesso", "Scansione reti...",
    "Velocita Move", "Velocita Framing", "Anteprima G-code", "Ripristino Lavoro CNC", "Lingua",
    "Velocita Jog (mm/min)", "Velocita Framing (mm/min)",
    "Altezza Z Sicura", "Calcola Framing", "Sospensione Schermo", "Timeout Sospensione", "Suono Pressione",
    "Memoria SD", "Controller", "Versione Controller", "Versione CNC", "Temperatura Controller",
    "Tempo di Accensione", "RAM Usata / Totale", "Frequenza CPU",
    "Porta Seriale", "Segnale Wi-Fi", "Indirizzo IP", "Indirizzo MAC",

    "Posizione Macchina (MPos)", "Posizione Lavoro (WPos)",
    "Tempo impulso step", "Ritardo step idle", "Inverti porta step", "Inverti porta direzione",
    "Inverti abilitazione step", "Inverti pin limite", "Inverti pin sonda", "Opzioni report stato",
    "Deviazione giunzione", "Tolleranza arco", "Report in pollici", "Limiti Software",
    "Limiti Hardware", "Ciclo Homing", "Inverti direzione Homing", "Avanzamento localizzazione Homing",
    "Avanzamento ricerca Homing", "Debounce Homing", "Pull-off Homing", "Velocita max mandrino",
    "Velocita min mandrino", "Modalita Laser", "X-axis step/mm", "Y-axis step/mm",
    "Z-axis step/mm", "Velocita max asse X", "Velocita max asse Y", "Velocita max asse Z",
    "Accelerazione asse X", "Accelerazione asse Y", "Accelerazione asse Z", "Corsa max asse X",
    "Corsa max asse Y", "Corsa max asse Z", 
    "us", "ms", "maschera", "mm", "mm/min", "RPM", "step/mm", "mm/s2",

    "Annulla", "Conferma", "Caricare file?", "Caricamento...", "Framing non disponibile"
};

static const char* GERMAN[(int)StringId::StringId_Count] = {
    "Oeffnen: ", "Dies setzt die Steuerung zurueck.", "Datei laden...", "Geraet neustarten...",
    "Ja", "Nein", "G-code Konsole", "Befehl schreiben",

    "Home", "Dateien", "Manuell", "Werkzeuge", "Einstellu",

    "X+", "X-", "Y+", "Y-", "Z+", "Z-",
    "Schritt (mm)", "HOME", "SET X0", "SET Y0", "SET Z0", "MESSEN Z", "$X",

    "CNC STEUERUNG", "In Bearbeitung:", "Keine Datei.", "LAEUFT", "PAUSIERT", "ABGESCHLOSSEN", "FEHLER", "BEREIT", "Vorschub", "Leistung",
    "X", "Y", "Z",
    "Vorschau deaktiviert",
    "Job abgeschlossen", "Zeit: ", "Zeilen: ",

    "Dateien", "SD-Karte nicht erkannt",
    "Ungueltige Datei",
    "================================== \n WARNUNG! Arbeit unterbrochen \n ================================== \n ", 
    "\n In Zeile ", " von ", 
    ". \n ================================== \n SO FUNKTIONIERT ES: \n 1- Das HOME-Protokoll wird ausgefuehrt. \n2- Das Werkzeug wird an der zuletzt registrierten Position platziert. \n3- Der Job wird ab der zuletzt gelesenen Codezeile neu gestartet. \n ================================== \n Moechten Sie fortfahren?", 
    "Arbeit wird fortgesetzt...",

    "Werkzeugtyp", "Spindel", "Laser", "Leistungsreferenz", "EIN", "AUS", "CW", "CCW",
    "+ Neues Werkzeug", "Werkzeug loeschen", "Werkzeug nutzen", "Durchmesser", "Nutzung", "Nutzung (Std)", "Ein/Aus", "Werkzeug ", "Moechten Sie dieses Werkzeug loeschen?",
    
    "Ueber das Geraet", "WLAN", "Maschine", "Steuerung-Einstellungen",
    "Firmware", "Projekt", "G-Code Offline Controller",
    "Netzwerke suchen", "Netzwerk verwerfen", "Nicht verbunden", " (Geschuetzt)",
    "Verbinden...", "Verbinden mit", "Verbunden", "Passwort fuer: ", "Access-Point-Modus", "Netzwerke werden gesucht...",
    "Verfahrgeschwindigkeit", "Framing-Geschwindigkeit", "G-Code Vorschau", "CNC Job-Wiederherstellung", "Sprache",
    "Verfahr-Geschwindigkeit (mm/min)", "Framing-Geschwindigkeit (mm/min)",
    "Sichere Z-Hoehe", "Framing berechnen", "Bildschirm-Ruhezustand", "Ruhezustand-Time-out", "Tastenton",
    "SD-Speicher", "Steuerung", "Steuerungsversion", "CNC-Version", "Steuerungstemperatur",
    "Betriebszeit", "RAM Genutzt / Gesamt", "CPU-Frequenz",
    "Serielle Schnittstelle", "WLAN-Signal", "IP-Adresse", "MAC-Adresse",

    "Maschinenposition (MPos)", "Arbeitsposition (WPos)",
    "Schrittimpulszeit", "Schrittleerlaufverzoegerung", "Schrittport invertieren", "Richtungsport invertieren",
    "Schrittfreigabe invertieren", "Endschalterpins invertieren", "Tasterpin invertieren", "Statusbericht-Optionen",
    "Knotenabweichung", "Bogentoleranz", "Bericht in Zoll", "Weiche Grenzen",
    "Harte Grenzen", "Referenzfahrt-Zyklus", "Referenzfahrt-Richtung invertieren", "Referenzfahrt-Suchvorschub",
    "Referenzfahrt-Einvorschub", "Referenzfahrt-Entprellung", "Referenzfahrt-Ruckzug", "Max. Spindeldrehzahl",
    "Min. Spindeldrehzahl", "Lasermodus", "X-Achse Schritte/mm", "Y-Achse Schritte/mm",
    "Z-Achse Schritte/mm", "X-Achse max. Rate", "Y-Achse max. Rate", "Z-Achse max. Rate",
    "X-Achse Beschleunigung", "Y-Achse Beschleunigung", "Z-Achse Beschleunigung", "X-Achse max. Weg",
    "Y-Achse max. Weg", "Z-Achse max. Weg", 
    "us", "ms", "Maske", "mm", "mm/min", "RPM", "Schritte/mm", "mm/s2",

    "Abbrechen", "Bestaetigen", "Datei laden?", "Laden...", "Framing nicht verfuegbar"
};



const char* tr(StringId id)
{
    int index = (int)id;

    AppLanguage currentLang = g_appSettings ? g_appSettings->getLanguage() : AppLanguage::English;

    switch (currentLang)
    {
        case AppLanguage::English:
            return ENGLISH[index];
        case AppLanguage::Italian:
            return ITALIAN[index];
        case AppLanguage::German:
            return GERMAN[index];
        case AppLanguage::Spanish:
        default:
            return SPANISH[index];
    }
}

// Aserciones de seguridad al final del archivo
static_assert(sizeof(SPANISH)/sizeof(SPANISH[0]) == (int)StringId::StringId_Count, "SPANISH size mismatch");
static_assert(sizeof(ENGLISH)/sizeof(ENGLISH[0]) == (int)StringId::StringId_Count, "ENGLISH size mismatch");
static_assert(sizeof(ITALIAN)/sizeof(ITALIAN[0]) == (int)StringId::StringId_Count, "ITALIAN size mismatch");
static_assert(sizeof(GERMAN)/sizeof(GERMAN[0]) == (int)StringId::StringId_Count, "GERMAN size mismatch");
