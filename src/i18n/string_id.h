#pragma once

enum class StringId
{
    // App
    App_Open_File, App_Modal_Language, App_Loading_File, App_Reset, 
    App_Yes, App_No,

    // Sidebar
    Sidebar_Home, Sidebar_Files, Sidebar_Jog, Sidebar_Tools, Sidebar_Settings,

    // Jog
    Jog_XUp, Jog_XDown, Jog_YUp, Jog_YDown, Jog_ZUp, Jog_ZDown,
    Jog_StepLabel, Jog_Home, Jog_SetX0, Jog_SetY0, Jog_SetZ0, Jog_ProbeZ,

    // Home
    Home_Title, Home_InProgress, Home_NoFile, Home_Running, Home_Paused, Home_Completed, Home_Error, Home_Ready,
    Home_Speed, Home_Power,
    Coords_X, Coords_Y, Coords_Z,

    // Files
    Files_Title, Files_NoSdCard,
    Files_Invalid,

    // Tools
    Tool_Type, Tools_Spindle, Tools_Laser, Tools_Power, Tools_On, Tools_Off, Tools_CW, Tools_CCW,

    // Settings - menu
    Settings_About, Settings_Wifi, Settings_Machine, Settings_Control,
    Settings_Firmware, Settings_Project, Control_Name,
    Settings_ScanNetworks, Settings_ForgetNetwork, Settings_NotConnected,
    Settings_Connecting, Settings_Connecting_To, Settings_Connected, Settings_Password, Settings_AccessPointMode, Settings_Scanning,
    Settings_JogSpeed, Settings_FramingSpeed, Settings_GcodePreview, Settings_JobRecovery, Settings_Language,
    Settings_Modal_JogSpeed, Settings_Modal_FramingSpeed,

    // GRBL Settings & Status
    StatusMPos, StatusWPos,
    Setting0_StepPulse, Setting1_StepDelay, Setting2_StepInvert, Setting3_DirInvert,
    Setting4_EnableInvert, Setting5_LimitInvert, Setting6_ProbeInvert, Setting10_StatusReport,
    Setting11_JunctionDev, Setting12_ArcTolerance, Setting13_ReportInches, Setting20_SoftLimits,
    Setting21_HardLimits, Setting22_HomingCycle, Setting23_HomingDir, Setting24_HomingFeed,
    Setting25_HomingSeek, Setting26_HomingDebounce, Setting27_HomingPulloff, Setting30_MaxSpindle,
    Setting31_MinSpindle, Setting32_LaserMode, Setting100_StepsX, Setting101_StepsY,
    Setting102_StepsZ, Setting110_MaxRateX, Setting111_MaxRateY, Setting112_MaxRateZ,
    Setting120_AccelX, Setting121_AccelY, Setting122_AccelZ, Setting130_TravelX,
    Setting131_TravelY, Setting132_TravelZ, 
    GRBL_Value_Type1, GRBL_Value_Type2, GRBL_Value_Type3, GRBL_Value_Type4, GRBL_Value_Type5, GRBL_Value_Type6, GRBL_Value_Type7, GRBL_Value_Type8,

    // Modales
    Modal_Cancel, Modal_Ok, Modal_LoadFile, Modal_Loading,

    StringId_Count
};