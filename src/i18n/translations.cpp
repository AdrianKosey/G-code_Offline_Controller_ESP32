#include "translations.h"
#include "../app/app_settings_manager.h"


static const char* SPANISH[(int)StringId::StringId_Count] = {
    "Abrir: ", "Esto reinicia el controlador.", "Cargando archivo...", "Reiniciando Dispositivo...", 
    "Si", "No",

    "Inicio", "Archivos", "Manual", "Utilidad", "Ajustes", // Max 8 for sidebar

    "X+", "X-", "Y+", "Y-", "Z+", "Z-",
    "Paso (mm)", "HOME", "SET X0", "SET Y0", "SET Z0", "PROBAR Z",

    "CONTROLADORA CNC", "En Curso:", "Sin Archivo", "CORRIENDO", "PAUSADO", "TERMINADO", "ERROR", "LISTO",
    "Velocidad", "Potencia",
    "X", "Y", "Z",
    "Previsualizacion Desactivada.",

    "Archivos", "No se detecta la tarjeta SD",
    "Archivo Invalido",
    "Trabajo Interrumpido:\n", "\nLinea ", " de ", ". Reanudar?", "Reanudando trabajo...",

    "Tipo de herramienta","Husillo", "Laser", "Potencia", "ON", "OFF", "CW", "CCW",

    "Acerca del dispositivo", "Wi-Fi", "Maquina", "Ajustes del Controlador",
    "Firmware", "Proyecto", "G-code Offline Controller",
    "Buscar redes", "Olvidar red", "No conectado", " (Protegida)",
    "Conectando...", "Conectando a", "Conectado", "Contraseña para: ", "Modo punto de acceso", "Buscando redes...",
    "Velocidad de Jog", "Velocidad de Framing", "Previsualizacion G-code", "Recuperar trabajos CNC", "Idioma",
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

    "Cancelar", "OK", "Cargar archivo?", "Cargando...", "Framing no Disponible."
};

static const char* ENGLISH[(int)StringId::StringId_Count] = {
    "Open: ", "This resets the controller.", "Loading file...", "Restarting Device...",
    "Yes", "No",

    "Home", "Files", "Jog", "Tools", "Settings",

    "X+", "X-", "Y+", "Y-", "Z+", "Z-",
    "Step (mm)", "HOME", "SET X0", "SET Y0", "SET Z0", "PROBE Z",

    "CNC CONTROLLER", "In Progress:", "No File.", "RUNNING", "PAUSED", "COMPLETED", "ERROR", "READY",
    "Feed", "Power",
    "X", "Y", "Z",
    "Preview Disabled",

    "Files", "SD card not detected",
    "Invalid File",
    "Work Interrupted:\n", "\nLine ", " of ", ". Resume?", "Resuming work...",

    "Tool type","Spindle", "Laser", "Power", "ON", "OFF", "CW", "CCW",

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

    "Cancel", "OK", "Load file?", "Loading...", "Framing Not Available"
};

const char* tr(StringId id)
{
    int index = (int)id;

    if (g_appSettings && g_appSettings->getLanguage() == AppLanguage::English)
        return ENGLISH[index];

    return SPANISH[index];
}

static_assert(sizeof(SPANISH)/sizeof(SPANISH[0]) == (int)StringId::StringId_Count, "SPANISH array size mismatch");
static_assert(sizeof(ENGLISH)/sizeof(ENGLISH[0]) == (int)StringId::StringId_Count, "ENGLISH array size mismatch");
